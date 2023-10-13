// DoFileView.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DoFileView.h"
#include "dofiledoc.h"
#include "dofileFrm.h"
#include "copylist.h"
#include "listitem.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDoFileView

IMPLEMENT_DYNCREATE(CDoFileView, CFormView)

CDoFileView::CDoFileView()
	: CFormView(CDoFileView::IDD)
{
	//{{AFX_DATA_INIT(CDoFileView)
	m_strFileEdit = _T("");
	m_bCheck = FALSE;
	//}}AFX_DATA_INIT
}

CDoFileView::~CDoFileView()
{
	m_FileListCtr.UpdateUserSetting();
	m_fontFace.DeleteObject();
}

void CDoFileView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDoFileView)
	DDX_Control(pDX, IDC_FILELIST, m_FileListCtr);
	DDX_Text(pDX, IDC_FILEEDIT, m_strFileEdit);
	DDX_Check(pDX, IDC_KEYCHECK, m_bCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDoFileView, CFormView)
	//{{AFX_MSG_MAP(CDoFileView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_KEYCHECK, OnKeycheck)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_EN_CHANGE(IDC_FILEEDIT, OnChangeFileedit)
	ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, OnDblclkFilelist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILELIST, OnItemchangedFilelist)
	ON_NOTIFY(NM_RCLICK, IDC_FILELIST, OnRclickFilelist)
	ON_COMMAND(ID_RDO_RECORD, OnRdoRecord)
	ON_COMMAND(ID_RDO_COMMENT, OnRdoComment)
	ON_COMMAND(ID_RDO_COPY, OnRdoCopy)
	ON_COMMAND(ID_RDO_SOURCE, OnRdoSource)
	ON_COMMAND(ID_UNLOCK, OnUnlock)
	ON_COMMAND(ID_PRINTFILE, OnPrintfile)
	ON_COMMAND(ID_ADD_TO_STORY, OnAddToStory)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_GETLISTCTRLTEXTCLR,OnGetListCtrlTextClr)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoFileView diagnostics

#ifdef _DEBUG
void CDoFileView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDoFileView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDoFileView message handlers

