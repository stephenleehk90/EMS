// DlgViewPicture.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DlgViewPicture.h"
//#include "L_ocx40.h"
#include "L_ocx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgViewPicture dialog


CDlgViewPicture::CDlgViewPicture(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgViewPicture::IDD, pParent)
	, m_hCur_GrabIng(NULL), m_hCur_BGrab(NULL), m_rcOriginalRect(0, 0, 0, 0)
	, m_bDragging(FALSE), m_nVScrollPos(0), m_nHScrollPos(0), m_nCurHeight(0)
	, m_nCurWidth(0), m_bHaveHScrollBar(FALSE), m_bHaveVScrollBar(FALSE)
{
	//{{AFX_DATA_INIT(CDlgViewPicture)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Create(CDlgViewPicture::IDD, pParent);
}


void CDlgViewPicture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewPicture)
	DDX_Control(pDX, IDC_LEADCTRL, m_leadCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgViewPicture, CDialog)
	//{{AFX_MSG_MAP(CDlgViewPicture)
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewPicture message handlers

BOOL CDlgViewPicture::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_hCur_BGrab = AfxGetApp()->LoadCursor(IDC_CURSORGRAB1);
	m_hCur_GrabIng = AfxGetApp()->LoadCursor(IDC_CURSORGRAB2);

	GetWindowRect(m_rcOriginalRect);
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_EXPRESS,L"sk39uTYp");
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_GIFLZW, L"78BY1jl78");
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_TIFLZW, L"oam5302FDio");
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_FPXEXTENSIONS, L"2jds903"); 
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_OCR, L"YU20G7"); 
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_VIDEO,L"sgjd0376W"); 
// 	m_leadCtrl.UnlockSupport(L_SUPPORT_MEDICAL,L"sd8i2slTY");
	m_leadCtrl.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgViewPicture::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_nCurHeight = cy;
	m_nCurWidth = cx;
	
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = m_rcOriginalRect.Width() + 48;
	si.nPage = cx;
	si.nPos = 0;
	SetScrollInfo(SB_HORZ, &si, TRUE);
	
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = m_rcOriginalRect.Height();
	si.nPage = cy;
	si.nPos = 0;
    SetScrollInfo(SB_VERT, &si, TRUE);
	//if(m_leadCtrl.GetSafeHwnd() != NULL)
	//	AdjustPicturePos(m_leadCtrl.GetInfoWidth(), m_leadCtrl.GetInfoHeight());
}

