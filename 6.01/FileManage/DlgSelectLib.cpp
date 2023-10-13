// DlgSelectLib.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSelectLib.h"
#include "afxdialogex.h"


// CDlgSelectLib dialog

IMPLEMENT_DYNAMIC(CDlgSelectLib, CDialog)

CDlgSelectLib::CDlgSelectLib(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectLib::IDD, pParent)
{

}

CDlgSelectLib::~CDlgSelectLib()
{
}

void CDlgSelectLib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DESLIBTREE, m_FileTree);
	DDX_Control(pDX, IDC_PLACECOMBO, m_combPlace);
}


BEGIN_MESSAGE_MAP(CDlgSelectLib, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DESLIBTREE, &CDlgSelectLib::OnTvnSelchangedDeslibtree)
	ON_BN_CLICKED(IDOK, &CDlgSelectLib::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSelectLib::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSelectLib message handlers


BOOL CDlgSelectLib::OnInitDialog()
{
	CDialog::OnInitDialog();
	int nLibID ;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	nLibID = GetPrivateProfileIntW( L"WiresFileTran" , L"Lib",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->GetProfileInt ( _T("TransFile") , _T("Lib") ,0 );
	//end 20120927
	//SetupLibTree(&m_FileTree,0,nLibID);
	CGlobal *pGL=CGlobal::GetInstance();
	CDbTree dbtree;
	dbtree.AssignTree(&m_FileTree);
	dbtree.AssignDb(pGL->m_FileDB.m_pDB,_T("news_lib"),_T("lib_id"),_T("parent_id"),_T("lib_name"));
	dbtree.BuildTree();
	pGL->m_FileDB.GetValidLibIds(pGL->m_pEmployee->m_nGroup, _T('S'), m_waValidLibIds);
	dbtree.Filter(m_waValidLibIds);

	wchar_t sz_temp[255];


	dbtree.SelectItem(nLibID);
	GetPrivateProfileStringW( L"WiresFileTran" , L"Place",NULL,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	CString strPlace = sz_temp;

	m_combPlace.SelectString(-1, strPlace);

	if(m_combPlace.GetCount()>0)
	{
		m_combPlace.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}else
	{
		m_combPlace.EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
	return TRUE; 
}

BOOL CDlgSelectLib::SetupLibTree(CTreeCtrl *pTreeCtrl, WORD wParent, int nLibID)
{
	CADORecordset rs;
	
	CGlobal *pGlobal=CGlobal::GetInstance();

	//if(pGlobal->m_FileDB.LoadLibAll(&rs))
	if(pGlobal->m_FileDB.LoadLibByUserGroup(&rs))
	{
		HTREEITEM hSelItem=NULL;
		InsertSubItem(&rs,pTreeCtrl,NULL,0,nLibID,hSelItem);
		if(hSelItem)
			pTreeCtrl->SelectItem(hSelItem);
		rs.Close();
		return TRUE;
	}
	
	return FALSE;
	
}

void CDlgSelectLib::InsertSubItem(CADORecordset *pRS,CTreeCtrl *pTreeCtrl,HTREEITEM hParent, WORD wParent, int nLibID,HTREEITEM &hSelItem)
{
	TV_INSERTSTRUCT		tvstruct;
	HTREEITEM hTreeItem = NULL;
	pRS->MoveFirst();
	int nLibTmpID=0,nParentID,nIsLeaf;
	int nPos;
	CString strLibName;
	while(!pRS->IsEOF())
	{
		pRS->GetFieldValue(0,nLibTmpID);
		pRS->GetFieldValue(1,nParentID);
		pRS->GetFieldValue(2,nIsLeaf);
		if(nParentID==wParent)
		{
			pRS->GetFieldValue(3,strLibName);
			tvstruct.hParent = hParent;
			tvstruct.hInsertAfter = TVI_SORT;
			tvstruct.item.pszText = (LPTSTR)(LPCTSTR)strLibName;
			tvstruct.item.mask = TVIF_TEXT|TVIF_IMAGE| TVIF_SELECTEDIMAGE;
			tvstruct.item.lParam = nLibTmpID ;
			tvstruct.item.iImage = 0 ;
			tvstruct.item.iSelectedImage = 1;
			hTreeItem = pTreeCtrl->InsertItem(&tvstruct);
			pTreeCtrl->SetItemData ( hTreeItem , nLibTmpID );
			if(nLibID==nLibTmpID)hSelItem=hTreeItem;
			nPos = pRS->GetAbsolutePosition();
			if(nIsLeaf!=0)
			{
				InsertSubItem(pRS,pTreeCtrl,hTreeItem,nLibTmpID,nLibID,hSelItem);
				pRS->SetAbsolutePosition(nPos);
			}
		}
		pRS->MoveNext();
	}
	return ;
}

void CDlgSelectLib::OnTvnSelchangedDeslibtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	DWORD dwLib;

	//CGlobal *pGL=CGlobal::GetInstance();
	iLibId = -1;
	str_FilePlace = L"";
	dwLib = m_FileTree.GetItemData (pNMTreeView->itemNew.hItem) ;

	//pGL->m_FileDB.LoadPlace()
	if (!LoadPlace(1, dwLib , &m_combPlace) )	
	{
		m_combPlace.EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return ;
	}

	if(m_combPlace.GetCount()>0)
	{
		m_combPlace.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}else
	{
		m_combPlace.EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}


	CWinApp *pApp=(CWinApp*)AfxGetApp();
//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString strPlace;
	wchar_t sz_temp[255];
	GetPrivateProfileStringW( L"WiresFileTran" , L"Place",NULL,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	strPlace = sz_temp;
	//CString strPlace=pApp->GetProfileString(_T("TransFile"),_T("Place"),NULL);
	//end 20120927
	int nSel=m_combPlace.SelectString(-1,strPlace);
	if(nSel==-1)
		m_combPlace.SetCurSel(0);
	
	*pResult = 0;
}

BOOL CDlgSelectLib::LoadPlace(int nType, int nLibID, CComboBox *pBox)
{
	CADORecordset rs;
	pBox->ResetContent();
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(pGlobal->m_FileDB.LoadPlace(1,&rs,nLibID,pGlobal->m_pEmployee->m_nGroup))
	{
		CString strPlace;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(0,strPlace);
			pBox->AddString(strPlace);
			rs.MoveNext();
		}
		rs.Close();
		return TRUE;
	}
	return FALSE;
	
}

void CDlgSelectLib::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CGlobal *pGL=CGlobal::GetInstance();
	
	int wLib;
	CString strPlace, str_Temp;
	if(!GetSelInfo ( wLib , strPlace ))
		return ;
	str_FilePlace = strPlace;
	iLibId = wLib;
	str_Temp.Format(L"%d", iLibId);
	WritePrivateProfileStringW(L"WiresFileTran", L"Lib" , str_Temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	WritePrivateProfileStringW(L"WiresFileTran", L"Place" , str_FilePlace.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	
	CDialog::OnOK();
}

BOOL CDlgSelectLib::GetSelInfo(int &nLib, CString &strPlace)
{
	int nIndex=m_combPlace.GetCurSel();
	if(nIndex!=-1)
		 m_combPlace.GetLBText(nIndex,strPlace);
	else
	{
		strPlace=_T("");
		return FALSE;
	}
	HTREEITEM hItem ;
	if ( hItem = m_FileTree.GetSelectedItem () )
		nLib = m_FileTree.GetItemData(hItem);
	else
	{
		nLib = 0;
		return FALSE;
	}
	return TRUE;
}

void CDlgSelectLib::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//if(AfxMessageBox(IDS_NOFILEPLACESELWARNING,MB_YESNO | MB_ICONQUESTION)==IDNO)
	//	return;
	CDialog::OnCancel();
}
