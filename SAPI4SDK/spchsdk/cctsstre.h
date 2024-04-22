/* TSStress.h - Header files to access list data */

#ifndef _TSSTRESS_H_
#define _TSSTRESS_H_


#define TEST_REQUIRED			0x0001
#define TEST_OPTIONAL			0x0002

#define MAXENGINECASES			26

typedef struct {	
	CHAR				*pszCaseName;
	DWORD				dwCaseType;
	BOOL				fRun;
} TESTCASE, *PTESTCASE;

TESTCASE gaEngineCases[] = {
	{ "pitchget", TEST_REQUIRED,TRUE},  					// case 0
   { "pitchset", TEST_REQUIRED,TRUE},						// 1
   { "realtimeget", TEST_REQUIRED,TRUE},              // 2
   { "realtimeset", TEST_REQUIRED,TRUE},              // 3
   { "speedget", TEST_REQUIRED,TRUE},						// 4
   { "speedset", TEST_REQUIRED,TRUE},						// 5
   { "volumeget", TEST_REQUIRED,TRUE},                // 6
   { "volumeset", TEST_REQUIRED,TRUE},                // 7
   { "AudioPause", TEST_REQUIRED,TRUE},               // 8
   { "AudioReset", TEST_REQUIRED,TRUE},               // 9
   { "AudioResume", TEST_REQUIRED,TRUE},              // 10
   { "Inject", TEST_REQUIRED,TRUE},                   // 11
   { "modeget", TEST_REQUIRED,TRUE},                  // 12
   { "phoneme", TEST_OPTIONAL,TRUE},                  // 13
   { "posnget", TEST_REQUIRED,TRUE},                  // 14
   { "register & unregister", TEST_REQUIRED,TRUE},    // 15
   { "textdata", TEST_REQUIRED,TRUE},                 // 16
   { "tofiletime", TEST_REQUIRED,TRUE},               // 17
   { "about dialog", TEST_REQUIRED,TRUE},             // 18
   { "general dialog", TEST_REQUIRED,TRUE},           // 19
   { "lexicon dialog", TEST_REQUIRED,TRUE},           // 20
   { "translate dialog", TEST_OPTIONAL,TRUE},         // 21
   { "ilexpronounce - add", TEST_OPTIONAL,TRUE},      // 22
   { "ilexpronounce - get", TEST_OPTIONAL,TRUE},      // 23
   { "ilexpronounce - remove", TEST_OPTIONAL,TRUE},   // 24
   { "wipe out wave file", TEST_REQUIRED,TRUE},       // 25
};



#endif // _TSSSTRESS_H_