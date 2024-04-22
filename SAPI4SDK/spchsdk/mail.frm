VERSION 5.00
Object = "{20C62CAE-15DA-101B-B9A8-444553540000}#1.1#0"; "MSMAPI32.OCX"
Object = "{EEE78583-FE22-11D0-8BEF-0060081841DE}#1.0#0"; "XVoice.dll"
Object = "{C1A8AF28-1257-101B-8FB0-0020AF039CA3}#1.1#0"; "MCI32.OCX"
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   Caption         =   "Mail Notify"
   ClientHeight    =   3180
   ClientLeft      =   165
   ClientTop       =   450
   ClientWidth     =   4080
   Icon            =   "Mail.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   3180
   ScaleWidth      =   4080
   StartUpPosition =   3  'Windows Default
   Visible         =   0   'False
   Begin ACTIVEVOICEPROJECTLibCtl.DirectSS DirectSS1 
      Height          =   1335
      Left            =   2280
      OleObjectBlob   =   "Mail.frx":030A
      TabIndex        =   2
      Top             =   0
      Width           =   1575
   End
   Begin MCI.MMControl MMControl2 
      Height          =   480
      Left            =   120
      TabIndex        =   1
      Top             =   2280
      Width           =   3555
      _ExtentX        =   6271
      _ExtentY        =   847
      _Version        =   327680
      DeviceType      =   ""
      FileName        =   ""
   End
   Begin MCI.MMControl MMControl1 
      Height          =   450
      Left            =   120
      TabIndex        =   0
      Top             =   1800
      Visible         =   0   'False
      Width           =   3540
      _ExtentX        =   6244
      _ExtentY        =   794
      _Version        =   327680
      DeviceType      =   ""
      FileName        =   ""
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   10000
      Left            =   120
      Top             =   960
   End
   Begin MSMAPI.MAPIMessages MAPIMessages1 
      Left            =   120
      Top             =   240
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   327680
      AddressEditFieldCount=   1
      AddressModifiable=   0   'False
      AddressResolveUI=   0   'False
      FetchSorted     =   -1  'True
      FetchUnreadOnly =   -1  'True
   End
   Begin MSMAPI.MAPISession MAPISession1 
      Left            =   840
      Top             =   360
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   327680
      DownloadMail    =   -1  'True
      LogonUI         =   -1  'True
      NewSession      =   0   'False
   End
   Begin VB.Label Label1 
      Caption         =   "This window will be invisible when the program runs"
      Height          =   375
      Left            =   600
      TabIndex        =   3
      Top             =   1320
      Width           =   3255
   End
   Begin VB.Menu notify 
      Caption         =   "Email Notify"
      Index           =   1
      Visible         =   0   'False
      Begin VB.Menu readbody 
         Caption         =   "Read Body"
         Checked         =   -1  'True
         Index           =   1
      End
      Begin VB.Menu exit 
         Caption         =   "Exit"
         Index           =   1
      End
      Begin VB.Menu sep 
         Caption         =   "-"
      End
      Begin VB.Menu stopread 
         Caption         =   "Stop reading!"
         Index           =   1
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Rem This sample will play a signon sound and read the read the subject of
Rem newly arrived e-mail for network email users.  It uses the MAPI control to
Rem access email, so your email must be MAPI compliant.
Rem Microsoft Exchange is MAPI compliant. So is Microsoft Outlook.

Rem NOTE: when MAPI polls for new mail, the applet will stop responding, as
Rem will the VB environement..until the transaction is complete.  Thus this
Rem applet works better as a compiled and installed executable. (use the
Rem application setup wizard and be sure to add the .wav files to the setup
Rem process.)  As a true executable, the applet will only 'hang' itself and
Rem let other programs (like VB) run normally.

Rem NOTE: To change how often mail is polled, change  POLLTIME interval constant
Rem This program is probably mostly useful for email
Rem users on a network, since dialing the phone just to read the subject line
Rem might be a bit ridiculous...you'd also have to automate the dial-out process.

Rem NOTIFYICONDATA is data type is needed for creating an icon on the tray
Private Type NOTIFYICONDATA
         cbSize As Long
         hwnd As Long
         uId As Long
         uFlags As Long
         uCallBackMessage As Long
         hIcon As Long
         szTip As String * 64
      End Type
      
