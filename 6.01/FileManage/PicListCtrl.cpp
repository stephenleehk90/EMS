// PicListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "PicListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicListCtrl

CPicListCtrl::CPicListCtrl()
{
}

CPicListCtrl::~CPicListCtrl()
{
}


BEGIN_MESSAGE_MAP(CPicListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CPicListCtrl)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicListCtrl message handlers

void CPicListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(HitTest(point)==-1)
		::SendMessage(m_hParent,WM_LEAVEPIC,point.x,point.y);
	CListCtrl::OnMouseMove(nFlags, point);
}
