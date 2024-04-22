# Microsoft Developer Studio Project File - Name="Ttsfake" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=Ttsfake - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ttsfake.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ttsfake.mak" CFG="Ttsfake - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ttsfake - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Ttsfake - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Ttsfake - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "Ttsfake - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/Demos/ttsfake", AWNAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "z:\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /Fr /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "z:\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "z:\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /Fr /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "z:\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Ttsfake_"
# PROP BASE Intermediate_Dir ".\Ttsfake_"
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
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /MTd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "z:\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /MTd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "z:\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Ttsfake0"
# PROP BASE Intermediate_Dir ".\Ttsfake0"
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
# ADD BASE CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /I "z:\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "z:\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:ALPHA

!ENDIF 

# Begin Target

# Name "Ttsfake - Win32 Release"
# Name "Ttsfake - Win32 Debug"
# Name "Ttsfake - Win32 (ALPHA) Debug"
# Name "Ttsfake - Win32 (ALPHA) Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\CTOOLS.CPP

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FAKETTS.CPP

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

DEP_CPP_FAKET=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

DEP_CPP_FAKET=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FAKETTS.RC
# End Source File
# Begin Source File

SOURCE=.\FAKETTSC.CPP

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

DEP_CPP_FAKETT=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

DEP_CPP_FAKETT=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FAKETTSE.CPP

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

DEP_CPP_FAKETTS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

DEP_CPP_FAKETTS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GUIDSEG.CPP

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

DEP_CPP_GUIDS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

DEP_CPP_GUIDS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	
# ADD BASE CPP /Gt0
# ADD CPP /Gt0
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TTSFAKE.DEF
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\ctools.h
# End Source File
# Begin Source File

SOURCE=.\FAKETTS.H
# End Source File
# Begin Source File

SOURCE=.\FAKETTSE.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
