/******************************************************************************\
*       Tray.h
*
*       Header file for the tray feedback class
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/


//This is a function pointer of the same type as Shell_NotifyIcon, the 
//Windows 95 API that is used to update the icons in the Tray.

typedef BOOL (WINAPI *FPNOTIFYICON)(DWORD dwMessage, PNOTIFYICONDATA lpData);

class CTrayFeedBack
{
  public:
    CTrayFeedBack();
    ~CTrayFeedBack();
    BOOL InitTray (HINSTANCE, HWND);
    BOOL SetIcon (int);
    void SetRecognizedCommand (PSTR);
    BOOL Free ();
  private:
   	FPNOTIFYICON fpNotifyIcon;
   	NOTIFYICONDATA m_IconStr[NUMSTATES]; //array of icon data
  	HINSTANCE m_hShell32;
   	BOOL m_bUseTray;
   	UINT m_ErrorMode;
   	int m_loop;  
};
