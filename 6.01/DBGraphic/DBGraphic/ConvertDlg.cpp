// Convert.cpp : implementation file
//

#include "stdafx.h"
#include "DBGraphic.h"
#include "Convert.h"
#include "DBGraphicMain.h"

#include <tlhelp32.h>

//#include "lead.h"
//#include <l_ocx40.h>
//#include "l_bitmap.h"
//#include <LTKRN.H>
//#include <io.h>

#include "lead1.h"
#include "l_ocx.h"

#include <Magick++.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConvertDlg dialog

bool CConvertDlg::ConvertEPStoJPG(CString strInputPath, CString& strOutputPath)
{
	/*Magick::Image newImage;
	strOutputPath=L"";
	char temp_dest[5120]={0};

	if (m_strConvertEPSPath== L"") return false;
	try
	{

		CString strPath=strInputPath.Left(strInputPath.ReverseFind(L'\\'));
		strOutputPath=strInputPath.Right(strInputPath.GetLength()-strInputPath.ReverseFind(L'\\')-1);
		CString strFileName=strOutputPath.Left(strOutputPath.ReverseFind('.'));
		CString strFileExt=strOutputPath.Mid(strOutputPath.ReverseFind('.'));

		CString strTempPath = m_szSystemTempPath;
		//strOutputPath =  strTempPath + L"\\" + strImageID + L".jpg"; 

		strOutputPath =  strTempPath + L"\\temp_image\\" + m_strConvertEPSPath + L".jpg";

		if(!IsFileExists(strOutputPath))
		{

			CString strInputPath_EPS =  strTempPath + L"\\temp_image\\" + m_strConvertEPSPath + strFileExt;

			if (_waccess(strInputPath,0)==0)
			{
				bool m_bCopyFileSuccess = CopyFile(strInputPath,strInputPath_EPS,FALSE);
				if ( m_bCopyFileSuccess == FALSE )
					return false;
			}
			//
			//	strInputPath = L"D:\\b.eps";
			char temp[5120]={0};
			ZeroMemory(temp, sizeof(temp));
//			int len = WideCharToMultiByte(CP_ACP, 0, strInputPath, -1, temp, sizeof(temp), NULL, NULL);
			int len = WideCharToMultiByte(CP_ACP, 0, strInputPath_EPS, -1, temp, sizeof(temp), NULL, NULL);

			ZeroMemory(temp_dest, sizeof(temp_dest));
			len = WideCharToMultiByte(CP_ACP, 0, strOutputPath, -1, temp_dest, sizeof(temp_dest), NULL, NULL);
			
			strFileExt = strFileExt.MakeLower();
			if (strFileExt == ".eps" || strFileExt == ".pdf" || strFileExt == ".ai")
				newImage.density("300");
			newImage.read(temp);  
//			::std::string des(temp);
//			newImage.read("C:\\founder1\\20140108\\a66 - 複製.eps");  
			newImage.write(temp_dest);
		}
	}
	catch(Magick::Warning &warning)
	{
		newImage.write(temp_dest);
		return true;
	}
	// Image is not usable
	catch(Magick::Error &error)
	{
		//		AfxMessageBox(error.what());
		newImage.isValid(false);
		return false;

	}
	return true;

	*/
	m_strInputPath = strInputPath;
	m_strOutputPath = strOutputPath;
	m_bLoad = FALSE;
	m_bThreadInProcess = TRUE;
	DWORD dwStartTick = GetTickCount();
	HANDLE hThread  = AfxBeginThread(ThreadFunc,this, THREAD_PRIORITY_NORMAL ,CREATE_SUSPENDED); 
	while (1 && m_bThreadInProcess)
	{
		if (m_bThreadInProcess)
		{
			DWORD dwEndTick = GetTickCount();
			if((dwEndTick - dwStartTick) / 1000 > 60 * m_pView->m_ProcessTime)
				break;
		}
		Sleep(1000);
	}
	if (m_bThreadInProcess)
	{
		HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0 );
		// Initialize the process entry structure.
		PROCESSENTRY32 ProcessEntry = { 0 };
		ProcessEntry.dwSize = sizeof( ProcessEntry );
		// Get the first process info.
		BOOL Return = FALSE;
		Return = Process32First( hProcessSnapShot,&ProcessEntry );
		do
		{
		   if (_tcsicmp(ProcessEntry.szExeFile, _T("gswin32c.exe"))==0 || _tcsicmp(ProcessEntry.szExeFile, _T("gswin64c.exe")) == 0) 
		   {
			   HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessEntry.th32ProcessID); //Open Process to terminate
			   TerminateProcess(hProcess,0);
			   CloseHandle(hProcess); //Close Handle }
		   }
		}
		while( Process32Next( hProcessSnapShot, &ProcessEntry ));
		// Close the handle
		CloseHandle( hProcessSnapShot );

		TerminateThread(hThread, 0);
 
		return false;
	}

	strOutputPath = m_strOutputPath;
	return m_bLoad;
}

