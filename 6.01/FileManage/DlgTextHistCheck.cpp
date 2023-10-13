// DlgTextValidation.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTextHistCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString p_str_Ver_Del_test2 = L" !@#¡$¢£%¤¥¦^§¨©ª&«¬­®¯°*±²³´µ¶(•¸¹º»¼½)¾¿ÀÁÂÃÄÅ+ÆÇÈÉÊËÌÍÎ{ÏÐÑÒÓÔÕÖ×Ø}ÙÚÛÜÝÞßàáâã<äåæçèéêëìíîï>ðñòóôõö÷øùúûý";
CString p_str_Ver_test2 = L"Aabcdefghijklmno";

const short FILENAME	=0;
const short CURPOSITION	=1;
const short CREATEDATE	=2;
const short LASTMODIFYDATE	=3;
const short FILETITLE	=4;
const short FILEAUTHOR	=5;
const short CURUSER		=6;
const short CONTENT	=7;
const short VERSION	=8;

/////////////////////////////////////////////////////////////////////////////
// CDlgTextHistCheck dialog

CDlgTextHistCheck::CDlgTextHistCheck(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTextHistCheck::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTextHistCheck)
		// NOTE: the ClassWizard will add member initialization here
		m_bThreadInProcess = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgTextHistCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTextHistCheck)
	DDX_Control(pDX, IDC_FILELIST, m_lstFile);
	DDX_Control(pDX, IDC_FILERECORD, m_lcRecord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTextHistCheck, CDialog)
	//{{AFX_MSG_MAP(CDlgTextHistCheck)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(ID_CANCEL, OnCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, OnDblclkFilelist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILELIST, OnItemchangedFilelist)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_FILELIST, OnColumnclickFilelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTextHistCheck message handlers
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, 
    LPARAM lParamSort)
{
	CString *p1,*p2;
	p1=(CString *)lParam1;
	p2=(CString *)lParam2;
	if(*p1>*p2) return lParamSort;
	if(*p1==*p2) return 0;
	return -lParamSort;
}

void CDlgTextHistCheck::Create()
{
	CDialog::Create(IDD, NULL);   
}

BOOL CDlgTextHistCheck::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CGlobal *pGlobal=CGlobal::GetInstance();
	rs = new CADORecordset(pGlobal->m_FileDB.m_pDB);
		
	DWORD dwstyle=m_lstFile.GetExtendedStyle();
	dwstyle &=~(LVS_EX_ONECLICKACTIVATE );
	dwstyle |= LVS_EX_FULLROWSELECT;
	m_lstFile.SetExtendedStyle(dwstyle);

	dwstyle=m_lcRecord.GetExtendedStyle();
	dwstyle &=~(LVS_EX_ONECLICKACTIVATE );
	dwstyle |= LVS_EX_FULLROWSELECT;
	m_lcRecord.SetExtendedStyle(dwstyle);

	OnInitFileList();
	OnInitRecordList();
	
	m_bRunning = TRUE;
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTextHistCheck::OnInitFileList()
{
	CRect			rect;
	int				i ;
	LV_COLUMN		lvcolumn;
	CString			rgtsz[9] ;
	int				rgtcx[9] = { 2 ,6, 3 , 3, 5, 2, 2, 0, 0};
	int				rgtcol[9] = {FILENAME, CURPOSITION, CREATEDATE, LASTMODIFYDATE, FILETITLE, FILEAUTHOR, CURUSER, CONTENT, VERSION};

	rgtsz[0].LoadString(IDS_FILENAME);
	rgtsz[1].LoadString(IDS_CURPOSITION);
	rgtsz[2] = L"創建日期";
	rgtsz[3].LoadString(IDS_UPDATEDATE);
	rgtsz[4].LoadString(IDS_TITLE);
	rgtsz[5].LoadString(IDS_FILEAUTHOR);
	rgtsz[6].LoadString(IDS_ASSIGNEDUSER);	
	rgtsz[7] = L"稿件";
	rgtsz[8] = L"歷史稿件";

	m_lstFile.GetWindowRect(&rect);
	
	// Insert ListCtrl Columns
	for (i = 0; i < sizeof(rgtcol)/sizeof(int); i++)  // add the columns to the list control
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		if(i==3 || i==2)
			lvcolumn.fmt = LVCFMT_RIGHT;
		else
			lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = (LPTSTR)(LPCTSTR)rgtsz[rgtcol[i]];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = rect.Width() * rgtcx[rgtcol[i]] / 24;  // SubItem is twice as large
		m_lstFile.InsertColumn(i, &lvcolumn);  // assumes return value is OK.
	}
}

