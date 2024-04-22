/*******************************************************
Answer.cpp - Code to demonstrate writing an Answering machine.


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
#include "answer.h"
#include "resource.h"


char  gszNewFile[DIRSIZE];
WCHAR gwszForward[PEOPLECOUNT][64];

WCHAR *gpwszMonth[] = {L"January", L"February", L"March", L"April", L"May",
                       L"June", L"July", L"August", L"September", L"October",
                       L"November", L"December"};

WCHAR *gpwszDay[] = {L"Sunday", L"Monday", L"Tuesday", L"Wednesday",
                     L"Thursday", L"Friday", L"Saturday"};

WCHAR gwszLeft[] = L"left on ";
WCHAR gwszAt[] = L"at ";

extern char  gszDirectory[];
extern char  *gpszName[];
extern WCHAR *gpwszPass[];
extern WCHAR *gpwszName[];
extern DWORD gdwFileNum;



CCallAnswer::CCallAnswer()
{
    m_pITelInfo     = NULL;
    m_pTCGrammar    = NULL;
    m_pTCYesNo      = NULL;
    m_pTCRecord     = NULL;
    m_pTCPhoneNum   = NULL;
    m_pTCExtension  = NULL;
    m_pQueue        = NULL;

}

CCallAnswer::~CCallAnswer()
{
    // left intentionally blank
}


HRESULT CCallAnswer::Shutdown()
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
    if (m_pTCRecord)
        delete m_pTCRecord;
    if (m_pTCPhoneNum)
        delete m_pTCPhoneNum;
    if (m_pTCExtension)
        delete m_pTCExtension;
    if (m_pQueue)
        delete m_pQueue;

    return NOERROR;
}

HRESULT CCallAnswer::Init(PITELINFO pITelInfo)
{
    HRESULT hRes;

    m_pITelInfo = pITelInfo;

    m_pTCGrammar = new CTelControl;
    m_pTCYesNo = new CTelControl;
    m_pTCRecord = new CTelControl;
    m_pTCPhoneNum = new CTelControl;
    m_pTCExtension = new CTelControl;

    if (!m_pTCGrammar || !m_pTCYesNo || !m_pTCRecord || !m_pTCPhoneNum)
        return E_OUTOFMEMORY;

    // init the objects
    hRes = m_pTCGrammar->Init (CLSID_GrammarControl, m_pITelInfo);
    if (hRes) return hRes;
    hRes = m_pTCYesNo->Init (CLSID_YesNoControl, m_pITelInfo);
    if (hRes) return hRes;
    hRes = m_pTCRecord->Init (CLSID_RecordControl, m_pITelInfo);
    if (hRes) return hRes;
    hRes = m_pTCPhoneNum->Init (CLSID_PhoneNumControl, m_pITelInfo);
    if (hRes) return hRes;
    hRes = m_pTCExtension->Init (CLSID_ExtensionControl, m_pITelInfo);
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
    // placed them in the directory specified by Answer.txt.
    // You may have to modify answer.txt to get the recordings to work
//    m_pQueue->WaveAddFromList (IDR_WAVEPROMPTS, ghInstance);

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
BOOL CCallAnswer::VerifyHangUp (void)
{
    WCHAR szHangUp[] =
        L"[Prompts]\n"
        L"Main=Are you sure you want to hang up?\n"
        L"Where=You just said you wanted to hang up.\n"
        ;

    DWORD dwRes;
    m_pTCYesNo->GoFromMemory (szHangUp, sizeof(szHangUp), &dwRes);
    switch (dwRes) {
        case 2:  // no
            return FALSE;
        default: // yes or other responses
            return TRUE;
    }

}

/*********************************************************************
GetForwardNumber - Asks the user for the a forwarding number

inputs
   none
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or TCR_ASKBACK to go back
*/
DWORD CCallAnswer::GetForwardNumber (DWORD dwIX)
{
    DWORD dwRes, dwTelNumSize;
    PVOID pBuf;

asknum:
   // ask phone number
    WCHAR szAskNum[] =
        L"[Prompts]\n"
        L"Main=OK, I'm ready for the number \n"
        L"Help=Input the forwarding number.\n"
        L"Where=You are being asked for a forwarding number.\n"
        ;

    m_pTCPhoneNum->GoFromMemory (szAskNum, sizeof(szAskNum), &dwRes, (void**) &pBuf, &dwTelNumSize);
    switch (dwRes) {
        case TCR_ABORT:
        case TCR_NORESPONSE:
        case TCR_ASKOPERATOR:
            if (pBuf) CoTaskMemFree (pBuf);
            return TCR_ABORT;

        case TCR_ASKHANGUP:
            if (pBuf) CoTaskMemFree (pBuf);

            if (VerifyHangUp())
                return TCR_ABORT;
            else
                goto asknum;

        case TCR_ASKBACK:
            if (pBuf) CoTaskMemFree (pBuf);
            return TCR_ASKBACK;
    }

    CopyMemory(gwszForward[dwIX], pBuf, dwTelNumSize * sizeof(WCHAR));
    CoTaskMemFree(pBuf);
    return 0;
}



