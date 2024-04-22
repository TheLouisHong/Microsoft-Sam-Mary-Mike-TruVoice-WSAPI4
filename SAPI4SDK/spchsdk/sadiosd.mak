# Microsoft Developer Studio Generated NMAKE File, Based on Audiosd.dsp
!IF "$(CFG)" == ""
CFG=Audiosd - Win32 Release
!MESSAGE No configuration specified. Defaulting to Audiosd - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "Audiosd - Win32 Release" && "$(CFG)" !=\
 "Audiosd - Win32 Debug" && "$(CFG)" != "Audiosd - Win32 (ALPHA) Debug" &&\
 "$(CFG)" != "Audiosd - Win32 (ALPHA) Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Audiosd.mak" CFG="Audiosd - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Audiosd - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Audiosd - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Audiosd - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "Audiosd - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Audiosd - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\.\WinRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Audiosd.dll"

!ELSE 

ALL : "$(OUTDIR)\Audiosd.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AUDIOIN.OBJ"
	-@erase "$(INTDIR)\AUDIOOUT.OBJ"
	-@erase "$(INTDIR)\CF.OBJ"
	-@erase "$(INTDIR)\CTOOLS.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Audiosd.dll"
	-@erase "$(OUTDIR)\Audiosd.exp"
	-@erase "$(OUTDIR)\Audiosd.lib"
	-@erase "$(OUTDIR)\Audiosd.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Audiosd.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Audiosd.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\Audiosd.pdb"\
 /map:"$(INTDIR)\Audiosd.map" /machine:I386 /def:".\SPEECH.DEF"\
 /out:"$(OUTDIR)\Audiosd.dll" /implib:"$(OUTDIR)\Audiosd.lib" 
DEF_FILE= \
	".\SPEECH.DEF"
LINK32_OBJS= \
	"$(INTDIR)\AUDIOIN.OBJ" \
	"$(INTDIR)\AUDIOOUT.OBJ" \
	"$(INTDIR)\CF.OBJ" \
	"$(INTDIR)\CTOOLS.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ"

"$(OUTDIR)\Audiosd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\.\WinDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Audiosd.dll"

!ELSE 

ALL : "$(OUTDIR)\Audiosd.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AUDIOIN.OBJ"
	-@erase "$(INTDIR)\AUDIOOUT.OBJ"
	-@erase "$(INTDIR)\CF.OBJ"
	-@erase "$(INTDIR)\CTOOLS.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Audiosd.dll"
	-@erase "$(OUTDIR)\Audiosd.exp"
	-@erase "$(OUTDIR)\Audiosd.ilk"
	-@erase "$(OUTDIR)\Audiosd.lib"
	-@erase "$(OUTDIR)\Audiosd.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Audiosd.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Audiosd.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\Audiosd.pdb" /debug\
 /machine:I386 /def:".\SPEECH.DEF" /out:"$(OUTDIR)\Audiosd.dll"\
 /implib:"$(OUTDIR)\Audiosd.lib" 
DEF_FILE= \
	".\SPEECH.DEF"
LINK32_OBJS= \
	"$(INTDIR)\AUDIOIN.OBJ" \
	"$(INTDIR)\AUDIOOUT.OBJ" \
	"$(INTDIR)\CF.OBJ" \
	"$(INTDIR)\CTOOLS.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ"

"$(OUTDIR)\Audiosd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

OUTDIR=.\AlphaDbg
INTDIR=.\AlphaDbg
# Begin Custom Macros
OutDir=.\.\AlphaDbg
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Audiosd.dll"

!ELSE 

ALL : "$(OUTDIR)\Audiosd.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AUDIOIN.OBJ"
	-@erase "$(INTDIR)\AUDIOOUT.OBJ"
	-@erase "$(INTDIR)\CF.OBJ"
	-@erase "$(INTDIR)\CTOOLS.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Audiosd.dll"
	-@erase "$(OUTDIR)\Audiosd.exp"
	-@erase "$(OUTDIR)\Audiosd.lib"
	-@erase "$(OUTDIR)\Audiosd.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Audiosd.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /MTd /c 
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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Audiosd.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /dll\
 /pdb:"$(OUTDIR)\Audiosd.pdb" /debug /machine:ALPHA /def:".\SPEECH.DEF"\
 /out:"$(OUTDIR)\Audiosd.dll" /implib:"$(OUTDIR)\Audiosd.lib" 
DEF_FILE= \
	".\SPEECH.DEF"
LINK32_OBJS= \
	"$(INTDIR)\AUDIOIN.OBJ" \
	"$(INTDIR)\AUDIOOUT.OBJ" \
	"$(INTDIR)\CF.OBJ" \
	"$(INTDIR)\CTOOLS.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ"

"$(OUTDIR)\Audiosd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

OUTDIR=.\AlphaRel
INTDIR=.\AlphaRel
# Begin Custom Macros
OutDir=.\.\AlphaRel
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Audiosd.dll"

!ELSE 