Rem constants for tray notifications
Private Const NIM_ADD = &H0
Private Const NIM_DELETE = &H2
Private Const WM_MOUSEMOVE = &H200
Private Const NIF_MESSAGE = &H1
Private Const NIF_ICON = &H2
Private Const NIF_TIP = &H4
Private Const WM_RBUTTONDOWN = &H204     'Button down

Private Declare Function Shell_NotifyIcon Lib "shell32" _
   Alias "Shell_NotifyIconA" _
   (ByVal dwMessage As Long, pnid As NOTIFYICONDATA) As Boolean
Dim nid As NOTIFYICONDATA
      
Public gStarted As Integer
Public gMode As Integer
Public LastRecieved As String

Private Const POLLTIME = 10000
Private Sub exit_Click(Index As Integer)
Rem Clean up the MM controls and tray icon before we exit
MMControl1.Command = "Close"
MMControl2.Command = "Close"
Shell_NotifyIcon NIM_DELETE, nid
End 'end execution
End Sub

Private Sub Form_Load()
Rem This applet's main UI is an icon on the tray.
Rem This tray icon logic is from a tech article in MSDN
nid.cbSize = Len(nid)
nid.hwnd = Form1.hwnd
nid.uId = vbNull
nid.uFlags = NIF_ICON Or NIF_TIP Or NIF_MESSAGE
nid.uCallBackMessage = WM_MOUSEMOVE
nid.szTip = "eMail Notify" & vbNullChar
nid.hIcon = Form1.Icon
Shell_NotifyIcon NIM_ADD, nid

Rem initialize the multimedia controls to play our sounds
MMControl1.notify = False
MMControl1.Wait = True
MMControl1.Shareable = False
MMControl1.DeviceType = "WaveAudio"
MMControl1.filename = App.Path + "\signon.wav"
MMControl1.Command = "Open"

MMControl2.notify = False
MMControl2.Wait = True
MMControl2.Shareable = False
MMControl2.DeviceType = "WaveAudio"
MMControl2.filename = App.Path + "\signoff.wav"
MMControl2.Command = "Open"

Rem Dont except on errors with speech output..the user may have some other
Rem sounds playing.  Ideally we would catch the errors and do the notify later.
DirectSS1.SuppressExceptions = 1

Rem set the last mail recieved time to a date in the past.
LastRecieved = "1996/08/05 16:40"

Rem Our state machine startes at mode 0
gMode = 0
gStarted = 0
Timer1.Interval = POLLTIME

Rem Play the startup sound
MMControl1.Command = "Prev"
MMControl1.Command = "Play"
Rem control picks up at MMControl1_Done after the sound is played, perhaps
End Sub

Rem this function should only be called from the timer so it does not get reentered
Rem instead, to call this do  Timer1.Interval = 1
Private Sub CheckForNewMail()
Rem This function is called occassionly to check if new mail has arrived

    Dim SayString As String
          
    
Rem Just a bit of a precaution..be sure not to start get errors if we
Rem are called before the program is initialized.  This would only happen
Rem if somebody modified the example code..but why not be safe?
    If (gStarted <> 1) Then
        Exit Sub
    End If
    
   
Rem Mark that we are checking mail, so we won't re-enter the process
Rem while were doing it.  We need to do this because the sounds and
Rem spoken words are played assyncronously.

    Rem Get the new mail list..
    
    MAPIMessages1.FetchSorted = False 'sort the way the user has their mailbox laid out
    MAPIMessages1.Fetch
Rem If there is some mail..
    tCount = 0
again:
    If (MAPIMessages1.MsgCount) Then
Rem see if it is new mail, not just old mail marked as unread...
        MAPIMessages1.MsgIndex = MAPIMessages1.MsgCount - tCount - 1
        If ((MAPIMessages1.MsgDateReceived) >= (LastRecieved)) Then
Rem remember the time of this mail..we wont read mail way older than this
Rem from now on, even if it is marked unread.

            LastRecieved = MAPIMessages1.MsgDateReceived
