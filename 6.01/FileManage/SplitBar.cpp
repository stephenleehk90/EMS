// SplitBar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SplitBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitterBar

CSplitterBar::CSplitterBar()
{
	m_bLButtonDown = FALSE;
	m_pwndLeft = m_pwndRight = NULL;
}

CSplitterBar::~CSplitterBar()
{
}

BOOL CSplitterBar::Create(CWnd* pParent, RECT* pRect)
{
	RECT rc;

	m_pParent = pParent;
	memcpy(&m_rcArea, pRect, sizeof(RECT));
	m_rcArea.left -= SPLITTERBAR_WIDTH;
	m_rcArea.right -= SPLITTERBAR_WIDTH;
	memcpy(&rc, pRect, sizeof(RECT));

	rc.right = rc.left;
	rc.left -= SPLITTERBAR_WIDTH;

	CWnd::Create(NULL, L"", WS_TABSTOP|WS_OVERLAPPED|WS_VISIBLE, rc, pParent, IDC_SPLITTERBAR);

	// Change the cursor for hwnd's window class
 
	SetClassLong(m_hWnd,		// window handle
		GCL_HCURSOR,			// change cursor
		(LONG) AfxGetApp()->LoadCursor(IDC_CURSORSPLITH));			// new cursor
	EnableWindow();

	return TRUE;
}

BOOL CSplitterBar::SetSplitWindows(CWnd* pwndLeft, CWnd* pwndRight)
{
	m_pwndLeft = pwndLeft;
	m_pwndRight = pwndRight;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSplitterBar, CWnd)
	//{{AFX_MSG_MAP(CSplitterBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterBar message handlers

void CSplitterBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = TRUE;
	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

void CSplitterBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	static BOOL bRunning = FALSE;

	if (bRunning)
		return;

	bRunning = TRUE;
	if (m_bLButtonDown)
	{
		ClientToScreen(&point);
		m_pParent->ScreenToClient(&point);
		if ((point.x >= m_rcArea.left) && (point.x <= m_rcArea.right))
		{
			RECT rc;
			GetWindowRect(&rc);
			m_pParent->ScreenToClient(&rc);
			rc.left = point.x;
			rc.right = rc.left + SPLITTERBAR_WIDTH;
			SetWindowPos(&wndTop, rc.left, rc.top, 0, 0, SWP_NOSIZE|SWP_NOREDRAW);

			if (m_pwndLeft)
			{
				m_pwndLeft->GetWindowRect(&rc);
				m_pParent->ScreenToClient(&rc);
				rc.right = point.x;
				m_pwndLeft->SetWindowPos(&wndTop, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, SWP_NOZORDER);
				m_pwndLeft->UpdateWindow();
			}

			if (m_pwndRight)
			{
				m_pwndRight->GetWindowRect(&rc);
				m_pParent->ScreenToClient(&rc);
				rc.left = point.x;
				m_pwndRight->SetWindowPos(&wndTop, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, SWP_NOZORDER);
				m_pwndRight->UpdateWindow();
			}
		}
	}
	bRunning = FALSE;

	CWnd::OnMouseMove(nFlags, point);
}

void CSplitterBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = FALSE;
	ReleaseCapture();
	
	CWnd::OnLButtonUp(nFlags, point);
}
