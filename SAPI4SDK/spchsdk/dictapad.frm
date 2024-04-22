VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.2#0"; "COMCTL32.OCX"
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.1#0"; "RICHTX32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.1#0"; "COMDLG32.OCX"
Object = "{582C2183-4016-11D1-8C55-0060081841DE}#1.0#0"; "VDICT.DLL"
Object = "{EEE78583-FE22-11D0-8BEF-0060081841DE}#1.0#0"; "XVOICE.DLL"
Begin VB.Form Form5 
   Caption         =   "Dictation Pad (Visual Basic Version)"
   ClientHeight    =   8205
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   9660
   LinkTopic       =   "Form1"
   ScaleHeight     =   8205
   ScaleWidth      =   9660
   StartUpPosition =   3  'Windows Default
   WindowState     =   2  'Maximized
   Begin ComctlLib.Toolbar Toolbar1 
      Align           =   1  'Align Top
      Height          =   420
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   9660
      _ExtentX        =   17039
      _ExtentY        =   741
      ButtonWidth     =   609
      ButtonHeight    =   582
      Appearance      =   1
      ImageList       =   "ImageList1"
      _Version        =   327682
      BeginProperty Buttons {0713E452-850A-101B-AFC0-4210102A8DA7} 
         NumButtons      =   15
         BeginProperty Button1 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "listen"
            Object.ToolTipText     =   "Listen"
            Object.Tag             =   ""
            ImageIndex      =   1
         EndProperty
         BeginProperty Button2 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "showhide"
            Object.ToolTipText     =   "Show/Hide the correction window"
            Object.Tag             =   ""
            ImageIndex      =   2
         EndProperty
         BeginProperty Button3 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "capitalizeword"
            Object.ToolTipText     =   "Capitalize/Uncapitalize selected word"
            Object.Tag             =   ""
            ImageIndex      =   3
         EndProperty
         BeginProperty Button4 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "addword"
            Object.ToolTipText     =   "Add a new word"
            Object.Tag             =   ""
            ImageIndex      =   4
         EndProperty
         BeginProperty Button5 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   ""
            Object.Tag             =   ""
            Style           =   3
            MixedState      =   -1  'True
         EndProperty
         BeginProperty Button6 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "new"
            Object.ToolTipText     =   "Create new dictation file"
            Object.Tag             =   ""
            ImageIndex      =   5
         EndProperty
         BeginProperty Button7 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "open"
            Object.ToolTipText     =   "Open a dictation file"
            Object.Tag             =   ""
            ImageIndex      =   6
         EndProperty
         BeginProperty Button8 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "save"
            Object.ToolTipText     =   "Save dictation file..."
            Object.Tag             =   ""
            ImageIndex      =   7
         EndProperty
         BeginProperty Button9 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   ""
            Object.Tag             =   ""
            Style           =   3
            MixedState      =   -1  'True
         EndProperty
         BeginProperty Button10 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "cut"
            Object.ToolTipText     =   "cut"
            Object.Tag             =   ""
            ImageIndex      =   8
         EndProperty
         BeginProperty Button11 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "copy"
            Object.ToolTipText     =   "copy"
            Object.Tag             =   ""
            ImageIndex      =   9
         EndProperty
         BeginProperty Button12 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "paste"
            Object.ToolTipText     =   "paste"
            Object.Tag             =   ""
            ImageIndex      =   10
         EndProperty
         BeginProperty Button13 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "cutall"
            Object.ToolTipText     =   "Cut all text"
            Object.Tag             =   ""
            ImageIndex      =   11
         EndProperty
         BeginProperty Button14 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   ""
            Object.Tag             =   ""
            Style           =   3
            MixedState      =   -1  'True
         EndProperty
         BeginProperty Button15 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "read"
            Object.ToolTipText     =   "Read from Insertion Point"
            Object.Tag             =   ""
            ImageIndex      =   14
         EndProperty
      EndProperty
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   1440
      Top             =   5400
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   327680
   End
   Begin RichTextLib.RichTextBox RichTextBox1 
      Height          =   3420
      Left            =   15
      TabIndex        =   0
      Top             =   1500
      Width           =   7140
      _ExtentX        =   12594
      _ExtentY        =   6033
      _Version        =   327680
      Enabled         =   -1  'True
      ScrollBars      =   3
      RightMargin     =   1
      TextRTF         =   $"dictapad.frx":0000
   End
   Begin ACTIVEVOICEPROJECTLibCtl.DirectSS DirectSS1 
      Height          =   615
      Left            =   2160
      OleObjectBlob   =   "dictapad.frx":00D5
      TabIndex        =   3
      Top             =   5400
      Visible         =   0   'False
      Width           =   855
   End
   Begin DICTLibCtl.Vdict Vdict1 
      Height          =   375
      Left            =   645
      OleObjectBlob   =   "dictapad.frx":012D
      TabIndex        =   2
      Top             =   6210
      Visible         =   0   'False
      Width           =   435
   End
   Begin ComctlLib.ImageList ImageList1 
      Left            =   600
      Top             =   5280
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   327682
      BeginProperty Images {0713E8C2-850A-101B-AFC0-4210102A8DA7} 
         NumListImages   =   14
         BeginProperty ListImage1 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":0151
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":04A3
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":07F5
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":0B47
            Key             =   ""
         EndProperty
         BeginProperty ListImage5 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":0E99
            Key             =   ""
         EndProperty
         BeginProperty ListImage6 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":13DB
            Key             =   ""
         EndProperty
         BeginProperty ListImage7 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":14ED
            Key             =   ""
         EndProperty
         BeginProperty ListImage8 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":15FF
            Key             =   ""
         EndProperty
         BeginProperty ListImage9 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":1711
            Key             =   ""
         EndProperty
         BeginProperty ListImage10 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":1823
            Key             =   ""
         EndProperty
         BeginProperty ListImage11 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":1935
            Key             =   ""
         EndProperty
         BeginProperty ListImage12 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":1C87
            Key             =   ""
         EndProperty
         BeginProperty ListImage13 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":1FD9
            Key             =   ""
         EndProperty
         BeginProperty ListImage14 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "dictapad.frx":20E3
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin VB.Menu File 
      Caption         =   "&File"
      Begin VB.Menu New 
         Caption         =   "&New"
         Shortcut        =   ^N
      End
      Begin VB.Menu open 
         Caption         =   "&Open..."
         Shortcut        =   ^O
      End
      Begin VB.Menu save 
         Caption         =   "&Save"
         Shortcut        =   ^S
      End
      Begin VB.Menu saveas 
         Caption         =   "Save &As..."
      End
      Begin VB.Menu qw 
         Caption         =   "-"
      End
      Begin VB.Menu Exit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu Edit 
      Caption         =   "&Edit"
      Begin VB.Menu Cut 
         Caption         =   "Cu&t"
      End
      Begin VB.Menu Copy 
         Caption         =   "&Copy"
      End
      Begin VB.Menu Paste 
         Caption         =   "&Paste"
      End
      Begin VB.Menu Delete 
         Caption         =   "De&lete"
      End
      Begin VB.Menu qwe 
         Caption         =   "-"
      End
      Begin VB.Menu SelectAll 
         Caption         =   "Select &All"
         Shortcut        =   ^A
      End
   End
   Begin VB.Menu Dictation 
      Caption         =   "&Dictation"
      Begin VB.Menu listening 
         Caption         =   "&Listening for dictation"
      End
      Begin VB.Menu notlistening 
         Caption         =   "&Not listening for dictation"
      End
      Begin VB.Menu sdf 
         Caption         =   "-"
      End
      Begin VB.Menu showcorrection 
         Caption         =   "Show &Correction Window"
      End
      Begin VB.Menu capitalizeword 
         Caption         =   "&Capitalize selected word"
      End
      Begin VB.Menu Addword 
         Caption         =   "A&dd Word..."
      End
      Begin VB.Menu fsa 
         Caption         =   "-"
      End
      Begin VB.Menu Alwaysselect 
         Caption         =   "&Always select entire word"
      End
      Begin VB.Menu Options 
         Caption         =   "Dictation Options"
      End
   End
   Begin VB.Menu showmenu 
      Caption         =   "&Show"
      Begin VB.Menu alwaysontop 
         Caption         =   "&Always on top"
      End
      Begin VB.Menu setfont 
         Caption         =   "Set &Font"
      End
   End
   Begin VB.Menu Read 
      Caption         =   "&Read"
      Begin VB.Menu ReadDocument 
         Caption         =   "&Read from Insertion Point"
      End
      Begin VB.Menu sssss 
         Caption         =   "-"
      End
      Begin VB.Menu Stopreading 
         Caption         =   "&Stop Reading"
      End
   End
