VERSION 5.00
Object = "{FC9E7401-6058-11D1-8C66-0060081841DE}#1.0#0"; "Xtel.dll"
Begin VB.Form Form1 
   Caption         =   "ACME Shareware"
   ClientHeight    =   1728
   ClientLeft      =   60
   ClientTop       =   348
   ClientWidth     =   4032
   LinkTopic       =   "Form1"
   ScaleHeight     =   1728
   ScaleWidth      =   4032
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command3 
      Caption         =   "Select Engines..."
      Height          =   375
      Left            =   2280
      TabIndex        =   5
      Top             =   1200
      Width           =   1455
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Exit"
      Height          =   375
      Left            =   2280
      TabIndex        =   4
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Start Monitoring..."
      Height          =   375
      Left            =   2280
      TabIndex        =   3
      Top             =   240
      Width           =   1455
   End
   Begin VB.OptionButton Option2 
      Caption         =   "Telephone "
      Height          =   255
      Left            =   255
      TabIndex        =   2
      Top             =   720
      Width           =   1395
   End
   Begin VB.OptionButton Option1 
      Caption         =   "PC simulation"
      Height          =   270
      Left            =   240
      TabIndex        =   1
      Top             =   360
      Value           =   -1  'True
      Width           =   1380
   End
   Begin TELLibCtl.phone phone1 
      Height          =   390
      Left            =   240
      OleObjectBlob   =   "sharware.frx":0000
      TabIndex        =   0
      Top             =   1200
      Visible         =   0   'False
      Width           =   1215
   End
   Begin VB.Frame Frame1 
      Caption         =   "Monitor..."
      Height          =   975
      Left            =   120
      TabIndex        =   6
      Top             =   120
      Width           =   1815
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'This is an example of how to write multi-line telephony controls in visual basic.
'
'NOTES ON TELEPHONY:
'Telephony apps can run in two modes: emulation mode and telephone mode.
'Emulation mode uses the microphone and speakers instead of the telephone.
'Telephone mode uses the TAPI compliant voice modem/multi-line card.
'
'The main differences between the emulator and the real phone are
'0. In order to use a Voice modem, you have to get a recent version of the modem driver.
'   Many voice modems use the driver named UnimodemV which can be downloaded from
'   Microsoft's web site.
'1. Voice modems often do not have the sound quality or response time of the emulator.
'   If the quality is too poor, you will be incented to purchase a better voice modem
'   or even a multi-line card for real world use.
'2. Multi-line cards are run with multiple threads.  The VB debugger cannot be used
'   with multithreaded applets, so you will have to debug with one line or with the
'   emulator.  To run with multiple lines, you must compile the app and run it
'   outside of the visual basic environment.
'CAVEAT PROGRAMMER: note that the BETA version of the Microsoft continuous recognition engine
'will not work multi-threaded.  Be sure to set the engine to "command and control" if you want
'to try this on a multi-line card.
'
'The sample uses Text-To-Speech (TTS) to speak all the prompts.  For a real app, you
'would want to use human voice recorded prompts.  Luckily, the SDK provides
'the Wave List Editor program and the WaveAddFromList* interface for doing this
'easily.
'First, you proto-type your app using TTS (like this applet).  Next, you
'run your applet with logging enabled.  Run through all the prompts.  Next,
'you run the Wave List Editor program, and tell it the directory of where the
'log files are located.  The editor gives you a list of text strings the applet
'said, and lets you record a human voice for them.  When you are done with all
'the prompts, you can save the text-to-wave mappings file.  All you need to do
'is call phone1.WaveAddFromListFile(lineID,"c:\directory\mapfile.txt") first thing
'in phone1_DoPhoneCall and the wave files will be played instead of the synthesized
'voice for each prompt that was recorded.
'You can get more information about this in the documentation.
'
'
'NOTES ON THE APPLET
'This applet is a model for how to do automatic shareware registration over the phone.
'The idea is that the shareware version of a (different) program expires or has a reduced feature set,
'and a user has to register the program to get the full program.  In order to do this
'robustly, the actual shareware program would generate a key (passkey) which uniquely
'identifies the computer it is running on..(based on drive C serial # perhaps?) so the registration
'key can be encrypted with this information and saved in the registry,
'thus making the full version un-copyable, (provided clever enough coding).
'
'This sample is simply that: a SAMPLE.  Although it demonstrates many features you
'can use to collect information such as credit card numbers from the caller, you would
'need to modify it extensively to do things like get the brand name of the card
'and adapt to that company's card format.  And of course you would need to plug in
'code that actually validates/charges the card, which is not supplied.
'The sample does not provide the logic for creating passkey's or applying registration
'numbers to applets either.  You have to figure that out for yourself!
'Finally, it should be noted that in the real world you might want to have the user
'type in all the information in the app's registration dialog, and include that information
'encrypted in the passkey, allowing you to get the text version of the card holder's name, perhaps
'with some checksums to validate correcteness.

