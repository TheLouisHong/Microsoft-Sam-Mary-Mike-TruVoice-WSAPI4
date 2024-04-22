/******************************************************************************\
*       stdafx.h : include file for standard system include files,
*       or project specific include files that are used frequently, but
*       are changed infrequently
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/


// add this for VC2.2...
#define _AFX_NO_BSTR_SUPPORT

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxtempl.h>           // MFC template classes

#ifndef _SPEECH_
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <speech.h>
#endif  // _SPEECH_

