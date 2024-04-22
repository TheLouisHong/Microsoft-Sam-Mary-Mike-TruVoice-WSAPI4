/************************************************************************
DiffWords.Cpp - Compares two files (containing only words) and figures
out what's different for them, where there errors are, etc.

Copyright c. 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/
#include <windows.h>
#include <spchwrap.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

// #define  DISPLAYRESULTS


#define  HYPAT_INSERTED    1
#define  HYPAT_DELETED     2
#define  HYPAT_SAMEORWRONG 3

class CHyp {
   public:
      CHyp (void);
      ~CHyp (void);

      void Init (DWORD dwCorrectIndex, DWORD dwRecogIndex);
      CHyp *CloneAndAdvance(DWORD dwAdvanceType);
      DWORD Score();

      DWORD    m_dwRight;
      DWORD    m_dwWrong;
      DWORD    m_dwInserted;
      DWORD    m_dwDeleted;
      DWORD    m_dwCorrectIndex;
      DWORD    m_dwRecogIndex;

#ifdef DISPLAYRESULTS
      DWORD    m_dwResPosn;
      BYTE     *m_pResults;
#endif
   };

typedef CHyp * PCHyp;


#define  MAXHYP         2000
#define  ELIMDEFAULT    8    // eliminate hypothesis if score is this much worse

/*******************************************************************
globals */
BOOL     gfWordMode=FALSE;
PCSWList   gpListCorrect, gpListRecog;
PCHyp    gapHyp[MAXHYP];
DWORD    gdwNumHyp;   // number of hypothesis fields filled
                     // Some are filled with NULL
DWORD    gdwMaxResultSize; // maximum size expect for results storage

DWORD gdwRight, gdwWrong, gdwInserted, gdwDeleted;

/* add rt */
extern DWORD WInsertions,WDeletions,WSubstitutions,WRight;
/* end add rt */

/************************************************************************
CompareStrings - Returns TRUE if they're effectively the same, FALSE
   if different

inputs
   char  *psz1, *psz2 - 2 strings, psz1 is from the recognizer
returns
   BOOL
*/
BOOL CompareStrings (char *psz1, char *psz2)
{
   if (!stricmp(psz1, psz2))
      return TRUE;

   // If there's a backslash then just compare the
   // beginnings
   if ((psz1[1] == '\\') && (psz2[1] == '\\'))
      return psz1[0] == psz2[0];

   return FALSE;
}



/*******************************************************************
CHyp - Hypothesis object
*/
CHyp::CHyp (void)
{
   m_dwRight = m_dwWrong = m_dwInserted = m_dwDeleted = 0;
   m_dwCorrectIndex = m_dwRecogIndex = 0;

#ifdef DISPLAYRESULTS
   m_dwResPosn = 0;
   m_pResults = (BYTE*) malloc(gdwMaxResultSize);
#endif
}

CHyp::~CHyp (void)
{
#ifdef DISPLAYRESULTS
   if (m_pResults)
      free (m_pResults);
#endif
   // do nothing
}

void CHyp::Init (DWORD dwCorrectIndex, DWORD dwRecogIndex)
{
   m_dwCorrectIndex = dwCorrectIndex;
   m_dwRecogIndex = dwRecogIndex;
}

DWORD CHyp::Score (void)
{
   return m_dwWrong + m_dwInserted + m_dwDeleted;
}

CHyp * CHyp::CloneAndAdvance(DWORD dwAdvanceType)
{
   char  *psz1, *psz2;
   switch (dwAdvanceType) {
   case HYPAT_INSERTED:
      psz1 = (char*) gpListRecog->GetElem(m_dwRecogIndex);
      if (!psz1)
         return NULL;   // can't assume inserted because no more words
      break;
   case HYPAT_DELETED:
      psz2 = (char*) gpListCorrect->GetElem(m_dwCorrectIndex);
      if (!psz2)
         return NULL;   // can't assume inserted because no more words
      break;
   case HYPAT_SAMEORWRONG:
      psz1 = (char*) gpListRecog->GetElem(m_dwRecogIndex);
      if (!psz1)
         return NULL;   // can't assume inserted because no more words
      psz2 = (char*) gpListCorrect->GetElem(m_dwCorrectIndex);
      if (!psz2)
         return NULL;   // can't assume inserted because no more words
      break;
   }

   // clone it
   CHyp  *pNew;
   pNew = new CHyp;
   if (!pNew)
      return NULL;
   pNew->Init (m_dwCorrectIndex, m_dwRecogIndex);
   pNew->m_dwRight = m_dwRight;
   pNew->m_dwWrong = m_dwWrong;
   pNew->m_dwInserted = m_dwInserted;
   pNew->m_dwDeleted = m_dwDeleted;
#ifdef DISPLAYRESULTS
   pNew->m_dwResPosn = m_dwResPosn;
   memcpy (pNew->m_pResults, m_pResults, m_dwResPosn);
   pNew->m_pResults[pNew->m_dwResPosn++] = (BYTE) dwAdvanceType;
#endif

   switch (dwAdvanceType) {
   case HYPAT_INSERTED:
      pNew->m_dwRecogIndex++;
      pNew->m_dwInserted++;
      break;
   case HYPAT_DELETED:
      pNew->m_dwCorrectIndex++;
      pNew->m_dwDeleted++;
      break;
   case HYPAT_SAMEORWRONG:
      pNew->m_dwRecogIndex++;
      pNew->m_dwCorrectIndex++;
      if (CompareStrings(psz1, psz2))
         pNew->m_dwRight++;
      else
         pNew->m_dwWrong++;
      break;
   }

   return pNew;
}


