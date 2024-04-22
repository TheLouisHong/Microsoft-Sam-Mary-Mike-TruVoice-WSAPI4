# Microsoft Developer Studio Generated NMAKE File, Based on TelCtl.dsp
!IF "$(CFG)" == ""
CFG=TelCtl - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TelCtl - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TelCtl - Win32 (ALPHA) Release" && "$(CFG)" !=\
 "TelCtl - Win32 (ALPHA) Debug" && "$(CFG)" != "TelCtl - Win32 Debug" &&\
 "$(CFG)" != "TelCtl - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TelCtl.mak" CFG="TelCtl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TelCtl - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "TelCtl - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "TelCtl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TelCtl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\TelCtl.dll"

!ELSE 

ALL : "$(OUTDIR)\TelCtl.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\CF.OBJ"
	-@erase "$(INTDIR)\date.obj"
	-@erase "$(INTDIR)\grammar.obj"
	-@erase "$(INTDIR)\name.obj"
	-@erase "$(INTDIR)\phonenum.obj"
	-@erase "$(INTDIR)\record.obj"
	-@erase "$(INTDIR)\spelling.obj"
	-@erase "$(INTDIR)\TelCtl.res"
	-@erase "$(INTDIR)\time.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\xtension.obj"
	-@erase "$(INTDIR)\YesNo.obj"
	-@erase "$(OUTDIR)\TelCtl.dll"
	-@erase "$(OUTDIR)\TelCtl.exp"
	-@erase "$(OUTDIR)\TelCtl.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\TelCtl.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TelCtl.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TelCtl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib spchwrap.lib\
 tapi32.lib /nologo /subsystem:windows /dll /pdb:"$(OUTDIR)\TelCtl.pdb"\
 /machine:ALPHA /def:".\telctl.def" /out:"$(OUTDIR)\TelCtl.dll"\
 /implib:"$(OUTDIR)\TelCtl.lib" 
DEF_FILE= \
	".\telctl.def"
LINK32_OBJS= \
	"$(INTDIR)\CF.OBJ" \
	"$(INTDIR)\date.obj" \
	"$(INTDIR)\grammar.obj" \
	"$(INTDIR)\name.obj" \
	"$(INTDIR)\phonenum.obj" \
	"$(INTDIR)\record.obj" \
	"$(INTDIR)\spelling.obj" \
	"$(INTDIR)\TelCtl.res" \
	"$(INTDIR)\time.obj" \
	"$(INTDIR)\xtension.obj" \
	"$(INTDIR)\YesNo.obj"

"$(OUTDIR)\TelCtl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\TelCtl.dll"

!ELSE 

ALL : "$(OUTDIR)\TelCtl.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\CF.OBJ"
	-@erase "$(INTDIR)\date.obj"
	-@erase "$(INTDIR)\grammar.obj"
	-@erase "$(INTDIR)\name.obj"
	-@erase "$(INTDIR)\phonenum.obj"
	-@erase "$(INTDIR)\record.obj"
	-@erase "$(INTDIR)\spelling.obj"
	-@erase "$(INTDIR)\TelCtl.res"
	-@erase "$(INTDIR)\time.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\xtension.obj"
	-@erase "$(INTDIR)\YesNo.obj"
	-@erase "$(OUTDIR)\TelCtl.dll"
	-@erase "$(OUTDIR)\TelCtl.exp"
	-@erase "$(OUTDIR)\TelCtl.lib"
	-@erase "$(OUTDIR)\TelCtl.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\TelCtl.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /MDd /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TelCtl.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TelCtl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib spchwrap.lib\
 tapi32.lib /nologo /subsystem:windows /dll /pdb:"$(OUTDIR)\TelCtl.pdb" /debug\
 /machine:ALPHA /def:".\telctl.def" /out:"$(OUTDIR)\TelCtl.dll"\
 /implib:"$(OUTDIR)\TelCtl.lib" 
DEF_FILE= \
	".\telctl.def"
LINK32_OBJS= \
	"$(INTDIR)\CF.OBJ" \
	"$(INTDIR)\date.obj" \
	"$(INTDIR)\grammar.obj" \
	"$(INTDIR)\name.obj" \
	"$(INTDIR)\phonenum.obj" \
	"$(INTDIR)\record.obj" \
	"$(INTDIR)\spelling.obj" \
	"$(INTDIR)\TelCtl.res" \
	"$(INTDIR)\time.obj" \
	"$(INTDIR)\xtension.obj" \
	"$(INTDIR)\YesNo.obj"

"$(OUTDIR)\TelCtl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

