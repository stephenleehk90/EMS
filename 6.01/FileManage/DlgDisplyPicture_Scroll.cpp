// DlgDisplyPicture.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DlgDisplyPicture.h"
#include "L_ocx.h"
//#include "L_ocx40.h"
#include <io.h>

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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplyPicture message handlers
void CDlgDisplyPicture::ShowCaptionPicture()
{
	if(m_arrPictureFile.GetSize() <= 0 || m_iCurPicIndex >= m_arrPictureFile.GetSize())
	{
		return;
	}
	if(m_iCurPicIndex < 0) m_iCurPicIndex = 0;
	CString strPictureFile = m_arrPictureFile.GetAt(m_iCurPicIndex);
	if(_taccess(strPictureFile, 0) == 0)
	{
		int iPos = strPictureFile.ReverseFind(_T('\\'));
		CString strPicFileName = strPictureFile;
		if(iPos > 0)
			strPicFileName = strPictureFile.Mid(iPos + 1);
		this->SetWindowText(strPicFileName);
		if(m_pDlgViewPicture != NULL)
			m_pDlgViewPicture->ShowImage(strPictureFile);
		AdjustButtonState();
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

void CDlgDisplyPicture::SetFrameRect(const RECT &rcFrame)
{
	RECT rcDlg;
	GetWindowRect(&rcDlg);
	SetWindowPos(NULL, rcFrame.right - (rcDlg.right - rcDlg.left)
		, rcFrame.bottom - (rcDlg.bottom - rcDlg.top),  0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetWindowRect(&rcDlg);
	RECT rcToolBar;
	m_toolBarViewPic.GetWindowRect(&rcToolBar);
	ScreenToClient(&rcToolBar);
	rcToolBar.bottom += 4;
	int iCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	m_staSeparable.MoveWindow(0, rcToolBar.bottom, rcDlg.right - rcDlg.left, 2, TRUE);
	rcToolBar.bottom += 4;
	m_frameViewPic.MoveWindow(0, rcToolBar.bottom, rcDlg.right - rcDlg.left - 8
		, rcDlg.bottom - rcDlg.top - iCaptionHeight - rcToolBar.bottom - 2 - 8, TRUE);
	if(m_pDlgViewPicture != NULL)
	{
		RECT rcFrame;
		m_frameViewPic.GetWindowRect(&rcFrame);
		ScreenToClient(&rcFrame);
		m_pDlgViewPicture->MoveWindow(&rcFrame, TRUE);
		m_pDlgViewPicture->ShowWindow(SW_NORMAL);
	}
	
}

BOOL CDlgDisplyPicture::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_leadCtrl.UnlockSupport(L_SUPPORT_EXPRESS,"sk39uTYp");
	m_leadCtrl.UnlockSupport(L_SUPPORT_GIFLZW, "78BY1jl78");
	m_leadCtrl.UnlockSupport(L_SUPPORT_TIFLZW, "oam5302FDio");
	m_leadCtrl.UnlockSupport(L_SUPPORT_FPXEXTENSIONS, "2jds903"); 
	m_leadCtrl.UnlockSupport(L_SUPPORT_OCR, "YU20G7"); 
	m_leadCtrl.UnlockSupport(L_SUPPORT_VIDEO,"sgjd0376W"); 
	m_leadCtrl.UnlockSupport(L_SUPPORT_MEDICAL,"sd8i2slTY");
	
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
	m_iCurPicIndex = 0;

	m_leadCtrl.ShowWindow(SW_HIDE);
	
	m_pDlgViewPicture = new CDlgViewPicture(this);

	/*
	RECT rcFramePic;
	m_frameViewPic.GetWindowRect(&rcFramePic);
	m_pDlgViewPicture->MoveWindow(&rcFramePic);
	m_pDlgViewPicture->ShowWindow(SW_NORMAL);
	*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDisplyPicture::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(m_frameViewPic.GetSafeHwnd() != NULL)
	{
		RECT rcSeparable;
		m_staSeparable.GetWindowRect(&rcSeparable);
		ScreenToClient(&rcSeparable);
		m_staSeparable.MoveWindow(0, rcSeparable.top, cx
					, rcSeparable.bottom - rcSeparable.top, TRUE);
		rcSeparable.left = 0;
		rcSeparable.top = rcSeparable.bottom + 2;
		rcSeparable.right = cx;
		rcSeparable.bottom = cy;
		m_frameViewPic.MoveWindow(&rcSeparable);
		if(m_pDlgViewPicture != NULL)
		{
			m_pDlgViewPicture->MoveWindow(&rcSeparable);
			m_pDlgViewPicture->AdjustPicturePos(m_pDlgViewPicture->m_leadCtrl.GetInfoWidth()
				, m_pDlgViewPicture->m_leadCtrl.GetInfoHeight());
		}
	}
}

BOOL CDlgDisplyPicture::DestroyWindow() 
{
	m_toolBarViewPic.DestroyWindow();
	m_barDisableImageList.DeleteImageList();
	m_barImageList.DeleteImageList();
	if(m_pDlgViewPicture != NULL)
	{
		m_pDlgViewPicture->DestroyWindow();
		m_pDlgViewPicture = NULL;
	}

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
	if(pMsg->message == WM_MOUSEWHEEL && m_pDlgViewPicture->GetSafeHwnd() != NULL)
	{
		pMsg->hwnd = m_pDlgViewPicture->GetSafeHwnd();
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
		, m_pDlgViewPicture->m_leadCtrl.GetPaintZoomFactor() != 100);
	m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICZOOMIN
		, m_pDlgViewPicture->m_leadCtrl.GetPaintZoomFactor() > 10);
	m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICZOOMOUT, TRUE);
	m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICVIEWPRE
		, m_iCurPicIndex > 0);
	m_toolBarViewPic.GetToolBarCtrl().EnableButton(IDR_PICVIEWNEXT
		, m_iCurPicIndex < (m_arrPictureFile.GetSize() - 1));
}

void CDlgDisplyPicture::OnPicinitsize() 
{
	if(m_pDlgViewPicture != NULL)
	{
		m_pDlgViewPicture->PicOriginalSize();
		AdjustButtonState();
	}
}

void CDlgDisplyPicture::OnPicviewnext() 
{
	m_iCurPicIndex++;
	ShowCaptionPicture();	
	AdjustButtonState();
}

void CDlgDisplyPicture::OnPicviewpre() 
{
	m_iCurPicIndex--;
	ShowCaptionPicture();	
	AdjustButtonState();
}

void CDlgDisplyPicture::OnPiczoomin() 
{
	if(m_pDlgViewPicture != NULL)
	{
		m_pDlgViewPicture->PicZoomOut();
		AdjustButtonState();
	}
}

void CDlgDisplyPicture::OnPiczoomout() 
{
	if(m_pDlgViewPicture != NULL)
	{
		m_pDlgViewPicture->PicZoomIn();
		AdjustButtonState();
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
