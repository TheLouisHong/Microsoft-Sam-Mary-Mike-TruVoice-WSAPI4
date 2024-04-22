# Microsoft Developer Studio Generated NMAKE File, Based on Srtest.dsp
!IF "$(CFG)" == ""
CFG=Srtest - Win32 (ALPHA) W2WRel
!MESSAGE No configuration specified. Defaulting to Srtest - Win32 (ALPHA)\
 W2WRel.
!ENDIF 

!IF "$(CFG)" != "Srtest - Win32 A2ADebug" && "$(CFG)" !=\
 "Srtest - Win32 A2ARel" && "$(CFG)" != "Srtest - Win32 W2WRel" && "$(CFG)" !=\
 "Srtest - Win32 W2WDebug" && "$(CFG)" != "Srtest - Win32 (ALPHA) A2ADebug" &&\
 "$(CFG)" != "Srtest - Win32 (ALPHA) A2ARel" && "$(CFG)" !=\
 "Srtest - Win32 (ALPHA) W2WDebug" && "$(CFG)" !=\
 "Srtest - Win32 (ALPHA) W2WRel"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Srtest.mak" CFG="Srtest - Win32 (ALPHA) W2WRel"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Srtest - Win32 A2ADebug" (based on "Win32 (x86) Application")
!MESSAGE "Srtest - Win32 A2ARel" (based on "Win32 (x86) Application")
!MESSAGE "Srtest - Win32 W2WRel" (based on "Win32 (x86) Application")
!MESSAGE "Srtest - Win32 W2WDebug" (based on "Win32 (x86) Application")
!MESSAGE "Srtest - Win32 (ALPHA) A2ADebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Srtest - Win32 (ALPHA) A2ARel" (based on "Win32 (ALPHA) Application")
!MESSAGE "Srtest - Win32 (ALPHA) W2WDebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Srtest - Win32 (ALPHA) W2WRel" (based on "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

OUTDIR=.\A2ADebug
INTDIR=.\A2ADebug
# Begin Custom Macros
OutDir=.\.\A2ADebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Srtest.exe" "$(OUTDIR)\Srtest.bsc"

!ELSE 

ALL : "$(OUTDIR)\Srtest.exe" "$(OUTDIR)\Srtest.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\DLGFINDM.SBR"
	-@erase "$(INTDIR)\GRAMFUNC.OBJ"
	-@erase "$(INTDIR)\GRAMFUNC.SBR"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.SBR"
	-@erase "$(INTDIR)\Srdlg.obj"
	-@erase "$(INTDIR)\Srdlg.sbr"
	-@erase "$(INTDIR)\Srfunc.obj"
	-@erase "$(INTDIR)\Srfunc.sbr"
	-@erase "$(INTDIR)\SRTEST.OBJ"
	-@erase "$(INTDIR)\Srtest.pch"
	-@erase "$(INTDIR)\Srtest.res"
	-@erase "$(INTDIR)\SRTEST.SBR"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\STDAFX.SBR"
	-@erase "$(INTDIR)\Tabisrce.obj"
	-@erase "$(INTDIR)\Tabisrce.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Srtest.bsc"
	-@erase "$(OUTDIR)\Srtest.exe"
	-@erase "$(OUTDIR)\Srtest.ilk"
	-@erase "$(OUTDIR)\Srtest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_MBCS" /D "_CRT_MAP_ALLOC" /Fr"$(INTDIR)\\" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\A2ADebug/
CPP_SBRS=.\A2ADebug/

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/mktyplib203 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Srtest.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Srtest.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DLGFINDM.SBR" \
	"$(INTDIR)\GRAMFUNC.SBR" \
	"$(INTDIR)\GUIDSEG.SBR" \
	"$(INTDIR)\Srdlg.sbr" \
	"$(INTDIR)\Srfunc.sbr" \
	"$(INTDIR)\SRTEST.SBR" \
	"$(INTDIR)\STDAFX.SBR" \
	"$(INTDIR)\Tabisrce.sbr"

"$(OUTDIR)\Srtest.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /incremental:yes\
 /pdb:"$(OUTDIR)\Srtest.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Srtest.exe"\
 /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GRAMFUNC.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\Srdlg.obj" \
	"$(INTDIR)\Srfunc.obj" \
	"$(INTDIR)\SRTEST.OBJ" \
	"$(INTDIR)\Srtest.res" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\Tabisrce.obj"

"$(OUTDIR)\Srtest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

OUTDIR=.\A2ARel
INTDIR=.\A2ARel
# Begin Custom Macros
OutDir=.\.\A2ARel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Srtest.exe"

!ELSE 

ALL : "$(OUTDIR)\Srtest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GRAMFUNC.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\Srdlg.obj"
	-@erase "$(INTDIR)\Srfunc.obj"
	-@erase "$(INTDIR)\SRTEST.OBJ"
	-@erase "$(INTDIR)\Srtest.pch"
	-@erase "$(INTDIR)\Srtest.res"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\Tabisrce.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Srtest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\A2ARel/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/mktyplib203 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Srtest.res" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Srtest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /incremental:no\
 /pdb:"$(OUTDIR)\Srtest.pdb" /machine:I386 /out:"$(OUTDIR)\Srtest.exe"\
 /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GRAMFUNC.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\Srdlg.obj" \
	"$(INTDIR)\Srfunc.obj" \
	"$(INTDIR)\SRTEST.OBJ" \
	"$(INTDIR)\Srtest.res" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\Tabisrce.obj"

"$(OUTDIR)\Srtest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

OUTDIR=.\W2WRel
INTDIR=.\W2WRel
# Begin Custom Macros
OutDir=.\.\W2WRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Srtest.exe"

!ELSE 

ALL : "$(OUTDIR)\Srtest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GRAMFUNC.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\Srdlg.obj"
	-@erase "$(INTDIR)\Srfunc.obj"
	-@erase "$(INTDIR)\SRTEST.OBJ"
	-@erase "$(INTDIR)\Srtest.pch"
	-@erase "$(INTDIR)\Srtest.res"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\Tabisrce.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Srtest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\W2WRel/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/mktyplib203 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Srtest.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Srtest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup"\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Srtest.pdb" /machine:I386\
 /out:"$(OUTDIR)\Srtest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GRAMFUNC.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\Srdlg.obj" \
	"$(INTDIR)\Srfunc.obj" \
	"$(INTDIR)\SRTEST.OBJ" \
	"$(INTDIR)\Srtest.res" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\Tabisrce.obj"

"$(OUTDIR)\Srtest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

OUTDIR=.\W2WDebug
INTDIR=.\W2WDebug
# Begin Custom Macros
OutDir=.\.\W2WDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Srtest.exe"

!ELSE 

ALL : "$(OUTDIR)\Srtest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GRAMFUNC.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\Srdlg.obj"
	-@erase "$(INTDIR)\Srfunc.obj"
	-@erase "$(INTDIR)\SRTEST.OBJ"
	-@erase "$(INTDIR)\Srtest.pch"
	-@erase "$(INTDIR)\Srtest.res"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\Tabisrce.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Srtest.exe"
	-@erase "$(OUTDIR)\Srtest.ilk"
	-@erase "$(OUTDIR)\Srtest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\W2WDebug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/mktyplib203 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Srtest.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Srtest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup"\
 /incremental:yes /pdb:"$(OUTDIR)\Srtest.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Srtest.exe" /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GRAMFUNC.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\Srdlg.obj" \
	"$(INTDIR)\Srfunc.obj" \
	"$(INTDIR)\SRTEST.OBJ" \
	"$(INTDIR)\Srtest.res" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\Tabisrce.obj"

"$(OUTDIR)\Srtest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

OUTDIR=.\A2ADebug
INTDIR=.\A2ADebug
# Begin Custom Macros
OutDir=.\.\A2ADebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Srtest.exe"

!ELSE 

ALL : "$(OUTDIR)\Srtest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GRAMFUNC.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\Srdlg.obj"
	-@erase "$(INTDIR)\Srfunc.obj"
	-@erase "$(INTDIR)\SRTEST.OBJ"
	-@erase "$(INTDIR)\Srtest.pch"
	-@erase "$(INTDIR)\Srtest.res"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\Tabisrce.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Srtest.exe"
	-@erase "$(OUTDIR)\Srtest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\A2ADebug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Srtest.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Srtest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /subsystem:windows\
 /pdb:"$(OUTDIR)\Srtest.pdb" /debug /machine:ALPHA /out:"$(OUTDIR)\Srtest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GRAMFUNC.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\Srdlg.obj" \
	"$(INTDIR)\Srfunc.obj" \
	"$(INTDIR)\SRTEST.OBJ" \
	"$(INTDIR)\Srtest.res" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\Tabisrce.obj"

"$(OUTDIR)\Srtest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

OUTDIR=.\A2ARel
INTDIR=.\A2ARel
# Begin Custom Macros
OutDir=.\.\A2ARel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Srtest.exe"

!ELSE 

ALL : "$(OUTDIR)\Srtest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GRAMFUNC.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\Srdlg.obj"
	-@erase "$(INTDIR)\Srfunc.obj"
	-@erase "$(INTDIR)\SRTEST.OBJ"
	-@erase "$(INTDIR)\Srtest.pch"
	-@erase "$(INTDIR)\Srtest.res"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\Tabisrce.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Srtest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\A2ARel/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Srtest.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Srtest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /subsystem:windows\
 /pdb:"$(OUTDIR)\Srtest.pdb" /machine:ALPHA /out:"$(OUTDIR)\Srtest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GRAMFUNC.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\Srdlg.obj" \
	"$(INTDIR)\Srfunc.obj" \
	"$(INTDIR)\SRTEST.OBJ" \
	"$(INTDIR)\Srtest.res" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\Tabisrce.obj"

"$(OUTDIR)\Srtest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

OUTDIR=.\W2WDebug
INTDIR=.\W2WDebug
# Begin Custom Macros
OutDir=.\.\W2WDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Srtest.exe"

!ELSE 

ALL : "$(OUTDIR)\Srtest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GRAMFUNC.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\Srdlg.obj"
	-@erase "$(INTDIR)\Srfunc.obj"
	-@erase "$(INTDIR)\SRTEST.OBJ"
	-@erase "$(INTDIR)\Srtest.pch"
	-@erase "$(INTDIR)\Srtest.res"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\Tabisrce.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Srtest.exe"
	-@erase "$(OUTDIR)\Srtest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\W2WDebug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Srtest.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Srtest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup"\
 /subsystem:windows /pdb:"$(OUTDIR)\Srtest.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\Srtest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GRAMFUNC.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\Srdlg.obj" \
	"$(INTDIR)\Srfunc.obj" \
	"$(INTDIR)\SRTEST.OBJ" \
	"$(INTDIR)\Srtest.res" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\Tabisrce.obj"

"$(OUTDIR)\Srtest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

OUTDIR=.\W2WRel
INTDIR=.\W2WRel
# Begin Custom Macros
OutDir=.\.\W2WRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Srtest.exe"

!ELSE 

ALL : "$(OUTDIR)\Srtest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GRAMFUNC.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\Srdlg.obj"
	-@erase "$(INTDIR)\Srfunc.obj"
	-@erase "$(INTDIR)\SRTEST.OBJ"
	-@erase "$(INTDIR)\Srtest.pch"
	-@erase "$(INTDIR)\Srtest.res"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\Tabisrce.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Srtest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\W2WRel/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Srtest.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Srtest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup"\
 /subsystem:windows /pdb:"$(OUTDIR)\Srtest.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\Srtest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GRAMFUNC.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\Srdlg.obj" \
	"$(INTDIR)\Srfunc.obj" \
	"$(INTDIR)\SRTEST.OBJ" \
	"$(INTDIR)\Srtest.res" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\Tabisrce.obj"

"$(OUTDIR)\Srtest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Srtest - Win32 A2ADebug" || "$(CFG)" ==\
 "Srtest - Win32 A2ARel" || "$(CFG)" == "Srtest - Win32 W2WRel" || "$(CFG)" ==\
 "Srtest - Win32 W2WDebug" || "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug" ||\
 "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel" || "$(CFG)" ==\
 "Srtest - Win32 (ALPHA) W2WDebug" || "$(CFG)" ==\
 "Srtest - Win32 (ALPHA) W2WRel"
SOURCE=.\DLGFINDM.CPP

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /D "_CRT_MAP_ALLOC" /Fr"$(INTDIR)\\"\
 /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\DLGFINDM.OBJ"	"$(INTDIR)\DLGFINDM.SBR" : $(SOURCE) $(DEP_CPP_DLGFI)\
 "$(INTDIR)" "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\GRAMFUNC.CPP

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /D "_CRT_MAP_ALLOC" /Fr"$(INTDIR)\\"\
 /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\GRAMFUNC.OBJ"	"$(INTDIR)\GRAMFUNC.SBR" : $(SOURCE) $(DEP_CPP_GRAMF)\
 "$(INTDIR)" "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GRAMFUNC.OBJ" : $(SOURCE) $(DEP_CPP_GRAMF) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GRAMFUNC.OBJ" : $(SOURCE) $(DEP_CPP_GRAMF) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GRAMFUNC.OBJ" : $(SOURCE) $(DEP_CPP_GRAMF) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GRAMFUNC.OBJ" : $(SOURCE) $(DEP_CPP_GRAMF) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GRAMFUNC.OBJ" : $(SOURCE) $(DEP_CPP_GRAMF) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GRAMFUNC.OBJ" : $(SOURCE) $(DEP_CPP_GRAMF) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_GRAMF=\
	".\GRAMFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GRAMFUNC.OBJ" : $(SOURCE) $(DEP_CPP_GRAMF) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\GUIDSEG.CPP

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /D "_CRT_MAP_ALLOC" /Fr"$(INTDIR)\\" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ"	"$(INTDIR)\GUIDSEG.SBR" : $(SOURCE) $(DEP_CPP_GUIDS)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Srdlg.cpp

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /D "_CRT_MAP_ALLOC" /Fr"$(INTDIR)\\"\
 /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\Srdlg.obj"	"$(INTDIR)\Srdlg.sbr" : $(SOURCE) $(DEP_CPP_SRDLG)\
 "$(INTDIR)" "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srdlg.obj" : $(SOURCE) $(DEP_CPP_SRDLG) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srdlg.obj" : $(SOURCE) $(DEP_CPP_SRDLG) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srdlg.obj" : $(SOURCE) $(DEP_CPP_SRDLG) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srdlg.obj" : $(SOURCE) $(DEP_CPP_SRDLG) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srdlg.obj" : $(SOURCE) $(DEP_CPP_SRDLG) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srdlg.obj" : $(SOURCE) $(DEP_CPP_SRDLG) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_SRDLG=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srdlg.obj" : $(SOURCE) $(DEP_CPP_SRDLG) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Srfunc.cpp

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /D "_CRT_MAP_ALLOC" /Fr"$(INTDIR)\\"\
 /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\Srfunc.obj"	"$(INTDIR)\Srfunc.sbr" : $(SOURCE) $(DEP_CPP_SRFUN)\
 "$(INTDIR)" "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srfunc.obj" : $(SOURCE) $(DEP_CPP_SRFUN) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srfunc.obj" : $(SOURCE) $(DEP_CPP_SRFUN) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srfunc.obj" : $(SOURCE) $(DEP_CPP_SRFUN) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srfunc.obj" : $(SOURCE) $(DEP_CPP_SRFUN) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srfunc.obj" : $(SOURCE) $(DEP_CPP_SRFUN) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srfunc.obj" : $(SOURCE) $(DEP_CPP_SRFUN) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_SRFUN=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Srfunc.obj" : $(SOURCE) $(DEP_CPP_SRFUN) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\SRTEST.CPP

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /D "_CRT_MAP_ALLOC" /Fr"$(INTDIR)\\"\
 /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\SRTEST.OBJ"	"$(INTDIR)\SRTEST.SBR" : $(SOURCE) $(DEP_CPP_SRTES)\
 "$(INTDIR)" "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SRTEST.OBJ" : $(SOURCE) $(DEP_CPP_SRTES) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SRTEST.OBJ" : $(SOURCE) $(DEP_CPP_SRTES) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SRTEST.OBJ" : $(SOURCE) $(DEP_CPP_SRTES) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SRTEST.OBJ" : $(SOURCE) $(DEP_CPP_SRTES) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SRTEST.OBJ" : $(SOURCE) $(DEP_CPP_SRTES) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SRTEST.OBJ" : $(SOURCE) $(DEP_CPP_SRTES) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_SRTES=\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SRTEST.OBJ" : $(SOURCE) $(DEP_CPP_SRTES) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Srtest.rc
DEP_RSC_SRTEST=\
	".\RES\srtest.ico"\
	".\RES\srtest.rc2"\
	

"$(INTDIR)\Srtest.res" : $(SOURCE) $(DEP_RSC_SRTEST) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\STDAFX.CPP

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /D "_CRT_MAP_ALLOC" /Fr"$(INTDIR)\\"\
 /Fp"$(INTDIR)\Srtest.pch" /Yc /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\STDAFX.SBR"	"$(INTDIR)\Srtest.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yc /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\Srtest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\Srtest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\Srtest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yc /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\Srtest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yc /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\Srtest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yc /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\Srtest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yc /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\Srtest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Tabisrce.cpp

!IF  "$(CFG)" == "Srtest - Win32 A2ADebug"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /D "_CRT_MAP_ALLOC" /Fr"$(INTDIR)\\"\
 /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\Tabisrce.obj"	"$(INTDIR)\Tabisrce.sbr" : $(SOURCE) $(DEP_CPP_TABIS)\
 "$(INTDIR)" "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 A2ARel"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Tabisrce.obj" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WRel"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Tabisrce.obj" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 W2WDebug"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Tabisrce.obj" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ADebug"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Tabisrce.obj" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) A2ARel"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Tabisrce.obj" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WDebug"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Tabisrce.obj" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Srtest - Win32 (ALPHA) W2WRel"

DEP_CPP_TABIS=\
	".\DLGFINDM.H"\
	".\GRAMFUNC.H"\
	".\SRDLG.H"\
	".\SRFUNC.H"\
	".\SRTEST.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\Srtest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Tabisrce.obj" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\Srtest.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

