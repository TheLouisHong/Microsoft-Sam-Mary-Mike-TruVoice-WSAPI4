# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (PPC) Application" 0x0701
# TARGTYPE "Win32 (MIPS) Application" 0x0501
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

!IF "$(CFG)" == ""
CFG=feedback - Win32 MIPS Debug
!MESSAGE No configuration specified.  Defaulting to feedback - Win32 MIPS\
 Debug.
!ENDIF 

!IF "$(CFG)" != "feedback - Win32 Release" && "$(CFG)" !=\
 "feedback - Win32 Debug" && "$(CFG)" != "feedback - Win32 (ALPHA) Debug" &&\
 "$(CFG)" != "feedback - Win32 (ALPHA) Release" && "$(CFG)" !=\
 "feedback - Win32 (PPC) Debug" && "$(CFG)" != "feedback - Win32 (PPC) Release"\
 && "$(CFG)" != "feedback - Win32 MIPS Debug" && "$(CFG)" !=\
 "feedback - Win32 MIPS Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "feedback.mak" CFG="feedback - Win32 MIPS Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "feedback - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "feedback - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "feedback - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "feedback - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "feedback - Win32 (PPC) Debug" (based on "Win32 (PPC) Application")
!MESSAGE "feedback - Win32 (PPC) Release" (based on "Win32 (PPC) Application")
!MESSAGE "feedback - Win32 MIPS Debug" (based on "Win32 (MIPS) Application")
!MESSAGE "feedback - Win32 MIPS Release" (based on "Win32 (MIPS) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "feedback - Win32 Debug"

!IF  "$(CFG)" == "feedback - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\feedback.exe"

CLEAN : 
	-@erase ".\WinRel\feedback.exe"
	-@erase ".\WinRel\tray.obj"
	-@erase ".\WinRel\feedback.pch"
	-@erase ".\WinRel\fdbkole.obj"
	-@erase ".\WinRel\feedback.obj"
	-@erase ".\WinRel\Icons.obj"
	-@erase ".\WinRel\guid.obj"
	-@erase ".\WinRel\dialogs.obj"
	-@erase ".\WinRel\stdafx.obj"
	-@erase ".\WinRel\feedback.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yc /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yc /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/feedback.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/feedback.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/feedback.pdb" /machine:I386 /out:"$(OUTDIR)/feedback.exe" 
LINK32_OBJS= \
	"$(INTDIR)/tray.obj" \
	"$(INTDIR)/fdbkole.obj" \
	"$(INTDIR)/feedback.obj" \
	"$(INTDIR)/Icons.obj" \
	"$(INTDIR)/guid.obj" \
	"$(INTDIR)/dialogs.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/feedback.res"

"$(OUTDIR)\feedback.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\feedback.exe"

CLEAN : 
	-@erase ".\WinDebug\vc40.pdb"
	-@erase ".\WinDebug\feedback.pch"
	-@erase ".\WinDebug\vc40.idb"
	-@erase ".\WinDebug\feedback.exe"
	-@erase ".\WinDebug\tray.obj"
	-@erase ".\WinDebug\fdbkole.obj"
	-@erase ".\WinDebug\dialogs.obj"
	-@erase ".\WinDebug\feedback.obj"
	-@erase ".\WinDebug\guid.obj"
	-@erase ".\WinDebug\Icons.obj"
	-@erase ".\WinDebug\stdafx.obj"
	-@erase ".\WinDebug\feedback.res"
	-@erase ".\WinDebug\feedback.ilk"
	-@erase ".\WinDebug\feedback.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /ML /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yc /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yc /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/feedback.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/feedback.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/feedback.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/feedback.exe" 
LINK32_OBJS= \
	"$(INTDIR)/tray.obj" \
	"$(INTDIR)/fdbkole.obj" \
	"$(INTDIR)/dialogs.obj" \
	"$(INTDIR)/feedback.obj" \
	"$(INTDIR)/guid.obj" \
	"$(INTDIR)/Icons.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/feedback.res"

"$(OUTDIR)\feedback.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "feedback"
# PROP BASE Intermediate_Dir "feedback"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AlphaDbg"
# PROP Intermediate_Dir "AlphaDbg"
# PROP Target_Dir ""
OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\feedback.exe"

CLEAN : 
	-@erase ".\AlphaDbg\vc40.pdb"
	-@erase ".\AlphaDbg\feedback.exe"
	-@erase ".\AlphaDbg\fdbkole.obj"
	-@erase ".\AlphaDbg\dialogs.obj"
	-@erase ".\AlphaDbg\tray.obj"
	-@erase ".\AlphaDbg\feedback.obj"
	-@erase ".\AlphaDbg\Icons.obj"
	-@erase ".\AlphaDbg\stdafx.obj"
	-@erase ".\AlphaDbg\guid.obj"
	-@erase ".\AlphaDbg\feedback.res"
	-@erase ".\AlphaDbg\feedback.ilk"
	-@erase ".\AlphaDbg\feedback.pdb"

CPP=cl.exe
# ADD BASE CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\AlphaDbg/
CPP_SBRS=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /alpha
MTL_PROJ=/nologo /D "_DEBUG" /alpha 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/feedback.res"  /d "_DEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/feedback.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 ole32.lib uuid.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=ole32.lib uuid.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/feedback.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)/feedback.exe" 
LINK32_OBJS= \
	"$(INTDIR)/fdbkole.obj" \
	"$(INTDIR)/dialogs.obj" \
	"$(INTDIR)/tray.obj" \
	"$(INTDIR)/feedback.obj" \
	"$(INTDIR)/Icons.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/guid.obj" \
	"$(INTDIR)/feedback.res"

