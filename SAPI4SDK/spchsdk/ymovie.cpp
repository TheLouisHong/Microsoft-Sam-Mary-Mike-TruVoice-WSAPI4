/*******************************************************
Movie.cpp - Code to demonstrate writing a Movie information server.


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/


#include <windows.h>
#include <mmsystem.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <dsound.h>
#include <spchwrap.h>
#include <stdio.h>
#include "Movie.h"
#include "resource.h"



CCallMovie::CCallMovie()
{
    m_pITelInfo     = NULL;
    m_pTCGrammar    = NULL;
    m_pTCYesNo      = NULL;
    m_pQueue        = NULL;

}

CCallMovie::~CCallMovie()
{
    // left intentionally blank
}


HRESULT CCallMovie::Shutdown()
{
    if (m_pITelInfo) {
        // allow the caller to say operator and to hang up
        m_pITelInfo->DWORDSet(TELDWORD_EnableOperator, FALSE);
        m_pITelInfo->DWORDSet(TELDWORD_EnableAskHangUp, FALSE);
    }

    // delete the objects
    if (m_pTCGrammar)
        delete m_pTCGrammar;
    if (m_pTCYesNo)
        delete m_pTCYesNo;
    if (m_pQueue)
        delete m_pQueue;

    return NOERROR;
}

HRESULT CCallMovie::Init(PITELINFO pITelInfo)
{
    HRESULT hRes;

    m_pITelInfo = pITelInfo;

    m_pTCGrammar = new CTelControl;
    m_pTCYesNo = new CTelControl;

    if (!m_pTCGrammar || !m_pTCYesNo)
        return E_OUTOFMEMORY;

    // init the objects
    hRes = m_pTCGrammar->Init (CLSID_GrammarControl, m_pITelInfo);
    if (hRes) return hRes;
    hRes = m_pTCYesNo->Init (CLSID_YesNoControl, m_pITelInfo);
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

    // Load in the resource containing the list of files.
    // THIS WILL NOT WORK unless you have recorded files and
    // placed them in the directory specified by Movie.txt.
    // You may have to modify movie.txt to get the recordings to work
    m_pQueue->WaveAddFromList (IDR_WAVEPROMPTS, ghInstance);

    // allow the caller to say operator and to hang up
    m_pITelInfo->DWORDSet(TELDWORD_EnableOperator, FALSE);
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
BOOL CCallMovie::VerifyHangUp (void)
{
   WCHAR szHangUp[] =
      L"[Prompts]\n"
      L"Main=Are you sure you want to leave?\n"
      L"Where=You just said you wanted to hang up.\n"
      ;

   DWORD dwRes;
   m_pTCYesNo->GoFromMemory (szHangUp, sizeof(szHangUp), &dwRes);
   switch (dwRes) {
   case 2:  // no
      return FALSE;
   default: // yes or other responses
      m_pQueue->Speak (L"Thank you for calling ACME Movie. Goodbye.", NULL, 1);
      return TRUE;
   }

}

/*****************************************************************
GetMovieInfo - Asks the user for the Movie type and for a list
   of toppings.

inputs
   none
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or TCR_ASKBACK to go back
*/
DWORD CCallMovie::GetMovieInfo (void)
{
   DWORD dwRes;

askmovie:
   WCHAR szSize[] =
      L"[Prompts]\n"
      L"Main=Speak the name of the movie you want to see, "
         L"or say, \"List Movies\" to hear currently playing movies.\n"
      L"Main.2=What movie do you want to see?\n"
      L"Help=You can speak the name of a movie or say, \"List movies\".\n"
      L"Where=You are being asked what movie you wish to see.\n"
      L"[<MyGrammar>]\n"
      L"<MyGrammar>=[opt] (JunkMovieBegin) (Movie) [opt] (JunkMovieEnd)\n"
      L"<MyGrammar>=(WCIS)\n"
      L"[(JunkMovieBegin)]\n"
      L"=I want to see\n"
      L"=I wanna see\n"
      L"=Can I see\n"
      L"=What time is\n"
      L"=[opy] When is\n"
      L"=[opt] when are you showing\n"
      L"[(JunkMovieEnd)]\n"
      L"=please\n"
      L"=today\n"
      L"=tomorrow\n"
      L"=showing\n"
      L"=playing\n"
      L"[(WCIS)]\n"
      L"100=list movies\n"
      L"100=what can I say\n"
      L"100=what movies are playing\n"
      L"100=[opt] please tell me what movies are playing\n"
      L"100=what's playing\n"
      L"[(Movie)]\n"
      L"1=Air Force One\n"
      L"2=Conspiracy Theory\n"
      L"3=Contact\n"
      L"4=George of the Jungle\n"
      L"5=Hoodlum\n"
      L"6=Masterminds\n"
      L"7=Men in Black\n"
      L"8=Mimic\n"
      L"9=A Smile Like Yours\n"
      L"10=Steel\n"
      L"11=Money talks\n"
      L"12=As good as it gets\n"
      L"13=Blues Brothers two thousand\n"
      L"14=The borrowers\n"
      L"15=Desperate Measures\n"
      L"16=Good will hunting\n"
      L"17=The replacement killers\n"
      L"18=Sphere\n"
      L"19=Spice World\n"
      L"20=Titanic\n"
      L"21=The wedding singer\n"
      ;
   m_pTCGrammar->GoFromMemory (szSize, sizeof(szSize), &dwRes);
   switch (dwRes) {
   case TCR_ABORT:
   case TCR_NORESPONSE:
   case TCR_ASKOPERATOR:
      return TCR_ABORT;
   case TCR_ASKHANGUP:
      if (VerifyHangUp())
         return TCR_ABORT;
      else
         goto askmovie;
   case TCR_ASKBACK:
      return TCR_ASKBACK;
   }

   char  cRating = 0;

   switch (dwRes) {
   case 100:   // list of movies
      m_pQueue->Speak (
         L"The following movies are playing at ACME theaters: "
         L"As good as it gets. Blues brothers 2000. The borrowers. "
         L"Desperate measures. Good will hunting. "
         L"Sphere. Spice world. And, Titanic."
         , NULL, 1);
      goto askmovie;
      break;
   case 1:
      m_pQueue->Speak (
         L"Air Force One, starring Harrison Ford, is showing at 1:15, 3:30, 6:00, and 9:15. "
         , NULL, 1);
      break;
   case 2:
      m_pQueue->Speak (
         L"Conspiracy Theory, starring Mel Gibson and Julia Roberts, is showing at 1:30, 3:45, 6:15, and 9:30. "
         , NULL, 1);
      break;
   case 3:
      m_pQueue->Speak (
         L"Contact, starring Jodie Foster, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      break;
   case 4:
      m_pQueue->Speak (
         L"George of the Jungle, starring Brendan Fraser, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = 'P';
      break;
   case 5:
      m_pQueue->Speak (
         L"Hoodlum, starring Laurence Fishburne, is showing at 1:15, 3:30, 6:00, and 9:15. "
         , NULL, 1);
      cRating = 'R';
      break;
   case 6:
      m_pQueue->Speak (
         L"Masterminds, starring Patrick Stewart, is showing at 1:15, 3:30, 6:00, and 9:15. "
         , NULL, 1);
      cRating = '1';
      break;
   case 7:
      m_pQueue->Speak (
         L"Men in Black, starring Tommy Lee Jones and Will Smith, is showing at 1:15, 3:30, 6:00, and 9:15. "
         , NULL, 1);
      cRating = '1';
      break;
   case 8:
      m_pQueue->Speak (
         L"Mimic, starring Mira Sorvino, is showing at 1:15, 3:30, 6:00, and 9:15. "
         , NULL, 1);
      cRating = 'R';
      break;
   case 9:
      m_pQueue->Speak (
         L"A Smile Like Yours, starring Greg Kinnear, is showing at 12:30, 2:30, 5:00, and 8:15. "
         , NULL, 1);
      cRating = 'R';
      break;
   case 10:
      m_pQueue->Speak (
         L"Steel, starring Shaquille O'Neal, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = '1';
      break;
   case 11:
      m_pQueue->Speak (
         L"Money Talks, starring Charles Sheen and Chris Tucker, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = 'R';
      break;
   case 12:
      m_pQueue->Speak (
         L"As good as it gets, starring Helen Hunt and Jack Nicholson, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = 'R';
      break;
   case 13:
      m_pQueue->Speak (
         L"Blues brothers 2000, starring Dan Akroyd and John Goodman, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = '1';
      break;
   case 14:
      m_pQueue->Speak (
         L"The borrowers, starring John Goodman and Mark Williams, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = 'P';
      break;
   case 15:
      m_pQueue->Speak (
         L"Desperate measures, starring Michael Keaton and Andy Garcia, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = 'R';
      break;
   case 16:
      m_pQueue->Speak (
         L"Good Will Hunting, starring Matt Damon and Ben Affleck, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = 'R';
      break;
   case 17:
      m_pQueue->Speak (
         L"The replacement killers, starring Chow Yun-Fat and Mira Sorvino, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = 'R';
      break;
   case 18:
      m_pQueue->Speak (
         L"Sphere, starring Dustin Hoffman and Sharon Stone, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = '1';
      break;
   case 19:
      m_pQueue->Speak (
         L"Spice World, starring the spice girls, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = 'P';
      break;
   case 20:
      m_pQueue->Speak (
         L"Titanic, starring Leonardo DiCaprio and Kate Winslet, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = '1';
      break;
   case 21:
      m_pQueue->Speak (
         L"The wedding singer, starring Adam Sandler and Drew Barrymore, is showing at 1:00, 3:15, 5:45, and 9:00. "
         , NULL, 1);
      cRating = '1';
      break;
   }
   
   
   switch (cRating) {
   case 'G':
      m_pQueue->Speak (
         L"This movie is rated G. "
         , NULL, 1);
      break;
   case 'P':
      m_pQueue->Speak (
         L"This movie is rated PG. "
         , NULL, 1);
      break;
   case '1':
      m_pQueue->Speak (
         L"This movie is rated PG-13. "
         , NULL, 1);
      break;
   case 'R':
      m_pQueue->Speak (
         L"This movie is rated R. "
         , NULL, 1);
      break;
   case 'N':
      m_pQueue->Speak (
         L"This movie is rated NC-17. "
         , NULL, 1);
      break;
   }

   return 0;
}

/*****************************************************************
DoPhoneCall - Does all the operations necessary to handle a Movie
   phone call. The phone call consists of the following parts:
      Get the phone number, and address
      Get Movie information
      Say how much it's going to cost and verify

inputs
   none
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or TCR_ASKBACK to go back
*/
HRESULT CCallMovie::DoPhoneCall (void)
{
   DWORD dwRes;
   BOOL  fEndLoop = FALSE;
   BOOL  fHangUp = FALSE;


   // speak the introduction
   m_pQueue->Speak (
      L"Welcome to the ACME Movie theater. "
      , NULL, 1);

   while (TRUE) {
      // get info about the Movie
      dwRes = GetMovieInfo();
      switch (dwRes) {
      case TCR_ASKHANGUP:
         if (VerifyHangUp()) {
            fEndLoop = TRUE;
            fHangUp = TRUE;
            break;   // break out of the loop
         }
         else
            continue;   // ask the movie again
      case TCR_ABORT:
         fEndLoop = TRUE;
         fHangUp = TRUE;
         break;   // break out of the infinite loop
      case TCR_ASKBACK:
         continue;
      }
      if (fEndLoop)
         break;

      // ask them if they want to hear another movie
      WCHAR szAnother[] =
         L"[Prompts]\n"
         L"Main=Do you want to hear about another movie?\n"
         L"Where=You are being asked if you want to hear about another movie.\n"
         ;

      DWORD dwRes;
      m_pTCYesNo->GoFromMemory (szAnother, sizeof(szAnother), &dwRes);
      switch (dwRes) {
      case 1: // yes
         continue;
      case TCR_ABORT:
         fEndLoop = TRUE;
         fHangUp = TRUE;
         break;   // break out of the infinite loop
      default: // no or other responses
         fEndLoop = TRUE;   // break out
      }
      if (fEndLoop)
         break;
   }

   // say thank you and hang up
   if (!fHangUp) {
      m_pQueue->Speak (
         L"The ACME theaters are located on Gilman Blvd., just east of 900. "
         L"Thank you for calling ACME theater's information service. Goodbye."
         , NULL, 1);

      // wait until we're done speaking
      m_pQueue->WaitUntilDoneSpeaking();
   }

   return NOERROR;
}
