// EditPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditOptionPage dialog

class CEditOptionPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CEditOptionPage)

// Construction
public:
	CEditOptionPage();
	~CEditOptionPage();

// Dialog Data
	//{{AFX_DATA(CEditOptionPage)
	enum { IDD = IDD_EDITOPTION };
	CSpinButtonCtrl	m_spinAutoSpaces;
	CSpinButtonCtrl	m_spinAutoSaveTime;
	CButton	m_checkAutoSave;
	CButton	m_checkAutoSpaceAfterReturn;
	CButton	m_checkLineEdit;
	CSpinButtonCtrl	m_spinUndoBufSize;
	CSpinButtonCtrl	m_spinLineWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditOptionPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditOptionPage)
	afx_msg void OnChecklineedit();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckautospaceafterreturn();
	afx_msg void OnChangeEditlinewidth();
	afx_msg void OnChangeEditundobufsize();
	afx_msg void OnChangeEditautosavetime();
	afx_msg void OnChangeEditautospaces();
	afx_msg void OnCheckautosave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