OUTDIR=c:\program files\microsoft speech sdk\tel
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=c:\program files\microsoft speech sdk\tel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\TelCtl.dll"

!ELSE 

ALL : "$(OUTDIR)\TelCtl.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\CF.OBJ"
	-@erase "$(INTDIR)\date.obj"
	-@erase "$(INTDIR)\grammar.obj"
	-@erase "$(INTDIR)\name.obj"
	-@erase "$(INTDIR)\phonenum.obj"
	-@erase "$(INTDIR)\record.obj"
	-@erase "$(INTDIR)\spelling.obj"
	-@erase "$(INTDIR)\TelCtl.res"
	-@erase "$(INTDIR)\time.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\xtension.obj"
	-@erase "$(INTDIR)\YesNo.obj"
	-@erase "$(OUTDIR)\TelCtl.dll"
	-@erase "$(OUTDIR)\TelCtl.exp"
	-@erase "$(OUTDIR)\TelCtl.ilk"
	-@erase "$(OUTDIR)\TelCtl.lib"
	-@erase "$(OUTDIR)\TelCtl.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\TelCtl.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TelCtl.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TelCtl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib spchwrap.lib tapi32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)\TelCtl.pdb" /debug /machine:IX86\
 /def:".\telctl.def" /out:"$(OUTDIR)\TelCtl.dll" /implib:"$(OUTDIR)\TelCtl.lib"\
 /pdbtype:sept 
DEF_FILE= \
	".\telctl.def"
LINK32_OBJS= \
	"$(INTDIR)\CF.OBJ" \
	"$(INTDIR)\date.obj" \
	"$(INTDIR)\grammar.obj" \
	"$(INTDIR)\name.obj" \
	"$(INTDIR)\phonenum.obj" \
	"$(INTDIR)\record.obj" \
	"$(INTDIR)\spelling.obj" \
	"$(INTDIR)\TelCtl.res" \
	"$(INTDIR)\time.obj" \
	"$(INTDIR)\xtension.obj" \
	"$(INTDIR)\YesNo.obj"

"$(OUTDIR)\TelCtl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\TelCtl.dll"

!ELSE 

ALL : "$(OUTDIR)\TelCtl.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\CF.OBJ"
	-@erase "$(INTDIR)\date.obj"
	-@erase "$(INTDIR)\grammar.obj"
	-@erase "$(INTDIR)\name.obj"
	-@erase "$(INTDIR)\phonenum.obj"
	-@erase "$(INTDIR)\record.obj"
	-@erase "$(INTDIR)\spelling.obj"
	-@erase "$(INTDIR)\TelCtl.res"
	-@erase "$(INTDIR)\time.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\xtension.obj"
	-@erase "$(INTDIR)\YesNo.obj"
	-@erase "$(OUTDIR)\TelCtl.dll"
	-@erase "$(OUTDIR)\TelCtl.exp"
	-@erase "$(OUTDIR)\TelCtl.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\TelCtl.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TelCtl.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TelCtl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib spchwrap.lib tapi32.lib /nologo /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)\TelCtl.pdb" /machine:IX86 /def:".\telctl.def"\
 /out:"$(OUTDIR)\TelCtl.dll" /implib:"$(OUTDIR)\TelCtl.lib" 
DEF_FILE= \
	".\telctl.def"
LINK32_OBJS= \
	"$(INTDIR)\CF.OBJ" \
	"$(INTDIR)\date.obj" \
	"$(INTDIR)\grammar.obj" \
	"$(INTDIR)\name.obj" \
	"$(INTDIR)\phonenum.obj" \
	"$(INTDIR)\record.obj" \
	"$(INTDIR)\spelling.obj" \
	"$(INTDIR)\TelCtl.res" \
	"$(INTDIR)\time.obj" \
	"$(INTDIR)\xtension.obj" \
	"$(INTDIR)\YesNo.obj"

"$(OUTDIR)\TelCtl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "TelCtl - Win32 (ALPHA) Release" || "$(CFG)" ==\
 "TelCtl - Win32 (ALPHA) Debug" || "$(CFG)" == "TelCtl - Win32 Debug" ||\
 "$(CFG)" == "TelCtl - Win32 Release"
