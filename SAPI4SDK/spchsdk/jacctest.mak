# Microsoft Developer Studio Generated NMAKE File, Based on AccTest.dsp
!IF "$(CFG)" == ""
CFG=AccTest - Win32 Debug
!MESSAGE No configuration specified. Defaulting to AccTest - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "AccTest - Win32 Release" && "$(CFG)" !=\
 "AccTest - Win32 Debug" && "$(CFG)" != "AccTest - Win32 (ALPHA) Debug" &&\
 "$(CFG)" != "AccTest - Win32 (ALPHA) Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AccTest.mak" CFG="AccTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AccTest - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AccTest - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "AccTest - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "AccTest - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "AccTest - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\AccTest.exe"

!ELSE 

ALL : "$(OUTDIR)\AccTest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DiffWord.obj"
	-@erase "$(INTDIR)\MAIN.OBJ"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\RES.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\AccTest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\AccTest.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\RES.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AccTest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib spchwrap.lib dsound.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)\AccTest.pdb" /machine:I386\
 /out:"$(OUTDIR)\AccTest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DiffWord.obj" \
	"$(INTDIR)\MAIN.OBJ" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\RES.res"

"$(OUTDIR)\AccTest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "AccTest - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\AccTest.exe"

!ELSE 

ALL : "$(OUTDIR)\AccTest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DiffWord.obj"
	-@erase "$(INTDIR)\MAIN.OBJ"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\RES.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\AccTest.exe"
	-@erase "$(OUTDIR)\AccTest.ilk"
	-@erase "$(OUTDIR)\AccTest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\AccTest.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\RES.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AccTest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib spchwrap.lib dsound.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)\AccTest.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\AccTest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DiffWord.obj" \
	"$(INTDIR)\MAIN.OBJ" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\RES.res"

"$(OUTDIR)\AccTest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "AccTest - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\AccTest.exe"

!ELSE 

ALL : "$(OUTDIR)\AccTest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DiffWord.obj"
	-@erase "$(INTDIR)\MAIN.OBJ"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\RES.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\AccTest.exe"
	-@erase "$(OUTDIR)\AccTest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\AccTest.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\RES.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AccTest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib spchwrap.lib dsound.lib /nologo /subsystem:windows\
 /pdb:"$(OUTDIR)\AccTest.pdb" /debug /machine:ALPHA /out:"$(OUTDIR)\AccTest.exe"\
 
LINK32_OBJS= \
	"$(INTDIR)\DiffWord.obj" \
	"$(INTDIR)\MAIN.OBJ" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\RES.res"

"$(OUTDIR)\AccTest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "AccTest - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\AccTest.exe"

!ELSE 

ALL : "$(OUTDIR)\AccTest.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DiffWord.obj"
	-@erase "$(INTDIR)\MAIN.OBJ"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\RES.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\AccTest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\AccTest.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\RES.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AccTest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib spchwrap.lib dsound.lib /nologo /subsystem:windows\
 /pdb:"$(OUTDIR)\AccTest.pdb" /machine:ALPHA /out:"$(OUTDIR)\AccTest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DiffWord.obj" \
	"$(INTDIR)\MAIN.OBJ" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\RES.res"

"$(OUTDIR)\AccTest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "AccTest - Win32 Release" || "$(CFG)" ==\
 "AccTest - Win32 Debug" || "$(CFG)" == "AccTest - Win32 (ALPHA) Debug" ||\
 "$(CFG)" == "AccTest - Win32 (ALPHA) Release"
SOURCE=.\DiffWord.cpp

!IF  "$(CFG)" == "AccTest - Win32 Release"

DEP_CPP_DIFFW=\
	".\parse.h"\
	

"$(INTDIR)\DiffWord.obj" : $(SOURCE) $(DEP_CPP_DIFFW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AccTest - Win32 Debug"

DEP_CPP_DIFFW=\
	".\parse.h"\
	

"$(INTDIR)\DiffWord.obj" : $(SOURCE) $(DEP_CPP_DIFFW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AccTest - Win32 (ALPHA) Debug"

DEP_CPP_DIFFW=\
	".\parse.h"\
	

"$(INTDIR)\DiffWord.obj" : $(SOURCE) $(DEP_CPP_DIFFW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AccTest - Win32 (ALPHA) Release"

DEP_CPP_DIFFW=\
	".\parse.h"\
	

"$(INTDIR)\DiffWord.obj" : $(SOURCE) $(DEP_CPP_DIFFW) "$(INTDIR)"


!ENDIF 

SOURCE=.\MAIN.CPP

!IF  "$(CFG)" == "AccTest - Win32 Release"

DEP_CPP_MAIN_=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\MAIN.OBJ" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AccTest - Win32 Debug"

DEP_CPP_MAIN_=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\MAIN.OBJ" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AccTest - Win32 (ALPHA) Debug"

DEP_CPP_MAIN_=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\MAIN.OBJ" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AccTest - Win32 (ALPHA) Release"

DEP_CPP_MAIN_=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\MAIN.OBJ" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ENDIF 

SOURCE=.\Parse.cpp

!IF  "$(CFG)" == "AccTest - Win32 Release"

DEP_CPP_PARSE=\
	".\parse.h"\
	

"$(INTDIR)\Parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AccTest - Win32 Debug"

DEP_CPP_PARSE=\
	".\parse.h"\
	

"$(INTDIR)\Parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AccTest - Win32 (ALPHA) Debug"

DEP_CPP_PARSE=\
	".\parse.h"\
	

"$(INTDIR)\Parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AccTest - Win32 (ALPHA) Release"

DEP_CPP_PARSE=\
	".\parse.h"\
	

"$(INTDIR)\Parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RES.RC

"$(INTDIR)\RES.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

