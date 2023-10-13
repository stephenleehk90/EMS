#if !defined(AFX_ATTACHFILES_H__7E927483_0921_4FEF_B17B_53556E3AE0FE__INCLUDED_)
#define AFX_ATTACHFILES_H__7E927483_0921_4FEF_B17B_53556E3AE0FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AttachFiles.h : header file
//
#include "resource.h"
#include "filemgrbase.h"
/////////////////////////////////////////////////////////////////////////////
// CAttachFiles dialog

class CAttachFiles : public CDialog
{
// Construction
public:
	BOOL DownLoad(CString strSourceName, CString & strDestName);
	BOOL CreateDir(const CString & strDir);
	BOOL UpLoad(CString & strSourceName, CString strDestName);
	BOOL DeleteFile(CString strFileName,CString strAttachName);
	BOOL m_bModify;
	CFileMgrBase* m_pMgr;
	void InitList();
	CAttachFiles(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAttachFiles)
	enum { IDD = IDD_ATTACH };
	CRowListCtrlEx	m_FileList;
	CString	m_strFileCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAttachFiles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAttachFiles)
	afx_msg void OnDelete();
	afx_msg void OnDownload();
	afx_msg void OnUpload();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATTACHFILES_H__7E927483_0921_4FEF_B17B_53556E3AE0FE__INCLUDED_)
