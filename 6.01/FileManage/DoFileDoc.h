#if !defined(AFX_DOFILEDOC_H__52BCEEA1_6C2B_4B51_9F02_58533EADB993__INCLUDED_)
#define AFX_DOFILEDOC_H__52BCEEA1_6C2B_4B51_9F02_58533EADB993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DoFileDoc.h : header file
//
#include "dofilemgr.h"
/////////////////////////////////////////////////////////////////////////////
// CDoFileDoc document

class CDoFileDoc : public CDocument
{
protected:
	CDoFileDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDoFileDoc)

// Attributes
public:
	CDoFileMgr m_Mgr;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoFileDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDoFileDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDoFileDoc)
	afx_msg void OnEdit();
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOFILEDOC_H__52BCEEA1_6C2B_4B51_9F02_58533EADB993__INCLUDED_)
