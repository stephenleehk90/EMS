//{{AFX_INCLUDES()
#include "crystalreportviewer9.h"
//}}AFX_INCLUDES
#if !defined(AFX_RECORDREPORT_H__325268B9_8C6E_4E79_9A13_009BC5585878__INCLUDED_)
#define AFX_RECORDREPORT_H__325268B9_8C6E_4E79_9A13_009BC5585878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordReport.h : header file
//
#import "..\ocx\craxdrt9.dll" no_namespace
/////////////////////////////////////////////////////////////////////////////
// CRecordReport dialog

class CRecordReport : public CDialog
{
// Construction
public:
	CString m_strFileCode;
	CRecordReport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecordReport)
	enum { IDD = IDD_RECORDREPORT };
	CCrystalReportViewer9	m_Viewer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordReport)
	afx_msg void OnRefreshButtonClickedRecordreport(BOOL FAR* UseDefault);
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDREPORT_H__325268B9_8C6E_4E79_9A13_009BC5585878__INCLUDED_)