Rem state change to reading mail..Play the sign-on sound
Rem control will go to MMControl1_Done when the sound is finished.
            gMode = 2
            MMControl1.Command = "Prev"
            MMControl1.Command = "Play"
        Else
          If (MAPIMessages1.MsgIndex = 0) Then
                Rem we didn't do anything, so we are finished
          Else
                tCount = tCount + 1
                GoTo again
          End If

        End If
    Else
Rem we didn't do anything, so we are finished
        
    End If
    

End Sub

Private Sub MMControl1_Done(NotifyCode As Integer)

If (gMode = 0) Then

    Rem if we are just starting up, speak our text and control goes to
    Rem DirectSS1_AudioStop after the text is spoken

    DirectSS1.Speak "Hi, welcome to the mail notify program. I will notify you about new email when it arrives in your inbox."
    
ElseIf (gMode = 2) Then

    Rem We just played the sign on sound..now speak the text. Control
    Rem goes to DirectSS1_AudioStop after the text is spoken
  
    SayString = "You have new mail from " + MAPIMessages1.MsgOrigDisplayName + ". Subject line," + MAPIMessages1.MsgSubject
    DirectSS1.Speak SayString
    
    If (readbody(1).Checked) Then
        Rem we try to skip forwards since reading them yet again can become tiresome
        bodystring = MAPIMessages1.MsgNoteText
        forwardsloc = InStr(bodystring, "-----Original Message-----")
        If (forwardsloc = 0) Then
            forwardsloc = InStr(bodystring, "From:")
        End If
        If (forwardsloc > 0) Then
            bodystring = Left(bodystring, forwardsloc - 1)
        End If
        SayString = "Message Body reads: " + bodystring
        If (forwardsloc > 0) Then
            SayString = SayString + " Skipping forwarded message body."
        End If
        DirectSS1.Speak SayString
    End If
    
End If

End Sub


Private Sub DirectSS1_AudioStop(ByVal hi As Long, ByVal lo As Long)
If (gMode = 0) Then
Rem We have just finished speaking our startup text, so now we play our
Rem sign off sound and start waiting for new mail
    
    gMode = 1
    
Rem Since we set our state to 1 nothing much happens when control goes to
Rem MMControl2_Done after the sound is played..
    MMControl2.Command = "Prev"
    MMControl2.Command = "Play"
        
Rem This call signs us on email..if the user is not already logged on to email,
Rem This call will prompt the user to do so.
    MAPISession1.SignOn
    MAPIMessages1.SessionID = MAPISession1.SessionID
Rem note that we are ready to start, and go ahead and check for mail right now.
    gStarted = 1
    Timer1.Interval = 1
Rem from now our timer will poll for new mail.
    Timer1.Enabled = True
ElseIf (gMode = 2) Then
Rem We have just finished reading the mail subject. Play sign off sound.
Rem Control goes to MMControl2_Done which will reset our mode/state once
Rem the sound is finished.

    MMControl2.Command = "Prev"
    MMControl2.Command = "Play"
    
End If

End Sub


Private Sub MMControl2_Done(NotifyCode As Integer)


If (gMode = 2) Then
Rem we have just played the sign off sound after reading a subject line.
Rem go back into ready mode
    gMode = 3
    Timer1.Interval = 1 'check for new mail now!
End If

End Sub



Private Sub readbody_Click(Index As Integer)
Rem always stop the current reading so you have a way of making the computer shut up
DirectSS1.AudioReset
readbody(1).Checked = Not readbody(1).Checked
End Sub

Private Sub stopread_Click(Index As Integer)
Rem always stop the current reading so you have a way of making the computer shut up
DirectSS1.AudioReset
End Sub


Private Sub Timer1_Timer()
Rem check for new mail every once in a while, according to how the
Rem timer objects interval property is set
Timer1.Interval = POLLTIME
CheckForNewMail
End Sub

'This function is for handling a click on the tray icon (code from MSDN)
'we just pop up a menu.
 Private Sub Form_MouseMove _
         (Button As Integer, _
          Shift As Integer, _
          x As Single, _
          y As Single)
          'Event occurs when the mouse pointer is within the rectangular
          'boundaries of the icon in the taskbar status area.
          Dim msg As Long
          Dim sFilter As String
          msg = x / Screen.TwipsPerPixelX
          Select Case msg
             Case WM_RBUTTONDOWN
               PopupMenu notify(1)
          End Select
      End Sub