"$(OUTDIR)\feedback.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "feedbac0"
# PROP BASE Intermediate_Dir "feedbac0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AlphaRel"
# PROP Intermediate_Dir "AlphaRel"
# PROP Target_Dir ""
OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\feedback.exe"

CLEAN : 
	-@erase ".\AlphaRel\feedback.exe"
	-@erase ".\AlphaRel\tray.obj"
	-@erase ".\AlphaRel\dialogs.obj"
	-@erase ".\AlphaRel\stdafx.obj"
	-@erase ".\AlphaRel\guid.obj"
	-@erase ".\AlphaRel\Icons.obj"
	-@erase ".\AlphaRel\fdbkole.obj"
	-@erase ".\AlphaRel\feedback.obj"
	-@erase ".\AlphaRel\feedback.res"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\AlphaRel/
CPP_SBRS=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /alpha
# ADD MTL /nologo /D "NDEBUG" /alpha
MTL_PROJ=/nologo /D "NDEBUG" /alpha 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/feedback.res"  /d "NDEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/feedback.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 ole32.lib uuid.lib /nologo /subsystem:windows /machine:ALPHA
LINK32_FLAGS=ole32.lib uuid.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/feedback.pdb" /machine:ALPHA /out:"$(OUTDIR)/feedback.exe" 
LINK32_OBJS= \
	"$(INTDIR)/tray.obj" \
	"$(INTDIR)/dialogs.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/guid.obj" \
	"$(INTDIR)/Icons.obj" \
	"$(INTDIR)/fdbkole.obj" \
	"$(INTDIR)/feedback.obj" \
	"$(INTDIR)/feedback.res"

"$(OUTDIR)\feedback.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "feedback"
# PROP BASE Intermediate_Dir "feedback"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ppcdbg"
# PROP Intermediate_Dir "ppcdbg"
# PROP Target_Dir ""
OUTDIR=.\ppcdbg
INTDIR=.\ppcdbg

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\feedback.exe"

CLEAN : 
	-@erase ".\ppcdbg\vc40.pdb"
	-@erase ".\ppcdbg\feedback.pch"
	-@erase ".\ppcdbg\feedback.exe"
	-@erase ".\ppcdbg\dialogs.obj"
	-@erase ".\ppcdbg\tray.obj"
	-@erase ".\ppcdbg\guid.obj"
	-@erase ".\ppcdbg\stdafx.obj"
	-@erase ".\ppcdbg\feedback.obj"
	-@erase ".\ppcdbg\Icons.obj"
	-@erase ".\ppcdbg\fdbkole.obj"
	-@erase ".\ppcdbg\feedback.res"
	-@erase ".\ppcdbg\feedback.pdb"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /PPC32
# ADD MTL /nologo /D "_DEBUG" /PPC32
MTL_PROJ=/nologo /D "_DEBUG" /PPC32 
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\ppcdbg/
CPP_SBRS=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/feedback.res"  /d "_DEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/feedback.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:PPC
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:PPC
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)/feedback.pdb" /debug\
 /machine:PPC /out:"$(OUTDIR)/feedback.exe" 
LINK32_OBJS= \
	"$(INTDIR)/dialogs.obj" \
	"$(INTDIR)/tray.obj" \
	"$(INTDIR)/guid.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/feedback.obj" \
	"$(INTDIR)/Icons.obj" \
	"$(INTDIR)/fdbkole.obj" \
	"$(INTDIR)/feedback.res"

