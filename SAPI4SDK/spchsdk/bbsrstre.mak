# Microsoft Developer Studio Generated NMAKE File, Based on SRStress.dsp
!IF "$(CFG)" == ""
CFG=SRStress - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SRStress - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SRStress - Win32 Release" && "$(CFG)" !=\
 "SRStress - Win32 Debug" && "$(CFG)" != "SRStress - Win32 (ALPHA) Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SRStress.mak" CFG="SRStress - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SRStress - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SRStress - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "SRStress - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SRStress - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SRStress.exe"

!ELSE 

ALL : "$(OUTDIR)\SRStress.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\SRStress.obj"
	-@erase "$(INTDIR)\Stress.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\SRStress.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\SRStress.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Stress.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SRStress.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=spchwrap.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\SRStress.pdb" /machine:I386 /out:"$(OUTDIR)\SRStress.exe" 
LINK32_OBJS= \
	"$(INTDIR)\SRStress.obj" \
	"$(INTDIR)\Stress.res"

"$(OUTDIR)\SRStress.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SRStress - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SRStress.exe"

!ELSE 

ALL : "$(OUTDIR)\SRStress.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\SRStress.obj"
	-@erase "$(INTDIR)\Stress.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\SRStress.exe"
	-@erase "$(OUTDIR)\SRStress.ilk"
	-@erase "$(OUTDIR)\SRStress.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\SRStress.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Stress.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SRStress.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=spchwrap.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\SRStress.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\SRStress.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\SRStress.obj" \
	"$(INTDIR)\Stress.res"

"$(OUTDIR)\SRStress.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SRStress - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SRStress.exe"

!ELSE 

ALL : "$(OUTDIR)\SRStress.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\SRStress.obj"
	-@erase "$(INTDIR)\Stress.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\SRStress.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\SRStress.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Stress.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SRStress.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=spchwrap.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\SRStress.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\SRStress.exe" 
LINK32_OBJS= \
	"$(INTDIR)\SRStress.obj" \
	"$(INTDIR)\Stress.res"

"$(OUTDIR)\SRStress.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "SRStress - Win32 Release" || "$(CFG)" ==\
 "SRStress - Win32 Debug" || "$(CFG)" == "SRStress - Win32 (ALPHA) Release"
SOURCE=.\SRStress.cpp

!IF  "$(CFG)" == "SRStress - Win32 Release"

DEP_CPP_SRSTR=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\SRStress.obj" : $(SOURCE) $(DEP_CPP_SRSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRStress - Win32 Debug"

DEP_CPP_SRSTR=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\SRStress.obj" : $(SOURCE) $(DEP_CPP_SRSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRStress - Win32 (ALPHA) Release"

DEP_CPP_SRSTR=\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\SRStress.obj" : $(SOURCE) $(DEP_CPP_SRSTR) "$(INTDIR)"


!ENDIF 

SOURCE=.\Stress.rc

"$(INTDIR)\Stress.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

