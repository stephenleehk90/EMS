// ChoseDicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "ChoseDicDlg.h"
#include "WordCheckDlg.h"
#include "Global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChoseDicDlg dialog


CChoseDicDlg::CChoseDicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChoseDicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChoseDicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChoseDicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChoseDicDlg)
	DDX_Control(pDX, IDC_LIST1, m_DicList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChoseDicDlg, CDialog)
	//{{AFX_MSG_MAP(CChoseDicDlg)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CHANGE_ALL_DEF, OnChangeAllDef)
	ON_BN_CLICKED(ID_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChoseDicDlg message handlers

void CChoseDicDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	if (m_DicList.GetSelCount()==0)
	{
		AfxMessageBox(L"請選擇字典");
		return;
	}
	CWordCheckDlg dlg;
	CString strSelectedDic;
	int nCount = m_DicList.GetSelCount();

	int* buffer = new int[nCount];
	memset(buffer, -1, sizeof(int)*nCount);
	m_DicList.GetSelItems(nCount, buffer);
	CString tmp,tmp1,tmp2,strLibGroup;
	for(int i=0;i<nCount; i++)
	{
		m_DicList.GetText(buffer[i],tmp1);

		if (nCount==0)
		{
			return;
		}
		else if (nCount==1)
		{
			strLibGroup.Format(L"(N'%s')",tmp1);
			break;
		}
		else if(nCount==2){
			m_DicList.GetText(buffer[1],tmp2);
			strLibGroup.Format(L"(N'%s',N'%s')",tmp1,tmp2);
			break;
		}
		else{
			if (i==0)
			{
				tmp.Format(L"(N'%s',",tmp1);
			}
			else if(i==(nCount-1)){
				tmp.Format(L"N'%s')",tmp1);
			}
			else 
				tmp.Format(L"N'%s',",tmp1);
			strLibGroup+=tmp;//lstrcat(strLibGroup,tmp);
		}

	}
	delete[] buffer;
	CDialog::OnOK();
	dlg.OnCheck(strLibGroup);
	
}

void CChoseDicDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

BOOL CChoseDicDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    CGlobal *pGlobal = CGlobal::GetInstance();
	pGlobal->m_FileDB.LoadWordLib(&m_DicList);
	m_DicList.SetSel(-1);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChoseDicDlg::OnChangeAllDef()
{
	if (m_DicList.GetSelCount()==0)
	{
		AfxMessageBox(L"請選擇字典");
		return;
	}
	CWordCheckDlg dlg;
	CString strSelectedDic;
	int nCount = m_DicList.GetSelCount();

	int* buffer = new int[nCount];
	memset(buffer, -1, sizeof(int)*nCount);
	m_DicList.GetSelItems(nCount, buffer);
	CString tmp,tmp1,tmp2,strLibGroup;
	for(int i=0;i<nCount; i++)
	{
		m_DicList.GetText(buffer[i],tmp1);

		if (nCount==0)
		{
			return;
		}
		else if (nCount==1)
		{
			strLibGroup.Format(L"(N'%s')",tmp1);
			break;
		}
		else if(nCount==2){
			m_DicList.GetText(buffer[1],tmp2);
			strLibGroup.Format(L"(N'%s',N'%s')",tmp1,tmp2);
			break;
		}
		else{
			if (i==0)
			{
				tmp.Format(L"(N'%s',",tmp1);
			}
			else if(i==(nCount-1)){
				tmp.Format(L"N'%s')",tmp1);
			}
			else 
				tmp.Format(L"N'%s',",tmp1);
			strLibGroup+=tmp;//lstrcat(strLibGroup,tmp);
		}

	}
	delete[] buffer;
	CDialog::OnOK();
	dlg.m_bCheckDefFirst=TRUE;
	dlg.OnCheck(strLibGroup);
}

BOOL CChoseDicDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
    return CDialog::PreTranslateMessage(pMsg);
}