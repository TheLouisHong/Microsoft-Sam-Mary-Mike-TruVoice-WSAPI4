# Microsoft Developer Studio Generated NMAKE File, Based on SpchWrap.dsp
!IF "$(CFG)" == ""
CFG=SpchWrap - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SpchWrap - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SpchWrap - Win32 (ALPHA) Release" && "$(CFG)" !=\
 "SpchWrap - Win32 (ALPHA) Debug" && "$(CFG)" != "SpchWrap - Win32 Release" &&\
 "$(CFG)" != "SpchWrap - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SpchWrap.mak" CFG="SpchWrap - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SpchWrap - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "SpchWrap - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "SpchWrap - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SpchWrap - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SpchWrap.lib"

!ELSE 

ALL : "$(OUTDIR)\SpchWrap.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\link.obj"
	-@erase "$(INTDIR)\LowSR.obj"
	-@erase "$(INTDIR)\LowTTS.obj"
	-@erase "$(INTDIR)\STools.obj"
	-@erase "$(INTDIR)\Teleph.obj"
	-@erase "$(INTDIR)\TelFrame.obj"
	-@erase "$(INTDIR)\Telobj.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\VCmd.obj"
	-@erase "$(INTDIR)\VDct.obj"
	-@erase "$(INTDIR)\VTxt.obj"
	-@erase "$(OUTDIR)\SpchWrap.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\SpchWrap.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SpchWrap.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\SpchWrap.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\link.obj" \
	"$(INTDIR)\LowSR.obj" \
	"$(INTDIR)\LowTTS.obj" \
	"$(INTDIR)\STools.obj" \
	"$(INTDIR)\Teleph.obj" \
	"$(INTDIR)\TelFrame.obj" \
	"$(INTDIR)\Telobj.obj" \
	"$(INTDIR)\VCmd.obj" \
	"$(INTDIR)\VDct.obj" \
	"$(INTDIR)\VTxt.obj"

"$(OUTDIR)\SpchWrap.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

OUTDIR=$(BUILDDRV)\lib
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=$(BUILDDRV)\lib
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SpchWrap.lib"

!ELSE 

ALL : "$(OUTDIR)\SpchWrap.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\link.obj"
	-@erase "$(INTDIR)\LowSR.obj"
	-@erase "$(INTDIR)\LowTTS.obj"
	-@erase "$(INTDIR)\STools.obj"
	-@erase "$(INTDIR)\Teleph.obj"
	-@erase "$(INTDIR)\TelFrame.obj"
	-@erase "$(INTDIR)\Telobj.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\VCmd.obj"
	-@erase "$(INTDIR)\VDct.obj"
	-@erase "$(INTDIR)\VTxt.obj"
	-@erase "$(OUTDIR)\SpchWrap.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\SpchWrap.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /MDd /c 
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

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SpchWrap.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\SpchWrap.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\link.obj" \
	"$(INTDIR)\LowSR.obj" \
	"$(INTDIR)\LowTTS.obj" \
	"$(INTDIR)\STools.obj" \
	"$(INTDIR)\Teleph.obj" \
	"$(INTDIR)\TelFrame.obj" \
	"$(INTDIR)\Telobj.obj" \
	"$(INTDIR)\VCmd.obj" \
	"$(INTDIR)\VDct.obj" \
	"$(INTDIR)\VTxt.obj"

"$(OUTDIR)\SpchWrap.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SpchWrap.lib"

!ELSE 

ALL : "$(OUTDIR)\SpchWrap.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\link.obj"
	-@erase "$(INTDIR)\LowSR.obj"
	-@erase "$(INTDIR)\LowTTS.obj"
	-@erase "$(INTDIR)\STools.obj"
	-@erase "$(INTDIR)\Teleph.obj"
	-@erase "$(INTDIR)\TelFrame.obj"
	-@erase "$(INTDIR)\Telobj.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\VCmd.obj"
	-@erase "$(INTDIR)\VDct.obj"
	-@erase "$(INTDIR)\VTxt.obj"
	-@erase "$(OUTDIR)\SpchWrap.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\SpchWrap.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SpchWrap.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\SpchWrap.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\link.obj" \
	"$(INTDIR)\LowSR.obj" \
	"$(INTDIR)\LowTTS.obj" \
	"$(INTDIR)\STools.obj" \
	"$(INTDIR)\Teleph.obj" \
	"$(INTDIR)\TelFrame.obj" \
	"$(INTDIR)\Telobj.obj" \
	"$(INTDIR)\VCmd.obj" \
	"$(INTDIR)\VDct.obj" \
	"$(INTDIR)\VTxt.obj"

