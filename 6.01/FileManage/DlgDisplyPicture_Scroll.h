//{{AFX_INCLUDES()
#include "lead.h"
//}}AFX_INCLUDES
#if !defined(AFX_DLGDISPLYPICTURE_H__4C900B93_9ADE_4458_9E33_AAFE3BD8876E__INCLUDED_)
#define AFX_DLGDISPLYPICTURE_H__4C900B93_9ADE_4458_9E33_AAFE3BD8876E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDisplyPicture.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplyPicture dialog
#include "DlgViewPicture.h"

class CDlgDisplyPicture : public CDialog
{
// Construction
public:
	CDlgDisplyPicture(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDisplyPicture)
	enum { IDD = IDD_DLGPICTURE };
	CStatic	m_frameViewPic;
	CStatic	m_staSeparable;
	CLEAD	m_leadCtrl;
	//}}AFX_DATA

	void ShowCaptionPicture();
	void SetPictureFile(LPCTSTR lpszPictureFile) ;
	void SetPictureFile(const CStringArray &arrRelPicFile);
	void SetFrameRect(const RECT &rcFrame);
	void SetToolBarImage();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDisplyPicture)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolBar  m_toolBarViewPic;
	CImageList m_barImageList;
	CImageList m_barDisableImageList;
	// Generated message map functions
	//{{AFX_MSG(CDlgDisplyPicture)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnPicinitsize();
	afx_msg void OnPicviewnext();
	afx_msg void OnPicviewpre();
	afx_msg void OnPiczoomin();
	afx_msg void OnPiczoomout();
	//}}AFX_MSG
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private:
	CStringArray m_arrPictureFile;
	int m_iCurPicIndex;
	int	m_iWinMinWidth;
	CDlgViewPicture *m_pDlgViewPicture;

	void AdjustButtonState();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDISPLYPICTURE_H__4C900B93_9ADE_4458_9E33_AAFE3BD8876E__INCLUDED_)
