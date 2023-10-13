#if !defined(AFX_DOFILETREEVIEW_H__603111BE_D496_4711_9956_EF7A09A3C22B__INCLUDED_)
#define AFX_DOFILETREEVIEW_H__603111BE_D496_4711_9956_EF7A09A3C22B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DoFileTreeView.h : header file
//
#include "treeviewex.h"
/////////////////////////////////////////////////////////////////////////////
// CDoFileTreeView view

class AFX_EXT_CLASS CDoFileTreeView : public CTreeViewEx
{
protected:
	CDoFileTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDoFileTreeView)
	CFont m_fontFace;
// Attributes
public:

// Operations
public:
	CTreeCtrl * m_pTreeCtrl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoFileTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDoFileTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDoFileTreeView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOFILETREEVIEW_H__603111BE_D496_4711_9956_EF7A09A3C22B__INCLUDED_)
