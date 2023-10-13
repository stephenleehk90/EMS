// FileInfoDisplayView.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "FileInfoDisplayView.h"
#include "DoFileDoc.h"
#include "DoColDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileInfoDisplayView
extern void ResizeBitmap(CBitmap* pbmpOld, CBitmap& bmpNew, int nNewSize);

IMPLEMENT_DYNCREATE(CFileInfoDisplayView, CFormView)

LRESULT CALLBACK RemoveHasSetSelSubclassProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uiMsg) {
    case WM_NCDESTROY:
        RemoveWindowSubclass(hwnd, RemoveHasSetSelSubclassProc, uIdSubclass);
        break;
    case WM_GETDLGCODE:
        return DefSubclassProc(hwnd, uiMsg, wParam, lParam) & ~DLGC_HASSETSEL;
    }
    return DefSubclassProc(hwnd, uiMsg, wParam, lParam);
}

CFileInfoDisplayView::CFileInfoDisplayView()
	: CFormView(CFileInfoDisplayView::IDD)
{
	//{{AFX_DATA_INIT(CFileInfoDisplayView)
	m_strFileContent = _T("");
	//}}AFX_DATA_INIT
	m_strFilecode=_T("");
	m_bContentUpdated=FALSE;
	m_bRecordUpdated=FALSE;
	m_bPicUpdated=FALSE;

	m_bInitialized = false;


	LOGFONT lf;	
	memset(&lf, 0, sizeof(lf));
	/*lf.lfHeight = -15;
	lf.lfWeight = 0; 
	lf.lfCharSet = ANSI_CHARSET; 
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lf.lfQuality = DEFAULT_QUALITY; 
	lf.lfPitchAndFamily = 49; 
	//lstrcpy(lf.lfFaceName, L"Arial Unicode MS"); 
	//lstrcpy(lf.lfFaceName, L"新細明體");

	// Edit Stephen 2013-03-22
	if (IsInstalled(L"新細明體-ExtB"))
		lstrcpy(lf.lfFaceName, L"新細明體-ExtB");
	else if (IsInstalled(L"細明體_HKSCS-ExtB"))
		lstrcpy(lf.lfFaceName, L"細明體_HKSCS-ExtB");
	else if (IsInstalled(L"細明體-ExtB"))
		lstrcpy(lf.lfFaceName, L"細明體-ExtB");
	else
		lstrcpy(lf.lfFaceName, L"新細明體");
	// End Edit Stephen 2013-03-22*/
	CString str_temp;
	wchar_t sz_temp[255];
	lf.lfHeight = GetPrivateProfileIntW(L"DisplayTextStyle",L"Height",-15,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfWidth  = GetPrivateProfileIntW(L"DisplayTextStyle",L"Width",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfEscapement = GetPrivateProfileIntW(L"DisplayTextStyle",L"Escapement",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfOrientation = GetPrivateProfileIntW(L"DisplayTextStyle",L"Orientation",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfWeight = GetPrivateProfileIntW(L"DisplayTextStyle", L"Weight",400,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfItalic = GetPrivateProfileIntW(L"DisplayTextStyle",L"Italic",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfUnderline = GetPrivateProfileIntW(L"DisplayTextStyle",L"Underline",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfStrikeOut = GetPrivateProfileIntW(L"DisplayTextStyle",L"StrikeOut",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfCharSet = GetPrivateProfileIntW(L"DisplayTextStyle",L"CharSet",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfOutPrecision = GetPrivateProfileIntW(L"DisplayTextStyle",L"OutPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfClipPrecision = GetPrivateProfileIntW(L"DisplayTextStyle",L"ClipPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfQuality = GetPrivateProfileIntW(L"DisplayTextStyle",L"Quality",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfPitchAndFamily = GetPrivateProfileIntW(L"DisplayTextStyle",L"PitchAndFamily",49,((CWinApp*)AfxGetApp())->m_pszProfileName);
	GetPrivateProfileStringW(L"DisplayTextStyle", L"FaceName",L"新細明體",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp = sz_temp;
	wcscpy(lf.lfFaceName, str_temp);

	m_fontFace.CreateFontIndirect(&lf);
//	m_fontFace.CreatePointFont(SYSDEFAULTFONTPOINT,SYSDEFAULTFONTNAME,NULL);
}

CFileInfoDisplayView::~CFileInfoDisplayView()
{
	m_lcRecord.UpdateUserSetting();
	m_fontFace.DeleteObject();
}

void CFileInfoDisplayView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileInfoDisplayView)
	DDX_Control(pDX, IDC_FILECONTENT, m_edtFileContent);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_LISTIMAGE, m_lcImage);
	DDX_Control(pDX, IDC_FILERECORD, m_lcRecord);
	DDX_Text(pDX, IDC_FILECONTENT, m_strFileContent);
	DDX_Control(pDX, IDC_LEADCTRL1, m_LeadCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileInfoDisplayView, CFormView)
	//{{AFX_MSG_MAP(CFileInfoDisplayView)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LISTIMAGE, OnGetdispinfoListimage)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FILESELCHANGED,OnFileSelChanged)
	ON_MESSAGE(WM_LEAVEPIC,OnShowTips)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNeedText)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTIMAGE, &CFileInfoDisplayView::OnNMDblclkListimage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileInfoDisplayView diagnostics

#ifdef _DEBUG
void CFileInfoDisplayView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFileInfoDisplayView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileInfoDisplayView message handlers
const int TabHeight=20;

void CFileInfoDisplayView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ShowScrollBar(SB_VERT,	FALSE);
	CRect clientRect;
	if(m_Tab.GetSafeHwnd())
	{	
		GetClientRect(&clientRect);
		m_Tab.MoveWindow(&clientRect);
		cx = clientRect.right - clientRect.left;
		cy = clientRect.bottom - clientRect.top;
		//m_Tab.MoveWindow(-1,-1,cx+2,cy+1);
		m_lcImage.MoveWindow(-2,-2,cx+2,cy-TabHeight-2);
		m_lcRecord.MoveWindow(-2,-2,cx+2,cy-TabHeight-2);
		m_edtFileContent.MoveWindow(-2,-2,cx+2,cy-TabHeight-2);
		m_edtFileContent.Invalidate(FALSE);
		m_Tab.UpdateWindow();
		m_edtFileContent.BringWindowToTop();
	}	
}

void CFileInfoDisplayView::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	switch(m_Tab.GetCurSel())
	{
	case 0:
		{
			m_edtFileContent.ShowWindow(SW_SHOW);
			m_lcImage.ShowWindow(SW_HIDE);
			m_lcRecord.ShowWindow(SW_HIDE);
			if(!m_bContentUpdated)
			{
				ShowFileContent(m_strFilecode);
				m_bContentUpdated=TRUE;
			}
			break;
		}
	case 1:
		{
			m_edtFileContent.ShowWindow(SW_HIDE);
			m_lcImage.ShowWindow(SW_HIDE);
			m_lcRecord.ShowWindow(SW_SHOW);
			if(!m_bRecordUpdated)
			{
				ShowFileRecord(m_strFilecode);
				m_bRecordUpdated=TRUE;
			}
			break;
		}
	case 2:
		{
			m_edtFileContent.ShowWindow(SW_HIDE);
			m_lcImage.ShowWindow(SW_SHOW);
			m_lcRecord.ShowWindow(SW_HIDE);
			if(!m_bPicUpdated)
			{
				ShowPicInfo(m_strFilecode);
				m_bPicUpdated=TRUE;
			}
		}
		break;
	default:
		break;
	}
	*pResult = 0;
}

void CFileInfoDisplayView::OnInitialUpdate() 
{
	//AfxInitRichEdit();
	CFormView::OnInitialUpdate();
	CGlobal::GetInstance()->RegistLeadTools(&m_LeadCtrl);
	// TODO: Add your specialized code here and/or call the base class
	OSVERSIONINFO osver;
	osver.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&osver);
	if(osver.dwPlatformId&(VER_PLATFORM_WIN32_WINDOWS|VER_PLATFORM_WIN32_NT ))
		m_Tab.ModifyStyle(NULL,TCS_BOTTOM);
	CResManage res(g_hInst);
	CString str;
	str.LoadString(IDS_FILECONTENT);
	TC_ITEM tc={0};
    
    tc.pszText=(LPWSTR)(LPCTSTR)str/*"原文記錄"*/;
	tc.cchTextMax = str.GetLength();
    tc. mask =TCIF_TEXT|TCIF_PARAM;
    tc.lParam=(LPARAM)0;
    m_Tab.InsertItem(0, &tc);
	
	str.LoadString(IDS_FILERECORD);
    tc.pszText=(LPWSTR)(LPCTSTR)str/*"操作記錄"*/;
    tc.cchTextMax = str.GetLength();
    tc. mask =TCIF_TEXT|TCIF_PARAM;
    tc.lParam=(LPARAM)1;
    m_Tab.InsertItem(1, &tc);
	
	
	if(CGlobal::GetInstance()->m_pShareData->m_bMediaColumnExist == TRUE 
		|| CGlobal::GetInstance()->m_pShareData->m_bSupportMedia == TRUE)
		str = L"關聯多媒體";
	else
		str.LoadString(IDS_RELATEIMAGE);
    tc.pszText=(LPWSTR)(LPCTSTR)str/*"關聯記錄"*/;
    tc.cchTextMax = str.GetLength();
    tc. mask =TCIF_TEXT|TCIF_PARAM;
    tc.lParam=(LPARAM)2;
    m_Tab.InsertItem(2, &tc);
	
	
	
	//SWP_NOSIZE   
	CHARFORMAT cf;
	
	// Modify the default character format so that all new
	// text is striked out and not bold.
	cf.dwMask = CFM_CHARSET;
	//cf.dwEffects = CFE_STRIKEOUT;
	cf.bCharSet = DEFAULT_CHARSET;
	
	m_edtFileContent.SetDefaultCharFormat(cf);
	m_edtFileContent.ShowWindow(SW_SHOW);
	m_edtFileContent.SetFont(&m_fontFace);	
	::SendMessage(m_edtFileContent.GetSafeHwnd(), EM_SETLANGOPTIONS, 0, 0);
	SetWindowSubclass(m_edtFileContent,RemoveHasSetSelSubclassProc, 0, 0);
	
	InitFileRecordList();
	InitPicList();
	CreateToolTip();
	m_lcImage.ShowWindow(SW_HIDE);
	m_lcRecord.ShowWindow(SW_HIDE);
	m_edtFileContent.SetBackgroundColor(FALSE,CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
	m_lcImage.SetBkColor(CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
	m_lcRecord.SetBkColor(CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
	m_lcRecord.SetTextBkShowMode(2,0,CGlobal::GetInstance()->m_pShareData->CLR_TEXTBKODD,CGlobal::GetInstance()->m_pShareData->CLR_TEXTBKEVEN);

	
	CRect	rect;
	GetWindowRect(&rect);
	m_Tab.MoveWindow(0,0,rect.right-rect.left, rect.bottom-rect.top);//-TabHeight);
	
	m_bInitialized = true;
	m_edtFileContent.SetEventMask(ENM_MOUSEEVENTS);
}
LRESULT CFileInfoDisplayView::OnFileSelChanged(WPARAM wParam,LPARAM lParam)
{
	if (!m_bInitialized)
		return 0;
	
	m_strFilecode=*(CString *)wParam;
	m_bContentUpdated=FALSE;
	m_bRecordUpdated=FALSE;
	m_bPicUpdated=FALSE;
	switch(m_Tab.GetCurSel())
	{
	case 0:
		{			
			m_Tab.LockWindowUpdate();
			ShowFileContent(*(CString *)wParam);
			m_bContentUpdated=TRUE;
			m_Tab.UnlockWindowUpdate();
			m_Tab.UpdateWindow();
			break;
		}
	case 1:
		{
			m_bRecordUpdated=TRUE;
			ShowFileRecord(*(CString *)wParam);
			break;
		}
	case 2:
		{
			m_bPicUpdated=TRUE;
			ShowPicInfo(*(CString *)wParam);
		}
		break;
	default:
		break;
	}
	return 0;
}

void CFileInfoDisplayView::ShowFileContent(CString strFileCode)
{
	if(m_strFilecode.IsEmpty())
	{		
		m_strFileContent = _T("");
	}
	else
	{
		CDocument* pDoc=GetDocument();
		if(pDoc->IsKindOf(RUNTIME_CLASS(CDoFileDoc)))
			((CDoFileDoc*)pDoc)->m_Mgr.GetFileText(strFileCode,m_strFileContent);
		else if(pDoc->IsKindOf(RUNTIME_CLASS(CDoColDoc)))
			((CDoColDoc*)pDoc)->m_Mgr.GetFileText(strFileCode,m_strFileContent);
	}
	UpdateData(FALSE);	
		
	if (m_strFileContent.GetLength() > 0 )
	{
		TCHAR char_temp = m_strFileContent.GetAt(0); 
		if (char_temp == 65279)
			m_strFileContent = m_strFileContent.Right(m_strFileContent.GetLength()-1);  
	}
//AfxMessageBox(m_strFileContent);
	GetDlgItem(IDC_FILECONTENT)->SetWindowText(m_strFileContent);/*
	m_edtFileContent.SetSel(0,0);
	m_edtFileContent.LineScroll(0);*/
	m_edtFileContent.PostMessage(WM_VSCROLL, SB_TOP, 0);
}

void CFileInfoDisplayView::ShowFileRecord(CString strFileCode)
{
	if(m_strFilecode.IsEmpty())m_lcRecord.DeleteAllItems();
	else
	{
		CDocument* pDoc=GetDocument();
		m_lcRecord.DeleteAllItems();
		CWnd *pWnd=m_lcRecord.GetParent();
		CGlobal *pGl = CGlobal::GetInstance();
		if(pDoc->IsKindOf(RUNTIME_CLASS(CDoFileDoc)))
			pGl->GetFileRecord(&m_lcRecord,strFileCode);
		else if(pDoc->IsKindOf(RUNTIME_CLASS(CDoColDoc)))
			pGl->GetFileRecord(&m_lcRecord,strFileCode);
	}
}

void CFileInfoDisplayView::ShowPicInfo(CString strFileCode)
{
	if(m_strFilecode.IsEmpty())
		m_lcImage.DeleteAllItems();
	else
	{
		CDocument* pDoc=GetDocument();
		m_lcImage.DeleteAllItems();
		m_ImageList.DeleteImageList();
		m_ImageList.Create(PIC_SIZE, PIC_SIZE,ILC_COLOR24 | ILC_MASK, 1,1);
		CBitmap	bitmapAdd;
		CBitmap bmpDisplay;
		bitmapAdd.LoadBitmap(IDB_BITMAP_BASE);
		ResizeBitmap(&bitmapAdd, bmpDisplay, PIC_SIZE);
		m_ImageList.Add(&bmpDisplay, CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
		bitmapAdd.DeleteObject();
		bmpDisplay.DeleteObject();

		//	將圖象列表與LIST控件相關聯﹔
		m_lcImage.SetImageList(&m_ImageList, LVSIL_NORMAL);
		if(pDoc->IsKindOf(RUNTIME_CLASS(CDoFileDoc)))
			((CDoFileDoc*)pDoc)->m_Mgr.SetupRelatePicList(&m_lcImage,&m_LeadCtrl,strFileCode);
		else if(pDoc->IsKindOf(RUNTIME_CLASS(CDoColDoc)))
			((CDoColDoc*)pDoc)->m_Mgr.SetupRelatePicList(&m_lcImage,&m_LeadCtrl,strFileCode);

	}	
}

void CFileInfoDisplayView::InitFileRecordList()
{
	LV_COLUMN		lvcolumn;
	CString			rgtsz[6] ;
	CRect			rect ;
	int				i;
	int				rgtcx[6] = { 3 , 2 , 2 ,2 , 4 , 12 };
	int				rgtcol[6]={FILERECORD_DOTIME,FILERECORD_OPERATION,FILERECORD_OPERATOR,
		FILERECORD_WORDCOUNT,FILERECORD_POSITION,FILERECORD_COMMENT
	};
	m_lcRecord.SetFullRowSel(TRUE);
	CResManage res(g_hInst);
	rgtsz[0].LoadString(IDS_RCDITEM0);
	rgtsz[1].LoadString(IDS_RCDITEM1);
	rgtsz[2].LoadString(IDS_RCDITEM2);
	rgtsz[3].LoadString(IDS_RCDITEM3);
	rgtsz[4].LoadString(IDS_RCDITEM4);
	rgtsz[5].LoadString(IDS_RCDITEM5);
    AfxGetMainWnd()->GetClientRect(&rect);
	
	// Insert ListCtrl Columns
	for (i = 0; i < 6; i++)  // add the columns to the list control
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = (LPTSTR)(LPCTSTR)rgtsz[rgtcol[i]];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = (rect.Width()- 100) * rgtcx[rgtcol[i]] / 25;  // SubItem is twice as large
		m_lcRecord.InsertColumnEx(i,i, &lvcolumn);  // assumes return value is OK.
	}
	m_lcRecord.EnableHeaderDrag(TRUE);
	m_lcRecord.RegistCtrlID(FILERELA_RECORDLIST, FILERELA_RECORDLIST_VER);
	m_lcRecord.UpdateUserSetting(TRUE);
}

void CFileInfoDisplayView::InitPicList()
{
	LV_COLUMN		lvcolumn;
	CString			rgtsz[6] ;
	CRect			rect ;
	int				i;
	int				rgtcx[6] = { 3 , 2 , 0 ,2 , 4 , 12 };
	CResManage res(g_hInst);
	rgtsz[0].LoadString(IDS_PHOTONAME);
	rgtsz[1].LoadString(IDS_PHOTOCODE);
	rgtsz[2].LoadString(IDS_PHOTOPATH);
	rgtsz[3].LoadString(IDS_PHOTOPOS);
	AfxGetMainWnd()->GetClientRect(&rect);
	
	// Insert ListCtrl Columns
	for (i = 0; i < 4; i++)  // add the columns to the list control
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = (LPTSTR)(LPCTSTR)rgtsz[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = (rect.Width() - 100) * rgtcx[i] / 21;  // SubItem is twice as large
		m_lcImage.InsertColumn(i, &lvcolumn);  // assumes return value is OK.
	}
	if(CGlobal::GetInstance()->m_pShareData->m_bMediaColumnExist == TRUE 
			|| CGlobal::GetInstance()->m_pShareData->m_bSupportMedia == TRUE)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = L"Media_Type";
		lvcolumn.iSubItem = 4;
		lvcolumn.cx = 0;  // SubItem is twice as large
		m_lcImage.InsertColumn(4, &lvcolumn);

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = L"Media_Path";
		lvcolumn.iSubItem = 5;
		lvcolumn.cx = 0;  // SubItem is twice as large
		m_lcImage.InsertColumn(5, &lvcolumn);
	}
	m_ImageList.Create(PIC_SIZE, PIC_SIZE,ILC_COLOR24 | ILC_MASK, 1,1);
	CBitmap		bitmapAdd;
	bitmapAdd.LoadBitmap(IDB_BITMAP_BASE);
	m_ImageList.Add(&bitmapAdd, CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
	//	將圖象列表與LIST控件相關聯﹔
	m_lcImage.SetImageList(&m_ImageList, LVSIL_NORMAL);
	m_lcImage.m_hParent=this->m_hWnd;

	m_lcImage.SetTextBkColor(CGlobal::GetInstance()->m_pShareData->CLR_TEXTBK);
	m_lcImage.SetBkColor(CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
	
//	m_lcImage.SetExtendedStyle(LVS_EX_BORDERSELECT);
	m_lcImage.SetIconSpacing(PIC_SIZE+15,PIC_SIZE+35);
}
BOOL CFileInfoDisplayView::OnToolTipNeedText(UINT id, NMHDR * pTTTStruct, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;
    UINT nID =pTTTStruct->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
		nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
 			CPoint CurPoint;
			int nItem,nPos;
			::GetCursorPos(&CurPoint);
			m_lcImage.ScreenToClient(&CurPoint);
			if((nItem=m_lcImage.HitTest(CurPoint))!=-1)
			{
				CString str;
				str=m_lcImage.GetItemText(nItem,3);//position
				nPos = _ttoi(str);
				str=m_lcImage.GetItemText(nItem,1);//photocode
				CDocument* pDoc=GetDocument();
				if(pDoc->IsKindOf(RUNTIME_CLASS(CDoFileDoc)))
					((CDoFileDoc*)pDoc)->m_Mgr.GetPicInfo(str,nPos,m_strTooltipsText);
				else if(pDoc->IsKindOf(RUNTIME_CLASS(CDoColDoc)))
					((CDoColDoc*)pDoc)->m_Mgr.GetPicInfo(str,nPos,m_strTooltipsText);
				pTTT->lpszText = (LPTSTR)(LPCTSTR)m_strTooltipsText;
				m_Tooltip.SetDelayTime(TTDT_AUTOPOP , 90000000);
				pTTT->hinst = AfxGetResourceHandle();
				return(TRUE);
			}
		}
    }
    return(FALSE);
}

BOOL CFileInfoDisplayView::CreateToolTip()
{
	BOOL bCreate = m_Tooltip.Create(this);
	if(bCreate)
	{
		m_Tooltip.AddTool((CWnd*)&m_lcImage);
		m_Tooltip.Activate(TRUE);
		m_Tooltip.SetMaxTipWidth(1000);
		EnableToolTips(TRUE);		
	}
	return bCreate;
}

BOOL CFileInfoDisplayView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message== WM_MOUSEMOVE)
		m_Tooltip.RelayEvent(pMsg);
	else if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == m_edtFileContent.m_hWnd)
	{
		if((pMsg->wParam == _T('C')) && (GetKeyState(VK_CONTROL) & 0x8000) != 0)
		{
			m_edtFileContent.Copy();
		}
		if((pMsg->wParam == _T('A')) && (GetKeyState(VK_CONTROL) & 0x8000) != 0)
		{
			m_edtFileContent.SetSel(0,-1);
		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}

void CFileInfoDisplayView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//if(m_lcImage.HitTest(point)!=-1)
	//	m_Tooltip.Pop();
	CFormView::OnMouseMove(nFlags, point);
}
LRESULT CFileInfoDisplayView::OnShowTips(WPARAM wParam,LPARAM lParam)
{
	m_Tooltip.Pop();
	return 0;
}

void CFileInfoDisplayView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Tab.UpdateWindow();
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CFileInfoDisplayView::OnGetdispinfoListimage(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	LV_ITEM * pImageItem;
	
	pImageItem = &(pDispInfo->item);
	int nItem = pImageItem->iItem;
	//AfxMessageBox("OK");
	if(pImageItem->mask&LVIF_IMAGE )
	{
		CDocument* pDoc=GetDocument();
		if(pDoc->IsKindOf(RUNTIME_CLASS(CDoFileDoc)))
			((CDoFileDoc*)pDoc)->m_Mgr.SetLargeItem(&m_lcImage,&m_LeadCtrl,pImageItem);
		else if(pDoc->IsKindOf(RUNTIME_CLASS(CDoColDoc)))
			((CDoColDoc*)pDoc)->m_Mgr.SetLargeItem(&m_lcImage,&m_LeadCtrl,pImageItem);
	}
	*pResult = 0;
}

BOOL CFileInfoDisplayView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	MSGFILTER * lpMsgFilter = (MSGFILTER *)lParam; 
	
	if ((lpMsgFilter->nmhdr.code == EN_MSGFILTER) && (lpMsgFilter->msg == WM_RBUTTONDOWN))                                       
		
    {
		CResManage myRes(g_hInst);
		if(wParam == IDC_FILECONTENT)
		{
			CPoint point;                                            
			::GetCursorPos(&point); 
			CMenu menu; 
			DWORD dwSelectionMade;                                       
			VERIFY(menu.LoadMenu(IDR_MENU_RICHEDIT));  
			CMenu *pmenuPopup = menu.GetSubMenu(0);
			ASSERT(pmenuPopup != NULL);
			UINT nUndo=(m_edtFileContent.CanUndo() ? 0 : MF_GRAYED);
			DWORD dwStyle = m_edtFileContent.GetStyle();
			BOOL bReadOnly = dwStyle & ES_READONLY;
			pmenuPopup->EnableMenuItem(ID_RICHEDIT_UNDO, MF_BYCOMMAND | nUndo);
			
			UINT nFlags = ((m_edtFileContent.GetSelectionType() != SEL_EMPTY) ? 0 : MF_GRAYED);
			pmenuPopup->EnableMenuItem(ID_RICHEDIT_COPY, MF_BYCOMMAND | nFlags);
			if(bReadOnly)
				nFlags = MF_GRAYED;
			pmenuPopup->EnableMenuItem(ID_RICHEDIT_CUT, MF_BYCOMMAND | nFlags);
			pmenuPopup->EnableMenuItem(ID_RICHEDIT_DELETE, MF_BYCOMMAND | nFlags);
			
			nFlags = (m_edtFileContent.CanPaste() ? 0 : MF_GRAYED);
			if(bReadOnly)
				nFlags = MF_GRAYED;
			pmenuPopup->EnableMenuItem(ID_RICHEDIT_PASTE, MF_BYCOMMAND | nFlags);
			int iLen = m_edtFileContent.GetWindowTextLength();
			long iStartSel;
			long iEndSel;
			m_edtFileContent.GetSel(iStartSel, iEndSel);
			nFlags = (!iLen || (iStartSel <= 0 && iEndSel >= iLen)) ? MF_GRAYED : 0;
			pmenuPopup->EnableMenuItem(ID_RICHEDIT_ALL, MF_BYCOMMAND | nFlags);
			
			dwSelectionMade = pmenuPopup->TrackPopupMenu( (TPM_LEFTALIGN|TPM_LEFTBUTTON|
				TPM_NONOTIFY|TPM_RETURNCMD), point.x, point.y, this
				);
			switch(dwSelectionMade) 
			{
			case ID_RICHEDIT_UNDO:
				m_edtFileContent.Undo();
				break;
			case ID_RICHEDIT_CUT:
				m_edtFileContent.Cut();
				break;
			case ID_RICHEDIT_COPY:
				m_edtFileContent.Copy();
				break;
			case  ID_RICHEDIT_PASTE:
				m_edtFileContent.Paste();
				break;
			case ID_RICHEDIT_DELETE:
				m_edtFileContent.Clear();
				break;
			case ID_RICHEDIT_ALL:
				m_edtFileContent.SetSel(0, -1);
				break;
			default:
				break;
			}
			pmenuPopup->DestroyMenu(); 
		}
	}
	else if (lpMsgFilter->nmhdr.code == HDN_ITEMCHANGING)
	{
		int nLastItem=m_lcRecord.GetItemCount()-1; 
		if (nLastItem >= 0)
			ShowFileRecord(m_strFilecode);
		/*if(m_lcRecord.GetItemCount()>0)
			m_lcRecord.SetColumnWidth(0,m_lcRecord.GetColumnWidth(0));*/
	}
	return CFormView::OnNotify(wParam, lParam, pResult);
}

void CFileInfoDisplayView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFormView::OnShowWindow(bShow, nStatus);
	/*
	if(bShow)
	{
		RECT rcWin;
		this->GetWindowRect(&rcWin);
		LPARAM lparam = ((rcWin.right - rcWin.left) | ((rcWin.bottom-rcWin.top) << 16));
		PostMessage(WM_SIZE, SIZE_RESTORED, lparam);	
	}
	*/
}

static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* /*lpelfe*/, NEWTEXTMETRICEX* /*lpntme*/, int /*FontType*/, LPARAM lParam)
{
    LPARAM* l = (LPARAM*)lParam;
    *l = TRUE;
    return TRUE;
}

bool CFileInfoDisplayView::IsInstalled(LPCTSTR lpszFont)
{
    // Get the screen DC
    CDC dc;
    if (!dc.CreateCompatibleDC(NULL))
    {
    	return false;
    }
    LOGFONT lf = { 0 };
    // Any character set will do
    lf.lfCharSet = DEFAULT_CHARSET;
    // Set the facename to check for
    _tcscpy(lf.lfFaceName, lpszFont);
    LPARAM lParam = 0;
    // Enumerate fonts
    ::EnumFontFamiliesEx(dc.GetSafeHdc(), &lf,  (FONTENUMPROC)EnumFontFamExProc, (LPARAM)&lParam, 0);
    return lParam ? true : false;
}


void CFileInfoDisplayView::OnNMDblclkListimage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int index = m_lcImage.GetSelectionMark();
	if(index <=-1)
		return;
	if(CGlobal::GetInstance()->m_pShareData->m_bMediaColumnExist == TRUE 
			|| CGlobal::GetInstance()->m_pShareData->m_bSupportMedia == TRUE)
	{
		if(_wtoi(m_lcImage.GetItemText(index, 4)) == DBG_TYPE_INT_AV)
		{
			CString strPath = L"";
			strPath = m_lcImage.GetItemText(index, 5);
			if(AfxMessageBox(L"是否直接播放多媒體?",MB_OKCANCEL)==IDOK)
			{
				try
				{
					ShellExecute(NULL,NULL,strPath,NULL,NULL,SW_SHOWNORMAL);
				}
				catch (...)
				{
					//AfxMessageBox(IDS_AFX_UNSUPPORTTYPE);
				}
			}else
			{
				CString str, strPhotoPos;
				strPhotoPos = m_lcImage.GetItemText(index, 3);
				str = m_lcImage.GetItemText(index, 1);
				if(strPhotoPos == L"0")
					AfxGetMainWnd()->SendMessage(WM_DO_PIC,(WPARAM)&str);
				else
					AfxGetMainWnd()->SendMessage(WM_DO_PIC_COL,(WPARAM)&str);
			}
		}else
		{

			CString str, strPhotoPos;
			strPhotoPos = m_lcImage.GetItemText(index, 3);
			str = m_lcImage.GetItemText(index, 1);
			if(strPhotoPos == L"0")
				AfxGetMainWnd()->SendMessage(WM_DO_PIC,(WPARAM)&str);
			else
				AfxGetMainWnd()->SendMessage(WM_DO_PIC_COL,(WPARAM)&str);
		}
	}
	*pResult = 0;
}
