/* srtress.h - Header files to access test list data */

#ifndef _SRSTRESS_H_
#define _SRSTRESS_H_


#define TEST_REQUIRED			0x0001
#define TEST_OPTIONAL			0x0002

#define MAXENGINECASES			44
#define MAXGRAMMARCASES			26
#define MAXRESULTCASES			29

typedef struct {	
	CHAR				*pszCaseName;
	DWORD				dwCaseType;
	BOOL				fRun;
} TESTCASE, *PTESTCASE;

TESTCASE gaEngineCases[] = {
	{ "autogain enable get", TEST_OPTIONAL,TRUE},  			// case 0
   { "autgainenableset", TEST_OPTIONAL,TRUE},				// 1
   { "echoget", TEST_OPTIONAL,TRUE},                     // 2
   { "echoset", TEST_OPTIONAL,TRUE},                     // 3
   { "energy floorget", TEST_OPTIONAL,TRUE},             // 4
   { "energy floor set", TEST_OPTIONAL,TRUE},            // 5
   { "microphoneget", TEST_OPTIONAL,TRUE},                                    // 6
   { "microphone set", TEST_OPTIONAL,TRUE},                                    // 7
   { "realtimeget", TEST_OPTIONAL,TRUE},                                    // 8
   { "realtimeset", TEST_OPTIONAL,TRUE},                                    // 9
   { "speakerget", TEST_REQUIRED,TRUE},                                    // 10
   { "speakerset", TEST_REQUIRED,TRUE},                                    // 11
   { "thresholdget", TEST_REQUIRED,TRUE},                                    // 12
   { "threadholdset", TEST_REQUIRED,TRUE},                                    // 13
   { "timeoutget", TEST_REQUIRED,TRUE},                                    // 14
   { "timeoutset", TEST_REQUIRED,TRUE},                                    // 15
   { "modeget", TEST_REQUIRED,TRUE},                                    // 16
   { "pause", TEST_REQUIRED,TRUE},                                    // 17
   { "resume", TEST_REQUIRED,TRUE},                                    // 18
   { "posnget", TEST_REQUIRED,TRUE},                                    // 19
   { "register & unregister", TEST_REQUIRED,TRUE},                                    // 20
   { "tofiletime", TEST_REQUIRED,TRUE},                                    // 21
   { "about dialog", TEST_REQUIRED,TRUE},                                    // 22
   { "general dialog", TEST_REQUIRED,TRUE},                                    // 23
   { "lexicon dialog", TEST_REQUIRED,TRUE},                                    // 24
   { "traingeneral dialog", TEST_REQUIRED,TRUE},                                    // 25
   { "trainmic dialog", TEST_OPTIONAL,TRUE},                                    // 26
   { "speaker - delete", TEST_REQUIRED,TRUE},                                    // 27
   { "speaker - enum", TEST_REQUIRED,TRUE},                                    // 28
   { "speaker - merge", TEST_REQUIRED,TRUE},                                    // 29
   { "speaker - new", TEST_REQUIRED,TRUE},                                    // 30
   { "speaker - query", TEST_REQUIRED,TRUE},                                    // 31
   { "speaker - read", TEST_REQUIRED,TRUE},                                    // 32
   { "speaker - revert", TEST_REQUIRED,TRUE},                                    // 33
   { "speaker - select", TEST_REQUIRED,TRUE},                                    // 34
   { "speaker - write", TEST_REQUIRED,TRUE},                                    // 35
   { "ilexpronounce - add", TEST_OPTIONAL,TRUE},                                    // 36
   { "ilexpronounce - get", TEST_OPTIONAL,TRUE},                                    // 37
   { "ilexpronounce - remove", TEST_OPTIONAL,TRUE},                                    // 38 
   { "ilexpronounce2 - getfrom", TEST_OPTIONAL,TRUE},                                    // 39
   { "ilexpronounce2 - addto", TEST_OPTIONAL,TRUE},                                    // 40
   { "ilexpronounce2 - removefrom", TEST_OPTIONAL,TRUE},                                    // 41
   { "ilexpronounce2 - changespelling", TEST_OPTIONAL,TRUE},                                    // 42
   { "ilexpronounce2 - querylexicons", TEST_OPTIONAL,TRUE},                                    // 43
};

