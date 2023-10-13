//{{AFX_INCLUDES()
#include "dtpicker.h"
//}}AFX_INCLUDES
#if !defined(AFX_SELPAPER_H__489BE9C2_E553_4A15_AB85_C3B64C0FEF82__INCLUDED_)
#define AFX_SELPAPER_H__489BE9C2_E553_4A15_AB85_C3B64C0FEF82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelPaper.h : header file
//
#include "resource.h"
#include "dofilemgr.h"
/////////////////////////////////////////////////////////////////////////////
// CSelPaper dialog

class CSelPaper : public CDialog
{
// Construction
public:
	int m_nLibID;
	CString m_strCol;
	CString m_strPaper;
	void InitList();
	CSelPaper(CWnd* pParent = NULL);   // standard constructor
	CDoFileMgr *m_pMgr;
// Dialog Data
	//{{AFX_DATA(CSelPaper)
	enum { IDD = IDD_SELPAPER };
	CButton	m_chkSecret;
	CComboBox	m_combUser;
	CTreeCtrlEx	m_LibTree;
	CListCtrlEx	m_ColumnList;
	CDTPicker	m_btnDate;
	CString	m_strNote;
	BOOL	m_bSecret;
	BOOL	m_bUser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelPaper)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelPaper)
	virtual BOOL OnInitDialog();
	afx_msg void OnUsercheck();
	afx_msg void OnItemchangedColumnlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedLibtree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELPAPER_H__489BE9C2_E553_4A15_AB85_C3B64C0FEF82__INCLUDED_)