BOOL CDlgViewPicture::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	int nMaxPos = m_rcOriginalRect.Height() - m_nCurHeight;
	if (zDelta<0)
	{
		if (m_nVScrollPos < nMaxPos)
		{
			zDelta = min(max(nMaxPos/20,5),nMaxPos - m_nVScrollPos);
			
			m_nVScrollPos += zDelta;
			SetScrollPos(SB_VERT,m_nVScrollPos,TRUE);
			ScrollWindow(0,-zDelta);
		}
	}
	else
	{
		if (m_nVScrollPos > 0)
		{
			zDelta = -min(max(nMaxPos/20,5),m_nVScrollPos);
			
			m_nVScrollPos += zDelta;
			SetScrollPos(SB_VERT,m_nVScrollPos,TRUE);
			ScrollWindow(0,-zDelta);
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CDlgViewPicture::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		int nYDelta = m_ptDragPoint.y - point.y;
		int nXDelta = m_ptDragPoint.x - point.x;
		m_ptDragPoint = point;
		int nNewPos = 0;

		if(m_bHaveVScrollBar)
		{
			nNewPos = m_nVScrollPos + nYDelta;
			if (nNewPos < 0)
				nNewPos = 0;
			else if (nNewPos > m_rcOriginalRect.Height() - m_nCurHeight)
				nNewPos = m_rcOriginalRect.Height() - m_nCurHeight;
			nYDelta = nNewPos - m_nVScrollPos;
			m_nVScrollPos = nNewPos;
		}
		else
		{
			nYDelta = 0;
		}
		
		if(m_bHaveHScrollBar)
		{
			nNewPos = m_nHScrollPos + nXDelta;
			if(nNewPos < 0)
				nNewPos = 0;
			else if(nNewPos > m_rcOriginalRect.Width() - m_nCurWidth)
				nNewPos = m_rcOriginalRect.Width() - m_nCurWidth;
			nXDelta = nNewPos - m_nHScrollPos;
			m_nHScrollPos = nNewPos;
		}
		else
		{
			nXDelta = 0;
		}
		
		if(nYDelta != 0)
			SetScrollPos(SB_VERT, m_nVScrollPos, TRUE);
		if(nXDelta != 0)
			SetScrollPos(SB_HORZ, m_nHScrollPos, TRUE);
		ScrollWindow(-nXDelta, -nYDelta);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgViewPicture::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bHaveVScrollBar || m_bHaveVScrollBar)
	{
		m_bDragging = TRUE;
		SetCapture();
	}
	else
	{
		m_bDragging = FALSE;
	}
	m_ptDragPoint = point;
	m_ptDragPoint.Offset(-m_nHScrollPos, -m_nVScrollPos);
	SetCursor(m_hCur_GrabIng);
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgViewPicture::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EndDrag();
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgViewPicture::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nDelta;
	int nMaxPos = m_rcOriginalRect.Height() - m_nCurHeight;
	
	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nVScrollPos >= nMaxPos)
			return;
		
		nDelta = min(max(nMaxPos / 20, 5), nMaxPos-m_nVScrollPos);
		break;
		
	case SB_LINEUP:
		if (m_nVScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos / 20, 5), m_nVScrollPos);
		break;
	case SB_PAGEDOWN:
		if (m_nVScrollPos >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos / 10, 5), nMaxPos - m_nVScrollPos);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nVScrollPos;
		break;
		
	case SB_PAGEUP:
		if (m_nVScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos / 10, 5), m_nVScrollPos);
		break;
		
	default:
		return;
	}
	m_nVScrollPos += nDelta;
	SetScrollPos(SB_VERT, m_nVScrollPos, TRUE);
	ScrollWindow(0, -nDelta);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDlgViewPicture::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nDelta;
	int nMaxPos = m_rcOriginalRect.Width() - m_nCurWidth;
	
	switch (nSBCode)
	{
	case SB_LINERIGHT:
		if (m_nHScrollPos >= nMaxPos)
			return;
		
		nDelta = min(max(nMaxPos/20,5),nMaxPos-m_nHScrollPos);
		break;
		
	case SB_LINELEFT:
		if (m_nHScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/20,5),m_nHScrollPos);
		break;
	case SB_PAGERIGHT:
		if (m_nHScrollPos >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5),nMaxPos-m_nHScrollPos);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nHScrollPos;
		break;
		
	case SB_PAGELEFT:
		if (m_nHScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5),m_nHScrollPos);
		break;
		
	default:
		return;
	}
	m_nHScrollPos += nDelta;
	SetScrollPos(SB_HORZ, m_nHScrollPos, TRUE);
	ScrollWindow(-nDelta, 0);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgViewPicture::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	EndDrag();
}

