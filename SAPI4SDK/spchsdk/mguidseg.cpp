/******************************************************************************\
*       guidseg.cpp
*
*       This file initializes GUIDS for the Microsoft Speech API
*       NOTE: do not use Precompiled Headers to compile this file
*       due to the inclusion of initguid.h.
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*


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
#include <initguid.h>
#include <objerror.h>

#ifdef DIRECTSOUND
#include <dsound.h>
#endif // DIRECTSOUND

#include <speech.h>	  // init GUIDs for Speech API (see initguid.h)
