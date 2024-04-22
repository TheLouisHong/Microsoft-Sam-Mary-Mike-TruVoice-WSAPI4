VERSION 5.00
Object = "{60462311-3373-11D1-8C43-0060081841DE}#1.0#0"; "XCommand.dll"
Begin VB.Form Form1 
   Caption         =   "Basic Voice Commands"
   ClientHeight    =   3105
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5445
   LinkTopic       =   "Form1"
   ScaleHeight     =   3105
   ScaleWidth      =   5445
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command3 
      Caption         =   "Stop Listening"
      Height          =   375
      Left            =   3840
      TabIndex        =   5
      Top             =   2280
      Width           =   1455
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Remove Command"
      Height          =   375
      Left            =   240
      TabIndex        =   4
      Top             =   2280
      Width           =   1575
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Add Command"
      Height          =   375
      Left            =   2040
      TabIndex        =   3
      Top             =   2280
      Width           =   1575
   End
   Begin VB.TextBox Text2 
      Height          =   375
      Left            =   2040
      TabIndex        =   2
      Text            =   "yellow"
      Top             =   1800
      Width           =   1575
   End
   Begin HSRLibCtl.Vcommand Vcommand1 
      Height          =   615
      Left            =   4320
      OleObjectBlob   =   "basiccmd.frx":0000
      TabIndex        =   1
      Top             =   1320
      Visible         =   0   'False
      Width           =   615
   End
   Begin VB.ListBox List1 
      Height          =   1815
      Left            =   240
      TabIndex        =   0
      Top             =   360
      Width           =   1575
   End
   Begin VB.Label Label3 
      Caption         =   "<nothing>"
      Height          =   255
      Left            =   3360
      TabIndex        =   8
      Top             =   120
      Width           =   1455
   End
   Begin VB.Label Label2 
      Caption         =   "Listening for:"
      Height          =   255
      Left            =   240
      TabIndex        =   7
      Top             =   120
      Width           =   1815
   End
   Begin VB.Label Label1 
      Caption         =   "Last heard:"
      Height          =   255
      Left            =   2160
      TabIndex        =   6
      Top             =   120
      Width           =   975
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim gMymenu As Long

Private Sub UpdateList()
Dim i As Long
Dim command As String
Dim description As String
Dim category As String
Dim flags As Long
Dim action As String

List1.Clear
tcount = Vcommand1.CountCommands(gMymenu)
For i = 1 To tcount
   Vcommand1.GetCommand gMymenu, i, command, description, category, flags, action
   List1.AddItem command
Next i
End Sub

Private Sub Command1_Click()
If (Text2.Text <> "") Then
    Vcommand1.AddCommand gMymenu, 1, Text2.Text, "when you say " + Text2.Text, "listen list", 0, ""
End If
UpdateList

End Sub

Private Sub Command2_Click()
If (List1.ListIndex >= 0) Then
    Vcommand1.Remove gMymenu, List1.ListIndex + 1
End If
UpdateList
End Sub

Private Sub Command3_Click()
If Command3.Caption = "Listen" Then
    Vcommand1.Activate gMymenu
    Command3.Caption = "Stop Listening"
    List1.Enabled = True
Else
    Vcommand1.Deactivate gMymenu
    Command3.Caption = "Listen"
    List1.Enabled = False
End If

End Sub

Private Sub Form_Load()
Vcommand1.Initialized = 1
gMymenu = Vcommand1.MenuCreate(App.EXEName, "state1", 4)
Vcommand1.Enabled = 1
Vcommand1.AddCommand gMymenu, 1, "red", "when you say" + "red", "listen list", 0, ""
Vcommand1.AddCommand gMymenu, 1, "blue", "when you say" + "blue", "listen list", 0, ""
Vcommand1.AddCommand gMymenu, 1, "green", "when you say" + "green", "listen list", 0, ""
UpdateList
Vcommand1.Activate gMymenu
End Sub


Private Sub Form_Unload(Cancel As Integer)
Vcommand1.ReleaseMenu gMymenu
End Sub

Private Sub Vcommand1_CommandRecognize(ByVal ID As Long, ByVal CmdName As String, ByVal flags As Long, ByVal action As String, ByVal NumLists As Long, ByVal ListValues As String, ByVal command As String)
Label3.Caption = command
End Sub
