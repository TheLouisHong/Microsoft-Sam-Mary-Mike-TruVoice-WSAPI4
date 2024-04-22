# Microsoft Developer Studio Project File - Name="Ttstst" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

CFG=Ttstst - Win32 A2ADebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ttstst.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ttstst.mak" CFG="Ttstst - Win32 A2ADebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ttstst - Win32 A2ADebug" (based on "Win32 (x86) Application")
!MESSAGE "Ttstst - Win32 A2ARel" (based on "Win32 (x86) Application")
!MESSAGE "Ttstst - Win32 W2WDebug" (based on "Win32 (x86) Application")
!MESSAGE "Ttstst - Win32 W2WRel" (based on "Win32 (x86) Application")
!MESSAGE "Ttstst - Win32 (ALPHA) A2ADebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Ttstst - Win32 (ALPHA) A2ARel" (based on "Win32 (ALPHA) Application")
!MESSAGE "Ttstst - Win32 (ALPHA) W2WDebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Ttstst - Win32 (ALPHA) W2WRel" (based on "Win32 (ALPHA) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/Test/tts/ttstst", WBPAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\A2ADebug"
# PROP BASE Intermediate_Dir ".\A2ADebug"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\A2ADebug"
# PROP Intermediate_Dir ".\A2ADebug"
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_S_UNICODE" /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uuid.lib ole32.lib /nologo /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT BASE LINK32 /pdb:none /map
# ADD LINK32 winmm.lib ole32.lib /nologo /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none /map

!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\A2ARel"
# PROP BASE Intermediate_Dir ".\A2ARel"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\A2ARel"
# PROP Intermediate_Dir ".\A2ARel"
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_S_UNICODE" /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uuid.lib ole32.lib /nologo /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT BASE LINK32 /pdb:none /map
# ADD LINK32 winmm.lib ole32.lib /nologo /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none /map

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\W2WDebug"
# PROP BASE Intermediate_Dir ".\W2WDebug"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\W2WDebug"
# PROP Intermediate_Dir ".\W2WDebug"
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_S_UNICODE" /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uuid.lib ole32.lib /nologo /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT BASE LINK32 /pdb:none /map
# ADD LINK32 winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup" /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none /map

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\W2WRel"
# PROP BASE Intermediate_Dir ".\W2WRel"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\W2WRel"
# PROP Intermediate_Dir ".\W2WRel"
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_S_UNICODE" /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uuid.lib ole32.lib /nologo /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT BASE LINK32 /pdb:none /map
# ADD LINK32 winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup" /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none /map

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Ttstst__"
# PROP BASE Intermediate_Dir ".\Ttstst__"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\A2ADebug"
# PROP Intermediate_Dir ".\A2ADebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /MDd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /MDd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:ALPHA
# ADD LINK32 winmm.lib ole32.lib /nologo /subsystem:windows /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Ttstst_0"
# PROP BASE Intermediate_Dir ".\Ttstst_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\A2ARel"
# PROP Intermediate_Dir ".\A2ARel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /alpha
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 winmm.lib ole32.lib /nologo /subsystem:windows /machine:ALPHA

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Ttstst_5"
# PROP BASE Intermediate_Dir ".\Ttstst_5"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\W2WDebug"
# PROP Intermediate_Dir ".\W2WDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /MDd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /YX /FD /MDd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:ALPHA
# ADD LINK32 winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:ALPHA
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Ttstst_6"
# PROP BASE Intermediate_Dir ".\Ttstst_6"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\W2WRel"
# PROP Intermediate_Dir ".\W2WRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /alpha
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:ALPHA
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Ttstst - Win32 A2ADebug"
# Name "Ttstst - Win32 A2ARel"
# Name "Ttstst - Win32 W2WDebug"
# Name "Ttstst - Win32 W2WRel"
# Name "Ttstst - Win32 (ALPHA) A2ADebug"
# Name "Ttstst - Win32 (ALPHA) A2ARel"
# Name "Ttstst - Win32 (ALPHA) W2WDebug"
# Name "Ttstst - Win32 (ALPHA) W2WRel"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\DLGFINDM.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GUIDSEG.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

# SUBTRACT BASE CPP /Yu
# SUBTRACT CPP /Yu

!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

# SUBTRACT BASE CPP /Yu
# SUBTRACT CPP /Yu

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

# SUBTRACT BASE CPP /Yu
# SUBTRACT CPP /Yu

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

# SUBTRACT BASE CPP /Yu
# SUBTRACT CPP /Yu

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yu

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yu

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yu

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\STDAFX.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TABISRCE.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TTSDLG.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TTSFNC.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TTSTST.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TTSTST.RC
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\DLGFINDM.H
# End Source File
# Begin Source File

SOURCE=.\STDAFX.H
# End Source File
# Begin Source File

SOURCE=.\TABISRCE.H
# End Source File
# Begin Source File

SOURCE=.\TTSDLG.H
# End Source File
# Begin Source File

SOURCE=.\TTSFNC.H
# End Source File
# Begin Source File

SOURCE=.\TTSTST.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\TTSTST.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\TTSTST.RC2
# End Source File
# End Group
# End Target
# End Project