void CDlgTextHistCheck::OnInitRecordList()
{
	CRect			rect;
	int				i ;
	LV_COLUMN		lvcolumn;
	CString			rgtsz[6] ;
	int				rgtcx[6] = { 3 , 2 , 2 ,1 , 6 , 6 };
	int				rgtcol[6]={FILERECORD_DOTIME,FILERECORD_OPERATION,FILERECORD_OPERATOR,
		FILERECORD_WORDCOUNT,FILERECORD_POSITION,FILERECORD_COMMENT};
	CResManage res(g_hInst);
	rgtsz[0].LoadString(IDS_RCDITEM0);
	rgtsz[1].LoadString(IDS_RCDITEM1);
	rgtsz[2].LoadString(IDS_RCDITEM2);
	rgtsz[3].LoadString(IDS_RCDITEM3);
	rgtsz[4].LoadString(IDS_RCDITEM4);
	rgtsz[5].LoadString(IDS_RCDITEM5);
    AfxGetMainWnd()->GetClientRect(&rect);
	
	// Insert ListCtrl Columns
	for (i = 0; i < 6; i++)  // add the columns to the list control
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = (LPTSTR)(LPCTSTR)rgtsz[rgtcol[i]];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = (rect.Width()- 100) * rgtcx[rgtcol[i]] / 30;  // SubItem is twice as large
		m_lcRecord.InsertColumn(i, &lvcolumn);  // assumes return value is OK.
	}
}

void CDlgTextHistCheck::OnSearch()
{
	if (m_bThreadInProcess)
		EndBackgroundInsert();
	m_lstFile.DeleteAllItems();
	SetDlgItemText(IDC_STATIC_TOTALCOUNT,L"0");
	SetDlgItemText(IDC_STATIC_TOTALCHECK,L"0");
	SetDlgItemText(IDC_STATIC_TOTALWRONG,L"0");
	CGlobal *pGlobal=CGlobal::GetInstance();
	if (pGlobal->m_FileDB.GetTextData(rs))
		AfxBeginThread(BeginBackgroundInsert, this); 
}

void CDlgTextHistCheck::OnCancel()
{
	if (m_bThreadInProcess)
		EndBackgroundInsert();
	m_bRunning = FALSE;
	CDialog::OnCancel();
}

BOOL CDlgTextHistCheck::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
    return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTextHistCheck::OnColumnclickFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString *pStr=NULL;
	int nCount = m_lstFile.GetItemCount();
	if(nCount<=0)
		return;

	*pResult = 0;
	m_lstFile.SetRedraw(0);
	int iSubItem = pNMListView->iSubItem;
	if(iSortSubItem != iSubItem)
	{
		iSortSubItem = iSubItem;
		iSortPara = 1;
	}
	else iSortPara =- iSortPara;
	for(int i=0;i<nCount;i++)
	{
		pStr=(CString*)m_lstFile.GetItemData(i);
		if(pStr==NULL) pStr=new CString;
		(*pStr)=m_lstFile.GetItemText(i,iSubItem);		
		m_lstFile.SetItemData(i,(DWORD)pStr);
	}
	int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	m_lstFile.SortItems( (PFNLVCOMPARE)CompareFunc, iSortPara );
	m_lstFile.EnsureVisible(m_lstFile.GetNextItem( -1, LVNI_FOCUSED) ,0);
	m_lstFile.SetRedraw(1);
}

void CDlgTextHistCheck::OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nItem=m_lstFile.GetNextItem(-1,LVNI_SELECTED);
	if(nItem<0)
		return;
	FILEPARAM fileParam;
	memset(&fileParam,0,sizeof(FILEPARAM));
	_tcscpy(fileParam.szFileCode,m_lstFile.GetItemText(nItem,FILENAME));
	CString strDate,strFileFMT = L"Txt";
	fileParam.bEdit = FALSE;
	CGlobal::GetInstance()->OpenFile(strFileFMT,strDate,&fileParam,0, 0,m_nWrong);
	ShowWindow(SW_MINIMIZE);
	*pResult = 0;
}

