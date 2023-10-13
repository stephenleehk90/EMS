// Recycle.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "Recycle.h"
#include "attachfiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecycle dialog


CRecycle::CRecycle(CWnd* pParent /*=NULL*/)
	: CDialog(CRecycle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecycle)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRecycle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecycle)
	DDX_Control(pDX, IDC_FILELIST, m_File);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecycle, CDialog)
	//{{AFX_MSG_MAP(CRecycle)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_RESTORE, OnRestore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecycle message handlers

BOOL CRecycle::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwstyle=m_File.GetExtendedStyle();
	dwstyle &=~(LVS_EX_ONECLICKACTIVATE );
	dwstyle |= LVS_EX_FULLROWSELECT;
	m_File.SetExtendedStyle(dwstyle);
	m_File.DeleteAllItems();

	InitList();
	LoadDeletedFile();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecycle::Refresh()
{

}

void CRecycle::InitList()
{
	CRect			rect;
	m_File.GetWindowRect(&rect);
	
	LV_COLUMN		lvcolumn;
	
	CString strItem;
	TCHAR szItem[100];
	
	
	lvcolumn.iSubItem = 0;
	strItem.LoadString(IDS_R0);
//	strcpy(szItem,strItem);
	lstrcpy(szItem,strItem);
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = szItem;
	lvcolumn.cx = rect.Width() * 20 /100;
	m_File.InsertColumn(0,&lvcolumn);
	
	lvcolumn.iSubItem = 1;
	strItem.LoadString(IDS_R1);
//	strcpy(szItem,strItem);
	lstrcpy(szItem,strItem);
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = szItem;
	lvcolumn.cx = rect.Width() * 20 /100;
	m_File.InsertColumn(1,&lvcolumn);
	
	lvcolumn.iSubItem = 2;
	strItem.LoadString(IDS_R2);
//	strcpy(szItem,strItem);
	lstrcpy(szItem,strItem);
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = szItem;
	lvcolumn.cx = rect.Width() * 20 /100;
	m_File.InsertColumn(2,&lvcolumn);

	lvcolumn.iSubItem = 3;
	strItem.LoadString(IDS_R3);
//	strcpy(szItem,strItem);
	lstrcpy(szItem,strItem);
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = szItem;
	lvcolumn.cx = rect.Width() * 15 /100;
	m_File.InsertColumn(3,&lvcolumn);

	lvcolumn.iSubItem = 4;
	strItem.LoadString(IDS_R4);
//	strcpy(szItem,strItem);
	lstrcpy(szItem,strItem);
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_RIGHT;
	lvcolumn.pszText = szItem;
	lvcolumn.cx = rect.Width() * 10 /100;
	m_File.InsertColumn(4,&lvcolumn);

	lvcolumn.iSubItem = 5;
	strItem.LoadString(IDS_R5);
//	strcpy(szItem,strItem);
	lstrcpy(szItem,strItem);
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = szItem;
	lvcolumn.cx = rect.Width() * 35 /100;
	m_File.InsertColumn(5,&lvcolumn);
}

void CRecycle::LoadDeletedFile()
{
	m_File.DeleteAllItems();
	CGlobal *pGL = CGlobal::GetInstance();
	CADORecordset rs(pGL->m_FileDB.m_pDB);
	pGL->m_FileDB.GetDeletedFile(&rs);
	if (!rs.IsOpen())
		return;
	if (rs.GetRecordCount() <= 0)
		return;

	int nRow;
	nRow = 0;
	rs.MoveFirst();
	while (!rs.IsEof())
	{
		CString strItem;
		TCHAR szItem[100];
		
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iItem = nRow;
		
		rs.GetFieldValue(L"filecode",strItem);
		//strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 0;
		lvi.pszText = szItem;
		m_File.InsertItem(&lvi);

		rs.GetFieldValue(L"position",strItem);
//		strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 1;
		lvi.pszText = szItem;
		m_File.SetItem(&lvi);

		rs.GetFieldValue(L"finish_time",strItem);
//		strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 2;
		lvi.pszText = szItem;
		m_File.SetItem(&lvi);

		rs.GetFieldValue(L"usercode",strItem);
		//strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 3;
		lvi.pszText = szItem;
		m_File.SetItem(&lvi);

		
		rs.GetFieldValue(L"txt_words",strItem);
//		strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 4;
		lvi.pszText = szItem;
		m_File.SetItem(&lvi);

		rs.GetFieldValue(L"txt_title",strItem);
//		strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 5;
		lvi.pszText = szItem;
		m_File.SetItem(&lvi);
		rs.MoveNext();
	}
			
	
}

