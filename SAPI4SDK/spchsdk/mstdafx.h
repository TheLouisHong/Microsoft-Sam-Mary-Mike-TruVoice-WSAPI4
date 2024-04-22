// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

// comment this out not to use direct sound
// #define DIRECTSOUND


#ifndef _DEBUG
#define _AFX_NOFORCE_LIBS
#endif

#define  _AFX_NO_BSTR_SUPPORT

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <objbase.h>                    // OLE stuff
#include <objerror.h>
#include <ole2ver.h>

#include <afxtempl.h>           // MFC template classes

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <mmsystem.h>

#ifdef DIRECTSOUND
#include <dsound.h>
#endif

#include "speech.h"
#include "TTSAPP.h"
#include "TTSAPPD.h"
#include "leaks.h"

// #ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
// #endif