void CDlgTextHistCheck::OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_lcRecord.DeleteAllItems();
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	UINT nNewState, nOldState;
	
	nNewState = (pNMListView->uNewState & (LVIS_FOCUSED|LVIS_SELECTED));
	nOldState = (pNMListView->uOldState & (LVIS_FOCUSED|LVIS_SELECTED));
	if (nNewState!=0&&nNewState != nOldState)
	{
		int nItem=m_lstFile.GetNextItem(-1,LVNI_SELECTED);
		if(nItem<0)
			return;
		CGlobal *pGlobal = CGlobal::GetInstance();
		GetFileRecord(&m_lcRecord,m_lstFile.GetItemText(nItem,FILENAME));
		CString str, strContent, strVersion;
		strContent = m_lstFile.GetItemText(nItem,CONTENT);
		strVersion = m_lstFile.GetItemText(nItem,VERSION);
		str.Format(L"稿件字數: %d",strContent.GetLength());
		SetDlgItemText(IDC_STATIC_TOTALWORDS,str);
		str.Format(L"歷史字數: %d",strVersion.GetLength());
		SetDlgItemText(IDC_STATIC_TOTALHISTWORDS,str);
		int i=0;
		for (; i<strContent.GetLength(); i++)
			if(strContent[i]!=strVersion[i])
			{
				break;
			}
		m_nWrong = i;
		str.Format(L"與歷史不符位置: %d",m_nWrong);
		SetDlgItemText(IDC_STATIC_WRONG,str);
		m_nWrong += strContent.Left(m_nWrong).Replace(L"\r",L"\r");
	}
	*pResult = 0;
}

BOOL CDlgTextHistCheck::GetFileRecord(CListCtrl *pList,CString &strFileCode)
{
	CGlobal *pGlobal = CGlobal::GetInstance();
	CADORecordset rs(pGlobal->m_FileDB.m_pDB);
	if(pGlobal->m_FileDB.GetFileRecord(&rs,strFileCode))
	{
		CString strOper,str,str1,str2,str3;
		strOper.LoadString(IDS_OPERATION);

		LV_ITEM			lvitem;
		lvitem.mask = LVIF_TEXT;
		int nRow=0;
		while(!rs.IsEof())
		{
			//操作時間
			rs.GetFieldValue(_T("finish_time"),str,DATETIMEFORMAT);
			lvitem.iItem = nRow;
			lvitem.iSubItem = 0;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str ;
			nRow=pList->InsertItem(&lvitem);
			pList->SetItem(&lvitem);
			//操作性質
			rs.GetFieldValue(_T("oper"),str);

		
			lvitem.iSubItem = 1 ;

			if(str.CompareNoCase(_T("Transfer"))==0)
				str=strOper.Left(2);
			else if(str.CompareNoCase(_T("secret"))==0)
				str=strOper.Mid(2,2);
			else if(str.CompareNoCase(_T("un secret"))==0)
				str=strOper.Mid(4,2);
			else if(str.CompareNoCase(_T("urgency"))==0)
				str=strOper.Mid(6,2);
			else if(str.CompareNoCase(_T("un urgency"))==0)
				str=strOper.Mid(8,2);
			else if(str.CompareNoCase(_T("Delete"))==0)
				str=strOper.Mid(10,2);
			else if(str.CompareNoCase(_T("Restore"))==0)
				str=strOper.Mid(12,2);
			else if(str.CompareNoCase(_T("Back"))==0||str.CompareNoCase(_T("B"))==0)
				str=strOper.Mid(14,2);
			else if(str.CompareNoCase(_T("ReSign"))==0||str.CompareNoCase(_T("R"))==0)
				str=strOper.Mid(16,2);
			else if(str.CompareNoCase(_T("Sign"))==0||str.CompareNoCase(_T("S"))==0)
				str=strOper.Mid(18,2);
			else if(str.CompareNoCase(_T("copy"))==0)
				str=strOper.Mid(20,2);
			else if(str.CompareNoCase(_T("C"))==0)
				str=strOper.Mid(22,4);
			else if(str.CompareNoCase(_T("T"))==0)
				str=strOper.Mid(26,2);
			
			lvitem.pszText = (LPTSTR)(LPCTSTR)str ;
			pList->SetItem(&lvitem);
			//操作者
			rs.GetFieldValue(_T("username"),str);
			lvitem.iSubItem = 2 ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str ;
			pList->SetItem (&lvitem );
			//字數
			int nWords;
			rs.GetFieldValue(_T("txt_words"),nWords);
			lvitem.iSubItem = 3 ;
			str.Format(_T("%d"),nWords);
			lvitem.pszText =  (LPTSTR)(LPCTSTR)str ;
			pList->SetItem(&lvitem);
			//位置
			int nPlace;
			rs.GetFieldValue(_T("place"),nPlace);
			if(nPlace==0)
			{
				//部門
				rs.GetFieldValue(_T("lib_name"),str1);
				//稿庫
				rs.GetFieldValue(_T("place_code"),str2);
				CString str3,str4;
				rs.GetFieldValue(_T("src_place"),str3);
				rs.GetFieldValue(_T("srclib_name"),str4);
				if(str1==str4&&str2==str3||str4.IsEmpty())
					str.Format(_T("%s(%s)"),str1,str2);
				else
					str.Format(_T("%s(%s)->%s(%s)"),str4,str3,str1,str2);
			}
			else if(nPlace==1)
			{
				//報紙名稱
				rs.GetFieldValue(_T("paper_code"),str1);
				//版面名稱
				rs.GetFieldValue(_T("column_code"),str2);
				//版面日期
				rs.GetFieldValue(_T("column_date"),str3,DATEFORMAT);
				str.Format(_T("%s %s(%s)"),str2,str3,str1);
			}
			lvitem.iSubItem = 4 ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str ;
			pList->SetItem(&lvitem);
			//備註
			rs.GetFieldValue(_T("note"),str);
			lvitem.iSubItem = 5 ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str ;
			pList->SetItem(&lvitem );
			rs.MoveNext();
		}
		rs.Close();
	}
	return FALSE;
}

