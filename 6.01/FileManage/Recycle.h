#if !defined(AFX_RECYCLE_H__CD4726C1_16A9_4199_9628_F0DC1B96ACD8__INCLUDED_)
#define AFX_RECYCLE_H__CD4726C1_16A9_4199_9628_F0DC1B96ACD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Recycle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecycle dialog

class CRecycle : public CDialog
{
// Construction
public:
	void LoadDeletedFile();
	void InitList();
	void Refresh();
	CRecycle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecycle)
	enum { IDD = IDD_RECYCLE };
	CRowListCtrlEx	m_File;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecycle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecycle)
	virtual BOOL OnInitDialog();
	afx_msg void OnClear();
	afx_msg void OnRestore();
	BOOL DeleteFile(CString strFileName, CString strAttachName);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECYCLE_H__CD4726C1_16A9_4199_9628_F0DC1B96ACD8__INCLUDED_)
