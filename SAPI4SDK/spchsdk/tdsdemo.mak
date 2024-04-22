# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (PPC) Application" 0x0701
# TARGTYPE "Win32 (MIPS) Application" 0x0501
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

!IF "$(CFG)" == ""
CFG=ttsdemo - Win32 MIPS Debug
!MESSAGE No configuration specified.  Defaulting to ttsdemo - Win32 MIPS Debug.
!ENDIF 

!IF "$(CFG)" != "ttsdemo - Win32 Release" && "$(CFG)" !=\
 "ttsdemo - Win32 Debug" && "$(CFG)" != "ttsdemo - Win32 (ALPHA) Debug" &&\
 "$(CFG)" != "ttsdemo - Win32 (ALPHA) Release" && "$(CFG)" !=\
 "ttsdemo - Win32 (PPC) Debug" && "$(CFG)" != "ttsdemo - Win32 (PPC) Release" &&\
 "$(CFG)" != "ttsdemo - Win32 MIPS Debug" && "$(CFG)" !=\
 "ttsdemo - Win32 MIPS Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "ttsdemo.mak" CFG="ttsdemo - Win32 MIPS Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ttsdemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ttsdemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ttsdemo - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "ttsdemo - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "ttsdemo - Win32 (PPC) Debug" (based on "Win32 (PPC) Application")
!MESSAGE "ttsdemo - Win32 (PPC) Release" (based on "Win32 (PPC) Application")
!MESSAGE "ttsdemo - Win32 MIPS Debug" (based on "Win32 (MIPS) Application")
!MESSAGE "ttsdemo - Win32 MIPS Release" (based on "Win32 (MIPS) Application")
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
# PROP Target_Last_Scanned "ttsdemo - Win32 MIPS Release"

!IF  "$(CFG)" == "ttsdemo - Win32 Release"

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

ALL : "$(OUTDIR)\ttsdemo.exe"

CLEAN : 
	-@erase ".\WinRel\ttsdemo.exe"
	-@erase ".\WinRel\demo.obj"
	-@erase ".\WinRel\ttsdemo.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/ttsdemo.pch" /YX /Fo"$(INTDIR)/" /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ttsdemo.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttsdemo.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/ttsdemo.pdb" /machine:I386\
 /out:"$(OUTDIR)/ttsdemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)/demo.obj" \
	"$(INTDIR)/ttsdemo.res"

"$(OUTDIR)\ttsdemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 Debug"

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

ALL : "$(OUTDIR)\ttsdemo.exe"

CLEAN : 
	-@erase ".\WinDebug\vc40.pdb"
	-@erase ".\WinDebug\vc40.idb"
	-@erase ".\WinDebug\ttsdemo.exe"
	-@erase ".\WinDebug\demo.obj"
	-@erase ".\WinDebug\ttsdemo.res"
	-@erase ".\WinDebug\ttsdemo.ilk"
	-@erase ".\WinDebug\ttsdemo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/ttsdemo.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ttsdemo.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttsdemo.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/ttsdemo.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/ttsdemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)/demo.obj" \
	"$(INTDIR)/ttsdemo.res"

"$(OUTDIR)\ttsdemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ttsdemo_"
# PROP BASE Intermediate_Dir "ttsdemo_"
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

ALL : "$(OUTDIR)\ttsdemo.exe"

CLEAN : 
	-@erase ".\AlphaDbg\vc40.pdb"
	-@erase ".\AlphaDbg\ttsdemo.exe"
	-@erase ".\AlphaDbg\demo.obj"
	-@erase ".\AlphaDbg\ttsdemo.res"
	-@erase ".\AlphaDbg\ttsdemo.ilk"
	-@erase ".\AlphaDbg\ttsdemo.pdb"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /Gt0 /W3 /GX /Zi /Od  /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)/ttsdemo.pch" /YX /Fo"$(INTDIR)/"\
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
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ttsdemo.res"  /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttsdemo.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/ttsdemo.pdb" /debug /machine:ALPHA /out:"$(OUTDIR)/ttsdemo.exe"\
 