UINT   CConvertDlg::ThreadFunc(LPVOID lpParam){
	CConvertDlg* pDlg=(CConvertDlg*) lpParam;

	Magick::Image newImage;
	CString strOutputPath = pDlg->m_strOutputPath;
	CString strInputPath = pDlg->m_strInputPath;
	CString m_strConvertEPSPath = pDlg->m_strConvertEPSPath;
	strOutputPath=L"";
	char temp_dest[5120]={0};

	if (m_strConvertEPSPath== L"") return false;
	try
	{

		CString strPath=strInputPath.Left(strInputPath.ReverseFind(L'\\'));
		strOutputPath=strInputPath.Right(strInputPath.GetLength()-strInputPath.ReverseFind(L'\\')-1);
		CString strFileName=strOutputPath.Left(strOutputPath.ReverseFind('.'));
		CString strFileExt=strOutputPath.Mid(strOutputPath.ReverseFind('.'));

		CString strTempPath = m_szSystemTempPath;
		//strOutputPath =  strTempPath + L"\\" + strImageID + L".jpg"; 

		strOutputPath =  strTempPath + L"\\temp_image\\" + m_strConvertEPSPath + L".jpg";

		if(!IsFileExists(strOutputPath))
		{

			CString strInputPath_EPS =  strTempPath + L"\\temp_image\\" + m_strConvertEPSPath + strFileExt;

			if (_waccess(strInputPath,0)==0)
			{
				bool m_bCopyFileSuccess = CopyFile(strInputPath,strInputPath_EPS,FALSE);
				if ( m_bCopyFileSuccess == FALSE )
				{
					pDlg->m_bLoad = false;
					pDlg->m_bThreadInProcess = false;
					return 1;
				}
			}
			//
			//	strInputPath = L"D:\\b.eps";
			char temp[5120]={0};
			ZeroMemory(temp, sizeof(temp));
//			int len = WideCharToMultiByte(CP_ACP, 0, strInputPath, -1, temp, sizeof(temp), NULL, NULL);
			int len = WideCharToMultiByte(CP_ACP, 0, strInputPath_EPS, -1, temp, sizeof(temp), NULL, NULL);

			ZeroMemory(temp_dest, sizeof(temp_dest));
			len = WideCharToMultiByte(CP_ACP, 0, strOutputPath, -1, temp_dest, sizeof(temp_dest), NULL, NULL);
			
			strFileExt = strFileExt.MakeLower();
			if (strFileExt == ".eps" || strFileExt == ".pdf" || strFileExt == ".ai")
				newImage.density("300");
			newImage.read(temp);  
//			::std::string des(temp);
//			newImage.read("C:\\founder1\\20140108\\a66 - 複製.eps");  
			newImage.write(temp_dest);
		}
	}
	catch(Magick::Warning &warning)
	{
		try
		{
			newImage.write(temp_dest);
			pDlg->m_bLoad = true;
			pDlg->m_bThreadInProcess = true;
			pDlg->m_strOutputPath = strOutputPath;
			return 1;
		}
		catch(Magick::Error &error)
		{
			newImage.isValid(false);
			pDlg->m_bLoad = false;
			pDlg->m_bThreadInProcess = false;
			return 1;
		}
	}
	// Image is not usable
	catch(Magick::Error &error)
	{
		//		AfxMessageBox(error.what());
		newImage.isValid(false);
		pDlg->m_bLoad = false;
		pDlg->m_bThreadInProcess = false;
		return 1;
	}
	pDlg->m_bLoad = true;
	pDlg->m_bThreadInProcess = false;
	pDlg->m_strOutputPath = strOutputPath;
	return 1;
}