"$(OUTDIR)\feedback.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "feedbac0"
# PROP BASE Intermediate_Dir "feedbac0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ppcrel"
# PROP Intermediate_Dir "ppcrel"
# PROP Target_Dir ""
OUTDIR=.\ppcrel
INTDIR=.\ppcrel

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\feedback.exe"

CLEAN : 
	-@erase ".\ppcrel\feedback.exe"
	-@erase ".\ppcrel\dialogs.obj"
	-@erase ".\ppcrel\feedback.pch"
	-@erase ".\ppcrel\tray.obj"
	-@erase ".\ppcrel\guid.obj"
	-@erase ".\ppcrel\stdafx.obj"
	-@erase ".\ppcrel\feedback.obj"
	-@erase ".\ppcrel\Icons.obj"
	-@erase ".\ppcrel\fdbkole.obj"
	-@erase ".\ppcrel\feedback.res"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /PPC32
# ADD MTL /nologo /D "NDEBUG" /PPC32
MTL_PROJ=/nologo /D "NDEBUG" /PPC32 
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\ppcrel/
CPP_SBRS=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/feedback.res"  /d "NDEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/feedback.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:PPC
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:PPC
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)/feedback.pdb"\
 /machine:PPC /out:"$(OUTDIR)/feedback.exe" 
LINK32_OBJS= \
	"$(INTDIR)/dialogs.obj" \
	"$(INTDIR)/tray.obj" \
	"$(INTDIR)/guid.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/feedback.obj" \
	"$(INTDIR)/Icons.obj" \
	"$(INTDIR)/fdbkole.obj" \
	"$(INTDIR)/feedback.res"

"$(OUTDIR)\feedback.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "feedback"
# PROP BASE Intermediate_Dir "feedback"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MIPSDbg"
# PROP Intermediate_Dir "MIPSDbg"
# PROP Target_Dir ""
OUTDIR=.\MIPSDbg
INTDIR=.\MIPSDbg

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\feedback.exe"

CLEAN : 
	-@erase ".\MIPSDbg\vc40.pdb"
	-@erase ".\MIPSDbg\feedback.exe"
	-@erase ".\MIPSDbg\dialogs.obj"
	-@erase ".\MIPSDbg\tray.obj"
	-@erase ".\MIPSDbg\guid.obj"
	-@erase ".\MIPSDbg\stdafx.obj"
	-@erase ".\MIPSDbg\feedback.obj"
	-@erase ".\MIPSDbg\Icons.obj"
	-@erase ".\MIPSDbg\fdbkole.obj"
	-@erase ".\MIPSDbg\feedback.res"
	-@erase ".\MIPSDbg\feedback.ilk"
	-@erase ".\MIPSDbg\feedback.pdb"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mips
# ADD MTL /nologo /D "_DEBUG" /mips
MTL_PROJ=/nologo /D "_DEBUG" /mips 
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /Gt0 /QMOb2000 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MDd /Gt0 /QMOb2000 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MDd /Gt0 /QMOb2000 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\MIPSDbg/
CPP_SBRS=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/feedback.res" /d "_DEBUG" /d "_AFXDLL" 
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:MIPS
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:MIPS
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/feedback.pdb" /debug /machine:MIPS\
 /out:"$(OUTDIR)/feedback.exe" 
LINK32_OBJS= \
	"$(INTDIR)/dialogs.obj" \
	"$(INTDIR)/tray.obj" \
	"$(INTDIR)/guid.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/feedback.obj" \
	"$(INTDIR)/Icons.obj" \
	"$(INTDIR)/fdbkole.obj" \
	"$(INTDIR)/feedback.res"

"$(OUTDIR)\feedback.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/feedback.bsc" 
BSC32_SBRS=

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "feedbac0"
# PROP BASE Intermediate_Dir "feedbac0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MIPSRel"
# PROP Intermediate_Dir "MIPSRel"
# PROP Target_Dir ""
OUTDIR=.\MIPSRel
INTDIR=.\MIPSRel

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\feedback.exe"

CLEAN : 
	-@erase ".\MIPSRel\feedback.exe"
	-@erase ".\MIPSRel\dialogs.obj"
	-@erase ".\MIPSRel\tray.obj"
	-@erase ".\MIPSRel\guid.obj"
	-@erase ".\MIPSRel\stdafx.obj"
	-@erase ".\MIPSRel\feedback.obj"
	-@erase ".\MIPSRel\Icons.obj"
	-@erase ".\MIPSRel\fdbkole.obj"
	-@erase ".\MIPSRel\feedback.res"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mips
