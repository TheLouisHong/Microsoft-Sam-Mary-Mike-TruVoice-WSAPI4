//NOTE you must edit post.bat and reset the SKDDRIVE and SDK variables before building
import java.applet.*;
import java.awt.*;
import colorsFrame;

import java.net.*;
import java.util.*;
import java.lang.Math;

import com.ms.activeX.ActiveXControl;
import com.ms.com.ConnectionPointCookie;
import directsr.*;

public class colors extends Applet
{
	String gHeard = "";
	int gFlash = 0;
	int gSequence[] = new int[200];
	int gSequenceIndex = 0;
	int gSequenceLength = 4;
	MyEventSinker Cbacks;
	ConnectionPointCookie Eventcookie;
	DirectSR recognizer;

	public class MyEventSinker implements _DirectSREventsDefault
	{  
		
  
			public void PhraseFinish(int flags, int beginhi, int beginlo, int endhi, int endlo, String Phrase, String parsed, int results)
			{
				gHeard = parsed;
				
				if(parsed.compareTo("restart") == 0) 
				{
					RestartGame();		
					
				}
				else
				if(parsed.compareTo("red") == 0) 
				{
					if(gSequence[gSequenceIndex] == 1)
					{
						Flash(1);
						++gSequenceIndex;
					}
					else
					{
						RestartGame();		
					}
					
				}
				else
				if(parsed.compareTo("green") == 0) 
				{
					if(gSequence[gSequenceIndex] == 2)
					{
						Flash(2);
						++gSequenceIndex;
					}
					else
					{
						RestartGame();		
					}
				}
				else
				if(parsed.compareTo("blue") == 0) 
				{
					if(gSequence[gSequenceIndex] == 3)
					{
						Flash(3);
						++gSequenceIndex;
					}
					else
					{
						RestartGame();		
					}
				}
				else
				if(parsed.compareTo("yellow") == 0) 
				{
					if(gSequence[gSequenceIndex] == 4)
					{
						Flash(4);
						++gSequenceIndex;
					}
					else
					{
						RestartGame();						
					}
				}
				if(gSequenceIndex >=gSequenceLength)
				{
					++gSequenceLength;
					Sequence(gSequenceLength);
					gSequenceIndex = 0;
				}
						    
			};
			public void ClickIn(int x, int y){};
			public void ClickOut(int x, int y){};
			public void PhraseStart(int hi, int lo){};
			public void BookMark(int MarkID){};
			public void Paused(){};
			public void PhraseHypothesis(int flags, int beginhi, int beginlo, int endhi, int endlo, String Phrase, int results){};
			public void ReEvaluate(int Result){};
			public void Training(int train){};
			public void UnArchive(int Result){};

		}
	// STANDALONE APPLICATION SUPPORT:
	//		m_fStandAlone will be set to true if applet is run standalone
	//--------------------------------------------------------------------------
	private boolean m_fStandAlone = false;

	public static void main(String args[])
	{
		colorsFrame frame = new colorsFrame("colors");

		// Must show Frame before we size it so insets() will return valid values
		//----------------------------------------------------------------------
		frame.show();
        frame.hide();
		frame.resize(frame.insets().left + frame.insets().right  + 400,
					 frame.insets().top  + frame.insets().bottom + 240);

		colors applet_colors = new colors();

		frame.add("Center", applet_colors);
		applet_colors.m_fStandAlone = true;
		applet_colors.init();
		applet_colors.start();
        frame.show();
	}

	public colors()
	{
	}

	public String getAppletInfo()
	{
		return "Name: colors\r\n" +
		       "Author: scottq\r\n" +
		       "Created with Microsoft Visual J++ Version 1.1";
	}


	public void init()
	{
    	resize(400, 240);

		recognizer = new DirectSR();      
		Cbacks = new MyEventSinker();
		try{
	    Eventcookie = new ConnectionPointCookie(recognizer,Cbacks,Class.forName("directsr._DirectSREvents"));
		} catch (Exception ClassNotFoundException){};
		recognizer.GrammarFromString("[grammar]\ntype=cfg\n[<start>]\n<start>=restart \"restart\"\n<start>=<color>\n[<color>]\n<color>=red \"red\"\n<color>=green \"green\"\n<color>=blue \"blue\"\n<color>=yellow \"yellow\"\n");
		recognizer.Activate();
	}

	public void destroy()
	{
	}

	public void RestartGame()
	{
		Flash(-1);
		Flash(-1);
		gSequenceLength = 4;
		gSequenceIndex = 0;
		Sequence(gSequenceLength);
	}
	public void Flash(int color)
	{
		gFlash = color;
		repaint();
				
		try{Thread.sleep(500);}
		catch (InterruptedException e){};

		gFlash = 0;
		repaint();

		try{Thread.sleep(500);}
		catch (InterruptedException e){};
	}
	public void Sequence(int count)
	{
		int i;
		int val;
		for (i=0;i<count;i++)
		{
			//val=i%4+1;
			val = (int)(Math.random()*4+1);
			gSequence[i] = val;
			Flash(val);		
		}
	
	}
	// javarec Paint Handler
	//--------------------------------------------------------------------------
	public void paint(Graphics g)
	{
		int reddiam = 0;
		int greendiam = 0;
		int bluediam = 0;
		int yellowdiam = 0;
		String s;


		g.drawString("Say 'restart' to start a new game.",10,20);
		g.drawString ("Say 'red', 'green', 'blue', or 'yellow' one at a time in correct sequence!", 10, 35);
		s = "Current sequence length: ";
		s=s.concat(Integer.toString(gSequenceLength));
		
		g.drawString(s, 10, 50);
		s = "Last heard: ";
		s=s.concat(gHeard);
		g.drawString(s, 10, 65);

		if((gFlash == 1) || (gFlash == -1))
		{
			reddiam = 12;
		}
		
		if((gFlash == 2) || (gFlash == -1))
		{
			greendiam = 12;
		}
		
		if((gFlash == 3) || (gFlash == -1))
		{
			bluediam = 12;
		}
		
		if((gFlash == 4) || (gFlash == -1))
		{
			yellowdiam = 12;
		}
		
			
		g.setColor(new Color(255, 0, 0));
		g.fillOval(80-reddiam/2,80-reddiam/2,40+reddiam,40+reddiam);
		g.setColor(new Color(0, 255, 0));
		g.fillOval(160-greendiam/2,80-greendiam/2,40+greendiam,40+greendiam);
		g.setColor(new Color(0, 0, 255));
		g.fillOval(80-bluediam/2,160-bluediam/2,40+bluediam,40+bluediam);
		g.setColor(new Color(255, 255, 0));
		g.fillOval(160-yellowdiam/2,160-yellowdiam/2,40+yellowdiam,40+yellowdiam);

		
	}

	public void start()
	{
	}
	
	public void stop()
	{ 
		recognizer.Deactivate();
	}



}
