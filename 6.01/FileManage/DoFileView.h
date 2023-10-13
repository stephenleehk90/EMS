#if !defined(AFX_DOFILEVIEW_H__51267C64_E4C5_49FC_9851_60778147E247__INCLUDED_)
#define AFX_DOFILEVIEW_H__51267C64_E4C5_49FC_9851_60778147E247__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DoFileView.h : header file
//
#include "resource.h"
#include "dofilemgr.h"
#include "filelistctrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDoFileView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDoFileView : public CFormView
{
	CImageList m_ImageList;
	CImageList m_stateImageList;
	CFont m_fontFace;

protected:
	CDoFileView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDoFileView)

// Form Data
public:
	//{{AFX_DATA(CDoFileView)
	enum { IDD = IDD_DOFILEV };
	CListCtrlEx	m_FileListCtr;
	CString	m_strFileEdit;
	BOOL	m_bCheck;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	int m_nListItem;
	void InitImageList(CListCtrl *pListCtrl);
	void ListFile();
	void InitList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoFileView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
protected:
	virtual ~CDoFileView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDoFileView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeycheck();
	afx_msg void OnQuery();
	afx_msg void OnChangeFileedit();
	afx_msg void OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdoRecord();
	afx_msg void OnRdoComment();
	afx_msg void OnRdoCopy();
	afx_msg void OnRdoSource();
	afx_msg void OnUnlock();
	afx_msg void OnPrintfile();
	afx_msg void OnAddToStory();
	//}}AFX_MSG
	afx_msg LRESULT OnGetListCtrlTextClr(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOFILEVIEW_H__51267C64_E4C5_49FC_9851_60778147E247__INCLUDED_)
