// PrintOpt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionPage dialog

class CPrintOptionPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrintOptionPage)

// Construction
public:
	CPrintOptionPage();
	~CPrintOptionPage();

// Dialog Data
	//{{AFX_DATA(CPrintOptionPage)
	enum { IDD = IDD_PRINTOPTION };
	CSpinButtonCtrl	m_spinMarginTop;
	CSpinButtonCtrl	m_spinMarginRight;
	CSpinButtonCtrl	m_spinMarginLeft;
	CSpinButtonCtrl	m_spinMarginBottom;
	CSpinButtonCtrl	m_spinColWidth;
	CSpinButtonCtrl	m_spinColumns;
	CSpinButtonCtrl	m_spinColSpace;
	CButton	m_checkSelectionOnly;
	CButton	m_checkResetSelectionLineNum;
	CButton	m_checkLineNum;
	CSpinButtonCtrl	m_spinLineSpace;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrintOptionPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPrintOptionPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckprintlinenum();
	afx_msg void OnCheckresetselectionlinenum();
	afx_msg void OnCheckselectiononly();
	afx_msg void OnChangeEditcolspace();
	afx_msg void OnChangeEditcolumns();
	afx_msg void OnChangeEditcolwidth();
	afx_msg void OnChangeEditmarginbottom();
	afx_msg void OnChangeEditmarginleft();
	afx_msg void OnChangeEditmarginright();
	afx_msg void OnChangeEditmargintop();
	afx_msg void OnChangeEditLinespace();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
