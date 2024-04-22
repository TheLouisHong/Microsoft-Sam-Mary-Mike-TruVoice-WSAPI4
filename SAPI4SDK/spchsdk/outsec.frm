VERSION 5.00
Object = "{FC9E7401-6058-11D1-8C66-0060081841DE}#1.0#0"; "Xtel.dll"
Begin VB.Form Form1 
   Caption         =   "Secretary"
   ClientHeight    =   1584
   ClientLeft      =   60
   ClientTop       =   348
   ClientWidth     =   3876
   LinkTopic       =   "Form1"
   ScaleHeight     =   1584
   ScaleWidth      =   3876
   StartUpPosition =   3  'Windows Default
   Begin VB.OptionButton Option1 
      Caption         =   "PC simulation"
      Height          =   270
      Left            =   240
      TabIndex        =   5
      Top             =   240
      Value           =   -1  'True
      Width           =   1380
   End
   Begin VB.OptionButton Option2 
      Caption         =   "Telephone "
      Height          =   255
      Left            =   240
      TabIndex        =   4
      Top             =   600
      Width           =   1395
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Start Monitoring..."
      Height          =   375
      Left            =   2280
      TabIndex        =   3
      Top             =   120
      Width           =   1455
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Exit"
      Height          =   375
      Left            =   2280
      TabIndex        =   2
      Top             =   600
      Width           =   1455
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Select Engines..."
      Height          =   375
      Left            =   2280
      TabIndex        =   1
      Top             =   1080
      Width           =   1455
   End
   Begin TELLibCtl.phone phone1 
      Height          =   375
      Left            =   240
      OleObjectBlob   =   "outsec.frx":0000
      TabIndex        =   0
      Top             =   1080
      Visible         =   0   'False
      Width           =   1335
   End
   Begin VB.Frame Frame1 
      Caption         =   "Monitor..."
      Height          =   975
      Left            =   120
      TabIndex        =   6
      Top             =   0
      Width           =   1815
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" _
         (hpvDest As Any, hpvSource As Any, ByVal cbCopy As Long)
         
Private Sub Init()
On Error GoTo ok
If Right(App.Path, 1) = "\" Then
    slash = ""
Else
    slash = "\"
End If
    MkDir App.Path + slash + "VoiceNotes"
ok:
End Sub

    
Private Sub Command1_Click()
Form1.Visible = False 'hide the main form
Init
If (Option1.Value) Then
    phone1.Initialized = 1   'run the emulator
Else
    phone1.Initialized = 2   'connect to the TAPI phone
End If
phone1.CallDialog            'Run a thread for each phone line. This thread 'hangs' until threads finish.
Form1.Visible = True         're-enable the UI for another session
End Sub

Private Sub Command2_Click()
End
End Sub

Private Sub Command3_Click()
phone1.ChooseEngineDialog
End Sub


Private Sub phone1_DoPhoneCall(ByVal lineID As Long)

Dim App As Object
Dim ns As Object
Dim Contacts As Object
Dim Cal As Object
Dim item As Object ' ContactItem
Dim ContactsGrammar As String
Dim ContactsPrompt As String
Dim ContactsTotal As String
Dim s As String
Dim result As Long
Dim temp As String
Dim email As Object
Dim Calendar As Object
Dim Tasks As Object
Dim Notes As Object


On Error GoTo FailMessage
Set App = CreateObject("Outlook.Application")

Set ns = App.GetNameSpace("MAPI")

Set Contacts = ns.GetDefaultFolder(10)
Set email = ns.GetDefaultFolder(6)
Set Calendar = ns.GetDefaultFolder(9)
Set Tasks = ns.GetDefaultFolder(13)
Set Notes = ns.GetDefaultFolder(12)

GoTo Succeeded:
FailMessage:
MsgBox "Unable to start Outlook"
GoTo terminate
Succeeded:
On Error GoTo terminate

firsttime = True
mainloop:

phone1.Speak lineID, "Hi. This is the Microsoft Outlook Secretary Main Menu."
If (firsttime) Then
    phone1.Speak lineID, "Tip of the day: press the star key to interrupt."
    firsttime = False
End If

