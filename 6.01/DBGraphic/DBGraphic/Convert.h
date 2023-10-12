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
const SHORT EPS_NOT_SUPPORT = 20408;
const SHORT PDF_FILTER_MISSING = 20358;

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
	BOOL ConvertImage(LPCWSTR szBmpFile, int width, int height, PhotoInfo *pInfo,BOOL bConv2JPG=FALSE);
	BOOL LoadSourceImage(LPCWSTR szSrcImageFile);
	BOOL ConvertImgFileByW_H(LPSTR szSrc, LPSTR szDest, 
						int nImgWidth,
						int nImgHeight,
						BOOL bIsResample);

	long GetPhotoWidth(wchar_t *szFile);
	long GetPhotoHeight(wchar_t *szFile); 
	//Edit by Leon Li 2013-04-15 for load photo info by leadtools
	BOOL LoadPhotoAdditionalInfo(LPCWSTR szSrcImageFile,CString& szAdditionalInfo);	//Load photo addtional info
	BOOL LoadPhotoCaption(LPCWSTR szSrcImageFile,CString& szPhotoCatpion);	//Load photo captioin
	//End Leon Li 2013-04-15
// Edit Stephen  2014-01-06
// For convert EPD to JPG temp file
CString m_strConvertEPSPath;
bool ConvertEPStoJPG(CString strInputPath, CString& strOutputPath);
void SetTempConvertFileName(CString strFile);
// Edit Stephen  2014-01-06
	BOOL LoadPhotoMeta(LPCWSTR szSrcImageFile, CString str_FileId);
	void ReadSafeArrayData(VARIANT v, long &lFist, long &lSecond);
// Dialog Data
	//{{AFX_DATA(CConvertDlg)
	enum { IDD = IDD_CONVERTPIC }; //leon
	CLeadctrl1	m_Lead1;
	CIPTC m_iptc;
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

	static UINT   ThreadFunc(LPVOID lpParam);
	CString m_strInputPath;
	CString m_strOutputPath;
	BOOL m_bThreadInProcess;
	BOOL m_bLoad;
};
#endif