/*********************************************************************
PlayIt - Plays a message

inputs
   file name of wave file to play
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or TCR_ASKBACK to go back
*/
DWORD CCallAnswer::PlayIt (PSTR pszFileName, FILETIME ftCreate, DWORD dwIX)
{
    BOOL        fAM;
    PWSTR       pwszFile;
    DWORD       dwLen, dwRes, dwRet = 0;
    HRESULT     hr;
    FILETIME    ftOut;
    SYSTEMTIME  stFile;
    WCHAR       wszBuf[256], wszDay[10], wszHour[10], wszMinute[10];
    char        szBuf[DIRSIZE], szDay[10], szHour[10], szMinute[10];

    WCHAR szRepDel[] =
        L"[Prompts]\n"
        L"Main=Please say \"repeat\", \"erase\", or \"continue\". \n"
        L"Help=You can repeat or erase the message. Say neither to continue. \n"
        L"Where=You are being asked if you want to repeat or erase a message.\n"

        L"[<MyGrammar>]\n"
        L"<MyGrammar>=[opt] (JunkRepDelBegin) (RepDel) [opt] (JunkRepDelEnd)\n"

        L"[(JunkRepDelBegin)]\n"
        L"=I want to\n"
        L"=I wanna\n"
        L"=Can I\n"

        L"[(JunkRepDelEnd)]\n"
        L"=please\n"

        L"[(RepDel)]\n"
        L"200=repeat [opt] it\n"
        L"201=erase\n"
        L"201=delete\n"
        L"201=remove\n"
        L"202=continue\n"
        L"202=next\n"
        ;

    if (FileTimeToLocalFileTime(&ftCreate, &ftOut) == FALSE)
        goto NoTime;

    if (FileTimeToSystemTime(&ftOut, &stFile) == FALSE)
        goto NoTime;

    if (stFile.wHour > 12) {
        fAM = FALSE;
        stFile.wHour -= 12;
    }
    else if (stFile.wHour == 12)
        fAM = FALSE;
    else
        fAM = TRUE;

    itoa(stFile.wDay, szDay, 10);
    itoa(stFile.wHour, szHour, 10);
    itoa(stFile.wMinute, szMinute, 10);
    MultiByteToWideChar(CP_ACP, NULL, szDay, -1, wszDay, 10);
    MultiByteToWideChar(CP_ACP, NULL, szHour, -1, wszHour, 10);
    MultiByteToWideChar(CP_ACP, NULL, szMinute, -1, wszMinute, 10);

    ZeroMemory(wszBuf, sizeof(wszBuf));
    CopyMemory(wszBuf, gwszLeft, wcslen(gwszLeft));
    wcscat(wszBuf, gpwszDay[stFile.wDayOfWeek]);
    wcscat(wszBuf, L", ");
    wcscat(wszBuf, gpwszMonth[stFile.wMonth-1]);
    wcscat(wszBuf, L" ");
    wcscat(wszBuf, wszDay);
    wcscat(wszBuf, L", ");
    wcscat(wszBuf, gwszAt);
    wcscat(wszBuf, wszHour);
    wcscat(wszBuf, L":");
    wcscat(wszBuf, wszMinute);
    wcscat(wszBuf, L" ");
    wcscat(wszBuf, fAM ? L"AM" : L"PM");
    m_pQueue->Speak(wszBuf, NULL, 1);

NoTime:
    CopyMemory(szBuf, gszDirectory, lstrlen(gszDirectory) + 1);
    lstrcat(szBuf, gpszName[dwIX]);
    lstrcat(szBuf, "\\");
    lstrcat(szBuf, pszFileName);

    dwLen = (lstrlen(szBuf) + 1) * sizeof(WCHAR);
    pwszFile = (PWSTR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwLen);
    if (!pwszFile)
        return 0;

    MultiByteToWideChar(CP_ACP, NULL, szBuf, -1, pwszFile, dwLen);
    hr = m_pQueue->WaveAddFromFile(L"<VoiceMessage>", pwszFile);
    if (hr != NOERROR)
        goto FAILURE;

speakagain:
    m_pQueue->Speak(L"<VoiceMessage>", NULL, 1);

// please say replay, delete, next message, or no more
askafter:
    m_pTCGrammar->GoFromMemory (szRepDel, sizeof(szRepDel), &dwRes);
    switch (dwRes) {
        case TCR_ABORT:
        case TCR_NORESPONSE:
        case TCR_ASKOPERATOR:
            dwRet = TCR_ABORT;
            goto FAILURE;
            break;

        case TCR_ASKHANGUP:
            if (VerifyHangUp()) {
                dwRet = TCR_ABORT;
                goto FAILURE;
            }
            else
                goto askafter;

        case TCR_ASKBACK:
            dwRet = TCR_ASKBACK;
            goto FAILURE;
            break;
    }

    switch (dwRes) {
        case 200:
            goto speakagain;
            break;

        case 201:
            hr = m_pQueue->WaveRemove(L"<VoiceMessage>");
            if (!DeleteFile(szBuf)) {
                DWORD dwErr = GetLastError();
                m_pQueue->Speak(L"Could not erase message", NULL, 1);
            }
            else
                m_pQueue->Speak(L"Message erased", NULL, 1);
            break;

        default:
            break;
    }

FAILURE:
    m_pQueue->WaveRemove(L"<VoiceMessage>");
    if (pwszFile)
        HeapFree(GetProcessHeap(), NULL, pwszFile);
    return dwRet;
}