End
Attribute VB_Name = "Form5"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Rem This contains the source code for a visual basic dictation pad.

Dim gThisFile As String
Dim gReadStart As Integer

Private Sub DirectSS1_AudioStop(ByVal hi As Long, ByVal lo As Long)
      Toolbar1.Buttons(15).Value = tbrUnpressed
      Toolbar1.Buttons(15).Image = 14
End Sub

Private Sub DirectSS1_WordPosition(ByVal hi As Long, ByVal low As Long, ByVal byteoffset As Long)
Dim firstspace As Integer

'VB strings are unicode, so we divide the byteoffset by 2 to get the character offset
RichTextBox1.selstart = byteoffset / 2 + gReadStart
firstspace = FindTextBreak(1, RichTextBox1.selstart, " " + vbNewLine + vbLf + vbCr + vbCrLf)
RichTextBox1.SelLength = firstspace - RichTextBox1.selstart

On Error GoTo done
Vdict1.Lock
Vdict1.TextSelSet RichTextBox1.selstart, RichTextBox1.SelLength
Vdict1.Unlock
done:
End Sub

Public Sub Form_Load()
Dim command As String
Dim Description As String
Dim category As String
Dim action As String
Dim FLAGS As Long

On Error GoTo ErrorMessage

If (Vdict1.Initialized = 0) Then
    Vdict1.Initialized = 1
    Vdict1.Mode = 32
