VERSION 5.00
Object = "{FC9E7401-6058-11D1-8C66-0060081841DE}#1.0#0"; "XTel.dll"
Begin VB.Form Form1 
   Caption         =   "Anwering Machine"
   ClientHeight    =   1065
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   3375
   LinkTopic       =   "Form1"
   ScaleHeight     =   1065
   ScaleWidth      =   3375
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command2 
      Caption         =   "Use Voice Modem"
      Height          =   375
      Left            =   1320
      TabIndex        =   2
      Top             =   600
      Width           =   1935
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Simulate Call"
      Height          =   375
      Left            =   1320
      TabIndex        =   1
      Top             =   120
      Width           =   1935
   End
   Begin TELLibCtl.phone phone1 
      Height          =   735
      Left            =   120
      OleObjectBlob   =   "answer.frx":0000
      TabIndex        =   0
      Top             =   120
      Width           =   975
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'This is a very simple answering machine.
'It just takes a message and writes it to a wave file in a subdirectory of the application directory.
'You can just double click on the messages and they will be played by the system sound player.

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" _
         (hpvDest As Any, hpvSource As Any, ByVal cbCopy As Long)
         

Private Sub ViewDirectory()
'This function simply causes the Explorer to display the Messages directory (and creates it if needed),
'so each time a new message is recorded, an icon appears that can be double clicked on.
'First we try command as if we are in windows 9x, and if that doesn't work, try NT's cmd function.
On Error Resume Next
Dim slash As String
If Right(App.Path, 1) = "\" Then
    slash = ""
Else
    slash = "\"
End If
    mdir = App.Path + slash + "Messages"
    MkDir mdir
    ChDrive mdir
    ChDir mdir
    Shell "command.com /c start .", vbMinimizedNoFocus
    Shell "cmd /c start .", vbMinimizedNoFocus
End Sub

Private Sub Command1_Click()

    phone1.Initialized = 1 'run on emulator
    phone1.CallDialog
End

End Sub

Private Sub Command2_Click()

   phone1.Initialized = 2 'run on real phone
   phone1.CallDialog
End

End Sub

Private Sub Form_Load()
    ViewDirectory
End Sub

Private Sub phone1_DoPhoneCall(ByVal lineID As Long)
'This function gets called each time the phone is answered, for each phone line.
'Because Telephony supports multiple phone lines, this function may be called simultaneously
'several times, each version (identified by lineID) running on a different thread.
Dim result As Long
Dim size As Long
Dim wave As Long
Dim wavefile() As Byte
Dim filename As String

On Error Resume Next
    size = 0

    'this line causes a recorded wave file to be used in place of the text wherever it is spoken.
    '(if you comment out this line, the text-to-speech engine would be used instead)
    'This feature exists so you can prototype your program with text-to-speech, then use
    'the wave list editor to do recordings of the string.  See the docs for more details.
    phone1.WaveAddFromListString lineID, "[FromFile]" + vbNewLine + App.Path + _
        "\hianswer.wav=Hi. We can't come to the phone right now. Please leave a message at the beep."

    'this is the heart of the program.  The prompts are spoken (or played if the waveadd worked),
    'and the callers message is stored in wave.  See the docs for definitions of the settings used.
    phone1.RecordFromString lineID, "[Prompts]" + vbNewLine + _
                                "Main=Hi. We can't come to the phone right now. Please leave a message at the beep." + vbNewLine + _
                                "[Settings]" + vbNewLine + _
                                "BetweenUtt=10000" + vbNewLine + _
                                "InitialBuf=30000" + vbNewLine + _
                                "ReallocBuf=30000" + vbNewLine + _
                                "MaxBuf=300000" + vbNewLine + _
                                "NoAnswerTime=10" + vbNewLine _
                                , result, wave, size
                                
    If (size <> 0) Then
        'The following "casts" the wave to an array (by making a copy of it) so we can save it out to disk.
        ReDim wavefile(size)
        CopyMemory wavefile(0), ByVal wave, size
    
        'free the wave as soon as possible so we dont pig up memory
        phone1.FreeWave wave
    
        'use the date and time as the filename, and put into the Messages directory
        filename = App.Path + "\Messages" + "\Message left at " + Format(Now, "hh mm ss AMPM") + " " + Format(Now, " mmm d yyyy") + ".wav"
    
        'write the wave data out to disk. you can double click on the file to play it with sound recorder.
        Open filename For Binary Access Write As #1
        Put #1, , wavefile
        Close #1
    End If
    'hang up and wait for next call
End Sub
