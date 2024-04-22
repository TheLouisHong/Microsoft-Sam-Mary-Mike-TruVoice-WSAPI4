# Microsoft Developer Studio Project File - Name="SpchWrap" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SpchWrap - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SpchWrap.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SpchWrap.mak" CFG="SpchWrap - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SpchWrap - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "SpchWrap - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "SpchWrap - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SpchWrap - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/SpchWrap", PUABAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AlphaRel"
# PROP BASE Intermediate_Dir "AlphaRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AlphaRel"
# PROP Intermediate_Dir "AlphaRel"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AlphaDbg"
# PROP BASE Intermediate_Dir "AlphaDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "z:\lib"
# PROP Intermediate_Dir "AlphaDbg"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /MDd /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SpchWrap"
# PROP BASE Intermediate_Dir "SpchWrap"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SpchWra0"
# PROP BASE Intermediate_Dir "SpchWra0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "SpchWrap - Win32 (ALPHA) Release"
# Name "SpchWrap - Win32 (ALPHA) Debug"
# Name "SpchWrap - Win32 Release"
# Name "SpchWrap - Win32 Debug"
# Begin Source File

SOURCE=.\ctools.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_CTOOL=\
	".\SpchWrap.h"\
	
NODEP_CPP_CTOOL=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_CTOOL=\
	".\SpchWrap.h"\
	
NODEP_CPP_CTOOL=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\link.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_LINK_=\
	".\link.h"\
	
NODEP_CPP_LINK_=\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_LINK_=\
	".\link.h"\
	
NODEP_CPP_LINK_=\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LowSR.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_LOWSR=\
	".\SpchWrap.h"\
	
NODEP_CPP_LOWSR=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_LOWSR=\
	".\SpchWrap.h"\
	
NODEP_CPP_LOWSR=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LowTTS.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_LOWTT=\
	".\SpchWrap.h"\
	
NODEP_CPP_LOWTT=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_LOWTT=\
	".\SpchWrap.h"\
	
NODEP_CPP_LOWTT=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\STools.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_STOOL=\
	".\SpchWrap.h"\
	
NODEP_CPP_STOOL=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_STOOL=\
	".\SpchWrap.h"\
	
NODEP_CPP_STOOL=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Teleph.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_TELEP=\
	".\SpchWrap.h"\
	
NODEP_CPP_TELEP=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_TELEP=\
	".\SpchWrap.h"\
	
NODEP_CPP_TELEP=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TelFrame.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_TELFR=\
	".\SpchWrap.h"\
	
NODEP_CPP_TELFR=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_TELFR=\
	".\SpchWrap.h"\
	
NODEP_CPP_TELFR=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Telobj.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_TELOB=\
	".\link.h"\
	".\SpchWrap.h"\
	
NODEP_CPP_TELOB=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_TELOB=\
	".\link.h"\
	".\SpchWrap.h"\
	
NODEP_CPP_TELOB=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VCmd.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_VCMD_=\
	".\SpchWrap.h"\
	
NODEP_CPP_VCMD_=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_VCMD_=\
	".\SpchWrap.h"\
	
NODEP_CPP_VCMD_=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VDct.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_VDCT_=\
	".\SpchWrap.h"\
	
NODEP_CPP_VDCT_=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_VDCT_=\
	".\SpchWrap.h"\
	
NODEP_CPP_VDCT_=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VTxt.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_VTXT_=\
	".\SpchWrap.h"\
	
NODEP_CPP_VTXT_=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_VTXT_=\
	".\SpchWrap.h"\
	
NODEP_CPP_VTXT_=\
	".\pchtel.h"\
	".\peech.h"\
	

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
