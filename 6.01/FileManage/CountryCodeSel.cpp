// CountryCodeSel.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "CountryCodeSel.h"
#include "replacefactory.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCountryCodeSel dialog


CCountryCodeSel::CCountryCodeSel(CWnd* pParent /*=NULL*/)
	: CDialog(CCountryCodeSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCountryCodeSel)
	//}}AFX_DATA_INIT
}


void CCountryCodeSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCountryCodeSel)
	DDX_Control(pDX, IDC_CMBCODE, m_cmbCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCountryCodeSel, CDialog)
	//{{AFX_MSG_MAP(CCountryCodeSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCountryCodeSel message handlers

BOOL CCountryCodeSel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(!CGlobal::GetInstance()->GetCountryCode(&m_cmbCode))
		CDialog::EndDialog(IDCANCEL);
	if(m_cmbCode.GetCount()==1)
	{
		int nCode = m_cmbCode.GetItemData(0);
		CRepFactory *pFactory=CRepFactory::Instance();
		if(pFactory->Init(nCode/*countrycode*/))
		{
			CDialog::EndDialog(IDOK);
			return TRUE;
		}
		else
		{
			CDialog::EndDialog(IDCANCEL);
			AfxMessageBox(IDS_LOADSPELLPARMERR);
			return TRUE;
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCountryCodeSel::OnOK() 
{
	// TODO: Add extra validation here
	int nIndex;
	if((nIndex=m_cmbCode.GetCurSel())==CB_ERR)
	{
		CDialog::OnCancel();
		return ;
	}
	int nCode = m_cmbCode.GetItemData(nIndex);
	CRepFactory *pFactory=CRepFactory::Instance();
	if(pFactory->Init(nCode/*countrycode*/))
		CDialog::OnOK();
	else
		AfxMessageBox(IDS_LOADSPELLPARMERR);
}
