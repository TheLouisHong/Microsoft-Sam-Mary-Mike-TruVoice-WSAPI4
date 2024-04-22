# Microsoft Developer Studio Generated NMAKE File, Based on tel.dsp
!IF "$(CFG)" == ""
CFG=tel - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tel - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tel - Win32 (ALPHA) Release" && "$(CFG)" !=\
 "tel - Win32 (ALPHA) Debug" && "$(CFG)" != "tel - Win32 Debug" && "$(CFG)" !=\
 "tel - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tel.mak" CFG="tel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tel - Win32 (ALPHA) Release" (based on "Win32 (ALPHA) Application")
!MESSAGE "tel - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "tel - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "tel - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tel - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tel.exe"

!ELSE 

ALL : "$(OUTDIR)\tel.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Editor.obj"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\link.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\tel.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\tel.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\tel.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tel.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\tel.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\tel.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Editor.obj" \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\link.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Test.obj"

"$(OUTDIR)\tel.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tel - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tel.exe"

!ELSE 

ALL : "$(OUTDIR)\tel.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Editor.obj"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\link.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\tel.exe"
	-@erase "$(OUTDIR)\tel.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\tel.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\tel.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tel.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\tel.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\tel.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Editor.obj" \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\link.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Test.obj"

"$(OUTDIR)\tel.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tel - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tel.exe"

!ELSE 

ALL : "$(OUTDIR)\tel.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Editor.obj"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\link.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\tel.exe"
	-@erase "$(OUTDIR)\tel.ilk"
	-@erase "$(OUTDIR)\tel.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\tel.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\tel.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tel.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\tel.pdb" /debug /machine:IX86 /out:"$(OUTDIR)\tel.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Editor.obj" \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\link.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Test.obj"

"$(OUTDIR)\tel.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tel - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tel.exe"

!ELSE 

ALL : "$(OUTDIR)\tel.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Editor.obj"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\link.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\tel.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\tel.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\tel.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tel.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\tel.pdb" /machine:IX86 /out:"$(OUTDIR)\tel.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Editor.obj" \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\link.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Test.obj"

"$(OUTDIR)\tel.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "tel - Win32 (ALPHA) Release" || "$(CFG)" ==\
 "tel - Win32 (ALPHA) Debug" || "$(CFG)" == "tel - Win32 Debug" || "$(CFG)" ==\
 "tel - Win32 Release"
SOURCE=.\Editor.cpp

!IF  "$(CFG)" == "tel - Win32 (ALPHA) Release"

DEP_CPP_EDITO=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Editor.obj" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 (ALPHA) Debug"

DEP_CPP_EDITO=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Editor.obj" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 Debug"

DEP_CPP_EDITO=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Editor.obj" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 Release"

DEP_CPP_EDITO=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Editor.obj" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"


!ENDIF 

SOURCE=.\guidseg.cpp

!IF  "$(CFG)" == "tel - Win32 (ALPHA) Release"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 (ALPHA) Debug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 Debug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 Release"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ENDIF 

SOURCE=.\link.cpp

!IF  "$(CFG)" == "tel - Win32 (ALPHA) Release"

DEP_CPP_LINK_=\
	".\link.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\link.obj" : $(SOURCE) $(DEP_CPP_LINK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 (ALPHA) Debug"

DEP_CPP_LINK_=\
	".\link.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\link.obj" : $(SOURCE) $(DEP_CPP_LINK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 Debug"

DEP_CPP_LINK_=\
	".\link.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\link.obj" : $(SOURCE) $(DEP_CPP_LINK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 Release"

DEP_CPP_LINK_=\
	".\link.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\link.obj" : $(SOURCE) $(DEP_CPP_LINK_) "$(INTDIR)"


!ENDIF 

SOURCE=.\tel.rc
DEP_RSC_TEL_R=\
	".\ico00001.ico"\
	".\icon1.ico"\
	

"$(INTDIR)\tel.res" : $(SOURCE) $(DEP_RSC_TEL_R) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Test.cpp

!IF  "$(CFG)" == "tel - Win32 (ALPHA) Release"

DEP_CPP_TEST_=\
	".\link.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test.obj" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 (ALPHA) Debug"

DEP_CPP_TEST_=\
	".\link.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test.obj" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 Debug"

DEP_CPP_TEST_=\
	".\link.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test.obj" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tel - Win32 Release"

DEP_CPP_TEST_=\
	".\link.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test.obj" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"


!ENDIF 


!ENDIF 

