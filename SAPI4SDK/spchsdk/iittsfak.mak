# Microsoft Developer Studio Generated NMAKE File, Based on Ttsfake.dsp
!IF "$(CFG)" == ""
CFG=Ttsfake - Win32 Release
!MESSAGE No configuration specified. Defaulting to Ttsfake - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "Ttsfake - Win32 Release" && "$(CFG)" !=\
 "Ttsfake - Win32 Debug" && "$(CFG)" != "Ttsfake - Win32 (ALPHA) Debug" &&\
 "$(CFG)" != "Ttsfake - Win32 (ALPHA) Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ttsfake.mak" CFG="Ttsfake - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ttsfake - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Ttsfake - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Ttsfake - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "Ttsfake - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Ttsfake.dll"

!ELSE 

ALL : "$(OUTDIR)\Ttsfake.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\CTOOLS.OBJ"
	-@erase "$(INTDIR)\FAKETTS.OBJ"
	-@erase "$(INTDIR)\FAKETTS.res"
	-@erase "$(INTDIR)\FAKETTSC.OBJ"
	-@erase "$(INTDIR)\FAKETTSE.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Ttsfake.dll"
	-@erase "$(OUTDIR)\Ttsfake.exp"
	-@erase "$(OUTDIR)\Ttsfake.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I {$(BUILDDRV)}"\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\FAKETTS.res" /i {$(BUILDDRV)}"\include" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ttsfake.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\Ttsfake.pdb"\
 /machine:I386 /def:".\TTSFAKE.DEF" /out:"$(OUTDIR)\Ttsfake.dll"\
 /implib:"$(OUTDIR)\Ttsfake.lib" 
DEF_FILE= \
	".\TTSFAKE.DEF"
LINK32_OBJS= \
	"$(INTDIR)\CTOOLS.OBJ" \
	"$(INTDIR)\FAKETTS.OBJ" \
	"$(INTDIR)\FAKETTS.res" \
	"$(INTDIR)\FAKETTSC.OBJ" \
	"$(INTDIR)\FAKETTSE.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ"

"$(OUTDIR)\Ttsfake.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Ttsfake.dll"

!ELSE 

ALL : "$(OUTDIR)\Ttsfake.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\CTOOLS.OBJ"
	-@erase "$(INTDIR)\FAKETTS.OBJ"
	-@erase "$(INTDIR)\FAKETTS.res"
	-@erase "$(INTDIR)\FAKETTSC.OBJ"
	-@erase "$(INTDIR)\FAKETTSE.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Ttsfake.dll"
	-@erase "$(OUTDIR)\Ttsfake.exp"
	-@erase "$(OUTDIR)\Ttsfake.ilk"
	-@erase "$(OUTDIR)\Ttsfake.lib"
	-@erase "$(OUTDIR)\Ttsfake.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I {$(BUILDDRV)}"\include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\FAKETTS.res" /i {$(BUILDDRV)}"\include" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ttsfake.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\Ttsfake.pdb" /debug\
 /machine:I386 /def:".\TTSFAKE.DEF" /out:"$(OUTDIR)\Ttsfake.dll"\
 /implib:"$(OUTDIR)\Ttsfake.lib" 
DEF_FILE= \
	".\TTSFAKE.DEF"
LINK32_OBJS= \
	"$(INTDIR)\CTOOLS.OBJ" \
	"$(INTDIR)\FAKETTS.OBJ" \
	"$(INTDIR)\FAKETTS.res" \
	"$(INTDIR)\FAKETTSC.OBJ" \
	"$(INTDIR)\FAKETTSE.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ"

"$(OUTDIR)\Ttsfake.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Ttsfake.dll"

!ELSE 

ALL : "$(OUTDIR)\Ttsfake.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\CTOOLS.OBJ"
	-@erase "$(INTDIR)\FAKETTS.OBJ"
	-@erase "$(INTDIR)\FAKETTS.res"
	-@erase "$(INTDIR)\FAKETTSC.OBJ"
	-@erase "$(INTDIR)\FAKETTSE.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Ttsfake.dll"
	-@erase "$(OUTDIR)\Ttsfake.exp"
	-@erase "$(OUTDIR)\Ttsfake.lib"
	-@erase "$(OUTDIR)\Ttsfake.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /I {$(BUILDDRV)}"\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\Ttsfake.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\FAKETTS.res" /i {$(BUILDDRV)}"\include" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ttsfake.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /pdb:"$(OUTDIR)\Ttsfake.pdb"\
 /debug /machine:ALPHA /def:".\TTSFAKE.DEF" /out:"$(OUTDIR)\Ttsfake.dll"\
 /implib:"$(OUTDIR)\Ttsfake.lib" 
DEF_FILE= \
	".\TTSFAKE.DEF"
