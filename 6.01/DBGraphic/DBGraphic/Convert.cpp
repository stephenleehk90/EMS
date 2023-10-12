// Convert.cpp : implementation file
//

#include "stdafx.h"
#include "DBnew.h"
#include "Convert.h"
#include "MainFrame.h"

//#include "lead.h"
//#include <l_ocx40.h>
//#include "l_bitmap.h"
//#include <LTKRN.H>
//#include <io.h>

#include "lead1.h"
#include "l_ocx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConvertDlg dialog


CConvertDlg::CConvertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConvertDlg)
	//}}AFX_DATA_INIT
	m_Pixels=0;
}


void CConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConvertDlg)
	DDX_Control(pDX, IDC_LEADCTRL1, m_Lead1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConvertDlg, CDialog)
	//{{AFX_MSG_MAP(CConvertDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConvertDlg message handlers



BOOL CConvertDlg::Create() 
{
	// TODO: Add your specialized code here and/or call the base class
	//

	return CDialog::Create(CConvertDlg::IDD, NULL);
}

/*enum ResizeConstants {
	RESIZE_NORMAL,
	RESIZE_RESAMPLE,
	RESIZE_FAVORBLACK,
	RESIZE_FAVORBLACK_OR_RESAMPLE,
};
*/
BOOL CConvertDlg::LoadSourceImage(wchar_t *szSrcImageFile)
{
	if(_taccess(szSrcImageFile, 04) == -1)
	{
		m_pView->DisplayMessage(L"Access photo error.\r\n",0);
		return FALSE;
	}

	SHORT nLoadInfo=0;
	TRY
	{
//		m_pView->DisplayMessage(L"Leadtools load image.\r\n",0);
		nLoadInfo = m_Lead1.Load(szSrcImageFile, 0, 0, 1);	
	}
	CATCH_ALL(e)
	{
		//e->ReportError();    2000.10.9  yulu
		m_pView->DisplayMessage(L"Load photo throws exception.\r\n",0);
		return FALSE;
	}
	END_CATCH_ALL
		
	if (nLoadInfo != 0)
	{
		CString str;
		str.Format(L"Load photo fail, the failcode is: %d\r\n",nLoadInfo);
		const wchar_t* wc_tmp = str;
		m_pView->DisplayMessage(wc_tmp,0);  
		return FALSE;
	}
	m_strSourceImage = szSrcImageFile;
//	m_pView->DisplayMessage(L"Get file info.\r\n",0);
//	m_Lead1.GetFileInfo(szSrcImageFile, 0);
	m_Lead1.GetFileInfo(szSrcImageFile, 0, 0);
	return TRUE;
}