End If

listen (0)

gThisFile = ""
ResetText
Alwaysselect.Checked = True
Toolbar1.ButtonHeight = ImageList1.ImageHeight
Toolbar1.ButtonWidth = ImageList1.ImageHeight
Toolbar1.Height = Toolbar1.ButtonHeight
RichTextBox1.Top = Toolbar1.Top + Toolbar1.Height

CommonDialog1.Filter = "Dictation (*.msd)|*.msd|Rich Text (*.rtf)|*.rtf|Text (*.txt)|*.txt"
GoTo NoError
ErrorMessage:
MsgBox "Unable to initialize dictation engine.  Make sure an engine that supports dictation is installed."
End
NoError:
End Sub



Private Sub Addword_Click()
On Error GoTo addend
Vdict1.LexiconDlg Form5.hwnd, "Add Word"
addend:
End Sub

Private Sub alwaysontop_Click()
alwaysontop.Checked = Not (alwaysontop.Checked)
If (alwaysontop.Checked) Then
    FloatPad (True)
Else
    FloatPad (False)
End If
End Sub

Private Sub ResetText()
RichTextBox1.Font.Name = "Times New Roman"
RichTextBox1.Font.Size = 14
SelectAll_Click
Delete_Click

RichTextBox1.Font.Name = "Times New Roman"
RichTextBox1.Font.Size = 14

End Sub

Private Sub Alwaysselect_Click()
Alwaysselect.Checked = Not (Alwaysselect.Checked)
End Sub

Private Sub capitalizeword_Click()
s = RichTextBox1.selstart
e = RichTextBox1.SelLength
Vdict1.Lock
Vdict1.FX (3)   'toggle word case
Vdict1.Unlock
RichTextBox1.selstart = s
RichTextBox1.SelLength = e

End Sub

Private Sub listening_Click()
listen (1)
End Sub

Private Sub notlistening_Click()
listen (0)
End Sub


Private Sub ReadDocument_Click()
Dim ReadText As String

On Error Resume Next  'if TTS cannot read because some other thing is making sound, dont except

If (DirectSS1.Speaking) Then
    Stopreading_Click
Else
    listen (0)
    Toolbar1.Buttons(15).Value = tbrPressed
    Toolbar1.Buttons(15).Image = 13
    gReadStart = RichTextBox1.selstart
    ReadText = Right$(RichTextBox1.Text, Len(RichTextBox1.Text) - RichTextBox1.selstart)
    DirectSS1.Speak ReadText
End If
End Sub

Private Function isTextBreak(c As String, breakchars As String)
For i = 1 To Len(breakchars)
    If c = Mid$(breakchars, i, 1) Then
        isTextBreak = True
        GoTo done
    End If
Next i
isTextBreak = False
done:
End Function

Rem This is used instead of the rich text box scan function to reduce screen flicker
Private Function FindTextBreak(direction As Integer, start As Integer, breakchars As String)
If (RichTextBox1.Text = "") Then
    FindTextBreak = 1
