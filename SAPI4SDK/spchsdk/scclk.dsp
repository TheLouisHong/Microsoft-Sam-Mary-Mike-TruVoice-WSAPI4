# Microsoft Developer Studio Project File - Name="srclk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (PowerPC) Application" 0x0701
# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

CFG=srclk - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "srclk.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "srclk.mak" CFG="srclk - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "srclk - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "srclk - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "srclk - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "srclk - Win32 (ALPHA) Release" (based on "Win32 (ALPHA) Application")
!MESSAGE "srclk - Win32 (PPC) Debug" (based on "Win32 (PowerPC) Application")
!MESSAGE "srclk - Win32 (PPC) Release" (based on "Win32 (PowerPC) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/Demos/clkdemo", VONAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "srclk - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /Fr
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /Fr
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /incremental:no /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\srclk___"
# PROP BASE Intermediate_Dir ".\srclk___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\AlphaDbg"
# PROP Intermediate_Dir ".\AlphaDbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\srclk__0"
# PROP BASE Intermediate_Dir ".\srclk__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\AlphaRel"
# PROP Intermediate_Dir ".\AlphaRel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /alpha
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:ALPHA

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\srclk___"
# PROP BASE Intermediate_Dir ".\srclk___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\ppcdbg"
# PROP Intermediate_Dir ".\ppcdbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /PPC32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /PPC32
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:IX86 /machine:PPC
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:IX86 /machine:PPC

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\srclk__0"
# PROP BASE Intermediate_Dir ".\srclk__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\ppcrel"
# PROP Intermediate_Dir ".\ppcrel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /PPC32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /PPC32
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:IX86 /machine:PPC
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:IX86 /machine:PPC

!ENDIF 

# Begin Target

# Name "srclk - Win32 Release"
# Name "srclk - Win32 Debug"
# Name "srclk - Win32 (ALPHA) Debug"
# Name "srclk - Win32 (ALPHA) Release"
# Name "srclk - Win32 (PPC) Debug"
# Name "srclk - Win32 (PPC) Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\clkhands.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\init.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\showdib.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\srclk.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_SRCLK=\
	".\clkhands.h"\
	".\showdib.h"\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_SRCLK=\
	".\clkhands.h"\
	".\showdib.h"\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_SRCLK=\
	".\clkhands.h"\
	".\showdib.h"\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_SRCLK=\
	".\clkhands.h"\
	".\showdib.h"\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\srclk.rc
# End Source File
# Begin Source File

SOURCE=.\srclkdlg.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vcmd.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\clkhands.h
# End Source File
# Begin Source File

SOURCE=.\showdib.h
# End Source File
# Begin Source File

SOURCE=.\Srclk.h
# End Source File
# Begin Source File

SOURCE=.\srclkdlg.h
# End Source File
# Begin Source File

SOURCE=.\vcmd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\srclk.ico
# End Source File
# End Group
# End Target
# End Project
