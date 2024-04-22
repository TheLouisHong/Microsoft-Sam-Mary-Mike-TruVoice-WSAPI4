VERSION 5.00
Object = "{4E3D9D11-0C63-11D1-8BFB-0060081841DE}#1.0#0"; "ActiveListenProject.dll"
Object = "{EEE78583-FE22-11D0-8BEF-0060081841DE}#1.0#0"; "ActiveVoiceProject.dll"
Begin VB.Form Form1 
   BackColor       =   &H00FFFFFF&
   Caption         =   "Virtual Burger"
   ClientHeight    =   8430
   ClientLeft      =   135
   ClientTop       =   360
   ClientWidth     =   11955
   Icon            =   "burger.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   8430
   ScaleWidth      =   11955
   StartUpPosition =   1  'CenterOwner
   Begin ACTIVEVOICEPROJECTLibCtl.DirectSS DirectSS1 
      Height          =   495
      Left            =   120
      OleObjectBlob   =   "burger.frx":030A
      TabIndex        =   3
      Top             =   960
      Visible         =   0   'False
      Width           =   615
   End
   Begin ACTIVELISTENPROJECTLibCtl.DirectSR DirectSR1 
      Height          =   615
      Left            =   930
      OleObjectBlob   =   "burger.frx":0362
      TabIndex        =   2
      Top             =   945
      Visible         =   0   'False
      Width           =   615
   End
   Begin VB.Timer Timer1 
      Interval        =   25
      Left            =   120
      Top             =   1680
   End
   Begin VB.ListBox List1 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1980
      Left            =   5400
      TabIndex        =   0
      Top             =   2760
      Width           =   3735
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "(Yes/No)"
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   14.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   495
      Index           =   29
      Left            =   9240
      TabIndex        =   33
      Top             =   4920
      Width           =   1335
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "order."
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFC0FF&
      Height          =   375
      Index           =   28
      Left            =   8880
      TabIndex        =   32
      Top             =   7800
      Width           =   1095
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "to complete"
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFC0FF&
      Height          =   375
      Index           =   27
      Left            =   8880
      TabIndex        =   31
      Top             =   7320
      Width           =   2775
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Thank You"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   14.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   375
      Index           =   26
      Left            =   9600
      TabIndex        =   30
      Top             =   6840
      Width           =   1815
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Say"
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFC0FF&
      Height          =   375
      Index           =   25
      Left            =   8880
      TabIndex        =   29
      Top             =   6840
      Width           =   735
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "If I misheard you."
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFC0FF&
      Height          =   375
      Index           =   24
      Left            =   8880
      TabIndex        =   28
      Top             =   6240
      Width           =   3015
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Mistake"
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   14.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Index           =   23
      Left            =   9600
      TabIndex        =   27
      Top             =   5760
      Width           =   1455
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Say"
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFC0FF&
      Height          =   375
      Index           =   22
      Left            =   8880
      TabIndex        =   26
      Top             =   5760
      Width           =   735
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$.50"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   21
      Left            =   7560
      TabIndex        =   25
      Top             =   7800
      Width           =   1215
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$.65"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   20
      Left            =   7560
      TabIndex        =   24
      Top             =   7440
      Width           =   1215
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$.99"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   19
      Left            =   7560
      TabIndex        =   23
      Top             =   7080
      Width           =   1215
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$.75"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   18
      Left            =   7560
      TabIndex        =   22
      Top             =   6480
      Width           =   1215
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$.95"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   17
      Left            =   7560
      TabIndex        =   21
      Top             =   6120
      Width           =   1215
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$1.05"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   16
      Left            =   7560
      TabIndex        =   20
      Top             =   5760
      Width           =   1215
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$1.65"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   15
      Left            =   3360
      TabIndex        =   19
      Top             =   5760
      Width           =   1335
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$.36"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   14
      Left            =   3000
      TabIndex        =   18
      Top             =   7320
      Width           =   1215
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$2.75"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   13
      Left            =   3360
      TabIndex        =   17
      Top             =   6840
      Width           =   1335
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$2.05"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   12
      Left            =   3360
      TabIndex        =   16
      Top             =   6480
      Width           =   1335
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "$2.25"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808080&
      Height          =   375
      Index           =   11
      Left            =   3360
      TabIndex        =   15
      Top             =   6120
      Width           =   1335
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Small Drink"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   10
      Left            =   5280
      TabIndex        =   14
      Top             =   7800
      Width           =   2175
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Medium Drink"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   9
      Left            =   5280
      TabIndex        =   13
      Top             =   7440
      Width           =   2175
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Large Drink"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   8
      Left            =   5280
      TabIndex        =   12
      Top             =   7080
      Width           =   2175
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Small Fries"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   7
      Left            =   5280
      TabIndex        =   11
      Top             =   6480
      Width           =   2175
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Medium Fries"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   6
      Left            =   5280
      TabIndex        =   10
      Top             =   6120
      Width           =   2175
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Large Fries"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   375
      Index           =   5
      Left            =   5280
      TabIndex        =   9
      Top             =   5760
      Width           =   2175
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "With Cheese"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   4
      Left            =   600
      TabIndex        =   8
      Top             =   7320
      Width           =   2175
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Double Bacon Burger"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   3
      Left            =   240
      TabIndex        =   7
      Top             =   6840
      Width           =   3015
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Bacon Burger"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   2
      Left            =   240
      TabIndex        =   6
      Top             =   6480
      Width           =   3015
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Double Burger"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   1
      Left            =   240
      TabIndex        =   5
      Top             =   6120
      Width           =   3015
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Hamburger"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   0
      Left            =   240
      TabIndex        =   4
      Top             =   5760
      Width           =   3015
   End
   Begin VB.Label Label1 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Your Order..."
      Height          =   252
      Left            =   5400
      TabIndex        =   1
      Top             =   2400
      Width           =   1812
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Rem gMode keeps track of the state were in, what we expect to hear
Public gMode As Integer
Rem gOrderedFries is set to True if this customer has mentioned fries yet, False otherwise
Public gOrderedFries As Boolean
Rem gRestoreGrammer is True if we need to reload the burger grammer after the yesno grammer is done
Public gRestoreGrammar As Boolean


