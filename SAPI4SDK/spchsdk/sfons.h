/******************************************************************************\
*       Icons.h
*
*       Header file for the Windows 3.1 style feedback class
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

typedef struct tagSTRING
{
  char szCaption[64];
} STRING;

class CIcons
{
  public:
    CIcons(void);
    ~CIcons(void);
    void SetRecognizedCommand (PSTR);
    BOOL InitIcons (HINSTANCE);
    HICON m_Icons[6];
    STRING m_strCaption[6];
};