MainMenu = "[Prompts]" + vbNewLine + _
            "Main=how can I help?" + vbNewLine + _
            "Help=You can say Contacts, Inbox, Calendar, Tasks, or Notes" + vbNewLine + _
            "Where=Main Menu" + vbNewLine + _
            "Unrecognized= ok" + vbNewLine + _
            "[<MyGrammar>]" + vbNewLine + _
            "<MyGrammar>=Contacts ""Contacts""" + vbNewLine + _
            "<MyGrammar>=Inbox ""Inbox""" + vbNewLine + _
            "<MyGrammar>=Email ""Inbox""" + vbNewLine + _
            "<MyGrammar>=Calendar ""Calendar""" + vbNewLine + _
            "<MyGrammar>=Tasks ""Tasks""" + vbNewLine + _
            "<MyGrammar>=Notes ""Notes""" + vbNewLine
            
phone1.GrammarFromString lineID, MainMenu, result, s
If (s = "Contacts") Then
    DoContacts lineID, App, Contacts
    GoTo mainloop
ElseIf (s = "Inbox") Then
    phone1.Speak lineID, s
    DoEmail lineID, App, email
    GoTo mainloop
ElseIf (s = "Calendar") Then
    phone1.Speak lineID, s
    DoCalendar lineID, App, Calendar
    GoTo mainloop
ElseIf (s = "Tasks") Then
    phone1.Speak lineID, s
    DoTasks lineID, App, Tasks
    GoTo mainloop
ElseIf (s = "Notes") Then
    phone1.Speak lineID, s
    DoNotes lineID, App, Notes
    GoTo mainloop
End If
terminate:
End Sub

Private Sub DoEmail(lineID As Long, Appl As Object, Inbox As Object)
Dim result As Long
Dim s As String

    phone1.Speak lineID, "I am scanning for uhn red email"
    phone1.Speak lineID, "you have " + Str(Inbox.unreaditemcount) + " new uhn red emails"
    If (Inbox.unreaditemcount > 0) Then
        i = 1
        For Each item In Inbox.items
        On Error GoTo skipit
            If (item.Unread) Then
                If item.MessageClass = "IPM.Note" Then
                    sender = item.SenderName
                Else
                    sender = "other message type"
                End If
                phone1.Speak lineID, sender + " subject " + item.Subject
                phone1.YesNoFromString lineID, "[Prompts]" + vbNewLine + "Main=should i read it?" + vbNewLine + "[Settings]" + vbNewLine + "CanGoBack=1" + vbNewLine, result, s
                If (result < 0) Then
                    GoTo done
                End If
                If (result = 1) Then 'read it
                    phone1.Speak lineID, item.Body
                    'item.Unread = False
                Else
                    phone1.Speak lineID, "next"
                End If
        
            Else
                'just scan first 100 since the rest are probably already read
                If i > 100 Then
                    GoTo donewithheaders
                End If
            End If
skipit:
        i = i + 1
        Next item
    End If
done:
donewithheaders:
    phone1.Speak lineID, "done with email"

End Sub

Private Sub NewContact(lineID As Long, App As Object, Contacts As Object)
Dim result As Long
Dim s As String
Dim newcon As Object
Dim FullName As String
Dim PhoneNumber As String

    phone1.Speak lineID, "New Contact."

    phone1.Speak lineID, "Spell or type in the contacts name on the key pad.  Say done or press pound pound when finished."
    FullName = GetSpelling(lineID)
    If (FullName = "") Then
        GoTo done
    End If
getphonenumber:
    
    phone1.PhoneFromString lineID, "[Prompts]" + vbNewLine + "Main=What is the contact's phone number?" + vbNewLine + "[Settings]" + vbNewLine + "CanGoBack=1" + vbNewLine, result, s
    PhoneNumber = s
    If (PhoneNumber = "") Then
        GoTo done
    End If

    Set newcon = App.CreateItem(2)

    newcon.FullName = FullName
    newcon.HomeTelephoneNumber = PhoneNumber
    newcon.Save
    phone1.Speak lineID, "Contact Created"
done:
End Sub

