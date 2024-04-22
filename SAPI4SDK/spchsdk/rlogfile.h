/***********************************************************************
LogFile.h - Header for logfile object.

*/

#ifndef _LOGFILE_H_
#define _LOGFILE_H_

// Reads in a log file, parses it, and provides access to its members
class CLogFile {
private:
	CList		m_list;	// list

public:
	CLogFile (void);
	~CLogFile (void);

   BOOL     FileOpen (char *pszFile);
	DWORD	   GetNumElems(void);
   PCWSTR   GetSource (DWORD dwNum);
   PCWSTR   GetMessage (DWORD dwNum);
   WORD     GetDetail (DWORD dwNum);
   PVOID    GetData (DWORD dwNum, DWORD *pdwSize);
   BOOL     GetFileTime (DWORD dwNum, FILETIME *pft);
   BOOL     MakeWave (DWORD dwCur, char *pszFile);

};
typedef CLogFile * PCLogFile;

#endif // _LOGFILE_H_
