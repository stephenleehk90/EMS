#if !defined(AFX_OPTIONMODIFYDLG_H__0C866973_3BF8_4BAD_9198_233D46D4F9FC__INCLUDED_)
#define AFX_OPTIONMODIFYDLG_H__0C866973_3BF8_4BAD_9198_233D46D4F9FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionModifyDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// COptionModifyDlg dialog
class COptionModifyDlg : public CDialog
{
// Construction
public:
	COptionModifyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionModifyDlg)
	enum { IDD = IDD_OPTIONMODIFY };
	CListCtrl	m_lstVersion;
	CString m_strModHistory;
	BOOL    m_bOldVersion;
	int     m_nSelVersion;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionModifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionModifyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnGetOldVersion();
	afx_msg void OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONMODIFYDLG_H__0C866973_3BF8_4BAD_9198_233D46D4F9FC__INCLUDED_)
