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
void CALLBACK Test200 (void);
void CALLBACK Test201 (void);
void CALLBACK Test202 (void);
void CALLBACK Test203 (void);
void CALLBACK Test204 (void);
void CALLBACK Test205 (void);
void CALLBACK Test206 (void);
void CALLBACK Test207 (void);
void CALLBACK Test208 (void);
void CALLBACK Test209 (void);
void CALLBACK Test210 (void);
void CALLBACK Test211 (void);
void CALLBACK Test212 (void);
void CALLBACK Test213 (void);
void CALLBACK Test214 (void);
void CALLBACK Test215 (void);
void CALLBACK Test216 (void);
void CALLBACK Test217 (void);
void CALLBACK Test218 (void);
void CALLBACK Test219 (void);
void CALLBACK Test220 (void);
void CALLBACK Test221 (void);
void CALLBACK Test222 (void);
void CALLBACK Test223 (void);
void CALLBACK Test224 (void);
void CALLBACK Test225 (void);
void CALLBACK Test226 (void);
void CALLBACK Test227 (void);
void CALLBACK Test228 (void);
void CALLBACK Test229 (void);
void CALLBACK Test230 (void);
void CALLBACK Test231 (void);
void CALLBACK Test232 (void);
void CALLBACK Test233 (void);
void CALLBACK Test234 (void);
void CALLBACK Test235 (void);
void CALLBACK Test236 (void);
void CALLBACK Test237 (void);
void CALLBACK Test238 (void);
void CALLBACK Test239 (void);
void CALLBACK Test240 (void);
void CALLBACK Test241 (void);
void CALLBACK Test242 (void);
void CALLBACK Test243 (void);
void CALLBACK Test244 (void);
void CALLBACK Test245 (void);
void CALLBACK Test246 (void);
void CALLBACK Test247 (void);
void CALLBACK Test248 (void);
void CALLBACK Test249 (void);
void CALLBACK Test250 (void);


// globals
char gszAppName[] = "Speech Recognition Functionality Test";
BOOL gfNeedAudioSREngine = FALSE;

