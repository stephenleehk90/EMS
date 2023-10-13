// DlgAddToStory.cpp : implementation file
//

#include "stdafx.h"
#include "FileManage.h"
#include "DlgAddToStory.h"
#include "afxdialogex.h"


// CDlgAddToStory dialog

IMPLEMENT_DYNAMIC(CDlgAddToStory, CDialog)

CDlgAddToStory::CDlgAddToStory(CADODatabase *pDB, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddToStory::IDD, pParent)
	, m_selectDate(COleDateTime::GetCurrentTime())
	, m_dt_Date_AddStory(COleDateTime::GetCurrentTime())
	, m_dt_Date(COleDateTime::GetCurrentTime())
{
		m_db = pDB;
		m_b_NewText = false;
		m_i_storyItemID = -1;
}

CDlgAddToStory::~CDlgAddToStory()
{
}

void CDlgAddToStory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSTSTORY, m_lstStory);
	DDX_Control(pDX, IDC_COMBOCATEGORY, m_cmbStoryCategory);
	DDX_Control(pDX, IDC_COMBOLIB, m_cmbNewLib);
	DDX_Control(pDX, IDC_DTPICKER, m_dtStoryDate);
	DDX_Control(pDX, IDC_CB_LIB, m_cbx_Lib);
	DDX_Control(pDX, IDC_LIST_STORY_TYPE, m_lst_Story_Cat);
	DDX_DateTimeCtrl(pDX, IDC_DTPICKER_NEWSTORY, m_dt_Date_AddStory);
	DDX_Control(pDX, IDC_DTPICKER_NEWSTORY, m_dtStoryDate_Add);
	DDX_DateTimeCtrl(pDX, IDC_DTPICKER, m_dt_Date);
}


BEGIN_MESSAGE_MAP(CDlgAddToStory, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDlgAddToStory::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgAddToStory::OnBnClickedOk)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DTPICKER, &CDlgAddToStory::OnDtnDatetimechangeDtpicker)
	ON_BN_CLICKED(IDC_RADEXIST, &CDlgAddToStory::OnBnClickedRadexist)
	ON_BN_CLICKED(IDC_RADNEW, &CDlgAddToStory::OnBnClickedRadnew)
	ON_CBN_SELCHANGE(IDC_CB_LIB, &CDlgAddToStory::OnCbnSelchangeCbLib)
END_MESSAGE_MAP()


// CDlgAddToStory message handlers


void CDlgAddToStory::OnDtnDatetimechangeStorydate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	//SetStoryList();
	*pResult = 0;
}


void CDlgAddToStory::OnBnClickedAdd()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if (m_lstMenu.GetCurSel() < 0) return;

	m_storyID = m_lstMenu.GetItemData(m_lstMenu.GetCurSel()); 
	m_PublishTypeID = m_lbPubType.GetItemData(m_lbPubType.GetCurSel());

	CDialog::OnOK(); 
}


void CDlgAddToStory::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel(); 
}


