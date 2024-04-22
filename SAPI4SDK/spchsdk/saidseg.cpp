/************************************************************************
GuidSeg.cpp - GUID segment

This file includes the speech Globally Unique ID's, which are used by
OLE2.  These GUIDs are necessary for all speech applications.

Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <dbt.h>
#include <mmsystem.h>
#include <objbase.h>
#include <initguid.h>
#include <objerror.h>
#include <speech.h>
#include "audio.h"
