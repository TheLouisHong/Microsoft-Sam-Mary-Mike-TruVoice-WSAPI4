# Microsoft Developer Studio Project File - Name="Audiosd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=Audiosd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Audiosd.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Audiosd.mak" CFG="Audiosd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Audiosd - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Audiosd - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Audiosd - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "Audiosd - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/Demos/AUDIOSD", AONAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "Audiosd - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /Fr
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /map /machine:I386

!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /Fr
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Audiosd_"
# PROP BASE Intermediate_Dir ".\Audiosd_"
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
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /MTd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Audiosd0"
# PROP BASE Intermediate_Dir ".\Audiosd0"
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
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /dll /machine:ALPHA

!ENDIF 

# Begin Target

# Name "Audiosd - Win32 Release"
# Name "Audiosd - Win32 Debug"
# Name "Audiosd - Win32 (ALPHA) Debug"
# Name "Audiosd - Win32 (ALPHA) Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\AUDIOIN.CPP

!IF  "$(CFG)" == "Audiosd - Win32 Release"

!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

DEP_CPP_AUDIO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

DEP_CPP_AUDIO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AUDIOOUT.CPP

!IF  "$(CFG)" == "Audiosd - Win32 Release"

!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

DEP_CPP_AUDIOO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

DEP_CPP_AUDIOO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CF.CPP

!IF  "$(CFG)" == "Audiosd - Win32 Release"

!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

DEP_CPP_CF_CP=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

DEP_CPP_CF_CP=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CTOOLS.CPP

!IF  "$(CFG)" == "Audiosd - Win32 Release"

!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

DEP_CPP_CTOOL=\
	".\CTOOLS.H"\
	

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

DEP_CPP_CTOOL=\
	".\CTOOLS.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GUIDSEG.CPP

!IF  "$(CFG)" == "Audiosd - Win32 Release"

!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

DEP_CPP_GUIDS=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

DEP_CPP_GUIDS=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPEECH.DEF
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\AUDIO.H
# End Source File
# Begin Source File

SOURCE=.\AUDIOINC.H
# End Source File
# Begin Source File

SOURCE=.\CTOOLS.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
