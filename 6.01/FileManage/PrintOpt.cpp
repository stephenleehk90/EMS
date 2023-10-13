// PrintOpt.cpp : implementation file
//

#include "stdafx.h"
//#include "EditPro.h"
#include "resource.h"
#include "PrintOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionPage property page

IMPLEMENT_DYNCREATE(CPrintOptionPage, CPropertyPage)

CPrintOptionPage::CPrintOptionPage() : CPropertyPage(CPrintOptionPage::IDD)
{
	//{{AFX_DATA_INIT(CPrintOptionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPrintOptionPage::~CPrintOptionPage()
{
}

void CPrintOptionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintOptionPage)
	DDX_Control(pDX, IDC_SPINMARGINTOP, m_spinMarginTop);
	DDX_Control(pDX, IDC_SPINMARGINRIGHT, m_spinMarginRight);
	DDX_Control(pDX, IDC_SPINMARGINLEFT, m_spinMarginLeft);
	DDX_Control(pDX, IDC_SPINMARGINBOTTOM, m_spinMarginBottom);
	DDX_Control(pDX, IDC_SPINCOLWIDTH, m_spinColWidth);
	DDX_Control(pDX, IDC_SPINCOLUMNS, m_spinColumns);
	DDX_Control(pDX, IDC_SPINCOLSPACE, m_spinColSpace);
	DDX_Control(pDX, IDC_CHECKSELECTIONONLY, m_checkSelectionOnly);
	DDX_Control(pDX, IDC_CHECKRESETSELECTIONLINENUM, m_checkResetSelectionLineNum);
	DDX_Control(pDX, IDC_CHECKPRINTLINENUM, m_checkLineNum);
	DDX_Control(pDX, IDC_SPINLINESPACE, m_spinLineSpace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintOptionPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPrintOptionPage)
	ON_BN_CLICKED(IDC_CHECKPRINTLINENUM, OnCheckprintlinenum)
	ON_BN_CLICKED(IDC_CHECKRESETSELECTIONLINENUM, OnCheckresetselectionlinenum)
	ON_BN_CLICKED(IDC_CHECKSELECTIONONLY, OnCheckselectiononly)
	ON_EN_CHANGE(IDC_EDITCOLSPACE, OnChangeEditcolspace)
	ON_EN_CHANGE(IDC_LINESPACE, OnChangeEditLinespace)
	ON_EN_CHANGE(IDC_EDITCOLUMNS, OnChangeEditcolumns)
	ON_EN_CHANGE(IDC_EDITCOLWIDTH, OnChangeEditcolwidth)
	ON_EN_CHANGE(IDC_EDITMARGINBOTTOM, OnChangeEditmarginbottom)
	ON_EN_CHANGE(IDC_EDITMARGINLEFT, OnChangeEditmarginleft)
	ON_EN_CHANGE(IDC_EDITMARGINRIGHT, OnChangeEditmarginright)
	ON_EN_CHANGE(IDC_EDITMARGINTOP, OnChangeEditmargintop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionPage message handlers

BOOL CPrintOptionPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CWinApp* pApp = AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010

	// Columns
	m_spinColumns.SetRange(1, 100);
	m_spinColumns.SetPos(GetPrivateProfileIntW( L"EditProPrint" , L"Columns",1,((CWinApp*)AfxGetApp())->m_pszProfileName));
	//m_spinColumns.SetPos(pApp->GetProfileInt(L"EditProPrint", L"Columns", 1));
	// Col Width
	m_spinColWidth.SetRange(1, 1000);
	m_spinColWidth.SetPos(GetPrivateProfileIntW( L"EditProPrint" , L"LineChars",38,((CWinApp*)AfxGetApp())->m_pszProfileName));
	//m_spinColWidth.SetPos(pApp->GetProfileInt(L"EditProPrint", L"LineChars", 38));
	// Col Space
	m_spinColSpace.SetRange(1, 1000);
	m_spinColSpace.SetPos(GetPrivateProfileIntW( L"EditProPrint" , L"ColumnDistance",10,((CWinApp*)AfxGetApp())->m_pszProfileName));
	//m_spinColSpace.SetPos(pApp->GetProfileInt(L"EditProPrint", L"ColumnDistance", 10));
	// Margins Top
	m_spinMarginTop.SetRange(1, 10000);
	m_spinMarginTop.SetPos(GetPrivateProfileIntW( L"EditProPrint" , L"TopMargin",2,((CWinApp*)AfxGetApp())->m_pszProfileName));
	//m_spinMarginTop.SetPos(pApp->GetProfileInt(L"EditProPrint", L"TopMargin", 2));
	// Margins Bottom
	m_spinMarginBottom.SetRange(1, 10000);
	m_spinMarginBottom.SetPos(GetPrivateProfileIntW( L"EditProPrint" , L"BottomMargin",2,((CWinApp*)AfxGetApp())->m_pszProfileName));
	//m_spinMarginBottom.SetPos(pApp->GetProfileInt(L"EditProPrint", L"BottomMargin", 2));
	// Margins Left
	m_spinMarginLeft.SetRange(1, 10000);
	m_spinMarginLeft.SetPos(GetPrivateProfileIntW( L"EditProPrint" , L"LeftMargin",2,((CWinApp*)AfxGetApp())->m_pszProfileName));
	//m_spinMarginLeft.SetPos(pApp->GetProfileInt(L"EditProPrint", L"LeftMargin", 2));
	// Margins Right
	m_spinMarginRight.SetRange(1, 10000);
	m_spinMarginRight.SetPos(GetPrivateProfileIntW( L"EditProPrint" , L"RightMargin",2,((CWinApp*)AfxGetApp())->m_pszProfileName));
	//m_spinMarginRight.SetPos(pApp->GetProfileInt(L"EditProPrint", L"RightMargin", 2));
	// Selection Only
	m_checkSelectionOnly.SetCheck(GetPrivateProfileIntW( L"EditProPrint" , L"PrintSelection",0,((CWinApp*)AfxGetApp())->m_pszProfileName));
	//m_checkSelectionOnly.SetCheck(pApp->GetProfileInt(L"EditProPrint", L"PrintSelection", 0));
	// Line Num
	m_checkLineNum.SetCheck(GetPrivateProfileIntW( L"EditProPrint" , L"PrintLineNum",0,((CWinApp*)AfxGetApp())->m_pszProfileName));
	//m_checkLineNum.SetCheck(pApp->GetProfileInt(L"EditProPrint", L"PrintLineNum", 0));
	m_checkResetSelectionLineNum.SetCheck(GetPrivateProfileIntW( L"EditProPrint" , L"NewSelectionLineNum",0,((CWinApp*)AfxGetApp())->m_pszProfileName));
	//m_checkResetSelectionLineNum.SetCheck(pApp->GetProfileInt(L"EditProPrint", L"NewSelectionLineNum", 0));
	//end 20120927

	// Col Space
	m_spinLineSpace.SetRange(0, 500);
	m_spinLineSpace.SetPos(GetPrivateProfileIntW( L"EditProPrint" , L"LineSpacing",20,((CWinApp*)AfxGetApp())->m_pszProfileName));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPrintOptionPage::OnCheckprintlinenum() 
{
	SetModified();
}

void CPrintOptionPage::OnCheckresetselectionlinenum() 
{
	SetModified();
}

void CPrintOptionPage::OnCheckselectiononly() 
{
	SetModified();
}

void CPrintOptionPage::OnChangeEditcolspace() 
{
	SetModified();
}

void CPrintOptionPage::OnChangeEditLinespace() 
{
	SetModified();
}

void CPrintOptionPage::OnChangeEditcolumns() 
{
	SetModified();
}

void CPrintOptionPage::OnChangeEditcolwidth() 
{
	SetModified();
}

void CPrintOptionPage::OnChangeEditmarginbottom() 
{
	SetModified();
}

void CPrintOptionPage::OnChangeEditmarginleft() 
{
	SetModified();
}

void CPrintOptionPage::OnChangeEditmarginright() 
{
	SetModified();
}

void CPrintOptionPage::OnChangeEditmargintop() 
{
	SetModified();
}


BOOL CPrintOptionPage::OnApply() 
{
	CWinApp* pApp = AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString str_temp;
	// Columns
	str_temp.Format(L"%d",m_spinColumns.GetPos());
	WritePrivateProfileStringW(L"EditProPrint", L"Columns" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"Columns", m_spinColumns.GetPos());
	// Col Width
	str_temp.Format(L"%d",m_spinColWidth.GetPos());
	WritePrivateProfileStringW(L"EditProPrint", L"LineChars" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"LineChars", m_spinColWidth.GetPos());
	// Col Space
	str_temp.Format(L"%d",m_spinColSpace.GetPos());
	WritePrivateProfileStringW(L"EditProPrint", L"ColumnDistance" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"ColumnDistance", m_spinColSpace.GetPos());
	// Margins Top
	str_temp.Format(L"%d",m_spinMarginTop.GetPos());
	WritePrivateProfileStringW(L"EditProPrint", L"TopMargin" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"TopMargin", m_spinMarginTop.GetPos());
	// Margins Bottom
	str_temp.Format(L"%d",m_spinMarginBottom.GetPos());
	WritePrivateProfileStringW(L"EditProPrint", L"BottomMargin" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"BottomMargin", m_spinMarginBottom.GetPos());
	// Margins Left
	str_temp.Format(L"%d",m_spinMarginLeft.GetPos());
	WritePrivateProfileStringW(L"EditProPrint", L"LeftMargin" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"LeftMargin", m_spinMarginLeft.GetPos());
	// Margins Right
	str_temp.Format(L"%d",m_spinMarginRight.GetPos());
	WritePrivateProfileStringW(L"EditProPrint", L"RightMargin" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"RightMargin", m_spinMarginRight.GetPos());
	// Selection Only
	str_temp.Format(L"%d", m_checkSelectionOnly.GetCheck());
	WritePrivateProfileStringW(L"EditProPrint", L"PrintSelection" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"PrintSelection", m_checkSelectionOnly.GetCheck());
	// Line Num
	str_temp.Format(L"%d",m_checkLineNum.GetCheck());
	WritePrivateProfileStringW(L"EditProPrint", L"PrintLineNum" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"PrintLineNum", m_checkLineNum.GetCheck());
	str_temp.Format(L"%d",m_checkResetSelectionLineNum.GetCheck());
	WritePrivateProfileStringW(L"EditProPrint", L"NewSelectionLineNum" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"NewSelectionLineNum", m_checkResetSelectionLineNum.GetCheck());
	//end 20120927
	// Line Space
	str_temp.Format(L"%d",m_spinLineSpace.GetPos());
	WritePrivateProfileStringW(L"EditProPrint", L"LineSpacing" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	return CPropertyPage::OnApply();
}