/**********************************************************************
FindScoreRange - Finds the highest and lowest score.

inputs
   DWORD    *pdwHighest - Filled with the highest score
   DWORD    *pdwLowest - Filled with the lowest score
*/
void FindScoreRange (DWORD *pdwHighest, DWORD *pdwLowest)
{
   DWORD i, dwScore;
   *pdwHighest = 0;
   *pdwLowest = (DWORD)-1;
   for (i = 0; i < gdwNumHyp; i++) {
      if (!gapHyp[i])
         continue;
      dwScore = gapHyp[i]->Score();
      *pdwHighest = max(dwScore, *pdwHighest);
      *pdwLowest = min(dwScore, *pdwLowest);
   }
}


/**********************************************************************
RemoveBlanks - This remove blank hypothesis from the list.

inputs
   none
returns
   DWORD - number of hypothesis
*/
DWORD RemoveBlanks (void)
{
   DWORD i, dwCopyTo;
   for (i = 0, dwCopyTo = 0; i <gdwNumHyp; i++) {
      if (!gapHyp[i])
         continue;

      // move it
      gapHyp[dwCopyTo++] = gapHyp[i];
   }

   gdwNumHyp = dwCopyTo;
   return dwCopyTo;
}

/***********************************************************************
FindLowestScore - Finds the lowest score inthe hypothesis
*/
DWORD FindLowestScore (void)
{
   DWORD i;
   DWORD dwLowest = (DWORD)-1L;
   for (i = 0; i < gdwNumHyp; i++)
      if (gapHyp[i])
         dwLowest = min(dwLowest, gapHyp[i]->Score());
   return dwLowest;
}



/*********************************************************************
RemoveHighScores - Remove hypothesis with high scores. It leaves
   blanks where the hypotheses are.

inputs
   DWORD dwScore - Hypothesis with this score or better are removed.
*/
void RemoveHighScores (DWORD dwScore)
{
   DWORD i;
   for (i = 0; i < gdwNumHyp; i++)
      if (gapHyp[i] && (gapHyp[i]->Score() >= dwScore)) {
         delete gapHyp[i];
         gapHyp[i] = NULL;
      }
}



/*********************************************************************
ExpandHypothesis - Expands a single hypothesis

inputs
   DWORD dwIndex- hypothesis index
*/
void ExpandHypothesis (DWORD dwIndex)
{
   if (!gapHyp[dwIndex])
      return;

   PCHyp pNew;
   pNew = gapHyp[dwIndex]->CloneAndAdvance(HYPAT_INSERTED);
   if (pNew)
      gapHyp[gdwNumHyp++] = pNew;
   pNew = gapHyp[dwIndex]->CloneAndAdvance(HYPAT_DELETED);
   if (pNew)
      gapHyp[gdwNumHyp++] = pNew;
   pNew = gapHyp[dwIndex]->CloneAndAdvance(HYPAT_SAMEORWRONG);
   if (pNew)
      gapHyp[gdwNumHyp++] = pNew;

   // delete the existing one
   delete gapHyp[dwIndex];
   gapHyp[dwIndex] = NULL;
}

/*********************************************************************
EliminateEquivalents - Removes hypothesis that are equivlant. IE:
   The have the same score, same correct-file parsed, and same
   recognized-file parsed.
*/
void EliminateEquivalents (void)
{
   DWORD i, j;
   PCHyp p1, p2;
   for (i = 0; i < gdwNumHyp; i++) {
      p1 = gapHyp[i];
      if (!p1)
         continue;
      for (j = i+1; j < gdwNumHyp; j++) {
         p2 = gapHyp[j];
         if (!p2)
            continue;

         // compare p1 to p2
         if ((p1->m_dwCorrectIndex == p2->m_dwCorrectIndex) &&
            (p1->m_dwRecogIndex == p2->m_dwRecogIndex) &&
            (p1->Score() == p2->Score()) ) {
               // delete p2
               delete p2;
               gapHyp[j] = NULL;
            }
      }
   }
}


