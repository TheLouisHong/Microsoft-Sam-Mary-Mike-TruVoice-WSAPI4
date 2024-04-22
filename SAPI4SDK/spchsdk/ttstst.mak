# Microsoft Developer Studio Generated NMAKE File, Based on ttstst.dsp
!IF "$(CFG)" == ""
CFG=Ttstst - Win32 A2ADebug
!MESSAGE No configuration specified. Defaulting to Ttstst - Win32 A2ADebug.
!ENDIF 

!IF "$(CFG)" != "Ttstst - Win32 A2ADebug" && "$(CFG)" !=\
 "Ttstst - Win32 A2ARel" && "$(CFG)" != "Ttstst - Win32 W2WDebug" && "$(CFG)" !=\
 "Ttstst - Win32 W2WRel" && "$(CFG)" != "Ttstst - Win32 (ALPHA) A2ADebug" &&\
 "$(CFG)" != "Ttstst - Win32 (ALPHA) A2ARel" && "$(CFG)" !=\
 "Ttstst - Win32 (ALPHA) W2WDebug" && "$(CFG)" !=\
 "Ttstst - Win32 (ALPHA) W2WRel"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ttstst.mak" CFG="Ttstst - Win32 A2ADebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ttstst - Win32 A2ADebug" (based on "Win32 (x86) Application")
!MESSAGE "Ttstst - Win32 A2ARel" (based on "Win32 (x86) Application")
!MESSAGE "Ttstst - Win32 W2WDebug" (based on "Win32 (x86) Application")
!MESSAGE "Ttstst - Win32 W2WRel" (based on "Win32 (x86) Application")
!MESSAGE "Ttstst - Win32 (ALPHA) A2ADebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Ttstst - Win32 (ALPHA) A2ARel" (based on "Win32 (ALPHA) Application")
!MESSAGE "Ttstst - Win32 (ALPHA) W2WDebug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "Ttstst - Win32 (ALPHA) W2WRel" (based on "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

OUTDIR=.\A2ADebug
INTDIR=.\A2ADebug
# Begin Custom Macros
OutDir=.\.\A2ADebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ttstst.exe" "$(OUTDIR)\ttstst.bsc"

!ELSE 

ALL : "$(OUTDIR)\ttstst.exe" "$(OUTDIR)\ttstst.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\DLGFINDM.SBR"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.SBR"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\STDAFX.SBR"
	-@erase "$(INTDIR)\TABISRCE.OBJ"
	-@erase "$(INTDIR)\TABISRCE.SBR"
	-@erase "$(INTDIR)\TTSDLG.OBJ"
	-@erase "$(INTDIR)\TTSDLG.SBR"
	-@erase "$(INTDIR)\TTSFNC.OBJ"
	-@erase "$(INTDIR)\TTSFNC.SBR"
	-@erase "$(INTDIR)\TTSTST.OBJ"
	-@erase "$(INTDIR)\ttstst.pch"
	-@erase "$(INTDIR)\TTSTST.res"
	-@erase "$(INTDIR)\TTSTST.SBR"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\ttstst.bsc"
	-@erase "$(OUTDIR)\ttstst.exe"
	-@erase "$(OUTDIR)\ttstst.ilk"
	-@erase "$(OUTDIR)\ttstst.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSTST.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttstst.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DLGFINDM.SBR" \
	"$(INTDIR)\GUIDSEG.SBR" \
	"$(INTDIR)\STDAFX.SBR" \
	"$(INTDIR)\TABISRCE.SBR" \
	"$(INTDIR)\TTSDLG.SBR" \
	"$(INTDIR)\TTSFNC.SBR" \
	"$(INTDIR)\TTSTST.SBR"

"$(OUTDIR)\ttstst.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /incremental:yes\
 /pdb:"$(OUTDIR)\ttstst.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ttstst.exe"\
 /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABISRCE.OBJ" \
	"$(INTDIR)\TTSDLG.OBJ" \
	"$(INTDIR)\TTSFNC.OBJ" \
	"$(INTDIR)\TTSTST.OBJ" \
	"$(INTDIR)\TTSTST.res"

"$(OUTDIR)\ttstst.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

OUTDIR=.\A2ARel
INTDIR=.\A2ARel
# Begin Custom Macros
OutDir=.\.\A2ARel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ttstst.exe" "$(OUTDIR)\ttstst.bsc"

!ELSE 