Private Sub DoCalendar(lineID As Long, App As Object, Calendar As Object)
Calendar.items.IncludeRecurrences = True
tdystart = Format(Date, "Short Date") & " 12:00 AM"
tdyend = Format(Date, "Short Date") & " 11:59 PM"

Set Citems = Calendar.items
Citems.Sort "[Start]", False
Set currentAppointment = Citems.Find("[Start] >= """ & _
    tdystart & """ and [Start] <= """ & tdyend & """")

phone1.Speak lineID, "Today's Appointments"
While TypeName(currentAppointment) <> "Nothing"
    phone1.Speak lineID, "subject is " + currentAppointment.Subject
    cc = CDate(currentAppointment.Start)
    cc = Format(cc, "hh:mm AMPM")
    
    phone1.Speak lineID, "start time is " + CStr(cc)
    phone1.Speak lineID, "located at " + currentAppointment.Location
   
    Set currentAppointment = Citems.FindNext
Wend

End Sub

Private Sub DoTasks(lineID As Long, App As Object, Tasks As Object)
For Each item In Tasks.items
        If (item.Subject <> "") Then
            If (item.complete <> True) Then
                phone1.Speak lineID, "Subject " + item.Subject
                phone1.Speak lineID, "Due " + CStr(item.duedate)
            End If
        End If
Next item
phone1.Speak lineID, "finished listing tasks"
End Sub

Private Sub DoNotes(lineID As Long, Appl As Object, Notes As Object)
Dim result As Long
Dim s As String

ContactsPrompt = "[Prompts]" + vbNewLine + _
                "Main=Read notes, or new note" + vbNewLine + _
                "Help=You can say read notes or new note" + vbNewLine + _
                "Where=Notes menu" + vbNewLine + _
                "Unrecognized= ok" + vbNewLine + _
                "[<MyGrammar>]" + vbNewLine + _
                "<MyGrammar>=new note ""new note""" + vbNewLine + _
                "<MyGrammar>=read notes ""read notes""" + vbNewLine
                
phone1.GrammarFromString lineID, ContactsPrompt, result, s

If (s = "new note") Then
    NewNote lineID, Appl, Notes
Else
    For Each item In Notes.items
            If (item.Subject <> "") Then
                    If (Left(item.Subject, 10) = "##wavfile:") Then
                        phone1.Speak lineID, "recorded note"
                        sss = Right(item.Subject, Len(item.Subject) - 10)
                        phone1.WaveAddFromListString lineID, "[FromFile]" + vbNewLine + sss + "=wave file deleted"
                        phone1.Speak lineID, "wave file deleted"
                    Else
                        phone1.Speak lineID, "Text Note " + item.Subject
                    End If
            End If
    Next item
    phone1.Speak lineID, "finished reading notes"
End If
End Sub

Private Sub NewNote(lineID As Long, Appl As Object, Notes As Object)
Dim result As Long
Dim size As Long
Dim wave As Long
Dim wavefile() As Byte
Dim filename As String


    phone1.RecordFromString lineID, "[Prompts]" + vbNewLine + _
                                "Main=Record your note now" + vbNewLine + _
                                "[Settings]" + vbNewLine + _
                                "BetweenUtt=3000" + vbNewLine + _
                                "InitialBuf=30000" + vbNewLine + _
                                "ReallocBuf=30000" + vbNewLine + _
                                "MaxBuf=300000" + vbNewLine + _
                                "NoAnswerTime=10" + vbNewLine _
                                , result, wave, size
                                
    'The following "casts" the wave to an array (by making a copy of it) so we can save it out to disk.
    ReDim wavefile(size)
    If (wave) Then
        CopyMemory wavefile(0), ByVal wave, size
    
        'free the wave as soon as possible so we dont pig up memory
        phone1.FreeWave wave
    
        'use the date and time as the filename, and put into the Messages directory
        filename = App.Path + "\VoiceNotes\Note Left at " + Format(Now, "hh mm ss AMPM") + " " + Format(Now, " mmm d yyyy") + ".wav"
    
        'write the wave data out to disk. you can double click on the file to play it with sound recorder.
        Open filename For Binary Access Write As #1
        Put #1, , wavefile
        Close #1
        Dim thisNote As Object
        Set thisNote = Appl.CreateItem(5)
        thisNote.Body = "##wavfile:" + filename
        thisNote.Save
        phone1.Speak lineID, "note recorded"
    End If
End Sub

Rem Strip removes some of the characters that are invalid in a grammar string
Private Function Strip(s As String) As String
Dim retstring As String
ll = Len(s)
For ii = 1 To ll
cc = Mid(s, ii, 1)
If ((cc <> "(") And (cc <> ")") And (cc <> "[") And (cc <> "]") And (cc <> "<") And (cc <> ">")) Then
    retstring = retstring + cc
End If
Next ii
Strip = retstring
End Function

Private Sub DoContacts(lineID As Long, App As Object, Contacts As Object)
Dim result As Long
Dim s As String

ContactsPrompt = "[Prompts]" + vbNewLine + _
                                 "Main=list contacts, include addresses, new contact, or say a contacts name" + vbNewLine + _
                                 "Help=You can say list contacts, include addresses, new contact, or say a contacts name" + vbNewLine + _
                                 "Where=Contacts menu" + vbNewLine + _
                                 "Unrecognized= ok" + vbNewLine
                                
                                 
ContactsGrammar = "[<MyGrammar>]" + vbNewLine + _
                  "<MyGrammar>=List contacts ""List""" + vbNewLine + _
                  "<MyGrammar>=include addresses ""Include""" + vbNewLine + _
                  "<MyGrammar>=new contact ""New Contact""" + vbNewLine
'fill in contacts grammar

i = 1
For Each item In Contacts.items
        If (item.FullName <> "") Then
                    ContactsGrammar = ContactsGrammar + "<MyGrammar>= " + Strip(item.FullName) + " """ + Str(i) + " """ + vbNewLine
        End If
         i = i + 1
Next item
 
ContactsTotal = ContactsPrompt + ContactsGrammar
repeat:
includeAddress = False
repeat2:
 phone1.GrammarFromString lineID, ContactsTotal, result, s
 If (result < 0) Then
    GoTo done
End If
 temp = s

If (temp = "List") Then 'list them
    phone1.Speak lineID, "contact list"
    For j = 1 To i - 1
        s = Contacts.items(j).FullName
        If (s <> "") Then
            phone1.Speak lineID, s
        End If
    Next j
    phone1.Speak lineID, "finished listing contacts"
    GoTo repeat2
ElseIf (temp = "New Contact") Then
    NewContact lineID, App, Contacts
Else
    If (temp = "Include") Then
        includeAddress = True
        phone1.Speak lineID, "including address"
        GoTo repeat2
    End If

    If (includeAddress) Then
        s = Contacts.items(Val(temp)).HomeAddress
        If s <> "" Then
            phone1.Speak lineID, Contacts.items(Val(temp)).FullName + "'s home address is " + s
        End If
        s = Contacts.items(Val(temp)).BusinessAddress
        If s <> "" Then
            phone1.Speak lineID, Contacts.items(Val(temp)).FullName + "'s business address is " + s
        End If
    End If
    
    s = Contacts.items(Val(temp)).HomeTelephoneNumber
    If s <> "" Then
        phone1.Speak lineID, Contacts.items(Val(temp)).FullName + "'s home phone number is " + s
    End If
    s = Contacts.items(Val(temp)).BusinessTelephoneNumber
    If s <> "" Then
        phone1.Speak lineID, Contacts.items(Val(temp)).FullName + "'s business number is " + s
    End If
    s = Contacts.items(Val(temp)).HomeFaxNumber
    If s <> "" Then
        phone1.Speak lineID, Contacts.items(Val(temp)).FullName + "'s home fax number is " + s
    End If
    s = Contacts.items(Val(temp)).BusinessFaxNumber
    If s <> "" Then
        phone1.Speak lineID, Contacts.items(Val(temp)).FullName + "'s business fax number is " + s
    End If
End If
done:
End Sub

Private Function GetSpelling(lineID As Long)
Dim s As String

Dim result As Long

phone1.SpellingFromString lineID, "", result, s
GetSpelling = s

End Function




