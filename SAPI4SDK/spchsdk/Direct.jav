//
// Auto-generated using JActiveX.EXE 4.79.2337
//   (d:jactivex /javatlb /xi directsr.tlb)
//
// WARNING: Do not remove the comments that include "@com" directives.
// This source file must be compiled by a @com-aware compiler.
// If you are using the Microsoft Visual J++ compiler, you must use
// version 1.02.3920 or later. Previous versions will not issue an error
// but will not generate COM-enabled class files.
//

package directsr;

import com.ms.com.*;
import com.ms.com.IUnknown;
import com.ms.com.Variant;

/** @com.class(classid=4E3D9D1F-0C63-11D1-8BFB-0060081841DE,DynamicCasts)
    @com.interface(iid=4E3D9D1E-0C63-11D1-8BFB-0060081841DE, thread=AUTO, type=DUAL)
*/
public class DirectSR implements IUnknown,com.ms.com.NoAutoScripting,IDirectSR
{
  /** @com.method(vtoffset=4, dispid=1, type=PROPGET, name="debug", name2="getdebug")
      @com.parameters([type=I2] return) */
  public native short getdebug();

  /** @com.method(vtoffset=5, dispid=1, type=PROPPUT, name="debug", name2="putdebug")
      @com.parameters([in,type=I2] pVal) */
  public native void putdebug(short pVal);

  /** @com.method(vtoffset=6, dispid=2, type=PROPGET, name="Initialized", name2="getInitialized")
      @com.parameters([type=I2] return) */
  public native short getInitialized();

  /** @com.method(vtoffset=7, dispid=2, type=PROPPUT, name="Initialized", name2="putInitialized")
      @com.parameters([in,type=I2] pVal) */
  public native void putInitialized(short pVal);

  /** @com.method(vtoffset=8, dispid=4, type=METHOD, name="Deactivate")
      @com.parameters() */
  public native void Deactivate();

  /** @com.method(vtoffset=9, dispid=5, type=METHOD, name="Activate")
      @com.parameters() */
  public native void Activate();

  /** @com.method(vtoffset=10, dispid=6, type=PROPGET, name="LastHeard", name2="getLastHeard")
      @com.parameters([type=STRING] return) */
  public native String getLastHeard();

  /** @com.method(vtoffset=11, dispid=6, type=PROPPUT, name="LastHeard", name2="putLastHeard")
      @com.parameters([in,type=STRING] pVal) */
  public native void putLastHeard(String pVal);

  /** @com.method(vtoffset=12, dispid=7, type=METHOD, name="GrammarFromString")
      @com.parameters([in,type=STRING] grammar) */
  public native void GrammarFromString(String grammar);

  /** @com.method(vtoffset=13, dispid=8, type=METHOD, name="GrammarFromFile")
      @com.parameters([in,type=STRING] FileName) */
  public native void GrammarFromFile(String FileName);

  /** @com.method(vtoffset=14, dispid=9, type=METHOD, name="GrammarFromResource")
      @com.parameters([in,type=I4] Instance, [in,type=I4] ResID) */
  public native void GrammarFromResource(int Instance, int ResID);

  /** @com.method(vtoffset=15, dispid=10, type=METHOD, name="GrammarFromStream")
      @com.parameters([in,type=I4] Stream) */
  public native void GrammarFromStream(int Stream);

  /** @com.method(vtoffset=16, dispid=11, type=PROPGET, name="AutoGain", name2="getAutoGain")
      @com.parameters([type=I4] return) */
  public native int getAutoGain();

  /** @com.method(vtoffset=17, dispid=11, type=PROPPUT, name="AutoGain", name2="putAutoGain")
      @com.parameters([in,type=I4] pVal) */
  public native void putAutoGain(int pVal);

  /** @com.method(vtoffset=18, dispid=12, type=PROPGET, name="MinAutoGain", name2="getMinAutoGain")
      @com.parameters([type=I4] return) */
  public native int getMinAutoGain();

  /** @com.method(vtoffset=19, dispid=13, type=PROPGET, name="MaxAutoGain", name2="getMaxAutoGain")
      @com.parameters([type=I4] return) */
  public native int getMaxAutoGain();

  /** @com.method(vtoffset=20, dispid=14, type=PROPGET, name="Echo", name2="getEcho")
      @com.parameters([type=I2] return) */
  public native short getEcho();

  /** @com.method(vtoffset=21, dispid=14, type=PROPPUT, name="Echo", name2="putEcho")
      @com.parameters([in,type=I2] pVal) */
  public native void putEcho(short pVal);

