#if !defined(AFX_CHILDREPFRM_H__D2FD21A1_9582_11D6_8DFE_00104B0E5435__INCLUDED_)
#define AFX_CHILDREPFRM_H__D2FD21A1_9582_11D6_8DFE_00104B0E5435__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ChildRepFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildRepFrm frame

class CChildRepFrm : public CEditProRepaceFrame
{
	DECLARE_DYNCREATE(CChildRepFrm)
protected:
	CChildRepFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void DoSearchAndReplace();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildRepFrm)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bHasDoRep;
	CImageList m_ToolBarImageList;
	CImageList m_ToolBarDisableImageList;
	virtual ~CChildRepFrm();

	// Generated message map functions
	//{{AFX_MSG(CChildRepFrm)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnSearchAndReplace();
	afx_msg void OnScanning();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	virtual BOOL PreTranslateMessage(MSG* pMsg) ;
	DECLARE_MESSAGE_MAP()
private:
	void SetToolBarBitmap();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDREPFRM_H__D2FD21A1_9582_11D6_8DFE_00104B0E5435__INCLUDED_)