LRESULT CDoFileView::OnGetListCtrlTextClr(WPARAM wParam,LPARAM lParam)
{
	if(IsWindow(m_hWnd))
	{
		CString strUser,strFlow,strState;
		int nItem=int(lParam),nState=0;

		CDoFileDoc* pDoc=(CDoFileDoc *)(GetDocument());	
		CListItem *pItem=NULL;
		CString strFileCode=m_FileListCtr.GetItemTextEx(nItem,DOFILE_FILENAME);
		pItem = pDoc->m_Mgr.GetListItem(strFileCode);
		if(!pItem)return 0;
		strFlow = pItem->strLastOper;//m_FileListCtr.GetItemTextEx(nItem,DOFILE_CURSTATUS);
		strUser = pItem->strAssignedUser;//m_FileListCtr.GetItemTextEx(nItem,DOFILE_ASSIGNEDUSER);
		//strState = m_FileListCtr.GetItemTextEx(nItem,DOFILE_FILESTATE);
		nState = pItem->nState;//_ttol(strState);
		
		if((nState&FL_URGENCY) == FL_URGENCY)//急稿
			*((COLORREF*)wParam)=CGlobal::GetInstance()->m_pShareData->CLR_URGENT;
		else 
		{
			CString strSend, strWrite, strWires, strModify, strGetfile, strRollback;
			strSend.LoadString(IDS_SENDFILE);
			strWrite.LoadString(IDS_WRITE);
			strWires.LoadString(IDS_WIRESFILE);
			strModify.LoadString(IDS_MODIFY);
			strGetfile.LoadString(IDS_GETFILE);
			strRollback.LoadString(IDS_ROLLBACK);
			if((nState&FL_EXCLUSIVE) == FL_EXCLUSIVE)
				*((COLORREF*)wParam)=CGlobal::GetInstance()->m_pShareData->CLR_EXCLUSIVE;
			else if(strSend==strFlow)
			{
				CGlobal *pGlobal=CGlobal::GetInstance();
				if((strUser.Compare(pGlobal->m_pEmployee->m_strUserName)==0))
					*((COLORREF*)wParam)=CGlobal::GetInstance()->m_pShareData->CLR_SENDFILETOME;
				else
					*((COLORREF*)wParam)=CGlobal::GetInstance()->m_pShareData->CLR_SENDFILE;
			}
			else  if(strWrite==strFlow)
			{
				*((COLORREF*)wParam)=CGlobal::GetInstance()->m_pShareData->CLR_WRITE;
			}
			else  if(strWires==strFlow)
			{
				*((COLORREF*)wParam)=CGlobal::GetInstance()->m_pShareData->CLR_WIRES;
			}
			else  if(strModify==strFlow)
			{
				*((COLORREF*)wParam)=CGlobal::GetInstance()->m_pShareData->CLR_MODIFY;
			}
			else  if(strGetfile==strFlow)
			{
				*((COLORREF*)wParam)=CGlobal::GetInstance()->m_pShareData->CLR_GETFILE;
			}
			else  if(strRollback==strFlow)
			{
				CGlobal *pGlobal=CGlobal::GetInstance();
				*((COLORREF*)wParam)=CGlobal::GetInstance()->m_pShareData->CLR_ROLLBACK;
			}
			else
			{
				*((COLORREF*)wParam)=::GetSysColor(COLOR_WINDOWTEXT);
			}
		}
	}
	else
		*((COLORREF*)wParam) = -1;
	return 0;
}
void CDoFileView::InitList()
{
	CRect			rect;
	int				i ;
	LV_COLUMN		lvcolumn;
	CString			rgtsz[12] ;
#ifdef _STANDARD_VERSION_
	int	rgtcx[12] = { 85, 30, 120 , 50, 0, 30,30,40 ,40,70, 70};//DOFILE_HAVERELATE is 0
#else
	int	rgtcx[12] = { 85, 30, 120 , 50, 32, 30,30,40 ,40,70, 70};
#endif
	
	int	rgtcol[12] = {DOFILE_FILENAME,DOFILE_FILEFORMAT,DOFILE_TITLE,DOFILE_CURSTATUS, DOFILE_HAVERELATE,
		DOFILE_WORDCOUNT,DOFILE_FILEAUTHOR,DOFILE_ASSIGNEDUSER,DOFILE_LOCKUSER,DOFILE_COMMENT,DOFILE_UPDATEDATE};

	CResManage res(g_hInst);
	m_FileListCtr.m_hParent = this->m_hWnd;
	m_FileListCtr.SetFullRowSel(TRUE);
	m_FileListCtr.m_bSort=TRUE;

//	m_FileListCtr.m_nUserCol = DOFILE_ASSIGNEDUSER;
//	long lStyle = LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS;
	long lStyle = LVS_REPORT|LVS_SHOWSELALWAYS;
	long lStyleOld = GetWindowLong(m_FileListCtr.m_hWnd, GWL_STYLE);
	lStyleOld &= ~lStyle;  // turn off bits specified by caller.
	lStyleOld |= lStyle;
	SetWindowLong(m_FileListCtr.m_hWnd, GWL_STYLE, lStyleOld);
	m_FileListCtr.SetBkColor(CGlobal::GetInstance()->m_pShareData->CLR_NORMALBK);
	m_FileListCtr.SetTextBkColor(CGlobal::GetInstance()->m_pShareData->CLR_TEXTBK);	
	m_FileListCtr.SetTextBkShowMode(2,0,CGlobal::GetInstance()->m_pShareData->CLR_TEXTBKODD,CGlobal::GetInstance()->m_pShareData->CLR_TEXTBKEVEN);
	GetWindowRect(&rect);
	if (m_FileListCtr) m_FileListCtr.MoveWindow(0,45,rect.right-rect.left, rect.bottom-rect.top);
	rgtsz[0].LoadString(IDS_FILENAME);
	rgtsz[1].LoadString(IDS_FILEFORMAT);
	rgtsz[2].LoadString(IDS_TITLE);
	rgtsz[3].LoadString(IDS_CURSTATUS);
	rgtsz[4].LoadString(IDS_HAVERELATE);
	rgtsz[5].LoadString(IDS_WORDCOUNT);
	rgtsz[6].LoadString(IDS_FILEAUTHOR);
	rgtsz[7].LoadString(IDS_ASSIGNEDUSER);
	rgtsz[8].LoadString(IDS_LOCKUSER);
	rgtsz[9].LoadString(IDS_COMMENT);
	rgtsz[10].LoadString(IDS_UPDATEDATE);
	//rgtsz[8].LoadString(IDS_FILESTATE);
	
	//rgtsz[10].LoadString(IDS_LASTMOTIME);
	m_FileListCtr.GetWindowRect(&rect);

	//m_FileListCtr.InsertColumn(0,_T(""),LVCFMT_LEFT,18);
	//m_FileListCtr.InsertColumn(1,_T(""),LVCFMT_LEFT,18);
	// Insert ListCtrl Columns
	UINT uType=0;
	for (i = 0; i < 11; i++)  // add the columns to the list control
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		////Commented by Ronald Saliman on 20120920
		//if(i == DOFILE_WORDCOUNT)
		//	lvcolumn.fmt = LVCFMT_RIGHT;
		//else
		////end 20120920
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = (TCHAR *)(LPCTSTR)rgtsz[rgtcol[i]];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = rect.Width() * rgtcx[rgtcol[i]] / 280;  // SubItem is twice as large
		m_FileListCtr.InsertColumnEx(i,i, &lvcolumn);  // assumes return value is OK.
	}
	
	m_FileListCtr.EnableHeaderDrag(TRUE,2);
	m_FileListCtr.RegistCtrlID(DOFILE_LIST, DOFILE_LIST_VER);
	//return;
	m_FileListCtr.UpdateUserSetting(TRUE);
	for (i = 0; i < 11; i++)
		m_FileListCtr.SetColType(i,0);
	m_FileListCtr.SetColType(DOFILE_WORDCOUNT,1);
	m_FileListCtr.SetColType(DOFILE_UPDATEDATE,2);

}