"$(OUTDIR)\SpchWrap.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SpchWrap.lib"

!ELSE 

ALL : "$(OUTDIR)\SpchWrap.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ctools.obj"
	-@erase "$(INTDIR)\link.obj"
	-@erase "$(INTDIR)\LowSR.obj"
	-@erase "$(INTDIR)\LowTTS.obj"
	-@erase "$(INTDIR)\STools.obj"
	-@erase "$(INTDIR)\Teleph.obj"
	-@erase "$(INTDIR)\TelFrame.obj"
	-@erase "$(INTDIR)\Telobj.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\VCmd.obj"
	-@erase "$(INTDIR)\VDct.obj"
	-@erase "$(INTDIR)\VTxt.obj"
	-@erase "$(OUTDIR)\SpchWrap.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\SpchWrap.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SpchWrap.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\SpchWrap.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ctools.obj" \
	"$(INTDIR)\link.obj" \
	"$(INTDIR)\LowSR.obj" \
	"$(INTDIR)\LowTTS.obj" \
	"$(INTDIR)\STools.obj" \
	"$(INTDIR)\Teleph.obj" \
	"$(INTDIR)\TelFrame.obj" \
	"$(INTDIR)\Telobj.obj" \
	"$(INTDIR)\VCmd.obj" \
	"$(INTDIR)\VDct.obj" \
	"$(INTDIR)\VTxt.obj"

"$(OUTDIR)\SpchWrap.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release" || "$(CFG)" ==\
 "SpchWrap - Win32 (ALPHA) Debug" || "$(CFG)" == "SpchWrap - Win32 Release" ||\
 "$(CFG)" == "SpchWrap - Win32 Debug"
SOURCE=.\ctools.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_CTOOL=\
	".\SpchWrap.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_CTOOL=\
	".\SpchWrap.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_CTOOL=\
	".\SpchWrap.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_CTOOL=\
	".\SpchWrap.h"\
	

"$(INTDIR)\ctools.obj" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ENDIF 

SOURCE=.\link.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_LINK_=\
	".\link.h"\
	

