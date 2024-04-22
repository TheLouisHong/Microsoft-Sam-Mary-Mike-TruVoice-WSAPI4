# Microsoft Developer Studio Project File - Name="Srtest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

CFG=Srtest - Win32 (ALPHA) W2WRel
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Srtest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Srtest.mak" CFG="Srtest - Win32 (ALPHA) W2WRel"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Srtest - Win32 A2ADebug" (based on "Win32 (x86) Application")
!MESSAGE "Srtest - Win32 A2ARel" (based on "Win32 (x86) Application")
!MESSAGE "Srtest - Win32 W2WRel" (based on "Win32 (x86) Application")
!MESSAGE "Srtest - Win32 W2WDebug" (based on "Win32 (x86) Application")
!MESSAGE "Srtest - Win32 (ALPHA) A2ADebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Srtest - Win32 (ALPHA) A2ARel" (based on "Win32 (ALPHA) Application")
!MESSAGE "Srtest - Win32 (ALPHA) W2WDebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Srtest - Win32 (ALPHA) W2WRel" (based on "Win32 (ALPHA) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/Test/sr/srtest", DVOAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\A2A_Debu"
# PROP BASE Intermediate_Dir ".\A2A_Debu"
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\A2ADebug"
# PROP Intermediate_Dir ".\A2ADebug"
# PROP Ignore_Export_Lib 0
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_S_UNICODE" /FR /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_CRT_MAP_ALLOC" /Fr /FD /c
MTL=midl.exe
# ADD MTL /mktyplib203
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ole32.lib winmm.lib /nologo /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 winmm.lib ole32.lib /nologo /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\A2A_Reta"
# PROP BASE Intermediate_Dir ".\A2A_Reta"
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\A2ARel"
# PROP Intermediate_Dir ".\A2ARel"
# PROP Ignore_Export_Lib 0
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /c
# ADD CPP /nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FD /c
# SUBTRACT CPP /Fr
MTL=midl.exe
# ADD MTL /mktyplib203
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ole32.lib winmm.lib /nologo /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 winmm.lib ole32.lib /nologo /incremental:no /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\W2WRel"
# PROP BASE Intermediate_Dir ".\W2WRel"
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\W2WRel"
# PROP Intermediate_Dir ".\W2WRel"
# PROP Ignore_Export_Lib 0
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_S_UNICODE" /Yu"stdafx.h" /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
MTL=midl.exe
# ADD MTL /mktyplib203
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uuid.lib ole32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\W2WDebug"
# PROP BASE Intermediate_Dir ".\W2WDebug"
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\W2WDebug"
# PROP Intermediate_Dir ".\W2WDebug"
# PROP Ignore_Export_Lib 0
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_S_UNICODE" /FR /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /FD /c
# SUBTRACT CPP /Fr
MTL=midl.exe
# ADD MTL /mktyplib203
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ole32.lib winmm.lib /nologo /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup" /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Srtest_1"
# PROP BASE Intermediate_Dir ".\Srtest_1"
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
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:ALPHA
# ADD LINK32 winmm.lib ole32.lib /nologo /subsystem:windows /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Srtest_2"
# PROP BASE Intermediate_Dir ".\Srtest_2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\A2ARel"
# PROP Intermediate_Dir ".\A2ARel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:ALPHA
# ADD LINK32 winmm.lib ole32.lib /nologo /subsystem:windows /machine:ALPHA
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Srtest_5"
# PROP BASE Intermediate_Dir ".\Srtest_5"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
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
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /Yu"stdafx.h" /FD /c
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

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Srtest__"
# PROP BASE Intermediate_Dir "Srtest__"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\W2WRel"
# PROP Intermediate_Dir ".\W2WRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /Yu"stdafx.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ole32.lib /nologo /subsystem:windows /machine:ALPHA
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:ALPHA
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "Srtest - Win32 A2ADebug"
# Name "Srtest - Win32 A2ARel"
# Name "Srtest - Win32 W2WRel"
# Name "Srtest - Win32 W2WDebug"
# Name "Srtest - Win32 (ALPHA) A2ADebug"
# Name "Srtest - Win32 (ALPHA) A2ARel"
# Name "Srtest - Win32 (ALPHA) W2WDebug"
# Name "Srtest - Win32 (ALPHA) W2WRel"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\DLGFINDM.CPP

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0 /Yu"stdafx.h"
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GRAMFUNC.CPP

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0 /Yu"stdafx.h"
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GUIDSEG.CPP

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

# SUBTRACT BASE CPP /Yu
# SUBTRACT CPP /Yu

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Gt0
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Srdlg.cpp

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0 /Yu"stdafx.h"
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Srfunc.cpp

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0 /Yu"stdafx.h"
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SRTEST.CPP

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0 /Yu"stdafx.h"
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Srtest.rc
# End Source File
# Begin Source File

SOURCE=.\STDAFX.CPP

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yc

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0 /Yc
# ADD CPP /Gt0 /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tabisrce.cpp

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

# ADD BASE CPP /Yu"stdafx.h"
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0 /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD BASE CPP /Gt0 /Yu"stdafx.h"
# ADD CPP /Gt0 /Yu"stdafx.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\DLGFINDM.H
# End Source File
# Begin Source File

SOURCE=.\GRAMFUNC.H
# End Source File
# Begin Source File

SOURCE=.\SRDLG.H
# End Source File
# Begin Source File

SOURCE=.\SRFUNC.H
# End Source File
# Begin Source File

SOURCE=.\SRTEST.H
# End Source File
# Begin Source File

SOURCE=.\STDAFX.H
# End Source File
# Begin Source File

SOURCE=.\TABISRCE.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\srtest.ico
# End Source File
# Begin Source File

SOURCE=.\RES\srtest.rc2
# End Source File
# End Group
# End Target
# End Project
