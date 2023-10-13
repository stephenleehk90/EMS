#if !defined(AFX_CLEARFILE_H__3BA4F668_AEEC_483C_B39C_D72184A439C2__INCLUDED_)
#define AFX_CLEARFILE_H__3BA4F668_AEEC_483C_B39C_D72184A439C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClearFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClearFile dialog

class CClearFile : public CDialog
{
// Construction
public:
	void InitList();
	void ListDate();
	CClearFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CClearFile)
	enum { IDD = IDD_CLEARFILE };
	CListCtrl	m_lcFile;
	CListBox	m_lbDate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClearFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CClearFile)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeldir();
	afx_msg void OnSelchangeDate();
	afx_msg void OnDelfile();
	CString GetFileTitle(CString szSrcFile);
	long GetFileWordCount(CString szSrcFile); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLEARFILE_H__3BA4F668_AEEC_483C_B39C_D72184A439C2__INCLUDED_)
