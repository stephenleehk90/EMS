// ShowMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileManage.h"
#include "ShowMsgDlg.h"
#include "afxdialogex.h"


// CShowMsgDlg dialog

IMPLEMENT_DYNAMIC(CShowMsgDlg, CDialog)

CShowMsgDlg::CShowMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowMsgDlg::IDD, pParent)
{

}

CShowMsgDlg::~CShowMsgDlg()
{
}

void CShowMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	 CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowMsgDlg, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

HBRUSH CShowMsgDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
    switch (nCtlColor)
    {
    case CTLCOLOR_STATIC:
        pDC->SetTextColor(RGB(255, 0, 0));
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    default:
        return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}

// CShowMsgDlg message handlers


BOOL CShowMsgDlg::OnInitDialog()
{
	int fontHeight = 20;
	CFont *font = new CFont();
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = fontHeight;
	_tcscpy(lf.lfFaceName, _T( "Arial" ));
	font->CreateFontIndirect(&lf);
	GetDlgItem(IDC_STATIC_MSG1)->SetFont(font);
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
