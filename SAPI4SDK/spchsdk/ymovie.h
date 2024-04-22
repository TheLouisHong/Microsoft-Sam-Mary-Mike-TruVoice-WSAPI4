extern HINSTANCE       ghInstance;

class CCallMovie : public CCall {
    private:
        PITELINFO       m_pITelInfo;
        PCTelControl    m_pTCGrammar;
        PCTelControl    m_pTCYesNo;
        PCSTTTSQueue    m_pQueue;

        BOOL    VerifyHangUp (void);
        DWORD   GetMovieInfo (void);

	public:
        CCallMovie  (void);
        ~CCallMovie (void);

        HRESULT Init(PITELINFO);
        HRESULT Shutdown(void);
        HRESULT DoPhoneCall(void);
};
typedef CCallMovie *PCCALLMovie;


class CTelMainPCMovie : public CTelMainPC {
   public:
        HRESULT CreateLineObject(PCCALL *);
};
typedef CTelMainPCMovie *PCTELMAINPCMovie;


class CTelMainTAPIMovie : public CTelMainTAPI14 {
   public:
        HRESULT CreateLineObject(PCCALL *);
};
typedef CTelMainTAPIMovie *PCTELMAINTAPIMovie;
