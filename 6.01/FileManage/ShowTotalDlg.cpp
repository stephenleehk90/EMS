// ShowTotalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "ShowTotalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowTotalDlg dialog


CShowTotalDlg::CShowTotalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowTotalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowTotalDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CShowTotalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowTotalDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowTotalDlg, CDialog)
	//{{AFX_MSG_MAP(CShowTotalDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowTotalDlg message handlers

void CShowTotalDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CShowTotalDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CDialog::OnCancel();
}
