# Microsoft Developer Studio Generated NMAKE File, Based on LogView.dsp
!IF "$(CFG)" == ""
CFG=LogView - Win32 Debug
!MESSAGE No configuration specified. Defaulting to LogView - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "LogView - Win32 (ALPHA) Release" && "$(CFG)" !=\
 "LogView - Win32 (ALPHA) Debug" && "$(CFG)" != "LogView - Win32 Release" &&\
 "$(CFG)" != "LogView - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LogView.mak" CFG="LogView - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LogView - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "LogView - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "LogView - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "LogView - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "LogView - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\LogView.exe"

!ELSE 

ALL : "$(OUTDIR)\LogView.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\LogFile.obj"
	-@erase "$(INTDIR)\LogView.obj"
	-@erase "$(INTDIR)\LogView.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\LogView.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\LogView.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\LogView.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\LogView.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\LogView.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\LogView.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\LogFile.obj" \
	"$(INTDIR)\LogView.obj" \
	"$(INTDIR)\LogView.res"

"$(OUTDIR)\LogView.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "LogView - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\LogView.exe"

!ELSE 

ALL : "$(OUTDIR)\LogView.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\LogFile.obj"
	-@erase "$(INTDIR)\LogView.obj"
	-@erase "$(INTDIR)\LogView.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\LogView.exe"
	-@erase "$(OUTDIR)\LogView.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\LogView.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\LogView.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\LogView.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\LogView.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\LogView.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\LogFile.obj" \
	"$(INTDIR)\LogView.obj" \
	"$(INTDIR)\LogView.res"

"$(OUTDIR)\LogView.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "LogView - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\LogView.exe"

!ELSE 

ALL : "$(OUTDIR)\LogView.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\LogFile.obj"
	-@erase "$(INTDIR)\LogView.obj"
	-@erase "$(INTDIR)\LogView.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\LogView.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\LogView.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\LogView.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\LogView.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\LogView.pdb" /machine:IX86 /out:"$(OUTDIR)\LogView.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\LogFile.obj" \
	"$(INTDIR)\LogView.obj" \
	"$(INTDIR)\LogView.res"

"$(OUTDIR)\LogView.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "LogView - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\LogView.exe"

!ELSE 

ALL : "$(OUTDIR)\LogView.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\LogFile.obj"
	-@erase "$(INTDIR)\LogView.obj"
	-@erase "$(INTDIR)\LogView.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\LogView.exe"
	-@erase "$(OUTDIR)\LogView.ilk"
	-@erase "$(OUTDIR)\LogView.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\LogView.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\LogView.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\LogView.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib\
 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib\
 oleaut32.lib uuid.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\LogView.pdb" /debug /machine:IX86 /out:"$(OUTDIR)\LogView.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\LogFile.obj" \
	"$(INTDIR)\LogView.obj" \
	"$(INTDIR)\LogView.res"

"$(OUTDIR)\LogView.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "LogView - Win32 (ALPHA) Release" || "$(CFG)" ==\
 "LogView - Win32 (ALPHA) Debug" || "$(CFG)" == "LogView - Win32 Release" ||\
 "$(CFG)" == "LogView - Win32 Debug"
SOURCE=.\ctools.cpp

!IF  "$(CFG)" == "LogView - Win32 (ALPHA) Release"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "LogView - Win32 (ALPHA) Debug"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "LogView - Win32 Release"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "LogView - Win32 Debug"

DEP_CPP_CTOOL=\
	".\ctools.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ENDIF 

SOURCE=.\LogFile.cpp

!IF  "$(CFG)" == "LogView - Win32 (ALPHA) Release"

DEP_CPP_LOGFI=\
	".\ctools.h"\
	".\LogFile.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\LogFile.obj" : $(SOURCE) $(DEP_CPP_LOGFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "LogView - Win32 (ALPHA) Debug"

DEP_CPP_LOGFI=\
	".\ctools.h"\
	".\LogFile.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\LogFile.obj" : $(SOURCE) $(DEP_CPP_LOGFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "LogView - Win32 Release"

DEP_CPP_LOGFI=\
	".\ctools.h"\
	".\LogFile.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\LogFile.obj" : $(SOURCE) $(DEP_CPP_LOGFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "LogView - Win32 Debug"

DEP_CPP_LOGFI=\
	".\ctools.h"\
	".\LogFile.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\LogFile.obj" : $(SOURCE) $(DEP_CPP_LOGFI) "$(INTDIR)"


!ENDIF 

SOURCE=.\LogView.cpp

!IF  "$(CFG)" == "LogView - Win32 (ALPHA) Release"

DEP_CPP_LOGVI=\
	".\ctools.h"\
	".\LogFile.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\LogView.obj" : $(SOURCE) $(DEP_CPP_LOGVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "LogView - Win32 (ALPHA) Debug"

DEP_CPP_LOGVI=\
	".\ctools.h"\
	".\LogFile.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\LogView.obj" : $(SOURCE) $(DEP_CPP_LOGVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "LogView - Win32 Release"

DEP_CPP_LOGVI=\
	".\ctools.h"\
	".\LogFile.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\LogView.obj" : $(SOURCE) $(DEP_CPP_LOGVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "LogView - Win32 Debug"

DEP_CPP_LOGVI=\
	".\ctools.h"\
	".\LogFile.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\LogView.obj" : $(SOURCE) $(DEP_CPP_LOGVI) "$(INTDIR)"


!ENDIF 

SOURCE=.\LogView.rc
DEP_RSC_LOGVIE=\
	".\ico00001.ico"\
	".\icon1.ico"\
	

"$(INTDIR)\LogView.res" : $(SOURCE) $(DEP_RSC_LOGVIE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

