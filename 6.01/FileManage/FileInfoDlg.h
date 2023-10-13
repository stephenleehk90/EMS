#if !defined(AFX_FILEINFODLG_H__9D52EF94_1564_4399_8409_BE205D1E252D__INCLUDED_)
#define AFX_FILEINFODLG_H__9D52EF94_1564_4399_8409_BE205D1E252D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileInfoDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CFileInfoDlg dialog

class CFileInfoDlg : public CDialog
{
// Construction
public:
	int m_nLibID;
	BOOL m_bModify;
	CString m_strFileCode;
	BOOL m_bNewFile;
	CFileInfoDlg(CWnd* pParent = NULL);   // standard constructor
	void DisableAllWnd();
	CToolTipCtrl m_ToolTip;

	

// Dialog Data
	//{{AFX_DATA(CFileInfoDlg)
	enum { IDD = IDD_FILEINFO };
	CString	m_strTitle;
	CString	m_strAuthor;
	BOOL	m_bLock;
	BOOL	m_bHasPhoto;
	BOOL	m_bSecret;
	BOOL	m_bExclusive;
	CString	m_strStyle;
	BOOL	m_bUrgency;
	double	m_dbWords;

	CEdit edtTest;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileInfoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEINFODLG_H__9D52EF94_1564_4399_8409_BE205D1E252D__INCLUDED_)