ALL : "$(OUTDIR)\Audiosd.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AUDIOIN.OBJ"
	-@erase "$(INTDIR)\AUDIOOUT.OBJ"
	-@erase "$(INTDIR)\CF.OBJ"
	-@erase "$(INTDIR)\CTOOLS.OBJ"
	-@erase "$(INTDIR)\GUIDSEG.OBJ"
	-@erase "$(OUTDIR)\Audiosd.dll"
	-@erase "$(OUTDIR)\Audiosd.exp"
	-@erase "$(OUTDIR)\Audiosd.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /alpha 
CPP=cl.exe
CPP_PROJ=/nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Audiosd.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Audiosd.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /dll\
 /pdb:"$(OUTDIR)\Audiosd.pdb" /machine:ALPHA /def:".\SPEECH.DEF"\
 /out:"$(OUTDIR)\Audiosd.dll" /implib:"$(OUTDIR)\Audiosd.lib" 
DEF_FILE= \
	".\SPEECH.DEF"
LINK32_OBJS= \
	"$(INTDIR)\AUDIOIN.OBJ" \
	"$(INTDIR)\AUDIOOUT.OBJ" \
	"$(INTDIR)\CF.OBJ" \
	"$(INTDIR)\CTOOLS.OBJ" \
	"$(INTDIR)\GUIDSEG.OBJ"

"$(OUTDIR)\Audiosd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Audiosd - Win32 Release" || "$(CFG)" ==\
 "Audiosd - Win32 Debug" || "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug" ||\
 "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"
SOURCE=.\AUDIOIN.CPP

!IF  "$(CFG)" == "Audiosd - Win32 Release"

DEP_CPP_AUDIO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\AUDIOIN.OBJ" : $(SOURCE) $(DEP_CPP_AUDIO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

DEP_CPP_AUDIO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\AUDIOIN.OBJ" : $(SOURCE) $(DEP_CPP_AUDIO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

DEP_CPP_AUDIO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\AUDIOIN.OBJ" : $(SOURCE) $(DEP_CPP_AUDIO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

DEP_CPP_AUDIO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\AUDIOIN.OBJ" : $(SOURCE) $(DEP_CPP_AUDIO) "$(INTDIR)"


!ENDIF 

SOURCE=.\AUDIOOUT.CPP

!IF  "$(CFG)" == "Audiosd - Win32 Release"

DEP_CPP_AUDIOO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\AUDIOOUT.OBJ" : $(SOURCE) $(DEP_CPP_AUDIOO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

DEP_CPP_AUDIOO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\AUDIOOUT.OBJ" : $(SOURCE) $(DEP_CPP_AUDIOO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

DEP_CPP_AUDIOO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\AUDIOOUT.OBJ" : $(SOURCE) $(DEP_CPP_AUDIOO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

DEP_CPP_AUDIOO=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\AUDIOOUT.OBJ" : $(SOURCE) $(DEP_CPP_AUDIOO) "$(INTDIR)"


!ENDIF 

SOURCE=.\CF.CPP

!IF  "$(CFG)" == "Audiosd - Win32 Release"

DEP_CPP_CF_CP=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\CF.OBJ" : $(SOURCE) $(DEP_CPP_CF_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

DEP_CPP_CF_CP=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\CF.OBJ" : $(SOURCE) $(DEP_CPP_CF_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

DEP_CPP_CF_CP=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\CF.OBJ" : $(SOURCE) $(DEP_CPP_CF_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

DEP_CPP_CF_CP=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\CF.OBJ" : $(SOURCE) $(DEP_CPP_CF_CP) "$(INTDIR)"


!ENDIF 

SOURCE=.\CTOOLS.CPP

!IF  "$(CFG)" == "Audiosd - Win32 Release"

DEP_CPP_CTOOL=\
	".\CTOOLS.H"\
	

"$(INTDIR)\CTOOLS.OBJ" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

DEP_CPP_CTOOL=\
	".\CTOOLS.H"\
	

"$(INTDIR)\CTOOLS.OBJ" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

DEP_CPP_CTOOL=\
	".\CTOOLS.H"\
	

"$(INTDIR)\CTOOLS.OBJ" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

DEP_CPP_CTOOL=\
	".\CTOOLS.H"\
	

"$(INTDIR)\CTOOLS.OBJ" : $(SOURCE) $(DEP_CPP_CTOOL) "$(INTDIR)"


!ENDIF 

SOURCE=.\GUIDSEG.CPP

!IF  "$(CFG)" == "Audiosd - Win32 Release"

DEP_CPP_GUIDS=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 Debug"

DEP_CPP_GUIDS=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Debug"

DEP_CPP_GUIDS=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Audiosd - Win32 (ALPHA) Release"

DEP_CPP_GUIDS=\
	".\AUDIO.H"\
	".\AUDIOINC.H"\
	".\CTOOLS.H"\
	{$(INCLUDE)}"speech.h"\
	

"$(INTDIR)\GUIDSEG.OBJ" : $(SOURCE) $(DEP_CPP_GUIDS) "$(INTDIR)"


!ENDIF 


!ENDIF 

