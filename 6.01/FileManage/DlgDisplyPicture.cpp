// DlgDisplyPicture.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DlgDisplyPicture.h"
//#include "L_ocx40.h"
#include "L_ocx.h"
#include <io.h>
#include "..\commkits\dostools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplyPicture dialog


CDlgDisplyPicture::CDlgDisplyPicture(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDisplyPicture::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDisplyPicture)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDisplyPicture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDisplyPicture)
	DDX_Control(pDX, IDC_FRAMEVIEWPIC, m_frameViewPic);
	DDX_Control(pDX, IDC_STASEPARABLE, m_staSeparable);
	DDX_Control(pDX, IDC_LEADCTRL, m_leadCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDisplyPicture, CDialog)
	//{{AFX_MSG_MAP(CDlgDisplyPicture)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_COMMAND(IDR_PICINITSIZE, OnPicinitsize)
	ON_COMMAND(IDR_PICVIEWNEXT, OnPicviewnext)
	ON_COMMAND(IDR_PICVIEWPRE, OnPicviewpre)
	ON_COMMAND(IDR_PICZOOMIN, OnPiczoomin)
	ON_COMMAND(IDR_PICZOOMOUT, OnPiczoomout)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_STN_DBLCLK(ID_STCPLAY, &CDlgDisplyPicture::OnStnDblclickStcplay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplyPicture message handlers
void CDlgDisplyPicture::ShowCaptionPicture()
{
	GetDlgItem(ID_STCPLAY)->ShowWindow(SW_HIDE);
	this->SetIcon(NULL, FALSE);
	if(m_arrPictureFile.GetSize() <= 0 || m_iCurPicIndex >= m_arrPictureFile.GetSize())
	{
		return;
	}
	if(m_iCurPicIndex < 0) m_iCurPicIndex = 0;
	CString strPictureFile = m_arrPictureFile.GetAt(m_iCurPicIndex);
	if(IsFileExists(strPictureFile))
	{
		m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICZOOMIN, TRUE);
		m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICZOOMOUT, TRUE);
		m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICINITSIZE, TRUE);

		m_leadCtrl.SetPaintWhileLoad(TRUE);
		m_leadCtrl.SetAppearance(APPEARANCE_FLAT);
		m_leadCtrl.SetBorderStyle(0);
		m_leadCtrl.SetBackColor(CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
		m_leadCtrl.SetPaintDither(PAINTDITHER_DIFFUSION);
		m_leadCtrl.SetPaintPalette(PAINTPALETTE_AUTO);
		m_leadCtrl.SetAutoRepaint(TRUE);	
		m_leadCtrl.SetAutoSize(TRUE);
		m_leadCtrl.SetPaintSizeMode(PAINTSIZEMODE_FIT);
		//m_leadCtrl.ShowWindow(SW_HIDE);
		m_strPath = strPictureFile;
		if(IsSupportedPic(strPictureFile))
			m_leadCtrl.Load(strPictureFile, 0, 0, 1);
		else
		{
			CADORecordset rs(CGlobal::GetInstance()->m_FileDB.m_pDB);
			CString strSql = L"", strSmallPic = L"";
			strSql.Format(L"select * from wires_photo_src where photo_path = N'%s'", strPictureFile);
			if(rs.Open(strSql))
			{
				while(!rs.IsEof())
				{
					rs.GetFieldValue(L"small_path", strSmallPic);
					rs.MoveNext();
				}
			}
			if(strSmallPic.GetLength()>0)
			{
				m_leadCtrl.Load(strSmallPic, 0,0,1);
				m_leadCtrl.SetAutoSize(TRUE);
			}
			else
				m_leadCtrl.ShowWindow(SW_HIDE);
		}
		RECT rcToolBar;
		m_toolBarViewPic.GetToolBarCtrl().GetClientRect(&rcToolBar);
		m_iToolBarHeight = rcToolBar.bottom - rcToolBar.top + 10;
		m_iCaptionHeight = GetSystemMetrics(SM_CYCAPTION) + 6;
		int iPos = strPictureFile.ReverseFind(_T('\\'));
		CString strPicFileName = strPictureFile;
		if(iPos > 0)
			strPicFileName = strPictureFile.Mid(iPos + 1);
		this->SetWindowText(strPicFileName);
		//int cyCaption = GetSystemMetrics(SM_CYCAPTION);
		int iWinWidth = m_leadCtrl.GetInfoWidth();//GetBitmapWidth();
		int iWinHeight = m_leadCtrl.GetInfoHeight();//GetBitmapHeight();
		int iDlgWidth = m_rcWinInit.right - m_rcWinInit.left;
		int iDlgHeight = m_rcWinInit.bottom - m_rcWinInit.top
					- m_iToolBarHeight - m_iCaptionHeight;
		if((iWinWidth > iDlgWidth) || (iWinHeight > iDlgHeight))
		{
			if((iWinWidth / iDlgWidth) < (iWinHeight / iDlgHeight))
			{
				iWinWidth = ((float)iWinWidth / iWinHeight) * iDlgHeight;
				iWinHeight = iDlgHeight;
			}
			else
			{
				iWinHeight = ((float)iWinHeight / iWinWidth) * iDlgWidth;
				iWinWidth = iDlgWidth;
			}
		}
		int iLeft = 0;
		int iTop = 0;
		int iLeadCtrlLeft = 0;
		iDlgWidth = iWinWidth;
		iDlgHeight = iWinHeight + m_iToolBarHeight + m_iCaptionHeight;
		if(iWinWidth < m_iWinMinWidth)
		{
			iLeadCtrlLeft = (m_iWinMinWidth - iWinWidth) / 2;
			iDlgWidth = m_iWinMinWidth;
			//iLeft += m_iWinMinWidth - iWinWidth;
		}
		if(m_bIsFirst)
		{
			iLeft = m_rcFrame.right - iDlgWidth;
			iTop = m_rcFrame.bottom - iDlgHeight;
			m_bIsFirst = FALSE;
		}
		else
		{
			RECT rcDialog;
			this->GetWindowRect(&rcDialog);
			iLeft = rcDialog.left;
			iTop = rcDialog.top;
			
		}
		if((iLeft + iDlgWidth) > m_iScreenWidth)
		{
			iLeft = m_iScreenWidth - iDlgWidth;
		}
		if((iTop + iDlgHeight) > m_iScreenHeight)
		{
			iTop = m_iScreenHeight - iDlgHeight;
		}
		if(iLeft < 0) iLeft = 0;
		if(iTop < 0) iTop = 0;
		
		this->SetWindowPos(NULL, iLeft, iTop, iDlgWidth, iDlgHeight, SWP_NOZORDER);
		
		if(m_leadCtrl.GetSafeHwnd() != NULL)
		{
			m_leadCtrl.SetWindowPos(NULL, iLeadCtrlLeft, m_iToolBarHeight, iWinWidth, iWinHeight, SWP_NOZORDER);
		}

		if(m_staSeparable.GetSafeHwnd() != NULL)
		{
			m_staSeparable.SetWindowPos(NULL, 0, m_iToolBarHeight - 4, iDlgWidth, 1, SWP_NOZORDER);
		}
		AdjustButtonState();
		m_bIsCurViewPic = TRUE;
		GetDlgItem(ID_STCPLAY)->ShowWindow(SW_HIDE);
		if(!IsSupportedPic(strPicFileName))
		{
			m_bIsCurViewPic = FALSE;
			m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICZOOMIN, FALSE);
			m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICZOOMOUT, FALSE);
			m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICINITSIZE, FALSE);
			/*GetDlgItem(ID_STCPLAY)->ShowWindow(SW_SHOW);
			GetDlgItem(ID_STCPLAY)->BringWindowToTop();
			GetDlgItem(ID_STCPLAY)->MoveWindow(iDlgWidth/2-25, iDlgHeight/2-25, 50,50);*/
			CBitmap bitmap;bitmap.LoadBitmapW(IDB_BITMAP_PLAY_BUTTON);
			BITMAP bmp;
			bitmap.GetBitmap(&bmp);
   
			HBITMAP hbmMask = ::CreateCompatibleBitmap(::GetDC(NULL), 
														bmp.bmWidth, bmp.bmHeight);

			ICONINFO ii = {0};
			ii.fIcon    = TRUE;
			ii.hbmColor = bitmap;
			ii.hbmMask  = hbmMask;

			HICON hIcon = ::CreateIconIndirect(&ii);
			::DeleteObject(hbmMask);
			this->SetIcon(hIcon, FALSE);
		}
		m_leadCtrl.ShowWindow(SW_NORMAL);
	}
	else
	{
		m_leadCtrl.ShowWindow(SW_HIDE);
	}
}

