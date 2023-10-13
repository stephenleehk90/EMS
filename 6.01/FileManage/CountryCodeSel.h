#if !defined(AFX_COUNTRYCODESEL_H__79E75AF7_5AFA_48FE_A489_648722ABB234__INCLUDED_)
#define AFX_COUNTRYCODESEL_H__79E75AF7_5AFA_48FE_A489_648722ABB234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CountryCodeSel.h : header file
//
#include "DoFileMgr.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CCountryCodeSel dialog

class CCountryCodeSel : public CDialog
{
// Construction
public:
	CDoFileMgr* m_pMgr;
	CCountryCodeSel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCountryCodeSel)
	enum { IDD = IDD_COUNTRYCODE };
	CComboBox	m_cmbCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCountryCodeSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCountryCodeSel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COUNTRYCODESEL_H__79E75AF7_5AFA_48FE_A489_648722ABB234__INCLUDED_)
