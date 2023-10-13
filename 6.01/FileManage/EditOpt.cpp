// EditPage.cpp : implementation file
//

#include "stdafx.h"
//#include "EditPro.h"
#include "resource.h"
#include "EditOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditOptionPage property page

IMPLEMENT_DYNCREATE(CEditOptionPage, CPropertyPage)

CEditOptionPage::CEditOptionPage() : CPropertyPage(CEditOptionPage::IDD)
{
	//{{AFX_DATA_INIT(CEditOptionPage)
	//}}AFX_DATA_INIT
}

CEditOptionPage::~CEditOptionPage()
{
}

void CEditOptionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditOptionPage)
	DDX_Control(pDX, IDC_SPINAUTOSPACES, m_spinAutoSpaces);
	DDX_Control(pDX, IDC_SPINAUTOSAVETIME, m_spinAutoSaveTime);
	DDX_Control(pDX, IDC_CHECKAUTOSAVE, m_checkAutoSave);
	DDX_Control(pDX, IDC_CHECKAUTOSPACEAFTERRETURN, m_checkAutoSpaceAfterReturn);
	DDX_Control(pDX, IDC_CHECKLINEEDIT, m_checkLineEdit);
	DDX_Control(pDX, IDC_SPINUNDOBUFSIZE, m_spinUndoBufSize);
	DDX_Control(pDX, IDC_SPINLINEWIDTH, m_spinLineWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditOptionPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEditOptionPage)
	ON_BN_CLICKED(IDC_CHECKLINEEDIT, OnChecklineedit)
	ON_BN_CLICKED(IDC_CHECKAUTOSPACEAFTERRETURN, OnCheckautospaceafterreturn)
	ON_EN_CHANGE(IDC_EDITLINEWIDTH, OnChangeEditlinewidth)
	ON_EN_CHANGE(IDC_EDITUNDOBUFSIZE, OnChangeEditundobufsize)
	ON_EN_CHANGE(IDC_EDITAUTOSAVETIME, OnChangeEditautosavetime)
	ON_EN_CHANGE(IDC_EDITAUTOSPACES, OnChangeEditautospaces)
	ON_BN_CLICKED(IDC_CHECKAUTOSAVE, OnCheckautosave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditOptionPage message handlers

void CEditOptionPage::OnChecklineedit() 
{
	if (m_checkLineEdit.GetCheck())	// line edit mode
	{
		m_spinLineWidth.EnableWindow(TRUE);
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		int nData = GetPrivateProfileIntW( L"EditPro" , L"LineChars",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//int nData = AfxGetApp()->GetProfileInt(L"EditPro", L"LineChars", 0);
		//end 20120927
		if (nData == 0)
			nData = 20;	// 20 Chinese char
		m_spinLineWidth.SetPos(nData);
	}
	else
	{
		m_spinLineWidth.EnableWindow(FALSE);
	}
	SetModified();
}


BOOL CEditOptionPage::OnApply() 
{
	CWinApp* pApp = AfxGetApp();
	CString str_temp;
	if (m_checkAutoSpaceAfterReturn.GetCheck())
	{
		int lower, upper;
		m_spinAutoSpaces.GetRange(lower,upper); 
		if ((m_spinAutoSpaces.GetPos()<lower) || (m_spinAutoSpaces.GetPos()>upper))
		{
			str_temp.Format(L"換段時自動增加段首空格數應該在 %d 到 %d 之間",lower,upper);
			AfxMessageBox(str_temp);
			return FALSE;
		}
	}
	if (m_checkLineEdit.GetCheck())	// line edit mode
	{
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		str_temp.Format(L"%d",m_spinLineWidth.GetPos());
		WritePrivateProfileStringW(L"EditPro", L"LineChars", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(L"EditPro", L"LineChars", m_spinLineWidth.GetPos());
		//end 20120927
	}
	else
	//edit by Ronald 20120927 for migrating from vc6 to 2010
		WritePrivateProfileStringW(L"EditPro", L"LineChars",L"0", ((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(L"EditPro", L"LineChars", 0);
	
	//CString str_temp;
	str_temp.Format(L"%d",m_checkAutoSpaceAfterReturn.GetCheck());
	WritePrivateProfileStringW(L"EditPro", L"AutoSpaceAfterReturn",str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",m_spinAutoSpaces.GetPos());
	WritePrivateProfileStringW(L"EditPro", L"AutoSpacesAfterReturn",str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",m_spinUndoBufSize.GetPos());
	WritePrivateProfileStringW(L"EditPro", L"UndoBufSize",str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d", m_checkAutoSave.GetCheck());
	WritePrivateProfileStringW(L"EditPro", L"AutoSave",str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d", m_spinAutoSaveTime.GetPos());
	WritePrivateProfileStringW(L"EditPro", L"AutoSaveTime",str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	/*pApp->WriteProfileInt(L"EditPro", L"AutoSpaceAfterReturn", m_checkAutoSpaceAfterReturn.GetCheck());
	pApp->WriteProfileInt(L"EditPro", L"AutoSpacesAfterReturn", m_spinAutoSpaces.GetPos());
	pApp->WriteProfileInt(L"EditPro", L"UndoBufSize", m_spinUndoBufSize.GetPos());
	pApp->WriteProfileInt(L"EditPro", L"AutoSave", m_checkAutoSave.GetCheck());
	pApp->WriteProfileInt(L"EditPro", L"AutoSaveTime", m_spinAutoSaveTime.GetPos());*/
	//end 20120927
	// Update All MDI Child
	CMDIChildWnd* pChildWnd, *pActiveChildWnd;
	CMDIFrameWnd* pFrameWnd= (CMDIFrameWnd*)AfxGetMainWnd();

	pActiveChildWnd = pFrameWnd->MDIGetActive();
	pChildWnd = pActiveChildWnd;
	while (pChildWnd)
	{
		pChildWnd->SendMessage(WM_COMMAND, ID_OPTION_CHANGE, 0L);
		pFrameWnd->MDINext();
		pChildWnd = pFrameWnd->MDIGetActive();
		if (pChildWnd == pActiveChildWnd)
			break;
	}

	return CPropertyPage::OnApply();
}


BOOL CEditOptionPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CWinApp* pApp = AfxGetApp();
	
	// Line Width
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	int nData = GetPrivateProfileIntW( L"EditPro" , L"LineChars",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//int nData = pApp->GetProfileInt(L"EditPro", L"LineChars", 0);
	//end 20120927
	
	m_spinLineWidth.SetRange(0, 200);
	if (nData == 0) // not LineEdit mode
	{
		m_spinLineWidth.EnableWindow(FALSE);
	}
	else
	{
		m_checkLineEdit.SetCheck(TRUE);
		m_spinLineWidth.SetPos(nData);
	}

	// Auto Space after Return
	m_spinAutoSpaces.SetRange(1, 20);
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	if (GetPrivateProfileIntW( L"EditPro" , L"AutoSpaceAfterReturn",0,((CWinApp*)AfxGetApp())->m_pszProfileName)) 
	//if (pApp->GetProfileInt(L"EditPro", L"AutoSpaceAfterReturn", 0))
	{
		m_checkAutoSpaceAfterReturn.SetCheck(TRUE);
		int iAutoSpaceAfterReturn =  GetPrivateProfileIntW( L"EditPro" , L"AutoSpacesAfterReturn",4,((CWinApp*)AfxGetApp())->m_pszProfileName);
		m_spinAutoSpaces.SetPos(iAutoSpaceAfterReturn);
		//m_spinAutoSpaces.SetPos(pApp->GetProfileInt(L"EditPro", L"AutoSpacesAfterReturn", 4));
	}
	else
		m_spinAutoSpaces.EnableWindow(FALSE);
	
	// Undo Buffer Size
	nData = GetPrivateProfileIntW( L"EditPro" , L"UndoBufSize",4,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//nData = pApp->GetProfileInt(L"EditPro", L"UndoBufSize", 4);
	m_spinUndoBufSize.SetPos(4);
	m_spinUndoBufSize.SetRange(2, 500);
	m_spinUndoBufSize.SetPos(nData);

	// Auto Save
	m_spinAutoSaveTime.SetRange(1, 180);
	if (GetPrivateProfileIntW( L"EditPro" , L"AutoSave",1,((CWinApp*)AfxGetApp())->m_pszProfileName)) 
	//if (pApp->GetProfileInt(L"EditPro", L"AutoSave", 1))
	{
		m_checkAutoSave.SetCheck(TRUE);
		m_spinAutoSaveTime.SetPos(GetPrivateProfileIntW( L"EditPro" , L"AutoSaveTime",2,((CWinApp*)AfxGetApp())->m_pszProfileName));
		//m_spinAutoSaveTime.SetPos(pApp->GetProfileInt(L"EditPro", L"AutoSaveTime", 2));
	}
	else
		m_spinAutoSaveTime.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	//end 20120927
}

void CEditOptionPage::OnCheckautospaceafterreturn() 
{
	if (m_checkAutoSpaceAfterReturn.GetCheck())	// Auto Space
	{
		m_spinAutoSpaces.EnableWindow(TRUE);
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		m_spinAutoSpaces.SetPos(GetPrivateProfileIntW( L"EditPro" , L"AutoSpacesAfterReturn",4,((CWinApp*)AfxGetApp())->m_pszProfileName));
		//m_spinAutoSpaces.SetPos(AfxGetApp()->GetProfileInt(L"EditPro", L"AutoSpacesAfterReturn", 4));
		//end 20120927
	}
	else
	{
		m_spinAutoSpaces.EnableWindow(FALSE);
	}
	SetModified();
}

void CEditOptionPage::OnChangeEditlinewidth() 
{
	SetModified();
}

void CEditOptionPage::OnChangeEditundobufsize() 
{
	SetModified();
}

void CEditOptionPage::OnChangeEditautosavetime() 
{
	SetModified();
}

void CEditOptionPage::OnChangeEditautospaces() 
{
	SetModified();
}

void CEditOptionPage::OnCheckautosave() 
{
	if (m_checkAutoSave.GetCheck())	// Auto Save
	{
		m_spinAutoSaveTime.EnableWindow(TRUE);
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		m_spinAutoSaveTime.SetPos(GetPrivateProfileIntW( L"EditPro" , L"AutoSaveTime",2,((CWinApp*)AfxGetApp())->m_pszProfileName));
		//m_spinAutoSaveTime.SetPos(AfxGetApp()->GetProfileInt(L"EditPro", L"AutoSaveTime", 2));
		//end 20120927
	}
	else
	{
		m_spinAutoSaveTime.EnableWindow(FALSE);
	}
	SetModified();
}
