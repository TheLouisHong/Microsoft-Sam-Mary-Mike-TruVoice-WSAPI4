/*******************************************************
Pizza.cpp - Code to demonstrate writing a pizza delivery server.


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/


// get the phone number
#define  GETPHONENUM

#include <windows.h>
//DEFINE_GUID(IID_IDirectSound, 0x279AFA83, 0x4981, 0x11CE, 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60);
#include <mmsystem.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <dsound.h>
#include <spchwrap.h>

#include <stdio.h>
#include "pizza.h"



CCallPizza::CCallPizza()
{
    m_pITelInfo     = NULL;
    m_pTCPhoneNum   = NULL;
    m_pTCGrammar    = NULL;
    m_pTCYesNo      = NULL;
    m_pTCRecord     = NULL;
    m_pQueue        = NULL;

    m_pTelNum       = NULL;
    m_dwTelNumSize  = 0;
    m_fKnown        = FALSE;
}

CCallPizza::~CCallPizza()
{
    // left intentionally blank
}


HRESULT CCallPizza::Shutdown()
{
    if (m_pITelInfo) {
        // allow the caller to say operator and to hang up
        m_pITelInfo->DWORDSet(TELDWORD_EnableOperator, FALSE);
        m_pITelInfo->DWORDSet(TELDWORD_EnableAskHangUp, FALSE);
    }

    // delete the objects
    if (m_pTCPhoneNum)
        delete m_pTCPhoneNum;
    if (m_pTCGrammar)
        delete m_pTCGrammar;
    if (m_pTCYesNo)
        delete m_pTCYesNo;
    if (m_pTCRecord)
        delete m_pTCRecord;
    if (m_pQueue)
        delete m_pQueue;

    return NOERROR;
}

HRESULT CCallPizza::Init(PITELINFO pITelInfo)
{
    HRESULT hRes;

    m_pITelInfo = pITelInfo;

    m_pTCPhoneNum = new CTelControl;
    m_pTCGrammar = new CTelControl;
    m_pTCYesNo = new CTelControl;
    m_pTCRecord = new CTelControl;

    if (!m_pTCPhoneNum || !m_pTCGrammar || !m_pTCYesNo ||
        !m_pTCRecord)
        return E_OUTOFMEMORY;

    // init the objects
    hRes = m_pTCPhoneNum->Init (CLSID_PhoneNumControl, m_pITelInfo);
    if (hRes) return hRes;
    hRes = m_pTCGrammar->Init (CLSID_GrammarControl, m_pITelInfo);
    if (hRes) return hRes;
    hRes = m_pTCYesNo->Init (CLSID_YesNoControl, m_pITelInfo);
    if (hRes) return hRes;
    hRes = m_pTCRecord->Init (CLSID_RecordControl, m_pITelInfo);
    if (hRes) return hRes;
   
    // get the queue from the telinfo
    LPUNKNOWN  pUnk;
    hRes = m_pITelInfo->ObjectGet(TELOBJ_TTSQUEUE, &pUnk);
    if (hRes) return hRes;
    m_pQueue = new CSTTTSQueue;
    if (!m_pQueue)
       return E_OUTOFMEMORY;
    m_pQueue->Init (pUnk);
    pUnk->Release();

    // allow the caller to say operator and to hang up
    m_pITelInfo->DWORDSet(TELDWORD_EnableOperator, TRUE);
    m_pITelInfo->DWORDSet(TELDWORD_EnableAskHangUp, TRUE);

    return NOERROR;
}


/*****************************************************************
VerifyHangUp - Verify with the user that they really want to hang
   up.

inputs
   none
returns
   BOOL - TRUE if really want to hang up, FALSE if not
*/
BOOL CCallPizza::VerifyHangUp (void)
{
   WCHAR szHangUp[] =
      L"[Prompts]\n"
      L"Main=Are you sure you want to leave? Please say yes or no.\n"
      L"Main.2=Are you sure you want to leave?\n"
      L"Where=You just said you wanted to hang up.\n"
      ;

   DWORD dwRes;
   m_pTCYesNo->GoFromMemory (szHangUp, sizeof(szHangUp), &dwRes);
   switch (dwRes) {
   case 2:  // no
      return FALSE;
   default: // yes or other responses
      m_pQueue->Speak (L"Thank you for calling ACME Pizza. Goodbye.", NULL, 1);
      return TRUE;
   }

}

