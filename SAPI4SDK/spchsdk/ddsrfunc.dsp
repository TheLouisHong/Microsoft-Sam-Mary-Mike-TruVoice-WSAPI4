# Microsoft Developer Studio Project File - Name="SRFunc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

CFG=SRFunc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "srfunc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "srfunc.mak" CFG="SRFunc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SRFunc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SRFunc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "SRFunc - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/Test/sr/SRFunc", EERCAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "SRFunc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib spchwrap.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib spchwrap.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SRFunc__"
# PROP BASE Intermediate_Dir "SRFunc__"
# PROP BASE Ignore_Export_Lib 0
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
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib spchwrap.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 winmm.lib spchwrap.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:ALPHA

!ENDIF 

# Begin Target

# Name "SRFunc - Win32 Release"
# Name "SRFunc - Win32 Debug"
# Name "SRFunc - Win32 (ALPHA) Release"
# Begin Source File

SOURCE=.\Frame.cpp

!IF  "$(CFG)" == "SRFunc - Win32 Release"

!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

DEP_CPP_FRAME=\
	".\Frame.h"\
	
NODEP_CPP_FRAME=\
	".\pchtel.h"\
	".\pchwrap.h"\
	".\peech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Frame.h
# End Source File
# Begin Source File

SOURCE=.\Info.cpp

!IF  "$(CFG)" == "SRFunc - Win32 Release"

!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

DEP_CPP_INFO_=\
	".\Frame.h"\
	
NODEP_CPP_INFO_=\
	".\pchtel.h"\
	".\pchwrap.h"\
	".\peech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Info.rc
# End Source File
# Begin Source File

SOURCE=.\Test1.cpp

!IF  "$(CFG)" == "SRFunc - Win32 Release"

!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

DEP_CPP_TEST1=\
	".\Frame.h"\
	
NODEP_CPP_TEST1=\
	".\pchtel.h"\
	".\pchwrap.h"\
	".\peech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Test2.cpp

!IF  "$(CFG)" == "SRFunc - Win32 Release"

!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

DEP_CPP_TEST2=\
	".\Frame.h"\
	
NODEP_CPP_TEST2=\
	".\pchtel.h"\
	".\pchwrap.h"\
	".\peech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Test3.cpp

!IF  "$(CFG)" == "SRFunc - Win32 Release"

!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

!ENDIF 

# End Source File
# End Target
# End Project
