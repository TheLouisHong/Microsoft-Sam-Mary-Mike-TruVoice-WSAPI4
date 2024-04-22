class CCallPizza : public CCall {
    private:
        PITELINFO       m_pITelInfo;
        PCTelControl    m_pTCPhoneNum;
        PCTelControl    m_pTCGrammar;
        PCTelControl    m_pTCYesNo;
        PCTelControl    m_pTCRecord;
        PCSTTTSQueue    m_pQueue;

        PWSTR           m_pTelNum;
        DWORD           m_dwTelNumSize;
        WCHAR           m_wszSize[64];
        WCHAR           m_wszToppings[256];
        BOOL            m_fKnown;    // caller has visited before


        BOOL    VerifyHangUp (void);
        DWORD   GetPhoneNumber (void);
        DWORD   GetPizzaInfo (void);

	public:
        CCallPizza  (void);
        ~CCallPizza (void);

        HRESULT Init(PITELINFO);
        HRESULT Shutdown(void);
        HRESULT DoPhoneCall(void);
};
typedef CCallPizza *PCCALLPIZZA;


class CTelMainPCPizza : public CTelMainPC {
   public:
        HRESULT CreateLineObject(PCCALL *);
};
typedef CTelMainPCPizza *PCTELMAINPCPIZZA;


class CTelMainTAPIPizza : public CTelMainTAPI14 {
   public:
        HRESULT CreateLineObject(PCCALL *);
};
typedef CTelMainTAPIPizza *PCTELMAINTAPIPIZZA;
