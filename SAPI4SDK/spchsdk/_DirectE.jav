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

// Dispatch-only interface _DirectSREvents
/** @com.interface(iid=4E3D9D20-0C63-11D1-8BFB-0060081841DE, thread=AUTO, type=DISPATCH) */
public interface _DirectSREvents extends IUnknown
{
  /** @com.method(dispid=1, type=METHOD, name="ClickIn", returntype=VOID)
      @com.parameters([in,type=I4] x, [in,type=I4] y) */
  public void ClickIn(int x, int y);

  /** @com.method(dispid=2, type=METHOD, name="ClickOut", returntype=VOID)
      @com.parameters([in,type=I4] x, [in,type=I4] y) */
  public void ClickOut(int x, int y);

  /** @com.method(dispid=3, type=METHOD, name="PhraseFinish")
      @com.parameters([in,type=I4] flags, [in,type=I4] beginhi, [in,type=I4] beginlo, [in,type=I4] endhi, [in,type=I4] endlo, [in,type=STRING] Phrase, [in,type=STRING] parsed, [in,type=I4] results) */
  public void PhraseFinish(int flags, int beginhi, int beginlo, int endhi, int endlo, String Phrase, String parsed, int results);

  /** @com.method(dispid=4, type=METHOD, name="PhraseStart")
      @com.parameters([in,type=I4] hi, [in,type=I4] lo) */
  public void PhraseStart(int hi, int lo);

  /** @com.method(dispid=5, type=METHOD, name="BookMark")
      @com.parameters([in,type=I4] MarkID) */
  public void BookMark(int MarkID);

  /** @com.method(dispid=6, type=METHOD, name="Paused")
      @com.parameters() */
  public void Paused();

  /** @com.method(dispid=7, type=METHOD, name="PhraseHypothesis")
      @com.parameters([in,type=I4] flags, [in,type=I4] beginhi, [in,type=I4] beginlo, [in,type=I4] endhi, [in,type=I4] endlo, [in,type=STRING] Phrase, [in,type=I4] results) */
  public void PhraseHypothesis(int flags, int beginhi, int beginlo, int endhi, int endlo, String Phrase, int results);

  /** @com.method(dispid=8, type=METHOD, name="ReEvaluate")
      @com.parameters([in,type=I4] Result) */
  public void ReEvaluate(int Result);

  /** @com.method(dispid=9, type=METHOD, name="Training")
      @com.parameters([in,type=I4] train) */
  public void Training(int train);

  /** @com.method(dispid=10, type=METHOD, name="UnArchive")
      @com.parameters([in,type=I4] Result) */
  public void UnArchive(int Result);


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0x4e3d9d20, (short)0xc63, (short)0x11d1, (byte)0x8b, (byte)0xfb, (byte)0x0, (byte)0x60, (byte)0x8, (byte)0x18, (byte)0x41, (byte)0xde);
}