LINK32_OBJS= \
	"$(INTDIR)\CTOOLS.OBJ" \
	"$(INTDIR)\FAKETTS.OBJ" \
	"$(INTDIR)\FAKETTS.res" \
	"$(INTDIR)\FAKETTSC.OBJ" \
	"$(INTDIR)\FAKETTSE.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ"

"$(OUTDIR)\Ttsfake.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Ttsfake.dll"

!ELSE 

ALL : "$(OUTDIR)\Ttsfake.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\CTOOLS.OBJ"
	-@erase "$(INTDIR)\FAKETTS.OBJ"
	-@erase "$(INTDIR)\FAKETTS.res"
	-@erase "$(INTDIR)\FAKETTSC.OBJ"
	-@erase "$(INTDIR)\FAKETTSE.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Ttsfake.dll"
	-@erase "$(OUTDIR)\Ttsfake.exp"
	-@erase "$(OUTDIR)\Ttsfake.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MT /Gt0 /W3 /GX /O2 /I {$(BUILDDRV)}"\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\Ttsfake.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\FAKETTS.res" /i {$(BUILDDRV)}"\include" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ttsfake.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /pdb:"$(OUTDIR)\Ttsfake.pdb"\
 /machine:ALPHA /def:".\TTSFAKE.DEF" /out:"$(OUTDIR)\Ttsfake.dll"\
 /implib:"$(OUTDIR)\Ttsfake.lib" 
DEF_FILE= \
	".\TTSFAKE.DEF"
LINK32_OBJS= \
	"$(INTDIR)\CTOOLS.OBJ" \
	"$(INTDIR)\FAKETTS.OBJ" \
	"$(INTDIR)\FAKETTS.res" \
	"$(INTDIR)\FAKETTSC.OBJ" \
	"$(INTDIR)\FAKETTSE.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ"

"$(OUTDIR)\Ttsfake.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Ttsfake - Win32 Release" || "$(CFG)" ==\
 "Ttsfake - Win32 Debug" || "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug" ||\
 "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"
SOURCE=.\CTOOLS.CPP

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\CTOOLS.OBJ" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\CTOOLS.OBJ" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\CTOOLS.OBJ" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\CTOOLS.OBJ" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ENDIF 

SOURCE=.\FAKETTS.CPP

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

DEP_CPP_FAKET=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTS.OBJ" : $(SOURCE) $(DEP_CPP_FAKET) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

DEP_CPP_FAKET=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTS.OBJ" : $(SOURCE) $(DEP_CPP_FAKET) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

DEP_CPP_FAKET=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTS.OBJ" : $(SOURCE) $(DEP_CPP_FAKET) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

DEP_CPP_FAKET=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTS.OBJ" : $(SOURCE) $(DEP_CPP_FAKET) "$(INTDIR)"


!ENDIF 

SOURCE=.\FAKETTS.RC

"$(INTDIR)\FAKETTS.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\FAKETTSC.CPP

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

DEP_CPP_FAKETT=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTSC.OBJ" : $(SOURCE) $(DEP_CPP_FAKETT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

DEP_CPP_FAKETT=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTSC.OBJ" : $(SOURCE) $(DEP_CPP_FAKETT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

DEP_CPP_FAKETT=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTSC.OBJ" : $(SOURCE) $(DEP_CPP_FAKETT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

DEP_CPP_FAKETT=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTSC.OBJ" : $(SOURCE) $(DEP_CPP_FAKETT) "$(INTDIR)"


!ENDIF 

SOURCE=.\FAKETTSE.CPP

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

DEP_CPP_FAKETTS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTSE.OBJ" : $(SOURCE) $(DEP_CPP_FAKETTS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

DEP_CPP_FAKETTS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTSE.OBJ" : $(SOURCE) $(DEP_CPP_FAKETTS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

DEP_CPP_FAKETTS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTSE.OBJ" : $(SOURCE) $(DEP_CPP_FAKETTS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

DEP_CPP_FAKETTS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	

"$(INTDIR)\FAKETTSE.OBJ" : $(SOURCE) $(DEP_CPP_FAKETTS) "$(INTDIR)"


!ENDIF 

SOURCE=.\GUIDSEG.CPP

!IF  "$(CFG)" == "Ttsfake - Win32 Release"

DEP_CPP_GUIDS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	
CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I {$(BUILDDRV)}"\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 Debug"

DEP_CPP_GUIDS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	
CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /I {$(BUILDDRV)}"\include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Debug"

DEP_CPP_GUIDS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /I {$(BUILDDRV)}"\include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /MTd /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ttsfake - Win32 (ALPHA) Release"

DEP_CPP_GUIDS=\
	"..\..\..\include\speech.h"\
	".\ctools.h"\
	".\FAKETTS.H"\
	".\FAKETTSE.H"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /I {$(BUILDDRV)}"\include" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