Rem This function is called back whenever a phrase is recognized. See documentation input values.
Rem All we care about here is Phrase, which is the verbatim phrase heard,
Rem and Parsed, which is the translated string from the context free grammar
Private Sub DirectSR1_PhraseFinish(ByVal flags As Long, ByVal beginhi As Long, ByVal beginlo As Long, ByVal endhi As Long, ByVal endlo As Long, ByVal Phrase As String, ByVal parsed As String, ByVal results As Long)

Rem If we got a null match, then we misheard
If (parsed = "") Then
    Rem Most sound cards cannot multi-plex recording and playing, so it is imperative
    Rem we disable the listening state of the recognizer before attempting to generate sound
    Rem otherwise we will get an error when we try to speak
    DirectSR1.Deactivate
    Rem Remember that control will go to DirectSS1_AudioStop after the phrase is spoken
    If (Rnd > 0.5) Then 'randomly pick a message to keep from being too boring
        DirectSS1.Speak "I'm sorry. Can you please repeat that?"
    Else
        DirectSS1.Speak "I didn't understand you."
    End If
Else
    Rem We heard something we should understand...
    If (parsed = "thank you") Then
            Rem We are about to speak something, so stop listening
            DirectSR1.Deactivate
            
            Rem don't forget to suggestive sell the boss always says
            If (gOrderedFries = 0) Then
                gMode = 2
                DirectSS1.Speak "Would you like a large french fry with that?"
                
            Else
                gMode = 3
                DirectSS1.Speak "Will that be all?"
            End If
    ElseIf (parsed = "mistake") Then
        Rem remove the last thing we added to the list..it was a mistake
        Item = List1.ListCount - 1
        If (Item >= 0) Then
            List1.RemoveItem (List1.ListCount - 1)
             Rem we are about to speak, so turn off listening
             DirectSR1.Deactivate
             DirectSS1.Speak "Oops"
        End If
    ElseIf (parsed = "no") Then
        Rem we asked question which the customer said no to..what we do depends on context gMode
        If (gMode = 2) Then
            gMode = 3
            gOrderedFries = True
            Rem they didn't want extra fries, stop listening and prompt again
            DirectSR1.Deactivate
            DirectSS1.Speak "Will that be all?"
        ElseIf (gMode = 3) Then
            gMode = 0
            gRestoreGrammar = True
            Rem we thought they were done, but they werent. stop listening and prompt for more
            DirectSR1.Deactivate
            DirectSS1.Speak "Please continue your order"
        Else
            gMode = 0
            gRestoreGrammar = True
        End If
        
    ElseIf (parsed = "yes") Then
        Rem BINGO we suggestive sold them. Add a large fries to the order and prompt.
        Rem dont forget to turn off listening!
        If (gMode = 2) Then
            List1.AddItem "1.05 large fries"
            gMode = 3
            DirectSR1.Deactivate
            DirectSS1.Speak "Will that be all?"
        ElseIf (gMode = 3) Then
            Rem ok, the order is done. total it up and tell them the price. Turn off listeing
            Rem before talking!
            total = 0
            For i = 0 To List1.ListCount - 1
                total = total + Val(List1.List(i))
            Next i
            List1.AddItem "---total is: $" + Format(total, "###0.00")
            gMode = 1
            gRestoreGrammar = True
            
            DirectSR1.Deactivate
            
            DirectSS1.Speak "Your total will be $" + Format(total, "###0.00") + " at the next window please."
        End If
    Else
        Rem we heard an order..stop listening because we will repeat it and add it to the list
        DirectSR1.Deactivate
        List1.AddItem parsed
        Rem if they say fries, then we won't suggestive sell fries when they are done
        If (Phrase = "large fries") Or (Phrase = "small fries") Or (Phrase = "medium fries") Then
            gOrderedFries = True
        End If
        Rem speak back what they said..notice we use Phrase, not Parsed, so we repeat
        Rem what they said, not the translated version the grammar uses (ie not the part in quotes)
        DirectSS1.Speak Phrase
    End If
