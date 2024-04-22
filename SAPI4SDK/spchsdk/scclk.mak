# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (PPC) Application" 0x0701
# TARGTYPE "Win32 (MIPS) Application" 0x0501
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

!IF "$(CFG)" == ""
CFG=srclk - Win32 MIPS Debug
!MESSAGE No configuration specified.  Defaulting to srclk - Win32 MIPS Debug.
!ENDIF 

!IF "$(CFG)" != "srclk - Win32 Release" && "$(CFG)" != "srclk - Win32 Debug" &&\
 "$(CFG)" != "srclk - Win32 (ALPHA) Debug" && "$(CFG)" !=\
 "srclk - Win32 (ALPHA) Release" && "$(CFG)" != "srclk - Win32 (PPC) Debug" &&\
 "$(CFG)" != "srclk - Win32 (PPC) Release" && "$(CFG)" !=\
 "srclk - Win32 MIPS Debug" && "$(CFG)" != "srclk - Win32 MIPS Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "srclk.mak" CFG="srclk - Win32 MIPS Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "srclk - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "srclk - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "srclk - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "srclk - Win32 (ALPHA) Release" (based on "Win32 (ALPHA) Application")
!MESSAGE "srclk - Win32 (PPC) Debug" (based on "Win32 (PPC) Application")
!MESSAGE "srclk - Win32 (PPC) Release" (based on "Win32 (PPC) Application")
!MESSAGE "srclk - Win32 MIPS Debug" (based on "Win32 (MIPS) Application")
!MESSAGE "srclk - Win32 MIPS Release" (based on "Win32 (MIPS) Application")
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
# PROP Target_Last_Scanned "srclk - Win32 Debug"

!IF  "$(CFG)" == "srclk - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\srclk.exe"

CLEAN : 
	-@erase ".\WinRel\srclk.exe"
	-@erase ".\WinRel\vcmd.obj"
	-@erase ".\WinRel\srclk.obj"
	-@erase ".\WinRel\showdib.obj"
	-@erase ".\WinRel\srclkdlg.obj"
	-@erase ".\WinRel\init.obj"
	-@erase ".\WinRel\clkhands.obj"
	-@erase ".\WinRel\srclk.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/srclk.pch" /YX /Fo"$(INTDIR)/" /c 
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
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/srclk.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/srclk.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /incremental:no /pdb:"$(OUTDIR)/srclk.pdb" /machine:I386\
 /out:"$(OUTDIR)/srclk.exe" /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	".\WinRel\vcmd.obj" \
	".\WinRel\srclk.obj" \
	".\WinRel\showdib.obj" \
	".\WinRel\srclkdlg.obj" \
	".\WinRel\init.obj" \
	".\WinRel\clkhands.obj" \
	".\WinRel\srclk.res"

"$(OUTDIR)\srclk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\srclk.exe"

CLEAN : 
	-@erase ".\WinDebug\vc40.pdb"
	-@erase ".\WinDebug\vc40.idb"
	-@erase ".\WinDebug\srclk.exe"
	-@erase ".\WinDebug\clkhands.obj"
	-@erase ".\WinDebug\srclkdlg.obj"
	-@erase ".\WinDebug\vcmd.obj"
	-@erase ".\WinDebug\srclk.obj"
	-@erase ".\WinDebug\showdib.obj"
	-@erase ".\WinDebug\init.obj"
	-@erase ".\WinDebug\srclk.res"
	-@erase ".\WinDebug\srclk.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/srclk.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/srclk.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/srclk.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /incremental:no /debug /machine:I386 /SUBSYSTEM:windows,4.0
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /incremental:no /pdb:"$(OUTDIR)/srclk.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/srclk.exe" /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	".\WinDebug\clkhands.obj" \
	".\WinDebug\srclkdlg.obj" \
	".\WinDebug\vcmd.obj" \
	".\WinDebug\srclk.obj" \
	".\WinDebug\showdib.obj" \
	".\WinDebug\init.obj" \
	".\WinDebug\srclk.res"

"$(OUTDIR)\srclk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "srclk___"
# PROP BASE Intermediate_Dir "srclk___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AlphaDbg"
# PROP Intermediate_Dir "AlphaDbg"
# PROP Target_Dir ""
OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\srclk.exe"