CConvertDlg::CConvertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConvertDlg)
	//}}AFX_DATA_INIT
	m_Pixels=0;
	m_strConvertEPSPath = L"";
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
BOOL CConvertDlg::LoadSourceImage(LPCWSTR szSrcImageFile)
{
	m_pView->DisplayTraceMessage(L"S LoadSourceImage by Leadtool",0,FALSE);
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
		m_iptc.Load(szSrcImageFile);
	}
	CATCH_ALL(e)
	{
		//e->ReportError();    2000.10.9  yulu
		m_pView->DisplayMessage(L"Load photo throws exception.\r\n",0);
		return FALSE;
	}
	END_CATCH_ALL
	m_pView->DisplayTraceMessage(L"E LoadSourceImage by Leadtool",0,FALSE);


	// Edit Stephen  2013-12-31
	// Handle EPS not supported by Leadtools, convert to JPG using imagenmagock library
	// Edit Stephen  2013-12-31
	if (nLoadInfo == EPS_NOT_SUPPORT || nLoadInfo == PDF_FILTER_MISSING || nLoadInfo != 0) //ERROR_PDF_NOT_ENABLED 20408 
	{
		m_pView->DisplayTraceMessage(L"S LoadSourceImage by imagenmagock",0,FALSE);
		CString strOutPath = L"";
		if  (!ConvertEPStoJPG(szSrcImageFile, strOutPath))
		{
			m_pView->DisplayMessage(L"Load photo throws exception.\r\n",0);
			return FALSE;
		}
		else
		{

			TRY
			{
				nLoadInfo = m_Lead1.Load(strOutPath,0,0,1);
				m_iptc.Load(strOutPath);
			}
			CATCH_ALL(e)
			{
				m_pView->DisplayMessage(L"Load photo throws exception.\r\n",0);
				return FALSE;
			}
			END_CATCH_ALL
			if (nLoadInfo !=0)
			{
				m_pView->DisplayMessage(L"Load photo throws exception.\r\n",0);
				return FALSE;
			}
		}
		m_pView->DisplayTraceMessage(L"E LoadSourceImage by imagenmagock",0,FALSE);
	}
	// Edit Stephen  2013-12-31

		
	if (nLoadInfo != 0)
	{
		CString str;
		str.Format(L"Load photo fail, the failcode is: %d\r\n",nLoadInfo);
		const wchar_t* wc_tmp = str;
		m_pView->DisplayMessage(wc_tmp,0);  
		return FALSE;
	}
	m_strSourceImage = szSrcImageFile;
	return TRUE;
}