void CDoFileView::OnInitialUpdate() 
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
	CDoFileDoc* pDoc=(CDoFileDoc *)(GetDocument());	
	pDoc->m_Mgr.m_pFileListCtrl=&m_FileListCtr;
	pDoc->m_Mgr.m_pParentFrm = (CMDIChildWnd*)(GetParent()->GetParent()->GetParent());
	InitImageList(&m_FileListCtr);
/*	m_FileListCtr.SetExtendedStyle(m_FileListCtr.GetExtendedStyle()|
		LVS_EX_FULLROWSELECT);*/
	m_bCheck = pDoc->m_Mgr.m_enQueryMod==BYFILENAME;
	(CWnd*)GetDlgItem(IDC_FILEEDIT)->EnableWindow(m_bCheck);

	UpdateData(FALSE);
}

void CDoFileView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
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

void CDoFileView::OnKeycheck() 
{
	// TODO: Add your control notification handler code here
	CDoFileDoc* pDoc=(CDoFileDoc *)(GetDocument());	

	if ( IsDlgButtonChecked ( IDC_KEYCHECK ))
	{
		(CWnd*)GetDlgItem(IDC_FILEEDIT)->EnableWindow(TRUE);
		SetDlgItemText(IDC_FILEEDIT,_T(""));
		SetDlgItemText ( IDC_TOTALFILE ,_T(""));
		pDoc->m_Mgr.m_enQueryMod = BYFILENAME;
		m_FileListCtr.DeleteAllItems();
	}
	else
	{
		(CWnd*)GetDlgItem(IDC_FILEEDIT)->EnableWindow(FALSE);
		pDoc->m_Mgr.m_enQueryMod = BYPLACE;
		ListFile();
	}	
}

void CDoFileView::ListFile()
{
//	CDoFileDoc* pDoc=(CDoFileDoc *)(GetDocument());	
//	pDoc->m_Mgr.LoadLibFile();

	CDoFileFrm *pFrm =(CDoFileFrm*) GetParentFrame();
	pFrm->RefreshFile();
}

void CDoFileView::InitImageList(CListCtrl *pListCtrl)
{
	CImageList	*pimagelist;
	pimagelist=&m_ImageList;
	CBitmap				bitmap;
	pimagelist->Create(16, 16, ILC_COLOR24|ILC_MASK/*bMask*/,	14/*nInitial*/, 14/*nGrow*/)
		;
	CResManage setRes(g_hInst);
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

	pListCtrl->SetImageList(pimagelist, LVSIL_SMALL);
	pimagelist=&m_stateImageList;
	pimagelist->Create(16, 16, ILC_COLOR24|ILC_MASK/*bMask*/,	2/*nInitial*/, 14/*nGrow*/);
	//bitmap.LoadBitmap(IDB_ATTACH);
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
	pListCtrl->SetImageList(pimagelist,LVSIL_STATE);
}

void CDoFileView::OnQuery() 
{
	// TODO: Add your control notification handler code here
	ListFile();
}

void CDoFileView::OnChangeFileedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
	CDoFileDoc* pDoc=(CDoFileDoc *)(GetDocument());	
	pDoc->m_Mgr.m_strCurFileCode=m_strFileEdit;	
}

void CDoFileView::OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
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
	CDoFileDoc *pDoc = (CDoFileDoc*)GetDocument();
	pDoc->m_Mgr.OpenFile(TRUE);
	*pResult = 0;
}