/*********************************************************************
PlayMessages - Plays the appropriate messages

inputs
   index into the mailbox of the person requesting administration privileges
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or TCR_ASKBACK to go back
*/
DWORD CCallAnswer::PlayMessages (DWORD dwIX)
{
    BOOL            fEndLoop = FALSE;
    HANDLE          hFile;
    WIN32_FIND_DATA wfdFile;

    char szBuf[DIRSIZE];
    CopyMemory(szBuf, gszDirectory, lstrlen(gszDirectory) + 1);
    lstrcat(szBuf, gpszName[dwIX]);
    lstrcat(szBuf, "\\msg*.wav");

    if ((hFile = FindFirstFile(szBuf, &wfdFile)) == INVALID_HANDLE_VALUE) {
        m_pQueue->Speak (L"No messages. ", NULL, 1);
        return 0;
    }
    
    m_pQueue->Speak (
        L"First message. "
        , NULL, 1);

    if (PlayIt(wfdFile.cFileName, wfdFile.ftCreationTime, dwIX) == TCR_ABORT) {
        FindClose (hFile);
        return TCR_ABORT;
    }

    while (TRUE) {
        // anymore messages?
        if (FindNextFile(hFile, &wfdFile) == FALSE) {
            m_pQueue->Speak (L"No more messages. ", NULL, 1);
            break;
        }

        // ask them if they want to hear another message
        WCHAR szNext[] =
            L"[Prompts]\n"
            L"Main=Do you want to hear the next message?\n"
            L"Where=You are being asked if you want to hear the next message.\n"
            ;

        DWORD dwRes;
        m_pTCYesNo->GoFromMemory (szNext, sizeof(szNext), &dwRes);
        switch (dwRes) {
            case 1: // yes
                break;

            case TCR_ABORT:
                FindClose(hFile);
                return TCR_ABORT;

            default: // no or other responses
                fEndLoop = TRUE;   // break out
        }

        if (fEndLoop)
            break;

        m_pQueue->Speak (
            L"Next message. "
            , NULL, 1);
        if (PlayIt(wfdFile.cFileName, wfdFile.ftCreationTime, dwIX) == TCR_ABORT) {
            FindClose (hFile);
            return TCR_ABORT;
        }
    }

    FindClose(hFile);
    return 0;
}



