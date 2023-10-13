#if !defined(AFX_CHOSEDICDLG_H__33F93891_D765_4C47_B8F1_6E9A3384BB9D__INCLUDED_)
#define AFX_CHOSEDICDLG_H__33F93891_D765_4C47_B8F1_6E9A3384BB9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChoseDicDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChoseDicDlg dialog

class CChoseDicDlg : public CDialog
{
// Construction
public:
	CChoseDicDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChoseDicDlg)
	enum { IDD = IDD_CHOOSE_DIC };
	CListBox	m_DicList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChoseDicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChoseDicDlg)
	afx_msg void OnApply();
	afx_msg void OnCancel();
	afx_msg void OnChangeAllDef();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOSEDICDLG_H__33F93891_D765_4C47_B8F1_6E9A3384BB9D__INCLUDED_)
