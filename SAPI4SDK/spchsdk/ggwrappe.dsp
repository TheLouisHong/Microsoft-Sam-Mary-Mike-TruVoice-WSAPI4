# Microsoft Developer Studio Project File - Name="Wrapper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=Wrapper - Win32 Alpha Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Wrapper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Wrapper.mak" CFG="Wrapper - Win32 Alpha Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Wrapper - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Wrapper - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Wrapper - Win32 Alpha Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "Wrapper - Win32 Alpha Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/Wrapper", DVCAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "Wrapper - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /FR /Yu"stdafx.h" /FD /c
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\.\Debug
TargetPath=.\Debug\Wrapper.dll
InputPath=.\Debug\Wrapper.dll
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "NDEBUG" /D "_ATL_MIN_CRT" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /dll /map /machine:I386
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\.\Release
TargetPath=.\Release\Wrapper.dll
InputPath=.\Release\Wrapper.dll
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WrapCgra"
# PROP BASE Intermediate_Dir "WrapCgra"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AlphaDebug"
# PROP Intermediate_Dir "AlphaDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /MTd /c
# ADD CPP /nologo /Gt0 /W3 /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /MTd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:".\Debug/WrapCgrm.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:".\AlphaDebug/Wrapper.dll" /pdbtype:sept
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\AlphaDebug
TargetPath=.\AlphaDebug\Wrapper.dll
InputPath=.\AlphaDebug\Wrapper.dll
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WrapCgr0"
# PROP BASE Intermediate_Dir "WrapCgr0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AlphaRelease"
# PROP Intermediate_Dir "AlphaRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
CPP=cl.exe
# ADD BASE CPP /nologo /MT /Gt0 /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /map /machine:ALPHA /out:".\Release/WrapCgrm.dll"
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /dll /map /machine:ALPHA /out:".\AlphaRelease/Wrapper.dll"
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\AlphaRelease
TargetPath=.\AlphaRelease\Wrapper.dll
InputPath=.\AlphaRelease\Wrapper.dll
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Wrapper - Win32 Debug"
# Name "Wrapper - Win32 Release"
# Name "Wrapper - Win32 Alpha Debug"
# Name "Wrapper - Win32 Alpha Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Wrapper - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Alpha Debug"

DEP_CPP_STDAF=\
	".\StdAfx.h"\
	
# ADD BASE CPP /Gt0 /Yc"stdafx.h"
# ADD CPP /Gt0 /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Alpha Release"

DEP_CPP_STDAF=\
	".\StdAfx.h"\
	
# ADD BASE CPP /Gt0 /Yc"stdafx.h"
# ADD CPP /Gt0 /Yc"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WrapObj.cpp

!IF  "$(CFG)" == "Wrapper - Win32 Debug"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Release"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Wrapper.cpp

!IF  "$(CFG)" == "Wrapper - Win32 Debug"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Release"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Wrapper.def
# End Source File
# Begin Source File

SOURCE=.\Wrapper.rc

!IF  "$(CFG)" == "Wrapper - Win32 Debug"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Release"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "Wrapper - Win32 Alpha Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WrapObj.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\WrapObj.rgs
# End Source File
# End Target
# End Project
