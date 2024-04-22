# Microsoft Developer Studio Project File - Name="DctPad" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

CFG=DctPad - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DctPad.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DctPad.mak" CFG="DctPad - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DctPad - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "DctPad - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "DctPad - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DctPad - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/apps/DctPad", WVYAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "DctPad - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DctPad__"
# PROP BASE Intermediate_Dir "DctPad__"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AlphaDebug"
# PROP Intermediate_Dir "AlphaDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib /nologo /subsystem:windows /debug /machine:ALPHA
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "DctPad - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DctPad_0"
# PROP BASE Intermediate_Dir "DctPad_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AlphaRel"
# PROP Intermediate_Dir "AlphaRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /alpha
# ADD MTL /nologo /D "NDEBUG" /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /Zi /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib /nologo /subsystem:windows /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "DctPad - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DctPad__"
# PROP BASE Intermediate_Dir "DctPad__"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /alpha
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /alpha
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:IX86
# ADD LINK32 odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib /nologo /subsystem:windows /debug /machine:IX86

!ELSEIF  "$(CFG)" == "DctPad - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DctPad_0"
# PROP BASE Intermediate_Dir "DctPad_0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /alpha
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:IX86 /pdbtype:sept
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 comctl32.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:IX86 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "DctPad - Win32 (ALPHA) Debug"
# Name "DctPad - Win32 (ALPHA) Release"
# Name "DctPad - Win32 Release"
# Name "DctPad - Win32 Debug"
# Begin Source File

SOURCE=.\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\cmnhdr.cpp

!IF  "$(CFG)" == "DctPad - Win32 (ALPHA) Debug"

DEP_CPP_CMNHD=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\zmouse.h"\
	
NODEP_CPP_CMNHD=\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "DctPad - Win32 (ALPHA) Release"

DEP_CPP_CMNHD=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\zmouse.h"\
	
NODEP_CPP_CMNHD=\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "DctPad - Win32 Release"

!ELSEIF  "$(CFG)" == "DctPad - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Command.txt
# End Source File
# Begin Source File

SOURCE=.\dctpad.rc
# End Source File
# Begin Source File

SOURCE=.\DictOff.ico
# End Source File
# Begin Source File

SOURCE=.\DictOn.ico
# End Source File
# Begin Source File

SOURCE=.\dicton1.ico
# End Source File
# Begin Source File

SOURCE=.\docicon.ico
# End Source File
# Begin Source File

SOURCE=.\guidseg.cpp

!IF  "$(CFG)" == "DctPad - Win32 (ALPHA) Debug"

NODEP_CPP_GUIDS=\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "DctPad - Win32 (ALPHA) Release"

NODEP_CPP_GUIDS=\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "DctPad - Win32 Release"

!ELSEIF  "$(CFG)" == "DctPad - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\interm.cpp

!IF  "$(CFG)" == "DctPad - Win32 (ALPHA) Debug"

DEP_CPP_INTER=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	
NODEP_CPP_INTER=\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "DctPad - Win32 (ALPHA) Release"

DEP_CPP_INTER=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	
NODEP_CPP_INTER=\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "DctPad - Win32 Release"

!ELSEIF  "$(CFG)" == "DctPad - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\msdict.ico
# End Source File
# Begin Source File

SOURCE=.\REditCB.cpp

!IF  "$(CFG)" == "DctPad - Win32 (ALPHA) Debug"

DEP_CPP_REDIT=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	
NODEP_CPP_REDIT=\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "DctPad - Win32 (ALPHA) Release"

DEP_CPP_REDIT=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	
NODEP_CPP_REDIT=\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "DctPad - Win32 Release"

!ELSEIF  "$(CFG)" == "DctPad - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