  /** @com.method(vtoffset=22, dispid=15, type=PROPGET, name="EnergyFloor", name2="getEnergyFloor")
      @com.parameters([type=I4] return) */
  public native int getEnergyFloor();

  /** @com.method(vtoffset=23, dispid=15, type=PROPPUT, name="EnergyFloor", name2="putEnergyFloor")
      @com.parameters([in,type=I4] pVal) */
  public native void putEnergyFloor(int pVal);

  /** @com.method(vtoffset=24, dispid=16, type=PROPGET, name="MaxEnergyFloor", name2="getMaxEnergyFloor")
      @com.parameters([type=I4] return) */
  public native int getMaxEnergyFloor();

  /** @com.method(vtoffset=25, dispid=17, type=PROPGET, name="MinEnergyFloor", name2="getMinEnergyFloor")
      @com.parameters([type=I4] return) */
  public native int getMinEnergyFloor();

  /** @com.method(vtoffset=26, dispid=18, type=PROPGET, name="Microphone", name2="getMicrophone")
      @com.parameters([type=STRING] return) */
  public native String getMicrophone();

  /** @com.method(vtoffset=27, dispid=18, type=PROPPUT, name="Microphone", name2="putMicrophone")
      @com.parameters([in,type=STRING] pVal) */
  public native void putMicrophone(String pVal);

  /** @com.method(vtoffset=28, dispid=19, type=PROPGET, name="Speaker", name2="getSpeaker")
      @com.parameters([type=STRING] return) */
  public native String getSpeaker();

  /** @com.method(vtoffset=29, dispid=19, type=PROPPUT, name="Speaker", name2="putSpeaker")
      @com.parameters([in,type=STRING] pVal) */
  public native void putSpeaker(String pVal);

  /** @com.method(vtoffset=30, dispid=20, type=PROPGET, name="RealTime", name2="getRealTime")
      @com.parameters([type=I4] return) */
  public native int getRealTime();

  /** @com.method(vtoffset=31, dispid=20, type=PROPPUT, name="RealTime", name2="putRealTime")
      @com.parameters([in,type=I4] pVal) */
  public native void putRealTime(int pVal);

  /** @com.method(vtoffset=32, dispid=21, type=PROPGET, name="MaxRealTime", name2="getMaxRealTime")
      @com.parameters([type=I4] return) */
  public native int getMaxRealTime();

  /** @com.method(vtoffset=33, dispid=22, type=PROPGET, name="MinRealTime", name2="getMinRealTime")
      @com.parameters([type=I4] return) */
  public native int getMinRealTime();

  /** @com.method(vtoffset=34, dispid=23, type=PROPGET, name="Threshold", name2="getThreshold")
      @com.parameters([type=I4] return) */
  public native int getThreshold();

  /** @com.method(vtoffset=35, dispid=23, type=PROPPUT, name="Threshold", name2="putThreshold")
      @com.parameters([in,type=I4] pVal) */
  public native void putThreshold(int pVal);

  /** @com.method(vtoffset=36, dispid=24, type=PROPGET, name="MaxThreshold", name2="getMaxThreshold")
      @com.parameters([type=I4] return) */
  public native int getMaxThreshold();

  /** @com.method(vtoffset=37, dispid=25, type=PROPGET, name="MinThreshold", name2="getMinThreshold")
      @com.parameters([type=I4] return) */
  public native int getMinThreshold();

  /** @com.method(vtoffset=38, dispid=26, type=PROPGET, name="CompleteTimeOut", name2="getCompleteTimeOut")
      @com.parameters([type=I4] return) */
  public native int getCompleteTimeOut();

  /** @com.method(vtoffset=39, dispid=26, type=PROPPUT, name="CompleteTimeOut", name2="putCompleteTimeOut")
      @com.parameters([in,type=I4] pVal) */
  public native void putCompleteTimeOut(int pVal);

  /** @com.method(vtoffset=40, dispid=27, type=PROPGET, name="IncompleteTimeOut", name2="getIncompleteTimeOut")
      @com.parameters([type=I4] return) */
  public native int getIncompleteTimeOut();

  /** @com.method(vtoffset=41, dispid=27, type=PROPPUT, name="IncompleteTimeOut", name2="putIncompleteTimeOut")
      @com.parameters([in,type=I4] pVal) */
  public native void putIncompleteTimeOut(int pVal);

  /** @com.method(vtoffset=42, dispid=28, type=PROPGET, name="MaxCompleteTimeOut", name2="getMaxCompleteTimeOut")
      @com.parameters([type=I4] return) */
  public native int getMaxCompleteTimeOut();