ALL : "$(OUTDIR)\ttstst.exe" "$(OUTDIR)\ttstst.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\DLGFINDM.SBR"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.SBR"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\STDAFX.SBR"
	-@erase "$(INTDIR)\TABISRCE.OBJ"
	-@erase "$(INTDIR)\TABISRCE.SBR"
	-@erase "$(INTDIR)\TTSDLG.OBJ"
	-@erase "$(INTDIR)\TTSDLG.SBR"
	-@erase "$(INTDIR)\TTSFNC.OBJ"
	-@erase "$(INTDIR)\TTSFNC.SBR"
	-@erase "$(INTDIR)\TTSTST.OBJ"
	-@erase "$(INTDIR)\ttstst.pch"
	-@erase "$(INTDIR)\TTSTST.res"
	-@erase "$(INTDIR)\TTSTST.SBR"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\ttstst.bsc"
	-@erase "$(OUTDIR)\ttstst.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSTST.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttstst.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DLGFINDM.SBR" \
	"$(INTDIR)\GUIDSEG.SBR" \
	"$(INTDIR)\STDAFX.SBR" \
	"$(INTDIR)\TABISRCE.SBR" \
	"$(INTDIR)\TTSDLG.SBR" \
	"$(INTDIR)\TTSFNC.SBR" \
	"$(INTDIR)\TTSTST.SBR"

"$(OUTDIR)\ttstst.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /incremental:no\
 /pdb:"$(OUTDIR)\ttstst.pdb" /machine:I386 /out:"$(OUTDIR)\ttstst.exe"\
 /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABISRCE.OBJ" \
	"$(INTDIR)\TTSDLG.OBJ" \
	"$(INTDIR)\TTSFNC.OBJ" \
	"$(INTDIR)\TTSTST.OBJ" \
	"$(INTDIR)\TTSTST.res"

"$(OUTDIR)\ttstst.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

OUTDIR=.\W2WDebug
INTDIR=.\W2WDebug
# Begin Custom Macros
OutDir=.\.\W2WDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ttstst.exe" "$(OUTDIR)\ttstst.bsc"

!ELSE 

ALL : "$(OUTDIR)\ttstst.exe" "$(OUTDIR)\ttstst.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\DLGFINDM.SBR"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.SBR"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\STDAFX.SBR"
	-@erase "$(INTDIR)\TABISRCE.OBJ"
	-@erase "$(INTDIR)\TABISRCE.SBR"
	-@erase "$(INTDIR)\TTSDLG.OBJ"
	-@erase "$(INTDIR)\TTSDLG.SBR"
	-@erase "$(INTDIR)\TTSFNC.OBJ"
	-@erase "$(INTDIR)\TTSFNC.SBR"
	-@erase "$(INTDIR)\TTSTST.OBJ"
	-@erase "$(INTDIR)\ttstst.pch"
	-@erase "$(INTDIR)\TTSTST.res"
	-@erase "$(INTDIR)\TTSTST.SBR"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\ttstst.bsc"
	-@erase "$(OUTDIR)\ttstst.exe"
	-@erase "$(OUTDIR)\ttstst.ilk"
	-@erase "$(OUTDIR)\ttstst.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSTST.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttstst.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DLGFINDM.SBR" \
	"$(INTDIR)\GUIDSEG.SBR" \
	"$(INTDIR)\STDAFX.SBR" \
	"$(INTDIR)\TABISRCE.SBR" \
	"$(INTDIR)\TTSDLG.SBR" \
	"$(INTDIR)\TTSFNC.SBR" \
	"$(INTDIR)\TTSTST.SBR"

"$(OUTDIR)\ttstst.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup"\
 /incremental:yes /pdb:"$(OUTDIR)\ttstst.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\ttstst.exe" /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABISRCE.OBJ" \
	"$(INTDIR)\TTSDLG.OBJ" \
	"$(INTDIR)\TTSFNC.OBJ" \
	"$(INTDIR)\TTSTST.OBJ" \
	"$(INTDIR)\TTSTST.res"

"$(OUTDIR)\ttstst.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

OUTDIR=.\W2WRel
INTDIR=.\W2WRel
# Begin Custom Macros
OutDir=.\.\W2WRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ttstst.exe" "$(OUTDIR)\ttstst.bsc"

!ELSE 