CLEAN : 
	-@erase ".\AlphaDbg\vc40.pdb"
	-@erase ".\AlphaDbg\srclk.exe"
	-@erase ".\AlphaDbg\vcmd.obj"
	-@erase ".\AlphaDbg\init.obj"
	-@erase ".\AlphaDbg\showdib.obj"
	-@erase ".\AlphaDbg\clkhands.obj"
	-@erase ".\AlphaDbg\srclk.obj"
	-@erase ".\AlphaDbg\srclkdlg.obj"
	-@erase ".\AlphaDbg\srclk.res"
	-@erase ".\AlphaDbg\srclk.ilk"
	-@erase ".\AlphaDbg\srclk.pdb"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od  /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /Gt0 /W3 /GX /Zi /Od  /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)/srclk.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409  /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/srclk.res"  /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/srclk.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/srclk.pdb" /debug /machine:ALPHA /out:"$(OUTDIR)/srclk.exe" 
LINK32_OBJS= \
	"$(INTDIR)/vcmd.obj" \
	"$(INTDIR)/init.obj" \
	"$(INTDIR)/showdib.obj" \
	"$(INTDIR)/clkhands.obj" \
	"$(INTDIR)/srclk.obj" \
	"$(INTDIR)/srclkdlg.obj" \
	"$(INTDIR)/srclk.res"

"$(OUTDIR)\srclk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "srclk__0"
# PROP BASE Intermediate_Dir "srclk__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AlphaRel"
# PROP Intermediate_Dir "AlphaRel"
# PROP Target_Dir ""
OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\srclk.exe"

CLEAN : 
	-@erase ".\AlphaRel\srclk.exe"
	-@erase ".\AlphaRel\vcmd.obj"
	-@erase ".\AlphaRel\init.obj"
	-@erase ".\AlphaRel\showdib.obj"
	-@erase ".\AlphaRel\clkhands.obj"
	-@erase ".\AlphaRel\srclk.obj"
	-@erase ".\AlphaRel\srclkdlg.obj"
	-@erase ".\AlphaRel\srclk.res"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2  /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /Gt0 /W3 /GX /O2  /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/srclk.pch" /YX /Fo"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409  /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/srclk.res"  /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/srclk.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:ALPHA
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/srclk.pdb" /machine:ALPHA /out:"$(OUTDIR)/srclk.exe" 
LINK32_OBJS= \
	"$(INTDIR)/vcmd.obj" \
	"$(INTDIR)/init.obj" \
	"$(INTDIR)/showdib.obj" \
	"$(INTDIR)/clkhands.obj" \
	"$(INTDIR)/srclk.obj" \
	"$(INTDIR)/srclkdlg.obj" \
	"$(INTDIR)/srclk.res"

"$(OUTDIR)\srclk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "srclk___"
# PROP BASE Intermediate_Dir "srclk___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ppcdbg"
# PROP Intermediate_Dir "ppcdbg"
# PROP Target_Dir ""
OUTDIR=.\ppcdbg
INTDIR=.\ppcdbg

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\srclk.exe"

CLEAN : 
	-@erase ".\ppcdbg\vc40.pdb"
	-@erase ".\ppcdbg\srclk.exe"
	-@erase ".\ppcdbg\vcmd.obj"
	-@erase ".\ppcdbg\init.obj"
	-@erase ".\ppcdbg\showdib.obj"
	-@erase ".\ppcdbg\clkhands.obj"
	-@erase ".\ppcdbg\srclk.obj"
	-@erase ".\ppcdbg\srclkdlg.obj"
	-@erase ".\ppcdbg\srclk.res"
	-@erase ".\ppcdbg\srclk.pdb"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /PPC32
# ADD MTL /nologo /D "_DEBUG" /PPC32
MTL_PROJ=/nologo /D "_DEBUG" /PPC32 
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Zi /Od  /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /GX /Zi /Od  /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/srclk.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409  /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/srclk.res"  /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/srclk.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:PPC
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:PPC
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)/srclk.pdb"\
 /debug /machine:PPC /out:"$(OUTDIR)/srclk.exe" 
LINK32_OBJS= \
	"$(INTDIR)/vcmd.obj" \
	"$(INTDIR)/init.obj" \
	"$(INTDIR)/showdib.obj" \
	"$(INTDIR)/clkhands.obj" \
	"$(INTDIR)/srclk.obj" \
	"$(INTDIR)/srclkdlg.obj" \
	"$(INTDIR)/srclk.res"

