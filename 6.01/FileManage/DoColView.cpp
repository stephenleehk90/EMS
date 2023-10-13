// DoColView.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DoColView.h"
#include "DoColDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDoColView

IMPLEMENT_DYNCREATE(CDoColView, CFormView)

CDoColView::CDoColView()
	: CFormView(CDoColView::IDD)
{
	//{{AFX_DATA_INIT(CDoFileView)
	//}}AFX_DATA_INIT
}

CDoColView::~CDoColView()
{
	m_FileListCtr.UpdateUserSetting();
}


BEGIN_MESSAGE_MAP(CDoColView, CFormView)
	//{{AFX_MSG_MAP(CDoColView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, OnDblclkFilelist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILELIST, OnItemchangedFilelist)
	ON_NOTIFY(NM_RCLICK, IDC_FILELIST, OnRclickFilelist)
	ON_COMMAND(ID_RDO_RECORD, OnRdoRecord)
	ON_COMMAND(ID_RDO_SOURCE, OnRdoSource)
	ON_COMMAND(ID_RDO_COMMENT, OnRdoComment)
	ON_COMMAND(ID_RDO_COPY, OnRdoCopy)
	ON_UPDATE_COMMAND_UI(ID_RDO_COPY, OnUpdateRdoCopy)
	ON_COMMAND(ID_UNLOCK, OnUnlock)
	ON_COMMAND(ID_PRINTFILE, OnPrintfile)
	ON_COMMAND(ID_ADD_TO_STORY, OnAddToStory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoColView drawing

void CDoColView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDoColView diagnostics

#ifdef _DEBUG
void CDoColView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDoColView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDoColView message handlers

void CDoColView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ShowScrollBar(SB_HORZ,FALSE);
	CRect clientRect,rect,rect1;
	GetClientRect(&clientRect);
	if(m_FileListCtr.GetSafeHwnd()!=NULL)
	{
		/*GetWindowRect(&rect1);
		(GetDlgItem(IDC_FILEEDIT))->GetWindowRect(&rect);
		clientRect.top=rect.bottom-rect1.top;*/
		m_FileListCtr.MoveWindow(clientRect);
		
	}	
}