void CDlgDisplyPicture::SetPictureFile(LPCTSTR lpszPictureFile)
{
	m_arrPictureFile.RemoveAll();
	m_arrPictureFile.Add(lpszPictureFile);
}

void CDlgDisplyPicture::SetPictureFile(const CStringArray &arrRelPicFile)
{
	m_arrPictureFile.RemoveAll();
	m_arrPictureFile.Copy(arrRelPicFile);
}

BOOL CDlgDisplyPicture::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_bIsCurViewPic = TRUE;
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_EXPRESS,L"sk39uTYp");
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_GIFLZW, L"78BY1jl78");
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_TIFLZW, L"oam5302FDio");
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_FPXEXTENSIONS, L"2jds903"); 
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_OCR, L"YU20G7"); 
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_VIDEO,L"sgjd0376W"); 
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_MEDICAL,L"sd8i2slTY");
	
	m_toolBarViewPic.Create(this, WS_CHILD | WS_VISIBLE | WS_BORDER | TBSTYLE_FLAT | CCS_NOPARENTALIGN);
    m_toolBarViewPic.LoadToolBar(IDR_PICVIEWTOOLBAR);
	m_barImageList.Create(24, 24, ILC_COLOR24 | ILC_MASK, 0, 1);
	m_barDisableImageList.Create(24, 24, ILC_COLOR24 | ILC_MASK, 0, 1);
	
	CImageList *barImageList = m_toolBarViewPic.GetToolBarCtrl().GetImageList();
	if(barImageList != NULL)
	{
		barImageList->DeleteImageList();
	}
	barImageList = m_toolBarViewPic.GetToolBarCtrl().GetDisabledImageList();
	if(barImageList != NULL)
	{
		barImageList->DeleteImageList();
	}
	
	SetToolBarImage();
	m_toolBarViewPic.GetToolBarCtrl().SetButtonSize(TOOLBARBUTTONSIZE);
	m_toolBarViewPic.GetToolBarCtrl().SetBitmapSize(TOOLBARBITMAPSIZE);
	
	m_toolBarViewPic.GetToolBarCtrl().SetImageList(&m_barImageList);
	m_toolBarViewPic.GetToolBarCtrl().SetDisabledImageList(&m_barDisableImageList);
	
    m_toolBarViewPic.ShowWindow(SW_SHOW);
    m_toolBarViewPic.SetBarStyle(CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	TBBUTTONINFO tbButtonInfo;
	tbButtonInfo.cbSize = sizeof(tbButtonInfo);
	m_iWinMinWidth = 200;
	CResManage myRes(g_hInst);
	for(int iIndex = 0;;iIndex++)
	{
		if(m_toolBarViewPic.GetItemID(iIndex) == IDR_PICVIEWNEXT)
		{
			RECT rcButton;
			m_toolBarViewPic.GetItemRect(iIndex, &rcButton);
			m_iWinMinWidth = rcButton.right + 8;
			break;
		}
	}
	m_iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	m_iCurPicIndex = 0;
	this->GetClientRect(&m_rcWinInit);
	m_bIsFirst = TRUE;
	m_frameViewPic.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDisplyPicture::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(m_leadCtrl.GetSafeHwnd() != NULL)
	{
	//	m_leadCtrl.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}

BOOL CDlgDisplyPicture::DestroyWindow() 
{
	m_toolBarViewPic.DestroyWindow();
	m_barDisableImageList.DeleteImageList();
	m_barImageList.DeleteImageList();
	return CDialog::DestroyWindow();
}

void CDlgDisplyPicture::OnDestroy() 
{
	CDialog::OnDestroy();	
}

void CDlgDisplyPicture::OnClose() 
{
	CResManage myRes(g_hInst);
	::SendMessage(GetParent()->m_hWnd, WM_COMMAND, ID_FILE_PICTURE, 0);
	
	CDialog::OnClose();
}

BOOL CDlgDisplyPicture::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgDisplyPicture::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
    ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
	
    // if there is a top level routing frame then let it handle the message
    if (GetRoutingFrame() != NULL) return FALSE;
	
    // to be thorough we will need to handle UNICODE versions of the message also !!
    TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
    TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
    CString strTipText;
    UINT nID = pNMHDR->idFrom;
	
    if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
        pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
    {
        // idFrom is actually the HWND of the tool 
        nID = ::GetDlgCtrlID((HWND)nID);
    }
	
    if (nID != 0) // will be zero on a separator
    {
		strTipText.LoadString(nID);
		int iPos = strTipText.ReverseFind(_T('\n'));
		if(iPos >= 0)
			strTipText = strTipText.Mid(iPos + 1);
#ifndef _UNICODE
        if (pNMHDR->code == TTN_NEEDTEXTA)
        {
            lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
        }
        else
        {
            _mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
        }
#else
        if (pNMHDR->code == TTN_NEEDTEXTA)
        {
            _wcstombsz(pTTTA->szText, strTipText,sizeof(pTTTA->szText));
        }
        else
        {
            lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
        }
#endif
		
        *pResult = 0;
        // bring the tooltip window above other popup windows
        ::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
            SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);
        
        return TRUE;
    }
	
    return FALSE;
}