LINK32_OBJS= \
	"$(INTDIR)/demo.obj" \
	"$(INTDIR)/ttsdemo.res"

"$(OUTDIR)\ttsdemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ttsdemo0"
# PROP BASE Intermediate_Dir "ttsdemo0"
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

ALL : "$(OUTDIR)\ttsdemo.exe"

CLEAN : 
	-@erase ".\AlphaRel\ttsdemo.exe"
	-@erase ".\AlphaRel\demo.obj"
	-@erase ".\AlphaRel\ttsdemo.res"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /Gt0 /W3 /GX /O2  /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/ttsdemo.pch" /YX /Fo"$(INTDIR)/" /c 
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
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ttsdemo.res"  /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttsdemo.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:ALPHA
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/ttsdemo.pdb" /machine:ALPHA /out:"$(OUTDIR)/ttsdemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)/demo.obj" \
	"$(INTDIR)/ttsdemo.res"

"$(OUTDIR)\ttsdemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (PPC) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ttsdemo_"
# PROP BASE Intermediate_Dir "ttsdemo_"
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

ALL : "$(OUTDIR)\ttsdemo.exe"

CLEAN : 
	-@erase ".\ppcdbg\vc40.pdb"
	-@erase ".\ppcdbg\ttsdemo.exe"
	-@erase ".\ppcdbg\demo.obj"
	-@erase ".\ppcdbg\ttsdemo.res"
	-@erase ".\ppcdbg\ttsdemo.pdb"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /PPC32
# ADD MTL /nologo /D "_DEBUG" /PPC32
MTL_PROJ=/nologo /D "_DEBUG" /PPC32 
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /GX /Zi /Od  /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/ttsdemo.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ttsdemo.res"  /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttsdemo.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:PPC
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:PPC
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)/ttsdemo.pdb" /debug\
 /machine:PPC /out:"$(OUTDIR)/ttsdemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)/demo.obj" \
	"$(INTDIR)/ttsdemo.res"

"$(OUTDIR)\ttsdemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (PPC) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ttsdemo0"
# PROP BASE Intermediate_Dir "ttsdemo0"
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

ALL : "$(OUTDIR)\ttsdemo.exe"

CLEAN : 
	-@erase ".\ppcrel\ttsdemo.exe"
	-@erase ".\ppcrel\demo.obj"
	-@erase ".\ppcrel\ttsdemo.res"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /PPC32
# ADD MTL /nologo /D "NDEBUG" /PPC32
MTL_PROJ=/nologo /D "NDEBUG" /PPC32 
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2  /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/ttsdemo.pch" /YX /Fo"$(INTDIR)/" /c 
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
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ttsdemo.res"  /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttsdemo.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:PPC
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:PPC
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)/ttsdemo.pdb"\
 /machine:PPC /out:"$(OUTDIR)/ttsdemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)/demo.obj" \
	"$(INTDIR)/ttsdemo.res"

"$(OUTDIR)\ttsdemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 MIPS Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ttsdemo_"
# PROP BASE Intermediate_Dir "ttsdemo_"
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

ALL : "$(OUTDIR)\ttsdemo.exe"

CLEAN : 
	-@erase ".\MIPSDbg\vc40.pdb"
	-@erase ".\MIPSDbg\ttsdemo.exe"
	-@erase ".\MIPSDbg\demo.obj"
	-@erase ".\MIPSDbg\ttsdemo.res"
	-@erase ".\MIPSDbg\ttsdemo.ilk"
	-@erase ".\MIPSDbg\ttsdemo.pdb"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mips
# ADD MTL /nologo /D "_DEBUG" /mips
MTL_PROJ=/nologo /D "_DEBUG" /mips 
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /QMOb2000 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /QMOb2000 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /Gt0 /QMOb2000 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/ttsdemo.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ttsdemo.res" /d "_DEBUG" 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:MIPS
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:MIPS
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/ttsdemo.pdb" /debug /machine:MIPS /out:"$(OUTDIR)/ttsdemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)/demo.obj" \
	"$(INTDIR)/ttsdemo.res"

