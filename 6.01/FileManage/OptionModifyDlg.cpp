// OptionModifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "OptionModifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionModifyDlg dialog

COLORREF p_Color_test2[MAX_MODIFY_NUM+1] = 
{
	RGB(0, 0, 255),
	RGB(255, 0, 0),
	RGB(128, 0, 0),
	RGB(0, 128, 128),
	RGB(128, 128, 0),
	RGB(128, 0, 128),
	RGB(0, 0, 128),
	RGB(0, 255, 0),
	RGB(0, 128, 0),
	RGB(255, 0, 255),
	RGB(0, 255, 255),
	RGB(192, 192, 192),
	RGB(192, 0, 128),
	RGB(255, 128, 0),
	RGB(128, 128, 128)
};

COptionModifyDlg::COptionModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionModifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionModifyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOpenUnsaveFile)
	DDX_Control(pDX, IDC_VERSION_LIST, m_lstVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionModifyDlg, CDialog)
	//{{AFX_MSG_MAP(CDlgOpenUnsaveFile)
	ON_BN_CLICKED(IDC_GET_VERSION, OnGetOldVersion)
	ON_NOTIFY (NM_CUSTOMDRAW, IDC_VERSION_LIST, OnNMCustomdrawList )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionModifyDlg message handlers

BOOL COptionModifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_bOldVersion = FALSE;
	m_nSelVersion = -1;

	CRect			rect;
	LV_COLUMN		lvcolumn;
	TCHAR			rgtsz[4][10] = {_T("修改人"),_T("顏色"),  _T("修改日期"),  _T("時間")};
	int				rgtcx[4] = {2,2,2,1};
	
	m_lstVersion.GetWindowRect(&rect);
	// Insert ListCtrl Columns
	for (int i = 0; i < 4; i++)  // add the columns to the list control
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = rgtsz[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = rect.Width() * rgtcx[i] / 7;  // SubItem is twice as large
		m_lstVersion.InsertColumn(i, &lvcolumn);  // assumes return value is OK.
	}
	int index;
	int i = 0;
	while((index = m_strModHistory.Find(L"於"))>= 0)
	{
		CString strtemp = m_strModHistory.Left(index);

		LV_ITEM	 lvitem;
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = i;
		lvitem.iSubItem = 0;
		lvitem.pszText = strtemp.AllocSysString();
		m_lstVersion.InsertItem(&lvitem);

		lvitem.iSubItem = 1;
		lvitem.pszText = L"";
		m_lstVersion.SetItem(&lvitem);

		strtemp = m_strModHistory.Mid(index+1,10);
		lvitem.iSubItem = 2;
		lvitem.pszText = strtemp.AllocSysString();
		m_lstVersion.SetItem(&lvitem);

		strtemp = m_strModHistory.Mid(index+12,5);
		lvitem.iSubItem = 3;
		lvitem.pszText = strtemp.AllocSysString();
		m_lstVersion.SetItem(&lvitem);

		if (m_strModHistory.Find(L"#") < 0)
			break;
		m_strModHistory = m_strModHistory.Mid(m_strModHistory.Find(L"#")+1);
		i++;
	}
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionModifyDlg::OnOK() 
{
	// TODO: Add extra validation here
	//m_arrFile.RemoveAll();
	int nCount=m_lstVersion.GetItemCount();
	BOOL bSel=FALSE;
	for(int i=0;i<nCount;i++)
	{
	}
	if(!bSel)
		return;

	CDialog::OnOK();
}

void COptionModifyDlg::OnGetOldVersion()
{
	int nItem = m_lstVersion.GetNextItem(-1, LVNI_SELECTED);
	if(nItem < 0)
	{
		AfxMessageBox(L"請選擇提取的版本");
		return;
	}
	else
	{
		m_bOldVersion = TRUE;
		m_nSelVersion = nItem;
		CDialog::OnOK();
	}
}

 	
void COptionModifyDlg::OnNMCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = CDRF_DODEFAULT;
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{  
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		if (pLVCD->iSubItem == 1)
		{
			if (pLVCD->nmcd.dwItemSpec <15)
				pLVCD->clrTextBk = p_Color_test2[pLVCD->nmcd.dwItemSpec];
			else
				pLVCD->clrTextBk = p_Color_test2[14];
		}
		else
		{
			pLVCD->clrTextBk = RGB(255, 255, 255);
		}
		*pResult = CDRF_DODEFAULT;
	}

}

BOOL COptionModifyDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
    return CDialog::PreTranslateMessage(pMsg);
}