# Microsoft Developer Studio Generated NMAKE File, Based on DestFile.dsp
!IF "$(CFG)" == ""
CFG=DestFile - Win32 Release
!MESSAGE No configuration specified. Defaulting to DestFile - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "DestFile - Win32 Release" && "$(CFG)" !=\
 "DestFile - Win32 Debug" && "$(CFG)" != "DestFile - Win32 (ALPHA) Debug" &&\
 "$(CFG)" != "DestFile - Win32 (ALPHA) Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DestFile.mak" CFG="DestFile - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DestFile - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DestFile - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "DestFile - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "DestFile - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "DestFile - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\DestFile.exe"

!ELSE 

ALL : "$(OUTDIR)\DestFile.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DEMO.OBJ"
	-@erase "$(INTDIR)\demo.res"
	-@erase "$(OUTDIR)\DestFile.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\DestFile.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\demo.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DestFile.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\DestFile.pdb" /machine:I386 /out:"$(OUTDIR)\DestFile.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DEMO.OBJ" \
	"$(INTDIR)\demo.res"

"$(OUTDIR)\DestFile.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DestFile - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\DestFile.exe"

!ELSE 

ALL : "$(OUTDIR)\DestFile.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DEMO.OBJ"
	-@erase "$(INTDIR)\demo.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\DestFile.exe"
	-@erase "$(OUTDIR)\DestFile.ilk"
	-@erase "$(OUTDIR)\DestFile.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\DestFile.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\demo.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DestFile.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\DestFile.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\DestFile.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DEMO.OBJ" \
	"$(INTDIR)\demo.res"

"$(OUTDIR)\DestFile.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DestFile - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\DestFile.exe"

!ELSE 

ALL : "$(OUTDIR)\DestFile.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DEMO.OBJ"
	-@erase "$(INTDIR)\demo.res"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\DestFile.exe"
	-@erase "$(OUTDIR)\DestFile.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\DestFile.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\demo.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DestFile.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\DestFile.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\DestFile.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DEMO.OBJ" \
	"$(INTDIR)\demo.res"

"$(OUTDIR)\DestFile.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DestFile - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\DestFile.exe"

!ELSE 

ALL : "$(OUTDIR)\DestFile.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DEMO.OBJ"
	-@erase "$(INTDIR)\demo.res"
	-@erase "$(OUTDIR)\DestFile.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\DestFile.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\demo.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DestFile.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\DestFile.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\DestFile.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DEMO.OBJ" \
	"$(INTDIR)\demo.res"

"$(OUTDIR)\DestFile.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "DestFile - Win32 Release" || "$(CFG)" ==\
 "DestFile - Win32 Debug" || "$(CFG)" == "DestFile - Win32 (ALPHA) Debug" ||\
 "$(CFG)" == "DestFile - Win32 (ALPHA) Release"
SOURCE=.\DEMO.CPP

!IF  "$(CFG)" == "DestFile - Win32 Release"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\DEMO.OBJ" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DestFile - Win32 Debug"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\DEMO.OBJ" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DestFile - Win32 (ALPHA) Debug"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\DEMO.OBJ" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DestFile - Win32 (ALPHA) Release"

DEP_CPP_DEMO_=\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\DEMO.OBJ" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"


!ENDIF 

SOURCE=.\demo.rc
DEP_RSC_DEMO_R=\
	".\icon1.ico"\
	

"$(INTDIR)\demo.res" : $(SOURCE) $(DEP_RSC_DEMO_R) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

