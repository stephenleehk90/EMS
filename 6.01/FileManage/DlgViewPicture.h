//{{AFX_INCLUDES()
#include "lead.h"
//}}AFX_INCLUDES
#if !defined(AFX_DLGVIEWPICTURE_H__B370FCE7_E49E_477A_9AE8_F0FC12DB1988__INCLUDED_)
#define AFX_DLGVIEWPICTURE_H__B370FCE7_E49E_477A_9AE8_F0FC12DB1988__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewPicture.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgViewPicture dialog

class CDlgViewPicture : public CDialog
{
// Construction
public:
	CDlgViewPicture(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgViewPicture)
	enum { IDD = IDD_DLGVIEWPICTURE };
	CLEAD	m_leadCtrl;
	//}}AFX_DATA

	void ShowImage(const CString &strPicFile);
	void AdjustPicturePos(int iPicWidth, int iPicHeight);

	void PicOriginalSize();
	void PicZoomIn();
	void PicZoomOut();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewPicture)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgViewPicture)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HCURSOR m_hCur_BGrab;
	HCURSOR m_hCur_GrabIng;
	// dialog size as you see in the resource view (original size)
	CRect	m_rcOriginalRect;
	// dragging
	BOOL	m_bDragging;
	CPoint	m_ptDragPoint;
	BOOL	m_bHaveHScrollBar;
	BOOL	m_bHaveVScrollBar;
	// actual scroll position
	int		m_nVScrollPos;
	int		m_nHScrollPos;
	// actual dialog height
	int		m_nCurHeight;
	int		m_nCurWidth;

	void EndDrag();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIEWPICTURE_H__B370FCE7_E49E_477A_9AE8_F0FC12DB1988__INCLUDED_)
