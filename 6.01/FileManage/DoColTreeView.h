#if !defined(AFX_DOCOLTREEVIEW_H__B8C2801B_FD3B_468C_8F99_D5F5D7832FDC__INCLUDED_)
#define AFX_DOCOLTREEVIEW_H__B8C2801B_FD3B_468C_8F99_D5F5D7832FDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DoColTreeView.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDoColTreeView view
#include "treeviewex.h"
class CDoColTreeView : public CTreeViewEx
{
protected:
	CDoColTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDoColTreeView)
	CFont m_fontFace;
// Attributes
public:

// Operations
public:
	CTreeCtrl * m_pTreeCtrl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoColTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDoColTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDoColTreeView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bInitializing;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCOLTREEVIEW_H__B8C2801B_FD3B_468C_8F99_D5F5D7832FDC__INCLUDED_)