FUNCTEST gaTest[] = {
   {
      "Mode Information",
      "This shows information about the mode, and quickly tests out some functionality.",
		TEST_REQUIRED,
		Test000
	  },
   {
      "Route audio to the recognizer",
      "Make sure that we can route audio to the speech recognizer from the text-to-speech engine.",
		TEST_REQUIRED,
      Test001
   },
   {
      "Make sure that necessary SRMODEINFO fields are set",
      "Make sure that necessary fields in SRMODEINFO are set.",
		TEST_REQUIRED,
      Test002
   },
   {
      "Verify interfaces",
      "Make sure that interfaces claimed in SRMODEINFO are supported.",
		TEST_REQUIRED,
      Test003
   },
   {
      "Verify training supported",
      "Make sure that if training is claimed in SRMODEINFO, it's supported in the dialogs.",
		TEST_REQUIRED,
      Test004
   },
   {
      "Verify word spotting supported",
      "Make sure that if word spotting is claimed in SRMODEINFO, it's supported.",
		TEST_REQUIRED,
      Test005
   },
   {
      "Verify SRFEATURE_ANYWORD",
      "Make sure that the recognizer can recognize arbitrary non-lexicon words, if it claims.",
		TEST_REQUIRED,
      Test006
   },
   {
      "Verify 16-bit 8 kHz mono",
      "Make sure phone-optimized recognizers support 16-bit 8 kHz mono as their first choice.",
		TEST_REQUIRED,
      Test007
   },
   {
      "Verify SRFEATURE_LISTS",
      "Make sure that the recognizer supports lists if it claims to.",
		TEST_REQUIRED,
      Test008
   },
   {
      "Verify enumerator consistency",
      "Make sure that the enumerator returns consistent results.",
		TEST_REQUIRED,
      Test009
   },
   {
      "Destroy and re-create the SR engine",
      "Make sure that we can destroy and recreate the SR engine.",
		TEST_REQUIRED,
      Test010
   },
   {
      "Destroy and re-create the SR engine while it's listening",
      "Make sure that we can destroy and recreate the SR engine while it's listening.",
		TEST_REQUIRED,
      Test011
   },
   {
      "RefCount on Audio",
      "Test the reference counting on the audio object.",
		TEST_REQUIRED,
      Test012
   },
   {
      "RefCount on ISRNotifySink",
      "Test the reference counting on the main SR notify sink.",
		TEST_REQUIRED,
      Test013
   },
   {
      "RefCount on ISRNotifySink when shut down",
      "Test the reference counting on the main SR notify sink.",
		TEST_REQUIRED,
      Test014
   },
   {
      "RefCount on ISRGramNotifySink",
      "Test the reference counting on the grammar SR notify sink.",
		TEST_REQUIRED,
      Test015
   },
   {
      "RefCount on ISRGramNotifySink when shut down SR",
      "Test the reference counting on the grammar SR notify sink.",
		TEST_REQUIRED,
      Test016
   },
   {
      "ISRNotifySink not called after UnRegistered",
      "Make sure ISRNotifySink is not called after it's unregistered..",
		TEST_REQUIRED,
      Test017
   },
   {
      "ISRNotifySink & ISRGramNotifySink not called after shut down",
      "Make sure ISRNotifySink & ISRGramNotifySink are not called after a shut down of the engine.",
		TEST_REQUIRED,
      Test018
   },
   {
      "ISRGramNotifySink not called after grammar shut down",
      "Make sure ISRGramNotifySink is not called after a grammar object is released.",
		TEST_REQUIRED,
      Test019
   },
   {
      "Test audio device usage",
      "Make sure that the audio object is only open when a grammar is active.",
		TEST_REQUIRED,
      Test020
   },
   {
      "Multiple ISRNotifySink",
      "Make sure that notifications go to more than one ISRNotifySink.",
		TEST_REQUIRED,
      Test021
   },
   {
      "Work with DirectSound",
      "Engines that with to work well with DirectSound should support 22 kHz",
      TEST_REQUIRED,
		Test022
   },
   {
      "Notification sync order",
      "Make sure get utterance begin/end, and phrase start/finish in the right order.",
      TEST_REQUIRED,
		Test023
   },
   {
      "CFG with NULL window",
      "Test to make sure that CFGs with NULL windows work",
		TEST_REQUIRED,
      Test024
   },
   {
      "CFG with app window",
      "Test to make sure that CFG works with the app's window.",
		TEST_REQUIRED,
      Test025
   },
   {
      "CFG with invalid window",
      "Test to make sure that CFG works with the invalid window.",
		TEST_REQUIRED,
      Test026
   },
   {
      "Dictation with NULL window",
      "Test to make sure that dictation grammars with NULL windows work",
		TEST_REQUIRED,
      Test027
   },
   {
      "Dictation with app window",
      "Test to make sure that dictation grammars works with the app's window.",
		TEST_REQUIRED,
      Test028
   },
   {
      "Dictation with invalid window",
      "Test to make sure that dictation grammars works with the invalid window.",
		TEST_REQUIRED,
      Test029
   },
   {
      "CFG & Dictation with NULL window",
      "Test to make sure that priority scheme between CFG and dictation works. Expect CFG to get result.",
		TEST_REQUIRED,
      Test030
   },
   {
      "CFG & Dictation with NULL window",
      "Test to make sure that priority scheme between CFG and dictation works. Expect Dictation to get result.",
		TEST_REQUIRED,
      Test031
   },
   {
      "CFG & Dictation with app window",
      "Test to make sure that priority scheme between CFG and dictation works. Expect CFG to get result.",
		TEST_REQUIRED,
      Test032
   },
   {
      "CFG & Dictation with app window",
      "Test to make sure that priority scheme between CFG and dictation works. Expect Dictation to get result.",
		TEST_REQUIRED,
      Test033
   },
   {
      "CFG with app window, Dictation with NULL window",
      "Test to make sure that priority scheme between CFG and dictation works. Expect CFG to get result.",
		TEST_REQUIRED,
      Test034
   },
   {
      "CFG with app window, Dictation with NULL window",
      "Test to make sure that priority scheme between CFG and dictation works. Expect Dictation to get result.",
		TEST_REQUIRED,
      Test035
   },
   {
      "CFG with app window, Dictation with NULL window, other window with focus",
      "Test to make sure that priority scheme between CFG and dictation works. Expect Dictation to get result.",
		TEST_REQUIRED,
      Test036
   },
   {
      "CFG (app-window), identical CFG (NULL window), app window with focus",
      "Test to make sure that priority scheme between CFGs work. Expect app CFG to get result.",
		TEST_REQUIRED,
      Test037
   },
   {
      "CFG (app-window), identical CFG (NULL window), NULL window with focus",
      "Test to make sure that priority scheme between CFGs work. Expect NULL CFG to get result.",
		TEST_REQUIRED,
      Test038
   },
   {
      "CFG (app-window), overlapping CFG (NULL window), App window with focus",
      "Test to make sure that priority scheme between CFGs work. Expect App CFG to get result.",
		TEST_REQUIRED,
      Test039
   },
   {
      "CFG (app-window), overlapping CFG (NULL window), NULL window with focus",
      "Test to make sure that priority scheme between CFGs work. Expect NULL CFG to get result.",
		TEST_REQUIRED,
      Test040
   },
   {
      "CFG (app-window), overlapping CFG (NULL window), App window with focus",
      "Test to make sure that priority scheme between CFGs work. Expect NULL CFG to get result.",
		TEST_REQUIRED,
      Test041
   },
   {
      "Dictation (app-window), Dictation (NULL window), App window with focus",
      "Test to make sure that priority scheme between Dictation grammars work. Expect App Dictation to get result.",
		TEST_REQUIRED,
      Test042
   },
   {
      "Dictation (app-window), Dictation (NULL window), random window with focus",
      "Test to make sure that priority scheme between Dictation grammars work. Expect NULL Dictation to get result.",
		TEST_REQUIRED,
      Test043
   },
   {
      "Activate one CFG after another",
      "Test to make sure that priority scheme between CFG grammars work, and that activation one after the other works.",
		TEST_REQUIRED,
      Test044
   },
   {
      "Activate two CFGs, and switch windows",
      "Test to make sure that priority scheme between CFG grammars work, and that switching windows is detected.",
		TEST_REQUIRED,
      Test045
   },
   {
      "Activate different rules with CFG, and switch windows",
      "Test to make sure that priority scheme between rules within CFG grammars work, and that switching windows is detected.",
		TEST_REQUIRED,
      Test046
   },
   {
      "Send bogus grammars in",
      "Make sure the engine can distinguish bogus data on a grammarload.",
		TEST_REQUIRED,
      Test047
   },
   {
      "Test language chunk",
      "Make sure the engine is looking at the language chunk and erroring out on invalid languages.",
		TEST_REQUIRED,
      Test048
   },
   {
      "Support dictation grammars",
      "If an engine claims to support dictation grammars, make sure it does.",
		TEST_REQUIRED,
      Test049
   },
   {
      "Support limited-domain grammars",
      "If an engine claims to support limited-domain grammars, make sure it does.",
		TEST_REQUIRED,
      Test050
   },
   {
      "Support simple CFGs",
      "Test to see if the engine can recognize from a CFG with a sequence of one-word alternatives.",
		TEST_REQUIRED,
      Test051
   },
   {
      "Support simple CFGs",
      "Test to see if the engine can recognize from a CFG with a sequence of multi-word alternatives.",
		TEST_REQUIRED,
      Test052
   },
   {
      "Support simple CFGs",
      "Test to see if the engine can recognize from a CFG with rules.",
		TEST_REQUIRED,
      Test053
   },
   {
      "Support complex CFGs",
      "Test to make sure the default rules provided by the grammar compiler object are supported.",
		TEST_REQUIRED,
      Test054
   },
   {
      "Support complex CFGs",
      "Test to make sure that long, hyphenated words are recognized.",
		TEST_REQUIRED,
      Test055
   },
   {
      "Support complex CFGs",
      "Test that word spotting works.",
		TEST_REQUIRED,
      Test056
   },
   {
      "Support complex CFGs",
      "CFGs with numbers instead of typed words.",
		TEST_REQUIRED,
      Test057
   },
   {
      "Support complex CFGs",
      "CFGs with varying sorts of lists.",
		TEST_REQUIRED,
      Test058
   },
   {
      "Support complex CFGs",
      "Test replacing lists while speech recognition is listening.",
		TEST_REQUIRED,
      Test059
   },
   {
      "Dictation grammars",
      "Make sure the dictation grammar can handle various informational chunks.",
		TEST_REQUIRED,
      Test060
   },
   {
      "Discrete Dictation grammars",
      "Make sure the discrete dictation works.",
		TEST_REQUIRED,
      Test061
   },
   {
      "Activate alternative CFG rule",
      "Make sure CFGs can activate an alternate rule, and that rule names are case independent.",
		TEST_REQUIRED,
      Test062
   },
   {
      "hwnd=App dictation grammar, and hwnd=NULL dictation grammar",
      "If two dictation grammars are active then make sure the appropriate one gets the audio. Expect app.",
		TEST_REQUIRED,
      Test063
   },
   {
      "hwnd=App dictation grammar, and hwnd=NULL dictation grammar",
      "If two dictation grammars are active then make sure the appropriate one gets the audio. Expect dictation.",
		TEST_REQUIRED,
      Test064
   },
   {
      "Dictation recognize punctuation",
      "Make sure dictation can recognize standard punctuation.",
		TEST_REQUIRED,
      Test065
   },
   {
      "Words with back-slashes",
      "If a word has a backslash, the recognizer should use the pronunciation occurring after the backslash.",
		TEST_REQUIRED,
      Test066
   },
   {
      "PhraseFinish has right WordIDs",
      "Make sure that phrase finish has the right word ID's.",
		TEST_REQUIRED,
      Test067
   },
   {
      "Large fan-out CFG",
      "Make sure that a large fan-out CFG won't crash the engine.",
		TEST_REQUIRED,
      Test068
   },
   {
      "AutoGainEnableGet/Set",
      "Make sure it remembers values passed into it, attrib-changed notification is sent, and that it handles invalid parameters.",
		TEST_OPTIONAL,
      Test069
   },
   {
      "EchoGet/Set",
      "Make sure it remembers values passed into it, attrib-changed notification is sent, and that it handles invalid parameters.",
		TEST_OPTIONAL,
      Test070
   },
   {
      "EnergyFloorGet/Set",
      "Make sure it remembers values passed into it, attrib-changed notification is sent, and that it handles invalid parameters.",
		TEST_OPTIONAL,
      Test071
   },
   {
      "MicrophoneGet/Set",
      "Make sure it remembers values passed into it, attrib-changed notification is sent, and that it handles invalid parameters.",
		TEST_OPTIONAL,
      Test072
   },
   {
      "RealTimeGet/Set",
      "Make sure it remembers values passed into it, attrib-changed notification is sent, and that it handles invalid parameters.",
		TEST_OPTIONAL,
      Test073
   },
   {
      "SpeakerGet/Set",
      "Make sure it remembers values passed into it, attrib-changed notification is sent, and that it handles invalid parameters.",
		TEST_REQUIRED,
      Test074
   },
   {
      "SpeakerGet/Set vs. ISRSpeaker",
      "Make sure that speaker get/set communicates with ISRSpeaker.",
		TEST_REQUIRED,
      Test075
   },
   {
      "ThresholdGet/Set",
      "Make sure it remembers values passed into it, attrib-changed notification is sent, and that it handles invalid parameters.",
		TEST_REQUIRED,
      Test076
   },
   {
      "ThresholdGet/Set",
      "Test to make sure that threshold get/set actually works.",
		TEST_REQUIRED,
      Test077
   },
   {
      "TimeOutGet/Set",
      "Make sure it remembers values passed into it, attrib-changed notification is sent, and that it handles invalid parameters.",
		TEST_REQUIRED,
      Test078
   },
   {
      "TimeOutGet/Set",
      "Test to see if values are really held to.",
		TEST_REQUIRED,
      Test079
   },
   {
      "ModeGet",
      "Invalid parameters",
		TEST_REQUIRED,
      Test080
   },
   {
      "PosnGet & ToFileTime",
      "Make sure PosnGet and ToFileTime return approximately the same values as the audio source. Also pass in invalid parameters",
		TEST_REQUIRED,
      Test081
   },
   {
      "Register",
      "Invalid Parameters",
		TEST_REQUIRED,
      Test082
   },
   {
      "Dialogs",
      "Make sure the dialog functions in ISRDialogs are supported.",
		TEST_REQUIRED,
      Test083
   },
   {
      "GrammarLoad",
      "Invalid parameters",
		TEST_REQUIRED,
      Test084
   },
   {
      "Pause/Resume",
      "Pause after a PhraseStart and make sure we don't get a PhraseFinish until we resume.",
		TEST_REQUIRED,
      Test085
   },
   {
      "Pause/Resume",
      "Pause after a PhraseStart and make sure we don't get a PhraseFinish until we resume. Make sure must call one resume per pause.",
		TEST_REQUIRED,
      Test086
   },
   {
      "Pause/Resume",
      "Pause before activating and make sure it doesn't listen.",
		TEST_REQUIRED,
      Test087
   },
   {
      "Pause/Resume",
      "Pause between two utterances and make sure no audio is processed.",
		TEST_REQUIRED,
      Test088
   },
   {
      "Pause/Resume",
      "Pause between two utterances and make sure no audio is processed. Have a second (CFG) grammar also active.",
		TEST_REQUIRED,
      Test089
   },
   {
      "Pause/Resume",
      "Pause between two utterances and make sure no audio is processed. Have a second (dictation) grammar also active.",
		TEST_REQUIRED,
      Test090
   },
   {
      "Pause/Resume",
      "Call Resume without first calling Pause.",
		TEST_REQUIRED,
      Test091
   },
   {
      "Delete",
      "Test deleting a speaker that's no select, that's selected, and an invalid parameter.",
		TEST_REQUIRED,
      Test092
   },
   {
      "Enum",
      "Test enumeration of speakers with valid and invalid parameters.",
		TEST_REQUIRED,
      Test093
   },
   {
      "Merge",
      "Test merging of speakers with valid and invalid parameters.",
		TEST_REQUIRED,
      Test094
   },
   {
      "New",
      "Test ISRSpeaker::New - Valid and invalid parameters.",
		TEST_REQUIRED,
      Test095
   },
   {
      "Query",
      "Test ISRSpeaker::Query - Valid and invalid parameters.",
		TEST_REQUIRED,
      Test096
   },
   {
      "Read",
      "Test ISRSpeaker::Read - Valid and invalid parameters.",
		TEST_REQUIRED,
      Test097
   },
   {
      "Revert",
      "Test ISRSpeaker::Revert - Valid and invalid parameters.",
		TEST_REQUIRED,
      Test098
   },
   {
      "Write",
      "Test ISRSpeaker::Write - Valid and invalid parameters.",
		TEST_REQUIRED,
      Test099
   },
   {
      "Grammar - Limited Domain",
      "Make sure limited domain grammars support ISRGramDictation.",
		TEST_REQUIRED,
      Test100
   },
   {
      "Grammar - ListAppend",
      "Add words onto a list and make sure they're recognized.",
		TEST_REQUIRED,
      Test101
   },
   {
      "Grammar - ListAppend",
      "Add words (with digits in them) onto a list and make sure they're recognized.",
		TEST_REQUIRED,
      Test102
   },
   {
      "Grammar - ListAppend",
      "Invalid parameters.",
		TEST_REQUIRED,
      Test103
   },
   {
      "Grammar - ListGet",
      "Test that the general functionality works.",
		TEST_REQUIRED,
      Test104
   },
   {
      "Grammar - ListGet",
      "Test that ListGet is returning the right data after adding/removing different words.",
		TEST_REQUIRED,
      Test105
   },
   {
      "Grammar - ListGet",
      "Invalid parameters.",
		TEST_REQUIRED,
      Test106
   },
   {
      "Grammar - ListQuery",
      "Test ListQuery with valid and invalid parameters.",
		TEST_REQUIRED,
      Test107
   },
   {
      "Grammar - ListRemove",
      "Test ListRemove with valid parameters",
		TEST_REQUIRED,
      Test108
   },
   {
      "Grammar - ListRemove",
      "Test ListRemove with valid and invalid words",
		TEST_REQUIRED,
      Test109
   },
   {
      "Grammar - ListRemove",
      "Test ListRemove with invalid parameters.",
		TEST_REQUIRED,
      Test110
   },
   {
      "Grammar - ListSet",
      "Test with a really large list.",
		TEST_REQUIRED,
      Test111
   },
   {
      "Grammar - ListSet",
      "Invalid parameters.",
		TEST_REQUIRED,
      Test112
   },
   {
      "Grammar - Activate",
      "Make sure autopause works with CFGs.",
		TEST_REQUIRED,
      Test113
   },
   {
      "Grammar - Activate",
      "Make sure autopause works with dictation grammars.",
		TEST_REQUIRED,
      Test114
   },
   {
      "Grammar - Activate",
      "Activate CFG with invalid rules.",
		TEST_REQUIRED,
      Test115
   },
   {
      "Grammar - Activate",
      "Activate dictation with invalid rules.",
		TEST_REQUIRED,
      Test116
   },
   {
      "Grammar - Archive",
      "Test that archiving of CFG works. Don't archive the results objects.",
		TEST_REQUIRED,
      Test117
   },
   {
      "Grammar - Archive",
      "Test that archiving of CFG works. Archive the results objects.",
		TEST_REQUIRED,
      Test118
   },
   {
      "Grammar - Archive",
      "Invalid parameters",
		TEST_REQUIRED,
      Test119
   },
   {
      "Grammar - Archive",
      "Test that archiving of Dictation works. Don't archive the results objects.",
		TEST_REQUIRED,
      Test120
   },
   {
      "Grammar - Archive",
      "Test that archiving of Dictation works. Archive the results objects.",
		TEST_REQUIRED,
      Test121
   },
   {
      "Grammar - Archive",
      "Invalid parameters (Dictation)",
		TEST_REQUIRED,
      Test122
   },
   {
      "Grammar - BookMark",
      "Test to make sure that the bookmark call works with CFGs",
		TEST_OPTIONAL,
      Test123
   },
   {
      "Grammar - BookMark",
      "Test to make sure that the bookmark call works with dictation grammars",
		TEST_OPTIONAL,
      Test124
   },
   {
      "Grammar - BookMark",
      "Invalid parameter.",
		TEST_OPTIONAL,
      Test125
   },
   {
      "Grammar - BookMark",
      "Set a bookmark to go off, and then clear the grammar. The bookmark should not then happen.",
		TEST_OPTIONAL,
      Test126
   },
   {
      "Grammar - Deactivate",
      "Deactivate rules that aren't active or that don't exist.",
		TEST_REQUIRED,
      Test127
   },
   {
      "Grammar - TrainPhrase",
      "Test valid and invalid inputs.",
		TEST_OPTIONAL,
      Test128
   },
   {
      "Grammar - TrainQuery",
      "Test valid and invalid inputs.",
		TEST_REQUIRED,
      Test129
   },
   {
      "Grammar - Context",
      "See if the Context call affects recognition results.",
		TEST_OPTIONAL,
      Test130
   },
   {
      "Grammar - Context",
      "Pass in invalid parameters",
		TEST_OPTIONAL,
      Test131
   },
   {
      "Grammar - Hint",
      "Pass in valid and invalid parameters",
		TEST_OPTIONAL,
      Test132
   },
   {
      "Grammar - Words",
      "Pass in valid and invalid parameters",
		TEST_OPTIONAL,
      Test133
   },
   {
      "Recognize yes/no",
      "Test to see if the recognizer can tell the difference between yes and no.",
		TEST_REQUIRED,
      Test134
   },
   {
      "Grammar - DeteriorationSet/Get",
      "Pass valid and invalid parameters to deterioration set/get.",
		TEST_REQUIRED,
      Test135
   },
   {
      "Grammar - DeteriorationSet/Get",
      "Test that deterioration actually works.",
		TEST_OPTIONAL,
      Test136
   },
   {
      "Free engine, except for results object",
      "Free the engine, except for the results object, and make sure the results object still works.",
		TEST_REQUIRED,
      Test137
   },
   {
      "Free engine, except for grammar object",
      "Free the engine, except for the grammar object, and make sure the results object still works.",
		TEST_REQUIRED,
      Test138
   },
   {
      "Bad audio format",
      "Create a speech recognition engine with an audio source that refuses the engine's wave format.",
		TEST_REQUIRED,
      Test139
   },
   {
      "Absolute silence",
      "Make sure the engine can handle absolute silence.",
		TEST_REQUIRED,
      Test140
   },
   {
      "ISRGramNotifySink::PhraseFinish",
      "Make sure the time stamps are reasonable.",
		TEST_REQUIRED,
      Test141
   },
   {
      "ISRGramNotifySink::PhraseHypothesis",
      "Test that it's called and that it's return values are reasonable.",
		TEST_OPTIONAL,
      Test142
   },
   {
      "ISRGramNotifySink::Training",
      "Make sure that the training request is called for new users.",
		TEST_OPTIONAL,
      Test143
   },
   {
      "ISRGramNotifySink::Interference - Audio messages",
      "Test that audio messages in Interference are called properly.",
		TEST_REQUIRED,
      Test144
   },
   {
      "ISRNotifySink::VUMeter",
      "Make sure that VU meter notifications are made.",
		TEST_OPTIONAL,
      Test145
   },
   {
      "ISRResBasic::FlagsGet",
      "Test that flags get is returning reasonable values, and that it can handle invalid parameters.",
		TEST_REQUIRED,
      Test146
   },
   {
      "ISRResBasic::Identify",
      "Make sure that identify works, and can handle invalid parameters.",
		TEST_REQUIRED,
      Test147
   },
   {
      "ISRResBasic::PhraseGet",
      "Make sure that phrase get works, and can handle invalid parameters.",
		TEST_REQUIRED,
      Test148
   },
   {
      "ISRResBasic::TimeGet",
      "Make sure that that it returns the same values as PhraseFinish, and can accept invalid parameters.",
		TEST_REQUIRED,
      Test149
   },
   {
      "ISRResCorrection::Correction",
      "Make sure that that works, and can accept invalid parameters.",
		TEST_REQUIRED,
      Test150
   },
   {
      "ISRResCorrection::Validate",
      "Make sure that that works, and can accept invalid parameters.",
		TEST_OPTIONAL,
      Test151
   },
   {
      "ISRResEval::ReEvaluate",
      "Make sure that that works, and can accept invalid parameters.",
		TEST_OPTIONAL,
      Test152
   },
   {
      "ISRResMemory::Get and Free",
      "Make sure that that works, and can accept invalid parameters.",
		TEST_REQUIRED,
      Test153
   },
   {
      "ISRResMemory::LockGet and LockSet",
      "Make sure that that works, and can accept invalid parameters.",
		TEST_REQUIRED,
      Test154
   },
   {
      "ISRResScore::GetPhraseScore and GetWordScore",
      "Make sure that that works, and can accept invalid parameters.",
		TEST_REQUIRED,
      Test155
   },
   {
      "ISRResAudio::GetWAV",
      "Make sure that that works, and can accept invalid parameters.",
		TEST_REQUIRED,
      Test156
   },
   {
      "Time-out on PhraseFinish",
      "Make sure the recognizer doens't time out too quickly on phrase finish, and that the time-out is independent of how the recognizer is written.",
		TEST_REQUIRED,
      Test157
	},
	{
      "ILexPronounce: Get pronunciations - known valid word",
      "Make sure Get can retrieve the pronounciation of a word known to be in the lex",
		TEST_OPTIONAL,
      Test158
   },
	{
      "ILexPronounce: Get pronunciations - known invalid word",
      "Make sure Get returns a pronunciation for word that is not in the lex",
		TEST_OPTIONAL,
      Test159
   },
	{
      "ILexPronounce: Get pronunciations - known valid word, return buffer too small",
      "Make sure Get returns an error that buffer too small when it is.",
		TEST_OPTIONAL,
      Test160
   },
	{
      "ILexPronounce: Add pronunciations - known valid word",
      "Make sure can add a valid word",
		TEST_OPTIONAL,
      Test161
   },
	{
      "ILexPronounce: Add pronunciations - known invalid word",
      "Make sure cannot add a word already in the lex",
		TEST_OPTIONAL,
      Test162
   },
	{
      "ILexPronounce: Add pronunciations - known valid word, get it",
      "Make sure can get added word",
		TEST_OPTIONAL,
      Test163
   },
	{
		"ILexPronounce: Add pronunciations - invalid param",
		"Make sure returns error with invalid parameter",
		TEST_OPTIONAL,
		Test164
	},
	{
		"ILexPronounce: Remove pronunciations - valid word",
		"add should succeed after remove valid word",
		TEST_OPTIONAL,
		Test165
	},
	{
		"ILexPronounce: Remove word - known invalid word",
		"Make sure remove returns error for remove of invalid word",
		TEST_OPTIONAL,
		Test166
	},
	{
      "ILexPronounce2: GetFrom pronunciations - known valid word",
      "Make sure GetFrom retrieve the pronounciation of a word known to be in the lex",
		TEST_OPTIONAL,
      Test167
   },
	{
      "ILexPronounce2: GetFrom pronunciations - known invalid word",
      "Make sure GetFrom returns a pronunciation for word that is not in the lex",
		TEST_OPTIONAL,
      Test168
   },
	{
      "ILexPronounce2: GetFrom pronunciations - known valid word, return buffer too small",
      "Make sure GetFrom returns an error that buffer too small when it is.",
		TEST_OPTIONAL,
      Test169
   },
	{
      "ILexPronounce2: AddTo pronunciations - attempt to AddTo and RemoveFrom LTS",
      "Make sure cannot add a word or remove from the LTS lexicon",
		TEST_OPTIONAL,
      Test170
   },
	{
      "ILexPronounce2: AddTo pronunciations - known valid word",
      "Make sure can add a valid word to all writable lexicons",
		TEST_OPTIONAL,
      Test171
   },
	{
      "ILexPronounce2: AddTo pronunciations - known invalid word",
      "Make sure cannot add a word already in the lex",
		TEST_OPTIONAL,
      Test172
   },
	{
      "ILexPronounce2: AddTo pronunciations - known valid word, get it",
      "Make sure can get added word",
		TEST_OPTIONAL,
      Test173
   },
	{
		"ILexPronounce2: AddTo pronunciations - invalid param",
		"Make sure returns error with invalid parameter",
		TEST_OPTIONAL,
		Test174
	},
	{
		"ILexPronounce2: QueryLexicons - verify all writable lex's are read lex's",
		"Make sure query lexicon returns read and write lexicons and that all writeable lexicons are readable",
		TEST_OPTIONAL,
		Test175
	},
	{
		"ILexPronounce2: ChangeSpelling - known valid word",
		"Make sure spelling is returns no error on change of valid word",
		TEST_OPTIONAL,
		Test176
	},
	{
		"ILexPronounce2: ChangeSpelling - known invalid word",
		"Make sure spelling returns error on attempt to change spelling of invalid word",
		TEST_OPTIONAL,
		Test177
	},
	{
		"ILexPronounce2: ChangeSpelling - invalid param",
		"Make sure returns error when ChangeSpelling is passed invalid parameters",
		TEST_OPTIONAL,
		Test178
	},
	{
		"ILexPronounce2: ChangeSpelling - known valid word, change back",
		"Make sure spelling is successfully changed for a valid word",
		TEST_OPTIONAL,
		Test179
	},
	{
		"ISRResGraphEx - NodeStartGet",
		"Make sure NodeStartGet works and can take invalid parameters",
		TEST_REQUIRED,
		Test180
	},
	{
		"ISRResGraphEx - NodeEndGet",
		"Make sure NodeEndGet works and can take invalid parameters",
		TEST_REQUIRED,
		Test181
	},
	{
		"ISRResGraphEx - ArcEnum",
		"Make sure ArcEnum works and can take invalid parameters",
		TEST_REQUIRED,
		Test182
	},
	{
		"ISRResGraphEx - GetAllArcs",
		"Make sure GetAllArcs works and can take invalid parameters",
		TEST_REQUIRED,
		Test183
	},
	{
		"ISRResGraphEx - GetAllNodes",
		"Make sure GetAllNodes works and can take invalid parameters",
		TEST_REQUIRED,
		Test184
	},
	{
		"ISRResGraphEx - GetNode",
		"Make sure GetNode works for all arc enumerated",
		TEST_REQUIRED,
		Test185
	},
	{
		"ISRResGraphEx - DWORDGet",
		"Make sure DWORDGet works for all arc enumerated",
		TEST_REQUIRED,
		Test186
	},
	{
		"ISRResGraphEx - DataGet",
		"Make sure DataGet works for all nodes",
		TEST_REQUIRED,
		Test187
	},
	{
		"ISRResGraphEx - DataGet",
		"Make sure DataGet works for all arcs",
		TEST_REQUIRED,
		Test188
	},
	{
		"ISRResGraphEx - ScoreGet",
		"Make sure DataGet works for all arcs",
		TEST_REQUIRED,
		Test189
	},
	{
		"ISRResGraphEx - BestPathEnum",
		"Make sure BestPathEnum works and returns scores that make sense",
		TEST_REQUIRED,
		Test190
	},
	{
		"complex CFGs",
		"check that can load word spotting grammer and CFG",
		TEST_REQUIRED,
		Test191
	},
	{
		"ILexPronounce - AddTo, fill dictionary",
		"make sure can fill dictionary without problems",
		TEST_OPTIONAL,
		Test192
	},
	{
		"IAttributes - DWORDGet",
		"make sure can DWORDGet attributes and handles invalid arguments",
		TEST_OPTIONAL,
		Test193
	},
	{
		"IAttributes - DWORDSet",
		"make sure can DWORDSet attributes and handles invalid arguments",
		TEST_OPTIONAL,
		Test194
	},
	{
		"IAttributes - DWORDSet - check specific attributes",
		"make sure can DWORDSet works as expected with specific attributes",
		TEST_OPTIONAL,
		Test195
	},
	{
		"IAttributes - StringGet",
		"make sure can StringGet attributes and handles invalid argumants",
		TEST_OPTIONAL,
		Test196
	},
	{
		"IAttributes - StringSet",
		"make sure can StringSet attributes and handles invalid argumants",
		TEST_OPTIONAL,
		Test197
	},
	{
		"IAttributes - MemoryGet",
		"make sure can MemoryGet attributes and handles invalid argumants",
		TEST_OPTIONAL,
		Test198
	},
	{
		"IAttributes - MemorySet",
		"make sure can MemorySet attributes and handles invalid argumants",
		TEST_OPTIONAL,
		Test199
	},
	{
		"Grammer - symbols as words",
		"make sure handles symbols as individual words correctly",
		TEST_REQUIRED,
		Test200
	},
   {
      "ISRGramNotifySink - destroy engine while registered",
      "make sure doesn't crash if destroy engine while registered",
		TEST_REQUIRED,
      Test201
   },
	{
		"AutoGainEnableSet & Get",
		"make sure handles min/max magic numbers correctly",
		TEST_OPTIONAL,
		Test202
	},
   {
      "EnergyFloorGet/Set",
      "Make sure it handles min/max magic number parameters.",
		TEST_OPTIONAL,
      Test203
   },
   {
      "RealTimeGet/Set - Make sure that set default values",
      "Make sure it handles min/max magic number parameters.",
		TEST_OPTIONAL,
      Test204
   },
   {
      "ThreasholdGet/Set - Make sure that set default values",
      "Make sure it handles min/max magic number parameters.",
		TEST_REQUIRED,
      Test205
   },
   {
      "TimeOutGet/Set - Make sure that set default values",
      "Make sure it handles min/max magic number parameters.",
		TEST_REQUIRED,
      Test206
   },
   {
      "Test ISRSpeaker2::Commit",
      "Test to make sure that Commit seems to work.",
		TEST_OPTIONAL,
      Test207
   },
   {
      "Test ISRSpeaker2::Rename",
      "Test to make sure that renaming works properly.",
		TEST_OPTIONAL,
      Test208
   },
   {
      "Test ISRSpeaker2::GetChangedInfo",
      "Test to make sure that GetChangedInfo works properly.",
		TEST_OPTIONAL,
      Test209
   },
   {
      "Test ISRDialogs2",
      "Test to make sure that ISRDialogs2 works properly.",
		TEST_OPTIONAL,
      Test210
   },
   {
      "Test ISRResMerge::Merge",
      "Test to make sure that ISRResMerge::Merge works with invalid parameters.",
		TEST_OPTIONAL,
      Test211
   },
   {
      "Test ISRResMerge::Split",
      "Test to make sure that ISRResMerge::Split works with invalid parameters",
		TEST_OPTIONAL,
      Test212
   },
   {
      "Test ISRResMerge::Split",
      "Test to make sure that ISRResMerge::Split splits begin/end timestamps properly",
		TEST_OPTIONAL,
      Test213
   },
   {
      "Test ISRResMerge::Merge",
      "Test to make sure that ISRResMerge::Merge merges split results properly",
		TEST_OPTIONAL,
      Test214
   },
   {
      " ",
      " ",
		TEST_OPTIONAL,
      Test215
   },
	{
		"IAttributes - DWORDGet/Set SRATTR_THREADPRIORITY",
		"make sure can DWORDGet/Set SRATTR_THREADPRIORITY attributes",
		TEST_OPTIONAL,
		Test216
	}
};

DWORD gdwTestElems = sizeof(gaTest) / sizeof(FUNCTEST);