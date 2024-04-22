VERSION 5.00
Object = "{2398E321-5C6E-11D1-8C65-0060081841DE}#1.0#0"; "htts.dll"
Begin VB.Form Form1 
   BackColor       =   &H00FFFFFF&
   Caption         =   "Basic Text-To-Speech"
   ClientHeight    =   8190
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9975
   LinkTopic       =   "Form1"
   ScaleHeight     =   8190
   ScaleWidth      =   9975
   StartUpPosition =   3  'Windows Default
   Begin HTTSLibCtl.TextToSpeech TextToSpeech1 
      Height          =   6615
      Left            =   120
      OleObjectBlob   =   "basictts.frx":0000
      TabIndex        =   3
      Top             =   1440
      Width           =   9615
   End
   Begin VB.ComboBox Combo1 
      Height          =   315
      Left            =   4920
      Style           =   2  'Dropdown List
      TabIndex        =   2
      Top             =   120
      Width           =   4815
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Say It!"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   480
      Width           =   4695
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Left            =   120
      TabIndex        =   0
      Text            =   "Please feed me pizza."
      Top             =   120
      Width           =   4695
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Rem This is a simple sample app that demonstrates how to pick a synthetic
Rem voice engine and speak some text.


Private Sub Form_Load()
Dim ModeName As String

    Rem First choose a default synthesizer. This is not strictly necessary as
    Rem the ActiveVoice object will pick a reasonable default
    Rem but we do this here for completeness.
    Rem
    Rem Here we set manufacturer="Microsoft" as the most
    Rem important quality (rank) and gender as female(0=male,1=female) as the second
    Rem ranked quality.  The closest match will be selected.
    Rem
    Rem This method for choosing engines is superior to examining the actual values
    Rem for each mode and picking the engine yourself because Find does a good
    Rem job of ranking the quality of the other parameters even though you were not
    Rem specific about those inputs (ie you set the rank=0).  For example, although
    Rem we only specify a female synthesizer made by Microsoft, Find will choose
    Rem the 'Female' driver as opposed to 'Female (for telephone)' because the 'Female'
    Rem engine sounds better.
    Rem
    Rem Note that this sample uses the "Microsoft Voice Text" control, which shares resources
    Rem with other apps.  That means changes made in this app will effect other apps.
    Rem You can replace the control with the "Microsoft Direct Speech Synthesis" control
    Rem which will not share resources, or have global effects.  The other control also has
    Rem a few more sophisticated interfaces for a finer control.
    
    
    engine = TextToSpeech1.Find("Mfg=Microsoft;Gender=1")
    Rem Now Select the engine, SAPI style. This is synonymous with
    Rem doing TextToSpeech1.CurrentMode = engine
    TextToSpeech1.Select engine
   
    
    Rem Add the name of each SAPI compliant synthesized voice engine/mode
    Rem registered on the machine to the combo box.
    For i = 1 To TextToSpeech1.CountEngines
        ModeName = TextToSpeech1.ModeName(i)
        Combo1.AddItem ModeName
    Next i
    Rem Set the combo box to the current voice setting. Subtract one because
    Rem The voice control is 1 based, but the Combo list zero based
    Rem Note That setting this variable will trigger a Combo1_Click() call,
    Rem so the lip type will be setup there.
    Combo1.ListIndex = TextToSpeech1.CurrentMode - 1

End Sub


Private Sub Combo1_Click()
Rem Each time somebody selects a new voice/engine/mode from the combo box,
Rem select that voice as the active speaker.
    TextToSpeech1.CurrentMode = Combo1.ListIndex + 1
Rem Set the gender of the lips..Gender=1 means female.
    If (TextToSpeech1.Gender(TextToSpeech1.CurrentMode) = 1) Then
        TextToSpeech1.LipType = 0    'female full red lips
    Else
        TextToSpeech1.LipType = 1    'male thinner paler lips
    End If
End Sub

Private Sub Command1_Click()
Rem speak the text in the text box when the button is pressed
TextToSpeech1.Speak Text1.Text
End Sub



