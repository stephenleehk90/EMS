// Convert.h : header file
//
#ifndef _CONVERTDLG_H
#define _CONVERTDLG_H

/////////////////////////////////////////////////////////////////////////////
// CConvertDlg dialog
//{{AFX_INCLUDES()
#include "imvb.h"
#include "leadctrl17.h"
//}}AFX_INCLUDES

class CDBGraphicMain;
class CConvertDlg : public CDialog
{
	// Construction
public:
	CDBGraphicMain *m_pView;
	long	m_Pixels;
	CConvertDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL Create(); 
	//	CWnd * m_pParent;
	char srcFile[MAX_PATH];
	char dstFile[MAX_PATH];
	short width;
	short height;
	void Destory();
	BOOL ConvertImage(wchar_t *szBmpFile, int width, int height, PhotoInfo *pInfo,BOOL bConv2JPG=FALSE);
	BOOL LoadSourceImage(wchar_t *szSrcImageFile);
	BOOL ConvertImgFileByW_H(LPSTR szSrc, LPSTR szDest, 
		int nImgWidth,
		int nImgHeight,
		BOOL bIsResample);

	long GetPhotoWidth(wchar_t *szFile);
	long GetPhotoHeight(wchar_t *szFile); 

	// Dialog Data
	//{{AFX_DATA(CConvertDlg)
	enum { IDD = IDD_CONVERTPIC }; //leon
	CLeadctrl1	m_Lead1;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConvertDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConvertDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	CString  m_strSourceImage;
};
#endif
