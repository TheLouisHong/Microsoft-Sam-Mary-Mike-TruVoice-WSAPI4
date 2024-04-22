# Microsoft Developer Studio Generated NMAKE File, Based on speak.dsp
!IF "$(CFG)" == ""
CFG=speak - Win32 Debug
!MESSAGE No configuration specified. Defaulting to speak - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "speak - Win32 Release" && "$(CFG)" != "speak - Win32 Debug" &&\
 "$(CFG)" != "speak - Win32 ALPHA Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "speak.mak" CFG="speak - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "speak - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "speak - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "speak - Win32 ALPHA Release" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "speak - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\speak.exe"

!ELSE 

ALL : "$(OUTDIR)\speak.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\speak.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\speak.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /D "_UNICODE" /Fp"$(INTDIR)\speak.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\speak.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib spchwrap.lib libc.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\speak.pdb" /machine:I386 /out:"$(OUTDIR)\speak.exe" 
LINK32_OBJS= \
	"$(INTDIR)\speak.obj"

"$(OUTDIR)\speak.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "speak - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\speak.exe"

!ELSE 

ALL : "$(OUTDIR)\speak.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\speak.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\speak.exe"
	-@erase "$(OUTDIR)\speak.ilk"
	-@erase "$(OUTDIR)\speak.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /D "_MBCS" /D "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\speak.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib spchwrap.lib libc.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\speak.pdb" /debug /machine:I386 /nodefaultlib\
 /out:"$(OUTDIR)\speak.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\speak.obj"

"$(OUTDIR)\speak.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "speak - Win32 ALPHA Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\speak.exe"

!ELSE 

ALL : "$(OUTDIR)\speak.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\speak.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\speak.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /D "_UNICODE" /Fp"$(INTDIR)\speak.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\speak.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=spchwrap.lib libc.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib /nologo /subsystem:console /pdb:"$(OUTDIR)\speak.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\speak.exe" 
LINK32_OBJS= \
	"$(INTDIR)\speak.obj"

"$(OUTDIR)\speak.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "speak - Win32 Release" || "$(CFG)" == "speak - Win32 Debug" ||\
 "$(CFG)" == "speak - Win32 ALPHA Release"
SOURCE=.\speak.cpp

!IF  "$(CFG)" == "speak - Win32 Release"

DEP_CPP_SPEAK=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\speak.obj" : $(SOURCE) $(DEP_CPP_SPEAK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "speak - Win32 Debug"

DEP_CPP_SPEAK=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\speak.obj" : $(SOURCE) $(DEP_CPP_SPEAK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "speak - Win32 ALPHA Release"

DEP_CPP_SPEAK=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\speak.obj" : $(SOURCE) $(DEP_CPP_SPEAK) "$(INTDIR)"


!ENDIF 


!ENDIF 