/*********************************************************************
DoAdministration - Asks for password, and if OK, can get messages remotely
                   or set a forwarding phone number.

inputs
   index into the mailbox of the person requesting administration privileges
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or TCR_ASKBACK to go back
*/
DWORD CCallAnswer::DoAdministration (DWORD dwIX)
{
    DWORD dwRes, dwExtSize, dwWAVSize;
    PVOID pBuf, pWAV;
    pWAV = NULL;

    // record greeting
    WCHAR szGreeting[] =
        L"[Prompts]\n"
        L"Main=Ready for the greeting after the beep. "
        L"Recording will stop after 2 seconds of silence\n"
        L"Where=You are recording a greeting.\n"
        L"[Settings]\n"
        L"BetweenUtt=3000\n"
        L"InitialBuf=30000\n"
        L"ReallocBuf=30000\n"
        L"MaxBuf=300000\n"
        L"NoAnswerTime=10\n"
        ;

    WCHAR szKeepGreet[] =
        L"[Prompts]\n"
        L"Main=Do you want to keep this greeting?\n"
        L"Where=You are being asked if you want to keep this greeting.\n"
        ;

askpass:
    WCHAR szPassword[] =
        L"[Prompts]\n"
        L"Main=Please enter your 5 digit password? \n"
        L"Help=Enter your 5 digit password.\n"
        L"Where=You are being asked for a password.\n"
        L"TooManyDigits=Passwords are only 5 digits. Please say the password. \n"
        L"TooFewDigits=You need to speak at least 5 digits. Please say the password. \n"
        L"[Settings]\n"
        L"NumDigits=5\n"
        L"Verify=0\n"
        ;
    m_pTCExtension->GoFromMemory (szPassword, sizeof(szPassword), &dwRes,
                                  &pBuf, &dwExtSize);
    switch (dwRes) {
        case TCR_ABORT:
        case TCR_NORESPONSE:
        case TCR_ASKOPERATOR:
            if (pBuf) CoTaskMemFree (pBuf);
            return TCR_ABORT;

        case TCR_ASKHANGUP:
            if (pBuf) CoTaskMemFree (pBuf);
            if (VerifyHangUp())
                return TCR_ABORT;
            else
                goto askpass;

        case TCR_ASKBACK:
            if (pBuf) CoTaskMemFree (pBuf);
            return TCR_ASKBACK;
    }

    if (wcscmp(gpwszPass[dwIX], (PCWSTR)pBuf)) {
        m_pQueue->Speak(L"I'm sorry this password is not correct", NULL, 1);
         if (pBuf) CoTaskMemFree (pBuf);
        return ADMIN_LEAVEMESSAGE;
    }
   if (pBuf) CoTaskMemFree (pBuf);

asktask:
    WCHAR szTask[] =
        L"[Prompts]\n"
        L"Main=What would you like to do? \n"
        L"Help=You can play messages, leave a message, set a forwarding number, erase the forwarding number, record a greeting, or leave.\n"
        L"Where=You are being what task you want to do.\n"

        L"[<MyGrammar>]\n"
        L"<MyGrammar>=[opt] (JunkAnswerBegin) (Answer) [opt] (JunkAnswerEnd)\n"

        L"[(JunkAnswerBegin)]\n"
        L"=I want to\n"
        L"=Can I\n"

        L"[(JunkAnswerEnd)]\n"
        L"=please\n"

        L"[(Answer)]\n"
        L"100=play messages\n"
        L"100=check messages\n"
        L"101=set [opt] a forwarding number\n"
        L"102=erase [opt] the forwarding number\n"
        L"103=leave a message\n"
        L"105=record a greeting\n"
        L"106=go back\n"
        L"106=quit\n"
        L"106=leave\n"
        L"106=return\n"
        L"-11=what can I do\n"
        L"-11=I don't know\n"
        ;
    m_pTCGrammar->GoFromMemory (szTask, sizeof(szTask), &dwRes);
    switch (dwRes) {
        case TCR_ABORT:
        case TCR_NORESPONSE:
        case TCR_ASKOPERATOR:
            return TCR_ABORT;

        case TCR_ASKHANGUP:
            if (VerifyHangUp())
                return TCR_ABORT;
            else
                goto asktask;

        case TCR_ASKBACK:
            return TCR_ASKBACK;
    }

    switch (dwRes) {
        case 100:
            if (PlayMessages(dwIX) == TCR_ABORT)
                return TCR_ABORT;
            break;

        case 101:
            if (GetForwardNumber(dwIX) == TCR_ABORT)
                return TCR_ABORT;
            break;

        case 102:
            gwszForward[dwIX][0] = 0;
            m_pQueue->Speak(L"Forwarding number erased", NULL, 1);
            break;

        case 103:
            return ADMIN_LEAVEMESSAGE;
            break;

        case 105:
askgreeting:
            m_pTCRecord->GoFromMemory (szGreeting, sizeof(szGreeting), &dwRes, &pWAV, &dwWAVSize);
            switch (dwRes) {
                case TCR_ABORT:
                    if (pWAV) CoTaskMemFree (pWAV);
                    pWAV = NULL;
                    return TCR_ABORT;
            }

            if (!pWAV)
                goto asktask; // didn't hear it

            m_pQueue->WaveAddFromMemory(L"<Greeting>", pWAV, dwWAVSize);
            m_pQueue->Speak(L"I heard", NULL, 1);
            m_pQueue->Speak(L"<Greeting>", NULL, 1);

askagain:
            m_pTCYesNo->GoFromMemory (szKeepGreet, sizeof(szKeepGreet), &dwRes);
            switch (dwRes) {
                case 1: // yes
                    goto keepgreet;

                case 2: // no
                    goto askgreeting;

                case TCR_ABORT:
                    if (pWAV) CoTaskMemFree (pWAV);
                    pWAV = NULL;
                    return TCR_ABORT;

                default:
                    goto askagain;
            }

keepgreet:
            // save the greeting
            CopyMemory(gszNewFile, gszDirectory, lstrlen(gszDirectory) + 1);
            lstrcat(gszNewFile, "\\");
            lstrcat(gszNewFile, "greeting.wav");

            HANDLE  hFile;
            DWORD   dwUsed;
            hFile = CreateFile(gszNewFile, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile == INVALID_HANDLE_VALUE) {
                dwUsed = GetLastError();
            }
            else {
                WriteFile(hFile, pWAV, dwWAVSize, &dwUsed, NULL);
                BOOL fRet = CloseHandle(hFile);
            }

            CoTaskMemFree(pWAV);
            pWAV = NULL;

            break;

        case 106:
            if (pWAV) CoTaskMemFree (pWAV);
            pWAV = NULL;
            return 0;
            break;
    }

    // go back and ask the user what else they want to do
    goto asktask;

}