BOOL CDlgViewPicture::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP
			|| pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_MOUSEWHEEL)
		//|| pMsg->message == WM_SETCURSOR || pMsg->message == WM_MOUSEWHEEL)
	{
		if(pMsg->hwnd == m_leadCtrl.GetSafeHwnd())
		{
			pMsg->hwnd = m_hWnd;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgViewPicture::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	BOOL bRet=TRUE;
	
	if (nHitTest==HTCLIENT)
	{
		SetCursor(m_hCur_BGrab);
		bRet=TRUE;
	}
	else
		bRet=CDialog::OnSetCursor(pWnd,nHitTest,message);
	
	return bRet;
}

void CDlgViewPicture::EndDrag()
{
	m_bDragging=FALSE;
	ReleaseCapture();
	SetCursor(m_hCur_BGrab);
}

void CDlgViewPicture::ShowImage(const CString &strPicFile)
{
	/*
	m_leadCtrl.SetPaintWhileLoad(TRUE);
	m_leadCtrl.SetAppearance(APPEARANCE_FLAT);
	m_leadCtrl.SetBorderStyle(0);
	m_leadCtrl.SetBackColor(CLR_NORMALBK);
	m_leadCtrl.SetPaintDither(PAINTDITHER_DIFFUSION);
	m_leadCtrl.SetPaintPalette(PAINTPALETTE_AUTO);
	m_leadCtrl.SetAutoRepaint(TRUE);	
	m_leadCtrl.SetAutoSize(TRUE);
	m_leadCtrl.SetPaintSizeMode(PAINTSIZEMODE_FIT);
	*/
	m_leadCtrl.SetPaintSizeMode(PAINTSIZEMODE_ZOOM);
	m_leadCtrl.SetPaintZoomFactor(100);
	
	m_leadCtrl.SetPaintWhileLoad(TRUE);
	m_leadCtrl.SetAppearance(APPEARANCE_FLAT);
	m_leadCtrl.SetBorderStyle(0);
	m_leadCtrl.SetBackColor(CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
	m_leadCtrl.SetPaintDither(PAINTDITHER_DIFFUSION);
	m_leadCtrl.SetPaintPalette(PAINTPALETTE_AUTO);
	m_leadCtrl.SetAutoRepaint(TRUE);	
	//m_leadCtrl.SetAutoSize(TRUE);
	m_leadCtrl.SetPaintSizeMode(PAINTSIZEMODE_FIT);
	
	m_leadCtrl.ShowWindow(SW_HIDE);
	m_leadCtrl.Load(strPicFile, 0, 0, 1);
	m_leadCtrl.ShowWindow(SW_NORMAL);
	AdjustPicturePos(m_leadCtrl.GetInfoWidth(), m_leadCtrl.GetInfoHeight());
}

void CDlgViewPicture::AdjustPicturePos(int iPicWidth, int iPicHeight)
{
	RECT rcWin;
	GetWindowRect(&rcWin);
	int iPicLeft = 0;
	int iPicTop = 0;
	m_rcOriginalRect.right = m_rcOriginalRect.left + iPicWidth;
	m_rcOriginalRect.bottom = m_rcOriginalRect.top + iPicHeight;
	m_nVScrollPos = 0;
	m_nHScrollPos = 0;
	
	if(iPicWidth > rcWin.right - rcWin.left)
	{
		this->ShowScrollBar(SB_HORZ, TRUE);
		m_bHaveHScrollBar = TRUE;

		SCROLLINFO scrollInfo;
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		scrollInfo.nMin = 0;
		scrollInfo.nMax = iPicWidth;
		scrollInfo.nPage = rcWin.right - rcWin.left;
		scrollInfo.nPos = 0;

		this->SetScrollInfo(SB_HORZ, &scrollInfo, TRUE);	
	}
	else
	{
		this->ShowScrollBar(SB_HORZ, FALSE);
		m_bHaveHScrollBar = FALSE;
		iPicLeft = rcWin.right - rcWin.left - iPicWidth;
		iPicLeft /= 2;
	}
	
	if(iPicHeight > rcWin.bottom - rcWin.top)
	{
		this->ShowScrollBar(SB_VERT, TRUE);
		m_bHaveVScrollBar = TRUE;
		
		SCROLLINFO scrollInfo;
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		scrollInfo.nMin = 0;
		scrollInfo.nMax = iPicHeight;
		scrollInfo.nPage = rcWin.bottom - rcWin.top;
		scrollInfo.nPos = 0;
		
		this->SetScrollInfo(SB_VERT, &scrollInfo, TRUE);
	}
	else
	{
		this->ShowScrollBar(SB_VERT, FALSE);
		m_bHaveVScrollBar = FALSE;
		iPicTop = rcWin.bottom - rcWin.top - iPicHeight;
		iPicTop /= 2;
	}
	
	m_leadCtrl.SetWindowPos(NULL, iPicLeft, iPicTop, iPicWidth
		, iPicHeight, SWP_NOZORDER);	
}

BOOL CDlgViewPicture::DestroyWindow() 
{
	if(m_hCur_BGrab != NULL)
	{
		DestroyCursor(m_hCur_BGrab);
		m_hCur_BGrab = NULL;
	}
	if(m_hCur_GrabIng != NULL)
	{
		DestroyCursor(m_hCur_GrabIng);
		m_hCur_GrabIng = NULL;
	}
	
	return CDialog::DestroyWindow();
}

void CDlgViewPicture::PicOriginalSize()
{
	m_leadCtrl.LockWindowUpdate();
	m_leadCtrl.SetPaintSizeMode( PAINTSIZEMODE_ZOOM);
	m_leadCtrl.SetPaintZoomFactor(100);
	m_leadCtrl.UnlockWindowUpdate();
	AdjustPicturePos(m_leadCtrl.GetDstWidth(), m_leadCtrl.GetDstHeight());
}

void CDlgViewPicture::PicZoomIn()
{
	m_leadCtrl.LockWindowUpdate();
	m_leadCtrl.SetPaintSizeMode( PAINTSIZEMODE_ZOOM);
	m_leadCtrl.SetPaintZoomFactor(m_leadCtrl.GetPaintZoomFactor() + 10);
	m_leadCtrl.UnlockWindowUpdate();
	AdjustPicturePos(m_leadCtrl.GetDstWidth(), m_leadCtrl.GetDstHeight());
}

void CDlgViewPicture::PicZoomOut()
{
	if(m_leadCtrl.GetPaintZoomFactor() > 10)
	{
		m_leadCtrl.LockWindowUpdate();
		m_leadCtrl.SetPaintSizeMode( PAINTSIZEMODE_ZOOM);
		m_leadCtrl.SetPaintZoomFactor(m_leadCtrl.GetPaintZoomFactor() - 10);
		m_leadCtrl.UnlockWindowUpdate();
		AdjustPicturePos(m_leadCtrl.GetDstWidth(), m_leadCtrl.GetDstHeight());
	}
}