BOOL CDlgAddToStory::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	InitList();
	LoadListOfStory();
	if(strStoryName.GetLength() == 0)
		strStoryName = L"新故事";
	GetDlgItem(IDC_EDITSTORYNAME)->SetWindowTextW(strStoryName);
	SetComboItems();
	
	CADORecordset rs(m_db);
	CString strSQL;
	m_lst_Story_Cat.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES);
	CRect rect;
	m_lst_Story_Cat.GetClientRect(&rect);
	m_lst_Story_Cat.InsertColumn(0, L"故事類型",LVCFMT_LEFT, rect.Width() - 20, 0);
	strSQL.Format(L"select story_category_name, story_category_id from t_story_category where flag=0");
	if(rs.Open(strSQL))
	{
		LV_ITEM lvitem;
		CString strCat;
		int nCat_id, isSelected;

		int nRow = 0;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(L"story_category_name", strCat);
			rs.GetFieldValue(L"story_category_id", nCat_id);

			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = nRow ;
			
			lvitem.iSubItem =0;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strCat;
			m_lst_Story_Cat.InsertItem(&lvitem);
			
			m_lst_Story_Cat.SetItemData(nRow, nCat_id);

			rs.MoveNext();
			nRow++;	
		}
		rs.Close();
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAddToStory::SetStoryList()
{

	UpdateData();

	CString strDate;
	strDate.Format(L"%d-%d-%d", m_selectDate.GetYear(), m_selectDate.GetMonth(), m_selectDate.GetDay()); 

	for (int i=m_lstMenu.GetCount()-1; i >=0 ; i--) 
		m_lstMenu.DeleteString(0);   

		CADORecordset rs(m_db);
		CString strSql;
		strSql.Format(L"select story_code, story_name, story_id from t_story where story_date = convert(datetime,'%s',120)", strDate);

		int i_index = 0;
		if(rs.Open(strSql))
		{
			while(!rs.IsEOF())
			{
				CString strCode, strName;
				int i_ID;
				rs.GetFieldValue(0, strCode);
				rs.GetFieldValue(1, strName);

				rs.GetFieldValue(2, i_ID);

				m_lstMenu.AddString(strCode + L" - " + strName); 

				m_lstMenu.SetItemData(i_index, i_ID); 
				i_index++;
				rs.MoveNext();
			}
			rs.Close(); 
		}

}


void CDlgAddToStory::SetPubTypeList()
{

	UpdateData();


	for (int i=m_lbPubType.GetCount()-1; i >=0 ; i--) 
		m_lbPubType.DeleteString(0);   

		CADORecordset rs(m_db);
		CString strSql;
		strSql.Format(L"select code, name, publish_type_id from t_publish_type where flag = 0");

		int i_index = 0;
		if(rs.Open(strSql))
		{
			while(!rs.IsEOF())
			{
				CString strCode, strName;
				int i_ID;
				rs.GetFieldValue(0, strCode);
				rs.GetFieldValue(1, strName);

				rs.GetFieldValue(2, i_ID);

				m_lbPubType.AddString(strCode + L" - " + strName); 

				m_lbPubType.SetItemData(i_index, i_ID); 

				i_index++;
				rs.MoveNext();
			}
			rs.Close(); 
		}

}



void CDlgAddToStory::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strPath, strVerPath, strStoryID;
	int i_StoryID = 0;

	UpdateData();

	if(((CButton*)GetDlgItem(IDC_RADEXIST))->GetCheck() != TRUE)
	{
		CString strStoryCode, strStoryName;
		GetDlgItem(IDC_EDITSTORYNAME)->GetWindowTextW(strStoryName);
		int iCategoryID, iLib_id;
		if(strStoryName.GetLength() == 0 ||
			m_cmbNewLib.GetCurSel() == -1 ) 
//			|| m_cmbStoryCategory.GetCurSel() == -1)
		{
			AfxMessageBox(L"請填好故事資料");
			return;
		}



		CGlobal::GetInstance()->m_FileDB.GetStoryCode(strStoryCode);
		
		int index1, index2;
		index1 = m_cmbStoryCategory.GetCurSel();
		index2 = m_cmbNewLib.GetCurSel();
		iCategoryID = vt_CateID[m_cmbStoryCategory.GetCurSel()];
		iLib_id = vt_LibID[m_cmbNewLib.GetCurSel()];
		if(CGlobal::GetInstance()->m_FileDB.CheckLibStoryPathExist(iLib_id) == FALSE)
		{
			AfxMessageBox(L"請先到Adminpro 設置故事路徑後再進行操作");
			return;
		}

		CString strStoryType = L"";
		int nCount = m_lst_Story_Cat.GetItemCount();
		for (int nIdx=0; nIdx<nCount; nIdx++)
		{
			if (ListView_GetCheckState(m_lst_Story_Cat.m_hWnd, nIdx) != 0)
			{
				int nCat_id = m_lst_Story_Cat.GetItemData(nIdx);
				CString strCat_id;
				if (strStoryType.IsEmpty())
					strCat_id.Format(L"%d",nCat_id);
				else
					strCat_id.Format(L",%d",nCat_id);
				strStoryType += strCat_id;
			}
		}

		i_StoryID = CGlobal::GetInstance()->m_FileDB.InsertStory(strStoryCode, strStoryName, &m_dt_Date_AddStory, iCategoryID, iLib_id);

		if (i_StoryID > 0)
		{
			if (!CGlobal::GetInstance()->m_FileDB.InsertStoryCategoryRelation(i_StoryID, strStoryType))
			{
				AfxMessageBox(L"設置故事類型失敗");
			}
		}
		else
		{
			AfxMessageBox(L"新增故事失敗");
			return;
		}

		strStoryID.Format(L"%d",i_StoryID);
	}
	
	if(((CButton*)GetDlgItem(IDC_RADEXIST))->GetCheck() == TRUE)
	{
		int nRow = 0;
		nRow = m_lstStory.GetNextItem(-1,LVNI_SELECTED);
		if(nRow == -1)
		{
			return;
		}
		CString strStoryCode =m_lstStory.GetItemText(nRow,1);
		CGlobal::GetInstance()->m_FileDB.GetStoryInfo(strStoryCode,strPath, strVerPath, strStoryID);
		CreateAllDirectory(strPath);
		CreateAllDirectory(strVerPath);
		i_StoryID = _wtoi(strStoryID);
	} 
	
	if(((CButton*)GetDlgItem(IDC_RADEXIST))->GetCheck() != TRUE)
	{
		if (!AddToStory(i_StoryID, m_dt_Date_AddStory))
			return;
	}
	else
	{
		if (!AddToStory(i_StoryID, m_dt_Date))
			return;
	}

	CDialog::OnOK();
}