/*********************************************************************
ProcessForwardRequest - Asks if caller wants the forwarding number of
                        the person they are trying to call. Also asks
                        if the caller wants to leave a message for the
                        person they are tyring to call.

inputs
   index into the mailbox of the person caller is trying to reach
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or ADMIN_LEAVEMESSAGE if they want
           to leave a message.
*/
DWORD CCallAnswer::ProcessForwardRequest(DWORD dwIX)
{
    DWORD dwRes;

    WCHAR szForward1[] =
        L"[Prompts]\n"
        L"Main=Would you like the number where Jill can be reached?\n"
        L"Where=You are being asked if you want Jill's forwarding number.\n"
        ;
    WCHAR szForward2[] =
        L"[Prompts]\n"
        L"Main=Would you like the number where Larry can be reached?\n"
        L"Where=You are being asked if you want Larry's forwarding number.\n"
        ;
    WCHAR szForward3[] =
        L"[Prompts]\n"
        L"Main=Would you like the number where Alison can be reached?\n"
        L"Where=You are being asked if you want Alison's forwarding number.\n"
        ;
    WCHAR szForward4[] =
        L"[Prompts]\n"
        L"Main=Would you like the number where Elliot can be reached?\n"
        L"Where=You are being asked if you want Elliot's forwarding number.\n"
        ;
    WCHAR szForward5[] =
        L"[Prompts]\n"
        L"Main=Would you like the number where Jennifer can be reached?\n"
        L"Where=You are being asked if you want Jennifer's forwarding number.\n"
        ;

    switch (dwIX) {
        case 0:
            m_pTCYesNo->GoFromMemory (szForward1, sizeof(szForward1), &dwRes);
            break;

        case 1:
            m_pTCYesNo->GoFromMemory (szForward2, sizeof(szForward2), &dwRes);
            break;

        case 2:
            m_pTCYesNo->GoFromMemory (szForward3, sizeof(szForward3), &dwRes);
            break;

        case 3:
            m_pTCYesNo->GoFromMemory (szForward4, sizeof(szForward4), &dwRes);
            break;

        case 4:
            m_pTCYesNo->GoFromMemory (szForward5, sizeof(szForward5), &dwRes);
            break;
    }

    switch (dwRes) {
        case 1: // yes
            // tell the number and break
            m_pQueue->Speak (gpwszName[dwIX], NULL, 1);
            m_pQueue->Speak (L"can be reached at", NULL, 1);
            m_pQueue->Speak (gwszForward[dwIX], NULL, 1);
            break;

        case TCR_ABORT:
            return TCR_ABORT;
    }

    WCHAR szLeaveMsg1[] =
        L"[Prompts]\n"
        L"Main=Would you like to leave a message for Jill?\n"
        L"Where=You are being asked if you want leave a message for Jill.\n"
        ;
    WCHAR szLeaveMsg2[] =
        L"[Prompts]\n"
        L"Main=Would you like to leave a message for Larry?\n"
        L"Where=You are being asked if you want leave a message for Larry.\n"
        ;
    WCHAR szLeaveMsg3[] =
        L"[Prompts]\n"
        L"Main=Would you like to leave a message for Alison?\n"
        L"Where=You are being asked if you want leave a message for Alison.\n"
        ;
    WCHAR szLeaveMsg4[] =
        L"[Prompts]\n"
        L"Main=Would you like to leave a message for Elliot?\n"
        L"Where=You are being asked if you want leave a message for Elliot.\n"
        ;
    WCHAR szLeaveMsg5[] =
        L"[Prompts]\n"
        L"Main=Would you like to leave a message for Jennifer?\n"
        L"Where=You are being asked if you want leave a message for Jennifer.\n"
        ;

    switch (dwIX) {
        case 0:
            m_pTCYesNo->GoFromMemory (szLeaveMsg1, sizeof(szLeaveMsg1), &dwRes);
            break;

        case 1:
            m_pTCYesNo->GoFromMemory (szLeaveMsg2, sizeof(szLeaveMsg2), &dwRes);
            break;

        case 2:
            m_pTCYesNo->GoFromMemory (szLeaveMsg3, sizeof(szLeaveMsg3), &dwRes);
            break;

        case 3:
            m_pTCYesNo->GoFromMemory (szLeaveMsg4, sizeof(szLeaveMsg4), &dwRes);
            break;

        case 4:
            m_pTCYesNo->GoFromMemory (szLeaveMsg5, sizeof(szLeaveMsg5), &dwRes);
            break;
    }

    switch (dwRes) {
        case 1: // yes
            return ADMIN_LEAVEMESSAGE;

        case TCR_ABORT:
            return TCR_ABORT;
    }

    return 0;
}

