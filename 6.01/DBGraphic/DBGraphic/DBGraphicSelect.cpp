// DBGraphicSelect.cpp : implementation file
//

#include "stdafx.h"
#include "DBGraphic.h"
#include "DBGraphicSelect.h"
#include "afxdialogex.h"
#include "DBGraphicMain.h"

// CDBGraphicSelect dialog

IMPLEMENT_DYNAMIC(CDBGraphicSelect, CDialog)

CDBGraphicSelect::CDBGraphicSelect(ADISTFILESTRUCT * pDistFileArr,CWnd* pParent /*=NULL*/)
	: CDialog(CDBGraphicSelect::IDD, pParent)
	,m_DistFileArr(pDistFileArr)
{
}

CDBGraphicSelect::~CDBGraphicSelect()
{
}

void CDBGraphicSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST1, m_dblist);
	DDX_Control(pDX, IDC_LIST1, m_dbList);
}


BEGIN_MESSAGE_MAP(CDBGraphicSelect, CDialog)
	ON_BN_CLICKED(IDAPPLY, &CDBGraphicSelect::OnApply)
	ON_BN_CLICKED(IDSELECT, &CDBGraphicSelect::OnSelect)
	ON_BN_CLICKED(IDRESELECT, &CDBGraphicSelect::OnReselect)
	ON_BN_CLICKED(ID_CANCEL, &CDBGraphicSelect::OnCancel)
END_MESSAGE_MAP()


// CDBGraphicSelect message handlers


void CDBGraphicSelect::OnApply()
{
	// TODO: Add your control notification handler code here
	CListBox *pList;
	CDBGraphicMain *dlg;
	CString content;
	BOOL resetDb = TRUE;
	dlg=(CDBGraphicMain *)GetParent();
	CString code, str_SelInfoCode = L"", str_Temp, str_DisplayInfoCode = L"";
	TCHAR db_code[20],db_type[20],db_name[20];
	pList = (CListBox*)(dlg->GetDlgItem(IDC_DBINFO));
	if(pList != NULL)
		pList->ResetContent();
	m_DistFileArr->RemoveAll();
	for(int i=0; i<m_dbList.GetItemCount(); i++)
	{
		if(m_dbList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			code = L"";
			m_dbList.GetItemText(i, 0,db_name,20);
			m_dbList.GetItemText(i, 1,db_type,20);
			m_dbList.GetItemText(i, 2,db_code,20);
			/*if(pList->FindString(0,db_code)<0){
				code.Format(L"%s[%s]",db_name,db_code);
				pList->AddString(code);	}*/
			if(pList != NULL)
			{
				if(pList->FindStringExact(0,db_name)<0){
					if (bSupportMedia==TRUE && bMediaColumnExist == TRUE)
						code.Format(L"%s [%s]",db_name,db_code);
					else
						code.Format(L"%s [%s] [%s]",db_name,db_type,db_code);
					pList->AddString(code);	
				}
			}
			str_Temp.Format(L"%s", db_name);
			str_SelInfoCode += str_Temp+L",";
			//if(code.GetLength()==0)
			if (bSupportMedia==TRUE && bMediaColumnExist == TRUE)
				code.Format(L"%s [%s] [%s]",db_name,db_type,db_code);
			else
				code.Format(L"%s [%s]",db_name,db_code);
			str_DisplayInfoCode += code+L",";
			for (int j=0;j<m_hpdfsDistFile.GetCount();j++)
			{
				if (!lstrcmp(m_hpdfsDistFile.GetAt(j).szCode,db_name))
				{
					m_DistFileArr->Add(m_hpdfsDistFile.GetAt(j));
				}
			}
		}
	}
	dlg->UpdateData(FALSE);
	//LoadConfig();
	CWinApp* pApp=AfxGetApp();
	WritePrivateProfileStringW(L"System",L"SelectInfoCode",str_SelInfoCode,pApp->m_pszProfileName);
	WritePrivateProfileStringW(L"System",L"DisplayInfoCode",str_DisplayInfoCode,pApp->m_pszProfileName);
	int iSaveAll = ((CButton*) GetDlgItem(IDC_SAVEALLCHECK))->GetCheck();
	str_Temp.Format(L"%d", iSaveAll);
	WritePrivateProfileString(L"System",L"SaveAll",str_Temp, AfxGetApp()->m_pszProfileName);
	CDialog::OnOK();
}


void CDBGraphicSelect::OnSelect()
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < m_dbList.GetItemCount(); i++)
	{
		m_dbList.SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
	}
	m_dbList.SetFocus();
}


