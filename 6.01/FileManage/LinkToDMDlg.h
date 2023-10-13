#if !defined(AFX_LINKTODMDLG_H__DF2D97D1_E972_463B_9DBF_C3DBB18B660A__INCLUDED_)
#define AFX_LINKTODMDLG_H__DF2D97D1_E972_463B_9DBF_C3DBB18B660A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkToDMDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLinkToDMDlg dialog

class CGlobal;

class CLinkToDMDlg : public CDialog
{
// Construction
public:
	CLinkToDMDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLinkToDMDlg)
	enum { IDD = IDD_LINKTODM };
	CDateTimeCtrl	m_dtcDate;
	CTreeCtrl	m_tvLib;
	CListCtrl	m_clcDeptFileInfo;
	BOOL	m_bShowAll;
	COleDateTime	m_odtSelDate;
	//}}AFX_DATA

	int m_nDeptFileInfoId;

private:
	void LoadDMList();
	BOOL IsInArrayLibID(CWordArray &arryLibID,DWORD wLib);
	void InitDMLibTree();
	void InitHeader();
	void InitImageList();
	CImageList      m_ImageListOfTree;
	CImageList		m_ImageListOfList;
	
	bool m_bInitializing;
	CGlobal* m_pGlobal;
	CWordArray m_waFilter;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinkToDMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLinkToDMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedLibtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnCheckShowall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKTODMDLG_H__DF2D97D1_E972_463B_9DBF_C3DBB18B660A__INCLUDED_)