BOOL CConvertDlg::ConvertImage(LPCWSTR szBmpFile, int width, int height, PhotoInfo *pInfo,BOOL bConv2JPG)
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
	CString strType = (CString)szDest;
	nIndex = strType.ReverseFind('.');
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

	// Edit Stephen  2013-12-31
	// Handle EPS not supported by Leadtools, convert to JPG using imagenmagock library
	// Edit Stephen  2013-12-31
	if (i_lead_tool_return == EPS_NOT_SUPPORT || i_lead_tool_return == PDF_FILTER_MISSING || i_lead_tool_return != 0) //ERROR_PDF_NOT_ENABLED 20408 
	{
		CString strOutPath = L"";
		if  (!ConvertEPStoJPG((LPCTSTR)szSrc, strOutPath))
		{
			return FALSE;
		}
		else
		{

			TRY
			{
				i_lead_tool_return = m_Lead1.LoadResize(strOutPath, (float)nImgWidth, (float)nImgHeight, RESIZE_RESAMPLE, 0);
			}
			CATCH_ALL(e)
			{
				return FALSE;
			}
			END_CATCH_ALL
			if (i_lead_tool_return !=0)
			{
				return FALSE;
			}
		}
	}
	// Edit Stephen  2013-12-31

	if (i_lead_tool_return != 0) 
	{
		//AfxMessageBox(L"Resample Failed!");
		return FALSE;
	}



		int i_ret = m_Lead1.Save((LPCTSTR)szDest, nType, nBitPixel, 50, SAVE_OVERWRITE);
		if (i_lead_tool_return != 0) 
		{
			//AfxMessageBox(L"Resample Failed!");
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


	// Edit Stephen  2013-12-31
	// Handle EPS not supported by Leadtools, convert to JPG using imagenmagock library
	// Edit Stephen  2013-12-31
	if (nLoadInfo == EPS_NOT_SUPPORT || nLoadInfo == PDF_FILTER_MISSING || nLoadInfo != 0) //ERROR_PDF_NOT_ENABLED 20408 
	{
		CString strOutPath = L"";
		if  (!ConvertEPStoJPG(szFile, strOutPath))
			return 0;
		else
		{

			TRY
			{
				nLoadInfo = m_Lead1.Load(strOutPath,0,0,1);
			}
			CATCH_ALL(e)
			{
				return 0;
			}
			END_CATCH_ALL
			if (nLoadInfo !=0)
			{
				return 0;
			}
		}
	}
	// Edit Stephen  2013-12-31


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

	// Edit Stephen  2013-12-31
	// Handle EPS not supported by Leadtools, convert to JPG using imagenmagock library
	// Edit Stephen  2013-12-31
	if (nLoadInfo == EPS_NOT_SUPPORT || nLoadInfo == PDF_FILTER_MISSING || nLoadInfo != 0) //ERROR_PDF_NOT_ENABLED 20408 
	{
		CString strOutPath = L"";
//		CString strImageID = L"";
		if  (!ConvertEPStoJPG(szFile, strOutPath))
			return 0;
		else
		{

			TRY
			{
				nLoadInfo = m_Lead1.Load(strOutPath,0,0,1);
			}
			CATCH_ALL(e)
			{
				return 0;
			}
			END_CATCH_ALL
			if (nLoadInfo !=0)
			{
				return 0;
			}
		}
	}
	// Edit Stephen  2013-12-31

	long lHeight=(long)m_Lead1.GetInfoHeight();
	
	// TODO: Add your specialized creation code here
	return lHeight;
}

void CConvertDlg::Destory(){
	CDialog::OnDestroy();
}

//Edit by Leon Li 2013-04-15 for Loading photo additional info
BOOL CConvertDlg::LoadPhotoAdditionalInfo(LPCWSTR szSrcImageFile,CString& szAdditionalInfo)
{
	CString strPhotoInfo;
	try
	{
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_OBJECTNAME);
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,5))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_CATEGORY );
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,15))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_SUPPLEMENTALCATEGORY );
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,20))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_DATECREATED);
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,55))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_BYLINE);
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,80))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_BYLINETITLE);
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,85))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_CITY );
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,90))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_SUBLOCATION );
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,92))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_PROVINCE_STATE );
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,95))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_PRIMARYLOCATIONNAME);
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,101))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_ORIGINALTRANSMISSIONREFERENCE );
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,103))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_HEADLINE );
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,105))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_CREDIT);
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,110))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_SOURCE );
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,115))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		//strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_AUTHOR );
		//if (!strPhotoInfo.IsEmpty())
		if(m_iptc.GetStrTag(strPhotoInfo,0x02,122))
			szAdditionalInfo+=strPhotoInfo+L"\r\n";
		szAdditionalInfo.TrimLeft();
		szAdditionalInfo.TrimRight();
		szAdditionalInfo=L"\r\n"+szAdditionalInfo;
	}
	catch (...){
		return FALSE;
	}
	return TRUE;
}
//End Leon Li 2013-04-15

//Edit by Leon Li 2013-04-15 for loading photo caption
BOOL CConvertDlg::LoadPhotoCaption(LPCWSTR szSrcImageFile,CString& szPhotoCatpion){
	try{
		//szPhotoCatpion=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_CAPTION );
		m_pView->DisplayTraceMessage(L"Start LoadPhotoCaption by IPTC DLL",0,FALSE);
		//m_iptc.m_strLog = m_pView->m_strLog;
		m_iptc.GetStrTag(szPhotoCatpion,0x02,120);
		m_pView->DisplayTraceMessage(L"End LoadPhotoCaption by IPTC DLL",0,FALSE);

		szPhotoCatpion.TrimLeft();
		szPhotoCatpion.TrimRight();
	}
	catch (...){
		return FALSE;
	}
	if (szPhotoCatpion.IsEmpty())
		return FALSE;
	else return TRUE;
}