ElseIf (direction = 0) Then
    For i = start To 1 Step -1
        If (isTextBreak(Mid$(RichTextBox1.Text, i, 1), breakchars)) Then
            FindTextBreak = i
            GoTo done
        End If
        
    Next i
    FindTextBreak = i
    GoTo done
Else
    For i = start To Len(RichTextBox1.Text)
        If (isTextBreak(Mid$(RichTextBox1.Text, i + 1, 1), breakchars)) Then
            FindTextBreak = i
            GoTo done
        End If
    Next i
    FindTextBreak = i
End If
done:
End Function

Private Sub RichTextBox1_Click()
Dim breakstring As String

If (Alwaysselect.Checked) Then
    If RichTextBox1.selstart = Len(RichTextBox1.Text) Then GoTo setsel
        
    breakstring = " -_!@#$%^&*(),.[]+=`'""" + vbNewLine + vbCrLf + vbCr + vbLf
    sStart = FindTextBreak(0, RichTextBox1.selstart, breakstring)
    If (sStart <> 0) Then
        If (sStart = RichTextBox1.selstart And (Not isTextBreak(Mid$(RichTextBox1.Text, (sStart), 1), breakstring))) Then
            sStart = sStart - 1
            sEnd = sStart + 1
        Else
            GoTo findit
        End If
        
    Else
findit:
        sEnd = FindTextBreak(1, RichTextBox1.selstart + RichTextBox1.SelLength, breakstring)
    End If
    RichTextBox1.selstart = sStart
    RichTextBox1.SelLength = sEnd - sStart
setsel:
    On Error GoTo done
    Vdict1.Lock
    Vdict1.TextSelSet RichTextBox1.selstart, RichTextBox1.SelLength
    Vdict1.Unlock
done:
End If
End Sub

Private Sub SetText(newText As String, ui As Boolean)
Vdict1.Lock
Vdict1.TextSelSet RichTextBox1.selstart, 0
Vdict1.TextSet newText, RichTextBox1.selstart, RichTextBox1.SelLength, 65536
Vdict1.Unlock
If (ui) Then
    RichTextBox1.SelText = newText
End If
End Sub

Private Sub RichTextBox1_KeyPress(KeyAscii As Integer)
Dim s As String
s = Chr$(KeyAscii)
SetText s, False
ShowCorrectionWindow (showcorrection.Checked)
End Sub

Private Sub RichTextBox1_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
Vdict1.Lock
Vdict1.TextSelSet RichTextBox1.selstart, RichTextBox1.SelLength
Vdict1.Unlock
ShowCorrectionWindow (showcorrection.Checked)
End Sub

Private Sub setfont_Click()
' Set Cancel to True
 CommonDialog1.CancelError = True
On Error GoTo ErrHandler
 ' Set the Flags property
CommonDialog1.FLAGS = cdlCFEffects Or cdlCFBoth
 ' Display the Font dialog box
CommonDialog1.ShowFont
RichTextBox1.Font.Name = CommonDialog1.FontName
RichTextBox1.Font.Size = CommonDialog1.FontSize
RichTextBox1.Font.Italic = CommonDialog1.FontItalic
RichTextBox1.Font.Bold = CommonDialog1.FontBold
RichTextBox1.Font.Underline = CommonDialog1.FontUnderline
RichTextBox1.Font.StrikeThru = CommonDialog1.FontStrikethru

ErrHandler:

End Sub

Private Sub showcorrection_Click()
If (showcorrection.Checked) Then
    ShowCorrectionWindow (0)
Else
    ShowCorrectionWindow (1)
End If
End Sub


Private Sub Stopreading_Click()
DirectSS1.AudioReset
End Sub

Private Sub Vdict1_AttribChanged(ByVal Attrib As Long)

If Attrib And 2 = 2 Then
    If (Vdict1.Mode And 32) Then
        ListenUI (1)
    Else
        ListenUI (0)
    End If

End If
End Sub

' This is the core of the program.  TextChanged is called whenever the text
' is changed by the dictation object.  Use GetChanges to get the differences
' and update the rich text control.
Private Sub Vdict1_TextChanged(ByVal reason As Long)
Dim newStart As Long
Dim newend As Long
Dim oldStart As Long
Dim oldEnd As Long
Dim selstart As Long
Dim sellen As Long
Dim theText As String

Vdict1.Lock

On Error GoTo spuriouserror
Vdict1.GetChanges newStart, newend, oldStart, oldEnd

