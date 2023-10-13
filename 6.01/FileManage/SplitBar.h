// SplitBar.h : header file
//

#define		SPLITTERBAR_WIDTH	2
/////////////////////////////////////////////////////////////////////////////
// CSplitterBar window

class CSplitterBar : public CWnd
{
// Construction
public:
	CSplitterBar();

// Attributes
protected:
	RECT	m_rcArea;
	BOOL	m_bLButtonDown;
	CWnd*	m_pParent;

	CWnd*	m_pwndRight;
	CWnd*	m_pwndLeft;

public:

// Operations
public:
	BOOL Create(CWnd* pParent, RECT* pRect);
	BOOL SetSplitWindows(CWnd* pwndLeft, CWnd* pwndRight);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSplitterBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplitterBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