"$(OUTDIR)\ttsdemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttsdemo.bsc" 
BSC32_SBRS=

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 MIPS Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ttsdemo0"
# PROP BASE Intermediate_Dir "ttsdemo0"
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

ALL : "$(OUTDIR)\ttsdemo.exe"

CLEAN : 
	-@erase ".\MIPSRel\ttsdemo.exe"
	-@erase ".\MIPSRel\demo.obj"
	-@erase ".\MIPSRel\ttsdemo.res"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mips
# ADD MTL /nologo /D "NDEBUG" /mips
MTL_PROJ=/nologo /D "NDEBUG" /mips 
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /QMOb2000 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /QMOb2000 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /Gt0 /QMOb2000 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/ttsdemo.pch" /YX /Fo"$(INTDIR)/" /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ttsdemo.res" /d "NDEBUG" 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:MIPS
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:MIPS
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/ttsdemo.pdb" /machine:MIPS /out:"$(OUTDIR)/ttsdemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)/demo.obj" \
	"$(INTDIR)/ttsdemo.res"

"$(OUTDIR)\ttsdemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttsdemo.bsc" 
BSC32_SBRS=

!ENDIF 

################################################################################
# Begin Target

# Name "ttsdemo - Win32 Release"
# Name "ttsdemo - Win32 Debug"
# Name "ttsdemo - Win32 (ALPHA) Debug"
# Name "ttsdemo - Win32 (ALPHA) Release"
# Name "ttsdemo - Win32 (PPC) Debug"
# Name "ttsdemo - Win32 (PPC) Release"
# Name "ttsdemo - Win32 MIPS Debug"
# Name "ttsdemo - Win32 MIPS Release"

!IF  "$(CFG)" == "ttsdemo - Win32 Release"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 Debug"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (ALPHA) Debug"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (PPC) Debug"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (PPC) Release"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 MIPS Debug"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 MIPS Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\demo.cpp

!IF  "$(CFG)" == "ttsdemo - Win32 Release"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttsdemo - Win32 Debug"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (ALPHA) Debug"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (ALPHA) Release"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (PPC) Debug"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (PPC) Release"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 MIPS Debug"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "ttsdemo - Win32 MIPS Release"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"\speech.h"\
	

"$(INTDIR)\demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ttsdemo.rc

!IF  "$(CFG)" == "ttsdemo - Win32 Release"


"$(INTDIR)\ttsdemo.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ttsdemo - Win32 Debug"


"$(INTDIR)\ttsdemo.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (ALPHA) Debug"

DEP_RSC_TTSDE=\
	".\Icon1.ico"\
	

"$(INTDIR)\ttsdemo.res" : $(SOURCE) $(DEP_RSC_TTSDE) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/ttsdemo.res"  /d "_DEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (ALPHA) Release"

DEP_RSC_TTSDE=\
	".\Icon1.ico"\
	

"$(INTDIR)\ttsdemo.res" : $(SOURCE) $(DEP_RSC_TTSDE) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/ttsdemo.res"  /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (PPC) Debug"

DEP_RSC_TTSDE=\
	".\Icon1.ico"\
	

"$(INTDIR)\ttsdemo.res" : $(SOURCE) $(DEP_RSC_TTSDE) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/ttsdemo.res"  /d "_DEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ttsdemo - Win32 (PPC) Release"

DEP_RSC_TTSDE=\
	".\Icon1.ico"\
	

"$(INTDIR)\ttsdemo.res" : $(SOURCE) $(DEP_RSC_TTSDE) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/ttsdemo.res"  /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ttsdemo - Win32 MIPS Debug"

DEP_RSC_TTSDE=\
	".\Icon1.ico"\
	

"$(INTDIR)\ttsdemo.res" : $(SOURCE) $(DEP_RSC_TTSDE) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/ttsdemo.res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ttsdemo - Win32 MIPS Release"

DEP_RSC_TTSDE=\
	".\Icon1.ico"\
	

"$(INTDIR)\ttsdemo.res" : $(SOURCE) $(DEP_RSC_TTSDE) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/ttsdemo.res" /d "NDEBUG" $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
