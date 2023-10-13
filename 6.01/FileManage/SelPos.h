#if !defined(AFX_SELPOS_H__DD1D4E44_A644_45FF_9505_D70477575CB9__INCLUDED_)
#define AFX_SELPOS_H__DD1D4E44_A644_45FF_9505_D70477575CB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelPos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelPos dialog

class CSelPos : public CDialog
{
// Construction
public:
	int m_nColID;
	CString m_strLibName;
	int m_nLib;
	BOOL m_bType;
	CString m_strColumn;
	CString m_strPaper;
	void FillSetting();
	CSelPos(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelPos)
	enum { IDD = IDD_FINDPOSITION };
	CComboBox	m_ccbPaper;
	CTreeCtrl	m_ctcLib;
	CListBox	m_clbColumn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelPos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelPos)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnLibradio();
	afx_msg void OnPapradio();
	afx_msg void OnSelchangePapcombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELPOS_H__DD1D4E44_A644_45FF_9505_D70477575CB9__INCLUDED_)