SOURCE=.\CF.CPP

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_CF_CP=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\CF.OBJ" : $(SOURCE) $(DEP_CPP_CF_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_CF_CP=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\CF.OBJ" : $(SOURCE) $(DEP_CPP_CF_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

DEP_CPP_CF_CP=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\CF.OBJ" : $(SOURCE) $(DEP_CPP_CF_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

DEP_CPP_CF_CP=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\CF.OBJ" : $(SOURCE) $(DEP_CPP_CF_CP) "$(INTDIR)"


!ENDIF 

SOURCE=.\date.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_DATE_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\date.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_DATE_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\date.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

DEP_CPP_DATE_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\date.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

DEP_CPP_DATE_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\date.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ENDIF 

SOURCE=.\grammar.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_GRAMM=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\grammar.obj" : $(SOURCE) $(DEP_CPP_GRAMM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_GRAMM=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\grammar.obj" : $(SOURCE) $(DEP_CPP_GRAMM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

DEP_CPP_GRAMM=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\grammar.obj" : $(SOURCE) $(DEP_CPP_GRAMM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

DEP_CPP_GRAMM=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\grammar.obj" : $(SOURCE) $(DEP_CPP_GRAMM) "$(INTDIR)"


!ENDIF 

SOURCE=.\name.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_NAME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\name.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_NAME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\name.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

DEP_CPP_NAME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\name.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

DEP_CPP_NAME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\name.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ENDIF 

SOURCE=.\phonenum.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_PHONE=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\phonenum.obj" : $(SOURCE) $(DEP_CPP_PHONE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_PHONE=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\phonenum.obj" : $(SOURCE) $(DEP_CPP_PHONE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

DEP_CPP_PHONE=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\phonenum.obj" : $(SOURCE) $(DEP_CPP_PHONE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

DEP_CPP_PHONE=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\phonenum.obj" : $(SOURCE) $(DEP_CPP_PHONE) "$(INTDIR)"


!ENDIF 

SOURCE=.\record.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_RECOR=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\record.obj" : $(SOURCE) $(DEP_CPP_RECOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_RECOR=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\record.obj" : $(SOURCE) $(DEP_CPP_RECOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

DEP_CPP_RECOR=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\record.obj" : $(SOURCE) $(DEP_CPP_RECOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

DEP_CPP_RECOR=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\record.obj" : $(SOURCE) $(DEP_CPP_RECOR) "$(INTDIR)"


!ENDIF 

SOURCE=.\spelling.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_SPELL=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\spelling.obj" : $(SOURCE) $(DEP_CPP_SPELL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_SPELL=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\spelling.obj" : $(SOURCE) $(DEP_CPP_SPELL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

DEP_CPP_SPELL=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\spelling.obj" : $(SOURCE) $(DEP_CPP_SPELL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

DEP_CPP_SPELL=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\spelling.obj" : $(SOURCE) $(DEP_CPP_SPELL) "$(INTDIR)"


!ENDIF 

SOURCE=.\TelCtl.rc
DEP_RSC_TELCT=\
	".\date.tc"\
	".\grammar.tc"\
	".\name.tc"\
	".\phonenum.tc"\
	".\record.tc"\
	".\spelling.tc"\
	".\time.tc"\
	".\xtension.tc"\
	".\yesno.tc"\
	

"$(INTDIR)\TelCtl.res" : $(SOURCE) $(DEP_RSC_TELCT) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\time.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_TIME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\time.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_TIME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\time.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

DEP_CPP_TIME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\time.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

DEP_CPP_TIME_=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\time.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ENDIF 

SOURCE=.\xtension.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_XTENS=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\xtension.obj" : $(SOURCE) $(DEP_CPP_XTENS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_XTENS=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\xtension.obj" : $(SOURCE) $(DEP_CPP_XTENS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

DEP_CPP_XTENS=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\xtension.obj" : $(SOURCE) $(DEP_CPP_XTENS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

DEP_CPP_XTENS=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\xtension.obj" : $(SOURCE) $(DEP_CPP_XTENS) "$(INTDIR)"


!ENDIF 

SOURCE=.\YesNo.cpp

!IF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Release"

DEP_CPP_YESNO=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\YesNo.obj" : $(SOURCE) $(DEP_CPP_YESNO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 (ALPHA) Debug"

DEP_CPP_YESNO=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\YesNo.obj" : $(SOURCE) $(DEP_CPP_YESNO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Debug"

DEP_CPP_YESNO=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\YesNo.obj" : $(SOURCE) $(DEP_CPP_YESNO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TelCtl - Win32 Release"

DEP_CPP_YESNO=\
	".\TelCtl.h"\
	{$(INCLUDE)}"spchtel.h"\
	{$(INCLUDE)}"spchwrap.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\YesNo.obj" : $(SOURCE) $(DEP_CPP_YESNO) "$(INTDIR)"


!ENDIF 


!ENDIF 