"$(OUTDIR)\srclk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "srclk__0"
# PROP BASE Intermediate_Dir "srclk__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ppcrel"
# PROP Intermediate_Dir "ppcrel"
# PROP Target_Dir ""
OUTDIR=.\ppcrel
INTDIR=.\ppcrel

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\srclk.exe"

CLEAN : 
	-@erase ".\ppcrel\srclk.exe"
	-@erase ".\ppcrel\vcmd.obj"
	-@erase ".\ppcrel\init.obj"
	-@erase ".\ppcrel\showdib.obj"
	-@erase ".\ppcrel\clkhands.obj"
	-@erase ".\ppcrel\srclk.obj"
	-@erase ".\ppcrel\srclkdlg.obj"
	-@erase ".\ppcrel\srclk.res"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /PPC32
# ADD MTL /nologo /D "NDEBUG" /PPC32
MTL_PROJ=/nologo /D "NDEBUG" /PPC32 
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2  /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2  /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/srclk.pch" /YX /Fo"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409  /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/srclk.res"  /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/srclk.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:PPC
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:PPC
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)/srclk.pdb"\
 /machine:PPC /out:"$(OUTDIR)/srclk.exe" 
LINK32_OBJS= \
	"$(INTDIR)/vcmd.obj" \
	"$(INTDIR)/init.obj" \
	"$(INTDIR)/showdib.obj" \
	"$(INTDIR)/clkhands.obj" \
	"$(INTDIR)/srclk.obj" \
	"$(INTDIR)/srclkdlg.obj" \
	"$(INTDIR)/srclk.res"

"$(OUTDIR)\srclk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "srclk___"
# PROP BASE Intermediate_Dir "srclk___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MIPSDbg"
# PROP Intermediate_Dir "MIPSDbg"
# PROP Target_Dir ""
OUTDIR=.\MIPSDbg
INTDIR=.\MIPSDbg

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\srclk.exe"

CLEAN : 
	-@erase ".\MIPSDbg\vc40.pdb"
	-@erase ".\MIPSDbg\srclk.exe"
	-@erase ".\MIPSDbg\vcmd.obj"
	-@erase ".\MIPSDbg\init.obj"
	-@erase ".\MIPSDbg\showdib.obj"
	-@erase ".\MIPSDbg\clkhands.obj"
	-@erase ".\MIPSDbg\srclk.obj"
	-@erase ".\MIPSDbg\srclkdlg.obj"
	-@erase ".\MIPSDbg\srclk.res"
	-@erase ".\MIPSDbg\srclk.ilk"
	-@erase ".\MIPSDbg\srclk.pdb"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mips
# ADD MTL /nologo /D "_DEBUG" /mips
MTL_PROJ=/nologo /D "_DEBUG" /mips 
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /QMOb2000 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /QMOb2000 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /Gt0 /QMOb2000 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/srclk.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/srclk.res" /d "_DEBUG" 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:MIPS
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:MIPS
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/srclk.pdb" /debug /machine:MIPS /out:"$(OUTDIR)/srclk.exe" 
LINK32_OBJS= \
	"$(INTDIR)/vcmd.obj" \
	"$(INTDIR)/init.obj" \
	"$(INTDIR)/showdib.obj" \
	"$(INTDIR)/clkhands.obj" \
	"$(INTDIR)/srclk.obj" \
	"$(INTDIR)/srclkdlg.obj" \
	"$(INTDIR)/srclk.res"

"$(OUTDIR)\srclk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/srclk.bsc" 
BSC32_SBRS=

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "srclk__0"
# PROP BASE Intermediate_Dir "srclk__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MIPSRel"
# PROP Intermediate_Dir "MIPSRel"
# PROP Target_Dir ""
OUTDIR=.\MIPSRel
INTDIR=.\MIPSRel

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : "$(OUTDIR)\srclk.exe"

CLEAN : 
	-@erase ".\MIPSRel\srclk.exe"
	-@erase ".\MIPSRel\vcmd.obj"
	-@erase ".\MIPSRel\init.obj"
	-@erase ".\MIPSRel\showdib.obj"
	-@erase ".\MIPSRel\clkhands.obj"
	-@erase ".\MIPSRel\srclk.obj"
	-@erase ".\MIPSRel\srclkdlg.obj"
	-@erase ".\MIPSRel\srclk.res"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mips