"$(INTDIR)\link.obj" : $(SOURCE) $(DEP_CPP_LINK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_LINK_=\
	".\link.h"\
	

"$(INTDIR)\link.obj" : $(SOURCE) $(DEP_CPP_LINK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_LINK_=\
	".\link.h"\
	

"$(INTDIR)\link.obj" : $(SOURCE) $(DEP_CPP_LINK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_LINK_=\
	".\link.h"\
	

"$(INTDIR)\link.obj" : $(SOURCE) $(DEP_CPP_LINK_) "$(INTDIR)"


!ENDIF 

SOURCE=.\LowSR.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_LOWSR=\
	".\SpchWrap.h"\
	

"$(INTDIR)\LowSR.obj" : $(SOURCE) $(DEP_CPP_LOWSR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_LOWSR=\
	".\SpchWrap.h"\
	

"$(INTDIR)\LowSR.obj" : $(SOURCE) $(DEP_CPP_LOWSR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_LOWSR=\
	".\SpchWrap.h"\
	

"$(INTDIR)\LowSR.obj" : $(SOURCE) $(DEP_CPP_LOWSR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_LOWSR=\
	".\SpchWrap.h"\
	

"$(INTDIR)\LowSR.obj" : $(SOURCE) $(DEP_CPP_LOWSR) "$(INTDIR)"


!ENDIF 

SOURCE=.\LowTTS.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_LOWTT=\
	".\SpchWrap.h"\
	

"$(INTDIR)\LowTTS.obj" : $(SOURCE) $(DEP_CPP_LOWTT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_LOWTT=\
	".\SpchWrap.h"\
	

"$(INTDIR)\LowTTS.obj" : $(SOURCE) $(DEP_CPP_LOWTT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_LOWTT=\
	".\SpchWrap.h"\
	

"$(INTDIR)\LowTTS.obj" : $(SOURCE) $(DEP_CPP_LOWTT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_LOWTT=\
	".\SpchWrap.h"\
	

"$(INTDIR)\LowTTS.obj" : $(SOURCE) $(DEP_CPP_LOWTT) "$(INTDIR)"


!ENDIF 

SOURCE=.\STools.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_STOOL=\
	".\SpchWrap.h"\
	

"$(INTDIR)\STools.obj" : $(SOURCE) $(DEP_CPP_STOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_STOOL=\
	".\SpchWrap.h"\
	

"$(INTDIR)\STools.obj" : $(SOURCE) $(DEP_CPP_STOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_STOOL=\
	".\SpchWrap.h"\
	

"$(INTDIR)\STools.obj" : $(SOURCE) $(DEP_CPP_STOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_STOOL=\
	".\SpchWrap.h"\
	

"$(INTDIR)\STools.obj" : $(SOURCE) $(DEP_CPP_STOOL) "$(INTDIR)"


!ENDIF 

SOURCE=.\Teleph.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_TELEP=\
	".\SpchWrap.h"\
	

"$(INTDIR)\Teleph.obj" : $(SOURCE) $(DEP_CPP_TELEP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_TELEP=\
	".\SpchWrap.h"\
	

"$(INTDIR)\Teleph.obj" : $(SOURCE) $(DEP_CPP_TELEP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_TELEP=\
	".\SpchWrap.h"\
	

"$(INTDIR)\Teleph.obj" : $(SOURCE) $(DEP_CPP_TELEP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_TELEP=\
	".\SpchWrap.h"\
	

"$(INTDIR)\Teleph.obj" : $(SOURCE) $(DEP_CPP_TELEP) "$(INTDIR)"


!ENDIF 

SOURCE=.\TelFrame.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_TELFR=\
	".\SpchWrap.h"\
	

"$(INTDIR)\TelFrame.obj" : $(SOURCE) $(DEP_CPP_TELFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_TELFR=\
	".\SpchWrap.h"\
	

"$(INTDIR)\TelFrame.obj" : $(SOURCE) $(DEP_CPP_TELFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_TELFR=\
	".\SpchWrap.h"\
	

"$(INTDIR)\TelFrame.obj" : $(SOURCE) $(DEP_CPP_TELFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_TELFR=\
	".\SpchWrap.h"\
	

"$(INTDIR)\TelFrame.obj" : $(SOURCE) $(DEP_CPP_TELFR) "$(INTDIR)"


!ENDIF 

SOURCE=.\Telobj.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_TELOB=\
	".\link.h"\
	".\SpchWrap.h"\
	

"$(INTDIR)\Telobj.obj" : $(SOURCE) $(DEP_CPP_TELOB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_TELOB=\
	".\link.h"\
	".\SpchWrap.h"\
	

"$(INTDIR)\Telobj.obj" : $(SOURCE) $(DEP_CPP_TELOB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_TELOB=\
	".\link.h"\
	".\SpchWrap.h"\
	

"$(INTDIR)\Telobj.obj" : $(SOURCE) $(DEP_CPP_TELOB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_TELOB=\
	".\link.h"\
	".\SpchWrap.h"\
	

"$(INTDIR)\Telobj.obj" : $(SOURCE) $(DEP_CPP_TELOB) "$(INTDIR)"


!ENDIF 

SOURCE=.\VCmd.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_VCMD_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VCmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_VCMD_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VCmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_VCMD_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VCmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_VCMD_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VCmd.obj" : $(SOURCE) $(DEP_CPP_VCMD_) "$(INTDIR)"


!ENDIF 

SOURCE=.\VDct.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_VDCT_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VDct.obj" : $(SOURCE) $(DEP_CPP_VDCT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_VDCT_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VDct.obj" : $(SOURCE) $(DEP_CPP_VDCT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_VDCT_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VDct.obj" : $(SOURCE) $(DEP_CPP_VDCT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_VDCT_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VDct.obj" : $(SOURCE) $(DEP_CPP_VDCT_) "$(INTDIR)"


!ENDIF 

SOURCE=.\VTxt.cpp

!IF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Release"

DEP_CPP_VTXT_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VTxt.obj" : $(SOURCE) $(DEP_CPP_VTXT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 (ALPHA) Debug"

DEP_CPP_VTXT_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VTxt.obj" : $(SOURCE) $(DEP_CPP_VTXT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Release"

DEP_CPP_VTXT_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VTxt.obj" : $(SOURCE) $(DEP_CPP_VTXT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SpchWrap - Win32 Debug"

DEP_CPP_VTXT_=\
	".\SpchWrap.h"\
	

"$(INTDIR)\VTxt.obj" : $(SOURCE) $(DEP_CPP_VTXT_) "$(INTDIR)"


!ENDIF 


!ENDIF 

