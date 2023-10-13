#if !defined(AFX_editFrameNew_H__90A11E90_AF82_4863_8703_D7458460EFC7__INCLUDED_)
#define AFX_editFrameNew_H__90A11E90_AF82_4863_8703_D7458460EFC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// editFrameNew.h : header file
//
#include "ShowMsgDlg.h"

/////////////////////////////////////////////////////////////////////////////
// editFrameNew frame

class editFrameNew : public CMDIChildWnd
{
	DECLARE_DYNCREATE(editFrameNew)
protected:
	editFrameNew();           // protected constructor used by dynamic creation

// Attributes
public:
	CDlgDisplyPicture *m_pDlgDisplyPicture;
	CShowMsgDlg	m_dlgMsg;
	BOOL  m_bCaptionPicShowed;
	int g_nSpecifyFontAndToolbar;
	BOOL bScrollView;
//	void SetTextToUpperView(CString strText, CString strTextVersion);
	void SetTextToUpperView(CString strText, CString strTextVersion, CString strModHistory);
	void ChangeUpperViewFont();

	BOOL GetUpperViewMode();
	BOOL SetUpperViewMode(BOOL bMode);
	void SwitchingModeOnTop();
	void SetUpperViewEditDoc(editDoc* m_pDoc);
	void UpdateUpperCommentModeView();
	void UpdateSwitchModeFlag(BOOL bFlag);
	BOOL GetUpperProcessingFlag();
	CComboBox m_comboBox;
	void KillUpperToopTips();
	void KillEditToopTips();
	BOOL GetEditStatus();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(editFrameNew)
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~editFrameNew();

	CImageList m_ToolBarImageList;
	CImageList m_ToolBarDisableImageList;
	int CreateToolBarOnMainFrm(UINT nID,CWnd *pParent=NULL);
	void SetToolBarBitmap();
	void CreateModifyToolbar();

	CSplitterWnd m_wndSplitter;

	// Generated message map functions
	BOOL m_bCreateToolbar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndModifyToolBar;
	
	//{{AFX_MSG(editFrameNew)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnFocusCombo();
	afx_msg void OnKillFocusCombo();
	afx_msg void OnCaptionPicture();
	afx_msg void OnUpdateCaptionPicture(CCmdUI* pCmdUI);
	afx_msg BOOL PreTranslateMessage(MSG* pMsg) ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_editFrameNew_H__90A11E90_AF82_4863_8703_D7458460EFC7__INCLUDED_)