# ADD MTL /nologo /D "NDEBUG" /mips
MTL_PROJ=/nologo /D "NDEBUG" /mips 
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /QMOb2000 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /QMOb2000 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /Gt0 /QMOb2000 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/srclk.pch" /YX /Fo"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/srclk.res" /d "NDEBUG" 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:MIPS
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:MIPS
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/srclk.pdb" /machine:MIPS /out:"$(OUTDIR)/srclk.exe" 
LINK32_OBJS= \
	"$(INTDIR)/vcmd.obj" \
	"$(INTDIR)/init.obj" \
	"$(INTDIR)/showdib.obj" \
	"$(INTDIR)/clkhands.obj" \
	"$(INTDIR)/srclk.obj" \
	"$(INTDIR)/srclkdlg.obj" \
	"$(INTDIR)/srclk.res"

"$(OUTDIR)\srclk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/srclk.bsc" 
BSC32_SBRS=

!ENDIF 

################################################################################
# Begin Target

# Name "srclk - Win32 Release"
# Name "srclk - Win32 Debug"
# Name "srclk - Win32 (ALPHA) Debug"
# Name "srclk - Win32 (ALPHA) Release"
# Name "srclk - Win32 (PPC) Debug"
# Name "srclk - Win32 (PPC) Release"
# Name "srclk - Win32 MIPS Debug"
# Name "srclk - Win32 MIPS Release"

!IF  "$(CFG)" == "srclk - Win32 Release"

!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Debug"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\vcmd.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	".\vcmd.h"\
	

"$(INTDIR)\vcmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	".\vcmd.h"\
	

"$(INTDIR)\vcmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\vcmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\vcmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\vcmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\vcmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Debug"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\vcmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Release"

DEP_CPP_VCMD_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\vcmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\init.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	".\vcmd.h"\
	

"$(INTDIR)\init.obj" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	".\vcmd.h"\
	

"$(INTDIR)\init.obj" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\init.obj" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\init.obj" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\init.obj" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\init.obj" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Debug"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\init.obj" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Release"

DEP_CPP_INIT_=\
	".\Srclk.h"\
	".\vcmd.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\init.obj" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\srclk.rc

!IF  "$(CFG)" == "srclk - Win32 Release"

DEP_RSC_SRCLK=\
	".\srclk.ico"\
	

"$(INTDIR)\srclk.res" : $(SOURCE) $(DEP_RSC_SRCLK) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

DEP_RSC_SRCLK=\
	".\srclk.ico"\
	

"$(INTDIR)\srclk.res" : $(SOURCE) $(DEP_RSC_SRCLK) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_RSC_SRCLK=\
	".\srclk.ico"\
	

"$(INTDIR)\srclk.res" : $(SOURCE) $(DEP_RSC_SRCLK) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/srclk.res"  /d "_DEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_RSC_SRCLK=\
	".\srclk.ico"\
	

"$(INTDIR)\srclk.res" : $(SOURCE) $(DEP_RSC_SRCLK) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/srclk.res"  /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_RSC_SRCLK=\
	".\srclk.ico"\
	

"$(INTDIR)\srclk.res" : $(SOURCE) $(DEP_RSC_SRCLK) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/srclk.res"  /d "_DEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_RSC_SRCLK=\
	".\srclk.ico"\
	

"$(INTDIR)\srclk.res" : $(SOURCE) $(DEP_RSC_SRCLK) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/srclk.res"  /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Debug"

DEP_RSC_SRCLK=\
	".\srclk.ico"\
	

"$(INTDIR)\srclk.res" : $(SOURCE) $(DEP_RSC_SRCLK) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/srclk.res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Release"

DEP_RSC_SRCLK=\
	".\srclk.ico"\
	

