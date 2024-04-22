extern HINSTANCE       ghInstance;

#define DIRSIZE     512
#define PEOPLECOUNT 5

#define ADMIN_DONE          1
#define ADMIN_LEAVEMESSAGE  2

class CCallAnswer : public CCall {
    private:
        PITELINFO       m_pITelInfo;
        PCTelControl    m_pTCGrammar;
        PCTelControl    m_pTCYesNo;
        PCTelControl    m_pTCRecord;
        PCTelControl    m_pTCPhoneNum;
        PCTelControl    m_pTCExtension;
        PCSTTTSQueue    m_pQueue;

        BOOL    VerifyHangUp (void);
        DWORD   GetMessageInfo (void);
        DWORD   GetForwardNumber (DWORD);
        DWORD   DoAdministration (DWORD);
        DWORD   PlayMessages (DWORD);
        DWORD   PlayIt (PSTR, FILETIME, DWORD);
        DWORD   ProcessForwardRequest (DWORD);

	public:
        CCallAnswer  (void);
        ~CCallAnswer (void);

        HRESULT Init(PITELINFO);
        HRESULT Shutdown(void);
        HRESULT DoPhoneCall(void);
};
typedef CCallAnswer *PCCallAnswer;


class CTelMainPCAnswer : public CTelMainPC {
   public:
        HRESULT CreateLineObject(PCCALL *);
};
typedef CTelMainPCAnswer *PCTELMAINPCAnswer;


class CTelMainTAPIAnswer : public CTelMainTAPI14 {
   public:
        HRESULT CreateLineObject(PCCALL *);
};
typedef CTelMainTAPIAnswer *PCTELMAINTAPIAnswer;