void CDlgDisplyPicture::AdjustButtonState()
{
	CResManage myRes(g_hInst);
	m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICINITSIZE
		, m_leadCtrl.GetPaintZoomFactor() != 100);
	m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICZOOMIN
		, m_leadCtrl.GetPaintZoomFactor() > 10);
	m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICZOOMOUT, TRUE);
	m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICVIEWPRE
		, m_iCurPicIndex > 0);
	m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICVIEWNEXT
		, m_iCurPicIndex < (m_arrPictureFile.GetSize() - 1));
}

void CDlgDisplyPicture::OnPicinitsize() 
{
	m_leadCtrl.LockWindowUpdate();
	m_leadCtrl.SetPaintSizeMode( PAINTSIZEMODE_ZOOM);
	m_leadCtrl.SetPaintZoomFactor(100);
	AdjustPicSize(m_leadCtrl.GetDstWidth(), m_leadCtrl.GetDstHeight());
	m_leadCtrl.UnlockWindowUpdate();
	AdjustButtonState();
}

void CDlgDisplyPicture::OnPicviewnext() 
{
	m_iCurPicIndex++;
	m_leadCtrl.LockWindowUpdate();
	//m_leadCtrl.SetPaintSizeMode( PAINTSIZEMODE_ZOOM);
	//m_leadCtrl.SetPaintZoomFactor(100);
	ShowCaptionPicture();
	m_leadCtrl.UnlockWindowUpdate();
	AdjustButtonState();
	m_leadCtrl.SetAutoSize(TRUE);
}

