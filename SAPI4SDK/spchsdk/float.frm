VERSION 5.00
Begin VB.Form Form2 
   ClientHeight    =   2820
   ClientLeft      =   60
   ClientTop       =   60
   ClientWidth     =   1380
   ControlBox      =   0   'False
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2820
   ScaleWidth      =   1380
   StartUpPosition =   3  'Windows Default
   Begin VB.ListBox List2 
      Height          =   1815
      Left            =   2400
      TabIndex        =   3
      Top             =   0
      Width           =   135
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Play"
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   2400
      Width           =   855
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   1920
      Width           =   1095
   End
   Begin VB.ListBox List1 
      Height          =   1815
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   1815
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()
On Error GoTo failed
Form5.Listen (0)
gWave = Form5.ActiveListen1.Wave(resultslist(gCurWord))
sndPlaySound gWave, SND_MEMORY
failed:
End Sub


Private Sub List1_Click()
    DoClick (List1.ListIndex)
End Sub

Private Sub Text1_KeyPress(KeyAscii As Integer)
If (KeyAscii = 13) Then
    tt = Form5.RichTextBox1.SelLength
    Form5.RichTextBox1.SelStart = Form5.RichTextBox1.SelStart + 1
    If (tt) Then
        Form5.RichTextBox1.SelLength = tt - 1
        Form5.RichTextBox1.SelText = Text1.Text
        
    End If
End If
End Sub
