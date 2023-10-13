//{{AFX_INCLUDES()
#include "crystalreportviewer9.h"
//}}AFX_INCLUDES
#if !defined(AFX_COLREPORT_H__6F4D29DC_5255_4322_95E6_DA455A1F91D4__INCLUDED_)
#define AFX_COLREPORT_H__6F4D29DC_5255_4322_95E6_DA455A1F91D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColReport.h : header file
#import "..\ocx\craxdrt9.dll" no_namespace
//

/////////////////////////////////////////////////////////////////////////////
// CColReport dialog

class CColReport : public CDialog
{
// Construction
public:
	CADORecordset * rs;
	CString m_strPreTitle;
	CColReport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColReport)
	enum { IDD = IDD_COLREPORT };
	CCrystalReportViewer9	m_Viewer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColReport)
	afx_msg void OnRefreshButtonClickedColreport(BOOL FAR* UseDefault);
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLREPORT_H__6F4D29DC_5255_4322_95E6_DA455A1F91D4__INCLUDED_)