# ADD MTL /nologo /D "NDEBUG" /mips
MTL_PROJ=/nologo /D "NDEBUG" /mips 
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /QMOb2000 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MD /Gt0 /QMOb2000 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MD /Gt0 /QMOb2000 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\MIPSRel/
CPP_SBRS=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/feedback.res" /d "NDEBUG" /d "_AFXDLL" 
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:MIPS
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:MIPS
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/feedback.pdb" /machine:MIPS /out:"$(OUTDIR)/feedback.exe" 
LINK32_OBJS= \
	"$(INTDIR)/dialogs.obj" \
	"$(INTDIR)/tray.obj" \
	"$(INTDIR)/guid.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/feedback.obj" \
	"$(INTDIR)/Icons.obj" \
	"$(INTDIR)/fdbkole.obj" \
	"$(INTDIR)/feedback.res"

"$(OUTDIR)\feedback.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/feedback.bsc" 
BSC32_SBRS=

!ENDIF 

################################################################################
# Begin Target

# Name "feedback - Win32 Release"
# Name "feedback - Win32 Debug"
# Name "feedback - Win32 (ALPHA) Debug"
# Name "feedback - Win32 (ALPHA) Release"
# Name "feedback - Win32 (PPC) Debug"
# Name "feedback - Win32 (PPC) Release"
# Name "feedback - Win32 MIPS Debug"
# Name "feedback - Win32 MIPS Release"

!IF  "$(CFG)" == "feedback - Win32 Release"

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Debug"

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\feedback.rc

!IF  "$(CFG)" == "feedback - Win32 Release"

DEP_RSC_FEEDB=\
	".\res\feedback.ico"\
	".\res\listen16.ico"\
	".\res\sleep16.ico"\
	".\res\off16.ico"\
	".\res\notrec16.ico"\
	".\res\recog16.ico"\
	".\res\think16.ico"\
	".\res\listen32.ico"\
	".\res\notrec32.ico"\
	".\res\off32.ico"\
	".\res\recog32.ico"\
	".\res\sleep32.ico"\
	".\res\think32.ico"\
	".\res\feedback.rc2"\
	

"$(INTDIR)\feedback.res" : $(SOURCE) $(DEP_RSC_FEEDB) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

DEP_RSC_FEEDB=\
	".\res\feedback.ico"\
	".\res\listen16.ico"\
	".\res\sleep16.ico"\
	".\res\off16.ico"\
	".\res\notrec16.ico"\
	".\res\recog16.ico"\
	".\res\think16.ico"\
	".\res\listen32.ico"\
	".\res\notrec32.ico"\
	".\res\off32.ico"\
	".\res\recog32.ico"\
	".\res\sleep32.ico"\
	".\res\think32.ico"\
	".\res\feedback.rc2"\
	

"$(INTDIR)\feedback.res" : $(SOURCE) $(DEP_RSC_FEEDB) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_RSC_FEEDB=\
	".\res\feedback.ico"\
	".\res\listen16.ico"\
	".\res\sleep16.ico"\
	".\res\off16.ico"\
	".\res\notrec16.ico"\
	".\res\recog16.ico"\
	".\res\think16.ico"\
	".\res\listen32.ico"\
	".\res\notrec32.ico"\
	".\res\off32.ico"\
	".\res\recog32.ico"\
	".\res\sleep32.ico"\
	".\res\think32.ico"\
	".\res\feedback.rc2"\
	

"$(INTDIR)\feedback.res" : $(SOURCE) $(DEP_RSC_FEEDB) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/feedback.res"  /d "_DEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_RSC_FEEDB=\
	".\res\feedback.ico"\
	".\res\listen16.ico"\
	".\res\sleep16.ico"\
	".\res\off16.ico"\
	".\res\notrec16.ico"\
	".\res\recog16.ico"\
	".\res\think16.ico"\
	".\res\listen32.ico"\
	".\res\notrec32.ico"\
	".\res\off32.ico"\
	".\res\recog32.ico"\
	".\res\sleep32.ico"\
	".\res\think32.ico"\
	".\res\feedback.rc2"\
	