ALL : "$(OUTDIR)\ttstst.exe" "$(OUTDIR)\ttstst.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\DLGFINDM.SBR"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.SBR"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\STDAFX.SBR"
	-@erase "$(INTDIR)\TABISRCE.OBJ"
	-@erase "$(INTDIR)\TABISRCE.SBR"
	-@erase "$(INTDIR)\TTSDLG.OBJ"
	-@erase "$(INTDIR)\TTSDLG.SBR"
	-@erase "$(INTDIR)\TTSFNC.OBJ"
	-@erase "$(INTDIR)\TTSFNC.SBR"
	-@erase "$(INTDIR)\TTSTST.OBJ"
	-@erase "$(INTDIR)\ttstst.pch"
	-@erase "$(INTDIR)\TTSTST.res"
	-@erase "$(INTDIR)\TTSTST.SBR"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\ttstst.bsc"
	-@erase "$(OUTDIR)\ttstst.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSTST.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttstst.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DLGFINDM.SBR" \
	"$(INTDIR)\GUIDSEG.SBR" \
	"$(INTDIR)\STDAFX.SBR" \
	"$(INTDIR)\TABISRCE.SBR" \
	"$(INTDIR)\TTSDLG.SBR" \
	"$(INTDIR)\TTSFNC.SBR" \
	"$(INTDIR)\TTSTST.SBR"

"$(OUTDIR)\ttstst.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup"\
 /incremental:no /pdb:"$(OUTDIR)\ttstst.pdb" /machine:I386\
 /out:"$(OUTDIR)\ttstst.exe" /SUBSYSTEM:windows,4.0 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABISRCE.OBJ" \
	"$(INTDIR)\TTSDLG.OBJ" \
	"$(INTDIR)\TTSFNC.OBJ" \
	"$(INTDIR)\TTSTST.OBJ" \
	"$(INTDIR)\TTSTST.res"

"$(OUTDIR)\ttstst.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

OUTDIR=.\A2ADebug
INTDIR=.\A2ADebug
# Begin Custom Macros
OutDir=.\.\A2ADebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ttstst.exe"

!ELSE 

ALL : "$(OUTDIR)\ttstst.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\TABISRCE.OBJ"
	-@erase "$(INTDIR)\TTSDLG.OBJ"
	-@erase "$(INTDIR)\TTSFNC.OBJ"
	-@erase "$(INTDIR)\TTSTST.OBJ"
	-@erase "$(INTDIR)\ttstst.pch"
	-@erase "$(INTDIR)\TTSTST.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\ttstst.exe"
	-@erase "$(OUTDIR)\ttstst.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /MDd /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSTST.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttstst.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /subsystem:windows\
 /pdb:"$(OUTDIR)\ttstst.pdb" /debug /machine:ALPHA /out:"$(OUTDIR)\ttstst.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABISRCE.OBJ" \
	"$(INTDIR)\TTSDLG.OBJ" \
	"$(INTDIR)\TTSFNC.OBJ" \
	"$(INTDIR)\TTSTST.OBJ" \
	"$(INTDIR)\TTSTST.res"

"$(OUTDIR)\ttstst.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

OUTDIR=.\A2ARel
INTDIR=.\A2ARel
# Begin Custom Macros
OutDir=.\.\A2ARel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ttstst.exe"

!ELSE 

ALL : "$(OUTDIR)\ttstst.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\TABISRCE.OBJ"
	-@erase "$(INTDIR)\TTSDLG.OBJ"
	-@erase "$(INTDIR)\TTSFNC.OBJ"
	-@erase "$(INTDIR)\TTSTST.OBJ"
	-@erase "$(INTDIR)\ttstst.pch"
	-@erase "$(INTDIR)\TTSTST.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\ttstst.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSTST.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttstst.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /subsystem:windows\
 /pdb:"$(OUTDIR)\ttstst.pdb" /machine:ALPHA /out:"$(OUTDIR)\ttstst.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABISRCE.OBJ" \
	"$(INTDIR)\TTSDLG.OBJ" \
	"$(INTDIR)\TTSFNC.OBJ" \
	"$(INTDIR)\TTSTST.OBJ" \
	"$(INTDIR)\TTSTST.res"

"$(OUTDIR)\ttstst.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

OUTDIR=.\W2WDebug
INTDIR=.\W2WDebug
# Begin Custom Macros
OutDir=.\.\W2WDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ttstst.exe"

!ELSE 

ALL : "$(OUTDIR)\ttstst.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\TABISRCE.OBJ"
	-@erase "$(INTDIR)\TTSDLG.OBJ"
	-@erase "$(INTDIR)\TTSFNC.OBJ"
	-@erase "$(INTDIR)\TTSTST.OBJ"
	-@erase "$(INTDIR)\ttstst.pch"
	-@erase "$(INTDIR)\TTSTST.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\ttstst.exe"
	-@erase "$(OUTDIR)\ttstst.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D\
 "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /MDd /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSTST.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttstst.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup"\
 /subsystem:windows /pdb:"$(OUTDIR)\ttstst.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\ttstst.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABISRCE.OBJ" \
	"$(INTDIR)\TTSDLG.OBJ" \
	"$(INTDIR)\TTSFNC.OBJ" \
	"$(INTDIR)\TTSTST.OBJ" \
	"$(INTDIR)\TTSTST.res"