'The 'start monitoring... button
Private Sub Command1_Click()
Form1.Visible = False 'hide the main form
If (Option1.Value) Then
    phone1.Initialized = 1   'run the emulator
Else
    phone1.Initialized = 2   'connect to the TAPI phone
End If
phone1.CallDialog            'Run a thread for each phone line. This thread 'hangs' until threads finish.
Form1.Visible = True         're-enable the UI for another session
End Sub

'The 'Selection Engines...' button
Private Sub Command3_Click()
phone1.ChooseEngineDialog 'this lets the user set the TTS voice used, as well as the voice engine used.
End Sub

'The 'Exit' button
Private Sub Command2_Click()
End '
End Sub

'This is the key multi-threaded callback which is called for each phoneline.
'lineID is a magic cookie which identifies which line/thread is being used.
Private Sub phone1_DoPhoneCall(ByVal lineID As Long)
'All variables used in this routine MUST BE LOCAL to the routine.  You cannot modify global
'variables from this routine since it may be executing on multiple asynchronous threads simultaneously.
Dim sPassKey As String
Dim CFirstFour As String
Dim CNextSix As String
Dim CLastFive As String
Dim CExpDate As String
Dim result As Long
Dim s As String
Dim ss As String
Dim sDTMF As String

'This call would be enabled to after using the Wave List Editor to change prompts to human voice
'files.  Be sure to set the directory correctly.
Rem phone1.WaveAddFromListFile lineID, "c:\directory\wavemap.txt"

'The first greeting.  Be sure to tell uses to wait for the beep and speak clearly, or they won't...

   phone1.Speak lineID, "Welcome to ACME shareware automated registration center.  The program pass key is required to register your shareware program. You can get the passkey from the shareware version of the program by choosing passkey from the register menu. Please speak clearly and wait for me to finish talking before answering the prompts."
PassKey:
'call our helper routine which gets numbers. See below
   s = GetNumber(lineID, 4, "Please say or enter the four digit passkey.", _
                            "What is the passkey?", _
                            "This is a sample. Just make up a four digit number.", _
                            "This is a sample. Just make up a four digit number.")
   If (s = "") Then
    GoTo done
   End If
   
   phone1.Speak lineID, "your passkey is "
   phone1.Speak lineID, s
   
   sPassKey = s
   
'Always verify information. Mistakes are common, so verify verify verify..
'We use the string versin of the YesNo Control because the prompts are small.
'NOTE you must add newlines as per the grammar syntax--newlines are semantic boundaries.
'If you don't like it, save it in a file and use the File version of the call.

   phone1.YesNoFromString lineID, "[Prompts]" + vbNewLine + _
                                  "Main=is this correct?" + vbNewLine + _
                                  "where=getting passkey" + vbNewLine + _
                                  "[Settings]" + vbNewLine + _
                                  "CanGoBack=1" + vbNewLine, result, s
   If (result < 0) Then
          GoTo done
   End If
   If (result <> 1) Then
    GoTo PassKey
   End If
  
 'Get the credit card number. Of course, in the real world we would need to get the brand
 'and use the correct format for that brand of credit card.
 
   phone1.Speak lineID, "now we need your credit card number."
firstfourcredit:
   s = GetNumber(lineID, 4, "Please say or enter the first four digits of your credit card number", _
                            "What are the first four digits?", _
                            "you are entering the first four numbers of your credit card.", _
                            "Tell me your credit card number.")
   If (s = "") Then
    GoTo done
   End If
   
   phone1.Speak lineID, "the first four digits of your credit card are"
   phone1.Speak lineID, s
   CFirstFour = s
'verify!
   phone1.YesNoFromString lineID, "[Prompts]" + vbNewLine + _
                                  "Main=is this correct?" + vbNewLine + _
                                   "where=getting first four" + vbNewLine + _
                                   "[Settings]" + vbNewLine + _
                                   "CanGoBack=1" + vbNewLine, result, s
   If (result < 0) Then
      GoTo done
    End If
   If (result <> 1) Then
    GoTo firstfourcredit
   End If
   
