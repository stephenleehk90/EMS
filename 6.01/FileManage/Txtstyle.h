// TxtStyle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextStylesPage dialog

class CTextStylesPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTextStylesPage)

// Construction
public:
	CTextStylesPage();
	~CTextStylesPage();

// Dialog Data
	//{{AFX_DATA(CTextStylesPage)
	enum { IDD = IDD_TEXTSTYLES };
//	CButton	m_BoldCheckBox;
//	CListBox	m_StyleList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTextStylesPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CSpinButtonCtrl	m_spinAutoSaveTime;
	CButton	m_checkAutoSave;
	//}}AFX_VIRTUAL

// Attrib
private:
	BOOL			m_bSysChange;
	int				m_nStyleNum;
	int				m_nCurStyle;
	CStringArray*	m_parrayStyleName;
	LOGFONT*		m_plfFont;
	int*			m_pnLineSpace;
	int*			m_pnCharSpace;
	//=============add by cy ============//
	LOGFONT*		m_plfFontP;
	int*			m_pnLineSpaceP;
	int*			m_pnCharSpaceP;
	//============End============//
	
	// Operator
private:
	void			StylePreview();
	void PrintSameDisplay();
	void InitPrintInfo();
	void InitDispInfo();
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTextStylesPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListstyles();
	afx_msg void OnChangeEditcharspace();
	afx_msg void OnChangeEditlinespace();
	afx_msg void OnBttnsetfont();
	afx_msg void OnPasint();
	afx_msg void OnDisplay();
	afx_msg void OnPaint();
	afx_msg void OnRadioPrint();
	afx_msg void OnCheckSame();
	afx_msg void OnChangeEditautosavetime();
	afx_msg void OnCheckautosave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