/*****************************************************************
GetPhoneNumber - ASk the user his/her phone number. If it's already
   in the database then verify the name, else get the information.
   For purposes of the demo we assume that numbers ending in an even
   digit are in the database.

inputs
   none
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or TCR_ASKBACK to go back
*/
DWORD CCallPizza::GetPhoneNumber (void)
{
   // ask phone number
   WCHAR szAskNum[] =
      L"[Prompts]\n"
      L"Main=Please tell me your phone number.\n"
      L"Where=You need to tell me your phone number so we can deliver your pizza.\n"
      L"[Settings]\n"
      L"CanGoBack=1\n"
      ;

asknum:
   DWORD dwRes;
   if (m_pTelNum) CoTaskMemFree (m_pTelNum);
   m_pTelNum = NULL;
   m_pTCPhoneNum->GoFromMemory (szAskNum, sizeof(szAskNum), &dwRes, (void**) &m_pTelNum, &m_dwTelNumSize);
   switch (dwRes) {
   case TCR_ABORT:
   case TCR_NORESPONSE:
   case TCR_ASKOPERATOR:
      return TCR_ABORT;
   case TCR_ASKHANGUP:
      if (VerifyHangUp())
         return TCR_ABORT;
      else
         goto asknum;
   case TCR_ASKBACK:
      return TCR_ASKBACK;
   }

   // look in the database and figure out if we know the address
   // in our case, look at the last digit. If it's odd then pretend we
   // know their address
   m_fKnown = FALSE;
   DWORD dwLen;
   if (!m_pTelNum || !(dwLen = wcslen (m_pTelNum))) {
      // didnt' get number correctly
      m_pQueue->Speak (L"I'm sorry. I didn't get that phone number correctly.", NULL, 1);
      goto asknum;
   }
   switch (m_pTelNum[dwLen-1]) {
   case L'1':
   case L'3':
   case L'5':
   case L'7':
   case L'9':
      m_fKnown = TRUE;
   }

askknown:
   if (m_fKnown) {
      // verify the right person
      WCHAR gszPerson[] =
         L"[Prompts]\n"
         L"Main=You are already in our database. Say yes to skip address verification.\n"
         L"Main.2=Do you want to skip address verification?\n"
         L"Where=You are verifying that we have the correct name in our database.\n"
         L"[Settings]\n"
         L"CanGoBack=1\n"
         ;
      m_pTCYesNo->GoFromMemory (gszPerson, sizeof(gszPerson), &dwRes);
      switch (dwRes) {
      case 1:  // yes
         // don't do anything
         break;
      case 2:
         m_fKnown = FALSE;
         break; // no
      case TCR_ABORT:
      case TCR_NORESPONSE:
      case TCR_ASKOPERATOR:
         return TCR_ABORT;
      case TCR_ASKHANGUP:
         if (VerifyHangUp())
            return TCR_ABORT;
         else
            goto askknown;
      case TCR_ASKBACK:
         goto asknum;
      }
   }

   // if we know the info then just return now
   if (m_fKnown)
      return 0;

   // if we're not known then ask for the person's address. Since speech
   // recognition is not good enough for arbitrary addressed, this will
   // just do a recording of the name, and then the address, and play them
   // back for verificaiton
askname:

   PVOID pWAV;
   DWORD dwWAVSize;

   //ask the name
   WCHAR szName[] =
      L"[Prompts]\n"
      L"Main=What is your name?\n"
      L"Where=We are asking you your name.\n"
      ;
   m_pTCRecord->GoFromMemory (szName, sizeof(szName), &dwRes, &pWAV, &dwWAVSize);
   if (!pWAV)
      goto askname; // didn't hear it

   // stick this in the queue object so we can speak it
   m_pQueue->WaveAddFromMemory (L"<PersonsName>", pWAV, dwWAVSize);
   CoTaskMemFree (pWAV);

askadd:
   // ask the address
   WCHAR szAdd[] =
      L"[Prompts]\n"
      L"Main=What is your address, including the street, city, and zip code.\n"
      L"Main.2=Please tell me your address now.\n"
      L"Where=We are asking you your street address.\n"
      L"[Settings]\n"
      L"BetweenUtt=2000\n" // 2 seconds
      L"InitialBuf=10000\n"   // 10 seconds
      L"ReallocBuf=5000\n" // make 5 seconds longer each time
      L"MaxBuf=60000\n" // max 60 seconds
      ;
   m_pTCRecord->GoFromMemory (szAdd, sizeof(szAdd), &dwRes, &pWAV, &dwWAVSize);
   if (!pWAV)
      goto askadd; // didn't hear it

   // stick this in the queue object so we can speak it
   m_pQueue->WaveAddFromMemory (L"<PersonsAddress>", pWAV, dwWAVSize);
   CoTaskMemFree (pWAV);

   // verify this information is correct
   WCHAR szVerifyRecording[] =
      L"[Prompts]\n"
      L"Main=Do I have your name and address correct?\n"
      L"Where=You are verifying that we have the correct name and address.\n"
      L"[Settings]\n"
      L"CanGoBack=1\n"
      ;
   m_pQueue->Speak (L"I have your name and address as:", NULL, 1);

   m_pQueue->Speak (L"<PersonsName>", NULL, 1);
   m_pQueue->Speak (L"<PersonsAddress>", NULL, 1);
   m_pTCYesNo->GoFromMemory (szVerifyRecording, sizeof(szVerifyRecording), &dwRes);
   switch (dwRes) {
   case 1:  // yes
      // don't do anything
      break;
   case 2:
      goto askname;
   case TCR_ABORT:
   case TCR_NORESPONSE:
   case TCR_ASKOPERATOR:
      return TCR_ABORT;
   case TCR_ASKHANGUP:
      if (VerifyHangUp())
         return TCR_ABORT;
      else
         goto askadd;
   case TCR_ASKBACK:
      goto asknum;
   }

   // if we got here then we have all the info on the user
   return 0;
}