void CDoFileView::OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	UINT nNewState, nOldState;
	
	nNewState = (pNMListView->uNewState & (LVIS_FOCUSED|LVIS_SELECTED));
	nOldState = (pNMListView->uOldState & (LVIS_FOCUSED|LVIS_SELECTED));
	if (nNewState!=0&&nNewState != nOldState)
	{
		
		CDoFileDoc *pDoc = (CDoFileDoc*)GetDocument();
		if(pDoc->m_Mgr.m_bInitList||m_FileListCtr.m_bSorting)
			return;
		if(!pDoc->m_Mgr.GetCurFileParam())
		{
			GetParent()->GetParent()->GetParent()->SendMessage(WM_FILESELCHANGED,(WPARAM)&pDoc->m_Mgr.m_strCurFileCode,0);
			return;
		}
		pDoc->m_Mgr.CheckLibFilePermission() ;
		GetParent()->GetParent()->GetParent()->SendMessage(WM_FILESELCHANGED,(WPARAM)&pDoc->m_Mgr.m_strCurFileCode,0);
	}
	*pResult = 0;
}

void CDoFileView::OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nListItem = m_FileListCtr.GetNextItem ( -1 , LVNI_SELECTED ) ;
	if(nListItem!=-1)
	{
		CDoFileDoc *pDoc = (CDoFileDoc*)GetDocument();
		pDoc->m_Mgr.TrackSubMenu(this);
	}
	*pResult = 0;
}

void CDoFileView::OnRdoRecord() 
{
	// TODO: Add your command handler code here
	CDoFileDoc *pDoc = (CDoFileDoc*)GetDocument();
	pDoc->m_Mgr.ShowFileRecord();
}

void CDoFileView::OnRdoComment() 
{
	// TODO: Add your command handler code here
	CDoFileDoc *pDoc = (CDoFileDoc*)GetDocument();
	pDoc->m_Mgr.ShowFileInfo();
}

void CDoFileView::OnRdoCopy() 
{
	// TODO: Add your command handler code here
	CDoFileDoc *pDoc = (CDoFileDoc*)GetDocument();
	pDoc->m_Mgr.ShowCopyInfo();
	
}

void CDoFileView::OnRdoSource() 
{
	// TODO: Add your command handler code here
	CString strFileCode;
	int nListItem = m_FileListCtr.GetNextItem ( -1 , LVNI_SELECTED ) ;
	if(nListItem==-1)
		return;
	CDoFileDoc *pDoc = (CDoFileDoc*)GetDocument();
	
	//strFileCode = m_FileListCtr.GetItemTextEx(nListItem,DOFILE_FILENAME);
	CGlobal::GetInstance()->ViewSource(pDoc->m_Mgr.m_strCurFileCode);
}

/*
void CDoFileView::OnBtnprint() 
{
	// TODO: Add your command handler code here
	CDoFileDoc* pDoc=(CDoFileDoc *)(GetDocument());	
	pDoc->m_Mgr.PrintView();
	
}

void CDoFileView::OnUpdateBtnprint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
	
}
*/

void CDoFileView::OnUnlock() 
{
	// TODO: Add your command handler code here
	CDoFileDoc* pDoc=(CDoFileDoc *)(GetDocument());	
	pDoc->m_Mgr.UnLockCurFile();
}

void CDoFileView::OnPrintfile() 
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
	CDoFileDoc *pDoc = (CDoFileDoc*)GetDocument();
	pDoc->m_Mgr.OpenFile(TRUE, FALSE, TRUE);
	
	SetFocus();
	m_FileListCtr.SelectItem(nListItem);
}

BOOL CDoFileView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	HD_NOTIFY *pHD = (HD_NOTIFY*)lParam;
	if (pHD->hdr.code == HDN_ITEMCHANGING)
	{
		int nLastItem=m_FileListCtr.GetItemCount()-1; 
		if (nLastItem >= 0)
			((CDoFileDoc*)GetDocument())->m_Mgr.Refresh();		
		/*if(m_FileListCtr.GetItemCount()>0)
			m_FileListCtr.SetColumnWidth(0,m_FileListCtr.GetColumnWidth(0));*/
	}
	*pResult = 0;
	return CFormView::OnNotify(wParam, lParam, pResult);
}

void CDoFileView::OnAddToStory() 
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

	CDoFileDoc *pDoc = (CDoFileDoc*)GetDocument();
	pDoc->m_Mgr.AddToStory();
	
	SetFocus();
	m_FileListCtr.SelectItem(nListItem);
	
}