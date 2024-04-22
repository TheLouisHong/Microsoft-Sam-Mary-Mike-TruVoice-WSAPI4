# Microsoft Developer Studio Generated NMAKE File, Based on Ttsapp.dsp
!IF "$(CFG)" == ""
CFG=TTSAPP - Win32 Release
!MESSAGE No configuration specified. Defaulting to TTSAPP - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "TTSAPP - Win32 Release" && "$(CFG)" != "TTSAPP - Win32 Debug"\
 && "$(CFG)" != "TTSAPP - Win32 (ALPHA) Debug" && "$(CFG)" !=\
 "TTSAPP - Win32 (ALPHA) Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ttsapp.mak" CFG="TTSAPP - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TTSAPP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TTSAPP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "TTSAPP - Win32 (ALPHA) Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "TTSAPP - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TTSAPP - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Ttsapp.exe"

!ELSE 

ALL : "$(OUTDIR)\Ttsapp.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TTSAPP.obj"
	-@erase "$(INTDIR)\TTSAPP.res"
	-@erase "$(INTDIR)\ttsappd.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Ttsapp.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSAPP.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ttsapp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=nafxcw.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib winmm.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)\Ttsapp.pdb" /machine:I386\
 /out:"$(OUTDIR)\Ttsapp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TTSAPP.obj" \
	"$(INTDIR)\TTSAPP.res" \
	"$(INTDIR)\ttsappd.obj"

"$(OUTDIR)\Ttsapp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Ttsapp.exe" "$(OUTDIR)\Ttsapp.pch"

!ELSE 

ALL : "$(OUTDIR)\Ttsapp.exe" "$(OUTDIR)\Ttsapp.pch"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TTSAPP.obj"
	-@erase "$(INTDIR)\Ttsapp.pch"
	-@erase "$(INTDIR)\TTSAPP.res"
	-@erase "$(INTDIR)\ttsappd.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Ttsapp.exe"
	-@erase "$(OUTDIR)\Ttsapp.ilk"
	-@erase "$(OUTDIR)\Ttsapp.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I {$(BUILDDRV)}"\api" /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSAPP.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ttsapp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ole32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\Ttsapp.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Ttsapp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TTSAPP.obj" \
	"$(INTDIR)\TTSAPP.res" \
	"$(INTDIR)\ttsappd.obj"

"$(OUTDIR)\Ttsapp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Ttsapp.exe"

!ELSE 

ALL : "$(OUTDIR)\Ttsapp.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TTSAPP.obj"
	-@erase "$(INTDIR)\TTSAPP.res"
	-@erase "$(INTDIR)\ttsappd.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Ttsapp.exe"
	-@erase "$(OUTDIR)\Ttsapp.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSAPP.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ttsapp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /pdb:"$(OUTDIR)\Ttsapp.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\Ttsapp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TTSAPP.obj" \
	"$(INTDIR)\TTSAPP.res" \
	"$(INTDIR)\ttsappd.obj"

"$(OUTDIR)\Ttsapp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Ttsapp.exe"

!ELSE 

ALL : "$(OUTDIR)\Ttsapp.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\guidseg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TTSAPP.obj"
	-@erase "$(INTDIR)\TTSAPP.res"
	-@erase "$(INTDIR)\ttsappd.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Ttsapp.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TTSAPP.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ttsapp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=nafxcw.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 comctl32.lib /nologo /subsystem:windows /pdb:"$(OUTDIR)\Ttsapp.pdb"\
 /machine:ALPHA /out:"$(OUTDIR)\Ttsapp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\guidseg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TTSAPP.obj" \
	"$(INTDIR)\TTSAPP.res" \
	"$(INTDIR)\ttsappd.obj"

"$(OUTDIR)\Ttsapp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "TTSAPP - Win32 Release" || "$(CFG)" == "TTSAPP - Win32 Debug"\
 || "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug" || "$(CFG)" ==\
 "TTSAPP - Win32 (ALPHA) Release"
SOURCE=.\guidseg.cpp

!IF  "$(CFG)" == "TTSAPP - Win32 Release"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Ttsapp.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 Debug"

DEP_CPP_GUIDS=\
	
CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /I {$(BUILDDRV)}"\api" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch"\
 /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Release"

DEP_CPP_GUIDS=\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\guidseg.obj" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "TTSAPP - Win32 Release"

DEP_CPP_STDAF=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 Debug"

DEP_CPP_STDAF=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	
CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /I {$(BUILDDRV)}"\api" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Ttsapp.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug"

DEP_CPP_STDAF=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Release"

DEP_CPP_STDAF=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TTSAPP.cpp

!IF  "$(CFG)" == "TTSAPP - Win32 Release"

DEP_CPP_TTSAP=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Ttsapp.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\TTSAPP.obj" : $(SOURCE) $(DEP_CPP_TTSAP) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 Debug"

DEP_CPP_TTSAP=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	
CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /I {$(BUILDDRV)}"\api" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch"\
 /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TTSAPP.obj" : $(SOURCE) $(DEP_CPP_TTSAP) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug"

DEP_CPP_TTSAP=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\TTSAPP.obj" : $(SOURCE) $(DEP_CPP_TTSAP) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Release"

DEP_CPP_TTSAP=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\TTSAPP.obj" : $(SOURCE) $(DEP_CPP_TTSAP) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TTSAPP.rc
DEP_RSC_TTSAPP=\
	".\res\TTSAPP.ico"\
	".\res\TTSAPP.rc2"\
	

"$(INTDIR)\TTSAPP.res" : $(SOURCE) $(DEP_RSC_TTSAPP) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ttsappd.cpp

!IF  "$(CFG)" == "TTSAPP - Win32 Release"

DEP_CPP_TTSAPPD=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Ttsapp.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\ttsappd.obj" : $(SOURCE) $(DEP_CPP_TTSAPPD) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 Debug"

DEP_CPP_TTSAPPD=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	
CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /I {$(BUILDDRV)}"\api" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch"\
 /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ttsappd.obj" : $(SOURCE) $(DEP_CPP_TTSAPPD) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Debug"

DEP_CPP_TTSAPPD=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /MDd /c 

"$(INTDIR)\ttsappd.obj" : $(SOURCE) $(DEP_CPP_TTSAPPD) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TTSAPP - Win32 (ALPHA) Release"

DEP_CPP_TTSAPPD=\
	".\leaks.h"\
	".\StdAfx.h"\
	".\TTSAPP.h"\
	".\TTSAPPD.h"\
	{$(INCLUDE)}"speech.h"\
	
CPP_SWITCHES=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\Ttsapp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\ttsappd.obj" : $(SOURCE) $(DEP_CPP_TTSAPPD) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

