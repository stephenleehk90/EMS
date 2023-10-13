#if !defined(AFX_SHOWTOTALDLG_H__989881BB_D650_4353_8A40_FA352EFD64D5__INCLUDED_)
#define AFX_SHOWTOTALDLG_H__989881BB_D650_4353_8A40_FA352EFD64D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowTotalDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowTotalDlg dialog

class CShowTotalDlg : public CDialog
{
// Construction
public:
	CShowTotalDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShowTotalDlg)
	enum { IDD = IDD_TOTAL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowTotalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShowTotalDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWTOTALDLG_H__989881BB_D650_4353_8A40_FA352EFD64D5__INCLUDED_)
