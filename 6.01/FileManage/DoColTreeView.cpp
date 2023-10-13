// DoColTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DoColTreeView.h"
#include "DoColDoc.h"
#include "DoColFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDoColTreeView

IMPLEMENT_DYNCREATE(CDoColTreeView, CTreeViewEx)

CDoColTreeView::CDoColTreeView()
{
}

CDoColTreeView::~CDoColTreeView()
{
}


BEGIN_MESSAGE_MAP(CDoColTreeView, CTreeViewEx)
	//{{AFX_MSG_MAP(CDoColTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoColTreeView drawing

void CDoColTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDoColTreeView diagnostics

#ifdef _DEBUG
void CDoColTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CDoColTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDoColTreeView message handlers

void CDoColTreeView::OnInitialUpdate() 
{
	CTreeViewEx::OnInitialUpdate();
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = GetPrivateProfileIntW( L"TreeTextStyle" , L"Height", -14,((CWinApp*)AfxGetApp())->m_pszProfileName);
	m_fontFace.CreateFontIndirect(&lf);
	CTreeViewEx::SetFont(&m_fontFace);
	// TODO: Add your specialized code here and/or call the base class
	//build lib tree
	m_pTreeCtrl=&GetTreeCtrl();
	CDbTree dbtree;
	dbtree.AssignTree(m_pTreeCtrl);
	CGlobal *pGL = CGlobal::GetInstance();
	dbtree.AssignDb(pGL->m_FileDB.m_pDB,_T("news_lib"),_T("lib_id"),_T("parent_id"),_T("lib_name"));
	dbtree.BuildTree();
	CString sql;
	sql.Format(_T("select t1.lib_id from news_column t1, column_permission t2 where group_id = %d and t1.column_id= t2.column_id "), pGL->m_pEmployee->m_nGroup );
	dbtree.Filter(sql);
	m_bInitializing = TRUE;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	if(CGlobal::GetInstance()->strLibId.GetLength()>0)
	{
		if(dbtree.SelectItem(_wtoi(CGlobal::GetInstance()->strLibId)) == FALSE)
		{
			AfxMessageBox(L"沒有相關部門的權限");
			CGlobal::GetInstance()->str_Share = CGlobal::GetInstance()->strLibId = CGlobal::GetInstance()->strColCode = L"";
		}
	}
	else
		dbtree.SelectItem(GetPrivateProfileIntW( L"DoColFile" , L"Tree",0,((CWinApp*)AfxGetApp())->m_pszProfileName));
//	dbtree.SelectItem(AfxGetApp()->GetProfileInt ( _T("DoColFile" ), _T("Tree") , 0 ));
	//end 20120927
	m_bInitializing = FALSE;
}
void CDoColTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hitem = pNMTreeView->itemNew.hItem;
	
	CDoColDoc* pDoc=((CDoColDoc*)GetDocument());
	pDoc->m_Mgr.m_strDepartName=m_pTreeCtrl->GetItemText ( hitem );
	pDoc->m_Mgr.m_nDepartID = m_pTreeCtrl->GetItemData (hitem) ;
	CDoColFrm *pFrm =(CDoColFrm*) GetParentFrame();
	pDoc->m_Mgr.LoadColumn(&pFrm->m_wndColumn);

	if(m_bInitializing)
	{
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		int nColID = GetPrivateProfileIntW( L"DoColFrm" , L"column_id",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//	int nColID = AfxGetApp()->GetProfileInt(_T("DoColFrm"), _T("column_id"), 0);
		//end 20120927
		for(int nIndex = 0; nIndex < pFrm->m_wndColumn.GetCount(); nIndex++)
		{
			if(pFrm->m_wndColumn.GetItemData(nIndex) == nColID)
			{
				if(pDoc->m_Mgr.m_nColumnID != nColID)
				{
					pFrm->m_wndColumn.SetCurSel(nIndex);
					pDoc->m_Mgr.m_nColumnID = nColID;
				}
				break;
			}
		}
	}
	pFrm->GetDoc()->m_Mgr.m_strCurFileCode = L"";
	pFrm->GetDoc()->m_Mgr.m_iCurFilePos = -1;
	pFrm->OnSelchangeColumncombo();
}

void CDoColTreeView::OnDestroy() 
{
	CTreeViewEx::OnDestroy();
	
	// TODO: Add your message handler code here
	CDoColDoc* pDoc=((CDoColDoc*)GetDocument());
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString strTemp; 
	strTemp.Format(L"%d",pDoc->m_Mgr.m_nDepartID);
	//AfxMessageBox(strTemp);
	WritePrivateProfileStringW(L"DoColFile", L"Tree",strTemp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//AfxGetApp()->WriteProfileInt ( _T("DoColFile") , _T("Tree") , pDoc->m_Mgr.m_nDepartID);
	//end 20120927
}