BOOL CConvertDlg::ConvertImage(wchar_t *szBmpFile, int width, int height, PhotoInfo *pInfo,BOOL bConv2JPG)
{
	CString strBmpFile = szBmpFile;
	int iPos = strBmpFile.ReverseFind(_T('\\'));
	if(iPos <= 0)
		iPos = strBmpFile.ReverseFind(_T('/'));
	if(iPos > 0)
		strBmpFile = strBmpFile.Left(iPos);
	if(_taccess(strBmpFile, 02) == -1)
	{
		return FALSE;
	}

//	m_pView->DisplayMessage(L"Get info width.\r\n",0);
	long lWidth=(long)m_Lead1.GetInfoWidth();
//	m_pView->DisplayMessage(L"Get info height.\r\n",0);
	long lHeight=(long)m_Lead1.GetInfoHeight();
	pInfo->nHeight=lHeight;
	pInfo->nWidth=lWidth;
//	m_pView->DisplayMessage(L"Get info bits.\r\n",0);
	pInfo->nBits = m_Lead1.GetInfoBits();
	m_Pixels = lWidth * lHeight;

//	m_pView->DisplayMessage(L"Get info format.\r\n",0);
	int iFormat = m_Lead1.GetInfoFormat();
	int iBits = pInfo->nBits;
	
	long lConvWidth = width < lWidth ? width :  lWidth;
	long lConvHeight = height < lHeight ?  height : lHeight;
	if(((float)lConvWidth) / lWidth <= ((float)lConvHeight) / lHeight)
	{
		lConvHeight = (long)(((float)lHeight) / lWidth) * lConvWidth;
	}
	else
	{
		lConvWidth = (long)(((float)lWidth) / lHeight) * lConvHeight;
	}
	/*
	long lwidth=0L,lheight=0L;
	BOOL  bTrue=FALSE;
	//long w,h;
	if(width>=lWidth&&height>=lHeight)
	{
		lwidth=lWidth;
		lheight=lHeight;
		bTrue=TRUE;
	}
	if(!bTrue)
	{
		if(height>=lHeight)
			lheight=lHeight;
		if(width>=lWidth)
			lwidth=lWidth;
		
	}
	if (!bTrue)
	{
		if(lWidth>lHeight)
		{
			int nWidth;
			if(lwidth!=lWidth)
				nWidth=width;
			else
				nWidth=lwidth;
			if(lheight!=lHeight)
			{
				lheight=nWidth*lHeight;
				float fTemp=(float)lheight/(float)lWidth;
				lheight=(long)(fTemp+0.5);
			}
			if(lwidth!=lWidth)
				lwidth=width;
		}
		else
		{
			int nHeight;
			if(lheight!=lHeight)
				nHeight=height;
			else
				nHeight=lHeight;
			if(lwidth!=lWidth)
			{
				lwidth=nHeight*lWidth;		
				float fTemp=(float)lwidth/(float)lHeight;
				lwidth=(long)(fTemp+0.5);
			}
			if(lheight!=lHeight)
				lheight=height;
			
		}
	}
	*/

	//if(ConvertImgFileByW_H((LPTSTR)(LPCTSTR)m_strSourceImage, szBmpFile , lwidth , lheight , FALSE))
	//			return TRUE;
	//		else
	//			return FALSE;
	
	BOOL bRet = FALSE;
	short nSize;
	TRY
	{
	//	m_pView->DisplayMessage(L"Size image.\r\n",0);
		nSize = m_Lead1.Size((float)lConvWidth,(float)lConvHeight, RESIZE_RESAMPLE);
	}
	CATCH_ALL(e)
	{
		m_pView->DisplayMessage(L"Size photo failed in convert process.\r\n",0);
		bRet = FALSE;
		return bRet;
	}
	END_CATCH_ALL;
	if( nSize == 0)
	{
		TRY
		{
	//		m_pView->DisplayMessage(L"Convert image.\r\n",0);
			CString strExt;
			CString strType = szBmpFile;
			int nIndex = strType.ReverseFind('.');
			strExt = strType.Right(strType.GetLength()-nIndex-1);
			strExt.MakeUpper();
			int nType,nBitPixel;
			if (strExt == "JPG"){
//				nType = FILE_JFIF;
				nType = 10;
				nBitPixel = 24;
			}
			else if (strExt == "GIF") {
				nType = FILE_GIF;
				nBitPixel = 8;
			}
			else if (strExt == "BMP") {
				nType = FILE_BMP;
				nBitPixel = 8;
			}
			else if (strExt == "TIF") {
				nType = FILE_TIF;
				nBitPixel = 24;
			}
			else return FALSE;
//			if(m_Lead1.Save(szBmpFile, bConv2JPG?FILE_JFIF:nType, nBitPixel, 10, 0) == 0)
			if(m_Lead1.Save(szBmpFile, bConv2JPG?10:nType, nBitPixel, 10, 0) == 0)
				bRet = TRUE;
		}
		CATCH_ALL(e)
		{
			//e->ReportError();    2000.10.9  yulu
			m_pView->DisplayMessage(L"Save photo failed in convert process.\r\n",0);
			bRet = FALSE;
		}
		END_CATCH_ALL;
	}
	OnInitDialog();
	return bRet;
}



