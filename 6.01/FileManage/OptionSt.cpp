// OptionSt.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "OptionSt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

IMPLEMENT_DYNAMIC(COptionSheet, CPropertySheet)

COptionSheet::COptionSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPages();
}

COptionSheet::COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPages();
}

COptionSheet::~COptionSheet()
{
}

void COptionSheet::AddPages()
{
	AddPage(&m_TextStylesPage);
	AddPage(&m_EditOptionPage);
	AddPage(&m_PrintOptionPage);
}

BEGIN_MESSAGE_MAP(COptionSheet, CPropertySheet)
	//{{AFX_MSG_MAP(COptionSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionSheet message handlers

BOOL COptionSheet::OnInitDialog() 
{
	CString str;

	str.Format(L"%s",L"����");
	GetDlgItem(ID_APPLY_NOW)->SetWindowText(str);
	str = L"�T�w";
	SetDlgItemText(IDOK, str);
	str = L"���";
	SetDlgItemText(IDCANCEL, str);
	str = L"���U";
	SetDlgItemText(IDHELP, str);
	
	return CPropertySheet::OnInitDialog();
}