nextsixcredit:
   s = GetNumber(lineID, 6, "Please say or enter the next six digits of your credit card number", _
                            "What are the next six digits?", _
                            "you are entering the next six numbers of your credit card.", _
                            "Tell me your credit card number.")
   If (s = "") Then
    GoTo done
   End If
   
   phone1.Speak lineID, "the next six digits of your credit card are "
   phone1.Speak lineID, s
   CNextSix = s
'verify!
   phone1.YesNoFromString lineID, "[Prompts]" + vbNewLine + _
                                  "Main=is this correct?" + vbNewLine + _
                                  "where=getting next six" + vbNewLine + _
                                  "[Settings]" + vbNewLine + _
                                  "CanGoBack=1" + vbNewLine, result, s
   If (result < 0) Then
     GoTo done
   End If
   If (result <> 1) Then
    GoTo nextsixcredit
   End If
   
lastfivecredit:
   
   s = GetNumber(lineID, 5, "Please say or enter the last five digits of your credit card number", _
                            "What are the last five digits?", _
                            "you are entering the last five numbers of your credit card.", _
                            "Tell me your credit card number.")
   If (s = "") Then
    GoTo done
   End If
   
   phone1.Speak lineID, "the last five digits of your credit card are "
   phone1.Speak lineID, s
   CLastFive = s
'verify!
   phone1.YesNoFromString lineID, "[Prompts]" + vbNewLine + _
                                   "Main=is this correct?" + vbNewLine + _
                                   "where=getting last five" + vbNewLine + _
                                   "[Settings]" + vbNewLine + _
                                   "CanGoBack=1" + vbNewLine, result, s
   If (result < 0) Then
        GoTo done
    End If
   If (result <> 1) Then
    GoTo lastfivecredit
   End If
   
   phone1.Speak lineID, "now we need the expiration date. Please say the month and year, for example if the date is june 1999, say 6 slash 99, or on the key pad press 0 6 9 9"
expiration:
'Since the format of a credit card date is unique, and not simple, we use
'a grammar from a file (see date.txt).  The grammar allows a 4 digit DTMF tone
'to be entered, so we have to normalize the output so it is always spoken back out
'in the same format (and could be used to fill in a computerized form...).
   phone1.GrammarFromFile lineID, App.Path + "\date.txt", result, s
   
   If (s = "") Then
    GoTo done
   End If
   If (result < 0) Then
    GoTo done
   End If
'call normalize to be sure the output is in form  # space # slash # space #
'the DTMF direct entry obviously would not have the word 'slash' so we add it
'You would modify this code to return a date in the format your credit card
'validator expected.
   CExpDate = NormalizeDate(s)
   
   phone1.Speak lineID, "the expiration date on the card reads"
   phone1.Speak lineID, CExpDate
'validate!
   phone1.YesNoFromString lineID, "[Prompts]" + vbNewLine + _
                                  "Main=is this correct?" + vbNewLine + _
                                  "where=getting expiration date yes no prompt" + vbNewLine + _
                                  "[Settings]" + vbNewLine + _
                                  "CanGoBack=1" + vbNewLine, result, s
   If (result < 0) Then
        GoTo done
    End If
   If (result <> 1) Then
    GoTo expiration
   End If
   
name:
   Dim wave As Long
   Dim wsize As Long
   
   
'Ok, there are too many names and spellings of names to recognize.
'So, we just record it! If you need the exact spelling of the name for verification,
'you would have to codify that information in the passkey generated by the program..
   phone1.RecordFromString lineID, "[Prompts]" + vbNewLine + _
                                   "Main=what is the name on the card?" + vbNewLine + _
                                   "Main.2=what is the name" + vbNewLine + _
                                   "Where=getting card name" + vbNewLine + _
                                   "Help=I need to know the name on the card" + vbNewLine, result, wave, wsize
   If (result < 0) Then
            GoTo done
        End If
'TTS will speak a wave if you place a word in <>'s and use WaveAddFromMemory to indicate
'the wave file substitution.
   phone1.WaveAddFromMemory lineID, "<name>", wave, wsize
   phone1.Speak lineID, "the name is"
   phone1.Speak lineID, "<name>"
   
'even though its a recording, verify it!  Users make mistakes too.
   phone1.YesNoFromString lineID, "[Prompts]" + vbNewLine + _
                                  "Main=is this correct?" + vbNewLine + _
                                  "where=verifying name" + vbNewLine + _
                                  "[Settings]" + vbNewLine + _
                                  "CanGoBack=1" + vbNewLine, result, s
   If (result < 0) Then
            phone1.FreeWave wave
            GoTo done
        End If
   If (result <> 1) Then
    phone1.FreeWave wave
    GoTo name
   End If