/*********************************************************************
KeepOnlyAFewHyp - If all the hypothesis have the same score, then this
   just keeps N of them. The rest are freed.

inputs
   DWORD dwNum - Number to keep
*/
void KeepOnlyAFewHyp (DWORD dwNum)
{
   DWORD i;
   for (i = dwNum; i < gdwNumHyp; i++)
      if (gapHyp[i])
         delete gapHyp[i];
   gdwNumHyp = dwNum;
}


/*********************************************************************
ExpandHypothesisOneLevel - Expands all hypothesis until they've
   parsed up to (not including) the word index in the orignial correct
   text.

This does the following:
   - Eliminate hypothesis which score N points above the rest (bad score)
   - If no hypothesis left then return
   - Eliminate blanks & count the number of hypothesis
   - If no hypothesis left then return
   - If there's too many hypothesis then decrease N and repeat the
         above two steps, until there are enough
   - Loop through all hypothesis and find those hypothesis which have
         not been expanded to the given correct word index.
   - If there aren't any more then return
   - Expand those hypothesis and repeat entire function

(Actually, there's a setting which says which should be checked,
   correct words or recognized words.)

inputs
   DWORD    dwWordIndex - Word index that it should expand up to
               (but not including)
   BOOL     fExpandCorrect - If TRUE expand the correct one,
               if FALSE expand the recognizer one
returns
   none
*/
void ExpandHypothesisOneLevel (DWORD dwWordIndex, BOOL fExpandCorrect)
{
   DWORD    dwElim;  // eliminate if the score is this much mre than
                     // the lowest
   DWORD    dwLowest;   // lowest score
   
#ifdef DISPLAYRESULTS
   // print a dot if every 100 words
   if (!(dwWordIndex % 100))
      printf (".");
#endif

   dwElim = ELIMDEFAULT; // if more than 5 off then eliminate

findlowestscore:
   // eliminate hypothesis that are equivalent to one another
   EliminateEquivalents();

   // Find the lowest score
   dwLowest = FindLowestScore();
   if (dwLowest == (DWORD) -1L)
      return;  // there are no hypothesies

   // eliminate all hypothesis dwElim points above the lowest
   RemoveHighScores (dwLowest + dwElim);

   // eliminate the blanks introduced by removing the high score
   RemoveBlanks();

   // if no hypothesis left than return
   if (!gdwNumHyp)
      return;

   // if there's too many hypothesis then make the eliination cutoff
   // more stringent and repeat
   if (gdwNumHyp > (MAXHYP / 8)) {
      dwElim--;
      if (dwElim)
         goto findlowestscore;

      // if we get here then there are lots of hypothesis, all
      // with the same score
      KeepOnlyAFewHyp (MAXHYP / 8);
   }

   // expand all the hypothesis that haven't advanced enough
   DWORD i;
   DWORD dwNum;
   BOOL  fFoundOneThatNeedsExpanding;
   fFoundOneThatNeedsExpanding = FALSE;
   dwNum = gdwNumHyp;
   for (i = 0; i < dwNum; i++) {
      if (!gapHyp[i])
         continue;

      if (fExpandCorrect) {
         if (gapHyp[i]->m_dwCorrectIndex < dwWordIndex) {
            fFoundOneThatNeedsExpanding = TRUE;
            ExpandHypothesis(i);
         }
      }
      else {   // expand the recognizer list
         if (gapHyp[i]->m_dwRecogIndex < dwWordIndex) {
            fFoundOneThatNeedsExpanding = TRUE;
            ExpandHypothesis(i);
         }
      }
   }

   // if we didn't need to expand anyting then return
   if (!fFoundOneThatNeedsExpanding)
      return;

   // else, we expanded something, so eliminate blanks and redo
   // all the steps
   RemoveBlanks();
   dwElim = ELIMDEFAULT; // if more than 5 off then eliminate
   goto findlowestscore;
}



