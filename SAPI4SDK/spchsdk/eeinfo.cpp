/*****************************************************************
Info.cpp - Provides information for frame.cpp
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <dsound.h>
#include <spchwrap.h>
#include "frame.h"
#include "resource.h"

/* tests */
void CALLBACK Test000 (void);
void CALLBACK Test001 (void);
void CALLBACK Test002 (void);
void CALLBACK Test003 (void);
void CALLBACK Test004 (void);
void CALLBACK Test005 (void);
void CALLBACK Test006 (void);
void CALLBACK Test007 (void);
void CALLBACK Test008 (void);
void CALLBACK Test009 (void);
void CALLBACK Test010 (void);
void CALLBACK Test011 (void);
void CALLBACK Test012 (void);
void CALLBACK Test013 (void);
void CALLBACK Test014 (void);
void CALLBACK Test015 (void);
void CALLBACK Test016 (void);
void CALLBACK Test017 (void);
void CALLBACK Test018 (void);
void CALLBACK Test019 (void);
void CALLBACK Test020 (void);
void CALLBACK Test021 (void);
void CALLBACK Test022 (void);
void CALLBACK Test023 (void);
void CALLBACK Test024 (void);
void CALLBACK Test025 (void);
void CALLBACK Test026 (void);
void CALLBACK Test027 (void);
void CALLBACK Test028 (void);
void CALLBACK Test029 (void);
void CALLBACK Test030 (void);
void CALLBACK Test031 (void);
void CALLBACK Test032 (void);
void CALLBACK Test033 (void);
void CALLBACK Test034 (void);
void CALLBACK Test035 (void);
void CALLBACK Test036 (void);
void CALLBACK Test037 (void);
void CALLBACK Test038 (void);
void CALLBACK Test039 (void);
void CALLBACK Test040 (void);
void CALLBACK Test041 (void);
void CALLBACK Test042 (void);
void CALLBACK Test043 (void);
void CALLBACK Test044 (void);
void CALLBACK Test045 (void);
void CALLBACK Test046 (void);
void CALLBACK Test047 (void);
void CALLBACK Test048 (void);
void CALLBACK Test049 (void);
void CALLBACK Test050 (void);
void CALLBACK Test051 (void);
void CALLBACK Test052 (void);
void CALLBACK Test053 (void);
void CALLBACK Test054 (void);
void CALLBACK Test055 (void);
void CALLBACK Test056 (void);
void CALLBACK Test057 (void);
void CALLBACK Test058 (void);
void CALLBACK Test059 (void);
void CALLBACK Test060 (void);
void CALLBACK Test061 (void);
void CALLBACK Test062 (void);
void CALLBACK Test063 (void);
void CALLBACK Test064 (void);
void CALLBACK Test065 (void);
void CALLBACK Test066 (void);
void CALLBACK Test067 (void);
void CALLBACK Test068 (void);
void CALLBACK Test069 (void);
void CALLBACK Test070 (void);
void CALLBACK Test071 (void);
void CALLBACK Test072 (void);
void CALLBACK Test073 (void);
void CALLBACK Test074 (void);
void CALLBACK Test075 (void);
void CALLBACK Test076 (void);
void CALLBACK Test077 (void);
void CALLBACK Test078 (void);
void CALLBACK Test079 (void);
void CALLBACK Test080 (void);
void CALLBACK Test081 (void);
void CALLBACK Test082 (void);
void CALLBACK Test083 (void);
void CALLBACK Test084 (void);
void CALLBACK Test085 (void);
void CALLBACK Test086 (void);
void CALLBACK Test087 (void);
void CALLBACK Test088 (void);
void CALLBACK Test089 (void);
void CALLBACK Test090 (void);
void CALLBACK Test091 (void);
void CALLBACK Test092 (void);
void CALLBACK Test093 (void);
void CALLBACK Test094 (void);
void CALLBACK Test095 (void);
void CALLBACK Test096 (void);
void CALLBACK Test097 (void);
void CALLBACK Test098 (void);
void CALLBACK Test099 (void);
void CALLBACK Test100 (void);
void CALLBACK Test101 (void);
void CALLBACK Test102 (void);
void CALLBACK Test103 (void);
void CALLBACK Test104 (void);
void CALLBACK Test105 (void);
void CALLBACK Test106 (void);
void CALLBACK Test107 (void);
void CALLBACK Test108 (void);
void CALLBACK Test109 (void);
void CALLBACK Test110 (void);
void CALLBACK Test111 (void);
void CALLBACK Test112 (void);
void CALLBACK Test113 (void);
void CALLBACK Test114 (void);
void CALLBACK Test115 (void);
void CALLBACK Test116 (void);
void CALLBACK Test117 (void);
void CALLBACK Test118 (void);
void CALLBACK Test119 (void);
void CALLBACK Test120 (void);
void CALLBACK Test121 (void);
void CALLBACK Test122 (void);
void CALLBACK Test123 (void);
void CALLBACK Test124 (void);
void CALLBACK Test125 (void);
void CALLBACK Test126 (void);
void CALLBACK Test127 (void);
void CALLBACK Test128 (void);
void CALLBACK Test129 (void);
void CALLBACK Test130 (void);
void CALLBACK Test131 (void);
void CALLBACK Test132 (void);
void CALLBACK Test133 (void);
void CALLBACK Test134 (void);
void CALLBACK Test135 (void);
void CALLBACK Test136 (void);
void CALLBACK Test137 (void);
void CALLBACK Test138 (void);
void CALLBACK Test139 (void);
void CALLBACK Test140 (void);
void CALLBACK Test141 (void);
void CALLBACK Test142 (void);
void CALLBACK Test143 (void);
void CALLBACK Test144 (void);
void CALLBACK Test145 (void);
void CALLBACK Test146 (void);
void CALLBACK Test147 (void);
void CALLBACK Test148 (void);
void CALLBACK Test149 (void);
void CALLBACK Test150 (void);
void CALLBACK Test151 (void);
void CALLBACK Test152 (void);
void CALLBACK Test153 (void);
void CALLBACK Test154 (void);
void CALLBACK Test155 (void);
void CALLBACK Test156 (void);
void CALLBACK Test157 (void);
void CALLBACK Test158 (void);
void CALLBACK Test159 (void);
void CALLBACK Test160 (void);
void CALLBACK Test161 (void);
void CALLBACK Test162 (void);
void CALLBACK Test163 (void);
void CALLBACK Test164 (void);
void CALLBACK Test165 (void);
void CALLBACK Test166 (void);
void CALLBACK Test167 (void);
void CALLBACK Test168 (void);
void CALLBACK Test169 (void);
void CALLBACK Test170 (void);
void CALLBACK Test171 (void);
void CALLBACK Test172 (void);
void CALLBACK Test173 (void);
void CALLBACK Test174 (void);
void CALLBACK Test175 (void);
void CALLBACK Test176 (void);
void CALLBACK Test177 (void);
void CALLBACK Test178 (void);
void CALLBACK Test179 (void);
void CALLBACK Test180 (void);
void CALLBACK Test181 (void);
void CALLBACK Test182 (void);
void CALLBACK Test183 (void);
void CALLBACK Test184 (void);
void CALLBACK Test185 (void);
void CALLBACK Test186 (void);
void CALLBACK Test187 (void);
void CALLBACK Test188 (void);
void CALLBACK Test189 (void);
void CALLBACK Test190 (void);
void CALLBACK Test191 (void);
void CALLBACK Test192 (void);
void CALLBACK Test193 (void);
void CALLBACK Test194 (void);
void CALLBACK Test195 (void);
void CALLBACK Test196 (void);
void CALLBACK Test197 (void);
void CALLBACK Test198 (void);
void CALLBACK Test199 (void);


