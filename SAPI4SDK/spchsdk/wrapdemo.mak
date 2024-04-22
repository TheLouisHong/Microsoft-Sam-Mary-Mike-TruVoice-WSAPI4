# Microsoft Developer Studio Generated NMAKE File, Based on WrapDemo.dsp
!IF "$(CFG)" == ""
CFG=WrapDemo - Win32 Debug
!MESSAGE No configuration specified. Defaulting to WrapDemo - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "WrapDemo - Win32 (ALPHA) Release" && "$(CFG)" !=\
 "WrapDemo - Win32 (ALPHA) Debug" && "$(CFG)" != "WrapDemo - Win32 Debug" &&\
 "$(CFG)" != "WrapDemo - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WrapDemo.mak" CFG="WrapDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WrapDemo - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "WrapDemo - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "WrapDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "WrapDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "WrapDemo - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\WrapDemo.exe"

!ELSE 

ALL : "$(OUTDIR)\WrapDemo.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\WrapDemo.obj"
	-@erase "$(INTDIR)\WrapDemo.res"
	-@erase "$(OUTDIR)\WrapDemo.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\WrapDemo.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\WrapDemo.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WrapDemo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib spchwrap.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\WrapDemo.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\WrapDemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\WrapDemo.obj" \
	"$(INTDIR)\WrapDemo.res"

"$(OUTDIR)\WrapDemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "WrapDemo - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\WrapDemo.exe"

!ELSE 

ALL : "$(OUTDIR)\WrapDemo.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\WrapDemo.obj"
	-@erase "$(INTDIR)\WrapDemo.res"
	-@erase "$(OUTDIR)\WrapDemo.exe"
	-@erase "$(OUTDIR)\WrapDemo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\WrapDemo.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\WrapDemo.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WrapDemo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=spchwrap.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\WrapDemo.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\WrapDemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\WrapDemo.obj" \
	"$(INTDIR)\WrapDemo.res"

"$(OUTDIR)\WrapDemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "WrapDemo - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\WrapDemo.exe"

!ELSE 

ALL : "$(OUTDIR)\WrapDemo.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\WrapDemo.obj"
	-@erase "$(INTDIR)\WrapDemo.res"
	-@erase "$(OUTDIR)\WrapDemo.exe"
	-@erase "$(OUTDIR)\WrapDemo.ilk"
	-@erase "$(OUTDIR)\WrapDemo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\WrapDemo.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\WrapDemo.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WrapDemo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=$(BUILDDRV)\api\spchwrap\alphadbg\spchwrap.lib odbc32.lib odbccp32.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib spchwrap.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\WrapDemo.pdb" /debug\
 /machine:IX86 /out:"$(OUTDIR)\WrapDemo.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\WrapDemo.obj" \
	"$(INTDIR)\WrapDemo.res"

"$(OUTDIR)\WrapDemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "WrapDemo - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\WrapDemo.exe"

!ELSE 

ALL : "$(OUTDIR)\WrapDemo.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\WrapDemo.obj"
	-@erase "$(INTDIR)\WrapDemo.res"
	-@erase "$(OUTDIR)\WrapDemo.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\WrapDemo.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\WrapDemo.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WrapDemo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib spchwrap.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\WrapDemo.pdb" /machine:IX86 /out:"$(OUTDIR)\WrapDemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\WrapDemo.obj" \
	"$(INTDIR)\WrapDemo.res"

"$(OUTDIR)\WrapDemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "WrapDemo - Win32 (ALPHA) Release" || "$(CFG)" ==\
 "WrapDemo - Win32 (ALPHA) Debug" || "$(CFG)" == "WrapDemo - Win32 Debug" ||\
 "$(CFG)" == "WrapDemo - Win32 Release"
SOURCE=.\WrapDemo.cpp

!IF  "$(CFG)" == "WrapDemo - Win32 (ALPHA) Release"

DEP_CPP_WRAPD=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\WrapDemo.obj" : $(SOURCE) $(DEP_CPP_WRAPD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "WrapDemo - Win32 (ALPHA) Debug"

DEP_CPP_WRAPD=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\WrapDemo.obj" : $(SOURCE) $(DEP_CPP_WRAPD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "WrapDemo - Win32 Debug"

DEP_CPP_WRAPD=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\WrapDemo.obj" : $(SOURCE) $(DEP_CPP_WRAPD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "WrapDemo - Win32 Release"

DEP_CPP_WRAPD=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\WrapDemo.obj" : $(SOURCE) $(DEP_CPP_WRAPD) "$(INTDIR)"


!ENDIF 

SOURCE=.\WrapDemo.rc

"$(INTDIR)\WrapDemo.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

