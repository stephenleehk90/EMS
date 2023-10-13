#if !defined(AFX_DOCOLDOC_H__279EAD79_4A17_474B_A5DB_9AEBB2349C3A__INCLUDED_)
#define AFX_DOCOLDOC_H__279EAD79_4A17_474B_A5DB_9AEBB2349C3A__INCLUDED_

#include "DoColMgr.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DoColDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDoColDoc document

class CDoColDoc : public CDocument
{
protected:
	CDoColDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDoColDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoColDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	CDoColMgr m_Mgr;
	virtual ~CDoColDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDoColDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCOLDOC_H__279EAD79_4A17_474B_A5DB_9AEBB2349C3A__INCLUDED_)
