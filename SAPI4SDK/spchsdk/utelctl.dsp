# Microsoft Developer Studio Project File - Name="TelCtl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=TelCtl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TelCtl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TelCtl.mak" CFG="TelCtl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TelCtl - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "TelCtl - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "TelCtl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TelCtl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/Demos/TelCtl", PWABAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AlphaRel"
# PROP BASE Intermediate_Dir "AlphaRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AlphaRel"
# PROP Intermediate_Dir "AlphaRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
CPP=cl.exe
# ADD BASE CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib spchwrap.lib tapi32.lib /nologo /subsystem:windows /dll /machine:ALPHA

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AlphaDbg"
# PROP BASE Intermediate_Dir "AlphaDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AlphaDbg"
# PROP Intermediate_Dir "AlphaDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /MTd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /MDd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib spchwrap.lib tapi32.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TelCtl__"
# PROP BASE Intermediate_Dir "TelCtl__"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "c:\program files\microsoft speech sdk\tel"
# PROP Intermediate_Dir "WinDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 z:\api\spchwrap\alphadbg\spchwrap.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /pdbtype:sept
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib spchwrap.lib tapi32.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TelCtl_0"
# PROP BASE Intermediate_Dir "TelCtl_0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 z:\api\spchwrap\alphadbg\spchwrap.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:IX86
# ADD LINK32 odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib spchwrap.lib tapi32.lib /nologo /subsystem:windows /dll /machine:IX86

!ENDIF 

# Begin Target

# Name "TelCtl - Win32 (ALPHA) Release"
# Name "TelCtl - Win32 (ALPHA) Debug"
# Name "TelCtl - Win32 Debug"
# Name "TelCtl - Win32 Release"
# Begin Source File

SOURCE=.\CF.CPP

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_CF_CP=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_CF_CP=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\date.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_DATE_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_DATE_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\date.tc
# End Source File
# Begin Source File

SOURCE=.\grammar.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_GRAMM=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_GRAMM=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\grammar.tc
# End Source File
# Begin Source File

SOURCE=.\name.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_NAME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_NAME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\name.tc
# End Source File
# Begin Source File

SOURCE=.\phonenum.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_PHONE=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_PHONE=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phonenum.tc
# End Source File
# Begin Source File

SOURCE=.\record.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_RECOR=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_RECOR=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\record.tc
# End Source File
# Begin Source File

SOURCE=.\spelling.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_SPELL=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_SPELL=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\spelling.tc
# End Source File
# Begin Source File

SOURCE=.\telctl.def
# End Source File
# Begin Source File

SOURCE=.\TelCtl.h
# End Source File
# Begin Source File

SOURCE=.\TelCtl.rc
# End Source File
# Begin Source File

SOURCE=.\time.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_TIME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_TIME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\time.tc
# End Source File
# Begin Source File

SOURCE=.\xtension.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_XTENS=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_XTENS=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\xtension.tc
# End Source File
# Begin Source File

SOURCE=.\YesNo.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_YESNO=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_YESNO=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\yesno.tc
# End Source File
# End Target
# End Project