void CDlgDisplyPicture::OnPicviewpre() 
{
	m_iCurPicIndex--;
	m_leadCtrl.LockWindowUpdate();
	//m_leadCtrl.SetPaintSizeMode( PAINTSIZEMODE_ZOOM);
	//m_leadCtrl.SetPaintZoomFactor(100);
	m_leadCtrl.SetAutoSize(TRUE);
	ShowCaptionPicture();
	m_leadCtrl.UnlockWindowUpdate();
	//AdjustButtonState();
}

void CDlgDisplyPicture::OnPiczoomin() 
{
	if(m_leadCtrl.GetPaintZoomFactor() > 10)
	{
		m_leadCtrl.LockWindowUpdate();
		m_leadCtrl.SetPaintSizeMode( PAINTSIZEMODE_ZOOM);
		m_leadCtrl.SetPaintZoomFactor(m_leadCtrl.GetPaintZoomFactor() - 10);
		AdjustPicSize(m_leadCtrl.GetDstWidth(), m_leadCtrl.GetDstHeight());
		m_leadCtrl.UnlockWindowUpdate();
		AdjustButtonState();
	}
}

void CDlgDisplyPicture::OnPiczoomout() 
{
	m_leadCtrl.LockWindowUpdate();
	m_leadCtrl.SetPaintSizeMode( PAINTSIZEMODE_ZOOM);
	m_leadCtrl.SetPaintZoomFactor(m_leadCtrl.GetPaintZoomFactor() + 10);
	AdjustPicSize(m_leadCtrl.GetDstWidth(), m_leadCtrl.GetDstHeight());
	m_leadCtrl.UnlockWindowUpdate();
	AdjustButtonState();
}

