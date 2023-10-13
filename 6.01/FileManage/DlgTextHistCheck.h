#if !defined(AFX_DLGTEXTHISTCHECK_H__0C866973_3BF8_4BAD_9198_233D46D4F9FC__INCLUDED_)
#define AFX_DLGTEXTHISTCHECK_H__0C866973_3BF8_4BAD_9198_233D46D4F9FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTextHistCheck.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTextHistCheck dialog
class CDlgTextHistCheck : public CDialog
{
// Construction
public:
	CDlgTextHistCheck(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTextHistCheck)
	enum { IDD = IDD_TEXT_HIST_CHECK };
	CADORecordset *rs;
	CListCtrl	m_lstFile;
	CListCtrl	m_lcRecord;
	BOOL		m_bThreadInProcess;
	BOOL		m_bRunning;
	int			m_nWrong;
	void Create();

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTextHistCheck)
	protected:
			int		iSortSubItem;
	int		iSortPara;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTextHistCheck)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	afx_msg void OnCancel();
	static UINT BeginBackgroundInsert(LPVOID pParam);
	void BackgroundInsert(); 
	void EndBackgroundInsert();
	void OnInitFileList();
	void OnInitRecordList();
	BOOL GetFileRecord(CListCtrl *pList,CString &strFileCode);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEXTHISTCHECK_H__0C866973_3BF8_4BAD_9198_233D46D4F9FC__INCLUDED_)