/*********************************************************************
GetMessageInfo - Asks the user for the person they would like to talk to

inputs
   none
returns
   DWORD - 0 if OK, or TCR_ABORT to quit, or TCR_ASKBACK to go back
*/
DWORD CCallAnswer::GetMessageInfo (void)
{
    DWORD dwRes;
    DWORD dwRetFromFunc = 0;

askname:
    WCHAR szSize[] =
        L"[Prompts]\n"
        L"Main=Say the name of the person you want to leave a message for. \n"
        L"Main.2=Who do you want to leave a message for? \n"
        L"Help=You can leave a message for the following: Larry, Jill, Alison, Elliot, or Jenny.\n"
        L"Where=You are being asked for a name.\n"

        L"[<MyGrammar>]\n"
        L"<MyGrammar>=[opt] (JunkAnswerBegin) (Answer) [opt] (JunkAnswerEnd)\n"
        L"<MyGrammar>=[opt] (JunkModeBegin) (Mode) [opt] (JunkModeEnd)\n"

        L"[(JunkAnswerBegin)]\n"
        L"=I want to leave a message for\n"
        L"=Can I leave a message for\n"
        L"=I wanna talk to\n"

        L"[(JunkAnswerEnd)]\n"
        L"=please\n"

        L"[(JunkModeBegin)]\n"
        L"=Hello\n"
        L"=Hi\n"

        L"[(JunkModeEnd)]\n"
        L"=calling\n"

        L"[(Mode)]\n"
        L"100=This is Jill\n"
        L"101=This is Larry\n"
        L"102=This is Alison\n"
        L"103=This is Elliot\n"
        L"104=This is Jenny\n"
        L"-11=I don't know\n"
        L"-11=What can I say\n"

        L"[(Answer)]\n"
        L"1=Jill\n"
        L"1=Jill Israel\n"
        L"1=Mrs. Israel\n"
        L"1=Mrs. Jill Israel\n"
        L"2=Larry\n"
        L"2=Larry Israel\n"
        L"2=Mr. Israel\n"
        L"2=Mr. Larry Israel\n"
        L"3=Ali\n"
        L"3=Alison\n"
        L"3=Alison Israel\n"
        L"4=Elliot\n"
        L"4=Elliot Israel\n"
        L"5=Jenny\n"
        L"5=Jennifer\n"
        L"5=Jennifer Israel\n"
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
                goto askname;

        case TCR_ASKBACK:
            return TCR_ASKBACK;
    }

    char  *pszName;
    DWORD dwIX = dwRes-1;
    switch (dwRes) {
        // request to get in administrative mode
        case 100:
        case 101:
        case 102:
        case 103:
        case 104:
            dwRes = DoAdministration(dwRes-100);
            if (dwRes == ADMIN_LEAVEMESSAGE)
                goto askname;
            return dwRes;
            break;

        case 1:
            if (gwszForward[dwIX][0]) {
                if ((dwRes = ProcessForwardRequest(dwIX)) == TCR_ABORT)
                    return TCR_ABORT;
                if (dwRes != ADMIN_LEAVEMESSAGE)
                    return 0;
            }

            m_pQueue->Speak (
                L"Jill's mailbox. "
                , NULL, 1);
            pszName = gpszName[dwIX];
            break;

        case 2:
            if (gwszForward[dwIX][0]) {
                if ((dwRes = ProcessForwardRequest(dwIX)) == TCR_ABORT)
                    return TCR_ABORT;
                if (dwRes != ADMIN_LEAVEMESSAGE)
                    return 0;
            }

            m_pQueue->Speak (
                L"Larry's mailbox. "
                , NULL, 1);
            pszName = gpszName[dwIX];
            break;

        case 3:
            if (gwszForward[dwIX][0]) {
                if ((dwRes = ProcessForwardRequest(dwIX)) == TCR_ABORT)
                    return TCR_ABORT;
                if (dwRes != ADMIN_LEAVEMESSAGE)
                    return 0;
            }

            m_pQueue->Speak (
                L"Alison's mailbox. "
                , NULL, 1);
            pszName = gpszName[dwIX];
            break;

        case 4:
            if (gwszForward[dwIX][0]) {
                if ((dwRes = ProcessForwardRequest(dwIX)) == TCR_ABORT)
                    return TCR_ABORT;
                if (dwRes != ADMIN_LEAVEMESSAGE)
                    return 0;
            }

            m_pQueue->Speak (
                L"Elliot's mailbox. "
                , NULL, 1);
            pszName = gpszName[dwIX];
            break;

        case 5:
            if (gwszForward[dwIX][0]) {
                if ((dwRes = ProcessForwardRequest(dwIX)) == TCR_ABORT)
                    return TCR_ABORT;
                if (dwRes != ADMIN_LEAVEMESSAGE)
                    return 0;
            }

            m_pQueue->Speak (
                L"Jennifer's mailbox. "
                , NULL, 1);
            pszName = gpszName[dwIX];
            break;
    }

    PVOID pWAV;
    DWORD dwWAVSize;
    pWAV = NULL;

