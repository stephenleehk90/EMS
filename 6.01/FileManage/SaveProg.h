#if !defined(AFX_SAVEPROG_H__C19DEE21_A3B1_11D6_963B_0050BAF27B24__INCLUDED_)
#define AFX_SAVEPROG_H__C19DEE21_A3B1_11D6_963B_0050BAF27B24__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SaveProg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSaveProg dialog

class CSaveProg : public CDialog
{
// Construction
public:
	void SetProgress(int nProgress);
	void SetStatus(const CString &str);
	CSaveProg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveProg)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_Progress;
	CString	m_status;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveProg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CSaveProg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CDoSaveProg  
{
private:
	CDoSaveProg();
	CSaveProg *pDlg;
public:
	~CDoSaveProg();
	static CDoSaveProg *Instance();
	void SetStatus(const CString &str);
	void SetProgress(int nProgress);
	void ShowDialog( int nCmdShow );
public:
	void Destroy();
	static CDoSaveProg * pInstance;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEPROG_H__C19DEE21_A3B1_11D6_963B_0050BAF27B24__INCLUDED_)


