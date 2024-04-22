# Microsoft Developer Studio Project File - Name="TTSAPP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

CFG=TTSAPP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ttsapp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ttsapp.mak" CFG="TTSAPP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TTSAPP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TTSAPP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "TTSAPP - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "TTSAPP - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/Demos/ttsapp", MWWAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "TTSAPP - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 nafxcw.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib comctl32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "Z:\api" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ole32.lib /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\TTSAPP__"
# PROP BASE Intermediate_Dir ".\TTSAPP__"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\AlphaDbg"
# PROP Intermediate_Dir ".\AlphaDbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /MDd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /MDd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\TTSAPP_0"
# PROP BASE Intermediate_Dir ".\TTSAPP_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\AlphaRel"
# PROP Intermediate_Dir ".\AlphaRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /alpha
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 nafxcw.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib /nologo /subsystem:windows /machine:ALPHA

!ENDIF 

# Begin Target

# Name "TTSAPP - Win32 Release"
# Name "TTSAPP - Win32 Debug"
# Name "TTSAPP - Win32 (ALPHA) Debug"
# Name "TTSAPP - Win32 (ALPHA) Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\guidseg.cpp

!IF  "$(CFG)" == "TTSAPP - Win32 Release"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Release"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "TTSAPP - Win32 Release"

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug"

DEP_CPP_STDAF=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Release"

DEP_CPP_STDAF=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TTSAPP.cpp

!IF  "$(CFG)" == "TTSAPP - Win32 Release"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug"

DEP_CPP_TTSAP=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Release"

DEP_CPP_TTSAP=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TTSAPP.rc
# End Source File
# Begin Source File

SOURCE=.\ttsappd.cpp

!IF  "$(CFG)" == "TTSAPP - Win32 Release"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug"

DEP_CPP_TTSAPPD=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Release"

DEP_CPP_TTSAPPD=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\leaks.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TTSAPP.h
# End Source File
# Begin Source File

SOURCE=.\TTSAPPD.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\TTSAPP.ico
# End Source File
# Begin Source File

SOURCE=.\res\TTSAPP.rc2
# End Source File
# End Group
# End Target
# End Project