BOOL CConvertDlg::ConvertImgFileByW_H(LPSTR szSrc, LPSTR szDest, 
						int nImgWidth,
						int nImgHeight,
						BOOL bIsResample)
{
	TRY
	{

	int i_lead_tool_return = 0;

	int nType, nBitPixel;
	int nIndex;
	CString strExt;
	CString strType = szDest;
	nIndex = strType.Find('.');
	strExt = strType.Right(strType.GetLength()-nIndex-1);
	strExt.MakeUpper();
	
	if (strExt == "JPG"){
		nType = FILE_JPEG;
		nBitPixel = 24;
	}
	else if (strExt == "GIF") {
		nType = FILE_GIF;
		nBitPixel = 8;
	}
	else if (strExt == "BMP") {
		nType = FILE_BMP;
		nBitPixel = 8;
	}
	else if (strExt == "TIF") {
		nType = FILE_TIF;
		nBitPixel = 24;
	}
	else return -1;

	    i_lead_tool_return = m_Lead1.LoadResize((LPCTSTR)szSrc, (float)nImgWidth, (float)nImgHeight, RESIZE_RESAMPLE, 0);
		if (i_lead_tool_return != 0) 
		{
			AfxMessageBox(L"Resample Failed!");
			return FALSE;
		}

		int i_ret = m_Lead1.Save((LPCTSTR)szDest, nType, nBitPixel, 50, SAVE_OVERWRITE);
		if (i_lead_tool_return != 0) 
		{
			AfxMessageBox(L"Resample Failed!");
			return FALSE;
		}


		return TRUE;

	}	
	CATCH_ALL(e)
	{
		TCHAR szCause[255];
		e->GetErrorMessage(szCause, 255, NULL);
		return FALSE;
	}
	END_CATCH_ALL
}	

int CConvertDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

BOOL CConvertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_Lead1.ShowWindow(SW_HIDE);
//	m_Lead1.UnlockSupport(L_SUPPORT_EXPRESS, L"sk39uTYp");
//	m_Lead1.UnlockSupport(L_SUPPORT_GIFLZW,  L"78BY1jl78");
//	m_Lead1.UnlockSupport(L_SUPPORT_TIFLZW,  L"oam5302FDio");
//	m_Lead1.UnlockSupport(L_SUPPORT_FPXEXTENSIONS, L"2jds903"); 

//	m_Lead1.IsSupportLocked (L_SUPPORT_EXPRESS);
//	m_Lead1.IsSupportLocked (L_SUPPORT_GIFLZW);
//	m_Lead1.IsSupportLocked (L_SUPPORT_TIFLZW);
//	m_Lead1.IsSupportLocked (L_SUPPORT_GIFLZW);
	// Set to pixel mode
	m_Lead1.SetScaleMode(3);

	//Add by Bob
	//先不讓LeadTools報錯，盡量用我們的代碼接獲錯誤
	m_Lead1.SetEnableMethodErrors(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



long CConvertDlg::GetPhotoWidth(wchar_t *szFile) 
{
	SHORT nLoadInfo=0;
	TRY
	{
		nLoadInfo=m_Lead1.Load(szFile, 0, 0, 1);	
	}
	CATCH_ALL(e)
	{
		return 0;
	}
	END_CATCH_ALL

	long lWidth=(long)m_Lead1.GetInfoWidth();
	
	// TODO: Add your specialized creation code here
	return lWidth;
}


long CConvertDlg::GetPhotoHeight(wchar_t *szFile) 
{
	SHORT nLoadInfo=0;
	TRY
	{
		nLoadInfo=m_Lead1.Load(szFile, 0, 0, 1);	
	}
	CATCH_ALL(e)
	{
		return 0;
	}
	END_CATCH_ALL

	long lHeight=(long)m_Lead1.GetInfoHeight();
	
	// TODO: Add your specialized creation code here
	return lHeight;
}

void CConvertDlg::Destory(){
	CDialog::OnDestroy();
}