void CDoColView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	CString str_temp;
	wchar_t sz_temp[255];
	lf.lfHeight = GetPrivateProfileIntW(L"ViewTextStyle",L"Height",-15,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfWidth  = GetPrivateProfileIntW(L"ViewTextStyle",L"Width",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfEscapement = GetPrivateProfileIntW(L"ViewTextStyle",L"Escapement",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfOrientation = GetPrivateProfileIntW(L"ViewTextStyle",L"Orientation",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfWeight = GetPrivateProfileIntW(L"ViewTextStyle", L"Weight",400,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfItalic = GetPrivateProfileIntW(L"ViewTextStyle",L"Italic",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfUnderline = GetPrivateProfileIntW(L"ViewTextStyle",L"Underline",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfStrikeOut = GetPrivateProfileIntW(L"ViewTextStyle",L"StrikeOut",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfCharSet = GetPrivateProfileIntW(L"ViewTextStyle",L"CharSet",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfOutPrecision = GetPrivateProfileIntW(L"ViewTextStyle",L"OutPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfClipPrecision = GetPrivateProfileIntW(L"ViewTextStyle",L"ClipPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfQuality = GetPrivateProfileIntW(L"ViewTextStyle",L"Quality",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	lf.lfPitchAndFamily = GetPrivateProfileIntW(L"ViewTextStyle",L"PitchAndFamily",49,((CWinApp*)AfxGetApp())->m_pszProfileName);
	GetPrivateProfileStringW(L"ViewTextStyle", L"FaceName",L"新細明體",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp = sz_temp;
	wcscpy(lf.lfFaceName, str_temp);
	m_fontFace.CreateFontIndirect(&lf);
	m_FileListCtr.SetFont(&m_fontFace);
	
	// TODO: Add your specialized code here and/or call the base class
	InitList();
	InitImageList();
	CDoColDoc *pDoc=(CDoColDoc*)GetDocument();
	pDoc->m_Mgr.m_pFileListCtrl = &m_FileListCtr;
	pDoc->m_Mgr.m_pParentFrm = (CMDIChildWnd*)(GetParent()->GetParent()->GetParent());
	pDoc->m_Mgr.LoadPaperFile(FALSE);
	//GetParentFrame()->SendMessage(WM_FILESELCHANGED,_T(""),0);
}

void CDoColView::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILELIST, m_FileListCtr);
}

void CDoColView::InitList()
{
	CRect			rect;
	int				i ;
	LV_COLUMN		lvcolumn;
	CString			rgtsz[12];
#ifdef _STANDARD_VERSION_
	int				rgtcx[12] = { 80,25,120,45, 0,30,100,65,60,65,40,40 }; //DOFILE_HAVERELATE is 0
#else
	int				rgtcx[12] = { 80,25,120,45, 32,30,100,65,60,65,40,40 };
#endif
	
	int				rgtcol[12]={DOCOL_FILENAME,DOCOL_FILEFORMAT,DOCOL_TITLE,DOCOL_CURSTATUS, DOCOL_HAVERELATE,
		DOCOL_WORDCOUNT,DOCOL_FILEAUTHOR,DOCOL_TRANSDATE,DOCOL_SIGNEDFILE,DOCOL_UPDATEDATE,DOCOL_ASSIGNEDUSER,DOCOL_LOCKUSER};

	m_FileListCtr.SetFullRowSel(TRUE);
	m_FileListCtr.m_bSort=TRUE;
	long lStyle = LVS_REPORT|LVS_SHOWSELALWAYS;
	long lStyleOld = GetWindowLong(m_FileListCtr.m_hWnd, GWL_STYLE);
	lStyleOld &= ~lStyle;  
	lStyleOld |= lStyle;
	SetWindowLong(m_FileListCtr.m_hWnd, GWL_STYLE, lStyleOld);
	m_FileListCtr.SetBkColor(CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
	m_FileListCtr.SetTextBkColor(CGlobal::GetInstance()->m_pShareData->CLR_TEXTBK);	
	m_FileListCtr.SetTextBkShowMode(2,0,CGlobal::GetInstance()->m_pShareData->CLR_TEXTBKODD,CGlobal::GetInstance()->m_pShareData->CLR_TEXTBKEVEN);
	GetWindowRect(&rect);
	if (m_FileListCtr) m_FileListCtr.MoveWindow(0,0,rect.right-rect.left, rect.bottom-rect.top);
	rgtsz[0].LoadString(IDS_FILENAME);
	rgtsz[1].LoadString(IDS_FILEFORMAT);
	rgtsz[2].LoadString(IDS_TITLE);
	rgtsz[3].LoadString(IDS_CURSTATUS);
	rgtsz[4].LoadString(IDS_HAVERELATE);
	rgtsz[5].LoadString(IDS_WORDCOUNT);
	rgtsz[6].LoadString(IDS_FILEAUTHOR);
	rgtsz[7].LoadString(IDS_TRANSDATE);	
	rgtsz[8].LoadString(IDS_SIGNEDFILE);
	rgtsz[9].LoadString(IDS_UPDATEDATE);
	//rgtsz[9].LoadString(IDS_LASTMOTIME);
	rgtsz[10].LoadString(IDS_ASSIGNEDUSER);
	rgtsz[11].LoadString(IDS_LOCKUSER);
	m_FileListCtr.GetWindowRect(&rect);

	// Insert ListCtrl Columns
	for (i = 0; i < 12; i++)  // add the columns to the list control
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		////Commented by Ronald Saliman on 20120920
		//if(i == DOCOL_WORDCOUNT)
		//	lvcolumn.fmt = LVCFMT_RIGHT;
		//else
		////end 20120920
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = (LPTSTR)(LPCTSTR)rgtsz[rgtcol[i]];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = rect.Width() * rgtcx[rgtcol[i]] / 280;  // SubItem is twice as large
		m_FileListCtr.InsertColumnEx(i,i, &lvcolumn);  // assumes return value is OK.
	}
	m_FileListCtr.EnableHeaderDrag(TRUE);
	m_FileListCtr.RegistCtrlID(DOCOL_LIST, DOCOL_LIST_VER);
	//return;
	m_FileListCtr.UpdateUserSetting(TRUE);
	for (i = 0; i < 12; i++)
		m_FileListCtr.SetColType(i,0);
	m_FileListCtr.SetColType(DOCOL_WORDCOUNT,1);
	m_FileListCtr.SetColType(DOCOL_UPDATEDATE,2);
}

void CDoColView::InitImageList()
{
	CImageList	*pimagelist;
	pimagelist = &m_ImageList;		
	pimagelist->Create(16, 16, ILC_COLOR24|ILC_MASK/*bMask*/,	14/*nInitial*/, 14/*nGrow*/);
	CBitmap				bitmap;
	CResManage res(g_hInst);
	/*
	bitmap.LoadBitmap(IDB_BMFILE);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL0001);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL0010);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL0011);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL0100);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL0101);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL0110);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL0111);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL1000);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL1001);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL1010);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL1011);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_FL1100);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BMSIGN);
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();*/
	//稿件-----0
	//bitmap.LoadBitmap(IDB_BMFILE);
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_FILE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//鎖定的稿件-------1
	//bitmap.LoadBitmap(IDB_FL0001);
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_LOCK);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//緊急稿件-------2
	//bitmap.LoadBitmap(IDB_FL0010);
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_EMERGENCY);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//鎖定的緊急稿件-------3
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_LOCKSECURITE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//送譯稿件-------4
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_TRANSLATE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//鎖定的送譯稿件-------5
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_LOCKTRANSLATEFILE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//送譯的緊急稿件-------6
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_TRANSLATEEMERGENCY);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//鎖定的送譯緊急稿件-------7
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_LOCKTRANSEMERGENCY);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//加密稿件-------8
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_SECURITE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//鎖定的加密稿件-------9
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_LOCKEDSECURITE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//緊急加密稿件-------10
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_EMERGENCYSECURITE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//鎖定的緊急加密稿件-------11
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_LOCKEMERGENCYSECURITE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//送譯加密稿件-------12
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_TRANSLATESECURITE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//鎖定的送譯加密稿件-------13
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_LOCKEDTRANSLATESECURITE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//緊急的送譯加密稿件-------14
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_EMERGENCYTRANSLATESECURITE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//鎖定的緊急的送譯加密稿件-------15
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_LOCKEMERGENCYTRANSLATESECURITE);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//簽發的稿件-------16
	//bitmap.LoadBitmap(IDB_BMSIGN);
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_SIGN);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	////////////////////////////////////////////////////////////////
	//gdi leak
	CImageList *pImagelist=m_FileListCtr.GetImageList(LVSIL_SMALL);
	if(pImagelist)
		pImagelist->DeleteImageList();
	pImagelist=m_FileListCtr.GetImageList(LVSIL_STATE);
	if(pImagelist)
		pImagelist->DeleteImageList();
	//////////////////////////////////////////////////////////////////////////
	
	m_FileListCtr.SetImageList(pimagelist, LVSIL_SMALL);
	pimagelist=&m_stateImageList;
	pimagelist->Create(16, 16, ILC_COLOR24|ILC_MASK/*bMask*/,	2/*nInitial*/, 14/*nGrow*/);
	//bitmap.LoadBitmap(IDB_ATTACH);
	//有附件
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_ATTACH);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//未關聯的稿件
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_UNRELATEMENU);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	//有附件且未關聯的稿件
	bitmap.LoadBitmap(IDB_BITMAP_STATUS_ATTACHUNRELATEMENU);
	pimagelist->Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	m_FileListCtr.SetImageList(pimagelist,LVSIL_STATE);
}

void CDoColView::OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CString strFileCode,strFMT;
	int nListItem = m_FileListCtr.GetNextItem ( -1 , LVNI_SELECTED ) ;
	if(nListItem==-1)
	{
		CResManage setRes(g_hInst);
		AfxMessageBox(IDS_NOTSELITEM);
		return;
	}
	CDoColDoc *pDoc = (CDoColDoc*)GetDocument();
	pDoc->m_Mgr.OpenFile(TRUE);
	*pResult = 0;
}

