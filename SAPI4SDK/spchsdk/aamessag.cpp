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
#include <spchwrap.h>
#include <dsound.h>
#include <stdio.h>
#include "answer.h"
#include "resource.h"



/*
 *  OpenRootStorage - This will get root for the message store
 */
BOOL OpenRootStorage( LPSTORAGE *ppStgOut,  PSTR pszFileName )
{
    HRESULT hrOpen, hr;
    LPSTORAGE pStg;
    PWCHAR pNameW;
    DWORD  i, dwLen;

    pStg = NULL;

    // UNICODE the doc file name
    dwLen = lstrlen(pszFileName)+1;
    pNameW = (PWCHAR) ALLOC(sizeof(WCHAR)*dwLen);
    if (!pNameW)
        return FALSE;

    i = MultiByteToWideChar(CP_ACP, NULL, pszFileName, -1, pNameW, dwLen);
    if (!i)
        return FALSE;

    // check if there is a doc file already
    hrOpen = StgOpenStorage( pNameW,
                             NULL,
                             // access mode
                             STGM_READWRITE | 
                             STGM_SHARE_EXCLUSIVE |
                             STGM_TRANSACTED,
                             NULL,
                             0,
                             &pStg );

    if (hrOpen == S_OK) {
        *ppStgOut = pStg;
        FREE(pNameW);
    }
    else {
        if (hrOpen == STG_E_FILENOTFOUND) {
            // if we couldn't find one, start a new one
            hr = StgCreateDocfile( pNameW,
                                   // access mode
                                   STGM_READWRITE | 
                                   STGM_SHARE_EXCLUSIVE |
                                   STGM_TRANSACTED,
                                   NULL,
                                   &pStg );

            if (hr != S_OK)
                return FALSE;
            else {
                *ppStgOut = pStg;
                FREE(pNameW);
            }
        }
        else {
            FREE(pNameW);
            return FALSE;
        }
    }

    return TRUE;
}


HRESULT SaveMessage(PVOID pWAV, DWORD dwWAVSize, DWORD dwIX)
{
    LPSTORAGE   pStg;
    BOOL        fRes;

    fRes = OpenRootStorage(&pStg, "c:\\program files\\Microsoft Speech SDK\\am.msg");
    if (fRes == FALSE)
        return E_FAIL;


    return NOERROR;
}