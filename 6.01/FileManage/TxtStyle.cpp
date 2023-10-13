// TxtStyle.cpp : implementation file
//

#include "stdafx.h"
//#include "EditPro.h"
#include "resource.h"
#include "TxtStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextStylesPage property page

IMPLEMENT_DYNCREATE(CTextStylesPage, CPropertyPage)

CTextStylesPage::CTextStylesPage() : CPropertyPage(CTextStylesPage::IDD)
{
	//{{AFX_DATA_INIT(CTextStylesPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nStyleNum = 0;
	m_nCurStyle = 0;
	m_pnLineSpace = m_pnCharSpace = NULL;
	m_pnLineSpaceP = m_pnCharSpaceP = NULL;//add by cy 
	m_parrayStyleName = NULL;
	m_plfFont = NULL;
	m_plfFontP = NULL;//add by cy
	m_bSysChange = FALSE;
}

CTextStylesPage::~CTextStylesPage()
{
	if (m_pnLineSpace)
		delete m_pnLineSpace;
	if (m_pnCharSpace)
		delete m_pnCharSpace;
	if (m_parrayStyleName)
		delete m_parrayStyleName;
	if (m_plfFont)
		delete m_plfFont;
	//===========add by cy 1224=========//
	if (m_pnLineSpaceP)
		delete m_pnLineSpaceP;
	if (m_pnCharSpaceP)
		delete m_pnCharSpaceP;
	if (m_plfFontP)
		delete m_plfFontP;
	//===========End============//
}

void CTextStylesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextStylesPage)
//	DDX_Control(pDX, IDC_CHECKBOLD, m_BoldCheckBox);
//	DDX_Control(pDX, IDC_LISTSTYLES, m_StyleList);
	DDX_Control(pDX, IDC_SPINAUTOSAVETIME, m_spinAutoSaveTime);
	DDX_Control(pDX, IDC_CHECKAUTOSAVE, m_checkAutoSave);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextStylesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTextStylesPage)
	ON_LBN_SELCHANGE(IDC_LISTSTYLES, OnSelchangeListstyles)
	ON_EN_CHANGE(IDC_EDITCHARSPACE, OnChangeEditcharspace)
	ON_EN_CHANGE(IDC_EDITLINESPACE, OnChangeEditlinespace)
	ON_EN_CHANGE(IDC_EDITAUTOSAVETIME, OnChangeEditautosavetime)
	ON_BN_CLICKED(IDC_BTTNSETFONT, OnBttnsetfont)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIODISP, OnDisplay)
	ON_BN_CLICKED(IDC_RADIOPRINT, OnRadioPrint)
	ON_BN_CLICKED(IDC_CHECKSAME, OnCheckSame)
	ON_BN_CLICKED(IDC_CHECKAUTOSAVE, OnCheckautosave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextStylesPage message handlers

BOOL CTextStylesPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	// Alloc Memory
	CWinApp* pApp = AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	m_nStyleNum = GetPrivateProfileIntW( L"EditPro" , L"TextStyleNum",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//m_nStyleNum = pApp->GetProfileInt(L"EditPro", L"TextStyleNum", 1);
	//end 20120927
	
	//===========add by cy 1224===========//
 	((CButton *)GetDlgItem(IDC_RADIODISP))->SetCheck(1);
// 	GetDlgItem(IDC_CHECKSAME)->EnableWindow(false);
	//===========End=============//
	// Get text styles from profile
	CClientDC dc(this);
	int nLogicalPixelX, nLogicalPixelY;
	int nDCLogicalPixelX, nDCLogicalPixelY;

	//edit by Ronald 20120927 for migrating from vc6 to 2010
	nLogicalPixelX = GetPrivateProfileIntW( L"EditPro" , L"LogicalPixelX",96,((CWinApp*)AfxGetApp())->m_pszProfileName);
	nLogicalPixelY = GetPrivateProfileIntW( L"EditPro" , L"LogicalPixelY",96,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//nLogicalPixelX = pApp->GetProfileInt(L"EditPro", L"LogicalPixelX", 96);
	//nLogicalPixelY = pApp->GetProfileInt(L"EditPro", L"LogicalPixelY", 96);
	//end 20120927
	nDCLogicalPixelX = dc.GetDeviceCaps(LOGPIXELSX);
	nDCLogicalPixelY = dc.GetDeviceCaps(LOGPIXELSY);
	InitDispInfo();
	InitPrintInfo();
//	m_StyleList.SetCurSel(m_nCurStyle);

	
	// Auto Save
	m_spinAutoSaveTime.SetRange(1, 180);
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	if (GetPrivateProfileIntW( L"EditPro" , L"AutoSave",1,((CWinApp*)AfxGetApp())->m_pszProfileName))
	//if (pApp->GetProfileInt(L"EditPro", L"AutoSave", 1))
	//end 20120927
	{
		m_checkAutoSave.SetCheck(TRUE);
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		m_spinAutoSaveTime.SetPos(GetPrivateProfileIntW( L"EditPro" , L"AutoSaveTime",2,((CWinApp*)AfxGetApp())->m_pszProfileName));
		//m_spinAutoSaveTime.SetPos(pApp->GetProfileInt(L"EditPro", L"AutoSaveTime", 2));
		//end 20120927
	}
	else
		m_spinAutoSaveTime.EnableWindow(FALSE);
	
	UpdateWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextStylesPage::OnSelchangeListstyles() 
{
	CString str;
	
	// add by Bob
	// 原來的版本中計算logfont的lfHeight錯誤。
	// logfont中的lfHeight應該一直保持自己的值﹐顯示的時候才根據下面的
	// 方式換算到具體的大小。
	// 原來的版本有兩個錯誤
	// 1. 初試化的時候﹐ini中保存的是lfHeight﹐不應該換算﹐應該保持該值.
	// 2. 在選擇了font之后﹐直接將lfHeight顯示出來﹐當然不對﹐應該先換算成字體大小。
	// 3. 換算公式錯誤。
	CWinApp* pApp = AfxGetApp();
	CClientDC dc(this);
	int nDCLogicalPixelX, nDCLogicalPixelY;
	
	nDCLogicalPixelX = dc.GetDeviceCaps(LOGPIXELSX);
	nDCLogicalPixelY = dc.GetDeviceCaps(LOGPIXELSY);

	m_bSysChange = TRUE;
	CButton *pDisplay=(CButton *)(GetDlgItem(IDC_RADIODISP));
	SetDlgItemText(IDC_EDITFONTNAME, m_plfFont[m_nCurStyle].lfFaceName);
	int abc;
	abc = abs(MulDiv(m_plfFont[m_nCurStyle].lfHeight,72,nDCLogicalPixelY));
	str.Format(L"%ld",abc);
	// end add by Bob
//	str.Format("%ld", abs(m_plfFont[m_nCurStyle].lfHeight));
	SetDlgItemText(IDC_EDITFONTSIZE, str);
// 	if (m_plfFont[m_nCurStyle].lfWeight == 700) // Bold
// 		m_BoldCheckBox.SetCheck(TRUE);
// 	else
// 		m_BoldCheckBox.SetCheck(FALSE);
//	str.Format(L"%d", m_pnLineSpace[m_nCurStyle]);

	if(pDisplay->GetCheck())
	{
	SetDlgItemText(IDC_EDITFONTNAME, m_plfFont[m_nCurStyle].lfFaceName);
	int abc;
	abc = abs(MulDiv(m_plfFont[m_nCurStyle].lfHeight,72,nDCLogicalPixelY));
	str.Format(L"%ld",abc);
	// end add by Bob
	//str.Format("%ld", abs(m_plfFont[m_nCurStyle].lfHeight));
	SetDlgItemText(IDC_EDITFONTSIZE, str);
	str.Format(L"%d", m_pnLineSpace[m_nCurStyle]);
	SetDlgItemText(IDC_EDITLINESPACE, str);
	str.Format(L"%d", m_pnCharSpace[m_nCurStyle]);
	SetDlgItemText(IDC_EDITCHARSPACE, str);
	//if (m_plfFont[m_nCurStyle].lfWeight == 700) // Bold
	//	m_BoldCheckBox.SetCheck(TRUE);
	//else
	//	m_BoldCheckBox.SetCheck(FALSE);
	}
	else
	{
		SetDlgItemText(IDC_EDITFONTNAME, m_plfFontP[m_nCurStyle].lfFaceName);
		int abc;
		abc = abs(MulDiv(m_plfFontP[m_nCurStyle].lfHeight,72,nDCLogicalPixelY));
		str.Format(L"%ld",abc);
		// end add by Bob
		//str.Format("%ld", abs(m_plfFont[m_nCurStyle].lfHeight));
		SetDlgItemText(IDC_EDITFONTSIZE, str);
		str.Format(L"%d", m_pnLineSpaceP[m_nCurStyle]);
		SetDlgItemText(IDC_EDITLINESPACE, str);
		str.Format(L"%d", m_pnCharSpaceP[m_nCurStyle]);
		SetDlgItemText(IDC_EDITCHARSPACE, str);
		//if (m_plfFontP[m_nCurStyle].lfWeight == 700) // Bold
		//	m_BoldCheckBox.SetCheck(TRUE);
		//else
		//	m_BoldCheckBox.SetCheck(FALSE);
	}
	m_bSysChange = FALSE;

	//StylePreview();
}

void CTextStylesPage::OnChangeEditcharspace() 
{
	if (m_bSysChange)
		return;
	CString str;
	GetDlgItemText(IDC_EDITCHARSPACE, str);
	if (str.GetLength() > 0)
	{
		CButton *pDisplay=(CButton *)(GetDlgItem(IDC_RADIODISP));
		if(pDisplay->GetCheck())
		{
//			const wchar_t * wc_tmp = (LPCTSTR) str;
			m_pnCharSpace[m_nCurStyle] = _wtoi(str);
		}
		else
		{
//			const wchar_t * wc_tmp = (LPCTSTR) str;
			m_pnCharSpaceP[m_nCurStyle] = _wtoi(str);
		}
		SetModified();
		StylePreview();
	}
}

void CTextStylesPage::OnChangeEditlinespace() 
{
	if (m_bSysChange)
		return;
	CString str;
	GetDlgItemText(IDC_EDITLINESPACE, str);
	if (str.GetLength() > 0)
	{
		CButton *pDisplay=(CButton *)(GetDlgItem(IDC_RADIODISP));
		if(pDisplay->GetCheck())
		{
//			const wchar_t * wc_tmp = (LPCTSTR) str;
//			m_pnLineSpace[m_nCurStyle] = atoi((const char *)wc_tmp);
			m_pnLineSpace[m_nCurStyle] = _wtoi(str);
		}
		else
		{
//			const wchar_t * wc_tmp = (LPCTSTR) str;
//			m_pnLineSpaceP[m_nCurStyle] = atoi((const char *)wc_tmp);
			m_pnLineSpaceP[m_nCurStyle] = _wtoi(str);
		}
		SetModified();
		StylePreview();
	}
}

void CTextStylesPage::OnBttnsetfont() 
{
	// TODO: Add your control notification handler code here
	LOGFONT logfont;

	CButton *pDisplay=(CButton *)(GetDlgItem(IDC_RADIODISP));
 	if(pDisplay->GetCheck()) 		
	memcpy(&logfont, &m_plfFont[m_nCurStyle], sizeof(LOGFONT));
 	else
 		memcpy(&logfont, &m_plfFontP[m_nCurStyle], sizeof(LOGFONT));
	
	//memcpy(&logfont, &m_plfFont[m_nCurStyle], sizeof(LOGFONT));
	CFontDialog dlg(&logfont, CF_USESTYLE | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT);
	//dlg.m_cf.rgbColors   =   RGB(0,0,0); 

	if (dlg.DoModal() == IDOK)
	{
		if(pDisplay->GetCheck())
			memcpy(&m_plfFont[m_nCurStyle], &logfont, sizeof(LOGFONT));
		else
			memcpy(&m_plfFontP[m_nCurStyle], &logfont, sizeof(LOGFONT));
		SetModified();
		OnSelchangeListstyles();
	}
}

void CTextStylesPage::StylePreview()
{
	CWnd* pwndItem = GetDlgItem(IDC_FONTPREVIEW);
	CClientDC dc(pwndItem);
	CFont	font, *pfontSave;
	CButton *pDisplay=(CButton *)(GetDlgItem(IDC_RADIODISP));
// 	if(pDisplay->GetCheck())
 		font.CreateFontIndirect(&m_plfFont[m_nCurStyle]);
// 	else
// 		font.CreateFontIndirect(&m_plfFontP[m_nCurStyle]);
	pfontSave = dc.SelectObject(&font);

	CString str;
	int *pnWidths, i, nLen;

	str = L"方正 Founder";
	str += "(" + m_parrayStyleName->GetAt(m_nCurStyle) + ")";
	//原來的版本中﹐計算中文字符寬度有問題。
	//如果字體是英文語系的話﹐則用
	
	
	//計算沒有問題﹐但是如果字體是中文語系的話﹐則錯誤。
	//所以調用int nChinese= dc.GetTextExtent("方").cx;來計算。
//9	int nChinese= dc.GetTextExtent(L"方").cx;
	nLen = str.GetLength();
	pnWidths = new int[nLen];
	for (i=0; i < nLen; i ++)
		dc.GetCharWidth(str.GetAt(i), str.GetAt(i), &pnWidths[i]);
/*		
		// add by Bob
		// 中文字符計算
//		if ((BYTE)str.GetAt(i)>(BYTE)ASCII_END)
//		{
			CButton *pDisplay=(CButton *)(GetDlgItem(IDC_RADIODISP));
			if(pDisplay->GetCheck())
				pnWidths[i]=nChinese+m_pnCharSpace[m_nCurStyle];
			else
				pnWidths[i]=nChinese+m_pnCharSpaceP[m_nCurStyle];
	}
*/
// 	RECT rc;
// 	pwndItem->GetClientRect(&rc);
// 	pwndItem->InvalidateRect(NULL);
// 	pwndItem->UpdateWindow();
// 	dc.SetBkMode(TRANSPARENT);
// 	//CButton *pDisplay=(CButton *)(GetDlgItem(IDC_RADIODISP));
// 	if(pDisplay->GetCheck())
 //		dc.ExtTextOut(m_pnCharSpace[m_nCurStyle], m_pnLineSpace[m_nCurStyle], 
 //		ETO_CLIPPED, &rc, str, pnWidths);
// 	else
// 		dc.ExtTextOut(m_pnCharSpaceP[m_nCurStyle], m_pnLineSpaceP[m_nCurStyle], 
// 		ETO_CLIPPED, &rc, str, pnWidths);
 	delete []pnWidths;
 	dc.SelectObject(pfontSave);
	font.DeleteObject();
}

BOOL CTextStylesPage::OnApply() 
{
	// Save Text Styles Setting
	int nStyle;
	CWinApp* pApp = AfxGetApp();
	CString str;
//	char	szSection[TMPSTR_SIZE];

	CString	szSection;

	CClientDC dc(this);

	// text styles for display
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString str_temp;
	str_temp.Format(L"%d",dc.GetDeviceCaps(LOGPIXELSX));
	WritePrivateProfileStringW(L"EditPro", L"LogicalPixelX",str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",dc.GetDeviceCaps(LOGPIXELSY));
	WritePrivateProfileStringW(L"EditPro", L"LogicalPixelY",str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",m_checkAutoSave.GetCheck());
	WritePrivateProfileStringW(L"EditPro", L"AutoSave",str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",m_spinAutoSaveTime.GetPos());
	WritePrivateProfileStringW(L"EditPro", L"AutoSaveTime",str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	/*pApp->WriteProfileInt(L"EditPro", L"LogicalPixelX", dc.GetDeviceCaps(LOGPIXELSX));
	pApp->WriteProfileInt(L"EditPro", L"LogicalPixelY", dc.GetDeviceCaps(LOGPIXELSY));
	pApp->WriteProfileInt(L"EditPro", L"AutoSave", m_checkAutoSave.GetCheck());
	pApp->WriteProfileInt(L"EditPro", L"AutoSaveTime", m_spinAutoSaveTime.GetPos());*/
	//end 20120927
	for(nStyle=0; nStyle < m_nStyleNum; nStyle ++)
	{
//		sprintf(szSection, "EditProTextStyle%d", nStyle+1);
		szSection.Format(L"EditProTextStyle%ld", nStyle+1);

		//edit by Ronald 20120927 for migrating from vc6 to 2010
		CString str_temp;
		str_temp.Format(L"%d",m_pnLineSpace[nStyle]);
		WritePrivateProfileStringW(szSection, L"LineSpace",str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp.Format(L"%d",m_pnCharSpace[nStyle]);
		WritePrivateProfileStringW(szSection, L"CharSpace",str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"LineSpace", m_pnLineSpace[nStyle]);
		//pApp->WriteProfileInt(szSection, L"CharSpace", m_pnCharSpace[nStyle]);
		
		str.Format(L"%d", m_plfFont[nStyle].lfHeight);
		WritePrivateProfileStringW(szSection, L"Height",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Height", str);
		str.Format(L"%d", m_plfFont[nStyle].lfWidth);
		WritePrivateProfileStringW(szSection, L"Width",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Width", str);

		str.Format(L"%d", m_plfFont[nStyle].lfEscapement);
		WritePrivateProfileStringW(szSection, L"Escapement",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Escapement", str);
		str.Format(L"%d", m_plfFont[nStyle].lfOrientation);
		WritePrivateProfileStringW(szSection, L"Orientation",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Orientation", str);
		str.Format(L"%d", m_plfFont[nStyle].lfWeight);
		WritePrivateProfileStringW(szSection, L"Weight",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Weight", str);

		str.Format(L"%d", m_plfFont[nStyle].lfItalic);
		WritePrivateProfileStringW(szSection, L"Italic",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"Italic", m_plfFont[nStyle].lfItalic);
		str.Format(L"%d", m_plfFont[nStyle].lfUnderline);
		WritePrivateProfileStringW(szSection, L"Underline",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"Underline", m_plfFont[nStyle].lfUnderline);
		str.Format(L"%d", m_plfFont[nStyle].lfStrikeOut);
		WritePrivateProfileStringW(szSection, L"StrikeOut",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"StrikeOut", m_plfFont[nStyle].lfStrikeOut);

		str.Format(L"%d", m_plfFont[nStyle].lfCharSet);
		WritePrivateProfileStringW(szSection, L"CharSet",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"CharSet", m_plfFont[nStyle].lfCharSet);
		str.Format(L"%d", m_plfFont[nStyle].lfOutPrecision);
		WritePrivateProfileStringW(szSection, L"OutPrecision",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"OutPrecision", m_plfFont[nStyle].lfOutPrecision);
		str.Format(L"%d", m_plfFont[nStyle].lfClipPrecision);
		WritePrivateProfileStringW(szSection, L"ClipPrecision",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"ClipPrecision", m_plfFont[nStyle].lfClipPrecision);

		str.Format(L"%d", m_plfFont[nStyle].lfPitchAndFamily);
		WritePrivateProfileStringW(szSection, L"PitchAndFamily",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"PitchAndFamily", m_plfFont[nStyle].lfPitchAndFamily);
		str.Format(L"%d",  m_plfFont[nStyle].lfQuality);
		WritePrivateProfileStringW(szSection, L"Quality",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"Quality", m_plfFont[nStyle].lfQuality);
		//str.Format(L"%d", m_plfFont[nStyle].lfFaceName);
		WritePrivateProfileStringW(szSection, L"FaceName",m_plfFont[nStyle].lfFaceName,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"FaceName", m_plfFont[nStyle].lfFaceName);
		//end 20120927
	}

	// text styles for print
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	str_temp.Format(L"%d",dc.GetDeviceCaps(LOGPIXELSX));
	WritePrivateProfileStringW(L"EditProPrint", L"LogicalPixelX",str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",dc.GetDeviceCaps(LOGPIXELSY));
	WritePrivateProfileStringW(L"EditProPrint", L"LogicalPixelY",str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"LogicalPixelX", dc.GetDeviceCaps(LOGPIXELSX));
	//pApp->WriteProfileInt(L"EditProPrint", L"LogicalPixelY", dc.GetDeviceCaps(LOGPIXELSY));
	//end 20120927
	for(nStyle=0; nStyle < m_nStyleNum; nStyle ++)
	{
//		sprintf((char *)szSection, (const char *)"EditProPrintTextStyle%d", nStyle+1);
		szSection.Format(L"EditProPrintTextStyle%ld", nStyle+1);

		//edit by Ronald 20120927 for migrating from vc6 to 2010
		str_temp.Format(L"%d",m_pnLineSpaceP[nStyle]);
		WritePrivateProfileStringW(szSection, L"LineSpace",str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp.Format(L"%d",m_pnCharSpaceP[nStyle]);
		WritePrivateProfileStringW(szSection, L"CharSpace",str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"LineSpace", m_pnLineSpaceP[nStyle]);
		//pApp->WriteProfileInt(szSection, L"CharSpace", m_pnCharSpaceP[nStyle]);

		str.Format(L"%d", m_plfFontP[nStyle].lfHeight);
		WritePrivateProfileStringW(szSection, L"Height",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Height", str);
		str.Format(L"%d", m_plfFontP[nStyle].lfWidth);
		WritePrivateProfileStringW(szSection, L"Width",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Width", str);

		str.Format(L"%d", m_plfFontP[nStyle].lfEscapement);
		WritePrivateProfileStringW(szSection, L"Escapement",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Escapement", str);
		str.Format(L"%d", m_plfFontP[nStyle].lfOrientation);
		WritePrivateProfileStringW(szSection, L"Orientation",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Orientation", str);

		str.Format(L"%d", m_plfFontP[nStyle].lfWeight);
		WritePrivateProfileStringW(szSection, L"Weight",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Weight", str);

		str.Format(L"%d", m_plfFontP[nStyle].lfItalic);
		WritePrivateProfileStringW(szSection, L"Italic",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"Italic", m_plfFontP[nStyle].lfItalic);
		str.Format(L"%d", m_plfFontP[nStyle].lfUnderline);
		WritePrivateProfileStringW(szSection, L"Underline",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"Underline", m_plfFontP[nStyle].lfUnderline);
		str.Format(L"%d", m_plfFontP[nStyle].lfStrikeOut);
		WritePrivateProfileStringW(szSection, L"StrikeOut",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"StrikeOut", m_plfFontP[nStyle].lfStrikeOut);

		str.Format(L"%d", m_plfFontP[nStyle].lfCharSet);
		WritePrivateProfileStringW(szSection, L"CharSet",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"CharSet", m_plfFontP[nStyle].lfCharSet);
		str.Format(L"%d", m_plfFontP[nStyle].lfOutPrecision);
		WritePrivateProfileStringW(szSection, L"OutPrecision",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"OutPrecision", m_plfFontP[nStyle].lfOutPrecision);
		str.Format(L"%d", m_plfFontP[nStyle].lfClipPrecision);
		WritePrivateProfileStringW(szSection, L"ClipPrecision",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"ClipPrecision", m_plfFontP[nStyle].lfClipPrecision);

		str.Format(L"%d", m_plfFontP[nStyle].lfPitchAndFamily);
		WritePrivateProfileStringW(szSection, L"PitchAndFamily",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"PitchAndFamily", m_plfFontP[nStyle].lfPitchAndFamily);
		str.Format(L"%d", m_plfFontP[nStyle].lfQuality);
		WritePrivateProfileStringW(szSection, L"Quality",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"Quality", m_plfFontP[nStyle].lfQuality);
		//str.Format(L"%d", m_plfFontP[nStyle].lfFaceName);
		WritePrivateProfileStringW(szSection, L"FaceName",m_plfFontP[nStyle].lfFaceName,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"FaceName", m_plfFontP[nStyle].lfFaceName);
		//end 20120927
	}

	// Update All MDI Child
	CMDIChildWnd* pChildWnd, *pActiveChildWnd;
	CMDIFrameWnd* pFrameWnd= (CMDIFrameWnd*)AfxGetMainWnd();

	pActiveChildWnd = pFrameWnd->MDIGetActive();
	pChildWnd = pActiveChildWnd;
	while (pChildWnd)
	{
		pChildWnd->PostMessage(WM_COMMAND, ID_OPTION_CHANGE, 0L);
		pFrameWnd->MDINext();
		pChildWnd = pFrameWnd->MDIGetActive();
		if (pChildWnd == pActiveChildWnd)
			break;
	}

	return CPropertyPage::OnApply();
}


void CTextStylesPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	OnSelchangeListstyles();
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}
//=============add by cy 1224 from here  to the end==============//
void CTextStylesPage::OnDisplay() 
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_CHECKSAME)->EnableWindow(false);
	OnSelchangeListstyles();
}

void CTextStylesPage::OnRadioPrint() 
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_CHECKSAME)->EnableWindow(true);
	OnSelchangeListstyles();
}

void CTextStylesPage::OnCheckSame() 
{
	// TODO: Add your control notification handler code here
	CButton *pSame=(CButton *)(GetDlgItem(IDC_CHECKSAME));
	if(pSame->GetCheck())
	{
		PrintSameDisplay();
		OnSelchangeListstyles();
	}	
}

void CTextStylesPage::InitDispInfo()
{
//	char szSection[TMPSTR_SIZE];

	CString szSection;
	
	int nStyle;
	CString str;
	CWinApp* pApp = AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	int m_nStyleNum = GetPrivateProfileIntW( L"EditPro" , L"TextStyleNum",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//int m_nStyleNum = pApp->GetProfileInt("EditPro", "TextStyleNum", 1);
	//end 20120927
	m_pnLineSpace = new int[m_nStyleNum];
	m_pnCharSpace = new int[m_nStyleNum];
	m_plfFont = new LOGFONT[m_nStyleNum];
	m_parrayStyleName = new CStringArray;
	m_parrayStyleName->RemoveAll();
//	m_StyleList.ResetContent();
	for(nStyle=0; nStyle < m_nStyleNum; nStyle ++)
	{
//		sprintf(szSection, "EditProTextStyle%d", nStyle+1);
		szSection.Format(L"EditProTextStyle%ld", nStyle+1);
		
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		m_pnLineSpace[nStyle] = GetPrivateProfileIntW(szSection, L"LineSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		m_pnCharSpace[nStyle] = GetPrivateProfileIntW(szSection, L"CharSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//m_pnLineSpace[nStyle] = pApp->GetProfileInt( szSection, L"LineSpace", 1 );
		//m_pnCharSpace[nStyle] = pApp->GetProfileInt( szSection, L"CharSpace", 1 );
		//end 20120927
		
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		CString str_temp;
		wchar_t sz_temp[255];
		GetPrivateProfileStringW(szSection, L"Height",L"-15" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//CString str_temp = pApp->GetProfileString( szSection, L"Height", L"-15" );
		//end 20120927
//		AfxMessageBox(str_temp);

//		const wchar_t * wc_tmp = (LPCTSTR) str_temp;
//		m_plfFont[nStyle].lfHeight = atol((const char *)wc_tmp);
		m_plfFont[nStyle].lfHeight = _wtol(str_temp);

		//edit by Ronald 20120927 for migrating from vc6 to 2010
		GetPrivateProfileStringW(szSection, L"Width",L"0" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Width", L"0" );
//		wc_tmp = (LPCTSTR) str_temp;
//		m_plfFont[nStyle].lfWidth  = atol((const char *)wc_tmp);
		m_plfFont[nStyle].lfWidth  = _wtol(str_temp);
		
		GetPrivateProfileStringW(szSection, L"Escapement",L"0" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Escapement", L"0" );
//		wc_tmp = (LPCTSTR) str_temp;
		m_plfFont[nStyle].lfEscapement = _wtol(str_temp);

		GetPrivateProfileStringW(szSection, L"Orientation",L"0" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Orientation", L"0" );
//		wc_tmp = (LPCTSTR) str_temp;
		m_plfFont[nStyle].lfOrientation = _wtol(str_temp);
		
		GetPrivateProfileStringW(szSection, L"Weight",L"400" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString(szSection, L"Weight", L"400" );
//		wc_tmp = (LPCTSTR) str_temp;
		m_plfFont[nStyle].lfWeight = _wtol(str_temp);
		
		m_plfFont[nStyle].lfItalic = GetPrivateProfileIntW(szSection, L"Italic",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Italic", 0 );
		m_plfFont[nStyle].lfUnderline = GetPrivateProfileIntW(szSection, L"Underline",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Underline", 0 );
		m_plfFont[nStyle].lfStrikeOut = GetPrivateProfileIntW(szSection, L"StrikeOut",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"StrikeOut", 0 );
		
		m_plfFont[nStyle].lfCharSet = GetPrivateProfileIntW(szSection, L"CharSet",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"CharSet", 0 );
		m_plfFont[nStyle].lfOutPrecision = GetPrivateProfileIntW(szSection, L"OutPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"OutPrecision", 0 );
		m_plfFont[nStyle].lfClipPrecision = GetPrivateProfileIntW(szSection, L"ClipPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"ClipPrecision", 0 );
		
		m_plfFont[nStyle].lfQuality = GetPrivateProfileIntW(szSection, L"Quality",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Quality", 0 );
		m_plfFont[nStyle].lfPitchAndFamily = GetPrivateProfileIntW(szSection, L"PitchAndFamily",49,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"PitchAndFamily", 49 );
		
		GetPrivateProfileStringW(szSection, L"FaceName",L"Courier New" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString(szSection, L"FaceName", L"Courier New");
//		wc_tmp = (LPCTSTR) str_temp;
//		strcpy((char *)m_plfFont[nStyle].lfFaceName, (const char *)wc_tmp);
		wcscpy(m_plfFont[nStyle].lfFaceName, str_temp);
		
		switch ( nStyle )
		{
		case 0:
			str.LoadString ( IDS_STYLE0 );
			break;
		case 1:
			str.LoadString ( IDS_STYLE1 ); 
			break;
		case 2:
			str.LoadString ( IDS_STYLE2 ); 
			break;
		case 3:
			str.LoadString ( IDS_STYLE3 ); 
			break;
		case 4:
			str.LoadString ( IDS_STYLE4 ); 
			break;
		case 5:
			str.LoadString ( IDS_STYLE5 ); 
			break;
		default:
			GetPrivateProfileStringW(szSection, L"StyleName",L"NORMAL" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
			str = sz_temp;
			//str = pApp->GetProfileString(szSection, L"StyleName", L"NORMAL");
			break;
		}
		m_parrayStyleName->Add(str);
		//end 20120927
//		m_StyleList.AddString(str);
	}

}

void CTextStylesPage::InitPrintInfo()
{
//	char szSection[TMPSTR_SIZE];
	CString szSection;
	int nStyle;
	CString str;
	CWinApp* pApp = AfxGetApp();
	//int m_nStyleNum = pApp->GetProfileInt("EditPro", "TextStyleNum", 1);
	int m_nStyleNum = GetPrivateProfileIntW( L"EditPro" , L"TextStyleNum",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	m_pnLineSpaceP = new int[m_nStyleNum];
	m_pnCharSpaceP = new int[m_nStyleNum];
	m_plfFontP = new LOGFONT[m_nStyleNum];
	for(nStyle=0; nStyle < m_nStyleNum; nStyle ++)
	{
		//sprintf((char *)szSection, (const char *)"EditProPrintTextStyle%d", nStyle+1);
		szSection.Format(L"EditProPrintTextStyle%ld", nStyle+1);
		
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		m_pnLineSpaceP[nStyle] = GetPrivateProfileIntW( szSection , L"LineSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		m_pnCharSpaceP[nStyle] = GetPrivateProfileIntW( szSection , L"CharSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//m_pnLineSpaceP[nStyle] = pApp->GetProfileInt( szSection, L"LineSpace", 1 );
		//m_pnCharSpaceP[nStyle] = pApp->GetProfileInt( szSection, L"CharSpace", 1 );
		
		CString str_temp;// = pApp->GetProfileString( szSection, L"Height", L"-15" );
		wchar_t sz_temp[255];
		GetPrivateProfileStringW( szSection , L"Height",L"-15",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
//		const wchar_t * wc_tmp = (LPCTSTR) str_temp;
		m_plfFontP[nStyle].lfHeight = _wtol(str_temp);


		GetPrivateProfileStringW( szSection , L"Width",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Width", L"0" );
//		wc_tmp = (LPCTSTR) str_temp;
		m_plfFontP[nStyle].lfWidth  = _wtol(str_temp);
		
		GetPrivateProfileStringW( szSection , L"Escapement",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Escapement", L"0" );
//		wc_tmp = (LPCTSTR) str_temp;
		m_plfFontP[nStyle].lfEscapement = _wtol(str_temp);

		GetPrivateProfileStringW( szSection , L"Orientation",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Orientation", L"0" );
//		wc_tmp = (LPCTSTR) str_temp;
		m_plfFontP[nStyle].lfOrientation = _wtol(str_temp);
		
		GetPrivateProfileStringW( szSection , L"Weight",L"400",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Weight", L"400" );
//		wc_tmp = (LPCTSTR) str_temp;
		m_plfFontP[nStyle].lfWeight = _wtol(str_temp);
		
		m_plfFontP[nStyle].lfItalic = GetPrivateProfileIntW( szSection , L"Italic",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Italic", 0 );
		m_plfFontP[nStyle].lfUnderline = GetPrivateProfileIntW( szSection , L"Underline",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Underline", 0 );
		m_plfFontP[nStyle].lfStrikeOut = GetPrivateProfileIntW( szSection , L"StrikeOut",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"StrikeOut", 0 );
		
		m_plfFontP[nStyle].lfCharSet = GetPrivateProfileIntW( szSection , L"CharSet",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"CharSet", 0 );
		m_plfFontP[nStyle].lfOutPrecision = GetPrivateProfileIntW( szSection , L"OutPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"OutPrecision", 0 );
		m_plfFontP[nStyle].lfClipPrecision = GetPrivateProfileIntW( szSection , L"ClipPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"ClipPrecision", 0 );
		
		m_plfFontP[nStyle].lfQuality = GetPrivateProfileIntW( szSection , L"Quality",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Quality", 0 );
		m_plfFontP[nStyle].lfPitchAndFamily = GetPrivateProfileIntW( szSection , L"PitchAndFamily",49,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"PitchAndFamily", 49 );
		
		GetPrivateProfileStringW( szSection , L"FaceName",L"Courier New",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString(szSection, L"FaceName", L"Courier New");
//		wc_tmp = (LPCTSTR) str_temp;
//		strcpy((char *)m_plfFontP[nStyle].lfFaceName, (const char *)wc_tmp);
		wcscpy(m_plfFontP[nStyle].lfFaceName, str_temp);
		
		switch ( nStyle )
		{
		case 0:
			str.LoadString ( IDS_STYLE0 );
			break;
		case 1:
			str.LoadString ( IDS_STYLE1 ); 
			break;
		case 2:
			str.LoadString ( IDS_STYLE2 ); 
			break;
		case 3:
			str.LoadString ( IDS_STYLE3 ); 
			break;
		case 4:
			str.LoadString ( IDS_STYLE4 ); 
			break;
		case 5:
			str.LoadString ( IDS_STYLE5 ); 
			break;
		default:
			GetPrivateProfileStringW( szSection , L"StyleName",L"NORMAL",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
			str = sz_temp;
			//str = pApp->GetProfileString((LPCTSTR)szSection, L"StyleName", L"NORMAL");
			break;
		}
		//end 20120927
		m_parrayStyleName->Add(str);
		//m_StyleList.AddString(str);
	}
}

void CTextStylesPage::PrintSameDisplay()
{
	CClientDC dc(this);
	int nStyle;
	CWinApp* pApp = AfxGetApp();
	CString str;
//	char	szSection[TMPSTR_SIZE];
	CString szSection;

	//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString str_temp;
	str_temp.Format(L"%d",dc.GetDeviceCaps(LOGPIXELSX));
	WritePrivateProfileStringW(L"EditProPrint", L"LogicalPixelX",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",dc.GetDeviceCaps(LOGPIXELSY));
	WritePrivateProfileStringW(L"EditProPrint", L"LogicalPixelX",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(L"EditProPrint", L"LogicalPixelX", dc.GetDeviceCaps(LOGPIXELSX));
	//pApp->WriteProfileInt(L"EditProPrint", L"LogicalPixelY", dc.GetDeviceCaps(LOGPIXELSY));
	//end 20120927
	for( nStyle=0; nStyle < m_nStyleNum; nStyle ++)
	{
//		sprintf(szSection, "EditProPrintTextStyle%d", nStyle+1);
		szSection.Format(L"EditProPrintTextStyle%ld", nStyle+1);
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		str_temp.Format(L"%d",m_pnLineSpace[nStyle]);
		WritePrivateProfileStringW(szSection, L"LineSpace",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"LineSpace", m_pnLineSpace[nStyle]);
		str_temp.Format(L"%d",m_pnCharSpace[nStyle]);
		WritePrivateProfileStringW(szSection, L"CharSpace",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"CharSpace", m_pnCharSpace[nStyle]);
		
		str.Format(L"%d", m_plfFont[nStyle].lfHeight);
		WritePrivateProfileStringW(szSection, L"Height",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Height", str);
		str.Format(L"%d", m_plfFont[nStyle].lfWidth);
		WritePrivateProfileStringW(szSection, L"Width",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Width", str);
		
		str.Format(L"%d", m_plfFont[nStyle].lfEscapement);
		WritePrivateProfileStringW(szSection, L"Escapement",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Escapement", str);
		str.Format(L"%d", m_plfFont[nStyle].lfOrientation);
		WritePrivateProfileStringW(szSection, L"Orientation",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Orientation", str);
		
		str.Format(L"%d", m_plfFont[nStyle].lfWeight);
		WritePrivateProfileStringW(szSection, L"Weight",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString(szSection, L"Weight", str);
		
		str.Format(L"%d", m_plfFont[nStyle].lfItalic);
		WritePrivateProfileStringW(szSection, L"Italic",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt( szSection, L"Italic", m_plfFont[nStyle].lfItalic);
		str.Format(L"%d", m_plfFont[nStyle].lfUnderline);
		WritePrivateProfileStringW(szSection, L"Underline",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"Underline", m_plfFont[nStyle].lfUnderline);
		str.Format(L"%d", m_plfFont[nStyle].lfStrikeOut);
		WritePrivateProfileStringW(szSection, L"StrikeOut",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(szSection, L"StrikeOut", m_plfFont[nStyle].lfStrikeOut);
		
		str.Format(L"%d", m_plfFont[nStyle].lfCharSet);
		WritePrivateProfileStringW(szSection, L"CharSet",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt( szSection, L"CharSet", m_plfFont[nStyle].lfCharSet);
		str.Format(L"%d", m_plfFont[nStyle].lfOutPrecision);
		WritePrivateProfileStringW(szSection, L"OutPrecision",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt( szSection, L"OutPrecision", m_plfFont[nStyle].lfOutPrecision);
		str.Format(L"%d", m_plfFont[nStyle].lfClipPrecision);
		WritePrivateProfileStringW(szSection, L"ClipPrecision",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt( szSection, L"ClipPrecision", m_plfFont[nStyle].lfClipPrecision);
		
		str.Format(L"%d",m_plfFont[nStyle].lfPitchAndFamily);
		WritePrivateProfileStringW(szSection, L"PitchAndFamily",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt( szSection, L"PitchAndFamily", m_plfFont[nStyle].lfPitchAndFamily);
		str.Format(L"%d", m_plfFont[nStyle].lfQuality);
		WritePrivateProfileStringW(szSection, L"Quality",str.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt( szSection, L"Quality", m_plfFont[nStyle].lfQuality);
		//str.Format(L"%d", m_plfFont[nStyle].lfFaceName);
		WritePrivateProfileStringW(szSection, L"FaceName",m_plfFont[nStyle].lfFaceName,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileString( szSection, L"FaceName", m_plfFont[nStyle].lfFaceName);
	}
	InitPrintInfo();
}


void CTextStylesPage::OnChangeEditautosavetime() 
{
	SetModified();
}


void CTextStylesPage::OnCheckautosave() 
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