// TreeViewEx.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "TreeViewEx.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeViewEx

IMPLEMENT_DYNCREATE(CTreeViewEx, CTreeView)

CTreeViewEx::CTreeViewEx()
{
}

CTreeViewEx::~CTreeViewEx()
{
}


BEGIN_MESSAGE_MAP(CTreeViewEx, CTreeView)
	//{{AFX_MSG_MAP(CTreeViewEx)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeViewEx drawing

void CTreeViewEx::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTreeViewEx diagnostics

#ifdef _DEBUG
void CTreeViewEx::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTreeViewEx::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTreeViewEx message handlers

int CTreeViewEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetNewStyle(TVS_HASBUTTONS, TRUE);
	SetNewStyle(TVS_HASLINES, TRUE);
	SetNewStyle(TVS_LINESATROOT, TRUE);
	SetNewStyle(TVS_SHOWSELALWAYS,TRUE);
	
	CImageList			*pimagelist;

	pimagelist = & m_ImageList;
	pimagelist->Create(16, 16, ILC_COLOR24|ILC_MASK, 6, 6);

	CResManage setRes(g_hInst);
	CBitmap	bitmap;
	bitmap.LoadBitmap(IDB_BMTREENODE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BMTREENODESEL);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BMTREELEAF);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BMTREELEAFSEL);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	GetTreeCtrl().SetImageList(pimagelist, TVSIL_NORMAL);
	GetTreeCtrl().SetBkColor(CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
	return 0;
}

void CTreeViewEx::SetNewStyle(long lStyleMask, BOOL bSetBits)
{
	long		lStyleOld;

	lStyleOld = GetWindowLong(m_hWnd, GWL_STYLE);
	lStyleOld &= ~lStyleMask;
	if (bSetBits)
		lStyleOld |= lStyleMask;

	SetWindowLong(m_hWnd, GWL_STYLE, lStyleOld);
	SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

void CTreeViewEx::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
