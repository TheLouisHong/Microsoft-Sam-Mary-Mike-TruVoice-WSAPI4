# Microsoft Developer Studio Generated NMAKE File, Based on SRFunc.dsp
!IF "$(CFG)" == ""
CFG=SRFunc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SRFunc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SRFunc - Win32 Release" && "$(CFG)" != "SRFunc - Win32 Debug"\
 && "$(CFG)" != "SRFunc - Win32 (ALPHA) Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SRFunc.mak" CFG="SRFunc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SRFunc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SRFunc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "SRFunc - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SRFunc - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SRFunc.exe"

!ELSE 

ALL : "$(OUTDIR)\SRFunc.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Frame.obj"
	-@erase "$(INTDIR)\Info.obj"
	-@erase "$(INTDIR)\Info.res"
	-@erase "$(INTDIR)\Test1.obj"
	-@erase "$(INTDIR)\Test2.obj"
	-@erase "$(INTDIR)\Test3.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\SRFunc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\SRFunc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Info.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SRFunc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib spchwrap.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\SRFunc.pdb" /machine:I386 /out:"$(OUTDIR)\SRFunc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Frame.obj" \
	"$(INTDIR)\Info.obj" \
	"$(INTDIR)\Info.res" \
	"$(INTDIR)\Test1.obj" \
	"$(INTDIR)\Test2.obj" \
	"$(INTDIR)\Test3.obj"

"$(OUTDIR)\SRFunc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SRFunc.exe"

!ELSE 

ALL : "$(OUTDIR)\SRFunc.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Frame.obj"
	-@erase "$(INTDIR)\Info.obj"
	-@erase "$(INTDIR)\Info.res"
	-@erase "$(INTDIR)\Test1.obj"
	-@erase "$(INTDIR)\Test2.obj"
	-@erase "$(INTDIR)\Test3.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\SRFunc.exe"
	-@erase "$(OUTDIR)\SRFunc.ilk"
	-@erase "$(OUTDIR)\SRFunc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\SRFunc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Info.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SRFunc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib spchwrap.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\SRFunc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\SRFunc.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Frame.obj" \
	"$(INTDIR)\Info.obj" \
	"$(INTDIR)\Info.res" \
	"$(INTDIR)\Test1.obj" \
	"$(INTDIR)\Test2.obj" \
	"$(INTDIR)\Test3.obj"

"$(OUTDIR)\SRFunc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SRFunc.exe"

!ELSE 

ALL : "$(OUTDIR)\SRFunc.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Frame.obj"
	-@erase "$(INTDIR)\Info.obj"
	-@erase "$(INTDIR)\Info.res"
	-@erase "$(INTDIR)\Test1.obj"
	-@erase "$(INTDIR)\Test2.obj"
	-@erase "$(INTDIR)\Test3.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\SRFunc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\SRFunc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Info.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SRFunc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib spchwrap.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)\SRFunc.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\SRFunc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Frame.obj" \
	"$(INTDIR)\Info.obj" \
	"$(INTDIR)\Info.res" \
	"$(INTDIR)\Test1.obj" \
	"$(INTDIR)\Test2.obj" \
	"$(INTDIR)\Test3.obj"

"$(OUTDIR)\SRFunc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "SRFunc - Win32 Release" || "$(CFG)" == "SRFunc - Win32 Debug"\
 || "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"
SOURCE=.\Frame.cpp

!IF  "$(CFG)" == "SRFunc - Win32 Release"

DEP_CPP_FRAME=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Frame.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

DEP_CPP_FRAME=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Frame.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

DEP_CPP_FRAME=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Frame.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


!ENDIF 

SOURCE=.\Info.cpp

!IF  "$(CFG)" == "SRFunc - Win32 Release"

DEP_CPP_INFO_=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Info.obj" : $(SOURCE) $(DEP_CPP_INFO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

DEP_CPP_INFO_=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Info.obj" : $(SOURCE) $(DEP_CPP_INFO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

DEP_CPP_INFO_=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Info.obj" : $(SOURCE) $(DEP_CPP_INFO_) "$(INTDIR)"


!ENDIF 

SOURCE=.\Info.rc

"$(INTDIR)\Info.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Test1.cpp

!IF  "$(CFG)" == "SRFunc - Win32 Release"

DEP_CPP_TEST1=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test1.obj" : $(SOURCE) $(DEP_CPP_TEST1) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

DEP_CPP_TEST1=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test1.obj" : $(SOURCE) $(DEP_CPP_TEST1) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

DEP_CPP_TEST1=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test1.obj" : $(SOURCE) $(DEP_CPP_TEST1) "$(INTDIR)"


!ENDIF 

SOURCE=.\Test2.cpp

!IF  "$(CFG)" == "SRFunc - Win32 Release"


"$(INTDIR)\Test2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

DEP_CPP_TEST2=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test2.obj" : $(SOURCE) $(DEP_CPP_TEST2) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

DEP_CPP_TEST2=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test2.obj" : $(SOURCE) $(DEP_CPP_TEST2) "$(INTDIR)"


!ENDIF 

SOURCE=.\Test3.cpp

!IF  "$(CFG)" == "SRFunc - Win32 Release"

DEP_CPP_TEST3=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test3.obj" : $(SOURCE) $(DEP_CPP_TEST3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRFunc - Win32 Debug"

DEP_CPP_TEST3=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test3.obj" : $(SOURCE) $(DEP_CPP_TEST3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SRFunc - Win32 (ALPHA) Release"

DEP_CPP_TEST3=\
	".\Frame.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Test3.obj" : $(SOURCE) $(DEP_CPP_TEST3) "$(INTDIR)"


!ENDIF 


!ENDIF 

