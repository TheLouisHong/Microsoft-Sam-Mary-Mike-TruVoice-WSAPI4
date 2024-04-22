# Microsoft Developer Studio Project File - Name="feedback" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (PowerPC) Application" 0x0701
# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

CFG=feedback - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "feedback.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "feedback.mak" CFG="feedback - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "feedback - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "feedback - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "feedback - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "feedback - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "feedback - Win32 (PPC) Debug" (based on\
 "Win32 (PowerPC) Application")
!MESSAGE "feedback - Win32 (PPC) Release" (based on\
 "Win32 (PowerPC) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/stg/API/Demos/feedback", SQNAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "feedback - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yc /FD /c
# SUBTRACT CPP /Fr
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
CPP=cl.exe
# ADD BASE CPP /nologo /ML /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yc /FD /c
# SUBTRACT CPP /Fr
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\feedback"
# PROP BASE Intermediate_Dir ".\feedback"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\AlphaDbg"
# PROP Intermediate_Dir ".\AlphaDbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /MDd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /MDd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:ALPHA
# ADD LINK32 ole32.lib uuid.lib /nologo /subsystem:windows /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\feedbac0"
# PROP BASE Intermediate_Dir ".\feedbac0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\AlphaRel"
# PROP Intermediate_Dir ".\AlphaRel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /alpha
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 ole32.lib uuid.lib /nologo /subsystem:windows /machine:ALPHA

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\feedback"
# PROP BASE Intermediate_Dir ".\feedback"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\ppcdbg"
# PROP Intermediate_Dir ".\ppcdbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /PPC32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /PPC32
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /MDd /c
# ADD CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /MDd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:IX86 /machine:PPC
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:IX86 /machine:PPC

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\feedbac0"
# PROP BASE Intermediate_Dir ".\feedbac0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\ppcrel"
# PROP Intermediate_Dir ".\ppcrel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /PPC32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /PPC32
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /MD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /MD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:IX86 /machine:PPC
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:IX86 /machine:PPC

!ENDIF 

# Begin Target

# Name "feedback - Win32 Release"
# Name "feedback - Win32 Debug"
# Name "feedback - Win32 (ALPHA) Debug"
# Name "feedback - Win32 (ALPHA) Release"
# Name "feedback - Win32 (PPC) Debug"
# Name "feedback - Win32 (PPC) Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\dialogs.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_DIALO=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_DIALO=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_DIALO=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_DIALO=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD CPP /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fdbkole.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_FDBKO=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_FDBKO=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_FDBKO=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_FDBKO=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD CPP /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\feedback.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_FEEDB=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_FEEDB=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_FEEDB=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_FEEDB=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD CPP /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\feedback.rc
# End Source File
# Begin Source File

SOURCE=.\guid.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"speech.h"\
	
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"speech.h"\
	
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Icons.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_ICONS=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_ICONS=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_ICONS=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_ICONS=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD CPP /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	
# ADD CPP /Yc"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tray.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_TRAY_=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_TRAY_=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_TRAY_=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_TRAY_=\
	".\feedback.h"\
	".\icons.h"\
	".\stdafx.h"\
	".\tray.h"\
	{$(INCLUDE)}"speech.h"\
	
# ADD CPP /Yu"stdafx.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\feedback.h
# End Source File
# Begin Source File

SOURCE=.\icons.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\tray.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\feedback.ico
# End Source File
# Begin Source File

SOURCE=.\res\feedback.rc2
# End Source File
# Begin Source File

SOURCE=.\res\listen16.ico
# End Source File
# Begin Source File

SOURCE=.\res\listen32.ico
# End Source File
# Begin Source File

SOURCE=.\res\notrec16.ico
# End Source File
# Begin Source File

SOURCE=.\res\notrec32.ico
# End Source File
# Begin Source File

SOURCE=.\res\off16.ico
# End Source File
# Begin Source File

SOURCE=.\res\off32.ico
# End Source File
# Begin Source File

SOURCE=.\res\recog16.ico
# End Source File
# Begin Source File

SOURCE=.\res\recog32.ico
# End Source File
# Begin Source File

SOURCE=.\res\sleep16.ico
# End Source File
# Begin Source File

SOURCE=.\res\sleep32.ico
# End Source File
# Begin Source File

SOURCE=.\res\think16.ico
# End Source File
# Begin Source File

SOURCE=.\res\think32.ico
# End Source File
# End Group
# End Target
# End Project