void CDoColView::OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	UINT nNewState, nOldState;
	
	nNewState = (pNMListView->uNewState & (LVIS_FOCUSED|LVIS_SELECTED));
	nOldState = (pNMListView->uOldState & (LVIS_FOCUSED|LVIS_SELECTED));
	if (nNewState!=0&&nNewState != nOldState)
	{
		CDoColDoc *pDoc = (CDoColDoc*)GetDocument();
		if(pDoc->m_Mgr.m_bInitList||m_FileListCtr.m_bSorting)
			return;
		if(!pDoc->m_Mgr.GetCurFileParam())
		{
			//GetParent()->GetParent()->GetParent()->SendMessage(WM_FILESELCHANGED,(WPARAM)&pDoc->m_Mgr.m_strCurFileCode,0);
			GetParentFrame()->SendMessage(WM_FILESELCHANGED,(WPARAM)&pDoc->m_Mgr.m_strCurFileCode,0);
			return;
		}
		pDoc->m_Mgr.LoadColPerm() ;
		//GetParent()->GetParent()->GetParent()->SendMessage(WM_FILESELCHANGED,(WPARAM)&pDoc->m_Mgr.m_strCurFileCode,0);
		GetParentFrame()->SendMessage(WM_FILESELCHANGED,(WPARAM)&pDoc->m_Mgr.m_strCurFileCode,0);
	}
	*pResult = 0;
}