void CDlgDisplyPicture::AdjustPicSize(int iWidth, int iHeight)
{
	int iLeadCtrlLeft = 0;
	int iWinWidth = iWidth;
	int iWinHeight = iHeight + m_iToolBarHeight + m_iCaptionHeight;
	RECT rcDialog;
	this->GetWindowRect(&rcDialog);
	int iWinLeft = rcDialog.left;
	int iWinTop = rcDialog.top;
	if(iWinWidth < m_iWinMinWidth)
	{
		iLeadCtrlLeft = (m_iWinMinWidth - iWinWidth) / 2;
		iWinWidth = m_iWinMinWidth;
	}
	/*
	if(iWinLeft + iWinWidth > m_iScreenWidth)
	{
		iWinLeft -= (iWinLeft + iWinWidth - m_iScreenWidth);
	}
	if(iWinLeft < 0) iWinLeft = 0;
	if(iWinTop + iWinHeight > m_iScreenHeight)
	{
		iWinTop -= (iWinTop + iWinHeight - m_iScreenHeight);
	}
	if(iWinTop < 0) iWinTop = 0;
	*/
	this->SetWindowPos(NULL, iWinLeft, iWinTop, iWinWidth, iWinHeight, SWP_NOZORDER);
	
	if(m_leadCtrl.GetSafeHwnd() != NULL)
	{
		m_leadCtrl.SetWindowPos(NULL, iLeadCtrlLeft, m_iToolBarHeight, iWidth, iHeight, SWP_NOZORDER);
	}
	
	if(m_staSeparable.GetSafeHwnd() != NULL)
	{
		m_staSeparable.SetWindowPos(NULL, 0, m_iToolBarHeight - 4, iWinWidth, 1, SWP_NOZORDER);
	}
}

void CDlgDisplyPicture::SetToolBarImage()
{
	CBitmap bitmap;
	//原始大小
	bitmap.LoadBitmap(IDB_BITMAP_ORIGINALSIZE);
	m_barImageList.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//縮小
	bitmap.LoadBitmap(IDB_BITMAP_ZOOMOUT);
	m_barImageList.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//放大
	bitmap.LoadBitmap(IDB_BITMAP_ZOOMIN);
	m_barImageList.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//前一幅
	bitmap.LoadBitmap(IDB_BITMAP_PREPICTURE);
	m_barImageList.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//下一幅
	bitmap.LoadBitmap(IDB_BITMAP_NEXTPICTURE);
	m_barImageList.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//////////////////////////////////////////////////////////////////////////
	//Disable ImageList
	//////////////////////////////////////////////////////////////////////////
	//原始大小
	bitmap.LoadBitmap(IDB_BITMAP_ORIGINALSIZEDISABLE);
	m_barDisableImageList.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//縮小
	bitmap.LoadBitmap(IDB_BITMAP_ZOOMOUTDISABLE);
	m_barDisableImageList.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//放大
	bitmap.LoadBitmap(IDB_BITMAP_ZOOMINDISABLE);
	m_barDisableImageList.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//前一幅
	bitmap.LoadBitmap(IDB_BITMAP_PREPICTUREDISABLE);
	m_barDisableImageList.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//下一幅
	bitmap.LoadBitmap(IDB_BITMAP_NEXTPICTUREDISABLE);
	m_barDisableImageList.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
}

BOOL CDlgDisplyPicture::IsSupportedPic(CString strFile)
{
	CString strext;
	strext=strFile.Mid(strFile.ReverseFind('.')+1);
	strext.MakeLower();
	for (int nPos=0;nPos<nSupportPicFormatCount;nPos++)
	{
		if(strext == arraySupportPicFormat[nPos])
			return TRUE;
	}
	return FALSE;
}

void CDlgDisplyPicture::OnStnDblclickStcplay()
{
	// TODO: Add your control notification handler code here
	try
	{
		ShellExecute(NULL,NULL,m_strPath,NULL,NULL,SW_SHOWNORMAL);
	}
	catch (...)
	{
		//AfxMessageBox(IDS_AFX_UNSUPPORTTYPE);
	}
}
BEGIN_EVENTSINK_MAP(CDlgDisplyPicture, CDialog)
	ON_EVENT(CDlgDisplyPicture, IDC_LEADCTRL, DISPID_DBLCLICK, CDlgDisplyPicture::DblClickLeadctrl, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgDisplyPicture::DblClickLeadctrl()
{
	// TODO: Add your message handler code here
	if(CGlobal::GetInstance()->m_pShareData->m_bMediaColumnExist == TRUE 
			|| CGlobal::GetInstance()->m_pShareData->m_bSupportMedia == TRUE)
	{
		if(m_bIsCurViewPic == FALSE)
		{
			try
			{
				ShellExecute(NULL,NULL,m_strPath,NULL,NULL,SW_SHOWNORMAL);
			}
			catch (...)
			{
				//AfxMessageBox(IDS_AFX_UNSUPPORTTYPE);
			}
		}
	}
}