  /** @com.method(vtoffset=43, dispid=29, type=PROPGET, name="MinCompleteTimeOut", name2="getMinCompleteTimeOut")
      @com.parameters([type=I4] return) */
  public native int getMinCompleteTimeOut();

  /** @com.method(vtoffset=44, dispid=30, type=PROPGET, name="MaxIncompleteTimeOut", name2="getMaxIncompleteTimeOut")
      @com.parameters([type=I4] return) */
  public native int getMaxIncompleteTimeOut();

  /** @com.method(vtoffset=45, dispid=31, type=PROPGET, name="MinIncompleteTimeOut", name2="getMinIncompleteTimeOut")
      @com.parameters([type=I4] return) */
  public native int getMinIncompleteTimeOut();

  /** @com.method(vtoffset=46, dispid=32, type=METHOD, name="Pause")
      @com.parameters() */
  public native void Pause();

  /** @com.method(vtoffset=47, dispid=33, type=METHOD, name="Resume")
      @com.parameters() */
  public native void Resume();

  /** @com.method(vtoffset=48, dispid=34, type=METHOD, name="PosnGet")
      @com.parameters([out,size=1,elementType=I4,type=ARRAY] hi, [out,size=1,elementType=I4,type=ARRAY] lo) */
  public native void PosnGet(int[] hi, int[] lo);

  /** @com.method(vtoffset=49, dispid=35, type=METHOD, name="AboutDlg")
      @com.parameters([in,type=I4] hwnd, [in,type=STRING] title) */
  public native void AboutDlg(int hwnd, String title);

  /** @com.method(vtoffset=50, dispid=36, type=METHOD, name="GeneralDlg")
      @com.parameters([in,type=I4] hwnd, [in,type=STRING] title) */
  public native void GeneralDlg(int hwnd, String title);

  /** @com.method(vtoffset=51, dispid=37, type=METHOD, name="LexiconDlg")
      @com.parameters([in,type=I4] hwnd, [in,type=STRING] title) */
  public native void LexiconDlg(int hwnd, String title);

  /** @com.method(vtoffset=52, dispid=38, type=METHOD, name="TrainGeneralDlg")
      @com.parameters([in,type=I4] hwnd, [in,type=STRING] title) */
  public native void TrainGeneralDlg(int hwnd, String title);

  /** @com.method(vtoffset=53, dispid=39, type=METHOD, name="TrainMicDlg")
      @com.parameters([in,type=I4] hwnd, [in,type=STRING] title) */
  public native void TrainMicDlg(int hwnd, String title);

  /** @com.method(vtoffset=54, dispid=40, type=PROPGET, name="Wave", name2="getWave")
      @com.parameters([in,type=I4] Result, [type=I4] return) */
  public native int getWave(int Result);

  /** @com.method(vtoffset=55, dispid=41, type=PROPGET, name="Phrase", name2="getPhrase")
      @com.parameters([in,type=I4] Result, [in,type=I4] rank, [type=STRING] return) */
  public native String getPhrase(int Result, int rank);

  /** @com.method(vtoffset=56, dispid=42, type=PROPGET, name="CreateResultsObject", name2="getCreateResultsObject")
      @com.parameters([in,type=I4] results, [type=I4] return) */
  public native int getCreateResultsObject(int results);

  /** @com.method(vtoffset=57, dispid=43, type=METHOD, name="DestroyResultsObject")
      @com.parameters([in,type=I4] resobj) */
  public native void DestroyResultsObject(int resobj);

  /** @com.method(vtoffset=58, dispid=44, type=METHOD, name="Select")
      @com.parameters([in,type=I4] index) */
  public native void Select(int index);

  /** @com.method(vtoffset=59, dispid=45, type=METHOD, name="Listen")
      @com.parameters() */
  public native void Listen();

  /** @com.method(vtoffset=60, dispid=46, type=METHOD, name="SelectEngine")
      @com.parameters([in,type=I4] index) */
  public native void SelectEngine(int index);

