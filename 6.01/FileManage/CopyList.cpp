// CopyList.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "CopyList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyList dialog


CCopyList::CCopyList(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopyList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCopyList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyList)
	DDX_Control(pDX, IDC_COPYLIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCopyList, CDialog)
	//{{AFX_MSG_MAP(CCopyList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyList message handlers

void CCopyList::OnOK() 
{
	// TODO: Add extra validation here
	
	
	CDialog::OnOK();
}

void CCopyList::InitList()
{
	LV_COLUMN		lv;
	CRect			rect ;
	int				rgtcx[8] = { 2 , 6 , 2 , 2 , 2 ,2 , 2};	

	CString strItem;
	TCHAR szItem[100];
	m_List.GetWindowRect(&rect);
	lv.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lv.fmt = LVCFMT_LEFT;
	
	lv.iSubItem = 0;
	strItem.LoadString(IDS_D0);
//	strcpy(szItem,strItem); 
	lstrcpy(szItem,strItem); 
	lv.pszText = szItem;	
	lv.cx = rect.Width() * rgtcx[0] / 10;
	m_List.InsertColumn(0, &lv);

	lv.iSubItem = 1;
	strItem.LoadString(IDS_D1);
//	strcpy(szItem,strItem); 
	lstrcpy(szItem,strItem); 
	lv.pszText = szItem;	
	lv.cx = rect.Width() * rgtcx[1] / 10;
	m_List.InsertColumn(1, &lv);

	lv.iSubItem = 2;
	strItem.LoadString(IDS_D2);
//	strcpy(szItem,strItem); 
	lstrcpy(szItem,strItem); 
	lv.pszText = szItem;	
	lv.cx = rect.Width() * rgtcx[2] / 10;
	m_List.InsertColumn(2, &lv);

	lv.iSubItem = 3;
	strItem.LoadString(IDS_D3);
//	strcpy(szItem,strItem); 
	lstrcpy(szItem,strItem); 
	lv.pszText = szItem;	
	lv.cx = rect.Width() * rgtcx[3] / 10;
	m_List.InsertColumn(3, &lv);

	lv.iSubItem = 4;
	strItem.LoadString(IDS_D4);
//	strcpy(szItem,strItem); 
	lstrcpy(szItem,strItem); 
	lv.pszText = szItem;	
	lv.cx = rect.Width() * rgtcx[4] / 10;
	m_List.InsertColumn(4, &lv);

	lv.iSubItem = 5;
	strItem.LoadString(IDS_D5);
//	strcpy(szItem,strItem); 
	lstrcpy(szItem,strItem); 
	lv.pszText = szItem;	
	lv.cx = rect.Width() * rgtcx[5] / 10;
	m_List.InsertColumn(5, &lv);

	lv.iSubItem = 6;
	strItem.LoadString(IDS_D6);
//	strcpy(szItem,strItem); 
	lstrcpy(szItem,strItem); 
	lv.pszText = szItem;	
	lv.cx = rect.Width() * rgtcx[6] / 10;
	m_List.InsertColumn(6,&lv);
	

}

BOOL CCopyList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwstyle=m_List.GetExtendedStyle();
	dwstyle &=~(LVS_EX_ONECLICKACTIVATE );
	dwstyle |= LVS_EX_FULLROWSELECT;
	m_List.SetExtendedStyle(dwstyle);
	m_List.DeleteAllItems();

	InitList();


	GetPaperCopyList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCopyList::GetPaperCopyList()
{
	m_List.DeleteAllItems();
	CGlobal *pGL = CGlobal::GetInstance();
	CADORecordset rs(pGL->m_FileDB.m_pDB);
	pGL->m_FileDB.GetPaperCopyList(&rs,m_strFileCode);
	if (!rs.IsOpen())
		return;

	if (rs.GetRecordCount() == 0)
		return;

	rs.MoveFirst();

	CString str_Temp;pGL->m_FileDB.GetConfig(L"banxing",str_Temp);
	BOOL bBanxing;
	if(str_Temp == L"F")
		bBanxing = FALSE;
	else
		bBanxing = TRUE;

	int nRow;
	nRow = 0;
	double dbWordCount=0.0;
	while(!rs.IsEof())
	{
		CString strItem;
		TCHAR szItem[100];
		
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iItem = nRow;
		
		rs.GetFieldValue(L"filecode",strItem);
//		strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 0;
		lvi.pszText = szItem;
		m_List.InsertItem(&lvi);
		
		rs.GetFieldValue(L"txt_title",strItem);
//		strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 1;
		lvi.pszText = szItem;
		m_List.SetItem(&lvi);

		rs.GetFieldValue(L"txt_words",dbWordCount);
//		sprintf(szItem,_T("%.1f"),dbWordCount);
		CString str;
		if(bBanxing==TRUE)
			str.Format(_T("%.1f"),dbWordCount);
		else
			str.Format(L"%.0f", dbWordCount);
		lvi.iSubItem = 2;
		lvi.pszText = (LPTSTR)(LPCTSTR)str;
		m_List.SetItem(&lvi);

		rs.GetFieldValue(L"lib_name",strItem);
//		strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 3;
		lvi.pszText = szItem;
		m_List.SetItem(&lvi);

		rs.GetFieldValue(L"paper_code",strItem);
//		strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 4;
		lvi.pszText = szItem;
		m_List.SetItem(&lvi);		


		rs.GetFieldValue(L"column_code",strItem);
//		strcpy(szItem,strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 5;
		lvi.pszText = szItem;
		m_List.SetItem(&lvi);		

		rs.GetFieldValue(L"column_date",strItem);
		lstrcpy(szItem,strItem);
		
		lvi.iSubItem = 6;
		lvi.pszText = szItem;
		m_List.SetItem(&lvi);		

		rs.MoveNext();
	}


}
BOOL CCopyList::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
    return CDialog::PreTranslateMessage(pMsg);
}