void CDlgAddToStory::OnDtnDatetimechangeDtpicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	LoadListOfStory();
	*pResult = 0;
}


void CDlgAddToStory::OnBnClickedRadexist()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STCEXIST)->EnableWindow(TRUE);
	GetDlgItem(IDC_LSTSTORY)->EnableWindow(TRUE);
	GetDlgItem(IDC_STCDATE)->EnableWindow(TRUE);
	m_dtStoryDate.EnableWindow(TRUE);
	m_dtStoryDate_Add.EnableWindow(FALSE);
	GetDlgItem(IDC_LIST_STORY_TYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STCNEW)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITSTORYNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_STCNEW3)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBOCATEGORY)->EnableWindow(FALSE);
	GetDlgItem(IDC_STCNEW4)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBOLIB)->EnableWindow(FALSE);

	GetDlgItem(IDC_STCDATE_ADDSTORY)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_CB_LIB)->EnableWindow(TRUE);

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	CheckDlgButton(IDC_RADNEW, FALSE);
}


void CDlgAddToStory::OnBnClickedRadnew()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STCEXIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_LSTSTORY)->EnableWindow(FALSE);
	GetDlgItem(IDC_STCDATE)->EnableWindow(FALSE);
	m_dtStoryDate.EnableWindow(FALSE);
	m_dtStoryDate_Add.EnableWindow(TRUE);
	GetDlgItem(IDC_LIST_STORY_TYPE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STCNEW)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITSTORYNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_STCNEW3)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBOCATEGORY)->EnableWindow(TRUE);
	GetDlgItem(IDC_STCNEW4)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBOLIB)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDC_CB_LIB)->EnableWindow(FALSE);
	GetDlgItem(IDC_STCDATE_ADDSTORY)->EnableWindow(TRUE);

	CheckDlgButton(IDC_RADEXIST, FALSE);
}

