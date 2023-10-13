#if !defined(AFX_PICLISTCTRL_H__68B93B09_4429_4EC9_B070_5F97366F0767__INCLUDED_)
#define AFX_PICLISTCTRL_H__68B93B09_4429_4EC9_B070_5F97366F0767__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPicListCtrl window
#define WM_LEAVEPIC	WM_USER+100
class CPicListCtrl : public CListCtrl
{
// Construction
public:
	CPicListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	HWND m_hParent;
	virtual ~CPicListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPicListCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICLISTCTRL_H__68B93B09_4429_4EC9_B070_5F97366F0767__INCLUDED_)