// globals
char gszAppName[] = "Text-to-Speech Functionality Test";
BOOL gfNeedAudioSREngine = TRUE;

FUNCTEST gaTest[] = {
   {
      "Mode Information",
      "This shows information about the mode, and quickly tests out some functionality.",
		TEST_REQUIRED,
      Test000
   },
   {
      "Route audio",
      "Tests routing audio from the text-to-speech engine to the speech recognizer.",
      TEST_REQUIRED,
      Test001
   },
   {
      "Check Mode info",
      "Look through the mode info and make sure parameters are valid",
      TEST_REQUIRED,
      Test002
   },
   {
      "Verify interfaces",
      "Make sure that interfaces claimed in TTSMODEINFO are supported.",
      TEST_REQUIRED,
      Test003
   },
   {
      "Phone Optimized",
      "If TTSMODEINFO has phone-optimized bit set, then make sure 8 kHz is supported.",
      TEST_REQUIRED,
      Test004
   },
   {
      "Check Enumerator",
      "Make sure the TTS enumeration is robust.",
      TEST_REQUIRED,
      Test005
   },
   {
      "Close & reload engine",
      "Close and reload the TTS engine to make sure it can.",
      TEST_REQUIRED,
      Test006
   },
   {
      "Close & reload engine while speaking",
      "Close and reload the TTS engine while speaking, to make sure it can.",
      TEST_REQUIRED,
      Test007
   },
   {
      "Audio Object Ref Counting",
      "Checks to make sure that the TTS engine properly releases the audio object when it's released.",
      TEST_REQUIRED,
      Test008
   },
   {
      "ITTSNotifySink Ref Counting",
      "Checks to make sure that the TTS engine properly releases the TTSNotifySink when register & unregister.",
      TEST_REQUIRED,
      Test009
   },
   {
      "ITTSNotifySink Ref Counting",
      "Reference counting on the main notification sinks, when register & close down.",
      TEST_REQUIRED,
      Test010
   },
   {
      "ITTSBufNotifySink Ref Counting",
      "Reference counting on the buffer notification sinks, when do audio-reset.",
      TEST_REQUIRED,
      Test011
   },
   {
      "ITTSBufNotifySink Ref Counting",
      "Reference counting on the buffer notification sinks, when let audio play through.",
      TEST_REQUIRED,
      Test012
   },
   {
      "Ref Counting on shut down",
      "Make sure when shut down, all reference counting is at 0.",
      TEST_REQUIRED,
      Test013
   },
   {
      "ITTSNotifySink not called after unregister",
      "Tests to see if ITTSNotifySink is called after UnRegister is called.",
      TEST_REQUIRED,
      Test014
   },
   {
      "ITTSNotifySink & ITTSBufNotySink not called after shut down",
      "Tests to see if ITTSNotifySink or ITTSBufNotifySink are called after shut down.",
      TEST_REQUIRED,
      Test015
   },
   {
      "Close audio at right times",
      "Makes sure that the engine closes the audio device down when the audio isn't in use.",
      TEST_REQUIRED,
      Test016
   },
   {
      "Multiple ISRNotifySink",
      "Test multiple ISRNotifySink and make sure they're both called.",
      TEST_REQUIRED,
      Test017
   },
   {
      "Support 22 kHz",
      "PC enable TTS should support 22 kHz 16-bit for best results with DirectSound.",
      TEST_REQUIRED,
      Test018
   },
   {
      "Support TTSFEATURE_ANYWORD",
      "Test claims of support for TTSFEATURE_ANYWORD and see if they're true.",
      TEST_REQUIRED,
      Test019
   },
   {
      "Support TTSFEATURE_VOLUME",
      "Test claims of support for TTSFEATURE_VOLUME and see if they're true.",
      TEST_REQUIRED,
      Test020
   },
   {
      "Support TTSFEATURE_SPEED",
      "Test claims of support for TTSFEATURE_SPEED and see if they're true.",
      TEST_REQUIRED,
      Test021
   },
   {
      "Support TTSFEATURE_PITCH",
      "Test claims of support for TTSFEATURE_PITCH and see if they're true.",
      TEST_REQUIRED,
      Test022
   },
   {
      "Order of notification messages",
      "Make sure notifications come in the right order.",
      TEST_REQUIRED,
      Test023
   },
   {
      "Support TTSFEATURE_VISUAL",
      "Test claims of support for TTSFEATURE_VISUAL and see if they're true.",
      TEST_REQUIRED,
      Test024
   },
   {
      "Support TTSFEATURE_WORDPOSITION",
      "Test claims of support for TTSFEATURE_WORDPOSITION and see if they're true.",
      TEST_REQUIRED,
      Test025
   },
   {
      "1 second pause",
      "Send down a 1 second pause and see if it's spoken properly.",
      TEST_REQUIRED,
      Test026
   },
   {
      "10 second pause between words",
      "Send down a 10 second pause between words, and see if it's spoken properly.",
      TEST_REQUIRED,
      Test027
   },
   {
      "10 second pause after words",
      "Send down a 10 second pause after words, and see if it's spoken properly.",
      TEST_REQUIRED,
      Test028
   },
   {
      "10 second pause before words",
      "Send down a 10 second pause before words, and see if it's spoken properly.",
      TEST_REQUIRED,
      Test029
   },
   {
      "Bookmarks in text",
      "Place bookmarks at different points in the text and see if they're hit.",
      TEST_REQUIRED,
      Test030
   },
   {
      "Speak empty string",
      "Send in an empty string and make sure they succeed.",
      TEST_REQUIRED,
      Test031
   },
   {
      "Speak with a bad character",
      "Make sure that a bad Unicode character doesn't cause speaking to abort.",
      TEST_REQUIRED,
      Test032
   },
   {
      "Bookmarks",
      "Test a series of bookmarks and make sure they're returned in the right order.",
      TEST_REQUIRED,
      Test033
   },
   {
      "Common text normalization",
      "Common text normalization.",
      TEST_REQUIRED,
      Test034
   },
   {
      "Bookmark tag",
      "Make sure a bookmark tag doesn't change the audio.",
      TEST_REQUIRED,
      Test035
   },
   {
      "Comment tag",
      "Make sure a comment tag at the end doesn't change the audio.",
      TEST_REQUIRED,
      Test036
   },
   {
      "Junk tag",
      "Make sure a junk tag doesn't change the audio.",
      TEST_REQUIRED,
      Test037
   },
   {
      "Invalid bookmark",
      "Make sure an invalid bookmark is ignored.",
      TEST_REQUIRED,
      Test038
   },
   {
      "Invalid pause",
      "Make sure an invalid pause doesn't change the duration.",
      TEST_REQUIRED,
      Test039
   },
   {
      "Pitch tag",
      "Make sure that the pitch tag functions properly.",
      TEST_REQUIRED,
      Test040
   },
   {
      "Pitch tag",
      "Make sure that the pitch tag ignores bogus values.",
      TEST_REQUIRED,
      Test041
   },
   {
      "Volume tag",
      "Make sure that the volume tag functions properly.",
      TEST_REQUIRED,
      Test042
   },
   {
      "Volume tag",
      "Make sure that the volume tag ignores bogus values.",
      TEST_REQUIRED,
      Test043
   },
   {
      "Speed tag",
      "Make sure that the speed tag functions properly.",
      TEST_REQUIRED,
      Test044
   },
   {
      "Speed tag",
      "Make sure that the speed tag ignores bogus values.",
      TEST_REQUIRED,
      Test045
   },
   {
      "Speed tag",
      "Make sure that the speed tag functions within a sentence.",
      TEST_REQUIRED,
      Test046
   },
   {
      "Backslash in tagged text",
      "Make sure a backslash is spoken in tagged text.",
      TEST_REQUIRED,
      Test047
   },
   {
      "Pitch get/set",
      "Make sure it remembers the value and sends an attribute changed.",
      TEST_REQUIRED,
      Test048
   },
   {
      "Pitch get/set",
      "Test minimum and maximum.",
      TEST_REQUIRED,
      Test049
   },
   {
      "Pitch get/set",
      "Invalid parameters.",
      TEST_REQUIRED,
      Test050
   },
   {
      "Speed get/set",
      "Make sure it remembers the value and sends an attribute changed.",
      TEST_REQUIRED,
      Test051
   },
   {
      "Speed get/set",
      "Test minimum and maximum.",
      TEST_REQUIRED,
      Test052
   },
   {
      "Speed get/set",
      "Invalid parameters.",
      TEST_REQUIRED,
      Test053
   },
   {
      "Volume get/set",
      "Make sure it remembers the value and sends an attribute changed.",
      TEST_REQUIRED,
      Test054
   },
   {
      "Volume get/set",
      "Test minimum and maximum.",
      TEST_REQUIRED,
      Test055
   },
   {
      "Volume get/set",
      "Invalid parameters.",
      TEST_REQUIRED,
      Test056
   },
   {
      "RealTime get/set",
      "Make sure it remembers the value and sends an attribute changed.",
      TEST_REQUIRED,
      Test057
   },
   {
      "RealTime get/set",
      "Invalid parameters.",
      TEST_OPTIONAL,
      Test058
   },
   {
      "SpeedSet",
      "Test that the speaking rates are valid.",
      TEST_REQUIRED,
      Test059
   },
   {
      "AudioReset",
      "Make sure that if there's more than one buffer left, that audio reset behaves properly.",
      TEST_REQUIRED,
      Test060
   },
   {
      "Inject",
      "Make sure that inject tag is working.",
      TEST_REQUIRED,
      Test061
   },
   {
      "Inject",
      "Invalid parameter.",
      TEST_REQUIRED,
      Test062
   },
   {
      "PosnGet & ToFileTime",
      "Make sure PosnGet and ToFileTime are returning the same parameters as the audio object.",
      TEST_REQUIRED,
      Test063
   },
   {
      "Register",
      "Invalid parameters.",
      TEST_REQUIRED,
      Test064
   },
   {
      "TextData",
      "Invalid parameters.",
      TEST_REQUIRED,
      Test065
   },
   {
      "Phoneme",
      "Invalid parameters.",
      TEST_OPTIONAL,
      Test066
   },
   {
      "Dialogs",
      "Make sure the dialogs exist.",
      TEST_REQUIRED,
      Test067
   },
   {
      "\\Rst\\ tag",
      "Make sure that the rst tag restores the pitch, speed, and volume values.",
      TEST_REQUIRED,
      Test068
   },
   {
      "Padding silence",
      "Make sure there isn't any padding silence at the beginning/end of the audio output.",
      TEST_REQUIRED,
      Test069
   },
   {
      "Invalid audio",
      "Test to see if the engine loads when there are no acceptable wave formats.",
      TEST_REQUIRED,
      Test070
   },
   {
      "AudioReset",
      "Make sure BookMarks are not returned after a call to AudioReset.",
      TEST_REQUIRED,
      Test071
   },
   {
      "Word Position",
      "Test that word position is supported.",
      TEST_REQUIRED,
      Test072
   },
   {
      "Visual Notification",
      "Make sure visual gets called, and make sure it finishes with a mouth closed.",
      TEST_REQUIRED,
      Test073
   },
   {
      "Speak phonemes",
      "Speak phonemes from text to speech and make sure they're understandable.",
      TEST_REQUIRED,
      Test074
   },
   {
      "Acoustic score",
      "Use a speech recognizer to generate the acoustic score for a text-to-speech voice. It might show off quality problems.",
      TEST_REQUIRED,
      Test075
   },
	{
      "ILexPronounce: Get pronunciations - known valid word",
      "Make sure Get can retrieve the pronunciation of a word known to be in the lex",
		TEST_OPTIONAL,
      Test076
   },
	{
      "ILexPronounce: Get pronunciations - known invalid word",
      "Make sure Get returns a pronunciation for word that is not in the lex",
		TEST_OPTIONAL,
      Test077
   },
	{
      "ILexPronounce: Get pronunciations - known valid word, return buffer too small",
      "Make sure Get returns an error that buffer too small when it is.",
		TEST_OPTIONAL,
      Test078
   },
	{
      "ILexPronounce: Add pronunciations - known valid word",
      "Make sure can add a valid word",
		TEST_OPTIONAL,
      Test079
   },
	{
      "ILexPronounce: Add pronunciations - known invalid word",
      "Make sure cannot add a word already in the lex",
		TEST_OPTIONAL,
      Test080
   },
	{
      "ILexPronounce: Add pronunciations - known valid word, get it",
      "Make sure can get added word",
		TEST_OPTIONAL,
      Test081
   },
	{
		"Emphasis in tagged text",
		"Make sure handles various tag",
		TEST_REQUIRED,
		Test082
	},
	{
		"IAttribute - DWORDSet/Get TTSATTR_THREADPRIORITY",
		"Make sure handles setting and getting thread priority",
		TEST_OPTIONAL,
		Test083
	},
	{
		"IAttribute - DWORDGet",
		"Make sure handles DWORDGet correctly",
		TEST_OPTIONAL,
		Test084
	},
	{
		"IAttribute - DWORDSet",
		"Make sure handles DWORDSet correctly",
		TEST_OPTIONAL,
		Test085
	},
	{
		"IAttribute - StringGet",
		"Make sure handles StringGet correctly",
		TEST_OPTIONAL,
		Test086
	},
	{
		"IAttribute - StringSet",
		"Make sure handles StringSet correctly",
		TEST_OPTIONAL,
		Test087
	},
	{
		"IAttribute - MemoryGet",
		"Make sure handles MemoryGet correctly",
		TEST_OPTIONAL,
		Test088
	},
	{
		"IAttribute - MemorySet",
		"Make sure handles MemorySet correctly",
		TEST_OPTIONAL,
		Test089
	},
	{
		"IAttribute - DWORDSet/Get TTSATTR_PITCH",
		"Make sure handles attribute pitch get/set correctly",
		TEST_OPTIONAL,
		Test090		
	},
	{
		"IAttribute - DWORDSet/Get TTSATTR_SPEED",
		"Make sure handles attribute speed get/set correctly",
		TEST_OPTIONAL,
		Test091
	},
	{
		"IAttribute - DWORDSet/Get TTSATTR_VOLUME",
		"Make sure handles attribute volume get/set correctly",
		TEST_OPTIONAL,
		Test092
	},
	{
      "ILexPronounce2: GetFrom pronunciations - known valid word",
      "Make sure GetFrom retrieve the pronunciation of a word known to be in the lex",
		TEST_OPTIONAL,
      Test093
   },
	{
      "ILexPronounce2: GetFrom pronunciations - known invalid word",
      "Make sure GetFrom returns a pronunciation for word that is not in the lex",
		TEST_OPTIONAL,
      Test094
   },
	{
      "ILexPronounce2: GetFrom pronunciations - known valid word, return buffer too small",
      "Make sure GetFrom returns an error that buffer too small when it is.",
		TEST_OPTIONAL,
      Test095
   },
	{
      "ILexPronounce2: AddTo pronunciations - attempt to AddTo and RemoveFrom LTS",
      "Make sure cannot add a word or remove from the LTS lexicon",
		TEST_OPTIONAL,
      Test096
   },
	{
      "ILexPronounce2: AddTo pronunciations - known valid word",
      "Make sure can add a valid word to all writable lexicons",
		TEST_OPTIONAL,
      Test097
   },
	{
      "ILexPronounce2: AddTo pronunciations - known invalid word, duplicate word",
      "Make sure cannot add a word already in the lex",
		TEST_OPTIONAL,
      Test098
   },
	{
      "ILexPronounce2: AddTo pronunciations - known valid word, get it",
      "Make sure can get added word",
		TEST_OPTIONAL,
      Test099
   },
	{
		"ILexPronounce2: AddTo pronunciations - invalid param",
		"Make sure returns error with invalid parameter",
		TEST_OPTIONAL,
		Test100
	},
	{
		"ILexPronounce2: QueryLexicons - verify all writable lex's are read lex's",
		"Make sure query lexicon returns read and write lexicons and that all writeable lexicons are readable",
		TEST_OPTIONAL,
		Test101
	},
	{
		"ILexPronounce2: ChangeSpelling - verify not applicable for TTS",
		"Make sure returns error when ChangeSpelling to LEXERR_NOTAPPLICABLE",
		TEST_OPTIONAL,
		Test102
	},
   {
      "RealTime get/set",
      "Make sure can set to default values",
      TEST_OPTIONAL,
      Test103
   },
   {
      "Phoneme",
      "Valid parameters.",
      TEST_OPTIONAL,
      Test104
   }
		

};

DWORD gdwTestElems = sizeof(gaTest) / sizeof(FUNCTEST);