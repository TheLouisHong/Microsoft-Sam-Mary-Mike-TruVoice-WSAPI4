# Microsoft Developer Studio Project File - Name="Vttest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

CFG=Vttest - Win32 A2ADebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vttest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vttest.mak" CFG="Vttest - Win32 A2ADebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Vttest - Win32 A2ADebug" (based on "Win32 (x86) Application")
!MESSAGE "Vttest - Win32 A2ARel" (based on "Win32 (x86) Application")
!MESSAGE "Vttest - Win32 W2WDebug" (based on "Win32 (x86) Application")
!MESSAGE "Vttest - Win32 W2WRel" (based on "Win32 (x86) Application")
!MESSAGE "Vttest - Win32 (ALPHA) A2ADebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Vttest - Win32 (ALPHA) A2ARel" (based on "Win32 (ALPHA) Application")
!MESSAGE "Vttest - Win32 (ALPHA) W2WDebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Vttest - Win32 (ALPHA) W2WRel" (based on "Win32 (ALPHA) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/Test/vtxt/vttest", KNPAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\A2ADebug"
# PROP Intermediate_Dir ".\A2ADebug"
# PROP Ignore_Export_Lib 0
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
MTL=midl.exe
# ADD MTL /mktyplib203
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ole32.lib /nologo /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\A2ARel"
# PROP Intermediate_Dir ".\A2ARel"
# PROP Ignore_Export_Lib 0
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
MTL=midl.exe
# ADD MTL /mktyplib203
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ole32.lib /nologo /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\W2WDebug"
# PROP BASE Intermediate_Dir ".\W2WDebug"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\W2WDebug"
# PROP Intermediate_Dir ".\W2WDebug"
# PROP Ignore_Export_Lib 0
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_UNICODE" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
MTL=midl.exe
# ADD MTL /mktyplib203
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ole32.lib uuid.lib /nologo /entry:"wWinMainCRTStartup" /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ole32.lib /nologo /entry:"wWinMainCRTStartup" /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\W2WRel"
# PROP BASE Intermediate_Dir ".\W2WRel"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\W2WRel"
# PROP Intermediate_Dir ".\W2WRel"
# PROP Ignore_Export_Lib 0
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_UNICODE" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
MTL=midl.exe
# ADD MTL /mktyplib203
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ole32.lib uuid.lib /nologo /entry:"wWinMainCRTStartup" /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ole32.lib /nologo /entry:"wWinMainCRTStartup" /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Vttest__"
# PROP BASE Intermediate_Dir ".\Vttest__"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\A2ADebug"
# PROP Intermediate_Dir ".\A2ADebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:ALPHA
# ADD LINK32 winmm.lib ole32.lib /nologo /subsystem:windows /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Vttest_0"
# PROP BASE Intermediate_Dir ".\Vttest_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
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
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 winmm.lib  ole32.lib /nologo /subsystem:windows /machine:ALPHA

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Vttest_5"
# PROP BASE Intermediate_Dir ".\Vttest_5"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\W2WDebug"
# PROP Intermediate_Dir ".\W2WDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:ALPHA
# ADD LINK32 winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:ALPHA
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Vttest_6"
# PROP BASE Intermediate_Dir ".\Vttest_6"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
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
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 winmm.lib  ole32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:ALPHA
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Vttest - Win32 A2ADebug"
# Name "Vttest - Win32 A2ARel"
# Name "Vttest - Win32 W2WDebug"
# Name "Vttest - Win32 W2WRel"
# Name "Vttest - Win32 (ALPHA) A2ADebug"
# Name "Vttest - Win32 (ALPHA) A2ARel"
# Name "Vttest - Win32 (ALPHA) W2WDebug"
# Name "Vttest - Win32 (ALPHA) W2WRel"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\GUIDSEG.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

# SUBTRACT CPP /Yu

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

# SUBTRACT BASE CPP /Yu
# SUBTRACT CPP /Yu

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\README.TXT
# End Source File
# Begin Source File

SOURCE=.\STDAFX.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TABDLGS.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VTNOT.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VTOBJ.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VTPICK.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vttesdlg.cpp

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VTTEST.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VTTEST.RC
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\STDAFX.H
# End Source File
# Begin Source File

SOURCE=.\TABDLGS.H
# End Source File
# Begin Source File

SOURCE=.\VTOBJ.H
# End Source File
# Begin Source File

SOURCE=.\VTPICK.H
# End Source File
# Begin Source File

SOURCE=.\VTTESDLG.H
# End Source File
# Begin Source File

SOURCE=.\VTTEST.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\VTTEST.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\VTTEST.RC2
# End Source File
# End Group
# End Target
# End Project