"$(OUTDIR)\ttstst.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

OUTDIR=.\W2WRel
INTDIR=.\W2WRel
# Begin Custom Macros
OutDir=.\.\W2WRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ttstst.exe"

!ELSE 

ALL : "$(OUTDIR)\ttstst.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DLGFINDM.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\STDAFX.OBJ"
	-@erase "$(INTDIR)\TABISRCE.OBJ"
	-@erase "$(INTDIR)\TTSDLG.OBJ"
	-@erase "$(INTDIR)\TTSFNC.OBJ"
	-@erase "$(INTDIR)\TTSTST.OBJ"
	-@erase "$(INTDIR)\ttstst.pch"
	-@erase "$(INTDIR)\TTSTST.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\ttstst.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSTST.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttstst.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ole32.lib /nologo /entry:"wWinMainCRTStartup"\
 /subsystem:windows /pdb:"$(OUTDIR)\ttstst.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\ttstst.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLGFINDM.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ" \
	"$(INTDIR)\STDAFX.OBJ" \
	"$(INTDIR)\TABISRCE.OBJ" \
	"$(INTDIR)\TTSDLG.OBJ" \
	"$(INTDIR)\TTSFNC.OBJ" \
	"$(INTDIR)\TTSTST.OBJ" \
	"$(INTDIR)\TTSTST.res"

"$(OUTDIR)\ttstst.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Ttstst - Win32 A2ADebug" || "$(CFG)" ==\
 "Ttstst - Win32 A2ARel" || "$(CFG)" == "Ttstst - Win32 W2WDebug" || "$(CFG)" ==\
 "Ttstst - Win32 W2WRel" || "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug" ||\
 "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel" || "$(CFG)" ==\
 "Ttstst - Win32 (ALPHA) W2WDebug" || "$(CFG)" ==\
 "Ttstst - Win32 (ALPHA) W2WRel"
SOURCE=.\DLGFINDM.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ"	"$(INTDIR)\DLGFINDM.SBR" : $(SOURCE) $(DEP_CPP_DLGFI)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ"	"$(INTDIR)\DLGFINDM.SBR" : $(SOURCE) $(DEP_CPP_DLGFI)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\"\
 /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\DLGFINDM.OBJ"	"$(INTDIR)\DLGFINDM.SBR" : $(SOURCE) $(DEP_CPP_DLGFI)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ"	"$(INTDIR)\DLGFINDM.SBR" : $(SOURCE) $(DEP_CPP_DLGFI)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_DLGFI=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DLGFINDM.OBJ" : $(SOURCE) $(DEP_CPP_DLGFI) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\GUIDSEG.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c\
 

"$(INTDIR)\GUIDSEG.OBJ"	"$(INTDIR)\GUIDSEG.SBR" : $(SOURCE) $(DEP_CPP_GUIDS)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ"	"$(INTDIR)\GUIDSEG.SBR" : $(SOURCE) $(DEP_CPP_GUIDS)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ"	"$(INTDIR)\GUIDSEG.SBR" : $(SOURCE) $(DEP_CPP_GUIDS)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\GUIDSEG.OBJ"	"$(INTDIR)\GUIDSEG.SBR" : $(SOURCE) $(DEP_CPP_GUIDS)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\STDAFX.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\STDAFX.SBR"	"$(INTDIR)\ttstst.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\STDAFX.SBR"	"$(INTDIR)\ttstst.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\"\
 /Fp"$(INTDIR)\ttstst.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\STDAFX.SBR"	"$(INTDIR)\ttstst.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\STDAFX.SBR"	"$(INTDIR)\ttstst.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yc /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /MDd /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\ttstst.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yc /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\ttstst.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yc /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /MDd /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\ttstst.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_STDAF=\
	".\STDAFX.H"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yc /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 