TESTCASE gaGrammarCases[] = {
   { "Activate", TEST_REQUIRED,TRUE},                                    // 0
   { "Archive", TEST_REQUIRED,TRUE},                                    // 1
   { "BookMark", TEST_OPTIONAL,TRUE},                                    // 2
   { "Deactivate", TEST_REQUIRED,TRUE},                                    // 3
   { "DeteriorationGet", TEST_OPTIONAL,TRUE},                                    // 4
   { "DeteriorationSet", TEST_OPTIONAL,TRUE},                                    // 5
   { "TrainDlg", TEST_OPTIONAL,TRUE},                                    // 6
   { "TrainPhrase", TEST_OPTIONAL,TRUE},                                    // 7
   { "TrainQuery", TEST_REQUIRED,TRUE},                                    // 8
   { "LinkQuery", TEST_OPTIONAL,TRUE},                                    // 9
   { "ListAppend", TEST_REQUIRED,TRUE},                                    // 10
   { "ListGet", TEST_REQUIRED,TRUE},                                    // 11
   { "ListQuery", TEST_REQUIRED,TRUE},                                    // 12
   { "ListRemove", TEST_REQUIRED,TRUE},                                    // 13
   { "ListSet", TEST_REQUIRED,TRUE},                                    // 14
   { "Context", TEST_OPTIONAL,TRUE},                                    // 15
   { "Hint", TEST_OPTIONAL,TRUE},                                    // 16
   { "Words", TEST_OPTIONAL,TRUE},                                    // 17
   { "Hide", TEST_OPTIONAL,TRUE},                                    // 18
   { "Move", TEST_OPTIONAL,TRUE},                                    // 19
   { "Show", TEST_OPTIONAL,TRUE},                                    // 20
   { "ilexpronounce2 - getfrom", TEST_OPTIONAL,TRUE},                                    // 21
   { "ilexpronounce2 - addto", TEST_OPTIONAL,TRUE},                                    // 22
   { "ilexpronounce2 - removefrom", TEST_OPTIONAL,TRUE},                                    // 23
   { "ilexpronounce2 - changespelling", TEST_OPTIONAL,TRUE},                                    // 24
   { "ilexpronounce2 - querylexicons", TEST_OPTIONAL,TRUE},                                    // 25
};

TESTCASE gaResultCases[] = {
   { "GetWAV",						TEST_REQUIRED,TRUE},                                   // case 0
   { "FlagsGet",					 TEST_REQUIRED,TRUE},                                   // 1
   { "Identify",					 TEST_REQUIRED,TRUE},                                    // 2
   { "PhraseGet",					 TEST_REQUIRED,TRUE},                                    // 3
   { "TimeGet",					 TEST_REQUIRED,TRUE},                                    // 4
   { "Correction",				TEST_REQUIRED,TRUE},                                    // 5 
   { "Validate",					 TEST_OPTIONAL,TRUE},                                    // 6 
   { "ReEvaluate",					TEST_OPTIONAL,TRUE},                                    // 7 
   { "BestPathPhoneme",				 TEST_OPTIONAL,TRUE},                                    // 8 
   { "BestPathWord",				 TEST_OPTIONAL,TRUE},                                    // 9 
   { "GetPhonemeNode",				TEST_OPTIONAL,TRUE},                                    // 10 
   { "GetWordNode",					TEST_OPTIONAL,TRUE},                                    // 11 
   { "PathScorePhoneme",		 TEST_OPTIONAL,TRUE},                                    // 12 
   { "PathScoreWord",				 TEST_OPTIONAL,TRUE},                                    // 13 
   { "Free",						TEST_REQUIRED,TRUE},                                    // 14
   { "Get",						TEST_REQUIRED,TRUE},                                    // 15
   { "LockGet",				 TEST_REQUIRED,TRUE},                                    // 16
   { "LockSet",					 TEST_REQUIRED,TRUE},                                    // 17
   { "Merge",						 TEST_OPTIONAL,TRUE},                                    // 18
   { "Split",						 TEST_OPTIONAL,TRUE},                                    // 19
   { "Hide",						TEST_OPTIONAL,TRUE},                                    // 20
	{ "Move",						 TEST_OPTIONAL,TRUE},                                    // 21
   { "Show",						 TEST_OPTIONAL,TRUE},                                    // 22
   { "GetPhraseScore",			 TEST_REQUIRED,TRUE},                                    // 23
   { "GetWordScores",				TEST_REQUIRED,TRUE},                                    // 24
   { "Speaker - Correction",			TEST_OPTIONAL,TRUE},                                    // 25
   { "Speaker - Identify",		 TEST_OPTIONAL,TRUE},                                    // 26
   { "Speaker - IdentifyForFree", TEST_OPTIONAL,TRUE},                                    // 27
   { "Speaker - Validate",		 TEST_OPTIONAL,TRUE},                                    // 28
};




#endif // _SRSTRESS_H_