'Now verify everything..
   phone1.Speak lineID, "Lets verify the information you have given me.  I have the passkey as " + sPassKey + ", the credit card as " + CFirstFour + CNextSix + CLastFive + "the expiration date as" + CExpDate + ", and the name as"
   phone1.Speak lineID, "<name>"
   
   phone1.YesNoFromString lineID, "[Prompts]" + vbNewLine + "Main=is this correct?" + vbNewLine + "where=verifying all" + vbNewLine + "[Settings]" + vbNewLine + "CanGoBack=1" + vbNewLine, result, s
   If (result <> 1) Then
    phone1.FreeWave wave
    GoTo PassKey
   End If
   phone1.FreeWave wave 'rem you'd record this for reference in a real program
   
'ask if its ok to charge the card
   phone1.YesNoFromString lineID, "[Prompts]" + vbNewLine + "Main=The charge will be five dollors. Is this ok??" + vbNewLine + "where=verifying price" + vbNewLine + "[Settings]" + vbNewLine + "CanGoBack=1" + vbNewLine, result, s
   If (result < 0) Then
            GoTo done
        End If
   If (result <> 1) Then
    GoTo NoPay
   End If
'here you would generate a real registration key, based on the passkey the user gave you.
'but since this is just a sample, we can hard code the number :)
   phone1.Speak lineID, "Your registration key is D O N 5 2 1. Enter this into the registration dialog box to enable the full features of the product. Again, the registration key is D O N 5 2 1.  Thank you for purchasing this ACME product."
repeat:
   phone1.YesNoFromString lineID, "[Prompts]" + vbNewLine + "Main=Registration key is D O N 5 2 1. Do you have it?" + vbNewLine + "where=giving out registration key" + vbNewLine + "[Settings]" + vbNewLine + "CanGoBack=1" + vbNewLine, result, s
   If (result < 0) Then
            GoTo done
        End If

   If (result <> 1) Then
    GoTo repeat
   End If
   
   GoTo done
NoPay:
    phone1.Speak lineID, "I am sorry, but you must agree to pay five dollars to get the registration key. Thanks for calling. goodbye."
    GoTo Done2
done:
   phone1.Speak lineID, "goodbye"
Done2:
End Sub

'Helper function to make sure the date is always in the same format,
'regardless of how it was entered (using DTMF or voice).
Private Function NormalizeDate(thedate As String) As String
Dim inputd As String
Dim ds As String
Dim c As String

inputd = thedate        'duplicate the string, since it may be a use-once string from a control
ds = ""                 'start with empty return value
slen = Len(inputd)
'strip out all non-numbers and add a space between each number.
For i = 1 To slen
    c = Mid$(inputd, i, 1)
    If (c >= "0") And (c <= "9") Then
        ds = ds + c + " "
    End If
    If (i = 2) Then
        ds = ds + "slash "  'add slash between month and year numbers
    End If
Next i

NormalizeDate = ds
End Function

'Helper function for getting a number.
'Allows retrieval of numbers using voice or DTMF
Private Function GetNumber(lineID As Long, numdigits As Integer, Main As String, main2 As String, where As String, help As String) As String
Dim result As Long
Dim s As String
Dim tstr As String

Dim NumGram As String

NumGram = ""
For i = 1 To numdigits
    NumGram = NumGram + "<0..9> "" """
Next i
phone1.GrammarFromString lineID, "[Prompts]" + vbNewLine + _
                                 "Main=" + Main + vbNewLine + _
                                 "Main.2=" + main2 + vbNewLine + _
                                 "Where=" + where + vbNewLine + _
                                 "Help=" + help + vbNewLine + _
                                 "[DTMFString]" + vbNewLine + _
                                 "Count=" + Str(numdigits) + vbNewLine + _
                                 "0=0" + vbNewLine + _
                                 "1=1" + vbNewLine + _
                                 "2=2" + vbNewLine + _
                                 "3=3" + vbNewLine + _
                                 "4=4" + vbNewLine + _
                                 "5=5" + vbNewLine + _
                                 "6=6" + vbNewLine + _
                                 "7=7" + vbNewLine + _
                                 "8=8" + vbNewLine + _
                                 "9=9" + vbNewLine + _
                                 "[<MyGrammar>]" + vbNewLine + _
                                 "<MyGrammar>= " + NumGram + vbNewLine, result, s

GetNumber = ""
tstr = s
For i = 1 To Len(tstr)
    GetNumber = GetNumber + Mid(tstr, i, 1) + " "
Next i

End Function