void CDlgAddToStory::InitList()
{
	LV_COLUMN		lv;
	CRect			rect ;

	CString strItem;
	TCHAR szItem[100];
	m_lstStory.GetWindowRect(&rect);
	lv.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lv.fmt = LVCFMT_LEFT;
	
	lv.iSubItem = 0;
	strItem = L"story_Name";
//	strcpy(szItem,strItem); 
	lstrcpy(szItem,strItem); 
	lv.pszText = szItem;	
	lv.cx = rect.Width();
	m_lstStory.InsertColumn(0, &lv);

	lv.iSubItem = 1;
	strItem = L"story_code";
//	strcpy(szItem,strItem); 
	lstrcpy(szItem,strItem); 
	lv.pszText = szItem;	
	lv.cx = 0;
	m_lstStory.InsertColumn(1, &lv);
	//m_lstStory.SetFullRowSel(TRUE);
	DWORD dwstyle=m_lstStory.GetExtendedStyle();
	dwstyle &=~(LVS_EX_ONECLICKACTIVATE );
	dwstyle |= LVS_EX_FULLROWSELECT;
	m_lstStory.SetExtendedStyle(dwstyle);
}

void CDlgAddToStory::LoadListOfStory()
{
	m_lstStory.DeleteAllItems();
	CADORecordset rs;
	CString strName, strCode, strDate;
	COleDateTime dt;
	m_dtStoryDate.GetTime(dt);
	strDate.Format(L"%04d-%02d-%02d", dt.GetYear(), dt.GetMonth(), dt.GetDay());

	int i_LibID = -1;
	if (m_cbx_Lib.GetCurSel() >0)
		i_LibID = vt_LibID[m_cbx_Lib.GetCurSel()-1];

	BOOL nRet = CGlobal::GetInstance()->m_FileDB.GetStoryList(rs, strDate, i_LibID);
	int nItem=0;
	if(nRet == FALSE)
		return;
	while(!rs.IsEof())
	{
		LV_ITEM lvItem;
		rs.GetFieldValue(L"story_name", strName);
		
		rs.GetFieldValue(L"story_code", strCode);
		nItem=m_lstStory.InsertItem(nItem,strCode);
		m_lstStory.SetItemText(nItem,0,strName);
		m_lstStory.SetItemText(nItem,1,strCode);
		lvItem.iItem=nItem;
		lvItem.pszText=strName.AllocSysString();
		lvItem.mask=LVIF_IMAGE;
		lvItem.iImage = I_IMAGECALLBACK;
		lvItem.iSubItem = 0;
		m_lstStory.SetItem(&lvItem);
		nItem++;
		rs.MoveNext();
	}
}

void CDlgAddToStory::SetComboItems()
{
	CADORecordset rs;
	CString strCateCode, strCateName, strLibName, strLibCode;
	CGlobal::GetInstance()->m_FileDB.GetStoryCategoryList(rs);
	int nItem=0, iCateID, iLibID;
	while(!rs.IsEof())
	{
		rs.GetFieldValue(L"story_category_code", strCateCode);
		rs.GetFieldValue(L"story_category_name", strCateName);
		rs.GetFieldValue(L"story_category_id", iCateID);
		vt_CateID.push_back(iCateID);
		m_cmbStoryCategory.AddString(strCateName);
		rs.MoveNext();
	}
	
	CGlobal::GetInstance()->m_FileDB.GetNewLibList(rs, CGlobal::GetInstance()->m_pShareData->m_Employee.m_nGroup);

	m_cbx_Lib.Clear(); 
	m_cbx_Lib.AddString(L"全部");

	while(!rs.IsEof())
	{
		rs.GetFieldValue(L"lib_code", strLibCode);
		rs.GetFieldValue(L"lib_name", strLibName);
		rs.GetFieldValue(L"lib_id", iLibID);
		vt_LibID.push_back(iLibID);
		m_cmbNewLib.AddString(strLibName);
		m_cbx_Lib.AddString(strLibName);
		rs.MoveNext();
	}

	m_cbx_Lib.SetCurSel(0); 
}

void CDlgAddToStory::CreateAllDirectory(CString strPath)
{
	int nPosStart = 0;
	int nPosEnd = strPath.Find('\\', nPosStart);
	while(nPosEnd != -1){
		CString  path = strPath.Mid(0, nPosEnd+1);
		if(GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES)
			CreateDirectory(path, NULL);
		nPosStart = nPosEnd+1;
		nPosEnd = strPath.Find('\\', nPosStart);
	}
	if(GetFileAttributes(strPath) == INVALID_FILE_ATTRIBUTES)
		CreateDirectory(strPath, NULL);
}

