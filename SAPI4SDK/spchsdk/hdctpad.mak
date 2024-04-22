# Microsoft Developer Studio Generated NMAKE File, Based on DctPad.dsp
!IF "$(CFG)" == ""
CFG=DctPad - Win32 Debug
!MESSAGE No configuration specified. Defaulting to DctPad - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "DctPad - Win32 (ALPHA) Debug" && "$(CFG)" !=\
 "DctPad - Win32 (ALPHA) Release" && "$(CFG)" != "DctPad - Win32 Release" &&\
 "$(CFG)" != "DctPad - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DctPad.mak" CFG="DctPad - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DctPad - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "DctPad - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "DctPad - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DctPad - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "DctPad - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDebug
INTDIR=.\AlphaDebug
# Begin Custom Macros
OutDir=.\AlphaDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\DctPad.exe"

!ELSE 

ALL : "$(OUTDIR)\DctPad.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\cmnhdr.obj"
	-@erase "$(INTDIR)\dctpad.res"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\interm.obj"
	-@erase "$(INTDIR)\REditCB.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\DctPad.exe"
	-@erase "$(OUTDIR)\DctPad.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)\DctPad.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\AlphaDebug/
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\dctpad.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DctPad.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\DctPad.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\DctPad.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cmnhdr.obj" \
	"$(INTDIR)\dctpad.res" \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\interm.obj" \
	"$(INTDIR)\REditCB.obj"

"$(OUTDIR)\DctPad.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DctPad - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\DctPad.exe"

!ELSE 

ALL : "$(OUTDIR)\DctPad.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\cmnhdr.obj"
	-@erase "$(INTDIR)\dctpad.res"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\interm.obj"
	-@erase "$(INTDIR)\REditCB.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\DctPad.exe"
	-@erase "$(OUTDIR)\DctPad.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /Zi /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)\DctPad.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\dctpad.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DctPad.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\DctPad.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\DctPad.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cmnhdr.obj" \
	"$(INTDIR)\dctpad.res" \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\interm.obj" \
	"$(INTDIR)\REditCB.obj"

"$(OUTDIR)\DctPad.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DctPad - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\DctPad.exe"

!ELSE 

ALL : "$(OUTDIR)\DctPad.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\cmnhdr.obj"
	-@erase "$(INTDIR)\dctpad.res"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\interm.obj"
	-@erase "$(INTDIR)\REditCB.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\DctPad.exe"
	-@erase "$(OUTDIR)\DctPad.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)\DctPad.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /alpha 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\dctpad.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DctPad.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib comctl32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\DctPad.pdb" /debug /machine:IX86 /out:"$(OUTDIR)\DctPad.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cmnhdr.obj" \
	"$(INTDIR)\dctpad.res" \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\interm.obj" \
	"$(INTDIR)\REditCB.obj"

"$(OUTDIR)\DctPad.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DctPad - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\DctPad.exe"

!ELSE 

ALL : "$(OUTDIR)\DctPad.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\cmnhdr.obj"
	-@erase "$(INTDIR)\dctpad.res"
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\interm.obj"
	-@erase "$(INTDIR)\REditCB.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\DctPad.exe"
	-@erase "$(OUTDIR)\DctPad.ilk"
	-@erase "$(OUTDIR)\DctPad.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)\DctPad.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /alpha 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\dctpad.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DctPad.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=comctl32.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib\
 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib\
 oleaut32.lib uuid.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\DctPad.pdb" /debug /machine:IX86 /out:"$(OUTDIR)\DctPad.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\cmnhdr.obj" \
	"$(INTDIR)\dctpad.res" \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\interm.obj" \
	"$(INTDIR)\REditCB.obj"

"$(OUTDIR)\DctPad.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "DctPad - Win32 (ALPHA) Debug" || "$(CFG)" ==\
 "DctPad - Win32 (ALPHA) Release" || "$(CFG)" == "DctPad - Win32 Release" ||\
 "$(CFG)" == "DctPad - Win32 Debug"
SOURCE=.\cmnhdr.cpp

!IF  "$(CFG)" == "DctPad - Win32 (ALPHA) Debug"

DEP_CPP_CMNHD=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\cmnhdr.obj" : $(SOURCE) $(DEP_CPP_CMNHD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 (ALPHA) Release"

DEP_CPP_CMNHD=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\cmnhdr.obj" : $(SOURCE) $(DEP_CPP_CMNHD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 Release"

DEP_CPP_CMNHD=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\cmnhdr.obj" : $(SOURCE) $(DEP_CPP_CMNHD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 Debug"

DEP_CPP_CMNHD=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\cmnhdr.obj" : $(SOURCE) $(DEP_CPP_CMNHD) "$(INTDIR)"


!ENDIF 

SOURCE=.\dctpad.rc
DEP_RSC_DCTPA=\
	".\bmp00002.bmp"\
	".\Command.txt"\
	".\DictOff.ico"\
	".\DictOn.ico"\
	".\dicton1.ico"\
	".\docicon.ico"\
	".\ico00001.ico"\
	".\msdict.ico"\
	{$(INCLUDE)}"buildnum.h"\
	{$(INCLUDE)}"buildnum.rc"\
	

"$(INTDIR)\dctpad.res" : $(SOURCE) $(DEP_RSC_DCTPA) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\guidseg.cpp

!IF  "$(CFG)" == "DctPad - Win32 (ALPHA) Debug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 (ALPHA) Release"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 Release"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 Debug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ENDIF 

SOURCE=.\interm.cpp

!IF  "$(CFG)" == "DctPad - Win32 (ALPHA) Debug"

DEP_CPP_INTER=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\interm.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 (ALPHA) Release"

DEP_CPP_INTER=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\interm.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 Release"

DEP_CPP_INTER=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\interm.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 Debug"

DEP_CPP_INTER=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\interm.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"


!ENDIF 

SOURCE=.\REditCB.cpp

!IF  "$(CFG)" == "DctPad - Win32 (ALPHA) Debug"

DEP_CPP_REDIT=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\REditCB.obj" : $(SOURCE) $(DEP_CPP_REDIT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 (ALPHA) Release"

DEP_CPP_REDIT=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\REditCB.obj" : $(SOURCE) $(DEP_CPP_REDIT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 Release"

DEP_CPP_REDIT=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\REditCB.obj" : $(SOURCE) $(DEP_CPP_REDIT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DctPad - Win32 Debug"

DEP_CPP_REDIT=\
	".\cmnhdr.h"\
	".\errcodes.h"\
	".\except.h"\
	".\interm.h"\
	".\REditCB.h"\
	".\zmouse.h"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\REditCB.obj" : $(SOURCE) $(DEP_CPP_REDIT) "$(INTDIR)"


!ENDIF 


!ENDIF 

