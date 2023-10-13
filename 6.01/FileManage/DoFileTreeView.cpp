// DoFileTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DoFileTreeView.h"
#include "dofiledoc.h"
#include "dofilefrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDoFileTreeView

IMPLEMENT_DYNCREATE(CDoFileTreeView, CTreeViewEx)

CDoFileTreeView::CDoFileTreeView()
{
	m_pTreeCtrl=NULL;
}

CDoFileTreeView::~CDoFileTreeView()
{
}


BEGIN_MESSAGE_MAP(CDoFileTreeView, CTreeViewEx)
	//{{AFX_MSG_MAP(CDoFileTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoFileTreeView drawing

void CDoFileTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDoFileTreeView diagnostics

#ifdef _DEBUG
void CDoFileTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CDoFileTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDoFileTreeView message handlers

void CDoFileTreeView::OnInitialUpdate() 
{
	CTreeViewEx::OnInitialUpdate();
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = GetPrivateProfileIntW( L"TreeTextStyle" , L"Height", -14,((CWinApp*)AfxGetApp())->m_pszProfileName);
	m_fontFace.CreateFontIndirect(&lf);
	CTreeViewEx::SetFont(&m_fontFace);
	// TODO: Add your specialized code here and/or call the base class
	m_pTreeCtrl=&GetTreeCtrl();
	CDoFileDoc* pDoc=((CDoFileDoc*)GetDocument());
	CDbTree dbtree;
	dbtree.AssignTree(m_pTreeCtrl);
	CGlobal *pGL = CGlobal::GetInstance();
	dbtree.AssignDb(pGL->m_FileDB.m_pDB,_T("news_lib"),_T("lib_id"),_T("parent_id"),_T("lib_name"));
	dbtree.BuildTree();
	CString sql;
	sql.Format(_T("select lib_id from trans_permission where group_id = %d and perm_style <> 'S'"), pGL->m_pEmployee->m_nGroup );
	dbtree.Filter(sql);
	if(!dbtree.SelectItem(pDoc->m_Mgr.m_nDepartID))
	{
		
		CDoFileDoc* pDoc=((CDoFileDoc*)GetDocument());	
		pDoc->m_Mgr.m_nDepartID=-1;
		pDoc->m_Mgr.m_strPlaceName.Empty();
	}
	if(CGlobal::GetInstance()->strLibId.GetLength()>0)
	{
		if(dbtree.SelectItem(_wtoi(CGlobal::GetInstance()->strLibId)) == FALSE)
		{
			AfxMessageBox(L"沒有相關部門的權限");
			CGlobal::GetInstance()->str_Share = CGlobal::GetInstance()->strLibId = CGlobal::GetInstance()->strColCode = L"";
		}
	}
}
void CDoFileTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hitem = pNMTreeView->itemNew.hItem;
	
	CDoFileDoc* pDoc=((CDoFileDoc*)GetDocument());
	pDoc->m_Mgr.m_strDepartName=m_pTreeCtrl->GetItemText ( hitem );
	pDoc->m_Mgr.m_nDepartID = m_pTreeCtrl->GetItemData (hitem) ;
	CDoFileFrm *pFrm =(CDoFileFrm*) GetParentFrame();
	pDoc->m_Mgr.SetupPlace(&pFrm->m_wndFile);
	//pDoc->m_Mgr.LoadLibFile();
	pFrm->GetDoc()->m_Mgr.m_strCurFileCode = L"";
	pFrm->GetDoc()->m_Mgr.m_iCurFilePos = -1;
	pFrm->RefreshFile();
	pDoc->m_Mgr.SetupDateFilter(&pFrm->m_wndTime);
}