bool CDlgAddToStory::AddToStory(int i_StoryID, COleDateTime dt_target)
{
	CGlobal *pGL=CGlobal::GetInstance();
	int i_PhotoID;
	bool b_Add_OK = true; 
	// copy photo to story item path
	CString runPath = L"";
	////

	COleDateTimeEx dt_sel(m_strDate,DATEFORMAT);
	CString strTempDate = dt_sel.Format(_T("%Y%m%d"));

	CString strTempDate_Target = dt_target.Format(_T("%Y%m%d"));


	CString strLibCode = L"";
	CString strStoryCode  = L"";
	//***************************************************
	// strLibCode, strStory Code Get form DB

	int iLibID = pGL->m_FileDB.GetLibIdStoryCodeFromStoryID(i_StoryID, strStoryCode);
	pGL->m_FileDB.GetLibCodeFromLibId(iLibID, strLibCode);

	CGlobal::GetInstance()->GetStoryItemPath(iLibID); 

//	CString strRootPath = CGlobal::GetInstance()->m_strSIRootPath;
	CString strRootPath_Target = CGlobal::GetInstance()->m_strSIRootPath;

	CString strTextSub = CGlobal::GetInstance()->m_strSITextPath;

//	if (strRootPath.Right(1) == L"\\"  )
//		strRootPath = strRootPath.Left(strRootPath.GetLength()-1); 

/*	CreateDirectory( strRootPath , NULL);
	CreateDirectory( strRootPath + L"\\" + strTempDate  , NULL);
	CreateDirectory( strRootPath + L"\\" + strTempDate + L"\\" + strLibCode  , NULL);
	CreateDirectory( strRootPath + L"\\" + strTempDate + L"\\" + strLibCode  + L"\\" + strStoryCode , NULL);
	CreateDirectory( strRootPath + L"\\" + strTempDate + L"\\" + strLibCode  + L"\\" + strStoryCode + L"\\" + strTextSub, NULL);

	strRootPath = strRootPath + L"\\" + strTempDate + L"\\" + strLibCode  + L"\\" + strStoryCode + L"\\" + strTextSub;
*/
	if (strRootPath_Target.Right(1) == L"\\"  )
		strRootPath_Target = strRootPath_Target.Left(strRootPath_Target.GetLength()-1); 

	/*CreateDirectory( strRootPath_Target , NULL);
	CreateDirectory( strRootPath_Target + L"\\" + strTempDate_Target  , NULL);
	CreateDirectory( strRootPath_Target + L"\\" + strTempDate_Target + L"\\" + strLibCode  , NULL);
	CreateDirectory( strRootPath_Target + L"\\" + strTempDate_Target + L"\\" + strLibCode  + L"\\" + strStoryCode , NULL);
	CreateDirectory( strRootPath_Target + L"\\" + strTempDate_Target + L"\\" + strLibCode  + L"\\" + strStoryCode + L"\\" + strTextSub, NULL);*/
	CreateAllDirectory(strRootPath_Target + L"\\" + strTempDate_Target + L"\\" + strLibCode  + L"\\" + strStoryCode + L"\\" + strTextSub);

	strRootPath_Target = strRootPath_Target + L"\\" + strTempDate_Target + L"\\" + strLibCode  + L"\\" + strStoryCode + L"\\" + strTextSub;

	if (m_b_NewText)
	{
		CString strTitle =L"", strTemp = L"", strContent = L"";
		strContent = LoadFileText(m_strFilePath);
		strTemp = strContent.Left(strContent.Find(L"\r\n")==-1?strContent.GetLength():strContent.Find(L"\r\n"));
		strTitle = strTemp.GetLength()>80?strTemp.Left(80):strTemp;
		int i_story_item_id = -1; 
		CopyFile(m_strFilePath, strRootPath_Target + L"\\" + strFileName + L".txt", FALSE);

		i_story_item_id  = pGL->m_FileDB.InsertStoryItem(i_StoryID, L"T", strRootPath_Target + L"\\" + strFileName + L".txt", strContent, strTitle, L"No Desc", 0, 0, 0, 0, L"0");

		if(i_story_item_id > 0)
		{
			//AfxMessageBox(L"加入故事成功");
			m_i_storyItemID = i_story_item_id;
		}
		else
		{
			b_Add_OK = false; 
			AfxMessageBox(L"加入故事失敗");
		}
	}
	else
	{

		for (int k=0; k < m_arrFileName.GetCount(); k++)
		{
			CString strTmpFileName = m_arrFileName.GetAt(k);

			if (pGL->m_FileDB.InitMissingTxtFile(pGL->GetTempPath(), strTmpFileName))
			{

					CString strNewFileCode = L"";

					if (pGL->m_FileDB.GetNewFileCode(strNewFileCode))
					{
						int i_story_item_id = -1; 
						CopyFile(pGL->GetTempPath() + L"\\" + strTmpFileName + L".txt", strRootPath_Target + L"\\" + strNewFileCode + L".txt", FALSE);
						CString strTitle =L"", strTemp = L"", strContent = L"";
						strContent = LoadFileText(strRootPath_Target + L"\\" + strNewFileCode + L".txt");
						strTemp = strContent.Left(strContent.Find(L"\r\n")==-1?strContent.GetLength():strContent.Find(L"\r\n"));
						strTitle = strTemp.GetLength()>80?strTemp.Left(80):strTemp;

						DeleteFile(pGL->GetTempPath() + L"\\" + strTmpFileName + L".txt");

						if (pGL->m_FileDB.IsStoryItemTagExist(strTmpFileName, i_StoryID))
							i_story_item_id  = pGL->m_FileDB.InsertStoryItem(i_StoryID, L"T", strRootPath_Target + L"\\" + strNewFileCode + L".txt", strContent, strTitle, L"No Desc", 0, 0, 0, 0, L"1");
						else
							i_story_item_id  = pGL->m_FileDB.InsertStoryItem(i_StoryID, L"T", strRootPath_Target + L"\\" + strNewFileCode + L".txt", strContent, strTitle, L"No Desc", 0, 0, 0, 0, L"0");
						CString strSql;
						strSql.Format(L"update t_story_item set editroom_id = N'%s' where story_item_id = %d", strTmpFileName, i_story_item_id);
						pGL->m_FileDB.m_pDB->Execute(strSql);
						if(i_story_item_id > 0)
						{
//							AfxMessageBox(L"加入故事成功");
							m_i_storyItemID = i_story_item_id;
							//
							strSql.Format(L"(select finish_time , dbo.f_GetLibName(t1.lib_id) lib_name , t1.place_code, '' paper_code ,'' column_code , '' column_date "
								L"from file_record t1, text_lib t2 , news_user t3 "
								L"where t1.filecode=N'%s' and t1.filecode=t2.filecode and t1.usercode=t3.usercode "
								L"union "
								L"select t4.finish_time , '' lib_name , '' place_code, paper_code , column_code , column_date "
								L"from text_lib t2 , news_user t3 , paper_record t4 ,news_column t5 "
								L"where t2.filecode=N'%s' and t4.usercode=t3.usercode  and t2.filecode=t4.filecode and t5.column_id=t4.column_id) "
								L"order by finish_time desc", strFileName, strFileName);
							CADORecordset rs1;
							BOOL nRet1 = pGL->m_FileDB.GetRecordFromSQL(strSql, rs1);
							CString strLibName = L"", strPlace = L"", strPaperCode = L"", strColCode = L"";
							if(nRet1 == TRUE)
							{
								rs1.GetFieldValue(L"lib_name", strLibName);
								rs1.GetFieldValue(L"place_code", strPlace);
								rs1.GetFieldValue(L"paper_code", strPaperCode);
								rs1.GetFieldValue(L"column_code", strColCode);
							}

							CString strRemark, strPos;
							if (strPaperCode != L"")
								strPos.Format(L"版面: %s(%s)", strPaperCode,strColCode);
							else
								strPos.Format(L"稿庫: %s(%s)", strLibName,strPlace);

							strRemark.Format(L"稿件名稱: %s %s", strTmpFileName, strPos);
							strSql.Format(L"exec InsertStoryItemOperHistory %d, N'%s', N'%s'",i_story_item_id, L"FE", strRemark);
							pGL->m_FileDB.m_pDB->Execute(strSql);
							CString strStoryName, strStoryLib;
							strSql.Format(L"select * from t_story a inner join news_lib b on a.lib_id = b.lib_id where story_id = %d", i_StoryID);
							CADORecordset rs;
							COleDateTime dtStory;
							BOOL nRet = pGL->m_FileDB.GetRecordFromSQL(strSql, rs);
							if(nRet == TRUE)
							{
								rs.GetFieldValue(L"story_date", dtStory);
								rs.GetFieldValue(L"lib_name", strStoryLib);
								rs.GetFieldValue(L"story_name", strStoryName);
							}
							strRemark.Format(L"送往 %s (%02d-%02d-%04d) 故事名稱: %s", strStoryLib, dtStory.GetMonth(), dtStory.GetDay(), dtStory.GetYear(),strStoryName);
							strSql.Format(L"exec PutFileRecord N'%s', N'加入故事', N'L', N'%s'",  strTmpFileName, strRemark);
							if (!pGL->m_FileDB.m_pDB->Execute(strSql))
							{
								b_Add_OK = false;
								break;
							}
						}
						else
						{
							b_Add_OK = false;
							break;
						}

					}
					else
					{
						b_Add_OK = false;
						break;
					}
			}
			else
			{
				b_Add_OK = false;
				break;
			}
		}

		if (!b_Add_OK)
			AfxMessageBox(L"加入故事失敗");
	}

	return b_Add_OK;
}