"$(INTDIR)\srclk.res" : $(SOURCE) $(DEP_RSC_SRCLK) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/srclk.res" /d "NDEBUG" $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\showdib.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\showdib.obj" : $(SOURCE) $(DEP_CPP_SHOWD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\showdib.obj" : $(SOURCE) $(DEP_CPP_SHOWD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\showdib.obj" : $(SOURCE) $(DEP_CPP_SHOWD) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\showdib.obj" : $(SOURCE) $(DEP_CPP_SHOWD) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\showdib.obj" : $(SOURCE) $(DEP_CPP_SHOWD) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\showdib.obj" : $(SOURCE) $(DEP_CPP_SHOWD) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Debug"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\showdib.obj" : $(SOURCE) $(DEP_CPP_SHOWD) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Release"

DEP_CPP_SHOWD=\
	".\showdib.h"\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\showdib.obj" : $(SOURCE) $(DEP_CPP_SHOWD) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\clkhands.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\clkhands.obj" : $(SOURCE) $(DEP_CPP_CLKHA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\clkhands.obj" : $(SOURCE) $(DEP_CPP_CLKHA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\clkhands.obj" : $(SOURCE) $(DEP_CPP_CLKHA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\clkhands.obj" : $(SOURCE) $(DEP_CPP_CLKHA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\clkhands.obj" : $(SOURCE) $(DEP_CPP_CLKHA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\clkhands.obj" : $(SOURCE) $(DEP_CPP_CLKHA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Debug"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\clkhands.obj" : $(SOURCE) $(DEP_CPP_CLKHA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Release"

DEP_CPP_CLKHA=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\clkhands.obj" : $(SOURCE) $(DEP_CPP_CLKHA) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\srclk.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

DEP_CPP_SRCLK_=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	".\clkhands.h"\
	".\srclkdlg.h"\
	".\showdib.h"\
	

"$(INTDIR)\srclk.obj" : $(SOURCE) $(DEP_CPP_SRCLK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

DEP_CPP_SRCLK_=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	".\clkhands.h"\
	".\srclkdlg.h"\
	".\showdib.h"\
	

"$(INTDIR)\srclk.obj" : $(SOURCE) $(DEP_CPP_SRCLK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_SRCLK_=\
	".\Srclk.h"\
	".\clkhands.h"\
	".\srclkdlg.h"\
	".\showdib.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclk.obj" : $(SOURCE) $(DEP_CPP_SRCLK_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_SRCLK_=\
	".\Srclk.h"\
	".\clkhands.h"\
	".\srclkdlg.h"\
	".\showdib.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclk.obj" : $(SOURCE) $(DEP_CPP_SRCLK_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_SRCLK_=\
	".\Srclk.h"\
	".\clkhands.h"\
	".\srclkdlg.h"\
	".\showdib.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclk.obj" : $(SOURCE) $(DEP_CPP_SRCLK_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_SRCLK_=\
	".\Srclk.h"\
	".\clkhands.h"\
	".\srclkdlg.h"\
	".\showdib.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclk.obj" : $(SOURCE) $(DEP_CPP_SRCLK_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Debug"

DEP_CPP_SRCLK_=\
	".\Srclk.h"\
	".\clkhands.h"\
	".\srclkdlg.h"\
	".\showdib.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclk.obj" : $(SOURCE) $(DEP_CPP_SRCLK_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Release"

DEP_CPP_SRCLK_=\
	".\Srclk.h"\
	".\clkhands.h"\
	".\srclkdlg.h"\
	".\showdib.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclk.obj" : $(SOURCE) $(DEP_CPP_SRCLK_) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\srclkdlg.cpp

!IF  "$(CFG)" == "srclk - Win32 Release"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	".\srclkdlg.h"\
	

"$(INTDIR)\srclkdlg.obj" : $(SOURCE) $(DEP_CPP_SRCLKD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 Debug"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	{$(INCLUDE)}"\speech.h"\
	".\srclkdlg.h"\
	

"$(INTDIR)\srclkdlg.obj" : $(SOURCE) $(DEP_CPP_SRCLKD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Debug"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclkdlg.obj" : $(SOURCE) $(DEP_CPP_SRCLKD) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (ALPHA) Release"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclkdlg.obj" : $(SOURCE) $(DEP_CPP_SRCLKD) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Debug"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclkdlg.obj" : $(SOURCE) $(DEP_CPP_SRCLKD) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 (PPC) Release"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclkdlg.obj" : $(SOURCE) $(DEP_CPP_SRCLKD) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Debug"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclkdlg.obj" : $(SOURCE) $(DEP_CPP_SRCLKD) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "srclk - Win32 MIPS Release"

DEP_CPP_SRCLKD=\
	".\Srclk.h"\
	".\srclkdlg.h"\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\srclkdlg.obj" : $(SOURCE) $(DEP_CPP_SRCLKD) "$(INTDIR)"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
