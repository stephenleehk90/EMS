#if !defined(AFX_DOCOLVIEW_H__56960A65_1984_4466_B9BB_B0655CB8CC40__INCLUDED_)
#define AFX_DOCOLVIEW_H__56960A65_1984_4466_B9BB_B0655CB8CC40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DoColView.h : header file
//
#include "filelistctrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDoColView view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
class CDoColView : public CFormView
{
protected:
	CDoColView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDoColView)

// Attributes
public:
	//{{AFX_DATA(CDoFileView)
	enum { IDD = IDD_DOFILEV };
	CListCtrlEx	m_FileListCtr;
	//}}AFX_DATA
	CImageList m_ImageList;
	CImageList m_stateImageList;
	CFont m_fontFace;
// Operations
public:
	void InitImageList();

	void InitList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoColView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
protected:
	virtual ~CDoColView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDoColView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdoRecord();
	afx_msg void OnRdoSource();
	afx_msg void OnRdoComment();
	afx_msg void OnRdoCopy();
	afx_msg void OnUpdateRdoCopy(CCmdUI* pCmdUI);
	afx_msg void OnUnlock();
	afx_msg void OnPrintfile();
	afx_msg void OnAddToStory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCOLVIEW_H__56960A65_1984_4466_B9BB_B0655CB8CC40__INCLUDED_)
