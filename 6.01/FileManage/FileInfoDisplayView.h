//{{AFX_INCLUDES()
#include "lead.h"
//}}AFX_INCLUDES
#if !defined(AFX_FILEINFODISPLAYVIEW_H__C15BC95D_B707_4FD7_B9C9_6EA6A0734C83__INCLUDED_)
#define AFX_FILEINFODISPLAYVIEW_H__C15BC95D_B707_4FD7_B9C9_6EA6A0734C83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileInfoDisplayView.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CFileInfoDisplayView form view
#include "lead.h"
//#include "l_ocx40.h"
#include "l_ocx.h"
#include "PicListCtrl.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFileInfoDisplayView : public CFormView
{
protected:
	CFileInfoDisplayView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFileInfoDisplayView)

// Form Data
public:
	//{{AFX_DATA(CFileInfoDisplayView)
	enum { IDD = IDD_FILEINFODISPLAY };
	CRichEditCtrl	m_edtFileContent;
	CTabCtrl	m_Tab;
	CPicListCtrl	m_lcImage;
	CListCtrlEx	m_lcRecord;
	CString	m_strFileContent;
	CLEAD	m_LeadCtrl;
	//}}AFX_DATA
	CToolTipCtrl m_Tooltip;
// Attributes
public:

// Operations
public:
	BOOL CreateToolTip();
	CString m_strTooltipsText;
	CImageList m_ImageList;
	void InitPicList();
	BOOL m_bPicUpdated;
	CFont m_fontFace;
	BOOL m_bRecordUpdated;
	BOOL m_bContentUpdated;
	CString m_strFilecode;
	void InitFileRecordList();
	void ShowPicInfo(CString strFileCode);
	void ShowFileRecord(CString strFileCode);
	void ShowFileContent(CString strFileCode);

private:
	bool IsInstalled(LPCTSTR lpszFont);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileInfoDisplayView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFileInfoDisplayView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFileInfoDisplayView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnGetdispinfoListimage(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg LRESULT OnFileSelChanged(WPARAM wParam,LPARAM lParam);
	afx_msg BOOL OnToolTipNeedText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
	afx_msg LRESULT OnShowTips(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	bool m_bInitialized;
public:
	afx_msg void OnNMDblclkListimage(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEINFODISPLAYVIEW_H__C15BC95D_B707_4FD7_B9C9_6EA6A0734C83__INCLUDED_)