/**********************************************************************
CompareData - This compares the two blocks of data, in gListCorrect and gListRecog,
   and returns a SINGLE hypothesis object that has the the score
   and info about which words were eliminated.

It does the following
   - Start out with a single hypothesis object
   - Loop on expandhypothesisOneLevel until can't expand anymore
   - Eliminate all hypothesis that aren't top scorers
   - Frees everything except one of the top scorers
   - Returns that one
*/
CHyp *CompareData (void)
{

   // remember the maximum results
   gdwMaxResultSize = gpListCorrect->GetNumElems() + gpListRecog->GetNumElems()+ 2 + ELIMDEFAULT;

   // start out with a blank hypothesis
   gdwNumHyp = 1;
   gapHyp[0] = new CHyp;
   gapHyp[0]->Init(0,0);

   // loop through all the words in the correct recognition text
   DWORD i;
   DWORD dwNum;
   dwNum = gpListCorrect->GetNumElems();

   for (i = 0; i < dwNum; i++)
      ExpandHypothesisOneLevel (i+1, TRUE);

   // at this point all of the data in the correct list has been
   // accounted for, but maybe not everything in the recognizer
   // list. So, expand that. First though, find the highest point
   // it's gotten into the recognizer data
   DWORD dwLowest;
   dwLowest = (DWORD)-1;
   for (i = 0; i < gdwNumHyp; i++)
      if (gapHyp[i])
         dwLowest = min(dwLowest, gapHyp[i]->m_dwRecogIndex);

   dwNum = gpListRecog->GetNumElems();
   for (i = dwLowest; i < dwNum; i++)
      ExpandHypothesisOneLevel (i+1, FALSE);

   // find the lowest score
   dwLowest = FindLowestScore();

   // eliminate anything except the lowest score
   RemoveHighScores (dwLowest+1);
   RemoveBlanks();

   // eliminate everything except the first hypothesis
   KeepOnlyAFewHyp(1);

   // return what we've got
   return gdwNumHyp ? gapHyp[0] : NULL;
}


/**********************************************************************
ParseFileIntoList - Parses a file and writes each of the parse strings
   into a list as a different element.

inputs
   char     *pszCorrect - correct text
   char     *pszRecog - recognition text
returns
   BOOL - TRUE if both succede
*/
BOOL ParseFileIntoList (char *pszCorrect, char *pszRecog)
{
   CParse   cNew;
   CParse   cOrig;

   // Init
   if (!cNew.Init(pszRecog)) {
#ifdef DISPLAYRESULTS
      printf ("Can't open recognized file\n");
#endif
      return FALSE;
   }
   if (!cOrig.Init(pszCorrect)) {
#ifdef DISPLAYRESULTS
      printf ("Can't open correct file.\n");
#endif
      return FALSE;
   }
   cNew.m_fWordMode = gfWordMode;
   cOrig.m_fWordMode = gfWordMode;

   // loop through the parse files and pull out the words
   char  szHuge[100000];
   while (cNew.Next (szHuge, sizeof(szHuge)))
      if (szHuge[0])
         gpListRecog->AddElem (szHuge, strlen(szHuge)+1);
   while (cOrig.Next (szHuge, sizeof(szHuge)))
      if (szHuge[0])
         gpListCorrect->AddElem (szHuge, strlen(szHuge)+1);

   return TRUE;
}

#ifdef DISPLAYRESULTS
/**********************************************************************
DisplayHypothesis - Prints the hypothesis comparison (showing the errors
   generated) out to the screen with printf
*/
void DisplayHypothesis (CHyp *pHyp)
{
   printf ("\n");

   // repeat the sequence
   DWORD dwCorrect = 0;
   DWORD dwRecog = 0;
   DWORD i;
   char  *psz1, *psz2;
   for (i = 0; i < pHyp->m_dwResPosn; i++) {
      // get the strings
      psz1 = (char*) gpListRecog->GetElem(dwRecog);
      psz2 = (char*) gpListCorrect->GetElem(dwCorrect);

      switch (pHyp->m_pResults[i]) {
      case HYPAT_INSERTED:
         printf (gfWordMode ? "\nIns  -  %-20s %-20s" : "(Inserted %s%s)\n", "",
            psz1);
         dwRecog++;
         break;
      case HYPAT_DELETED:
         printf (gfWordMode ? "\nDel  -  %-20s %-20s" : "(Deleted %s%s)\n",
            psz2, "");
         dwCorrect++;
         break;
      case HYPAT_SAMEORWRONG:
         if (CompareStrings (psz1,psz2)) {
            if (gfWordMode)
               printf ("\nCor  -  %-20s %-20s", psz2, psz1);
            else
               printf (gfWordMode ? "%s " : "%s\n", psz1);
         }
         else {
            printf (gfWordMode ? "\nSub  -  %-20s %-20s" : "(%s -> %s)\n",
               psz2, psz1);
         }
         dwCorrect++;
         dwRecog++;
         break;
      }

   }
   return;
}