"$(INTDIR)\feedback.res" : $(SOURCE) $(DEP_RSC_FEEDB) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/feedback.res"  /d "NDEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_RSC_FEEDB=\
	".\res\feedback.ico"\
	".\res\listen16.ico"\
	".\res\sleep16.ico"\
	".\res\off16.ico"\
	".\res\notrec16.ico"\
	".\res\recog16.ico"\
	".\res\think16.ico"\
	".\res\listen32.ico"\
	".\res\notrec32.ico"\
	".\res\off32.ico"\
	".\res\recog32.ico"\
	".\res\sleep32.ico"\
	".\res\think32.ico"\
	".\res\feedback.rc2"\
	

"$(INTDIR)\feedback.res" : $(SOURCE) $(DEP_RSC_FEEDB) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/feedback.res"  /d "_DEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_RSC_FEEDB=\
	".\res\feedback.ico"\
	".\res\listen16.ico"\
	".\res\sleep16.ico"\
	".\res\off16.ico"\
	".\res\notrec16.ico"\
	".\res\recog16.ico"\
	".\res\think16.ico"\
	".\res\listen32.ico"\
	".\res\notrec32.ico"\
	".\res\off32.ico"\
	".\res\recog32.ico"\
	".\res\sleep32.ico"\
	".\res\think32.ico"\
	".\res\feedback.rc2"\
	

"$(INTDIR)\feedback.res" : $(SOURCE) $(DEP_RSC_FEEDB) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/feedback.res"  /d "NDEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Debug"

DEP_RSC_FEEDB=\
	".\res\feedback.ico"\
	".\res\listen16.ico"\
	".\res\sleep16.ico"\
	".\res\off16.ico"\
	".\res\notrec16.ico"\
	".\res\recog16.ico"\
	".\res\think16.ico"\
	".\res\listen32.ico"\
	".\res\notrec32.ico"\
	".\res\off32.ico"\
	".\res\recog32.ico"\
	".\res\sleep32.ico"\
	".\res\think32.ico"\
	".\res\feedback.rc2"\
	

"$(INTDIR)\feedback.res" : $(SOURCE) $(DEP_RSC_FEEDB) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/feedback.res" /d "_DEBUG" /d "_AFXDLL"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Release"

DEP_RSC_FEEDB=\
	".\res\feedback.ico"\
	".\res\listen16.ico"\
	".\res\sleep16.ico"\
	".\res\off16.ico"\
	".\res\notrec16.ico"\
	".\res\recog16.ico"\
	".\res\think16.ico"\
	".\res\listen32.ico"\
	".\res\notrec32.ico"\
	".\res\off32.ico"\
	".\res\recog32.ico"\
	".\res\sleep32.ico"\
	".\res\think32.ico"\
	".\res\feedback.rc2"\
	

"$(INTDIR)\feedback.res" : $(SOURCE) $(DEP_RSC_FEEDB) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/feedback.res" /d "NDEBUG" /d "_AFXDLL"\
 $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dialogs.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

DEP_CPP_DIALO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\dialogs.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

DEP_CPP_DIALO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\dialogs.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_DIALO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\dialogs.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_DIALO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\dialogs.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_DIALO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\dialogs.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_DIALO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\dialogs.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Debug"

DEP_CPP_DIALO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\dialogs.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Release"

DEP_CPP_DIALO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\dialogs.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tray.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

DEP_CPP_TRAY_=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\tray.obj" : $(SOURCE) $(DEP_CPP_TRAY_) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

