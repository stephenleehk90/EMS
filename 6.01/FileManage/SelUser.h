#if !defined(AFX_SELUSER_H__42443BAB_F7B5_4950_9953_B517B7E42803__INCLUDED_)
#define AFX_SELUSER_H__42443BAB_F7B5_4950_9953_B517B7E42803__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelUser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelUser dialog

class CSelUser : public CDialog
{
// Construction
public:
	CString m_strUserName;
	void LoadLibUser();
	CSelUser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelUser)
	enum { IDD = IDD_USERSELECT };
	CListBox	m_clbList;
	CTreeCtrl	m_ctcLib;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelUser)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelUser)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedSystree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELUSER_H__42443BAB_F7B5_4950_9953_B517B7E42803__INCLUDED_)