void CDlgAddToStory::OnCbnSelchangeCbLib()
{
	// TODO: Add your control notification handler code here
	LoadListOfStory();
}

CString CDlgAddToStory::LoadFileText(CString strFilePath)
{
	CFile fil;
	int pos = 0;
	if ( fil.Open( strFilePath , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) == 0 )
	{		
		return L"" ;
	}
	CString str_temp;
	WORD buffer;
	int bufferCount = 0;
	str_temp.Empty();
	int  bytelen = 0;
	WORD wSignature = 0;
	fil.Read(&wSignature, 2);
	if(wSignature == 0xBBEF || wSignature ==0xBBBF || wSignature ==0xBBEFBF )
	{
		fil.Seek(3,CFile::begin);
		int nFileLen = (int)fil.GetLength()-2;
		char* szDbsBuffer = new char[nFileLen+1];
		ZeroMemory(szDbsBuffer, sizeof(char)*(nFileLen+1));
		int nRead = (int)fil.Read(szDbsBuffer, nFileLen);
		ASSERT(nRead == nFileLen);

		int nMBLen = MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, NULL, NULL);
		WCHAR* szWcsBuffer = new WCHAR[nMBLen+1];
		ZeroMemory(szWcsBuffer, sizeof(WCHAR)*(nMBLen+1));
		MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, szWcsBuffer, nMBLen);
		str_temp = szWcsBuffer;
		str_temp  = str_temp.Right(nFileLen - 1);
	}
	else
	{
		fil.SeekToBegin();
		fil.Read(&wSignature, 2);
		if(wSignature == 0xFEFF || wSignature == 0xFFFE )
		{
			fil.SeekToBegin();
			while ((bytelen=fil.Read(&buffer,sizeof(WORD)))!= 0)
			{
					if(buffer == 65279)
						continue;
					str_temp += CString((WCHAR)buffer);
			} 
		}
		else
		{
 			fil.SeekToBegin();
			char buffer[8192];
			UINT unRead;
    
			unRead = fil.Read(buffer, sizeof(buffer)-1);
			while(unRead)
			{
				buffer[unRead] = '\0'; 
				TRACE(_T("%s"),buffer);
				str_temp += (CString)buffer;
				str_temp += _T("\n");
				unRead = fil.Read(buffer, sizeof(buffer)-1);
			}
		}
	}
	fil.Close();
	return str_temp;
}