"$(INTDIR)\STDAFX.OBJ"	"$(INTDIR)\ttstst.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TABISRCE.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TABISRCE.OBJ"	"$(INTDIR)\TABISRCE.SBR" : $(SOURCE) $(DEP_CPP_TABIS)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TABISRCE.OBJ"	"$(INTDIR)\TABISRCE.SBR" : $(SOURCE) $(DEP_CPP_TABIS)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\"\
 /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\TABISRCE.OBJ"	"$(INTDIR)\TABISRCE.SBR" : $(SOURCE) $(DEP_CPP_TABIS)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TABISRCE.OBJ"	"$(INTDIR)\TABISRCE.SBR" : $(SOURCE) $(DEP_CPP_TABIS)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\TABISRCE.OBJ" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TABISRCE.OBJ" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\TABISRCE.OBJ" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_TABIS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TABISRCE.OBJ" : $(SOURCE) $(DEP_CPP_TABIS) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TTSDLG.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSDLG.OBJ"	"$(INTDIR)\TTSDLG.SBR" : $(SOURCE) $(DEP_CPP_TTSDL)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSDLG.OBJ"	"$(INTDIR)\TTSDLG.SBR" : $(SOURCE) $(DEP_CPP_TTSDL)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\"\
 /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\TTSDLG.OBJ"	"$(INTDIR)\TTSDLG.SBR" : $(SOURCE) $(DEP_CPP_TTSDL)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSDLG.OBJ"	"$(INTDIR)\TTSDLG.SBR" : $(SOURCE) $(DEP_CPP_TTSDL)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\TTSDLG.OBJ" : $(SOURCE) $(DEP_CPP_TTSDL) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSDLG.OBJ" : $(SOURCE) $(DEP_CPP_TTSDL) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\TTSDLG.OBJ" : $(SOURCE) $(DEP_CPP_TTSDL) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_TTSDL=\
	".\DLGFINDM.H"\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSDLG.OBJ" : $(SOURCE) $(DEP_CPP_TTSDL) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TTSFNC.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSFNC.OBJ"	"$(INTDIR)\TTSFNC.SBR" : $(SOURCE) $(DEP_CPP_TTSFN)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSFNC.OBJ"	"$(INTDIR)\TTSFNC.SBR" : $(SOURCE) $(DEP_CPP_TTSFN)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\"\
 /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\TTSFNC.OBJ"	"$(INTDIR)\TTSFNC.SBR" : $(SOURCE) $(DEP_CPP_TTSFN)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSFNC.OBJ"	"$(INTDIR)\TTSFNC.SBR" : $(SOURCE) $(DEP_CPP_TTSFN)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\TTSFNC.OBJ" : $(SOURCE) $(DEP_CPP_TTSFN) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSFNC.OBJ" : $(SOURCE) $(DEP_CPP_TTSFN) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\TTSFNC.OBJ" : $(SOURCE) $(DEP_CPP_TTSFN) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_TTSFN=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSFNC.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"bufcvt.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSFNC.OBJ" : $(SOURCE) $(DEP_CPP_TTSFN) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TTSTST.CPP

!IF  "$(CFG)" == "Ttstst - Win32 A2ADebug"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSTST.OBJ"	"$(INTDIR)\TTSTST.SBR" : $(SOURCE) $(DEP_CPP_TTSTS)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 A2ARel"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSTST.OBJ"	"$(INTDIR)\TTSTST.SBR" : $(SOURCE) $(DEP_CPP_TTSTS)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WDebug"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\"\
 /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\TTSTST.OBJ"	"$(INTDIR)\TTSTST.SBR" : $(SOURCE) $(DEP_CPP_TTSTS)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 W2WRel"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttstst.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSTST.OBJ"	"$(INTDIR)\TTSTST.SBR" : $(SOURCE) $(DEP_CPP_TTSTS)\
 "$(INTDIR)" "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ADebug"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\TTSTST.OBJ" : $(SOURCE) $(DEP_CPP_TTSTS) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) A2ARel"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSTST.OBJ" : $(SOURCE) $(DEP_CPP_TTSTS) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WDebug"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\TTSTST.OBJ" : $(SOURCE) $(DEP_CPP_TTSTS) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttstst - Win32 (ALPHA) W2WRel"

DEP_CPP_TTSTS=\
	".\STDAFX.H"\
	".\TABISRCE.H"\
	".\TTSDLG.H"\
	".\TTSTST.H"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /Fp"$(INTDIR)\ttstst.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSTST.OBJ" : $(SOURCE) $(DEP_CPP_TTSTS) "$(INTDIR)"\
 "$(INTDIR)\ttstst.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TTSTST.RC
DEP_RSC_TTSTST=\
	".\RES\TTSTST.ICO"\
	".\RES\TTSTST.RC2"\
	

"$(INTDIR)\TTSTST.res" : $(SOURCE) $(DEP_RSC_TTSTST) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