#endif


/*********************************************************************
Diff - This takes the difference of two files and calculates
the difference.

inputs
   char     *pszRecognized - Text that came from recognizer
   char     *pszCorrect - Correct text
returns
   BOOL - TRUE if success, FALSE if error
*/
BOOL Diff (char *pszRecognized, char *pszCorrect)
{
   gpListCorrect = new CSWList;
   gpListRecog = new CSWList;

   if (!ParseFileIntoList (pszCorrect, pszRecognized))
      return FALSE;

   gdwRight = gdwWrong = gdwInserted = gdwDeleted = 0;

   // compare the data
   CHyp *pHyp;
   pHyp = CompareData ();
   if (!pHyp)
      return FALSE;


   // store the values away
   gdwRight = pHyp->m_dwRight;
   gdwWrong = pHyp->m_dwWrong;
   gdwInserted = pHyp->m_dwInserted;
   gdwDeleted = pHyp->m_dwDeleted;

#ifdef DISPLAYRESULTS
   DisplayHypothesis(pHyp);
#endif

   delete pHyp;
   delete gpListCorrect;
   delete gpListRecog;

   return TRUE;
}

#ifdef DISPLAYRESULTS
/*****************************************************************************
main - Main entry point
*/
int main (int argc, char **argv)
{
if (argc != 4) {
	// Short instructions
	printf ("%s <Recognizer Output> <Correct Words> <-w or -l>\n", argv[0]);
   printf ("This compares the output from a recognizer with what was\n");
   printf ("supposed to be recognized and comes up with an error score.\n\n");
	printf ("<Recognizer output> - Words from recognizer\n\n");
	printf ("<Correct words> - File with what should have been recognized. \n\n");
	printf ("<-w or -l> - Use -w to do word difference, -l to do line difference. \n\n");
	return 0;
	};

if (!stricmp(argv[3], "-w")) {
   printf ("Word difference\n");
   gfWordMode = TRUE;
}
else {
   printf ("Line difference\n");
   gfWordMode = FALSE;
}

// Compare
if (!Diff (argv[1], argv[2]))
   return -1;

// Show statistics
DWORD dwNumWords = gdwRight +
        gdwWrong + gdwDeleted;
DWORD dwNumWrong = gdwWrong +
         gdwInserted + gdwDeleted;

printf ("\n\n*** Statistics ***\n");
printf ("Total number of words = %d\n", (int) dwNumWords);
printf ("Correct = %d\n", (int) gdwRight);
printf ("Misrecognized = %d\n", (int) gdwWrong);
printf ("Inserted = %d\n", (int) gdwInserted);
printf ("Deleted  = %d\n", (int) gdwDeleted);
printf ("Error rate = %f\n",
         (float) dwNumWrong / (float) dwNumWords);
printf ("\n");
printf ("\n");

return 0;
}
#endif

/*****************************************************************************
CalcAccuracy - Calculates the accuracy

inputs
   char     *pszRecog - recognizer output
   char     *pszCorrect - correct text file
   float    *fWordError - Filled with the word error rate
   float    *fLineError - Filled with the line error rate
returns
   BOOL - TRUE if succede, FALSE if fail
*/
BOOL CalcAccuracy (char *pszRecog, char *pszCorrect,
                   float *fWordError, float *fLineError)
{
   DWORD dwNumWords, dwNumWrong;
   *fWordError = *fLineError = 100.0f;

   // calculate word error rate
   gfWordMode = TRUE;
   gdwRight = gdwWrong = gdwInserted = gdwDeleted = 0;

   if (!Diff (pszRecog, pszCorrect))
      return FALSE;
   dwNumWords = gdwRight + gdwWrong + gdwDeleted;
   dwNumWrong = gdwWrong + gdwInserted + gdwDeleted;
   *fWordError = 100.0f * dwNumWrong / dwNumWords;

     /* add rt */
   WInsertions = gdwInserted;
   WDeletions =  gdwDeleted;
   WSubstitutions = gdwWrong;
   WRight = gdwRight;
  /* end add rt */

  // calculate line error rate
   gfWordMode = FALSE;
   gdwRight = gdwWrong = gdwInserted = gdwDeleted = 0;

   if (!Diff (pszRecog, pszCorrect))
      return FALSE;
   dwNumWords = gdwRight + gdwWrong + gdwDeleted;
   dwNumWrong = gdwWrong + gdwInserted + gdwDeleted;
   *fLineError = 100.0f * dwNumWrong / dwNumWords;


   return TRUE;
}