  /** @com.method(vtoffset=61, dispid=47, type=METHOD, name="FindEngine")
      @com.parameters([in,type=STRING] EngineId, [in,type=STRING] MfgName, [in,type=STRING] ProductName, [in,type=STRING] ModeID, [in,type=STRING] ModeName, [in,type=I4] LanguageID, [in,type=STRING] dialect, [in,type=I4] Sequencing, [in,type=I4] MaxWordsVocab, [in,type=I4] MaxWordsState, [in,type=I4] Grammars, [in,type=I4] Features, [in,type=I4] Interfaces, [in,type=I4] EngineFeatures, [in,type=I4] RankEngineID, [in,type=I4] RankMfgName, [in,type=I4] RankProductName, [in,type=I4] RankModeID, [in,type=I4] RankModeName, [in,type=I4] RankLanguage, [in,type=I4] RankDialect, [in,type=I4] RankSequencing, [in,type=I4] RankMaxWordsVocab, [in,type=I4] RankMaxWordsState, [in,type=I4] RankGrammars, [in,type=I4] RankFeatures, [in,type=I4] RankInterfaces, [in,type=I4] RankEngineFeatures, [type=I4] return) */
  public native int FindEngine(String EngineId, String MfgName, String ProductName, String ModeID, String ModeName, int LanguageID, String dialect, int Sequencing, int MaxWordsVocab, int MaxWordsState, int Grammars, int Features, int Interfaces, int EngineFeatures, int RankEngineID, int RankMfgName, int RankProductName, int RankModeID, int RankModeName, int RankLanguage, int RankDialect, int RankSequencing, int RankMaxWordsVocab, int RankMaxWordsState, int RankGrammars, int RankFeatures, int RankInterfaces, int RankEngineFeatures);

  /** @com.method(vtoffset=62, dispid=48, type=PROPGET, name="CountEngines", name2="getCountEngines")
      @com.parameters([type=I4] return) */
  public native int getCountEngines();

  /** @com.method(vtoffset=63, dispid=49, type=METHOD, name="ModeName")
      @com.parameters([in,type=I4] index, [type=STRING] return) */
  public native String ModeName(int index);

  /** @com.method(vtoffset=64, dispid=50, type=METHOD, name="EngineId")
      @com.parameters([in,type=I4] index, [type=STRING] return) */
  public native String EngineId(int index);

  /** @com.method(vtoffset=65, dispid=51, type=METHOD, name="MfgName")
      @com.parameters([in,type=I4] index, [type=STRING] return) */
  public native String MfgName(int index);

  /** @com.method(vtoffset=66, dispid=52, type=METHOD, name="ProductName")
      @com.parameters([in,type=I4] index, [type=STRING] return) */
  public native String ProductName(int index);

  /** @com.method(vtoffset=67, dispid=53, type=METHOD, name="ModeID")
      @com.parameters([in,type=I4] index, [type=STRING] return) */
  public native String ModeID(int index);

  /** @com.method(vtoffset=68, dispid=54, type=METHOD, name="Features")
      @com.parameters([in,type=I4] index, [type=I4] return) */
  public native int Features(int index);

  /** @com.method(vtoffset=69, dispid=55, type=METHOD, name="Interfaces")
      @com.parameters([in,type=I4] index, [type=I4] return) */
  public native int Interfaces(int index);

  /** @com.method(vtoffset=70, dispid=56, type=METHOD, name="EngineFeatures")
      @com.parameters([in,type=I4] index, [type=I4] return) */
  public native int EngineFeatures(int index);

  /** @com.method(vtoffset=71, dispid=57, type=METHOD, name="LanguageID")
      @com.parameters([in,type=I4] index, [type=I4] return) */
  public native int LanguageID(int index);

  /** @com.method(vtoffset=72, dispid=58, type=METHOD, name="dialect")
      @com.parameters([in,type=I4] index, [type=STRING] return) */
  public native String dialect(int index);

  /** @com.method(vtoffset=73, dispid=59, type=METHOD, name="Sequencing")
      @com.parameters([in,type=I4] index, [type=I4] return) */
  public native int Sequencing(int index);

  /** @com.method(vtoffset=74, dispid=60, type=METHOD, name="MaxWordsVocab")
      @com.parameters([in,type=I4] index, [type=I4] return) */
  public native int MaxWordsVocab(int index);

  /** @com.method(vtoffset=75, dispid=61, type=METHOD, name="MaxWordsState")
      @com.parameters([in,type=I4] index, [type=I4] return) */
  public native int MaxWordsState(int index);

  /** @com.method(vtoffset=76, dispid=62, type=METHOD, name="Grammars")
      @com.parameters([in,type=I4] index, [type=I4] return) */
  public native int Grammars(int index);

  /** @com.method(vtoffset=77, dispid=63, type=METHOD, name="InitAudioSourceDirect")
      @com.parameters([in,type=I4] direct) */
  public native void InitAudioSourceDirect(int direct);