If (oldStart < oldEnd) Then
    RichTextBox1.selstart = oldStart
    RichTextBox1.SelLength = oldEnd - oldStart
    RichTextBox1.SelText = ""
End If

If (newend > newStart) Then

    RichTextBox1.selstart = newStart
    RichTextBox1.SelLength = 0

    Vdict1.TextGet newStart, newend - newStart, theText
    RichTextBox1.SelText = theText
End If
    ShowCorrectionWindow 2
    
spuriouserror:
Vdict1.Unlock
End Sub


Private Sub Vdict1_TextSelChanged()
Dim selstart As Long
Dim sellen As Long

Vdict1.TextSelGet selstart, sellen
RichTextBox1.selstart = selstart
RichTextBox1.SelLength = sellen
End Sub

Private Sub Bold_Click()
RichTextBox1.SelBold = True
End Sub

Private Sub Copy_Click()
Clipboard.SetText RichTextBox1.SelText
End Sub

Private Sub Cut_Click()
Clipboard.SetText RichTextBox1.SelText
SetText "", True
End Sub

Private Sub Delete_Click()
SetText "", True
End Sub

Private Sub Exit_Click()
Form_Unload (1)
End
End Sub


Public Sub ShowCorrectionWindow(Show)
Static caret As POINTAPI
If (Show) Then
 If (Show = 2) Then
    If (showcorrection.Checked) Then
        sflags = 1
    Else
        sflags = 0
    End If
 Else
    showcorrection.Checked = True
    sflags = 1
 End If
  
Else
 showcorrection.Checked = False
 sflags = 0
End If

Vdict1.FLAGS = (Vdict1.FLAGS And (Not 1)) Or sflags
 
GetCaretPos caret
caret.x = caret.x + Form5.Left / Screen.TwipsPerPixelX
caret.y = caret.y + Form5.Top / Screen.TwipsPerPixelY + RichTextBox1.Top / Screen.TwipsPerPixelY + 60
 
Vdict1.SetSelRect caret.x, caret.y, caret.x, caret.y

End Sub


Private Sub FloatPad(ontop)
If (ontop) Then
    tp = SetWindowPos(Form5.hwnd, HWND_TOPMOST, 0, 0, 0, 0, FLAGS)
Else
    tp = SetWindowPos(Form5.hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, FLAGS)
End If
End Sub

Private Function Max(a, b)
If (a > b) Then
    Max = a
Else
    Max = b
End If
End Function

Private Sub Form_Resize()
    RichTextBox1.Width = Max(Form5.Width - 109, 0)
    RichTextBox1.Height = Max(Form5.Height - 1158, 0)
End Sub

Public Sub Form_Unload(Cancel As Integer)
listen (0)
ShowCorrectionWindow (0)
End
End Sub


Private Sub Italic_Click()
RichTextBox1.SelItalic = True
End Sub

Private Sub New_Click()
If MsgBox("This will erase the current text. Are you sure?", vbOKCancel, "New") = 1 Then
    ResetText
    gThisFile = ""
End If
End Sub


Private Sub open_Click()
Dim versionstring As String
Dim fn As String
Dim fs As Long
Dim fi As Long
Dim fb As Long
Dim fu As Long
Dim fst As Long

CommonDialog1.ShowOpen
If (CommonDialog1.filename <> "") Then
    New_Click
    gThisFile = CommonDialog1.filename
    
    If (Right(gThisFile, 3) = "msd") Then
        
        hand = Vdict1.CreateDocFile(gThisFile, 18)
        stream2 = Vdict1.CreateStream(hand, "Version", 18)
        Vdict1.StreamRead stream2, versionstring, 36
        
        If versionstring <> "46FC730A-D849-11d0-AB8A-08002BE4E3B7" Then
            dummy = MsgBox("This program cannot open the file. Invalid format", vbOK, "Invalid Format")
        Else
            stream = Vdict1.CreateStream(hand, "Header", 18)
            Vdict1.ReadStreamFont stream, fn, fs, fi, fb, fu, fst
            Vdict1.ReleaseStream stream
            Vdict1.SessionDeserialize hand
            RichTextBox1.Font.Name = fn
            If (fs < 0) Then
                fs = -fs
            End If
            RichTextBox1.Font.Size = fs
            RichTextBox1.Font.Italic = fi
            RichTextBox1.Font.Bold = fb
            RichTextBox1.Font.Underline = fu
            Rem RichTextBox1.Font.StrikeThru = fst
        End If
        Vdict1.ReleaseStream stream2
        Vdict1.ReleaseStore hand
    Else
        RichTextBox1.filename = CommonDialog1.filename
        gThisFile = RichTextBox1.filename
        SetText RichTextBox1.Text, False
    End If
