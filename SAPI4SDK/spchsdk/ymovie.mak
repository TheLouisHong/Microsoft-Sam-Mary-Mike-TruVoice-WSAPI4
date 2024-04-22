# Microsoft Developer Studio Generated NMAKE File, Based on Movie.dsp
!IF "$(CFG)" == ""
CFG=Movie - Win32 (ALPHA) Debug
!MESSAGE No configuration specified. Defaulting to Movie - Win32 (ALPHA) Debug.
!ENDIF 

!IF "$(CFG)" != "Movie - Win32 Release" && "$(CFG)" != "Movie - Win32 Debug" &&\
 "$(CFG)" != "Movie - Win32 (ALPHA) Release" && "$(CFG)" !=\
 "Movie - Win32 (ALPHA) Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Movie.mak" CFG="Movie - Win32 (ALPHA) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Movie - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Movie - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Movie - Win32 (ALPHA) Release" (based on "Win32 (ALPHA) Application")
!MESSAGE "Movie - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Movie - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Movie.exe"

!ELSE 

ALL : "$(OUTDIR)\Movie.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Movie.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Telsamp.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Movie.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Movie.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Movie.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib spchwrap.lib tapi32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\Movie.pdb" /machine:I386 /out:"$(OUTDIR)\Movie.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Movie.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Telsamp.obj"

"$(OUTDIR)\Movie.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Movie - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Movie.exe"

!ELSE 

ALL : "$(OUTDIR)\Movie.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Movie.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Telsamp.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Movie.exe"
	-@erase "$(OUTDIR)\Movie.ilk"
	-@erase "$(OUTDIR)\Movie.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Movie.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Movie.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib spchwrap.lib tapi32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)\Movie.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Movie.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Movie.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Telsamp.obj"

"$(OUTDIR)\Movie.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Movie - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Movie.exe"

!ELSE 

ALL : "$(OUTDIR)\Movie.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Movie.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Telsamp.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Movie.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Movie.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Movie.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=spchwrap.lib tapi32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)\Movie.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\Movie.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Movie.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Telsamp.obj"

"$(OUTDIR)\Movie.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Movie - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Movie.exe"

!ELSE 

ALL : "$(OUTDIR)\Movie.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Movie.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Telsamp.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Movie.exe"
	-@erase "$(OUTDIR)\Movie.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Movie.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /MDd /c 
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Movie.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=spchwrap.lib tapi32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)\Movie.pdb" /debug\
 /machine:ALPHA /out:"$(OUTDIR)\Movie.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Movie.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Telsamp.obj"

"$(OUTDIR)\Movie.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Movie - Win32 Release" || "$(CFG)" == "Movie - Win32 Debug" ||\
 "$(CFG)" == "Movie - Win32 (ALPHA) Release" || "$(CFG)" ==\
 "Movie - Win32 (ALPHA) Debug"
SOURCE=.\Movie.cpp

!IF  "$(CFG)" == "Movie - Win32 Release"

DEP_CPP_Movie=\
	".\Movie.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Movie.obj" : $(SOURCE) $(DEP_CPP_Movie) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Movie - Win32 Debug"

DEP_CPP_Movie=\
	".\Movie.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Movie.obj" : $(SOURCE) $(DEP_CPP_Movie) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Movie - Win32 (ALPHA) Release"

DEP_CPP_Movie=\
	".\Movie.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Movie.obj" : $(SOURCE) $(DEP_CPP_Movie) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Movie - Win32 (ALPHA) Debug"

DEP_CPP_Movie=\
	".\Movie.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Movie.obj" : $(SOURCE) $(DEP_CPP_Movie) "$(INTDIR)"


!ENDIF 

SOURCE=.\tel.rc

"$(INTDIR)\tel.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Telsamp.cpp

!IF  "$(CFG)" == "Movie - Win32 Release"

DEP_CPP_TELSA=\
	".\Movie.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Telsamp.obj" : $(SOURCE) $(DEP_CPP_TELSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Movie - Win32 Debug"

DEP_CPP_TELSA=\
	".\Movie.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Telsamp.obj" : $(SOURCE) $(DEP_CPP_TELSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Movie - Win32 (ALPHA) Release"

DEP_CPP_TELSA=\
	".\Movie.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Telsamp.obj" : $(SOURCE) $(DEP_CPP_TELSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Movie - Win32 (ALPHA) Debug"

DEP_CPP_TELSA=\
	".\Movie.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Telsamp.obj" : $(SOURCE) $(DEP_CPP_TELSA) "$(INTDIR)"


!ENDIF 


!ENDIF 