void CDBGraphicSelect::OnReselect()
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < m_dbList.GetItemCount(); i++)
	{
		if(m_dbList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			m_dbList.SetItemState(i,0,-1);
		}
	}
	m_dbList.SetFocus();
}


void CDBGraphicSelect::OnCancel()
{
	if(bIsStartUp == TRUE)
		bIsCancel = TRUE;
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

BOOL CDBGraphicSelect::OnLoadDfalt() 
{
	// TODO: Add your control notification handler code here
	wchar_t str_temp[1024];
	CString str_SelInfoCode, str_Temp, str_Token ;
	GetPrivateProfileStringW( L"System",L"SelectInfoCode", L"", str_temp,1024,((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_SelInfoCode.Format(L"%s",str_temp);
	int iSaveAll;
	GetPrivateProfileStringW( L"System",L"SaveAll", L"0", str_temp,100,((CWinApp*)AfxGetApp())->m_pszProfileName);
	iSaveAll = _wtoi(str_temp);
	CheckDlgButton(IDC_SAVEALLCHECK, iSaveAll);
	CString arr_SelInfoCode[100];
	int iCurPos = 0, iCount = 0;
	str_Token = str_SelInfoCode.Tokenize(L",", iCurPos);
	while(str_Token.GetLength()>0)
	{
		arr_SelInfoCode[iCount] = str_Token;
		iCount++;
		str_Token = str_SelInfoCode.Tokenize(L",", iCurPos);
	}


	wchar_t szBuffer1[256]={0};
	wchar_t szBuffer2[256]={0};
	wchar_t szBuffer3[256]={0};
	wchar_t szBuffer4[256]={0};
	wchar_t szBuffer5[256]={0};
	wchar_t szBuffer6[256]={0};
	wchar_t szMediaType[50]={0};
	CSQLApi  Database1 ;
	CSQLApi  *p_dbCmd1 = &Database1;
	int dmptid ;
	int agencyid , iMediaType, iTempMediaType = -1;

	if(!m_bDbFail)
	{
		if(!p_dbCmd->LoginDb(TRUE)&!p_dbCmd1->LoginDb(TRUE))
			return FALSE;
	}	
	RETCODE nResult1,nResult2;
	SDWORD	cbValue1,cbValue2,cbValue3,cbValue4,cbValue5,cbValue6,cbValue7,cbValue8, cbValueMediaType;
	BOOL bFil = FALSE ;
	p_dbCmd->ODBCAllocStmt ();

	if (bSupportMedia==TRUE && bMediaColumnExist == TRUE)
		nResult1 = p_dbCmd->ODBCExecCmdW( L"select db_Code,db_PicSource,db_dpmtid,db_agencyid, db_media_type from dbgraphi_info");
	else
		nResult1 = p_dbCmd->ODBCExecCmdW( L"select db_Code,db_PicSource,db_dpmtid,db_agencyid from dbgraphi_info");
	p_dbCmd->ODBCBind(1, SQL_C_WCHAR, szBuffer1, 255, &cbValue1);
	p_dbCmd->ODBCBind(2, SQL_C_WCHAR, szBuffer2, 255, &cbValue2);
	p_dbCmd->ODBCBind(3, SQL_C_LONG, &dmptid, 0, &cbValue3);
	p_dbCmd->ODBCBind(4, SQL_C_LONG, &agencyid, 0, &cbValue4);
	if (bSupportMedia==TRUE && bMediaColumnExist == TRUE)
		p_dbCmd->ODBCBind(5, SQL_C_LONG, &iTempMediaType, 0, &cbValueMediaType);
	while (nResult1 != SQL_NO_DATA_FOUND) 
	{
		BOOL bSuccess = FALSE;
		nResult1 = p_dbCmd->ODBCFetch();
		if(iTempMediaType == -1)
		{
			iMediaType=DBG_TYPE_INT_PIC;
			lstrcpy(szMediaType,DBG_TYPE_STR_PIC);
		}
		if (bSupportMedia==TRUE && bMediaColumnExist == TRUE&&iTempMediaType!=-1)
		{
			iMediaType = iTempMediaType;
			if (!iMediaType==DBG_TYPE_INT_PIC)
				lstrcpy(szMediaType,DBG_TYPE_STR_AV);
			else
				lstrcpy(szMediaType,DBG_TYPE_STR_PIC);
		}
		if (nResult1 == SQL_SUCCESS || nResult1== SQL_SUCCESS_WITH_INFO) 
		{
			ChangeSpaceZeroW ( szBuffer1 , cbValue1 ) ;
			ChangeSpaceZeroW ( szBuffer2 , cbValue2 ) ;
			ChangeSpaceZeroW ( szBuffer3 , cbValue3 ) ;
			ChangeSpaceZeroW ( szBuffer4 , cbValue4 ) ;
			bFil = TRUE;
			if(dmptid>0){
				p_dbCmd1->ODBCAllocStmt ();
				nResult2 = p_dbCmd1->ODBCExecCmdW( L"select lib_code,lib_name from news_lib where lib_id = N'%d'",dmptid);
				p_dbCmd1->ODBCBind(1, SQL_C_WCHAR, szBuffer5, 255, &cbValue5);
				p_dbCmd1->ODBCBind(2, SQL_C_WCHAR, szBuffer6, 255, &cbValue6);
				while (nResult2 != SQL_NO_DATA_FOUND) 
				{

					nResult2 = p_dbCmd1->ODBCFetch();
					if (nResult2 == SQL_SUCCESS || nResult2== SQL_SUCCESS_WITH_INFO) 
					{
						ChangeSpaceZeroW ( szBuffer5 , cbValue5 ) ;
						ChangeSpaceZeroW ( szBuffer6 , cbValue6 ) ;
						bFil = TRUE;
						bSuccess = TRUE;

					}
					else
						nResult2 = p_dbCmd1->ODBCMoreResults();
				}
				//p_dbCmd1->ODBCFreeStmt(SQL_DROP);
				if (bSuccess)
					AddItem(szBuffer1,szMediaType,szBuffer6,szBuffer5,szBuffer2);
				for(int i=0;i<iCount;i++)
				{
					if(arr_SelInfoCode[i].CompareNoCase(szBuffer1)==0)
					{
						m_dbList.SetItemState(m_dbList.GetItemCount()-1,LVIS_SELECTED, LVIS_SELECTED);
						break;
					}
				}
			}
			else if(agencyid)
			{ 
				p_dbCmd1->ODBCAllocStmt ();
				nResult2 = p_dbCmd1->ODBCExecCmdW( L"select tele_code,tele_name from news_agency_photo where tele_id = N'%d'",agencyid);
				p_dbCmd1->ODBCBind(1, SQL_C_WCHAR, szBuffer5, 255, &cbValue7);
				p_dbCmd1->ODBCBind(2, SQL_C_WCHAR, szBuffer6, 255, &cbValue8);
				while (nResult2 != SQL_NO_DATA_FOUND) 
				{

					nResult2 = p_dbCmd1->ODBCFetch();
					if (nResult2 == SQL_SUCCESS || nResult2== SQL_SUCCESS_WITH_INFO) 
					{
						ChangeSpaceZeroW ( szBuffer5 , cbValue7 ) ;
						ChangeSpaceZeroW ( szBuffer6 , cbValue8 ) ;
						bFil = TRUE;
						bSuccess = TRUE;
					}
					else
						nResult2 = p_dbCmd1->ODBCMoreResults();
				}
				if (bSuccess)
					AddItem(szBuffer1,szMediaType,szBuffer6,szBuffer5,szBuffer2);
				for(int i=0;i<iCount;i++)
				{
					if(arr_SelInfoCode[i].CompareNoCase(szBuffer1)==0)
					{
						m_dbList.SetItemState(m_dbList.GetItemCount()-1,LVIS_SELECTED, LVIS_SELECTED);
						break;
					}
				}
			}

		}
		else
			nResult1 =p_dbCmd->ODBCMoreResults();
	}
	p_dbCmd->ODBCFreeStmt(SQL_DROP);

	UpdateData(FALSE);
	return bFil;

}

int toInt(wchar_t *szValue){
	wchar_t length_db[2];
	length_db[0] = szValue[0];
	length_db[1] = 0;
	int numeric_peek = _wtoi(length_db);
	return numeric_peek;
}

void CDBGraphicSelect::AddItem(LPWSTR lpszCode,LPWSTR lpszMediaType, LPWSTR lpszDpmt,LPWSTR lpszDcode
	, LPWSTR lpszPath)
{
	LV_ITEM	lvitem;
	int nRow ;
	nRow = m_dbList.GetItemCount ();

	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = nRow ;
	lvitem.iSubItem = 0;
	lvitem.pszText = (LPWSTR)lpszCode;
	nRow = m_dbList.InsertItem(&lvitem);
	
	lvitem.iSubItem = 1;
	lvitem.pszText = lpszMediaType;
	m_dbList.SetItem(&lvitem);

	lvitem.iSubItem = 2;
	lvitem.pszText = (LPWSTR)lpszDpmt;
	m_dbList.SetItem(&lvitem);

	lvitem.iSubItem = 3;
	lvitem.pszText = (LPWSTR)lpszDcode;
	m_dbList.SetItem(&lvitem);

	lvitem.iSubItem = 4;
	lvitem.pszText = (LPWSTR)lpszPath;
	m_dbList.SetItem(&lvitem);

	m_dbList.EnsureVisible(0, FALSE);
	m_dbList.SendMessage(WM_PAINT);	
}

BOOL CDBGraphicSelect::LoadConfig(){

	wchar_t szBuffer1[256]={0};
	wchar_t szBuffer2[256]={0};
	wchar_t szBuffer3[256]={0};
	wchar_t szBuffer4[256]={0};
	wchar_t szBuffer5[256]={0};
	wchar_t szBuffer6[256]={0};
	wchar_t szBuffer7[256]={0};
	wchar_t szBuffer8[256]={0};
	wchar_t szBuffer9[256]={0};
	wchar_t szBuffer10[256]={0};
	wchar_t szBuffer11[256]={0};
	wchar_t szBuffer12[256]={0};
	wchar_t szBuffer13[256]={0};

	RETCODE nResult;
	int angency_id,dmpt_id,xml_output,db_id,db_default,nCheckTime,nTitleLine=1,nMarkLine=1, nUseFolderDate=0, iTempMediaType, iMediaType;
	CString code,text;
	SDWORD	cbValue1,cbValue2,cbValue3,cbValue4,cbValue5,cbValue6,cbValue7,cbValue8,cbValue9,cbValue10,cbValue11,cbValue12,cbValue13,cbValue14,cbValue15,cbValue16,cbValue17,cbValue18,cbValue19,cbValue20,cbValue21, cbValue22, cbValueMediaType;
	BOOL bFil = FALSE ;
	if(m_bDbFail)
	{
		if(!p_dbCmd->LoginDb(TRUE))
			return FALSE;
	}	


	for(int i =0; i < m_dbList.GetItemCount(); i++){
		BOOL bSuccess = FALSE;
		nTitleLine=1;nMarkLine=1;
		nUseFolderDate=0;
		DISTFILESTRUCT fileToRead;
		memset( &fileToRead, '\0', sizeof(DISTFILESTRUCT) );
		p_dbCmd->ODBCAllocStmt ();
		code = m_dbList.GetItemText(i,0);
		nResult = p_dbCmd->ODBCExecCmdW( L"select * from dbgraphi_info where db_Code = N'%s'",code);
		p_dbCmd->ODBCBind(1, SQL_C_WCHAR, szBuffer1, 255, &cbValue1);
		p_dbCmd->ODBCBind(2, SQL_C_WCHAR, szBuffer2, 255, &cbValue2);
		p_dbCmd->ODBCBind(3, SQL_C_WCHAR, szBuffer3, 255, &cbValue3);
		p_dbCmd->ODBCBind(4, SQL_C_WCHAR, szBuffer4, 255, &cbValue4);
		p_dbCmd->ODBCBind(5, SQL_C_WCHAR, szBuffer5, 255, &cbValue5);
		p_dbCmd->ODBCBind(6, SQL_C_WCHAR, szBuffer6, 255, &cbValue6);
		p_dbCmd->ODBCBind(7, SQL_C_WCHAR, szBuffer7, 255, &cbValue7);
		p_dbCmd->ODBCBind(8, SQL_C_WCHAR, szBuffer8, 255, &cbValue8);
		p_dbCmd->ODBCBind(9, SQL_C_WCHAR, szBuffer9, 255, &cbValue9);
		p_dbCmd->ODBCBind(10, SQL_C_WCHAR, szBuffer10, 255, &cbValue10);
		p_dbCmd->ODBCBind(11, SQL_C_LONG, &angency_id, 0, &cbValue11);
		p_dbCmd->ODBCBind(12, SQL_C_LONG, &dmpt_id, 0, &cbValue12);
		p_dbCmd->ODBCBind(13, SQL_C_LONG, &xml_output, 0, &cbValue13);
		p_dbCmd->ODBCBind(14, SQL_C_LONG, &db_id,0,&cbValue14);
		p_dbCmd->ODBCBind(15, SQL_C_LONG, &db_default, 0, &cbValue15);
		p_dbCmd->ODBCBind(16, SQL_C_WCHAR, &szBuffer11, 255, &cbValue16);
		p_dbCmd->ODBCBind(17, SQL_C_LONG, &nCheckTime, 0, &cbValue19);
		p_dbCmd->ODBCBind(18, SQL_C_LONG, &nUseFolderDate, 0, &cbValue22);
		if (bSupportMedia==TRUE && bMediaColumnExist == TRUE)
			p_dbCmd->ODBCBind(19, SQL_C_LONG, &iTempMediaType, 0, &cbValueMediaType);
//		p_dbCmd->ODBCBind(18, SQL_C_LONG, &nMaxTm,0,&cbValue22);
//		p_dbCmd->ODBCBind(19, SQL_C_LONG, &nInterval, 0, &cbValue23);
		while (nResult != SQL_NO_DATA_FOUND) 
		{

			nResult = p_dbCmd->ODBCFetch();
			if(iTempMediaType == -1)
				iMediaType=DBG_TYPE_INT_PIC;
			else
				iMediaType = iTempMediaType;
			if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
			{
				ChangeSpaceZeroW ( szBuffer1 , cbValue1 ) ;
				ChangeSpaceZeroW ( szBuffer2 , cbValue2 ) ;
				ChangeSpaceZeroW ( szBuffer3 , cbValue3 ) ;
				ChangeSpaceZeroW ( szBuffer4 , cbValue4 ) ;
				ChangeSpaceZeroW ( szBuffer5 , cbValue5 ) ;
				ChangeSpaceZeroW ( szBuffer6 , cbValue6 ) ;
				ChangeSpaceZeroW ( szBuffer7 , cbValue7 ) ;
				ChangeSpaceZeroW ( szBuffer8 , cbValue8 ) ;
				ChangeSpaceZeroW ( szBuffer9 , cbValue9 ) ;
				ChangeSpaceZeroW ( szBuffer10 , cbValue10 ) ;
				ChangeSpaceZeroW ( szBuffer11 , cbValue16 ) ;
				bFil = TRUE;
				bSuccess = TRUE;
			}
			else
				nResult = p_dbCmd->ODBCMoreResults();

		}
		p_dbCmd->ODBCFreeStmt(SQL_DROP);
		if (!bSuccess)
			continue;
		bSuccess = FALSE;
		if(angency_id>0){
			p_dbCmd->ODBCAllocStmt ();
			nResult = p_dbCmd->ODBCExecCmdW( L"select tele_code,tele_name,title_line,mark_line from news_agency_photo where tele_id = N'%d'",angency_id);
			p_dbCmd->ODBCBind(1, SQL_C_WCHAR, szBuffer12, 255, &cbValue17);
			p_dbCmd->ODBCBind(2, SQL_C_WCHAR, szBuffer13, 255, &cbValue18);
			p_dbCmd->ODBCBind(3, SQL_C_LONG, &nTitleLine, 0, &cbValue20);
			p_dbCmd->ODBCBind(4, SQL_C_LONG, &nMarkLine, 0, &cbValue21);
			while (nResult != SQL_NO_DATA_FOUND) 
			{

				nResult = p_dbCmd->ODBCFetch();

				if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
				{
					ChangeSpaceZeroW ( szBuffer12 , cbValue17 );
					ChangeSpaceZeroW ( szBuffer13 , cbValue18 );
					bFil = TRUE;
					bSuccess = TRUE;
				}
				else
					nResult = p_dbCmd->ODBCMoreResults();

			}

			p_dbCmd->ODBCFreeStmt(SQL_DROP);
			if (!bSuccess)
				continue;
			bSuccess = FALSE;
			fileToRead.iType = 0;
			fileToRead.nAgencyID = angency_id;
			lstrcpy(fileToRead.szTeleName,szBuffer13);
			lstrcpy(fileToRead.szTeleCode,szBuffer12);
		}		
		else if(dmpt_id){

			p_dbCmd->ODBCAllocStmt ();
			nResult = p_dbCmd->ODBCExecCmdW( L"select lib_code,lib_name from news_lib where lib_id = N'%d'",dmpt_id);
			p_dbCmd->ODBCBind(1, SQL_C_WCHAR, szBuffer12, 255, &cbValue17);
			p_dbCmd->ODBCBind(2, SQL_C_WCHAR, szBuffer13, 255, &cbValue18);
			while (nResult != SQL_NO_DATA_FOUND) 
			{

				nResult = p_dbCmd->ODBCFetch();
				if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
				{
					ChangeSpaceZeroW ( szBuffer12 , cbValue17 );
					ChangeSpaceZeroW ( szBuffer13 , cbValue18 );
					bFil = TRUE;
					bSuccess = TRUE;
				}
				else
					nResult = p_dbCmd->ODBCMoreResults();

			}

			p_dbCmd->ODBCFreeStmt(SQL_DROP);
			if (!bSuccess)
				continue;
			bSuccess = FALSE;
			fileToRead.iType = 1;
			fileToRead.nAgencyID = dmpt_id;
			lstrcpy(fileToRead.szTeleName,szBuffer13);
			lstrcpy(fileToRead.szTeleCode,szBuffer12);
		}
		if (nCheckTime>0)
			fileToRead.bCheckTime=TRUE;
		else fileToRead.bCheckTime=FALSE;
		fileToRead.version = 100;
		fileToRead.nTitleLine=nTitleLine;
		fileToRead.nMarkLine=nMarkLine;
		/*fileToRead.nMaxTm=nMaxTm;
		fileToRead.nInterval=nInterval;
		COleDateTime dt = COleDateTime::GetCurrentTime();
		fileToRead.dtLastImportTime = COleDateTime(dt.GetYear(),dt.GetMonth(),dt.GetDay(),0,0,0)-COleDateTimeSpan(1,0,0,0);*/
		lstrcpy(fileToRead.sign,L"lfish");
		fileToRead.size=sizeof(fileToRead);
		lstrcpy(fileToRead.szCode,szBuffer1);
		lstrcpy(fileToRead.szSourcePath,szBuffer2);
		lstrcpy(fileToRead.szTextPath,szBuffer3);
		lstrcpy(fileToRead.szTextExtName,szBuffer4);
		//	lstrcpy(fileToRead.szTextExtName,szBuffer5);
		lstrcpy(fileToRead.szPhotoExtName,szBuffer5);
		lstrcpy(fileToRead.szCaption,szBuffer6);
		//	if(szBuffer7!="")
		lstrcpy(fileToRead.szXMLPath,szBuffer7);
		lstrcpy(fileToRead.szDesPath,szBuffer8);
		//////////
		lstrcpy(fileToRead.szServerPath,szBuffer8);
		/////////
		lstrcpy(fileToRead.szMidPath,szBuffer9);
		lstrcpy(fileToRead.szSmallPath,szBuffer10);
		lstrcpy(fileToRead.szStartTime,szBuffer11);
		if(nUseFolderDate>0)
			fileToRead.bUseFolderDate = TRUE;
		else
			fileToRead.bUseFolderDate = FALSE;
		if(xml_output>0){
			fileToRead.bXMLOutput=1;
			fileToRead.bXMLByFile=1;
		}
		else if (xml_output == 0){
			fileToRead.bXMLOutput=1;
			fileToRead.bXMLByAgency=1;
		}
		else fileToRead.bXMLOutput=0;
		CString path=(CString)fileToRead.szMidPath;
		path.TrimLeft();
		path.TrimRight();
		if(path.IsEmpty())
			fileToRead.bTransMidPhoto=0;
		else {
			fileToRead.bTransMidPhoto=1;
		}
		fileToRead.iMediaType = iMediaType;
		m_hpdfsDistFile.Add(fileToRead);
	}
	return bFil;
}


BOOL CDBGraphicSelect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_dbList.SetExtendedStyle(m_dbList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_dbList.InsertColumn(0, _T("入庫代碼名"), LVCFMT_LEFT, 100);
	if (bSupportMedia==TRUE && bMediaColumnExist == TRUE)
		m_dbList.InsertColumn(1,_T("多媒體類別"),LVCFMT_LEFT,80);
	else
		m_dbList.InsertColumn(1,_T("多媒體類別"),LVCFMT_LEFT,0);
	m_dbList.InsertColumn(2,_T("通訊社名"),LVCFMT_LEFT,80);
	m_dbList.InsertColumn(3, _T("通訊代碼"), LVCFMT_LEFT,80);
	m_dbList.InsertColumn(4,_T("源路徑"),LVCFMT_LEFT,200);
	BOOL bIsOK = OnLoadDfalt();
	LoadConfig();
	m_dbList.SetActiveWindow();
	UpdateData(FALSE);
	if(bIsStartUp==TRUE&&bIsOK)
		OnApply();
	if(!bIsOK&&bIsStartUp==TRUE)
		AfxMessageBox(L"DBGraphic 裡的設定有所變動, 請重新選擇");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
