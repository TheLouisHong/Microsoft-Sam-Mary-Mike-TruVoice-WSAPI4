typedef struct tagGRAMMAR {
   SRHEADER srHeader;         // Grammar file header
   PSRCHUNK psrckLanguage;
   PSRCHUNK psrckCFGWords;
   PSRCHUNK psrckCFGRules;
   PSRCHUNK psrckCFGExportRules;
   PSRCHUNK psrckCFGImportRules;
   PSRCHUNK psrckCFGLists;
   PSRCHUNK psrckLDWords;
   PSRCHUNK psrckLDGroup;
   PSRCHUNK psrckLDSample;
   PSRCHUNK psrckDTopic;
   PSRCHUNK psrckDCommon;
   PSRCHUNK psrckDGroup;
   PSRCHUNK psrckDSample;
   DWORD dwNumWords;
   DWORD dwNumRules;
   DWORD dwNumExports;
   DWORD dwNumImports;
   DWORD dwNumLists;
} GRAMMAR, *PGRAMMAR, *HGRAMMAR;

HGRAMMAR GrammarOpen( PTCHAR pszGrammarFile );
void GrammarClose( HGRAMMAR hGrammar );

/* End of gramlib.h */
