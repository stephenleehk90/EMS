#if !defined(AFX_DOFILEFRM_H__05FF2EBD_C8E1_4224_A10D_73C348864F51__INCLUDED_)
#define AFX_DOFILEFRM_H__05FF2EBD_C8E1_4224_A10D_73C348864F51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DoFileFrm.h : header file
//
#include "..\include\childmdifrm.h"
#include "dofiledoc.h"
#include "ShowTotalDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CDoFileFrm frame

class CDoFileFrm : public CChildMDIFrm
{
	DECLARE_DYNCREATE(CDoFileFrm)
protected:
	CDoFileFrm();           // protected constructor used by dynamic creation

	CSplitterWndEx m_wndSplitter;
	CSplitterWndEx m_wndSplitter1;
	CSplitterWndEx m_wndSplitter2;
// Attributes
public:
	CComboBox		m_wndTime;
	CComboBox		m_wndFile;
	CShowTotalDlg	m_dlgTotal;
	CStatic			m_staticText1,m_staticText2;
// Operations
public:
//	CString m_strOldFileCode;
	void RefreshFile();
	CDoFileDoc* GetDoc();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoFileFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDoFileFrm();
	// Generated message map functions
	//{{AFX_MSG(CDoFileFrm)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeFilecombo();
	afx_msg void OnSelchangeTimecombo();
	afx_msg void OnDestroy();
	afx_msg void OnGetfile();
	afx_msg void OnUpdateGetfile(CCmdUI* pCmdUI);
	afx_msg void OnTransferfile();
	afx_msg void OnUpdateTransferfile(CCmdUI* pCmdUI);
	afx_msg void OnSigncoloumn();
	afx_msg void OnUpdateSigncoloumn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSendtranslate(CCmdUI* pCmdUI);
	afx_msg void OnSendtranslate();
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void OnRefreshfile();
	afx_msg void OnUpdateRefreshfile(CCmdUI* pCmdUI);
	afx_msg void OnAttach();
	afx_msg void OnUpdateAttach(CCmdUI* pCmdUI);
	afx_msg void OnSigndirect();
	afx_msg void OnUpdateSigndirect(CCmdUI* pCmdUI);
	afx_msg void OnDelete();
	afx_msg void OnLinktodm();
	afx_msg void OnUpdateLinktodm(CCmdUI* pCmdUI);
	afx_msg void OnUnlinktodm();
	afx_msg void OnUpdateUnlinktodm(CCmdUI* pCmdUI);
	afx_msg void OnBtnprint();
	afx_msg void OnUpdateBtnprint(CCmdUI* pCmdUI);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnSpellcheck();
	afx_msg void OnUpdateSpellcheck(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg BOOL DestroyWindow();
	void QueryFileWithID(CString strFileID);
	//}}AFX_MSG
	virtual BOOL PreTranslateMessage(MSG* pMsg) ;
	afx_msg LRESULT OnFileSelChanged(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void SetToolBarBitmap();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOFILEFRM_H__05FF2EBD_C8E1_4224_A10D_73C348864F51__INCLUDED_)