getmessage:
    // get the message
    WCHAR szName[] =
        L"[Prompts]\n"
        L"Main=Please leave a message after the beep. "
        L"Recording will stop after 2 seconds of silence\n"
        L"Where=You are leaving a message.\n"
        L"[Settings]\n"
        L"BetweenUtt=2000\n"
        L"InitialBuf=30000\n"
        L"ReallocBuf=30000\n"
        L"MaxBuf=300000\n"
        L"NoAnswerTime=10\n"
        ;

    WCHAR szReview[] =
        L"[Prompts]\n"
        L"Main=Would you like to review this message?\n"
        L"Where=You are being asked if you want to review the message you just left.\n"
        ;

    BOOL fReview = FALSE;
    m_pTCRecord->GoFromMemory (szName, sizeof(szName), &dwRes, &pWAV, &dwWAVSize);
    switch (dwRes) {
        case TCR_ABORT:
            if (pWAV) {
               dwRetFromFunc = TCR_ABORT;
               goto saveit;
            }
            else {
              if (pWAV) CoTaskMemFree (pWAV);
              pWAV = NULL;
               return TCR_ABORT;
            }
    }

    if (!pWAV)
        goto getmessage; // didn't hear it

    // ask them if they want to review the message
    m_pTCYesNo->GoFromMemory (szReview, sizeof(szReview), &dwRes);
    switch (dwRes) {
        case 1: // yes
            m_pQueue->WaveAddFromMemory (L"<ReviewMessage>", pWAV, dwWAVSize);
            m_pQueue->Speak(L"<ReviewMessage>", NULL, 1);
            fReview = TRUE;
            break;

        case TCR_ABORT: {
           if (pWAV) CoTaskMemFree (pWAV);
           pWAV = NULL;
            return TCR_ABORT;
            }
    }

    if (fReview) {
        // ask them if they want to save the message
        WCHAR szSave[] =
            L"[Prompts]\n"
            L"Main=Do you want to send this message?\n"
            L"Where=You are being asked if you want to send the message you just left.\n"
            ;

        m_pTCYesNo->GoFromMemory (szSave, sizeof(szSave), &dwRes);
        switch (dwRes) {
            case 1: // yes
                break;

            case TCR_ABORT: {
                 if (pWAV) CoTaskMemFree (pWAV);
                 pWAV = NULL;
                return TCR_ABORT;
                }

            default: {
                 if (pWAV) CoTaskMemFree (pWAV);
                 pWAV = NULL;
                return 0;
                }
        }
    }