void CDoColView::OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nListItem = m_FileListCtr.GetNextItem ( -1 , LVNI_SELECTED ) ;
	if(nListItem != -1)
	{
		CDoColDoc *pDoc = (CDoColDoc*)GetDocument();
		pDoc->m_Mgr.TrackSubMenu(this);
	}
	*pResult = 0;
}

void CDoColView::OnRdoRecord() 
{
	// TODO: Add your command handler code here
	CDoColDoc *pDoc = (CDoColDoc*)GetDocument();
	pDoc->m_Mgr.ShowFileRecord();
}

void CDoColView::OnRdoSource() 
{
	// TODO: Add your command handler code here
	CString strFileCode;
	int nListItem = m_FileListCtr.GetNextItem ( -1 , LVNI_SELECTED ) ;
	if(nListItem==-1)
		return;
	strFileCode = m_FileListCtr.GetItemTextEx(nListItem,DOFILE_FILENAME);
	CGlobal::GetInstance()->ViewSource(strFileCode);
	
}

void CDoColView::OnRdoComment() 
{
	// TODO: Add your command handler code here
	CDoColDoc *pDoc = (CDoColDoc*)GetDocument();
	pDoc->m_Mgr.ShowFileInfo();
}

void CDoColView::OnRdoCopy() 
{
	// TODO: Add your command handler code here
	CDoColDoc *pDoc = (CDoColDoc*)GetDocument();
	pDoc->m_Mgr.ShowCopyInfo();
	
}

void CDoColView::OnUpdateRdoCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
}

void CDoColView::OnUnlock() 
{
	// TODO: Add your command handler code here
	CDoColDoc *pDoc = (CDoColDoc*)GetDocument();
	pDoc->m_Mgr.UnLockCurFile();
}

void CDoColView::OnPrintfile() 
{
	// TODO: Add your command handler code here
		CString strFileCode,strFMT;
	int nListItem = m_FileListCtr.GetNextItem ( -1 , LVNI_SELECTED ) ;
	if(nListItem==-1)
	{
		CResManage setRes(g_hInst);
		AfxMessageBox(IDS_NOTSELITEM);
		return;
	}
	CDoColDoc *pDoc = (CDoColDoc*)GetDocument();
	pDoc->m_Mgr.OpenFile(TRUE, FALSE, TRUE);
	
	
	SetFocus();
	m_FileListCtr.SelectItem(nListItem);
	
}

BOOL CDoColView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	HD_NOTIFY *pHD = (HD_NOTIFY*)lParam;
	if (pHD->hdr.code == HDN_ITEMCHANGING)
	{
		int nLastItem=m_FileListCtr.GetItemCount()-1;
		if (nLastItem >= 0)
			((CDoColDoc*)GetDocument())->m_Mgr.Refresh();
		/*if(m_FileListCtr.GetItemCount()>0)
			m_FileListCtr.SetColumnWidth(0,m_FileListCtr.GetColumnWidth(0));*/
	}
	*pResult = 0;
	return CFormView::OnNotify(wParam, lParam, pResult);
}

void CDoColView::OnAddToStory() 
{
	// TODO: Add your command handler code here
		CString strFileCode,strFMT;
	int nListItem = m_FileListCtr.GetNextItem ( -1 , LVNI_SELECTED ) ;
	if(nListItem==-1)
	{
		CResManage setRes(g_hInst);
		AfxMessageBox(IDS_NOTSELITEM);
		return;
	}
	CDoColDoc *pDoc = (CDoColDoc*)GetDocument();
	pDoc->m_Mgr.AddToStory();
	
	SetFocus();
	m_FileListCtr.SelectItem(nListItem);
	
}