End If

End Sub

Private Sub Options_Click()
Vdict1.GeneralDlg Form5.hwnd, "Dictation Options"

End Sub

Private Sub Paste_Click()
SetText Clipboard.GetText, True
End Sub

Private Sub DoSave()
Dim hand As Long
Dim fs As Long
Dim fi As Long
Dim fb As Long
Dim fu As Long
Dim fst As Long

If (Right(gThisFile, 3) = "msd") Then
    
    fs = -RichTextBox1.Font.Size
    fi = RichTextBox1.Font.Italic
    fb = RichTextBox1.Font.Bold
    fu = RichTextBox1.Font.Underline
    fst = False
    
    hand = Vdict1.CreateDocFile(gThisFile, 18)
    stream = Vdict1.CreateStream(hand, "Header", 18)
    Vdict1.SetSize stream, 0
    Vdict1.WriteStreamFont stream, RichTextBox1.Font.Name, fs, fi, fb, fu, fst
    Vdict1.SessionSerialize hand
    stream2 = Vdict1.CreateStream(hand, "Version", 18)
    Vdict1.SetSize stream2, 0
    Vdict1.StreamWrite stream2, "46FC730A-D849-11d0-AB8A-08002BE4E3B7"
    Vdict1.ReleaseStream stream
    Vdict1.ReleaseStream stream2
    Vdict1.ReleaseStore hand
ElseIf (Right(gThisFile, 3) = "rtf") Then
    Open gThisFile For Output As 1
    Print #1, RichTextBox1.TextRTF
    Close 1
Else
    Open gThisFile For Output As 1
    Print #1, RichTextBox1.Text
    Close 1
End If

End Sub
Private Sub save_Click()
If (gThisFile <> "") Then
    DoSave
Else
    saveas_Click
End If
End Sub

Private Sub saveas_Click()

CommonDialog1.ShowSave
If (CommonDialog1.filename <> "") Then
    gThisFile = CommonDialog1.filename
    DoSave
End If
End Sub

Public Sub ListenUI(op As Integer)
If (op = 1) Then

      Toolbar1.Buttons(1).Value = tbrPressed
      Toolbar1.Buttons(1).Image = 1
      listening.Checked = True
      notlistening.Checked = False
Else
      Toolbar1.Buttons(1).Value = tbrUnpressed
      Toolbar1.Buttons(1).Image = 12
      listening.Checked = False
      notlistening.Checked = True
End If
End Sub

Public Sub listen(op As Integer)

If (op = 1) Then
            Stopreading_Click
            Vdict1.Mode = 32
On Error GoTo NoActivate
            Vdict1.Activate
NoActivate:
    Else
           Vdict1.Mode = 2
On Error GoTo NoDeactivate
            Vdict1.Deactivate
NoDeactivate:
      
    End If
    ListenUI (op)
GoTo NoError
ErrorMessage:
MsgBox "Unable to initialize dictation engine.  Make sure an engine that supports dictation is installed."
End
NoError:
End Sub



Private Sub toolbar1_ButtonClick(ByVal Button As Button)
    ' Use the Key property with the SelectCase statement to specify
    ' an action.
    Select Case Button.Key
    Case Is = "cut"
        Cut_Click
        
    Case Is = "cutall"
        SelectAll_Click
        Cut_Click
        
    Case Is = "copy"
        Copy_Click
        
    Case Is = "paste"
        Paste_Click
        
    Case Is = "new"
        New_Click
        
    Case Is = "save"
        save_Click
        
    Case Is = "open"
        open_Click
        
    Case Is = "listen"
        If (listening.Checked) Then
            listen (0)
        Else
            listen (1)
        End If
        
    Case Is = "showhide"
        showcorrection_Click
        
    Case Is = "capitalizeword"
        capitalizeword_Click
        
    Case Is = "addword"
        Addword_Click
                
    Case Is = "read"
        ReadDocument_Click
    End Select
End Sub



Private Sub SelectAll_Click()
RichTextBox1.selstart = 0
RichTextBox1.SelLength = Len(RichTextBox1.Text)
End Sub
