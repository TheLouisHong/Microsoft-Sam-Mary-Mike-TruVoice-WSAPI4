# Microsoft Developer Studio Generated NMAKE File, Based on vttest.dsp
!IF "$(CFG)" == ""
CFG=Vttest - Win32 A2ADebug
!MESSAGE No configuration specified. Defaulting to Vttest - Win32 A2ADebug.
!ENDIF 

!IF "$(CFG)" != "Vttest - Win32 A2ADebug" && "$(CFG)" !=\
 "Vttest - Win32 A2ARel" && "$(CFG)" != "Vttest - Win32 W2WDebug" && "$(CFG)" !=\
 "Vttest - Win32 W2WRel" && "$(CFG)" != "Vttest - Win32 (ALPHA) A2ADebug" &&\
 "$(CFG)" != "Vttest - Win32 (ALPHA) A2ARel" && "$(CFG)" !=\
 "Vttest - Win32 (ALPHA) W2WDebug" && "$(CFG)" !=\
 "Vttest - Win32 (ALPHA) W2WRel"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vttest.mak" CFG="Vttest - Win32 A2ADebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Vttest - Win32 A2ADebug" (based on "Win32 (x86) Application")
!MESSAGE "Vttest - Win32 A2ARel" (based on "Win32 (x86) Application")
!MESSAGE "Vttest - Win32 W2WDebug" (based on "Win32 (x86) Application")
!MESSAGE "Vttest - Win32 W2WRel" (based on "Win32 (x86) Application")
!MESSAGE "Vttest - Win32 (ALPHA) A2ADebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Vttest - Win32 (ALPHA) A2ARel" (based on "Win32 (ALPHA) Application")
!MESSAGE "Vttest - Win32 (ALPHA) W2WDebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Vttest - Win32 (ALPHA) W2WRel" (based on "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

OUTDIR=.\A2ADebug
INTDIR=.\A2ADebug
# Begin Custom Macros
OutDir=.\.\A2ADebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vttest.exe" "$(OUTDIR)\vttest.bsc"

!ELSE 

ALL : "$(OUTDIR)\vttest.exe" "$(OUTDIR)\vttest.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.SBR"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\STDAFX.SBR"
	-@erase "$(INTDIR)\TABDLGS.OBJ"
	-@erase "$(INTDIR)\TABDLGS.SBR"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\VTNOT.OBJ"
	-@erase "$(INTDIR)\VTNOT.SBR"
	-@erase "$(INTDIR)\VTOBJ.OBJ"
	-@erase "$(INTDIR)\VTOBJ.SBR"
	-@erase "$(INTDIR)\VTPICK.OBJ"
	-@erase "$(INTDIR)\VTPICK.SBR"
	-@erase "$(INTDIR)\vttesdlg.obj"
	-@erase "$(INTDIR)\vttesdlg.sbr"
	-@erase "$(INTDIR)\VTTEST.OBJ"
	-@erase "$(INTDIR)\vttest.pch"
	-@erase "$(INTDIR)\VTTEST.res"
	-@erase "$(INTDIR)\VTTEST.SBR"
	-@erase "$(OUTDIR)\vttest.bsc"
	-@erase "$(OUTDIR)\vttest.exe"
	-@erase "$(OUTDIR)\vttest.ilk"
	-@erase "$(OUTDIR)\vttest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\vttest.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VTTEST.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vttest.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\GUIDSEG.SBR" \
	"$(INTDIR)\STDAFX.SBR" \
	"$(INTDIR)\TABDLGS.SBR" \
	"$(INTDIR)\VTNOT.SBR" \
	"$(INTDIR)\VTOBJ.SBR" \
	"$(INTDIR)\VTPICK.SBR" \
	"$(INTDIR)\vttesdlg.sbr" \
	"$(INTDIR)\VTTEST.SBR"

"$(OUTDIR)\vttest.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ole32.lib /nologo /incremental:yes /pdb:"$(OUTDIR)\vttest.pdb"\
 /debug /machine:I386 /out:"$(OUTDIR)\vttest.exe" /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABDLGS.OBJ" \
	"$(INTDIR)\VTNOT.OBJ" \
	"$(INTDIR)\VTOBJ.OBJ" \
	"$(INTDIR)\VTPICK.OBJ" \
	"$(INTDIR)\vttesdlg.obj" \
	"$(INTDIR)\VTTEST.OBJ" \
	"$(INTDIR)\VTTEST.res"

"$(OUTDIR)\vttest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

OUTDIR=.\A2ARel
INTDIR=.\A2ARel
# Begin Custom Macros
OutDir=.\.\A2ARel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vttest.exe" "$(OUTDIR)\vttest.bsc"

!ELSE 

ALL : "$(OUTDIR)\vttest.exe" "$(OUTDIR)\vttest.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.SBR"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\STDAFX.SBR"
	-@erase "$(INTDIR)\TABDLGS.OBJ"
	-@erase "$(INTDIR)\TABDLGS.SBR"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\VTNOT.OBJ"
	-@erase "$(INTDIR)\VTNOT.SBR"
	-@erase "$(INTDIR)\VTOBJ.OBJ"
	-@erase "$(INTDIR)\VTOBJ.SBR"
	-@erase "$(INTDIR)\VTPICK.OBJ"
	-@erase "$(INTDIR)\VTPICK.SBR"
	-@erase "$(INTDIR)\vttesdlg.obj"
	-@erase "$(INTDIR)\vttesdlg.sbr"
	-@erase "$(INTDIR)\VTTEST.OBJ"
	-@erase "$(INTDIR)\vttest.pch"
	-@erase "$(INTDIR)\VTTEST.res"
	-@erase "$(INTDIR)\VTTEST.SBR"
	-@erase "$(OUTDIR)\vttest.bsc"
	-@erase "$(OUTDIR)\vttest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\vttest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\A2ARel/
CPP_SBRS=.\A2ARel/

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VTTEST.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vttest.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\GUIDSEG.SBR" \
	"$(INTDIR)\STDAFX.SBR" \
	"$(INTDIR)\TABDLGS.SBR" \
	"$(INTDIR)\VTNOT.SBR" \
	"$(INTDIR)\VTOBJ.SBR" \
	"$(INTDIR)\VTPICK.SBR" \
	"$(INTDIR)\vttesdlg.sbr" \
	"$(INTDIR)\VTTEST.SBR"

"$(OUTDIR)\vttest.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ole32.lib /nologo /incremental:no /pdb:"$(OUTDIR)\vttest.pdb"\
 /machine:I386 /out:"$(OUTDIR)\vttest.exe" /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABDLGS.OBJ" \
	"$(INTDIR)\VTNOT.OBJ" \
	"$(INTDIR)\VTOBJ.OBJ" \
	"$(INTDIR)\VTPICK.OBJ" \
	"$(INTDIR)\vttesdlg.obj" \
	"$(INTDIR)\VTTEST.OBJ" \
	"$(INTDIR)\VTTEST.res"

"$(OUTDIR)\vttest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

OUTDIR=.\W2WDebug
INTDIR=.\W2WDebug
# Begin Custom Macros
OutDir=.\.\W2WDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vttest.exe" "$(OUTDIR)\vttest.bsc"

!ELSE 

ALL : "$(OUTDIR)\vttest.exe" "$(OUTDIR)\vttest.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.SBR"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\STDAFX.SBR"
	-@erase "$(INTDIR)\TABDLGS.OBJ"
	-@erase "$(INTDIR)\TABDLGS.SBR"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\VTNOT.OBJ"
	-@erase "$(INTDIR)\VTNOT.SBR"
	-@erase "$(INTDIR)\VTOBJ.OBJ"
	-@erase "$(INTDIR)\VTOBJ.SBR"
	-@erase "$(INTDIR)\VTPICK.OBJ"
	-@erase "$(INTDIR)\VTPICK.SBR"
	-@erase "$(INTDIR)\vttesdlg.obj"
	-@erase "$(INTDIR)\vttesdlg.sbr"
	-@erase "$(INTDIR)\VTTEST.OBJ"
	-@erase "$(INTDIR)\vttest.pch"
	-@erase "$(INTDIR)\VTTEST.res"
	-@erase "$(INTDIR)\VTTEST.SBR"
	-@erase "$(OUTDIR)\vttest.bsc"
	-@erase "$(OUTDIR)\vttest.exe"
	-@erase "$(OUTDIR)\vttest.ilk"
	-@erase "$(OUTDIR)\vttest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_UNICODE" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\vttest.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\W2WDebug/
CPP_SBRS=.\W2WDebug/

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VTTEST.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vttest.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\GUIDSEG.SBR" \
	"$(INTDIR)\STDAFX.SBR" \
	"$(INTDIR)\TABDLGS.SBR" \
	"$(INTDIR)\VTNOT.SBR" \
	"$(INTDIR)\VTOBJ.SBR" \
	"$(INTDIR)\VTPICK.SBR" \
	"$(INTDIR)\vttesdlg.sbr" \
	"$(INTDIR)\VTTEST.SBR"

"$(OUTDIR)\vttest.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ole32.lib /nologo /entry:"wWinMainCRTStartup" /incremental:yes\
 /pdb:"$(OUTDIR)\vttest.pdb" /debug /machine:I386 /out:"$(OUTDIR)\vttest.exe"\
 /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABDLGS.OBJ" \
	"$(INTDIR)\VTNOT.OBJ" \
	"$(INTDIR)\VTOBJ.OBJ" \
	"$(INTDIR)\VTPICK.OBJ" \
	"$(INTDIR)\vttesdlg.obj" \
	"$(INTDIR)\VTTEST.OBJ" \
	"$(INTDIR)\VTTEST.res"

"$(OUTDIR)\vttest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

OUTDIR=.\W2WRel
INTDIR=.\W2WRel
# Begin Custom Macros
OutDir=.\.\W2WRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vttest.exe" "$(OUTDIR)\vttest.bsc"

!ELSE 

ALL : "$(OUTDIR)\vttest.exe" "$(OUTDIR)\vttest.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.SBR"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\STDAFX.SBR"
	-@erase "$(INTDIR)\TABDLGS.OBJ"
	-@erase "$(INTDIR)\TABDLGS.SBR"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\VTNOT.OBJ"
	-@erase "$(INTDIR)\VTNOT.SBR"
	-@erase "$(INTDIR)\VTOBJ.OBJ"
	-@erase "$(INTDIR)\VTOBJ.SBR"
	-@erase "$(INTDIR)\VTPICK.OBJ"
	-@erase "$(INTDIR)\VTPICK.SBR"
	-@erase "$(INTDIR)\vttesdlg.obj"
	-@erase "$(INTDIR)\vttesdlg.sbr"
	-@erase "$(INTDIR)\VTTEST.OBJ"
	-@erase "$(INTDIR)\vttest.pch"
	-@erase "$(INTDIR)\VTTEST.res"
	-@erase "$(INTDIR)\VTTEST.SBR"
	-@erase "$(OUTDIR)\vttest.bsc"
	-@erase "$(OUTDIR)\vttest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\vttest.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\W2WRel/
CPP_SBRS=.\W2WRel/

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VTTEST.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vttest.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\GUIDSEG.SBR" \
	"$(INTDIR)\STDAFX.SBR" \
	"$(INTDIR)\TABDLGS.SBR" \
	"$(INTDIR)\VTNOT.SBR" \
	"$(INTDIR)\VTOBJ.SBR" \
	"$(INTDIR)\VTPICK.SBR" \
	"$(INTDIR)\vttesdlg.sbr" \
	"$(INTDIR)\VTTEST.SBR"

"$(OUTDIR)\vttest.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ole32.lib /nologo /entry:"wWinMainCRTStartup" /incremental:no\
 /pdb:"$(OUTDIR)\vttest.pdb" /machine:I386 /out:"$(OUTDIR)\vttest.exe"\
 /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABDLGS.OBJ" \
	"$(INTDIR)\VTNOT.OBJ" \
	"$(INTDIR)\VTOBJ.OBJ" \
	"$(INTDIR)\VTPICK.OBJ" \
	"$(INTDIR)\vttesdlg.obj" \
	"$(INTDIR)\VTTEST.OBJ" \
	"$(INTDIR)\VTTEST.res"

"$(OUTDIR)\vttest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

OUTDIR=.\A2ADebug
INTDIR=.\A2ADebug
# Begin Custom Macros
OutDir=.\.\A2ADebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vttest.exe"

!ELSE 

ALL : "$(OUTDIR)\vttest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\TABDLGS.OBJ"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\VTNOT.OBJ"
	-@erase "$(INTDIR)\VTOBJ.OBJ"
	-@erase "$(INTDIR)\VTPICK.OBJ"
	-@erase "$(INTDIR)\vttesdlg.obj"
	-@erase "$(INTDIR)\VTTEST.OBJ"
	-@erase "$(INTDIR)\vttest.pch"
	-@erase "$(INTDIR)\VTTEST.res"
	-@erase "$(OUTDIR)\vttest.exe"
	-@erase "$(OUTDIR)\vttest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MTd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\vttest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VTTEST.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vttest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /subsystem:windows\
 /pdb:"$(OUTDIR)\vttest.pdb" /debug /machine:ALPHA /out:"$(OUTDIR)\vttest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABDLGS.OBJ" \
	"$(INTDIR)\VTNOT.OBJ" \
	"$(INTDIR)\VTOBJ.OBJ" \
	"$(INTDIR)\VTPICK.OBJ" \
	"$(INTDIR)\vttesdlg.obj" \
	"$(INTDIR)\VTTEST.OBJ" \
	"$(INTDIR)\VTTEST.res"

"$(OUTDIR)\vttest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

OUTDIR=.\A2ARel
INTDIR=.\A2ARel
# Begin Custom Macros
OutDir=.\.\A2ARel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vttest.exe"

!ELSE 

ALL : "$(OUTDIR)\vttest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\TABDLGS.OBJ"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\VTNOT.OBJ"
	-@erase "$(INTDIR)\VTOBJ.OBJ"
	-@erase "$(INTDIR)\VTPICK.OBJ"
	-@erase "$(INTDIR)\vttesdlg.obj"
	-@erase "$(INTDIR)\VTTEST.OBJ"
	-@erase "$(INTDIR)\vttest.pch"
	-@erase "$(INTDIR)\VTTEST.res"
	-@erase "$(OUTDIR)\vttest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\vttest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VTTEST.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vttest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib  ole32.lib /nologo /subsystem:windows\
 /pdb:"$(OUTDIR)\vttest.pdb" /machine:ALPHA /out:"$(OUTDIR)\vttest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABDLGS.OBJ" \
	"$(INTDIR)\VTNOT.OBJ" \
	"$(INTDIR)\VTOBJ.OBJ" \
	"$(INTDIR)\VTPICK.OBJ" \
	"$(INTDIR)\vttesdlg.obj" \
	"$(INTDIR)\VTTEST.OBJ" \
	"$(INTDIR)\VTTEST.res"

"$(OUTDIR)\vttest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

OUTDIR=.\W2WDebug
INTDIR=.\W2WDebug
# Begin Custom Macros
OutDir=.\.\W2WDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vttest.exe"

!ELSE 

ALL : "$(OUTDIR)\vttest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\TABDLGS.OBJ"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\VTNOT.OBJ"
	-@erase "$(INTDIR)\VTOBJ.OBJ"
	-@erase "$(INTDIR)\VTPICK.OBJ"
	-@erase "$(INTDIR)\vttesdlg.obj"
	-@erase "$(INTDIR)\VTTEST.OBJ"
	-@erase "$(INTDIR)\vttest.pch"
	-@erase "$(INTDIR)\VTTEST.res"
	-@erase "$(OUTDIR)\vttest.exe"
	-@erase "$(OUTDIR)\vttest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /D "_AFXDLL" /Fp"$(INTDIR)\vttest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VTTEST.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vttest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup"\
 /subsystem:windows /pdb:"$(OUTDIR)\vttest.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\vttest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABDLGS.OBJ" \
	"$(INTDIR)\VTNOT.OBJ" \
	"$(INTDIR)\VTOBJ.OBJ" \
	"$(INTDIR)\VTPICK.OBJ" \
	"$(INTDIR)\vttesdlg.obj" \
	"$(INTDIR)\VTTEST.OBJ" \
	"$(INTDIR)\VTTEST.res"

"$(OUTDIR)\vttest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

OUTDIR=.\W2WRel
INTDIR=.\W2WRel
# Begin Custom Macros
OutDir=.\.\W2WRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vttest.exe"

!ELSE 

ALL : "$(OUTDIR)\vttest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\TABDLGS.OBJ"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\VTNOT.OBJ"
	-@erase "$(INTDIR)\VTOBJ.OBJ"
	-@erase "$(INTDIR)\VTPICK.OBJ"
	-@erase "$(INTDIR)\vttesdlg.obj"
	-@erase "$(INTDIR)\VTTEST.OBJ"
	-@erase "$(INTDIR)\vttest.pch"
	-@erase "$(INTDIR)\VTTEST.res"
	-@erase "$(OUTDIR)\vttest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_UNICODE" /D "_AFXDLL" /Fp"$(INTDIR)\vttest.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VTTEST.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vttest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib  ole32.lib /nologo /entry:"wWinMainCRTStartup"\
 /subsystem:windows /pdb:"$(OUTDIR)\vttest.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\vttest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABDLGS.OBJ" \
	"$(INTDIR)\VTNOT.OBJ" \
	"$(INTDIR)\VTOBJ.OBJ" \
	"$(INTDIR)\VTPICK.OBJ" \
	"$(INTDIR)\vttesdlg.obj" \
	"$(INTDIR)\VTTEST.OBJ" \
	"$(INTDIR)\VTTEST.res"

"$(OUTDIR)\vttest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Vttest - Win32 A2ADebug" || "$(CFG)" ==\
 "Vttest - Win32 A2ARel" || "$(CFG)" == "Vttest - Win32 W2WDebug" || "$(CFG)" ==\
 "Vttest - Win32 W2WRel" || "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug" ||\
 "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel" || "$(CFG)" ==\
 "Vttest - Win32 (ALPHA) W2WDebug" || "$(CFG)" ==\
 "Vttest - Win32 (ALPHA) W2WRel"
SOURCE=.\GUIDSEG.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ"	"$(INTDIR)\GUIDSEG.SBR" : $(SOURCE) $(DEP_CPP_GUIDS)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ"	"$(INTDIR)\GUIDSEG.SBR" : $(SOURCE) $(DEP_CPP_GUIDS)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ"	"$(INTDIR)\GUIDSEG.SBR" : $(SOURCE) $(DEP_CPP_GUIDS)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\GUIDSEG.OBJ"	"$(INTDIR)\GUIDSEG.SBR" : $(SOURCE) $(DEP_CPP_GUIDS)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\STDAFX.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\vttest.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\STDAFX.SBR"	"$(INTDIR)\vttest.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\vttest.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\STDAFX.SBR"	"$(INTDIR)\vttest.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /FR"$(INTDIR)\\"\
 /Fp"$(INTDIR)\vttest.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\STDAFX.SBR"	"$(INTDIR)\vttest.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\vttest.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\STDAFX.SBR"	"$(INTDIR)\vttest.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\vttest.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\vttest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\vttest.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\vttest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /Fp"$(INTDIR)\vttest.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\vttest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /D "_AFXDLL" /Fp"$(INTDIR)\vttest.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\vttest.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TABDLGS.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\TABDLGS.OBJ"	"$(INTDIR)\TABDLGS.SBR" : $(SOURCE) $(DEP_CPP_TABDL)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\TABDLGS.OBJ"	"$(INTDIR)\TABDLGS.SBR" : $(SOURCE) $(DEP_CPP_TABDL)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\TABDLGS.OBJ"	"$(INTDIR)\TABDLGS.SBR" : $(SOURCE) $(DEP_CPP_TABDL)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\TABDLGS.OBJ"	"$(INTDIR)\TABDLGS.SBR" : $(SOURCE) $(DEP_CPP_TABDL)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\TABDLGS.OBJ" : $(SOURCE) $(DEP_CPP_TABDL) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\TABDLGS.OBJ" : $(SOURCE) $(DEP_CPP_TABDL) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\TABDLGS.OBJ" : $(SOURCE) $(DEP_CPP_TABDL) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_TABDL=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	".\VTTESDLG.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\TABDLGS.OBJ" : $(SOURCE) $(DEP_CPP_TABDL) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ENDIF 

SOURCE=.\VTNOT.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTNOT.OBJ"	"$(INTDIR)\VTNOT.SBR" : $(SOURCE) $(DEP_CPP_VTNOT)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTNOT.OBJ"	"$(INTDIR)\VTNOT.SBR" : $(SOURCE) $(DEP_CPP_VTNOT)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTNOT.OBJ"	"$(INTDIR)\VTNOT.SBR" : $(SOURCE) $(DEP_CPP_VTNOT)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTNOT.OBJ"	"$(INTDIR)\VTNOT.SBR" : $(SOURCE) $(DEP_CPP_VTNOT)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTNOT.OBJ" : $(SOURCE) $(DEP_CPP_VTNOT) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTNOT.OBJ" : $(SOURCE) $(DEP_CPP_VTNOT) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTNOT.OBJ" : $(SOURCE) $(DEP_CPP_VTNOT) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_VTNOT=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTNOT.OBJ" : $(SOURCE) $(DEP_CPP_VTNOT) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ENDIF 

SOURCE=.\VTOBJ.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTOBJ.OBJ"	"$(INTDIR)\VTOBJ.SBR" : $(SOURCE) $(DEP_CPP_VTOBJ)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTOBJ.OBJ"	"$(INTDIR)\VTOBJ.SBR" : $(SOURCE) $(DEP_CPP_VTOBJ)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTOBJ.OBJ"	"$(INTDIR)\VTOBJ.SBR" : $(SOURCE) $(DEP_CPP_VTOBJ)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTOBJ.OBJ"	"$(INTDIR)\VTOBJ.SBR" : $(SOURCE) $(DEP_CPP_VTOBJ)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTOBJ.OBJ" : $(SOURCE) $(DEP_CPP_VTOBJ) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTOBJ.OBJ" : $(SOURCE) $(DEP_CPP_VTOBJ) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTOBJ.OBJ" : $(SOURCE) $(DEP_CPP_VTOBJ) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_VTOBJ=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTOBJ.OBJ" : $(SOURCE) $(DEP_CPP_VTOBJ) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ENDIF 

SOURCE=.\VTPICK.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTPICK.OBJ"	"$(INTDIR)\VTPICK.SBR" : $(SOURCE) $(DEP_CPP_VTPIC)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTPICK.OBJ"	"$(INTDIR)\VTPICK.SBR" : $(SOURCE) $(DEP_CPP_VTPIC)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTPICK.OBJ"	"$(INTDIR)\VTPICK.SBR" : $(SOURCE) $(DEP_CPP_VTPIC)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTPICK.OBJ"	"$(INTDIR)\VTPICK.SBR" : $(SOURCE) $(DEP_CPP_VTPIC)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTPICK.OBJ" : $(SOURCE) $(DEP_CPP_VTPIC) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTPICK.OBJ" : $(SOURCE) $(DEP_CPP_VTPIC) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTPICK.OBJ" : $(SOURCE) $(DEP_CPP_VTPIC) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_VTPIC=\
	".\STDAFX.H"\
	".\VTOBJ.H"\
	".\VTPICK.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTPICK.OBJ" : $(SOURCE) $(DEP_CPP_VTPIC) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ENDIF 

SOURCE=.\vttesdlg.cpp

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\vttesdlg.obj"	"$(INTDIR)\vttesdlg.sbr" : $(SOURCE) $(DEP_CPP_VTTES)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\vttesdlg.obj"	"$(INTDIR)\vttesdlg.sbr" : $(SOURCE) $(DEP_CPP_VTTES)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\vttesdlg.obj"	"$(INTDIR)\vttesdlg.sbr" : $(SOURCE) $(DEP_CPP_VTTES)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\vttesdlg.obj"	"$(INTDIR)\vttesdlg.sbr" : $(SOURCE) $(DEP_CPP_VTTES)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\vttesdlg.obj" : $(SOURCE) $(DEP_CPP_VTTES) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\vttesdlg.obj" : $(SOURCE) $(DEP_CPP_VTTES) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\vttesdlg.obj" : $(SOURCE) $(DEP_CPP_VTTES) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_VTTES=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\vttesdlg.obj" : $(SOURCE) $(DEP_CPP_VTTES) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ENDIF 

SOURCE=.\VTTEST.CPP

!IF  "$(CFG)" == "Vttest - Win32 A2ADebug"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTTEST.OBJ"	"$(INTDIR)\VTTEST.SBR" : $(SOURCE) $(DEP_CPP_VTTEST)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 A2ARel"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTTEST.OBJ"	"$(INTDIR)\VTTEST.SBR" : $(SOURCE) $(DEP_CPP_VTTEST)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WDebug"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTTEST.OBJ"	"$(INTDIR)\VTTEST.SBR" : $(SOURCE) $(DEP_CPP_VTTEST)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 W2WRel"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTTEST.OBJ"	"$(INTDIR)\VTTEST.SBR" : $(SOURCE) $(DEP_CPP_VTTEST)\
 "$(INTDIR)" "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ADebug"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTTEST.OBJ" : $(SOURCE) $(DEP_CPP_VTTEST) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) A2ARel"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTTEST.OBJ" : $(SOURCE) $(DEP_CPP_VTTEST) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WDebug"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTTEST.OBJ" : $(SOURCE) $(DEP_CPP_VTTEST) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ELSEIF  "$(CFG)" == "Vttest - Win32 (ALPHA) W2WRel"

DEP_CPP_VTTEST=\
	".\STDAFX.H"\
	".\TABDLGS.H"\
	".\VTOBJ.H"\
	".\VTTESDLG.H"\
	".\VTTEST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\VTTEST.OBJ" : $(SOURCE) $(DEP_CPP_VTTEST) "$(INTDIR)"\
 "$(INTDIR)\vttest.pch"


!ENDIF 

SOURCE=.\VTTEST.RC
DEP_RSC_VTTEST_=\
	".\RES\VTTEST.ICO"\
	".\RES\VTTEST.RC2"\
	

"$(INTDIR)\VTTEST.res" : $(SOURCE) $(DEP_RSC_VTTEST_) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

