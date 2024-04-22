# Microsoft Developer Studio Generated NMAKE File, Based on srfake.dsp
!IF "$(CFG)" == ""
CFG=srfake - Win32 Release
!MESSAGE No configuration specified. Defaulting to srfake - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "srfake - Win32 Release" && "$(CFG)" != "srfake - Win32 Debug"\
 && "$(CFG)" != "srfake - Win32 (ALPHA) Debug" && "$(CFG)" !=\
 "srfake - Win32 (ALPHA) Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "srfake.mak" CFG="srfake - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "srfake - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "srfake - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "srfake - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "srfake - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "srfake - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\srfake.dll"

!ELSE 

ALL : "$(OUTDIR)\srfake.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\fakesr.obj"
	-@erase "$(INTDIR)\fakesr.res"
	-@erase "$(INTDIR)\fakesrcf.obj"
	-@erase "$(INTDIR)\fakesren.obj"
	-@erase "$(INTDIR)\fakesrgr.obj"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\srfake.dll"
	-@erase "$(OUTDIR)\srfake.exp"
	-@erase "$(OUTDIR)\srfake.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I {$(BUILDDRV)}"\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\srfake.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\WinRel/
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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\fakesr.res" /i {$(BUILDDRV)}"\include" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\srfake.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\srfake.pdb"\
 /machine:I386 /def:".\srfake.def" /out:"$(OUTDIR)\srfake.dll"\
 /implib:"$(OUTDIR)\srfake.lib" 
DEF_FILE= \
	".\srfake.def"
LINK32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\fakesr.obj" \
	"$(INTDIR)\fakesr.res" \
	"$(INTDIR)\fakesrcf.obj" \
	"$(INTDIR)\fakesren.obj" \
	"$(INTDIR)\fakesrgr.obj" \
	"$(INTDIR)\guidseg.obj"

"$(OUTDIR)\srfake.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "srfake - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\srfake.dll"

!ELSE 

ALL : "$(OUTDIR)\srfake.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\fakesr.obj"
	-@erase "$(INTDIR)\fakesr.res"
	-@erase "$(INTDIR)\fakesrcf.obj"
	-@erase "$(INTDIR)\fakesren.obj"
	-@erase "$(INTDIR)\fakesrgr.obj"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\srfake.dll"
	-@erase "$(OUTDIR)\srfake.exp"
	-@erase "$(OUTDIR)\srfake.ilk"
	-@erase "$(OUTDIR)\srfake.lib"
	-@erase "$(OUTDIR)\srfake.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I {$(BUILDDRV)}"\include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\srfake.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\WinDebug/
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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\fakesr.res" /i {$(BUILDDRV)}"\include" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\srfake.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\srfake.pdb" /debug\
 /machine:I386 /def:".\srfake.def" /out:"$(OUTDIR)\srfake.dll"\
 /implib:"$(OUTDIR)\srfake.lib" 
DEF_FILE= \
	".\srfake.def"
LINK32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\fakesr.obj" \
	"$(INTDIR)\fakesr.res" \
	"$(INTDIR)\fakesrcf.obj" \
	"$(INTDIR)\fakesren.obj" \
	"$(INTDIR)\fakesrgr.obj" \
	"$(INTDIR)\guidseg.obj"

"$(OUTDIR)\srfake.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\srfake.dll"

!ELSE 

ALL : "$(OUTDIR)\srfake.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\fakesr.obj"
	-@erase "$(INTDIR)\fakesr.res"
	-@erase "$(INTDIR)\fakesrcf.obj"
	-@erase "$(INTDIR)\fakesren.obj"
	-@erase "$(INTDIR)\fakesrgr.obj"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\srfake.dll"
	-@erase "$(OUTDIR)\srfake.exp"
	-@erase "$(OUTDIR)\srfake.lib"
	-@erase "$(OUTDIR)\srfake.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /I {$(BUILDDRV)}"\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\srfake.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /MTd /c 
CPP_OBJS=.\AlphaDbg/
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\fakesr.res" /i {$(BUILDDRV)}"\include" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\srfake.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /pdb:"$(OUTDIR)\srfake.pdb" /debug\
 /machine:ALPHA /def:".\srfake.def" /out:"$(OUTDIR)\srfake.dll"\
 /implib:"$(OUTDIR)\srfake.lib" 
DEF_FILE= \
	".\srfake.def"
LINK32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\fakesr.obj" \
	"$(INTDIR)\fakesr.res" \
	"$(INTDIR)\fakesrcf.obj" \
	"$(INTDIR)\fakesren.obj" \
	"$(INTDIR)\fakesrgr.obj" \
	"$(INTDIR)\guidseg.obj"

"$(OUTDIR)\srfake.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\srfake.dll"

!ELSE 