UINT CDlgTextHistCheck::BeginBackgroundInsert(LPVOID pParam)
{
	CDlgTextHistCheck* pMgr=(CDlgTextHistCheck*) pParam;
	if(pMgr->m_bThreadInProcess == TRUE)
		return 0;
	pMgr->m_bThreadInProcess = TRUE;
    pMgr->BackgroundInsert();
	pMgr->m_bThreadInProcess = FALSE;
	return 1;
}

void CDlgTextHistCheck::BackgroundInsert()
{
	if (rs->IsEOF())
		return;
	CString strFileCode, strVersion, strVersion_Mark, strLastUser, strModHistory, strContent, strValue;
	CGlobal *pGlobal=CGlobal::GetInstance();
	int iItem=0, k = 1;
	strValue.Format(L"%d",rs->GetRecordCount());
	SetDlgItemText(IDC_STATIC_TOTALCOUNT,strValue);
	while (!rs->IsEOF())
	{
		rs->GetFieldValue(_T("filecode"), strFileCode);
		rs->GetFieldValueContent(_T("txt_textcontent"),strContent);
		rs->GetFieldValueContent(_T("text_version"),strVersion);
		rs->GetFieldValue(_T("text_version_mark"),strVersion_Mark);
		rs->GetFieldValue(_T("text_mod_hist"),strModHistory);
		int i = 1 + strModHistory.Replace(L"#",L"#");
		CString str_Ver_Del = p_str_Ver_Del_test2[1];
		for (int j = 1; j < p_str_Ver_test2.GetLength(); j++)
		{
			if (j<i)
				str_Ver_Del += p_str_Ver_Del_test2.Mid(j*(j-1)/2+2,i-1) + p_str_Ver_Del_test2.Mid(j*(j-1)/2+2+i,j-i);
			else
				str_Ver_Del +=  p_str_Ver_Del_test2.Mid(j*(j-1)/2+2+i,j-i);
		}
		str_Ver_Del += p_str_Ver_test2.Mid(i+1);
		int index;
		while((index = strVersion_Mark.FindOneOf(str_Ver_Del))>= 0)
		{
			strVersion.Delete(index);
			strVersion_Mark.Delete(index);
		}

		if (strVersion.GetLength()==0)
		{
			strLastUser.Format(L"%c%c",65279,32);
			strContent.Replace(strLastUser,L"");
		}
		TCHAR char_temp = strContent.GetAt(0);
		if (char_temp == 65279)
			strContent = strContent.Right(strContent.GetLength()-1);
		strContent.Replace(L"\r\n",L"\r");
		strContent.Replace(L"\n",L"\r");
		strVersion.Replace(L"\r\n",L"\r");
		strVersion.Replace(L"\n",L"\r");
		if (strContent != strVersion)
		{
			LV_ITEM	lvitem;
			CString str;
						//filecode
			rs->GetFieldValue(_T("filecode"),str);
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iItem ;
			lvitem.iSubItem = FILENAME ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstFile.InsertItem(&lvitem);
			//標題
			rs->GetFieldValue(_T("txt_title"),str);
			lvitem.iSubItem = FILETITLE ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstFile.SetItem(&lvitem);

 			lvitem.iSubItem = CONTENT ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strContent;
			m_lstFile.SetItem(&lvitem);

			lvitem.iSubItem = VERSION ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strVersion;
			m_lstFile.SetItem(&lvitem);

			rs->GetFieldValue(_T("createtime"), str);
			lvitem.iSubItem = CREATEDATE;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstFile.SetItem(&lvitem);

			rs->GetFieldValue(_T("last_modtime"), str);
			lvitem.iSubItem = LASTMODIFYDATE;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstFile.SetItem(&lvitem);
			//作者
			rs->GetFieldValue(_T("txt_author"),str);
			lvitem.iSubItem = FILEAUTHOR ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstFile.SetItem(&lvitem);
			
			//位置
			COleDateTime dt;
			rs->GetFieldValue(_T("position"),str);
			if(!str.IsEmpty())
			{
				CString str1,str2,str3;
				TCHAR tcPos=str.GetAt(0);
				switch(tcPos) {
				case 'L':
					str3.LoadString(IDS_LIBRARY);
					rs->GetFieldValue(_T("libname"),str1);
					rs->GetFieldValue(_T("place_code"),str2);
					str.Format(_T("%s (%s %s)"),str3,str1,str2);
					break;
				case 'T':
					str3.LoadString(IDS_TRANSLIB);
					rs->GetFieldValue(_T("libname"),str1);
					rs->GetFieldValue(_T("place_code"),str2);
					str.Format(_T("%s (%s %s)"),str3,str1,str2);
					break;
				case 'C':
					str3.LoadString(IDS_COLUMN);
					rs->GetFieldValue(_T("paper"),str1);
					rs->GetFieldValue(_T("colcode"),str2);
					
					rs->GetFieldValue(_T("column_date"),dt);
					str.Format(_T("%s (%s %s %02d-%02d)"),str3,str1,str2,dt.GetMonth(),dt.GetDay());
					break;
				case 'P':
					str3.LoadString(IDS_SIGNED);
					rs->GetFieldValue(_T("paper"),str1);
					rs->GetFieldValue(_T("colcode"),str2);
					rs->GetFieldValue(_T("column_date"),dt);
					str.Format(_T("%s (%s %s %02d-%02d)"),str3,str1,str2,dt.GetMonth(),dt.GetDay());
					break;
				default:
					str.Empty();
				}
			}
			lvitem.iSubItem = CURPOSITION;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstFile.SetItem(&lvitem);
			//當前用戶			
			rs->GetFieldValue(_T("username"), str);
			lvitem.iSubItem = CURUSER;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstFile.SetItem(&lvitem);

			iItem++;
			strValue.Format(L"%d",iItem);
			SetDlgItemText(IDC_STATIC_TOTALWRONG,strValue);
		}
		strValue.Format(L"%d",k);
		SetDlgItemText(IDC_STATIC_TOTALCHECK,strValue);
		k++;
		rs->MoveNext();
	}
	rs->Close();
}

void CDlgTextHistCheck::EndBackgroundInsert()
{
	rs->MoveLast();
	do
	{
		MSG msg;
		GetMessage(&msg,NULL,0,0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}while(m_bThreadInProcess); 
	if (rs->IsOpen())
		rs->Close();
}