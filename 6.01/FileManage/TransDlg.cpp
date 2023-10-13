// TransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "TransDlg.h"
#include "LinkToDMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransDlg dialog


CTransDlg::CTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransDlg)
	m_bCopy = FALSE;
	m_bSecret = FALSE;
	m_bUser = FALSE;
	m_strFileCode = _T("");
	m_strNote = _T("");
	m_strTitle = _T("");
	m_strPlace = _T("");
	m_bExclusive = FALSE;
	//}}AFX_DATA_INIT
	m_bNewFile = FALSE;
	m_bLayout = FALSE;
}


void CTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransDlg)
	DDX_Control(pDX, IDC_USERCHECK, m_checkUser);
	DDX_Control(pDX, IDC_USERCOMBO, m_combUser);
	DDX_Control(pDX, IDC_PLACECOMBO, m_combPlace);
	DDX_Control(pDX, IDC_DESLIBTREE, m_FileTree);
	DDX_Check(pDX, IDC_COPYCHECK, m_bCopy);
	DDX_Check(pDX, IDC_SECRET, m_bSecret);
	DDX_Check(pDX, IDC_USERCHECK, m_bUser);
	DDX_Text(pDX, IDC_FILE, m_strFileCode);
	DDX_Text(pDX, IDC_NOTE, m_strNote);
	DDX_Text(pDX, IDC_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_SRCPATH, m_strPlace);
	DDX_Check(pDX, IDC_EXCLUSIVE, m_bExclusive);
	DDX_Control(pDX, IDC_LAYOUTCOMBO, m_combLayout);
	DDX_Check(pDX, IDC_LAYOUTCHECK, m_bLayout);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_dtLayout);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransDlg, CDialog)
	//{{AFX_MSG_MAP(CTransDlg)
	ON_CBN_SELCHANGE(IDC_PLACECOMBO, OnSelchangePlacecombo)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DESLIBTREE, OnSelchangedDeslibtree)
	ON_BN_CLICKED(IDC_USERCHECK, OnUsercheck)
	ON_BN_CLICKED(IDC_BTNLINK, OnBtnlink)
	ON_BN_CLICKED(IDC_LAYOUTCHECK, OnLayoutcheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransDlg message handlers

void CTransDlg::OnOK() 
{
	// TODO: Add extra validation here
#define _TRANS_FILE 0
	

	CGlobal *pGL=CGlobal::GetInstance();
	
	int wLib;
	CString strPlace;
	if(!GetSelInfo ( wLib , strPlace ))
		return ;

	UpdateData();

	
	CString sTitle, sAuthor, sStyle, sPostbox, sUser;
	double dWords;
	BOOL bExclusive, bSecret, bUrgency, bLock;
	
	pGL->m_FileDB.LoadFileInfo(m_strFileCode, sTitle,&dWords,&bExclusive,&bSecret,
		&bUrgency, &bLock, sAuthor, sStyle, sPostbox, sUser);

	if (bSecret && !m_bSecret)
	{
		if (AfxMessageBox(IDS_UNSECRET, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
			return;
	}

	/*
	 *	Add by Bob
		是否提醒關聯,首先由系統設定IsRelGMTipOnTrans控制
	 */
	CString strIsRelGMTipOnTrans;
	pGL->m_FileDB.GetConfig(_T("IsRelGMTipOnTrans"),strIsRelGMTipOnTrans);
	if (strIsRelGMTipOnTrans == _T("1"))
	{
		if (m_bAlert && m_bDM)
		{
			//CGlobal *pGL=CGlobal::GetInstance();
			if (!pGL->m_FileDB.IsLinkedByDM(m_strFileCode))
			{			
				if (AfxMessageBox(IDS_Q_TRANS, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
					return;
			}
		}
	}
	

	CString strUserName,strUserCode;
	CResManage res(g_hInst);
	if ( m_bUser )
	{
		int nIdx;
		if((nIdx=m_combUser.GetCurSel())==-1)
		{
			AfxMessageBox(IDS_SELUSER);
			return ;
		}
		m_combUser.GetLBText(nIdx,strUserName);

		if(!pGL->m_FileDB.GetUserCode ( strUserName , strUserCode ))
			return;
		//顯示出差留言,此功能以後再加
		/*int nSel=pCombo->GetCurSel();
		int nNote=pCombo->GetItemData(nSel);
		if(nNote==1)
		{
			COutNote dlg;
			dlg.m_nShowStyle=2;
			strcpy(dlg.m_szUserCode,szCode);
			if(dlg.DoModal()!=IDOK)
				return;
		}
		bSecret=IsDlgButtonChecked ( IDC_SECRET );*/
	}
	if (m_bLayout)
	{
		if(m_combLayout.GetCurSel()==-1)
		{
			AfxMessageBox(IDS_SELLAYOUT);
			return ;
		}
		UpdataFileTitle();
	}

	CWinApp *pApp=(CWinApp*)AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString str_temp;
	str_temp.Format(L"%d",wLib);
	WritePrivateProfileStringW(L"TransFile", L"Lib" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	WritePrivateProfileStringW(L"TransFile", L"Place" , strPlace.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",m_bUser);
	WritePrivateProfileStringW(L"TransFile", L"UserCheck" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",m_bSecret);
	WritePrivateProfileStringW(L"TransFile", L"SecretCheck" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	WritePrivateProfileStringW(L"TransFile", L"User" , strUserName.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	/*pApp->WriteProfileInt ( _T("TransFile") , _T("Lib" ), wLib );
	pApp->WriteProfileString ( _T("TransFile") , _T("Place") , strPlace);
	pApp->WriteProfileInt ( _T("TransFile") , _T("UserCheck") , m_bUser );
	pApp->WriteProfileInt ( _T("TransFile" ), _T("SecretCheck") , m_bSecret);
	pApp->WriteProfileString ( _T("TransFile") , _T("User") , strUserName);*/
	//end 20120927
	short nRet;
	CString strOper;
	strOper.LoadString(IDS_SENDFILE);
	if ( TransFile(m_strFileCode,wLib,strPlace,m_bCopy?_T("Y"):_T("N"),pGL->m_pEmployee->m_nGroup,
		strUserCode,m_bSecret,&m_strNote,strOper,0,nRet))
		CDialog::OnOK();
}

void CTransDlg::OnSelchangePlacecombo() 
{
	// TODO: Add your control notification handler code here
	OnUsercheck();
}

void CTransDlg::OnSelchangedDeslibtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	DWORD dwLib;

	//CGlobal *pGL=CGlobal::GetInstance();

	dwLib = m_FileTree.GetItemData (pNMTreeView->itemNew.hItem) ;

	//pGL->m_FileDB.LoadPlace()
	if (!LoadPlace(1, dwLib , &m_combPlace) )	
	{
		OnUsercheck() ;
		return ;
	}

	CWinApp *pApp=(CWinApp*)AfxGetApp();
//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString strPlace;
	wchar_t sz_temp[255];
	GetPrivateProfileStringW( L"TransFile" , L"Place",NULL,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	strPlace = sz_temp;
	//CString strPlace=pApp->GetProfileString(_T("TransFile"),_T("Place"),NULL);
	//end 20120927
	int nSel=m_combPlace.SelectString(-1,strPlace);
	if(nSel==-1)
		m_combPlace.SetCurSel(0);
	//((CButton*)GetDlgItem(IDC_USERCHECK))->SetCheck(FALSE);
	OnUsercheck() ;

	
	*pResult = 0;
}

BOOL CTransDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWinApp *pApp= (CWinApp*)AfxGetApp();
	
	
	((CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER1))->SetFormat(DATETIMEPICKERDATEFORMAT);
	int nLibID ;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	nLibID = GetPrivateProfileIntW( L"TransFile" , L"Lib",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	if(nLibID<0)
		nLibID = 0;
		//pApp->GetProfileInt ( _T("TransFile") , _T("Lib") ,0 );
	//end 20120927
	//if(m_bNewFile)
		//SetupLibTree(&m_FileTree,0,nLibID);
	//else
	//	SetupLibTree(&m_FileTree,0,m_nLibID);	
	CDbTree dbtree;
	dbtree.AssignTree(&m_FileTree);
	CGlobal *pGL = CGlobal::GetInstance();
	dbtree.AssignDb(pGL->m_FileDB.m_pDB,_T("news_lib"),_T("lib_id"),_T("parent_id"),_T("lib_name"));
	dbtree.BuildTree();
	CString sql;
	sql.Format(_T("select lib_id from trans_permission where group_id = %d and perm_style <> 'S'"), pGL->m_pEmployee->m_nGroup );
	dbtree.Filter(sql);

//	CGlobal *pGL=CGlobal::GetInstance();
	dbtree.SelectItem(nLibID);

	m_combLayout.ResetContent();
	GetLayoutData(&m_combLayout);
	m_dtLayout = COleDateTime::GetCurrentTime();	
	COleDateTimeSpan dts(1,0,0,0);
	dts -= pGL->m_pEntry->tsSpanOfDay;	
	m_dtLayout += dts;
	CString str = m_strTitle;
	if(str.Find(L"[")==0)
	{
		COleDateTime oldDate;
		oldDate.SetDate(_ttoi(str.Mid(1,4)),_ttoi(str.Mid(5,2)),_ttoi(str.Mid(7,2)));
		if (oldDate.GetStatus() == COleDateTime::valid && str.GetAt(9)==' ' && str.GetAt(10)!=']')
		{
			if(m_dtLayout < oldDate)
				m_dtLayout = oldDate;
			str = str.Mid(10,str.Find(']')-10);
			m_combLayout.SelectString(-1,str);
		}
	}
	
	wchar_t sz_temp[255];
	GetPrivateProfileStringW( L"TransFile" , L"RefDate",NULL,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	str = sz_temp;
	if (str.GetLength()>0)
	{
		COleDateTime oldDate;
		oldDate.SetDate(_ttoi(str.Mid(0,4)),_ttoi(str.Mid(4,2)),_ttoi(str.Mid(6,2)));
		if (m_dtLayout < oldDate)
			m_dtLayout = oldDate;
	}
	GetPrivateProfileStringW( L"TransFile" , L"RefCode",NULL,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	str = sz_temp;
	if (str.GetLength()>0)
		m_combLayout.SelectString(-1,str);

	// Below modify by Bob 2006-4-19
	// 原來的編譯模式改用系統參數調節控制
	// 傳遞時可複製稿件由IsCopyFileOnTrans控制
	// 無論有沒有設置，傳遞的稿件庫都會記住上次的值。而且m_bUser都為FALSE，這樣，每次都需要手工設置是否指定用戶,以免誤操作
/*#ifdef  _MINGPAO_DEMO
	m_bUser = FALSE;
	(CWnd*)GetDlgItem(IDC_COPYCHECK)->EnableWindow(FALSE);
	
	
	CString strPlace = pApp->GetProfileString ( _T("TransFile") , _T("Place"));

	if (strPlace != _T(""))
	{
		m_combPlace.SelectString(-1, strPlace);
	}

#else
	m_bUser = pApp->GetProfileInt ( _T("TransFile") , _T("UserCheck") , 0 );
	
#endif*/
	m_bUser = FALSE;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	//CString strPlace; = pApp->GetProfileString ( _T("TransFile") , _T("Place"));
	//wchar_t sz_temp[255];
	GetPrivateProfileStringW( L"TransFile" , L"Place",NULL,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	CString strPlace = sz_temp;
	//end 20120927

	//if(m_bNewFile && strPlace != _T(""))
	//{
		m_combPlace.SelectString(-1, strPlace);
	//}
	//else
	//{
	//	m_combPlace.SelectString(-1, m_strPlaceName);
	//}
	CString strIsCopyFileOnTrans;
	pGL->m_FileDB.GetConfig(_T("IsCopyFileOnTrans"),strIsCopyFileOnTrans);
	if (strIsCopyFileOnTrans == _T("1"))
		(CWnd*)GetDlgItem(IDC_COPYCHECK)->EnableWindow(TRUE);
	else
		(CWnd*)GetDlgItem(IDC_COPYCHECK)->EnableWindow(FALSE);
	// End modify by Bob

	
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	m_bSecret= GetPrivateProfileIntW( L"TransFile" , L"SecretCheck",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//m_bSecret = pApp->GetProfileInt ( _T("TransFile") , _T("SecretCheck") , 0 );
	int nAlert = GetPrivateProfileIntW( L"Alert" , L"Alert",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//int nAlert = pApp->GetProfileInt(_T("Alert"), _T("Alert"), 1);
	//end 20120927
	if (nAlert != 0 )
		m_bAlert = true;	// 應當判斷警告模塊是否存在
	else
		m_bAlert =false;
#ifdef _STANDARD_VERSION_	
	m_bDM = false;		// 應當判斷部門分組菜單模塊是否存在
#else
	m_bDM = true;
#endif
	pGL->m_FileDB.GetFileLastModTime(m_strLastModifyTime, m_strFileCode);

	if(pGL->HasSecretPerm())
	{
		(CWnd*)GetDlgItem(IDC_SECRET)->EnableWindow(TRUE);
	}
	else
		(CWnd*)GetDlgItem(IDC_SECRET)->EnableWindow(FALSE);

	BOOL bCopy;
	bCopy = FALSE;
	pGL->m_FileDB.GetLibCopy(m_nLibID,m_strFileCode,bCopy);
	//if(pGL->HasCopyPerm())
	//if (bCopy)
	//	(CWnd*)GetDlgItem(IDC_COPYCHECK)->EnableWindow(TRUE);
	//else
	//	(CWnd*)GetDlgItem(IDC_COPYCHECK)->EnableWindow(FALSE);
	
	m_checkUser.EnableWindow(!m_bExclusive);
	
	if(m_bExclusive)
		m_bUser=TRUE;
	if(m_bUser)
		m_combUser.EnableWindow(TRUE);
	else
		m_combUser.EnableWindow(FALSE);
	UpdateData(FALSE);
	OnUsercheck();

	if (m_bAlert && m_bDM)
	{
		GetDlgItem(IDC_BTNLINK)->ShowWindow(SW_SHOW);
		
		if (!pGL->m_FileDB.IsLinkedByDM(m_strFileCode))
			GetDlgItem(IDC_BTNLINK)->EnableWindow();
		
	}

	CString strValue;
	pGL->m_FileDB.GetConfig(L"IsTextAddInfo", strValue);
	if(strValue.CompareNoCase(L"1")==0)
	{
		GetDlgItem(IDC_LAYOUTSTATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LAYOUTSTATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LAYOUTSTATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LAYOUTCHECK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DATETIMEPICKER1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LAYOUTCOMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LAYOUTCHECK)->EnableWindow(TRUE);
	}else
	{
		GetDlgItem(IDC_LAYOUTSTATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LAYOUTSTATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LAYOUTSTATIC2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LAYOUTCHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_LAYOUTCHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATETIMEPICKER1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LAYOUTCOMBO)->ShowWindow(SW_HIDE);
		m_bLayout = FALSE;
	}
//	CString strValue;
	//pGL->m_FileDB.GetConfig(L"IsSingleFile", strValue);
	//if(strValue.CompareNoCase(L"1")==0)
	//	GetDlgItem(IDC_EXCLUSIVE)->EnableWindow(TRUE);
	//else
		GetDlgItem(IDC_EXCLUSIVE)->EnableWindow(FALSE);
#ifdef _STANDARD_VERSION_
	GetDlgItem(IDC_BTNLINK)->ShowWindow(SW_HIDE);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTransDlg::OnUsercheck() 
{
	// TODO: Add your control notification handler code here
	m_combUser.ResetContent();
	CWnd *pSecretWnd=(CWnd*)GetDlgItem(IDC_SECRET);
	CheckDlgButton(IDC_SECRET, 0);
	
	UpdateData();
	if ( m_bUser )
	{
		m_combUser.EnableWindow(TRUE);
		CGlobal *pGL=CGlobal::GetInstance();
		//if(pGL->HasSecretPerm())
		{
			(CWnd*)GetDlgItem(IDC_SECRET)->EnableWindow(TRUE);
			CString strValue;
			pGL->m_FileDB.GetConfig(L"IsCopyFileOnTrans", strValue);
			if(strValue.CompareNoCase(L"1")==0)
				(CWnd*)GetDlgItem(IDC_COPYCHECK)->EnableWindow(TRUE);//Added by Ronald Saliman on 20120831
			else
				(CWnd*)GetDlgItem(IDC_COPYCHECK)->EnableWindow(FALSE);
		}
		//else
		//	(CWnd*)GetDlgItem(IDC_SECRET)->EnableWindow(FALSE);
		int nLib ;
		CString strPlace;
		GetSelInfo ( nLib , strPlace ) ;
		GetLibUser( nLib , strPlace , &m_combUser ) ;
		CString strUser;
		CWinApp *pApp=(CWinApp*)AfxGetApp();
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		wchar_t sz_temp[255];
		GetPrivateProfileStringW( L"TransFile" , L"User",NULL,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		strUser = sz_temp;
		//strUser=pApp->GetProfileString(_T("TransFile"),_T("User"),NULL);
		//end 20120927
		m_combUser.SelectString(-1,strUser);
	}
	else
	{
		pSecretWnd->EnableWindow(FALSE);
		m_combUser.EnableWindow(FALSE);
		(CWnd*)GetDlgItem(IDC_COPYCHECK)->EnableWindow(FALSE);//Added by Ronald Saliman on 20120831
	}
}

BOOL CTransDlg::GetSelInfo(int &nLib, CString &strPlace)
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

void CTransDlg::OnBtnlink() 
{
	// TODO: Add your control notification handler code here
	CGlobal *pGL=CGlobal::GetInstance();
	LinkToDM(m_strFileCode);
/*
	CGlobal *pGL=CGlobal::GetInstance();
	if (!pGL->m_FileDB.IsLinkedByDM(m_strFileCode))
	{
		CLinkToDMDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			if (!pGL->m_FileDB.IsLinkedByDM(m_strFileCode))
			{// 關聯到部門分組菜單
				pGL->m_FileDB.LinkToDM(dlg.m_nDeptFileInfoId, m_strFileCode);
			}
			else
			{//該稿件已經被其他用戶關聯

			}
		}
	}
*/
	GetDlgItem(IDC_BTNLINK)->EnableWindow(!(pGL->m_FileDB.IsLinkedByDM(m_strFileCode)));
}

BOOL CTransDlg::LoadPlace(int nType, int nLibID, CComboBox *pBox)
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
			strPlace.TrimRight();
			pBox->AddString(strPlace);
			rs.MoveNext();
		}
		rs.Close();
		return TRUE;
	}
	return FALSE;
	
}

BOOL CTransDlg::SetupLibTree(CTreeCtrl *pTreeCtrl, WORD wParent, int nLibID)
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

void CTransDlg::InsertSubItem(CADORecordset *pRS,CTreeCtrl *pTreeCtrl,HTREEITEM hParent, WORD wParent, int nLibID,HTREEITEM &hSelItem)
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

BOOL CTransDlg::GetLibUser(int nLib, CString strPlace, CComboBox *pBox)
{
	CADORecordset rs;
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(pGlobal->m_FileDB.GetLibUser(nLib,strPlace,&rs))
	{
		CString str;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(0,str);
			pBox->AddString(str);
			rs.MoveNext();
		}
		rs.Close();
		return TRUE;
	}
	return FALSE;
}


void CTransDlg::LinkToDM(LPCTSTR lpszFileCode)
{
	CGlobal *pGlobal=CGlobal::GetInstance();
	if (!pGlobal->m_FileDB.IsLinkedByDM(lpszFileCode))
	{
		CLinkToDMDlg dlg;
		CResManage res(g_hInst);
		if (dlg.DoModal() == IDOK)
		{
			if (!pGlobal->m_FileDB.IsLinkedByDM(lpszFileCode))
			{// 關聯到部門分組菜單
				CString str;
				str.LoadString(IDS_LINKTODM);
				pGlobal->m_FileDB.LinkToDM(dlg.m_nDeptFileInfoId, lpszFileCode,str);
			}
			else
			{//該稿件已經被其他用戶關聯
				AfxMessageBox(IDS_P_LINKED_BY_OTHER, MB_OK | MB_ICONSTOP);
			}
		}
	}
	
}

BOOL CTransDlg::TransFile(CString strFileCode, int nDesLibID, CString strDesPlace,
						   CString strCopyStyle, int nGroupID, CString strUserCode, short nSecret, CString *strNote, CString strOper, int nAction, short &nRet)
{
	CResManage res(g_hInst);
	CGlobal *pGL= CGlobal::GetInstance();
	if(!pGL->LockFile(strFileCode))
	{
		AfxMessageBox(IDS_LOCKFILEFAIL);
		return FALSE;
	}
	FILEPARAM FileParam;
	memset(&FileParam,0,sizeof(FileParam));
	_tcscpy(FileParam.szFileCode,strFileCode);
	_tcscpy(FileParam.szLastModTime, m_strLastModifyTime);
	FileParam.nDocStyle=0;
	FileParam.bLib = TRUE;
	FileParam.nLibID = m_nLibID;
	_tcscpy(FileParam.szParam,m_strPlaceName);
	if(!pGL->CanFileModify(&FileParam))
	{
		pGL->UnlockFile(strFileCode,FALSE);
		return FALSE;
	}
	
	BOOL bRet=pGL->m_FileDB.TransFile(strFileCode,nDesLibID,strDesPlace,strCopyStyle,nGroupID,strUserCode,
		nSecret,strNote,strOper,nAction, nRet);
	pGL->UnlockFile(strFileCode,FALSE);
	return bRet;
}

void CTransDlg::OnLayoutcheck()
{
	UpdateData();
	if ( m_bLayout )
	{
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(TRUE);
		m_combLayout.EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);
		m_combLayout.EnableWindow(FALSE);
	}
}

BOOL CTransDlg::GetLayoutData(CComboBox *pBox)
{
	CADORecordset rs;
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(pGlobal->m_FileDB.GetLayoutData(&rs))
	{
		CString str1, str2;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(0,str1);
			rs.GetFieldValue(1,str2);
			str2.TrimRight();
			str1 += L"(" +str2 + L")";
			pBox->AddString(str1);
			rs.MoveNext();
		}
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

void CTransDlg::UpdataFileTitle()
{
	CString str = m_strTitle;
	CString strDate, strCode, strTitle;
	if(str.Find(L"[")==0)
	{
		COleDateTime oldDate;
		oldDate.SetDate(_ttoi(str.Mid(1,4)),_ttoi(str.Mid(5,2)),_ttoi(str.Mid(7,2)));
		if (oldDate.GetStatus() == COleDateTime::valid && str.GetAt(9)==' ' && str.GetAt(10)!=']')
			str = str.Mid(str.Find(L']')+1);
	}
	strDate = m_dtLayout.Format(L"%Y%m%d");
	m_combLayout.GetLBText(m_combLayout.GetCurSel(),strCode);
	//strCode = strCode.Left(strCode.Find(L'('));
	strCode = strCode.Mid(strCode.Find(L'(')+1, strCode.Find(L')')-strCode.Find(L'(')-1);
	strTitle.Format(L"[%s %s]%s",strDate,strCode,str);
	CGlobal *pGlobal=CGlobal::GetInstance();

	BOOL bTemp, bUrgency, bLock;
	
	CString strTemp, sAuthor, sStyle;
	double dWords;
	pGlobal->m_FileDB.LoadFileInfo(m_strFileCode, strTemp,&dWords,&bTemp,&bTemp,
		&bUrgency, &bLock, sAuthor, sStyle, strTemp, strTemp);

	FILEINFO FI;
	FI.nExclusive = m_bExclusive;
	FI.strFilecode = m_strFileCode;
	FI.strPosBox = L"";
	FI.nSecret = m_bSecret;
	FI.strTxtAuthor = sAuthor;
	FI.bLock = bLock;
	FI.strTxtStyle = sStyle.IsEmpty()?_T(" "):sStyle;
	FI.strTxtTitle = strTitle;
	FI.strUrgency = bUrgency?'Y':'N';
	pGlobal->m_FileDB.UpdateFileInfo(&FI);
	
	WritePrivateProfileStringW(L"TransFile", L"RefDate" , strDate.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	WritePrivateProfileStringW(L"TransFile", L"RefCode" , strCode.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	
}

BOOL CTransDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
    return CDialog::PreTranslateMessage(pMsg);
}