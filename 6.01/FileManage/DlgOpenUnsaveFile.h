#if !defined(AFX_DLGOPENUNSAVEFILE_H__918EAD0B_79CA_46A7_B87C_BFB25F72E6FE__INCLUDED_)
#define AFX_DLGOPENUNSAVEFILE_H__918EAD0B_79CA_46A7_B87C_BFB25F72E6FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOpenUnsaveFile.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgOpenUnsaveFile dialog
class UNSAVEFILE
{
public:
	BOOL	bNeedOpen;
	CString strFileName;
};
typedef CArray<UNSAVEFILE,UNSAVEFILE> ARRAY_UNSAVEFILE;
class CDlgOpenUnsaveFile : public CDialog
{
// Construction
public:
	CString m_strPath;
	CDlgOpenUnsaveFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOpenUnsaveFile)
	enum { IDD = IDD_SELUNSAVEFILE };
	CListCtrl	m_lstFile;
	//}}AFX_DATA

	ARRAY_UNSAVEFILE m_arrFile;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOpenUnsaveFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOpenUnsaveFile)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPENUNSAVEFILE_H__918EAD0B_79CA_46A7_B87C_BFB25F72E6FE__INCLUDED_)