DEP_CPP_TRAY_=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\tray.obj" : $(SOURCE) $(DEP_CPP_TRAY_) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_TRAY_=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\tray.obj" : $(SOURCE) $(DEP_CPP_TRAY_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_TRAY_=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\tray.obj" : $(SOURCE) $(DEP_CPP_TRAY_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_TRAY_=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\tray.obj" : $(SOURCE) $(DEP_CPP_TRAY_) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_TRAY_=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\tray.obj" : $(SOURCE) $(DEP_CPP_TRAY_) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Debug"

DEP_CPP_TRAY_=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\tray.obj" : $(SOURCE) $(DEP_CPP_TRAY_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Release"

DEP_CPP_TRAY_=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\tray.obj" : $(SOURCE) $(DEP_CPP_TRAY_) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\guid.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"\speech.h"\
	
# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\guid.obj" : $(SOURCE) $(DEP_CPP_GUID_) "$(INTDIR)"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"\speech.h"\
	
# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\guid.obj" : $(SOURCE) $(DEP_CPP_GUID_) "$(INTDIR)"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\guid.obj" : $(SOURCE) $(DEP_CPP_GUID_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\guid.obj" : $(SOURCE) $(DEP_CPP_GUID_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"\speech.h"\
	
# SUBTRACT CPP /YX

"$(INTDIR)\guid.obj" : $(SOURCE) $(DEP_CPP_GUID_) "$(INTDIR)"
   $(CPP) /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"\speech.h"\
	
# SUBTRACT CPP /YX

"$(INTDIR)\guid.obj" : $(SOURCE) $(DEP_CPP_GUID_) "$(INTDIR)"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Debug"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\guid.obj" : $(SOURCE) $(DEP_CPP_GUID_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Release"

DEP_CPP_GUID_=\
	".\stdafx.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\guid.obj" : $(SOURCE) $(DEP_CPP_GUID_) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdafx.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\feedback.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\feedback.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	
# ADD CPP /Yc"stdafx.h"
BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\feedback.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	
# ADD CPP /Yc"stdafx.h"
BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\feedback.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Debug"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Release"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\feedback.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

DEP_CPP_FEEDBA=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\feedback.obj" : $(SOURCE) $(DEP_CPP_FEEDBA) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

DEP_CPP_FEEDBA=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\feedback.obj" : $(SOURCE) $(DEP_CPP_FEEDBA) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_FEEDBA=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\feedback.obj" : $(SOURCE) $(DEP_CPP_FEEDBA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_FEEDBA=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\feedback.obj" : $(SOURCE) $(DEP_CPP_FEEDBA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_FEEDBA=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\feedback.obj" : $(SOURCE) $(DEP_CPP_FEEDBA) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_FEEDBA=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\feedback.obj" : $(SOURCE) $(DEP_CPP_FEEDBA) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Debug"

DEP_CPP_FEEDBA=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\feedback.obj" : $(SOURCE) $(DEP_CPP_FEEDBA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Release"

DEP_CPP_FEEDBA=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\feedback.obj" : $(SOURCE) $(DEP_CPP_FEEDBA) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Icons.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

DEP_CPP_ICONS=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\Icons.obj" : $(SOURCE) $(DEP_CPP_ICONS) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

DEP_CPP_ICONS=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\Icons.obj" : $(SOURCE) $(DEP_CPP_ICONS) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_ICONS=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\Icons.obj" : $(SOURCE) $(DEP_CPP_ICONS) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_ICONS=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\Icons.obj" : $(SOURCE) $(DEP_CPP_ICONS) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_ICONS=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\Icons.obj" : $(SOURCE) $(DEP_CPP_ICONS) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_ICONS=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\Icons.obj" : $(SOURCE) $(DEP_CPP_ICONS) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Debug"

DEP_CPP_ICONS=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\Icons.obj" : $(SOURCE) $(DEP_CPP_ICONS) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Release"

DEP_CPP_ICONS=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\Icons.obj" : $(SOURCE) $(DEP_CPP_ICONS) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fdbkole.cpp

!IF  "$(CFG)" == "feedback - Win32 Release"

DEP_CPP_FDBKO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\fdbkole.obj" : $(SOURCE) $(DEP_CPP_FDBKO) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 Debug"

DEP_CPP_FDBKO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\fdbkole.obj" : $(SOURCE) $(DEP_CPP_FDBKO) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Debug"

DEP_CPP_FDBKO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\fdbkole.obj" : $(SOURCE) $(DEP_CPP_FDBKO) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (ALPHA) Release"

DEP_CPP_FDBKO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\fdbkole.obj" : $(SOURCE) $(DEP_CPP_FDBKO) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Debug"

DEP_CPP_FDBKO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\fdbkole.obj" : $(SOURCE) $(DEP_CPP_FDBKO) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 (PPC) Release"

DEP_CPP_FDBKO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	
# ADD CPP /Yu"stdafx.h"

"$(INTDIR)\fdbkole.obj" : $(SOURCE) $(DEP_CPP_FDBKO) "$(INTDIR)"\
 "$(INTDIR)\feedback.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/feedback.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Debug"

DEP_CPP_FDBKO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\fdbkole.obj" : $(SOURCE) $(DEP_CPP_FDBKO) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "feedback - Win32 MIPS Release"

DEP_CPP_FDBKO=\
	".\stdafx.h"\
	".\feedback.h"\
	{$(INCLUDE)}"\speech.h"\
	".\tray.h"\
	".\icons.h"\
	

"$(INTDIR)\fdbkole.obj" : $(SOURCE) $(DEP_CPP_FDBKO) "$(INTDIR)"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