End If
 Rem highlight the last item in the listbox
 List1.ListIndex = List1.ListCount - 1
End Sub

Rem this is called after we have finished speaking a sentence
Private Sub DirectSS1_AudioStop(ByVal hi As Long, ByVal lo As Long)
If (gMode = 1) Then
    List1.Clear
    gOrderedFries = 0
    gMode = 0
    DirectSS1.Speak "Hi! May I take your order?"
Else
    If (gMode = 2) Or (gMode = 3) Then
      HighLightOrder (False)
      DirectSR1.GrammarFromFile App.Path + "\yesno.txt"
      
    ElseIf (gRestoreGrammar) Then
        HighLightOrder (True)
        DirectSR1.GrammarFromFile App.Path + "\burger.txt"
        gRestoreGrammar = False
        
    End If
    Rem we had to turn off listening before talking, so now that we are done talking we
    Rem should start listening again
    DirectSR1.Activate
End If
End Sub

Rem most of the initialization is done on the first timer callback. This is so we get the window
Rem up and painted before we do the CPU intensive task of initializing the recognizer
Private Sub Form_Load()
gMode = 0
gRestoreGrammar = False
Form1.Picture = LoadPicture(App.Path + "\Bursign3.bmp")
End Sub

Private Sub Form_Terminate()
Rem stop listening. This is necessary because the VB environment may still have the
Rem engine mapped in when we try to run again; we will be unable to reuse these resources
Rem until they are freed from the VB process.
DirectSR1.Deactivate
End Sub

Private Sub Timer1_Timer()
On Error GoTo GeneralFail
Rem we just use this function once, on the first callback, then the timer isn't needed anymore
Timer1.Enabled = False

Rem Initialize the recognizer. We want command and control, so find and select that engine (or best match)
DirectSR1.Initialized = 1

engine = DirectSR1.Find("MfgName=Microsoft;Grammars=1")
DirectSR1.Select engine

Rem Burger.txt is our uncompiled grammar. Compile and load it now.
HighLightOrder (True)
DirectSR1.GrammarFromFile App.Path + "\burger.txt"

Rem Now load the synthesizer, find and select any female voice from microsoft

voice = DirectSS1.Find("MfgName=Microsoft;Gender=1")
DirectSS1.Select voice

Rem Start the voice interface...control goes to DirectSS1_AudioStop after this is said, gMode=0
DirectSS1.Speak "Thank you for coming to Virtual Burger. May I take your order?"

Rem sometimes the vb environment comes to front while we're loading, so take the screen back
Form1.Show
GoTo done
GeneralFail:
MsgBox "Unable to start using text to speech or recognition."
done:
End Sub

Sub HighLightOrder(burgers As Boolean)
    If (burgers) Then
        fColor = &H0
        Label2(29).ForeColor = &HFFFFFF
    Else
        fColor = &HC0C0C0
        Label2(29).ForeColor = &H0
    End If
    For i = 0 To 10
        Label2(i).ForeColor = fColor
    Next i
    Label2(23).ForeColor = fColor 'mistake
    Label2(26).ForeColor = fColor 'thank you
    
End Sub