saveit:
    // save the message
    char szBuf[128];
    wsprintf(szBuf, "msg%05d.wav", gdwFileNum++);
    CopyMemory(gszNewFile, gszDirectory, lstrlen(gszDirectory) + 1);
    lstrcat(gszNewFile, pszName);
    lstrcat(gszNewFile, "\\");
    lstrcat(gszNewFile, szBuf);

    HANDLE  hFile;
    DWORD   dwUsed;
    hFile = CreateFile(gszNewFile, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                       CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        dwUsed = GetLastError();
    }
    else {
        WriteFile(hFile, pWAV, dwWAVSize, &dwUsed, NULL);
        BOOL fRet = CloseHandle(hFile);
    }

     if (pWAV) CoTaskMemFree (pWAV);
     pWAV = NULL;

    return dwRetFromFunc;
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
HRESULT CCallAnswer::DoPhoneCall (void)
{
    DWORD dwRes, dwLen;
    BOOL  fEndLoop = FALSE;
    PWSTR pwszFile;

    // check if there a greeting wave
    CopyMemory(gszNewFile, gszDirectory, lstrlen(gszDirectory) + 1);
    lstrcat(gszNewFile, "\\");
    lstrcat(gszNewFile, "greeting.wav");

    HANDLE  hFile;
    hFile = CreateFile(gszNewFile, GENERIC_READ, 0, NULL,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hFile);
        dwLen = (lstrlen(gszNewFile) + 1) * sizeof(WCHAR);
        pwszFile = (PWSTR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwLen);
        if (!pwszFile)
            goto NoFile;

        MultiByteToWideChar(CP_ACP, NULL, gszNewFile, -1, pwszFile, dwLen);

        if (m_pQueue->WaveAddFromFile(L"<MainGreeting>", pwszFile) != NOERROR)
            goto NoFile;

        m_pQueue->Speak (L"<MainGreeting>", NULL, 1);
    }
    else {
NoFile:
        // speak the introduction
        m_pQueue->Speak (
            L"Hello, you've reached the mail box of Larry and Jennifer. "
            , NULL, 1);
    }

    while (TRUE) {
        // get info about the Movie
        dwRes = GetMessageInfo();
        switch (dwRes) {
            case TCR_ASKHANGUP:
                if (VerifyHangUp()) {
                    fEndLoop = TRUE;
                    break;   // break out of the loop
                }
                else
                    continue;   // ask the movie again

            case TCR_ABORT:
                return NOERROR;

            case TCR_ASKBACK:
                continue;
        }

        if (fEndLoop || dwRes == ADMIN_DONE)
            break;

        // ask them if they want to leave another message
        WCHAR szAdminDone[] =
            L"[Prompts]\n"
            L"Main=Do you want to leave a message for anybody?\n"
            L"Where=You are being asked if you want to hear about another movie.\n"
            ;

        // ask them if they want to leave another message
        WCHAR szAnother[] =
            L"[Prompts]\n"
            L"Main=Do you want to leave another message?\n"
            L"Where=You are being asked if you want to hear about another movie.\n"
            ;

        m_pTCYesNo->GoFromMemory (szAnother, sizeof(szAnother), &dwRes);
        switch (dwRes) {
            case 1: // yes
                continue;

            case TCR_ABORT:
                return NOERROR;

            default: // no or other responses
                fEndLoop = TRUE;   // break out
        }

        if (fEndLoop)
            break;
    }

    // say thank you and hang up
    m_pQueue->Speak (
        L"Thank you for calling. Goodbye."
        , NULL, 1);

    // wait until we're done speaking
   m_pQueue->WaitUntilDoneSpeaking();

    return NOERROR;
}