  /** @com.method(vtoffset=78, dispid=64, type=METHOD, name="InitAudioSourceObject")
      @com.parameters([in,type=I4] object) */
  public native void InitAudioSourceObject(int object);

  /** @com.method(vtoffset=79, dispid=65, type=PROPGET, name="FileName", name2="getFileName")
      @com.parameters([type=STRING] return) */
  public native String getFileName();

  /** @com.method(vtoffset=80, dispid=65, type=PROPPUT, name="FileName", name2="putFileName")
      @com.parameters([in,type=STRING] pVal) */
  public native void putFileName(String pVal);

  /** @com.method(vtoffset=81, dispid=66, type=PROPGET, name="FlagsGet", name2="getFlagsGet")
      @com.parameters([in,type=I4] results, [in,type=I4] rank, [type=I4] return) */
  public native int getFlagsGet(int results, int rank);

  /** @com.method(vtoffset=82, dispid=67, type=PROPGET, name="Identify", name2="getIdentify")
      @com.parameters([in,type=I4] results, [type=STRING] return) */
  public native String getIdentify(int results);

  /** @com.method(vtoffset=83, dispid=68, type=METHOD, name="TimeGet")
      @com.parameters([in,type=I4] results, [in,size=1,elementType=I4,type=ARRAY] beginhi, [in,size=1,elementType=I4,type=ARRAY] beginlo, [in,size=1,elementType=I4,type=ARRAY] endhi, [in,size=1,elementType=I4,type=ARRAY] endlo) */
  public native void TimeGet(int results, int[] beginhi, int[] beginlo, int[] endhi, int[] endlo);

  /** @com.method(vtoffset=84, dispid=69, type=METHOD, name="Correction")
      @com.parameters([in,type=I4] results, [in,type=STRING] Phrase, [in,type=I2] confidence) */
  public native void Correction(int results, String Phrase, short confidence);

  /** @com.method(vtoffset=85, dispid=70, type=METHOD, name="Validate")
      @com.parameters([in,type=I4] results, [in,type=I2] confidence) */
  public native void Validate(int results, short confidence);

  /** @com.method(vtoffset=86, dispid=71, type=PROPGET, name="ReEvaluate", name2="getReEvaluate")
      @com.parameters([in,type=I4] results, [type=I4] return) */
  public native int getReEvaluate(int results);

  /** @com.method(vtoffset=87, dispid=72, type=PROPGET, name="GetPhraseScore", name2="getGetPhraseScore")
      @com.parameters([in,type=I4] results, [in,type=I4] rank, [type=I4] return) */
  public native int getGetPhraseScore(int results, int rank);

  /** @com.method(vtoffset=88, dispid=73, type=METHOD, name="Archive")
      @com.parameters([in,type=I4] keepresults, [out,size=1,elementType=I4,type=ARRAY] size, [out,size=1,elementType=I4,type=ARRAY] pVal) */
  public native void Archive(int keepresults, int[] size, int[] pVal);

  /** @com.method(vtoffset=89, dispid=74, type=METHOD, name="DeleteArchive")
      @com.parameters([in,type=I4] Archive) */
  public native void DeleteArchive(int Archive);

  /** @com.method(vtoffset=90, dispid=75, type=METHOD, name="GrammarFromMemory")
      @com.parameters([in,type=I4] grammar, [in,type=I4] size) */
  public native void GrammarFromMemory(int grammar, int size);

  /** @com.method(vtoffset=91, dispid=76, type=METHOD, name="GrammarDataSet")
      @com.parameters([in,type=I4] Data, [in,type=I4] size) */
  public native void GrammarDataSet(int Data, int size);

  /** @com.method(vtoffset=92, dispid=77, type=METHOD, name="GrammarToMemory")
      @com.parameters([in,size=1,elementType=I4,type=ARRAY] grammar, [in,size=1,elementType=I4,type=ARRAY] size) */
  public native void GrammarToMemory(int[] grammar, int[] size);


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0x4e3d9d1e, (short)0xc63, (short)0x11d1, (byte)0x8b, (byte)0xfb, (byte)0x0, (byte)0x60, (byte)0x8, (byte)0x18, (byte)0x41, (byte)0xde);

  public static final com.ms.com._Guid clsid = new com.ms.com._Guid((int)0x4e3d9d1f, (short)0xc63, (short)0x11d1, (byte)0x8b, (byte)0xfb, (byte)0x0, (byte)0x60, (byte)0x8, (byte)0x18, (byte)0x41, (byte)0xde);
}