/*****************************************************************
GetPizzaInfo - Asks the user for the pizza type and for a list
   of toppings.

inputs
   none
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or TCR_ASKBACK to go back
*/
DWORD CCallPizza::GetPizzaInfo (void)
{
   DWORD dwRes;
   PVOID pMem;
   DWORD dwMemSize;
   WCHAR LastTopping[256];

getsize:
   WCHAR szSize[] =
      L"[Prompts]\n"
      L"Main=What size pizza do you want, small, medium, or large?\n"
      L"Main.2=What size pizza do you want?\n"	
      L"Where=You are specifying the pizza size.\n"
      L"Help=Please say small, medium, or large. A small pizza is 8 inchess. A medium is 12, and a large is 16.\n"
      L"[<MyGrammar>]\n"
      L"<MyGrammar>=[opt] (JunkPizzaBegin) <PizzaSize> [opt] (JunkPizzaEnd)\n"
      L"[(JunkPizzaBegin)]\n"
      L"=I want a\n"
      L"=give me a\n"
      L"=deliver a\n"
      L"=send me a\n"
      L"[(JunkPizzaEnd)]\n"
      L"=pizza [opt] pie [opt] please\n"
      L"[<PizzaSize>]\n"
      L"<PizzaSize>=small \"small\"\n"
      L"<PizzaSize>=eight inch \"small\"\n"
      L"<PizzaSize>=medium \"medium\"\n"
      L"<PizzaSize>=twelve inch \"medium\"\n"
      L"<PizzaSize>=large \"large\"\n"
      L"<PizzaSize>=sixteen inch \"large\"\n"
      L"<PizzaSize>=jumbo \"large\"\n"
      ;
   m_pTCGrammar->GoFromMemory (szSize, sizeof(szSize), &dwRes, &pMem, &dwMemSize);
   switch (dwRes) {
   case TCR_ABORT:
   case TCR_NORESPONSE:
   case TCR_ASKOPERATOR:
      return TCR_ABORT;
   case TCR_ASKHANGUP:
      if (VerifyHangUp())
         return TCR_ABORT;
      else
         goto getsize;
   case TCR_ASKBACK:
      return TCR_ASKBACK;
   }
   if (!pMem)
      goto getsize;  // error of some sort

   // back up this string
   wcscpy (m_wszSize, (PCWSTR) pMem);
   CoTaskMemFree (pMem);

gettoppings:
   // get the pizza toppings

   // get the pizza toppings
   // trials have shown that having the user select one topping at a time in this manner
   // is more effective than attempting to recognize a list or more complex grammar, 
   // so just prompt the user for one choice at a time, 
   // and allow them to undo if the recognizer mishears.

   WCHAR szToppingsPrompt[]=
      L"[Prompts]\n"
      L"Main=Select a topping for your pizza.\n"
      L"Main.2=Select a topping?\n"
      L"Where=You are specifying pizza toppings.\n"
      L"Help=You can say one of the following: pepperoni, anchovies, sausages, onions, mushrooms, etc.\n"
      ;
   WCHAR szToppingsGrammarMain[]=
      L"[<MyGrammar>]\n"
      L"<MyGrammar>=<PizzaTop>\n"
      ;

   WCHAR szToppingsGrammarTop[]=
      L"[<PizzaTop>]\n"
      L"<PizzaTop>=anchovies \"anchovies\"\n"
      L"<PizzaTop>=[opt] tiny fish \"anchovies\"\n"
      L"<PizzaTop>=bacon \"bacon\"\n"
      L"<PizzaTop>=beef \"beef\"\n"
      L"<PizzaTop>=cow \"beef\"\n"
      L"<PizzaTop>=ham \"ham\"\n"
      L"<PizzaTop>=canadian bacon \"Canadian bacon\"\n"
      L"<PizzaTop>=pepperoni \"pepperoni\"\n"
      L"<PizzaTop>=pineapple \"pineapple\"\n"
      L"<PizzaTop>=pork \"pork\"\n"
      L"<PizzaTop>=[opt] italian sausage \"sausage\"\n"
      L"<PizzaTop>=[opt] black olives \"olives\"\n"
      L"<PizzaTop>=green peppers \"green peppers\"\n"
      L"<PizzaTop>=jalapenos \"jalapenos\"\n"
      L"<PizzaTop>=mushrooms \"mushrooms\"\n"
      L"<PizzaTop>=onions \"onions\"\n"
      L"<PizzaTop>=pinapple \"pinapple\"\n"
      L"<PizzaTop>=[opt] diced tomatoes \"tomatoes\"\n"
      L"<PizzaTop>=no cheese \"no cheese\"\n"
      L"<PizzaTop>=cheese \"cheese\"\n"
      L"<PizzaTop>=[opt] cheese \"cheese\"\n"
      L"<PizzaTop>=nothing [opt] extra \"cheese\"\n"
      L"<PizzaTop>=none \"cheese\"\n"
      L"<PizzaTop>=extra cheese \"extra cheese\"\n"
      L"<PizzaTop>=no sauce \"no sauce\"\n"
      L"<PizzaTop>=light sauce \"light sauce\"\n"
      L"<PizzaTop>=not too much sauce \"light sauce\"\n"
      L"<PizzaTop>=extra sauce \"extra sauce\"\n"
      ;
#define MAXTOP 8192
   WCHAR szToppings[MAXTOP];
   wcscpy(szToppings,szToppingsPrompt);
   wcscat(szToppings,szToppingsGrammarMain);
   wcscat(szToppings,szToppingsGrammarTop);
   pMem = NULL;
   m_pTCGrammar->GoFromMemory (szToppings, sizeof(szToppings), &dwRes, &pMem, &dwMemSize);
   switch (dwRes) {
   case TCR_ABORT:
   case TCR_NORESPONSE:
   case TCR_ASKOPERATOR:
      return TCR_ABORT;
   case TCR_ASKHANGUP:
      if (VerifyHangUp())
         return TCR_ABORT;
      else
         goto gettoppings;
   case TCR_ASKBACK:
      goto getsize;
   }
   if (!pMem)
      goto gettoppings;  // error of some sort

   wcscpy(m_wszToppings, L"");
   wcscpy (LastTopping, (PCWSTR) pMem); //last topping is now set..add to list after check
   CoTaskMemFree (pMem);


WCHAR szToppingsPromptV[]=
      L"[Prompts]\n"
      L"Main=I heard %s. Is this correct?\n"
      L"Main.2=had you said %s?\n"
      L"Where=Specifying if the last topping was heard correctly.\n"
      L"Help=You can say yes or no\n"
      ;
 WCHAR szToppingsPrompt2[]=
      L"[Prompts]\n"
      L"Main=Do you want any other toppings?\n"
      L"Main.2=Do you want another topping?\n"
      L"Where=You are deciding if you want more toppings.\n"
      L"Help=You can say yes or no.\n"
      ;
 WCHAR szToppingsPrompt3[]=
      L"[Prompts]\n"
      L"Main=Go Ahead\n"
      L"Where=You are specifying more toppings.\n"
      L"Help=You can pick one or more of the following: pepperoni, anchovies, sausages, onions, mushrooms, etc.  Say finished to stop adding toppings.\n"
      ;
 WCHAR szToppingsMistake[]=
      L"[Prompts]\n"
      L"Main=Oops. Say it again.\n"
      L"Where=You are specifying more toppings.\n"
      L"Help=You can  pick one or more of the following: pepperoni, anchovies, sausages, onions, mushrooms, etc.  Say finished to stop adding toppings.\n"
      ;

 WCHAR szExitMenu[]=
   L"[<exitmenu>]\n"
   L"<exitmenu>=I'm done \"finished\"\n"
   L"<exitmenu>=finished \"finished\"\n"
   L"<exitmenu>=done \"finished\"\n"
   L"<exitmenu>=thats it \"finished\"\n"
   ;


#define CHECKTOPPING 1
#define ANOTHERTOPPING 2
#define GOAHEAD 3
#define MISTAKE 4
   
   int mode = CHECKTOPPING;

moretoppings:
   switch(mode)
   {
	case CHECKTOPPING:
          swprintf (szToppings, szToppingsPromptV, LastTopping, LastTopping);
	break;
        case ANOTHERTOPPING:
	  wcscpy(szToppings,szToppingsPrompt2);
        break;
        case GOAHEAD:
	  wcscpy(szToppings,szToppingsPrompt3);
 	break;
        case MISTAKE:
	  wcscpy(szToppings,szToppingsMistake);
	break;
   }
   if((mode==CHECKTOPPING) || (mode == ANOTHERTOPPING))
   {
	m_pTCYesNo->GoFromMemory (szToppings, (wcslen(szToppings) + 1)*2, &dwRes);
	switch (dwRes) 
        {
	   case 1:  // yes
anothertop:  
	      if(mode == CHECKTOPPING)
              {
                 if(wcscmp(m_wszToppings,L"")!=0)
		   wcscat (m_wszToppings,L" and ");
                 wcscat (m_wszToppings, LastTopping);
		 mode = ANOTHERTOPPING;
	      }
	      else
	      {
		 mode = GOAHEAD;
	      }
	      
 	      goto moretoppings;
	      break;
	   case 2:  // no
	      if(mode == CHECKTOPPING)
              {
   		 mode = MISTAKE;
	      	 goto moretoppings;
              }
	      else
	      {
                 goto verify;
	      }

	   case TCR_ABORT:
	   case TCR_NORESPONSE:
	   case TCR_ASKOPERATOR:
	      return TCR_ABORT;
	   case TCR_ASKHANGUP:
	      if (VerifyHangUp())
        	 return TCR_ABORT;
	      else
        	 goto anothertop;
	   case TCR_ASKBACK:
        	 goto anothertop;
        }
   }
   else
   {

           wcscat(szToppings,szToppingsGrammarMain);

	   wcscat(szToppings,L"<MyGrammar>=<exitmenu>\n");
	   wcscat(szToppings,szToppingsGrammarTop);
	   wcscat(szToppings,szExitMenu);

	   pMem = NULL;
	   m_pTCGrammar->GoFromMemory (szToppings, sizeof(szToppings), &dwRes, &pMem, &dwMemSize);

   
	   switch (dwRes) {
	   case TCR_ABORT:
	   case TCR_NORESPONSE:
	   case TCR_ASKOPERATOR:
	      return TCR_ABORT;
	   case TCR_ASKHANGUP:
	      if (VerifyHangUp())
	         return TCR_ABORT;
	      else
	         goto moretoppings;
	   case TCR_ASKBACK:
	      goto getsize;
	   }
	   if (!pMem)
	      goto gettoppings;  // error of some sort
	
	   if(wcscmp((PCWSTR)pMem,L"finished") != 0)  //said a pizza word
	   {
		// tack on the new toppings to the topping string
		wcscpy (LastTopping,(PCWSTR) pMem);

		CoTaskMemFree (pMem);
		mode = CHECKTOPPING;
		goto moretoppings;
	   }  
	   else //finished case
	   { 
		CoTaskMemFree (pMem);   
	   }
   }
   // verify the info
verify:
   WCHAR szVerifyOrig[] =
      L"[Prompts]\n"
      L"Main=You have ordered a %s pizza with %s. The charge will be $15.23. Do you want this order?\n"
      L"Where=You are verifying the order.\n"
      L"[Settings]\n"
      L"CanGoBack=1\n"
      ;
   WCHAR szVerify[256];
   swprintf (szVerify, szVerifyOrig, m_wszSize, m_wszToppings);
   m_pTCYesNo->GoFromMemory (szVerify, (wcslen(szVerify) + 1)*2, &dwRes);
   switch (dwRes) {
   case 1:  // yes
      // don't do anything
      break;
   case 2:
      m_pQueue->Speak (L"Ok, lets try again from the top",NULL,1);
      goto getsize;
   case TCR_ABORT:
   case TCR_NORESPONSE:
   case TCR_ASKOPERATOR:
      return TCR_ABORT;
   case TCR_ASKHANGUP:
      if (VerifyHangUp())
         return TCR_ABORT;
      else
         goto verify;
   case TCR_ASKBACK:
      goto gettoppings;
   }

   return 0;
}
/*****************************************************************
DoPhoneCall - Does all the operations necessary to handle a pizza
   phone call. The phone call consists of the following parts:
      Get the phone number, and address
      Get pizza information
      Say how much it's going to cost and verify

inputs
   none
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or TCR_ASKBACK to go back
*/
HRESULT CCallPizza::DoPhoneCall (void)
{
   DWORD dwRes;


  // Speak the introduction.  Be sure to tell user about the beep; the most
   // common error is for users to start speaking before listening is enabled.
   m_pQueue->Speak (
      L"Welcome to the ACME pizza company automated ordering service. "
      L"We deliver in 20 minutes or less. Wait until I'm finished talking before answering my questions."
      , NULL, 1);

phonenum:
#ifdef GETPHONENUM
   dwRes = GetPhoneNumber();
   switch (dwRes) {
   case TCR_ABORT:
      return dwRes;
   case TCR_ASKBACK:
      goto phonenum;
   }
#endif // GETPHONENUM

   // get info about the pizza
   dwRes = GetPizzaInfo();
   switch (dwRes) {
   case TCR_ABORT:
      return dwRes;
   case TCR_ASKBACK:
      goto phonenum;
   }

   // say thank you and hang up
   m_pQueue->Speak (
      L"Thank you for ordering from ACME pizza. "
      L"Your pizza will be delivered in 20 minutes or less."
      , NULL, 1);

   // wait until we're done speaking
   m_pQueue->WaitUntilDoneSpeaking();

   return NOERROR;
}