BOOL CConvertDlg::LoadPhotoMeta(LPCWSTR szSrcImageFile, CString str_FileId)
{
	CString strPhotoInfo, str_MetaData;//str_MetaData for testing only
	COleVariant v_ole = COleVariant();
	VARIANT v;
	COleSafeArray s_array;
	long lFirst = -1, lSecond = -1;
	try
	{
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SZDESC);
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Title",strPhotoInfo);
			str_MetaData+=L"Title: "+strPhotoInfo+L"\r\n";
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_IPTC_SUBJECTREFERENCE );
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Subject",strPhotoInfo);
			str_MetaData+=L"Subject: "+strPhotoInfo+L"\r\n";
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SZARTIST );
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Author",strPhotoInfo);
			str_MetaData+=L"Author: "+strPhotoInfo+L"\r\n";
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_USERCOMMENT );
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Comment",strPhotoInfo);
			str_MetaData+=L"Comment: "+strPhotoInfo+L"\r\n";
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SZDATETIMEORIGINAL );
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Date taken",strPhotoInfo);
			str_MetaData+=L"Date taken: "+strPhotoInfo+L"\r\n";
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SZCOPYRIGHT);
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Copyright",strPhotoInfo);
			str_MetaData+=L"Copyright: "+strPhotoInfo+L"\r\n";
		m_Lead1.Load(szSrcImageFile,0,0,1);
		int x =-1,y = -1, bit = -1;
		long YRes = -1, XRes = -1;
		x = m_Lead1.GetInfoWidth();
		y = m_Lead1.GetInfoHeight();
		bit = m_Lead1.GetInfoBits();
		XRes = m_Lead1.GetInfoXRes();
		YRes = m_Lead1.GetInfoYRes();
		CString str_Compress = L"";
		str_Compress = m_Lead1.GetInfoCompress();
		if(XRes != -1)
		{
			strPhotoInfo.Format(L"%ld", XRes);
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"horizontal resolution",strPhotoInfo);
			str_MetaData+=L"horizontal  resolution: "+strPhotoInfo+L"\r\n";
		}
		if(YRes != -1)
		{
			strPhotoInfo.Format(L"%ld", YRes);
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Vertical resolution",strPhotoInfo);
			str_MetaData+=L"Vertical resolution: "+strPhotoInfo+L"\r\n";
		}
		if(x != -1)
		{
			strPhotoInfo.Format(L"%d", x);
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Width",strPhotoInfo);
			str_MetaData+=L"Width: "+strPhotoInfo+L"\r\n";
		}
		if(y != -1)
		{
			strPhotoInfo.Format(L"%d", y);
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Height",strPhotoInfo);
			str_MetaData+=L"Height: "+strPhotoInfo+L"\r\n";
		}
		if(bit != -1)
		{
			strPhotoInfo.Format(L"%d", bit);
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Bit depth ",strPhotoInfo);
			str_MetaData+=L"Bit depth: "+strPhotoInfo+L"\r\n";
		}
		if (x!=0 && y!=0)
		{
			strPhotoInfo.Format(L"%d x %d", x,y);
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Dimensions",strPhotoInfo);
			str_MetaData+=L"Dimensions : "+strPhotoInfo+L"\r\n";
		}
		if(str_Compress.GetLength()>0)
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"Compression",str_Compress);
			str_MetaData+=L"Compression: "+str_Compress+L"\r\n";
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SZMAKE);
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"camera maker",strPhotoInfo);
			str_MetaData+=L"camera maker : "+strPhotoInfo+L"\r\n";
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SZMODEL );
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"camera model",strPhotoInfo);
			str_MetaData+=L"camera model : "+strPhotoInfo+L"\r\n";
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SZSOFTWARE);
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"program name",strPhotoInfo);
			str_MetaData+=L"program name : "+strPhotoInfo+L"\r\n";
		v=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_FNUMBER );
		if(v.vt!=0)
		{
			ReadSafeArrayData(v, lFirst,lSecond);
			if (lSecond != 0)
			{
				strPhotoInfo.Format(L"F/%d.%d", lFirst/lSecond, lFirst%lSecond);
				p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"F-stop",strPhotoInfo);
				str_MetaData+=L"F-stop: "+strPhotoInfo+L"\r\n";
			}
		}
		v=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_EXPOSURETIME);
		if(v.vt!=0)
		{
			ReadSafeArrayData(v, lFirst,lSecond);
			strPhotoInfo.Format(L"%d/%d",lFirst,lSecond );
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"exposure time",strPhotoInfo);
			str_MetaData+=L"exposure time: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_ISOSPEEDRATINGS);
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"ISO speed",strPhotoInfo);
			str_MetaData+=L"ISO speed: "+strPhotoInfo+L"\r\n";
		v=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_EXPOSUREBIAS);
		if(v.vt!=0)
		{
			ReadSafeArrayData(v, lFirst,lSecond);
			strPhotoInfo.Format(L"%d/%d EV",lFirst,lSecond );
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"exposure bias",strPhotoInfo);
			str_MetaData+=L"exposure bias: "+strPhotoInfo+L"\r\n";
		}
		v=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_FOCALLENGTH);
		if(v.vt!=0)
		{
			ReadSafeArrayData(v, lFirst,lSecond);			
			if (lSecond != 0)
			{
				strPhotoInfo.Format(L"%d.%d mm", lFirst/lSecond, lFirst%lSecond);
				p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"focal length",strPhotoInfo);
 				str_MetaData+=L"focal length: "+strPhotoInfo+L"\r\n";
			}
		}
		v=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_MAXAPERTURE);
		if(v.vt!=0)
		{
			ReadSafeArrayData(v, lFirst,lSecond);			
			if (lSecond != 0)
			{
				strPhotoInfo.Format(L"F/%d.%d", lFirst/lSecond, lFirst%lSecond);
				p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"max aperture",strPhotoInfo);
				str_MetaData+=L"max aperture: "+strPhotoInfo+L"\r\n";
			}
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_METERINGMODE);
		if (!strPhotoInfo.IsEmpty())
		{
			switch(_wtoi(strPhotoInfo))
			{
				case 1: strPhotoInfo = L"Average";
					break;
				case 2: strPhotoInfo= L"Center Weighted Average";
					break;
				case 3: strPhotoInfo= L"Spot";
					break;
				case 4: strPhotoInfo= L"MultiSpot";
					break;
				case 5: strPhotoInfo= L"Pattern";
					break;
				case 6: strPhotoInfo= L"Partial";
					break;
				case 255 : strPhotoInfo= L"Others";
					break;
				default:
					strPhotoInfo= L"Unidentified";
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"metering mode",strPhotoInfo);
			str_MetaData+=L"metering mode: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SUBJECTDISTANCERANGE);
		if (!strPhotoInfo.IsEmpty())
		{
			switch(_wtoi(strPhotoInfo))
			{
				case 0: strPhotoInfo = L"Unknown";
					break;
				case 1: strPhotoInfo = L"Macro";
					break;
				case 2: strPhotoInfo= L"Close view";
					break;
				case 3: strPhotoInfo= L"Distant view";
					break;
				default:
					strPhotoInfo= L"none";
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"subject distance",strPhotoInfo);
			str_MetaData+=L"subject distance: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_FLASH );
		if (!strPhotoInfo.IsEmpty())
		{
			switch(_wtoi(strPhotoInfo))
			{
				case 1: strPhotoInfo = L"on";
					break;
				default:
					strPhotoInfo= L"off";
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"flash mode",strPhotoInfo);
			str_MetaData+=L"flash mode: "+strPhotoInfo+L"\r\n";
		}
		v=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_FLASHENERGY );
		if(v.vt!=0)
		{
			ReadSafeArrayData(v, lFirst,lSecond);
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"flash energy",strPhotoInfo);
			str_MetaData+=L"flash energy: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_FOCALLENGTHIN35MMFILM );
		if (!strPhotoInfo.IsEmpty())
		{
			if(_wtoi(strPhotoInfo)==0)
				strPhotoInfo = L"Unknown";
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"35mm focal length",strPhotoInfo);
			str_MetaData+=L"35mm focal length: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_COLORSPACE );
		if (!strPhotoInfo.IsEmpty())
		{
			switch(_wtoi(strPhotoInfo))
			{
				case 0xFFFF: strPhotoInfo = L"Uncalibrated";
					break;
				case 1: strPhotoInfo = L"sRGB";
					break;
				default:
					strPhotoInfo= L"Reserved";
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"photometric interpretation",strPhotoInfo);
			str_MetaData+=L"photometric interpretation: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_CONTRAST );
		if (!strPhotoInfo.IsEmpty())
		{
			switch(_wtoi(strPhotoInfo))
			{
				case 0:
					strPhotoInfo = L"Normal";
					break;
				case 1: strPhotoInfo = L"Soft";
					break;
				case 2: strPhotoInfo = L"Hard";
					break;
				default:
					strPhotoInfo= L"Reserved";
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"contrast",strPhotoInfo);
			str_MetaData+=L"contrast: "+strPhotoInfo+L"\r\n";
		}
		v=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_BRIGHTNESS );
		if(v.vt!=0)
		{
			ReadSafeArrayData(v, lFirst,lSecond);
			if(lFirst>0 && lSecond != 0)
				strPhotoInfo.Format(L"%d.%d", lFirst/lSecond, lFirst%lSecond);
			else if (lSecond != 0)
			{
				lFirst = lFirst*-1;
				strPhotoInfo.Format(L"-%d.%d", lFirst/lSecond, lFirst%lSecond);
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"brightness",strPhotoInfo);
			str_MetaData+=L"brightness: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_LIGHTSOURCE );
		if (!strPhotoInfo.IsEmpty())
		{
			switch(_wtoi(strPhotoInfo))
			{
				case 0:
					strPhotoInfo = L"Unidentified";
					break;
				case 1: strPhotoInfo = L"Daylight";
					break;
				case 2: strPhotoInfo = L"Florescence light";
					break;
				case 3: strPhotoInfo = L"Tungsten Lamp";
					break;
				case 17: strPhotoInfo = L"Standard light source A";
					break;
				case 18: strPhotoInfo = L"Standard light source B";
					break;
				case 19: strPhotoInfo = L"Standard light source C";
					break;
				case 20: strPhotoInfo = L"D55";
					break;
				case 21: strPhotoInfo = L"D65";
					break;
				case 22: strPhotoInfo = L"D75";
					break;
				default:
					strPhotoInfo= L"Others";
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"light source",strPhotoInfo);
			str_MetaData+=L"light source: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_EXPOSUREPROGRAM  );
		if (!strPhotoInfo.IsEmpty())
		{
			switch(_wtoi(strPhotoInfo))
			{
				case 0:
					strPhotoInfo = L"Not defined";
					break;
				case 1: strPhotoInfo = L"Manual";
					break;
				case 2: strPhotoInfo = L"Aperture priority";
					break;
				case 4: strPhotoInfo = L"Shutter priority";
					break;
				case 5: strPhotoInfo = L"Creative program";
					break;
				case 6: strPhotoInfo = L"Action program";
					break;
				case 7: strPhotoInfo = L"Portrait mode";
					break;
				default:
					strPhotoInfo= L"reserved";
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"exposure program",strPhotoInfo);
			str_MetaData+=L"exposure program: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SATURATION );
		if (!strPhotoInfo.IsEmpty())
		{
			switch(_wtoi(strPhotoInfo))
			{
				case 0:
					strPhotoInfo = L"Normal";
					break;
				case 1: strPhotoInfo = L"Low saturation";
					break;
				case 2: strPhotoInfo = L"High saturation";
					break;
				default:
					strPhotoInfo= L"Reserved";
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"saturation",strPhotoInfo);
			str_MetaData+=L"saturation: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SHARPNESS );
		if (!strPhotoInfo.IsEmpty())
		{
			switch(_wtoi(strPhotoInfo))
			{
				case 0:
					strPhotoInfo = L"Normal";
					break;
				case 1: strPhotoInfo = L"Soft";
					break;
				case 2: strPhotoInfo = L"Hard";
					break;
				default:
					strPhotoInfo= L"Reserved";
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"sharpness",strPhotoInfo);
			str_MetaData+=L"sharpness: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_WHITEBALANCE );
		if (!strPhotoInfo.IsEmpty())
		{
			
			switch(_wtoi(strPhotoInfo))
			{
			case 0:
				strPhotoInfo = L"Auto white balance";
				break;
			case 1: strPhotoInfo = L"Manual white balance";
				break;
			default:
				strPhotoInfo= L"Reserved";
			}
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"white balance",strPhotoInfo);
			str_MetaData+=L"white balance: "+strPhotoInfo+L"\r\n";
		}
		v=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_DIGITALZOOMRATIO );
		if(v.vt!=0)
		{
			ReadSafeArrayData(v, lFirst,lSecond);
			//str_MetaData+=L"digital zoom: "+strPhotoInfo+L"\r\n";
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"digital zoom",strPhotoInfo);
		}
		v=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_EXIFVERSION );
		if(v.vt!=0)
		{
			if  (v.vt == (VT_ARRAY | VT_UI1))
			{
				int len = 4;
				BYTE *pByte = NULL;
				::SafeArrayAccessData(v.parray, (void **)&pByte);
				::SafeArrayUnaccessData(v.parray);
				strPhotoInfo=L"";
				BYTE b = pByte[0];
				char a = b;
				strPhotoInfo+=a;
				b = pByte[1];a = b;
				strPhotoInfo+=a;
				b = pByte[2];a = b;
				strPhotoInfo+=a;
				b = pByte[3];a = b;
				strPhotoInfo+=a;
				pByte = NULL;
			}	
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"EXIF version",strPhotoInfo);
			str_MetaData+=L"EXIF version: "+strPhotoInfo+L"\r\n";
		}
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0,  CMNT_IPTC_DATECREATED );
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"date created",strPhotoInfo);
			str_MetaData+=L"date created: "+strPhotoInfo+L"\r\n";
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0,  CMNT_SZHOSTCOMP );
		if (!strPhotoInfo.IsEmpty())
			p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"computer",strPhotoInfo);
			str_MetaData+=L"computer: "+strPhotoInfo+L"\r\n";
		strPhotoInfo=m_Lead1.ReadComment(szSrcImageFile,0, CMNT_SZMAKE);
		
		CFile file;
		CFileStatus status;
		file.Open(szSrcImageFile, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone);
		file.GetStatus(status);
		strPhotoInfo.Format(L"%ld bytes", status.m_size);
		p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"size",strPhotoInfo);
		str_MetaData+=L"size: "+strPhotoInfo+L"\r\n";
		CTime time;
		time = status.m_ctime;
		strPhotoInfo = time.Format(L"%Y-%m-%d %H:%M:%S");
		p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"date create",strPhotoInfo);
		str_MetaData+=L"date create: "+strPhotoInfo+L"\r\n";
		time = status.m_mtime;
		strPhotoInfo = time.Format(L"%Y-%m-%d %H:%M:%S");
		p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"date modify",strPhotoInfo);
		str_MetaData+=L"date modify: "+strPhotoInfo+L"\r\n";

		strPhotoInfo.Format(L"%s", szSrcImageFile);
		p_dbCmd->InsertMediaInfo(str_FileId,L"Image",L"file path",strPhotoInfo);
		str_MetaData+=L"file path: "+strPhotoInfo+L"\r\n";

		str_MetaData.TrimLeft();
		str_MetaData.TrimRight();
		str_MetaData=L"\r\n"+str_MetaData;
	}
	catch (...){
		return FALSE;
	}
	return TRUE;
}

void CConvertDlg::ReadSafeArrayData(VARIANT v, long &lFist, long &lSecond)
{
	long MyIndex;
	MyIndex = 0;
	SafeArrayGetElement(v.parray, &MyIndex, &lFist);
	MyIndex = 1;
	SafeArrayGetElement(v.parray, &MyIndex, &lSecond);
}

void CConvertDlg::SetTempConvertFileName(CString strFile)
{
	m_strConvertEPSPath = strFile;
}
//End by Leon Li 2013-04-15 