ALL : "$(OUTDIR)\srfake.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\fakesr.obj"
	-@erase "$(INTDIR)\fakesr.res"
	-@erase "$(INTDIR)\fakesrcf.obj"
	-@erase "$(INTDIR)\fakesren.obj"
	-@erase "$(INTDIR)\fakesrgr.obj"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\srfake.dll"
	-@erase "$(OUTDIR)\srfake.exp"
	-@erase "$(OUTDIR)\srfake.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MT /Gt0 /W3 /GX /O2 /I {$(BUILDDRV)}"\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\srfake.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\AlphaRel/
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\fakesr.res" /i {$(BUILDDRV)}"\include" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\srfake.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /pdb:"$(OUTDIR)\srfake.pdb"\
 /machine:ALPHA /def:".\srfake.def" /out:"$(OUTDIR)\srfake.dll"\
 /implib:"$(OUTDIR)\srfake.lib" 
DEF_FILE= \
	".\srfake.def"
LINK32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\fakesr.obj" \
	"$(INTDIR)\fakesr.res" \
	"$(INTDIR)\fakesrcf.obj" \
	"$(INTDIR)\fakesren.obj" \
	"$(INTDIR)\fakesrgr.obj" \
	"$(INTDIR)\guidseg.obj"

"$(OUTDIR)\srfake.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "srfake - Win32 Release" || "$(CFG)" == "srfake - Win32 Debug"\
 || "$(CFG)" == "srfake - Win32 (ALPHA) Debug" || "$(CFG)" ==\
 "srfake - Win32 (ALPHA) Release"
SOURCE=.\ctools.cpp

!IF  "$(CFG)" == "srfake - Win32 Release"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 Debug"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Debug"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Release"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ENDIF 

SOURCE=.\fakesr.cpp

!IF  "$(CFG)" == "srfake - Win32 Release"

DEP_CPP_FAKES=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesr.obj" : $(SOURCE) $(DEP_CPP_FAKES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 Debug"

DEP_CPP_FAKES=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesr.obj" : $(SOURCE) $(DEP_CPP_FAKES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Debug"

DEP_CPP_FAKES=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesr.obj" : $(SOURCE) $(DEP_CPP_FAKES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Release"

DEP_CPP_FAKES=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesr.obj" : $(SOURCE) $(DEP_CPP_FAKES) "$(INTDIR)"


!ENDIF 

SOURCE=.\fakesr.rc

"$(INTDIR)\fakesr.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\fakesrcf.cpp

!IF  "$(CFG)" == "srfake - Win32 Release"

DEP_CPP_FAKESR=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesrcf.obj" : $(SOURCE) $(DEP_CPP_FAKESR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 Debug"

DEP_CPP_FAKESR=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesrcf.obj" : $(SOURCE) $(DEP_CPP_FAKESR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Debug"

DEP_CPP_FAKESR=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesrcf.obj" : $(SOURCE) $(DEP_CPP_FAKESR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Release"

DEP_CPP_FAKESR=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesrcf.obj" : $(SOURCE) $(DEP_CPP_FAKESR) "$(INTDIR)"


!ENDIF 

SOURCE=.\fakesren.cpp

!IF  "$(CFG)" == "srfake - Win32 Release"

DEP_CPP_FAKESRE=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesren.obj" : $(SOURCE) $(DEP_CPP_FAKESRE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 Debug"

DEP_CPP_FAKESRE=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesren.obj" : $(SOURCE) $(DEP_CPP_FAKESRE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Debug"

DEP_CPP_FAKESRE=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesren.obj" : $(SOURCE) $(DEP_CPP_FAKESRE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Release"

DEP_CPP_FAKESRE=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\fakesren.obj" : $(SOURCE) $(DEP_CPP_FAKESRE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fakesrgr.cpp

!IF  "$(CFG)" == "srfake - Win32 Release"

DEP_CPP_FAKESRG=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	

"$(INTDIR)\fakesrgr.obj" : $(SOURCE) $(DEP_CPP_FAKESRG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 Debug"

DEP_CPP_FAKESRG=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	

"$(INTDIR)\fakesrgr.obj" : $(SOURCE) $(DEP_CPP_FAKESRG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Debug"

DEP_CPP_FAKESRG=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	

"$(INTDIR)\fakesrgr.obj" : $(SOURCE) $(DEP_CPP_FAKESRG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Release"

DEP_CPP_FAKESRG=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	

"$(INTDIR)\fakesrgr.obj" : $(SOURCE) $(DEP_CPP_FAKESRG) "$(INTDIR)"


!ENDIF 

SOURCE=.\guidseg.cpp

!IF  "$(CFG)" == "srfake - Win32 Release"

DEP_CPP_GUIDS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 Debug"

DEP_CPP_GUIDS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Debug"

DEP_CPP_GUIDS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "srfake - Win32 (ALPHA) Release"

DEP_CPP_GUIDS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKESR.H"\
	".\fakesren.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ENDIF 


!ENDIF 

