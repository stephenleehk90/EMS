// ReplaceView.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "ReplaceView.h"
#include "replacefactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReplaceView

IMPLEMENT_DYNCREATE(CReplaceView, CEditProReplaceView)

CReplaceView::CReplaceView()
{
}

CReplaceView::~CReplaceView()
{
}


BEGIN_MESSAGE_MAP(CReplaceView, CEditProReplaceView)
	//{{AFX_MSG_MAP(CReplaceView)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CReplaceView diagnostics

#ifdef _DEBUG
void CReplaceView::AssertValid() const
{
	CEditProReplaceView::AssertValid();
}

void CReplaceView::Dump(CDumpContext& dc) const
{
	CEditProReplaceView::Dump(dc);
}
#endif //_DEBUG



void CReplaceView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*	CMenu	m_pPopupMenu ;
	m_pPopupMenu.LoadMenu(IDR_POPREPLACE);
	CMenu* pPopup = m_pPopupMenu.GetSubMenu(0);
	ClientToScreen(&point);
	pPopup->TrackPopupMenu(
		TPM_RIGHTBUTTON, point.x, point.y, this, &m_rcWnd);
*/	CEditProReplaceView::OnRButtonDown(nFlags, point);
	//

}

void CReplaceView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString strSynon=FindSynonymStr();
	if(strSynon.IsEmpty())
		return;
	//
	
	CStringList *pList;
	POSITION pos;
	CRepFactory *pFactory=CRepFactory::Instance();
	pList=(*pFactory->m_pmapSynonymns)[strSynon];
	m_ListSynBox.ResetContent();
	if(!pList)
		return;
	for(pos=pList->GetHeadPosition();pos!=NULL;)
	{
		m_ListSynBox.AddString(pList->GetAt(pos));
		pList->GetNext(pos);
	}

	int nCount=m_ListSynBox.GetCount();
	if(nCount<1) return;
	int nLen;
//	char buf[100];
	wchar_t buf[100];

	m_ListSynBox.GetText(0,buf);
	nLen = lstrlen(buf);
	
	for(int i=1;i<nCount;i++)
	{
		if(LB_ERR==m_ListSynBox.GetText(i,buf))
			break;
//		int nCLen = strlen(buf);
		int nCLen = lstrlen(buf);
		if(nLen<nCLen)
			nLen = nCLen;
	}
	long lWidth=10;
	int x,y,cx,cy;
	CPoint pt = GetCaretPos();
	long lHeight=m_pFontSet->GetCharHeight();
	x=pt.x;
	y=pt.y+lHeight;
	cx = nLen*lWidth;
	cy = 90;
	if(x+cx>m_rcWnd.right)
		x = m_rcWnd.right - cx;
	if(y+cy>m_rcWnd.bottom)
		y = m_rcWnd.bottom - cy;
	m_ListSynBox.MoveWindow(x,y,cx,cy);
	m_ListSynBox.ShowWindow(SW_SHOW);
	m_ListSynBox.SetFocus();
	m_ListSynBox.SetSel(0);
	CEditProReplaceView::OnLButtonDblClk(nFlags, point);
}
