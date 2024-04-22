# Microsoft Developer Studio Generated NMAKE File, Based on Pizza.dsp
!IF "$(CFG)" == ""
CFG=Pizza - Win32 (ALPHA) Debug
!MESSAGE No configuration specified. Defaulting to Pizza - Win32 (ALPHA) Debug.
!ENDIF 

!IF "$(CFG)" != "Pizza - Win32 Release" && "$(CFG)" != "Pizza - Win32 Debug" &&\
 "$(CFG)" != "Pizza - Win32 (ALPHA) Release" && "$(CFG)" !=\
 "Pizza - Win32 (ALPHA) Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pizza.mak" CFG="Pizza - Win32 (ALPHA) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pizza - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Pizza - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Pizza - Win32 (ALPHA) Release" (based on "Win32 (ALPHA) Application")
!MESSAGE "Pizza - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Pizza - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Pizza.exe"

!ELSE 

ALL : "$(OUTDIR)\Pizza.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Pizza.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Telsamp.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Pizza.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Pizza.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Pizza.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib spchwrap.lib tapi32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\Pizza.pdb" /machine:I386 /out:"$(OUTDIR)\Pizza.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Pizza.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Telsamp.obj"

"$(OUTDIR)\Pizza.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Pizza - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Pizza.exe"

!ELSE 

ALL : "$(OUTDIR)\Pizza.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Pizza.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Telsamp.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Pizza.exe"
	-@erase "$(OUTDIR)\Pizza.ilk"
	-@erase "$(OUTDIR)\Pizza.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Pizza.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Pizza.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib spchwrap.lib tapi32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)\Pizza.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Pizza.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Pizza.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Telsamp.obj"

"$(OUTDIR)\Pizza.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Pizza - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Pizza.exe"

!ELSE 

ALL : "$(OUTDIR)\Pizza.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Pizza.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Telsamp.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Pizza.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Pizza.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Pizza.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=spchwrap.lib tapi32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)\Pizza.pdb" /machine:ALPHA\
 /out:"$(OUTDIR)\Pizza.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Pizza.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Telsamp.obj"

"$(OUTDIR)\Pizza.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Pizza - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Pizza.exe"

!ELSE 

ALL : "$(OUTDIR)\Pizza.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Pizza.obj"
	-@erase "$(INTDIR)\tel.res"
	-@erase "$(INTDIR)\Telsamp.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Pizza.exe"
	-@erase "$(OUTDIR)\Pizza.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Pizza.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /MDd /c 
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Pizza.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=spchwrap.lib tapi32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)\Pizza.pdb" /debug\
 /machine:ALPHA /out:"$(OUTDIR)\Pizza.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Pizza.obj" \
	"$(INTDIR)\tel.res" \
	"$(INTDIR)\Telsamp.obj"

"$(OUTDIR)\Pizza.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Pizza - Win32 Release" || "$(CFG)" == "Pizza - Win32 Debug" ||\
 "$(CFG)" == "Pizza - Win32 (ALPHA) Release" || "$(CFG)" ==\
 "Pizza - Win32 (ALPHA) Debug"
SOURCE=.\Pizza.cpp

!IF  "$(CFG)" == "Pizza - Win32 Release"

DEP_CPP_PIZZA=\
	".\pizza.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Pizza.obj" : $(SOURCE) $(DEP_CPP_PIZZA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pizza - Win32 Debug"

DEP_CPP_PIZZA=\
	".\pizza.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Pizza.obj" : $(SOURCE) $(DEP_CPP_PIZZA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pizza - Win32 (ALPHA) Release"

DEP_CPP_PIZZA=\
	".\pizza.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Pizza.obj" : $(SOURCE) $(DEP_CPP_PIZZA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pizza - Win32 (ALPHA) Debug"

DEP_CPP_PIZZA=\
	".\pizza.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Pizza.obj" : $(SOURCE) $(DEP_CPP_PIZZA) "$(INTDIR)"


!ENDIF 

SOURCE=.\tel.rc

"$(INTDIR)\tel.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Telsamp.cpp

!IF  "$(CFG)" == "Pizza - Win32 Release"

DEP_CPP_TELSA=\
	".\pizza.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Telsamp.obj" : $(SOURCE) $(DEP_CPP_TELSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pizza - Win32 Debug"

DEP_CPP_TELSA=\
	".\pizza.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Telsamp.obj" : $(SOURCE) $(DEP_CPP_TELSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pizza - Win32 (ALPHA) Release"

DEP_CPP_TELSA=\
	".\pizza.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Telsamp.obj" : $(SOURCE) $(DEP_CPP_TELSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pizza - Win32 (ALPHA) Debug"

DEP_CPP_TELSA=\
	".\pizza.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\Telsamp.obj" : $(SOURCE) $(DEP_CPP_TELSA) "$(INTDIR)"


!ENDIF 


!ENDIF 

