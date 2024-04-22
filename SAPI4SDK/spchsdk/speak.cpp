/*******************************************************
Speak.cpp - Example command line program that speaks its command line or stdin


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <stdio.h>
#include <initguid.h>
#include <spchwrap.h>

//usage: either "speak text to speak"
//or speak < file
//
void main(int argc,char *argv[])
{
	WCHAR *theText;
	CVoiceText *SpeakObject;

	CoInitialize(0);					//must call this to initilize COM
	SpeakObject = new CVoiceText;	    //create a voice object

	if(!SpeakObject)					
	{
		printf("unable to create speak object.\n");
		exit(-1);
	}

	if(SpeakObject->Init(L"speak.exe")) //initialize object
	{
		printf("unable to initialize speak object.\n");
		exit(-1);
	}

	if(argc > 1)						    //got a command line...
	{
		//get the UNICODE version of the command line, and start at the first space
		//(the first word is the name of the program!)
		theText = wcsstr (GetCommandLineW(),L" "); 
		SpeakObject->SpeakAndWait(theText);		//say it..and wait
	}
	else							        // no command line, must be file input (stdin)
	{
#define READBUFFERSIZE 10000		        //#characters to buffer up before calling speak
		WCHAR s[READBUFFERSIZE];
		int i;
		do
		{
			for(i=0;i<READBUFFERSIZE;i++)	//get a UNICODE text buffer from sdin
			{
				s[i]=getwchar()	;			//one wchar at a time..
				if(s[i] == WEOF)			//until end of file (^Z on console)
				{
					s[i] = 0;			    //null terminate
					break;
				}
			}
			SpeakObject->Speak(s);			//speak buffer
		}
		while(i >= READBUFFERSIZE);			//if there is more than one buffer full, go get it
		while(SpeakObject->IsSpeaking())	//wait until speaking is finished.
		{
			Sleep(500);					    //yeild cpu time
		}
	}

	CoUninitialize();						
}