void CRecycle::OnClear() 
{
	// TODO: Add your control notification handler code here
	int nItem;
	nItem =  m_File.GetNextItem(-1,LVNI_SELECTED);
	if (nItem < 0) 
	{
		return;
	}
	CString strCode;
	BOOL bDelAttach = FALSE;
	strCode = m_File.GetItemText(nItem,0);
	CGlobal *pGL = CGlobal::GetInstance();
	if(pGL->m_FileDB.CheckAttachment(strCode)==TRUE)
	{
		if (AfxMessageBox(L"稿件有附件關聯，繼續清除會一併把附件清除\n確定要刪除嗎？",MB_YESNO) != IDYES)
			return;
		bDelAttach = TRUE;
	}
	else
	{
		if (AfxMessageBox(IDS_DELETE,MB_YESNO) != IDYES)
			return;
	}
	pGL->m_FileDB.ClearFile(strCode);
	if(bDelAttach == TRUE)
	{
		CADORecordset rs(pGL->m_FileDB.m_pDB);
		pGL->m_FileDB.LoadAttach(&rs,strCode);
		CString m_strFileCode, strUser, strUploadDate, strSourceName, strAttachName, strSql;
		COleDateTime dt;
		for(int i=0;i<rs.GetRecordCount();i++)
		{
			if(!rs.IsEof())
			{
				rs.GetFieldValue(L"attach_name", strAttachName);
				rs.GetFieldValue(L"username", strUser);
				rs.GetFieldValue(L"upload_date", dt);
				strUploadDate = dt.Format(L"%Y%m%d");

				strSourceName=strUploadDate+_T("\\");
				strSourceName+=strCode;
				strSourceName+=strAttachName;
				if(DeleteFile(strSourceName,strAttachName))
				{
				}
			}
		}
		strSql.Format(L"delete from attachs where filecode = N'%s'", strCode);
		pGL->m_FileDB.m_pDB->Execute(strSql);
	}
	LoadDeletedFile();
	
	
}

void CRecycle::OnRestore() 
{
	// TODO: Add your control notification handler code here
	int nItem;
	nItem =  m_File.GetNextItem(-1,LVNI_SELECTED);
	
	if (nItem < 0) 
	{
		return;
	}

	CString strCode;
	strCode = m_File.GetItemText(nItem,0);
	CGlobal *pGL = CGlobal::GetInstance();
	pGL->m_FileDB.RestoreFile(strCode);

	LoadDeletedFile();
	
}

BOOL CRecycle::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CRecycle::DeleteFile(CString strFileName, CString strAttachName)
{
	strFileName.Format(_T("%s\\%s"),CGlobal::GetInstance()->m_pEntry->strAttachPath,strFileName);
//	AfxMessageBox(strFileName);

	if( _taccess(strFileName, 0 ) == -1)
	{
		return TRUE;	
	}

	TRY
	{
		CFile fSource;			
		CFileStatus FileStatus;			
		fSource.GetStatus((LPCTSTR)strFileName,FileStatus);
		FileStatus.m_attribute=0x00;
		fSource.SetStatus((LPCTSTR)strFileName,FileStatus);
		//_t
		CGlobal::GetInstance()->delete_file(strFileName,CGlobal::GetInstance()->m_pShareData->m_Employee.m_strUserCode,CGlobal::GetInstance()->m_pShareData->str_company,L"_t");
	}
	CATCH( CFileException, e )
	{
		return false;
	}
	END_CATCH
	return true;
}