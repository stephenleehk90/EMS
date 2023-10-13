// OptionSt.h : header file
//
#include "TxtStyle.h"
#include "EditOpt.h"
#include "PrintOpt.h"

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

class COptionSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionSheet)

// Construction
public:
	COptionSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CTextStylesPage		m_TextStylesPage;
	CEditOptionPage		m_EditOptionPage;
	CPrintOptionPage	m_PrintOptionPage;

// Operations
public:
	void AddPages();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
