#if !defined(AFX_DOCOLFRM_H__ED464E40_5074_4908_AB29_ADAFBED4694B__INCLUDED_)
#define AFX_DOCOLFRM_H__ED464E40_5074_4908_AB29_ADAFBED4694B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DoColFrm.h : header file
//
#include "docoldoc.h"
#include "..\include\childmdifrm.h"
#include "ShowTotalDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CDoColFrm frame

class CDoColFrm : public CChildMDIFrm
{
	DECLARE_DYNCREATE(CDoColFrm)
protected:
	CDoColFrm();           // protected constructor used by dynamic creation
	CSplitterWndEx m_wndSplitter;
	CSplitterWndEx m_wndSplitter2;
	CDoColDoc* m_pDoc;
// Attributes
public:
	CComboBox	m_wndTime;
	CFont m_fontFace;
	CDateTimeCtrl m_wndDTPicker;
	CComboBox	m_wndColumn;
	CStatic		m_staticText1,m_staticText2;
	CShowTotalDlg m_dlgTotal;
	BOOL m_bDateCombo;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoColFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDoColFrm();

	// Generated message map functions
	//{{AFX_MSG(CDoColFrm)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnModifycol();
	afx_msg void OnUpdateModifycol(CCmdUI* pCmdUI);
	afx_msg void OnTranspaper();
	afx_msg void OnUpdateTranspaper(CCmdUI* pCmdUI);
	afx_msg void OnRollback();
	afx_msg void OnUpdateRollback(CCmdUI* pCmdUI);
	afx_msg void OnAttach();
	afx_msg void OnUpdateAttach(CCmdUI* pCmdUI);
	afx_msg void OnSignpaper();
	afx_msg void OnUpdateSignpaper(CCmdUI* pCmdUI);
	afx_msg void OnBackfile();
	afx_msg void OnUpdateBackfile(CCmdUI* pCmdUI);
	afx_msg void OnRefresh();
	afx_msg void OnPrint();
	afx_msg void OnUpdatePrint(CCmdUI* pCmdUI);
	afx_msg void OnRdoCopy();
	afx_msg void OnUpdateRdoCopy(CCmdUI* pCmdUI);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnClose();
	afx_msg BOOL DestroyWindow();
	//}}AFX_MSG
	afx_msg void OnDatetimechangeDtpicker(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
//	CString m_strOldFileCode;
	void RefreshFile();
	CDoColDoc * GetDoc();
	afx_msg void OnSelchangeDatecombo();
	afx_msg void OnSelchangeColumncombo();
	afx_msg LRESULT OnFileSelChanged(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void SetToolBarBitmap();
	void ConditionSelChange();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCOLFRM_H__ED464E40_5074_4908_AB29_ADAFBED4694B__INCLUDED_)
