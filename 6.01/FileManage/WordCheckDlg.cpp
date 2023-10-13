// WordCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "WordCheckDlg.h"
#include <mbstring.h>
#include "SaveProg.h"

// #include "editDoc.h"
// #include "TransDlg.h"
// #include "dofilemgr.h"
// #include "fileinfodlg.h"
// #include "..\commkits\dostools.h"
#include "editview_bottom.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CWordCheckDlg dialog


CWordCheckDlg::CWordCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWordCheckDlg::IDD, pParent)
{
	
	//edit by leon 2012-09-04 for word bank
	m_nWordLength=0;
	m_nUseWordLength=0;
	m_nCurPos=0;
	m_nPos1=0;
	m_nPos2=0;
	m_nDicCount=0;
	m_strReplaceWord=L"";
	m_nReplacePos=0;
	m_bChangeAllDef=FALSE;
	m_bCheckDefFirst=FALSE;
	//{{AFX_DATA_INIT(CWordCheckDlg)
	m_DicText = _T("");
	//}}AFX_DATA_INIT
//	m_strClass=TEXT("RichEdit20W");
	LoadLibrary(TEXT("msftedit.dll"));
	 
}



WORDITEM::WORDITEM(){
	bValid=TRUE;
	strName=L"";
	nPosition=0;
}

void CWordCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWordCheckDlg)
	DDX_Control(pDX, IDC_RICH_TEXT, m_richCrl);
	DDX_Control(pDX, IDC_LIB_LIST, m_wordList);
	DDX_Control(pDX, IDC_REPLACE_LIST, m_ctrList);
	DDX_Text(pDX, IDC_RICH_TEXT, m_richText);
	DDX_LBString(pDX, IDC_LIB_LIST, m_DicText);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RICHEDIT21, m_rtcShowMask);
}


BEGIN_MESSAGE_MAP(CWordCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CWordCheckDlg)
	ON_BN_CLICKED(IDC_IGNORE_Once, OnIgnoreOnce)
	ON_BN_CLICKED(IDC_IGNORE_ALL, OnIgnoreAll)
	ON_BN_CLICKED(IDC_CHANGE, OnChangeWord)
	ON_BN_CLICKED(IDC_CHANGE_ALL, OnChangeAll)
	ON_EN_SETFOCUS(IDC_EDIT_TEXT, OnSetfocusEditText)
	ON_BN_CLICKED(IDC_DICTIONARY, OnChangeDictionary)
	ON_LBN_DBLCLK(IDC_REPLACE_LIST, OnDblclkReplaceList)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//ON_BN_CLICKED(IDC_CHANGE_ALL_DEF, OnChangeAllDef)
    ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_EN_SETFOCUS(IDC_RICHEDIT21, &CWordCheckDlg::OnEnSetfocusRichedit21)
	ON_BN_CLICKED(IDC_FINISH, &CWordCheckDlg::OnBnClickedFinish)
END_MESSAGE_MAP()



// Note: we add support for IID_IWordCheckDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {EDC7B1DD-BE53-4789-966A-0868A7B31B26}


/////////////////////////////////////////////////////////////////////////////
// CWordCheckDlg message handlers

BOOL CWordCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
//	UpdateData(TRUE);
	CGlobal *pGL=CGlobal::GetInstance();	
	m_richCrl.HideSelection(FALSE, TRUE);
	m_richCrl.ModifyStyle(0,ES_MULTILINE |ES_AUTOVSCROLL |ES_WANTRETURN); 
	m_richCrl.SetWordWrapMode(WBF_WORDWRAP);
	m_richCrl.SetReadOnly();
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
 	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();
	CDocument* pDoc = pChild->GetActiveDocument(); 
	POSITION pos = pDoc->GetFirstViewPosition(); 
	GetDlgItem(IDC_FINISH)->ShowWindow(FALSE);
	while (pos != NULL)   
	{   
		pView = (editView_Bottom *)pDoc->GetNextView(pos);   
		if (pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))   
		{   
			//pView->GetParentFrame()->ActivateFrame();   
			break;   
		}   
	}   	 
	pView->GetWindowText(m_richText);
	m_strShowText = m_richText;
	m_strMask = L"";
	iTextLenAdded = 0;
	for(int i=0;i<m_strShowText.GetLength();i++)
	{
		if(m_strShowText.GetAt(i) == L'\r')
			continue;
		m_strMask +="a";
	}
	m_OrgText=m_richText;
	m_rtcShowMask.ModifyStyle(0,ES_MULTILINE |ES_AUTOVSCROLL |ES_WANTRETURN); 
	m_rtcShowMask.SetWordWrapMode(WBF_WORDWRAP);
	m_rtcShowMask.SetReadOnly();
	m_rtcShowMask.SetWindowTextW(m_strShowText);
	m_rtcShowMask.SetSel(0,1);

	int nPos1=0,nPos2=0;	
	CString str,temp;
	int bGroup=0;
	for (int i=0;i<m_DicText.GetLength();i++)
	{
		str=m_DicText.Mid(i,1);
		if (!lstrcmp(str,L"'")){
			if (bGroup==0)
				bGroup=1;
			else if(bGroup==1){
				bGroup=0;
				m_wordList.AddString(temp);
				temp=L"";
			}
			continue;
		}
		if (bGroup==1)
			temp+=str;
	}
	
	CFont m_fontFace;
	LOGFONT logfont;
	wchar_t sz_temp[255];
	logfont.lfHeight = GetPrivateProfileIntW(L"EditProTextStyle1" , L"Height",-15,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfWidth  = GetPrivateProfileIntW(L"EditProTextStyle1" , L"Width",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfEscapement = GetPrivateProfileIntW(L"EditProTextStyle1" , L"Escapement",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfOrientation = GetPrivateProfileIntW(L"EditProTextStyle1" , L"Orientation",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfWeight = GetPrivateProfileIntW(L"EditProTextStyle1" , L"Weight",400,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfItalic = GetPrivateProfileIntW(L"EditProTextStyle1" , L"Italic",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfUnderline = GetPrivateProfileIntW(L"EditProTextStyle1" , L"Underline",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfStrikeOut = GetPrivateProfileIntW(L"EditProTextStyle1" , L"StrikeOut",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfCharSet = GetPrivateProfileIntW(L"EditProTextStyle1" , L"CharSet",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfOutPrecision = GetPrivateProfileIntW(L"EditProTextStyle1" , L"OutPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfClipPrecision = GetPrivateProfileIntW(L"EditProTextStyle1" , L"ClipPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfQuality = GetPrivateProfileIntW(L"EditProTextStyle1" , L"Quality",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	logfont.lfPitchAndFamily = GetPrivateProfileIntW(L"EditProTextStyle1" , L"PitchAndFamily",49,((CWinApp*)AfxGetApp())->m_pszProfileName);
	GetPrivateProfileStringW(L"WordCheck", L"FaceName", L"",sz_temp,255, ((CWinApp*)AfxGetApp())->m_pszProfileName);
	if (wcscmp(sz_temp,L"") == 0)
		GetPrivateProfileStringW(L"EditProTextStyle1" , L"FaceName",L"Courier New",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	wcscpy(logfont.lfFaceName, sz_temp);
	m_fontFace.CreateFontIndirect(&logfont);

	m_richCrl.SetFont(&m_fontFace);
	m_ctrList.SetFont(&m_fontFace);
	m_rtcShowMask.SetFont(&m_fontFace);

	UpdateData(FALSE);
	FillUseWordList(m_richText);
	if (!m_bCheckDefFirst)
	{
		HightLightWord();	
		SetReplaceWordPos();
	}
	else	
		OnChangeAllDef();

	CRect rect;
	GetClientRect(&rect);
	m_pt.x=rect.right-rect.left;
	m_pt.y=rect.bottom-rect.top;
	int xSize = ::GetSystemMetrics (SM_CXSCREEN);
	int ySize = ::GetSystemMetrics (SM_CYSCREEN);
	SetWindowPos(NULL, 0, 0, xSize,ySize,0);

	EnableToolTips(TRUE);
	m_toolTip.Create(this); 
	m_toolTip.Activate(TRUE);
    m_toolTip.AddTool(GetDlgItem(IDC_IGNORE_Once),L"略過一次(&I)");
    m_toolTip.AddTool(GetDlgItem(IDC_IGNORE_ALL),L"全部略過(&G)");
    m_toolTip.AddTool(GetDlgItem(IDC_CHANGE),L"變更(&C)");
    m_toolTip.AddTool(GetDlgItem(IDC_CHANGE_ALL),L"全部變更(&L)");
    m_toolTip.AddTool(GetDlgItem(IDC_CANCEL),L"取消");

	::SendMessage(m_richCrl.GetSafeHwnd(), EM_SETLANGOPTIONS, 0, 0);
	::SendMessage(m_rtcShowMask.GetSafeHwnd(), EM_SETLANGOPTIONS, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWordCheckDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	float fx,fy;
	POINT ptNew;
	CRect recta;    
	GetClientRect(&recta);  
	ptNew.x = recta.right - recta.left;
	ptNew.y = recta.bottom - recta.top;
	fx = (float)ptNew.x / m_pt.x;
	fy = (float)ptNew.y / m_pt.y;
	CRect rc;
	int nid, nButtonL;
	CPoint ptOldTL, ptTL, ptOldBR, ptBR;
	HWND  hwndChild = ::GetWindow(m_hWnd,GW_CHILD);

	while(hwndChild)
	{
		nid = ::GetDlgCtrlID(hwndChild);
		GetDlgItem(nid)->GetWindowRect(rc);
		ScreenToClient(rc);
		ptOldTL = rc.TopLeft();
		ptOldBR = rc.BottomRight();
		if(nid == IDC_IGNORE_Once || nid == IDC_IGNORE_ALL || nid == IDC_CHANGE || nid == IDC_CHANGE_ALL || nid == IDC_CANCEL)
		{
			ptTL.x = long(ptOldTL.x * fx);
			ptTL.y = ptOldTL.y + (ptNew.y - m_pt.y)*6/7;
			ptBR.x = ptOldBR.x - ptOldTL.x + ptTL.x;
			ptBR.y = ptOldBR.y - ptOldTL.y + ptTL.y;
		}
		else if (nid == IDC_REPLACE_LIST || nid == IDC_STATIC || nid == IDC_DIM2)
		{
			ptTL.x = long(ptOldTL.x * fx);
			ptTL.y = ptOldTL.y + (ptNew.y - m_pt.y)*6/7;
			ptBR.x = long(ptOldBR.x * fx);
			ptBR.y = ptOldBR.y + ptNew.y - m_pt.y;
		}
		else if (nid == IDC_LIB_LIST || nid == IDC_STATIC2 || nid == IDC_DIM1)
		{
			ptTL.x = ptOldTL.x - ptOldTL.x + long(ptOldTL.x * fx);
			ptTL.y = ptOldTL.y + (ptNew.y - m_pt.y)*6/7;
			ptBR.x = long(ptOldBR.x * fx);
			ptBR.y = ptOldBR.y + ptNew.y - m_pt.y;
		}
		else if (nid == IDC_RICH_TEXT )
		{
			/*ptTL.x = long(ptOldTL.x * fx);
			ptTL.y = long(ptOldTL.y * fy);
			ptBR.x = long(ptOldBR.x * fx);
			ptBR.y = ptOldBR.y + (ptNew.y - m_pt.y)*6/7;*/
			ptTL.x = 0;
			ptTL.y = 0;
			ptBR.x = 1;
			ptBR.y = 1;
		}
		else if (nid == IDC_RICHEDIT21)
		{
			ptTL.x = long(ptOldTL.x * fx);
			ptTL.y = long(ptOldTL.y * fy);
			ptBR.x = long(ptOldBR.x * fx);
			ptBR.y = ptOldBR.y + (ptNew.y - m_pt.y)*6/7;
		}else if(nid == IDC_FINISH)
		{
			ptTL.x = long(ptOldTL.x * fx);
			ptTL.y = ptOldTL.y + (ptNew.y - m_pt.y)*6/7;
			ptBR.x = ptOldBR.x - ptOldTL.x + ptTL.x;
			ptBR.y = ptOldBR.y - ptOldTL.y + ptTL.y;
		}
		rc.SetRect(ptTL, ptBR);
		GetDlgItem(nid)->MoveWindow(rc);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

void CWordCheckDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	m_nUseWordLength=0;
	m_arrUseWordRecord.RemoveAll();
	HightLightWord();
}

void CWordCheckDlg::OnIgnoreOnce() 
{
	// TODO: Add your control notification handler code here
	if (m_nUseWordLength<=0)
		return;		
	m_nUseWordLength--;
	m_arrUseWordRecord.RemoveAt(0);
	m_nCurPos=0;
	HightLightWord();
	SetReplaceWordPos();	
}

//for fill the list of words to be use and sort it
void CWordCheckDlg::FillUseWordList(CString strContent){
	CGlobal *pGL=CGlobal::GetInstance();
	CString tmp;
	int i=0;
	OnReset();
	//SetLibGroup(strLibGroup);
	m_nWordLength=pGL->m_FileDB.LoadWordInfo(&m_arrSqlWordRecord,m_DicText);
	int nCurPos=0,nNextPos=0,nMinNum,nArrLength=0;
	ARRAY_WORDINFO arrTmpList;
	for (i=0;i<m_nWordLength;i++)
	{
		WORDITEM wItem;
		wItem=m_arrSqlWordRecord.GetAt(i);
		nCurPos=strContent.Find(wItem.strName,0);
		if (nCurPos<0 || wItem.strName.GetLength()==0)
		{
			continue;
		}
		tmp=wItem.strName;
		while (nCurPos>=0)
		{
			wItem.nPosition=nCurPos;
			arrTmpList.Add(wItem);
			nNextPos=nCurPos+lstrlen(tmp);
			nCurPos=strContent.Find(tmp,nNextPos);
			m_nUseWordLength++;
		}
		
	}
	nArrLength=m_nUseWordLength;
	
	
	for(i=0;i<m_nUseWordLength;i++){
	nCurPos=0;
	nMinNum=arrTmpList.GetAt(0).nPosition;
	for(int j=0;j<nArrLength;j++){
		if(nMinNum>arrTmpList.GetAt(j).nPosition){
			nCurPos=j;
			nMinNum=arrTmpList.GetAt(j).nPosition;
		}
	}
	nArrLength--;
	m_arrUseWordRecord.Add(arrTmpList.GetAt(nCurPos));
	arrTmpList.RemoveAt(nCurPos);
	}
}

void CWordCheckDlg::HightLightWord(){
	if (m_nUseWordLength==0 && !m_bChangeAllDef)
	{
		//AfxMessageBox(L"檢查完畢");
		m_ctrList.ResetContent();
		m_richCrl.SetSel(0,0);
		if (!lstrcmp(m_OrgText,m_richText))
		{
			AfxMessageBox(L"檢查完畢,稿件無更新");
			CDialog::OnOK();
			return;
		}
		m_rtcShowMask.ShowWindow(TRUE);
		GetDlgItem(IDC_DIM2)->ShowWindow(TRUE);
		GetDlgItem(IDC_DIM1)->ShowWindow(TRUE);
		m_rtcShowMask.SetBackgroundColor(FALSE, RGB(212,208,200));
		((CRichEditCtrl*)GetDlgItem(IDC_DIM2))->SetBackgroundColor(FALSE, RGB(212,208,200));
		((CRichEditCtrl*)GetDlgItem(IDC_DIM1))->SetBackgroundColor(FALSE, RGB(212,208,200));
		m_rtcShowMask.SetSel(0,0);
		CFont m_font;
		m_font.CreateFont(10, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
		GetDlgItem(IDC_FINISH)->SendMessage(WM_SETFONT, WPARAM(HFONT(m_font)), 0);
		GetDlgItem(IDC_FINISH)->ShowWindow(TRUE);
		GetDlgItem(IDC_IGNORE_Once)->ShowWindow(FALSE);
		GetDlgItem(IDC_IGNORE_ALL)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHANGE)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHANGE_ALL)->ShowWindow(FALSE);
		
		m_rtcShowMask.LockWindowUpdate();
		m_rtcShowMask.HideSelection(TRUE, FALSE);
		CHARFORMAT2 cf;
		for(int i=0;i<m_strMask.GetLength();i++)
		{
			m_rtcShowMask.SetSel(i, i+1);
			
			cf.cbSize = sizeof(CHARFORMAT);
			cf.dwMask = CFM_COLOR;
			m_rtcShowMask.GetSelectionCharFormat(cf);
			cf.crBackColor = RGB(212,208,200);
			cf.dwMask= CFM_BACKCOLOR;
			if(m_strMask.GetAt(i) == L'd')
			{
				cf.crTextColor = RGB(255, 0, 0);
				cf.dwMask= CFM_COLOR;
				cf.dwEffects ^= CFE_AUTOCOLOR;
			}
			else 
			{
				if(m_strMask.GetAt(i) == L'd')
					cf.crTextColor = RGB(0, 0, 255);
				else
					cf.crTextColor = RGB(0, 0, 0);
				cf.dwEffects ^= CFE_AUTOCOLOR;
			}
			m_rtcShowMask.SetSelectionCharFormat(cf);
		}
		m_rtcShowMask.HideSelection(FALSE, FALSE);
		m_rtcShowMask.UnlockWindowUpdate();
		m_rtcShowMask.HideSelection(TRUE, FALSE);
		
//		if( MessageBoxW( L"檢查完畢,是否更新稿件?" , 
//			L"字詞檢查系統", MB_YESNO | MB_ICONQUESTION ) == IDYES ){
//		//	GetParent()->SendMessage(WM_COTENTCHANGED,0,0);
//
//			CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
//// 			// Get the active MDI child window.
// 			CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
//			editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();
//			CDocument* pDoc = pChild->GetActiveDocument(); 
//			POSITION pos = pDoc->GetFirstViewPosition(); 
//			while (pos != NULL)   
//			{   
//				pView = (editView_Bottom *)pDoc->GetNextView(pos);   
//				if (pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))   
//				{   
//					//pView->GetParentFrame()->ActivateFrame();   
//					break;   
//				}   
//			}   
//			pView->SetWindowText(m_richText);
//			pView->WordBankReplace(&m_arrReplaceWord);
//			//pView->ReplaceWord(m_arrReplaceWord);
//			
//		}
//		CDialog::OnOK();
//		return;
	}
	else if(m_nUseWordLength==0 && m_bChangeAllDef)
	{
		m_ctrList.ResetContent();
		m_richCrl.SetSel(0,0);
		return;
	}
	CGlobal *pGL=CGlobal::GetInstance();
//	CString strLibGroup;
//	SetLibGroup(strLibGroup);
	if(m_arrUseWordRecord.GetCount() == 0)
		return;
	pGL->m_FileDB.LoadDictionary(m_arrUseWordRecord.GetAt(m_nCurPos).strName,&m_ctrList,m_DicText);
	if (m_ctrList.GetCount() == 0)
	{
		OnIgnoreOnce();
		return;
	}
	int Pos1,Pos2;
	CString strName,strTmp1,strTmp2;
	strName=m_arrUseWordRecord.GetAt(m_nCurPos).strName;
	Pos1=m_arrUseWordRecord.GetAt(m_nCurPos).nPosition;
	Pos2=Pos1+lstrlen(strName);	
	CHARFORMAT cf;
	m_richCrl.GetDefaultCharFormat(cf);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;
	

	cf.crTextColor = RGB(0,0,0);	
	m_richCrl.SetSel(0,m_richText.GetLength());
//	m_richCrl.HideSelection(FALSE, TRUE);	
	m_richCrl.SetSelectionCharFormat(cf);	
//	m_richCrl.HideSelection(TRUE, TRUE);

	int nReturnPos=0,nReturnCount=0,nCurPos;
	nReturnPos=m_richText.Find(L"\r\n",0);

	if (nReturnPos==0)
	{
		nReturnCount++;
		nReturnPos+=2;
	//	nCurPos=nReturnPos;
		while (nReturnPos==m_richText.Find(L"\r\n",nReturnPos))
		{
			nReturnPos+=2;
			nReturnCount++;
		}
		nReturnPos=m_richText.Find(L"\r\n",nReturnPos);
	}
	while (nReturnPos>0 && nReturnPos<Pos1)
	{
		nCurPos=nReturnPos+2;
		nReturnCount++;
		nReturnPos=m_richText.Find(L"\r\n",nCurPos);
	}
	cf.crTextColor = RGB(255,0,0);	
	int a=Pos1-nReturnCount;
	int b=Pos2-nReturnCount;
	m_richCrl.SetSel(Pos1-nReturnCount,0);
	m_richCrl.SetSel(Pos1-nReturnCount,Pos2-nReturnCount);
	m_richCrl.HideSelection(FALSE, TRUE);	
	m_richCrl.SetSelectionCharFormat(cf);
//	if (m_richCrl.GetFirstVisibleLine()>0)
//		m_richCrl.LineScroll((m_richCrl.LineFromChar(m_richCrl.LineIndex(nCurPos))-m_richCrl.GetFirstVisibleLine())/2);
	m_richCrl.HideSelection(TRUE, TRUE);
	ClearHightLight();
	MarkColorOnShowRTC();
}

void CWordCheckDlg::OnIgnoreAll() 
{
	// TODO: Add your control notification handler code here
	if (m_nUseWordLength<=0)
		return;	
	CString strContent;
	m_richCrl.SetSel(0,0);
	strContent=m_arrUseWordRecord.GetAt(0).strName;
	for (int i=0;i<m_nUseWordLength;i++)
	{
		if (!lstrcmp(strContent,m_arrUseWordRecord.GetAt(i).strName))
		{
			m_arrUseWordRecord.RemoveAt(i);
			m_nUseWordLength--;
			i--;
		}
	}
	m_nCurPos=0;
	HightLightWord();

}

int CharIndexToMultiIndex(CString strSrc,int nIndex){
	int nLen = strSrc.GetLength();
	int nTotalLen=0;
	CString str;
	if(nIndex < 0)
		return -1;
	if(nLen <= nIndex )
		nIndex = nLen;
	CString strText = strSrc.Left(nIndex);
	for (int i=0;i<=strText.GetLength();i++)
	{
		if (!lstrcmp(strText.Mid(i,1),L"一"))
		{
			nTotalLen+=2;
		}
		else nTotalLen+=_mbstrlen(( char*)(LPCTSTR)strText.Mid(i,1));
	}
	nLen = nTotalLen;
	return nTotalLen;
}

void CWordCheckDlg::OnChangeWord() 
{
	// TODO: Add your control notification handler code here

	if (m_nUseWordLength<=0)
		return;	
	ReplaceText();
	UpdateData(FALSE);
	m_nCurPos=0;
	HightLightWord();
	SetReplaceWordPos();
//	UpdateData(FALSE);
}

void CWordCheckDlg::ResetPosition(int nLength,BOOL bReset){
	if (bReset)
	{
		for(int i=0;i<m_arrUseWordRecord.GetSize();i++){
			WORDITEM wItem;
			int nCurPos;
			wItem=m_arrUseWordRecord.GetAt(i);
			nCurPos=wItem.nPosition;
			nCurPos=m_richText.Find(m_arrUseWordRecord.GetAt(i).strName,nCurPos);
//			nCurPos=m_editText.Find(m_arrUseWordRecord.GetAt(i).strName,nCurPos);
			wItem.nPosition=nCurPos;
			m_arrUseWordRecord.SetAt(i,wItem);
		}
	}
	else{
		for(int i=m_nPos1;i<m_nUseWordLength;i++){
			WORDITEM wItem;
			int nCurPos;
			wItem=m_arrUseWordRecord.GetAt(i);
			nCurPos=wItem.nPosition;
			nCurPos+=nLength;
			wItem.nPosition=nCurPos;
			m_arrUseWordRecord.SetAt(i,wItem);
		}
	}
	m_nPos1=0;
}

void CWordCheckDlg::ReplaceText(BOOL bReplace){
	CString str,strSelect,strFirstHalf,strSecondHalf;
	REPLACEWORDITEM rpWordItem;
 	int nIndex = m_ctrList.GetCurSel();
 		if (nIndex<0) 
		{
			AfxMessageBox(L"請選擇要替換的文字");
			return;
		}
	m_ctrList.GetText( nIndex, strSelect );
	m_nReplacePos=nIndex;
	int nPos1,nPos2,nNameLength,nStartPos;
	nStartPos=m_arrUseWordRecord.GetAt(0).nPosition;
	if (bReplace)
	{
		str=m_arrUseWordRecord.GetAt(0).strName;
		nNameLength=lstrlen(str);
		for (int i=0;i<m_nUseWordLength;i++)
		{
		//	AfxMessageBox(m_arrUseWordRecord.GetAt(i).strName);
			if (!lstrcmp(m_arrUseWordRecord.GetAt(i).strName,str))
			{
				nPos1=m_arrUseWordRecord.GetAt(i).nPosition;
				nPos2=lstrlen(m_richText)-nPos1-nNameLength;
				//edit by leon for replace word in editor
				rpWordItem.nPosition=nPos1;
				rpWordItem.strOrgText=m_arrUseWordRecord.GetAt(i).strName;
				rpWordItem.strTrgText=strSelect;
				m_arrReplaceWord.Add(rpWordItem);
				//end
				MarkReplacedMask(nPos1,m_arrUseWordRecord.GetAt(i).strName, strSelect);

				strFirstHalf=m_richText.Left(nPos1);
				strSecondHalf=m_richText.Right(nPos2);
				if (nPos2==0)
				{
					m_richText=strFirstHalf+strSelect;

				//	m_editText=strFirstHalf+strSelect;
				}
				else {
					m_richText=strFirstHalf+strSelect+strSecondHalf;

				//	m_editText=strFirstHalf+strSelect;
				}
				m_arrUseWordRecord.RemoveAt(i);
				
				m_nUseWordLength--;
				m_nPos1=i;
				i--;
			//	if (i<0)
			//		m_nPos1=0;
			//	else m_nPos1=i;
				ResetPosition(lstrlen(strSelect)-nNameLength,FALSE);
				
				while(m_nUseWordLength > 0)
				{
					if(m_arrUseWordRecord.GetAt(m_nPos1).nPosition < nStartPos+strSelect.GetLength())
					{
						m_arrUseWordRecord.RemoveAt(m_nPos1);
						m_nUseWordLength--;
					}
					else
						break;
				}
			}
		}

	}
	else{
		nPos1=m_arrUseWordRecord.GetAt(0).nPosition;
		nNameLength=lstrlen(m_arrUseWordRecord.GetAt(0).strName);
		nPos2=lstrlen(m_richText)-nPos1-nNameLength;
		strFirstHalf=m_richText.Left(nPos1);
 		strSecondHalf=m_richText.Right(nPos2);
		m_richText=strFirstHalf+strSelect+strSecondHalf;

		//edit by leon for replace word in editor
		rpWordItem.nPosition=nPos1;
		rpWordItem.strOrgText=m_arrUseWordRecord.GetAt(0).strName;
		rpWordItem.strTrgText=strSelect;
		m_arrReplaceWord.Add(rpWordItem);
		//end
		MarkReplacedMask(nPos1,m_arrUseWordRecord.GetAt(0).strName, strSelect);

//		nPos2=lstrlen(m_editText)-nPos1-nNameLength;
//		strFirstHalf=m_editText.Left(nPos1);
//		strSecondHalf=m_editText.Right(nPos2);
//		m_editText=strFirstHalf+strSelect+strSecondHalf;
		m_arrUseWordRecord.RemoveAt(0);
		m_nUseWordLength--;
		ResetPosition(lstrlen(strSelect)-nNameLength,bReplace);		
		while(m_nUseWordLength > 0)
		{
			if(m_arrUseWordRecord.GetAt(0).nPosition < nPos1+strSelect.GetLength())
			{
				m_arrUseWordRecord.RemoveAt(0);
				m_nUseWordLength--;
			}
			else
				break;
		}
	}
}

void CWordCheckDlg::OnChangeAll() 
{
	// TODO: Add your control notification handler code here
	if (m_nUseWordLength<=0)
		return;	
	ReplaceText(TRUE);
	UpdateData(FALSE);
	m_nCurPos=0;
	HightLightWord();
	SetReplaceWordPos();
}



void CWordCheckDlg::OnSetfocusEditText() 
{
	// TODO: Add your control notification handler code here
	  ;
}

void CWordCheckDlg::OnChangeDictionary() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_wordList.GetSelCount();
	CGlobal *pGL=CGlobal::GetInstance();
	if (nCount==0)
	{
		AfxMessageBox(L"請選擇字典語言");
		return;
	}
	int* buffer = new int[nCount];
	memset(buffer, -1, sizeof(int)*nCount);
	m_wordList.GetSelItems(nCount, buffer);
	CString tmp;
	m_carrDic.RemoveAll();
	m_nDicCount=0;
	for(int i=nCount-1; i>=0; i--)
	{
		m_wordList.GetText(buffer[i],tmp);
		m_carrDic.Add(tmp);
		m_nDicCount++;
	}
	delete[] buffer;
	
	FillUseWordList(m_richText);
	HightLightWord();	
	SetReplaceWordPos();
}

void CWordCheckDlg::SetLibGroup(CString &strLibGroup){
	int nDicCount=m_carrDic.GetSize();
	CString tmp;
	for(int i=0;i<nDicCount;i++){
		if (nDicCount==0)
		{
			return;
		}
		else if (nDicCount==1)
		{
			strLibGroup.Format(L"(N'%s')",m_carrDic[0]);
			break;
		}
		else if(nDicCount==2){
			strLibGroup.Format(L"(N'%s',N'%s')",m_carrDic[0],m_carrDic[1]);
			break;
		}
		else{
			if (i==0)
			{
				tmp.Format(L"(N'%s',",m_carrDic[i]);
			}
			else if(i==(nDicCount-1)){
				tmp.Format(L"N'%s')",m_carrDic[i]);
			}
			else 
				tmp.Format(L"N'%s',",m_carrDic[i]);
			strLibGroup+=tmp;//lstrcat(strLibGroup,tmp);
		}
		
	}
}

void CWordCheckDlg::OnCheck(CString strCotent){

	m_DicText=strCotent;
	CDialog::DoModal();
}

void CWordCheckDlg::OnReset(){
	m_nUseWordLength=0;
	m_arrSqlWordRecord.RemoveAll();
	m_arrUseWordRecord.RemoveAll();
	m_ctrList.ResetContent();
	CHARFORMAT cf;
	m_richCrl.GetDefaultCharFormat(cf);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;	
	cf.crTextColor = RGB(0,0,0);	
	m_richCrl.SetSel(0,m_richText.GetLength());
	m_richCrl.SetSelectionCharFormat(cf);
}

void CWordCheckDlg::OnDblclkReplaceList() 
{
	// TODO: Add your control notification handler code here
	OnChangeWord();
}

void CWordCheckDlg::SetReplaceWordPos(){
	/*if (m_nUseWordLength>0){
	if (!lstrcmp(m_strReplaceWord,m_arrUseWordRecord.GetAt(0).strName))
	{
		m_ctrList.SetCurSel(m_nReplacePos);
	}
	else{ 
		m_nReplacePos=0;
		m_ctrList.SetCurSel(m_nReplacePos);
		m_strReplaceWord=m_arrUseWordRecord.GetAt(0).strName;
	}
	}*/
	m_ctrList.SetCurSel(-1);
	for (int i = 0; i < m_ctrList.GetCount(); i++)
	{
		int iSel = m_ctrList.GetCurSel();
		int iGroupID = m_ctrList.GetItemData(i);
		if (iGroupID > -1)
		{
			m_ctrList.SetCurSel(i);
			if (iSel >= 0)
			{
				m_ctrList.SetCurSel(-1);
				break;
			}
		}
	}
}

void CWordCheckDlg::SetDefReplaceWordPos()
{
	m_ctrList.SetCurSel(-1);
	int min_id = 0;
	for (int i = 0; i < m_ctrList.GetCount(); i++)
	{
		int iSel = m_ctrList.GetCurSel();
		int iGroupID = m_ctrList.GetItemData(i);
		if (iGroupID > -1)
		{
			if (min_id == 0 ||  m_ctrList.GetItemData(i)<min_id)
			{
				min_id = m_ctrList.GetItemData(i);
				m_ctrList.SetCurSel(i);
			}
		}
	}
}

void CWordCheckDlg::OnChangeAllDef()
{
	ARRAY_WORDINFO m_arrNotDefRecord;
	CSaveProg dlg;
	CFont font;
	dlg.Create(IDD_PROGRESS,this);
	m_bChangeAllDef = TRUE;
	dlg.SetWindowText(L"");
	dlg.ShowWindow(SW_SHOW);
	dlg.SetStatus(L"正在變更預設字，請稍候...");
	int total = m_nUseWordLength;
	if (m_nUseWordLength > 0)
		HightLightWord();
	while(m_nUseWordLength > 0)
	{
		::PeekMessage(NULL, NULL, 0, 0, PM_REMOVE);
		dlg.SetProgress((int)((total-m_nUseWordLength)*10/total));
		SetDefReplaceWordPos();
		if (m_ctrList.GetCurSel()<0)
		{
			int lenfth=m_arrUseWordRecord.GetCount();
			m_arrNotDefRecord.Add(m_arrUseWordRecord.GetAt(0));
			OnIgnoreOnce();
			continue;
		}
		ReplaceText(TRUE);
		UpdateData(FALSE);
		m_nCurPos=0;
		HightLightWord();
	}
	dlg.ShowWindow(SW_HIDE);
	/*if (total>0)
		AfxMessageBox(L"檢查完畢,已變更全部預設字");*/
	m_arrUseWordRecord.Append(m_arrNotDefRecord);
	m_nUseWordLength = m_arrUseWordRecord.GetCount();
	m_bChangeAllDef =FALSE;	
	HightLightWord();
	SetReplaceWordPos();
}

BOOL CWordCheckDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class

    if (NULL != m_toolTip.GetSafeHwnd())
    {
        m_toolTip.RelayEvent(pMsg);
    }
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if (m_nUseWordLength==0 && !m_bChangeAllDef)
				OnBnClickedFinish();
			return FALSE;
		}
	}
	m_rtcShowMask.HideCaret();
    return CDialog::PreTranslateMessage(pMsg);
}

void CWordCheckDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if( MessageBoxW( L"檢查完畢,是否更新稿件?" , 
			L"字詞檢查系統", MB_YESNO | MB_ICONQUESTION ) == IDYES ){
		//	GetParent()->SendMessage(WM_COTENTCHANGED,0,0);

			CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
// 			// Get the active MDI child window.
 			CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
			editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();
			CDocument* pDoc = pChild->GetActiveDocument(); 
			POSITION pos = pDoc->GetFirstViewPosition(); 
			while (pos != NULL)   
			{   
				pView = (editView_Bottom *)pDoc->GetNextView(pos);   
				if (pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))   
				{   
					//pView->GetParentFrame()->ActivateFrame();   
					break;   
				}   
			}   
			pView->SetWindowText(m_richText);
			pView->WordBankReplace(&m_arrReplaceWord);
			//pView->ReplaceWord(m_arrReplaceWord);
			
		}
	CDialog::OnOK();
}

void CWordCheckDlg::MarkReplacedMask(int iPosition, CString strOrg, CString strDest)
{
	int iStrDestLen = strDest.GetLength(), iStrOrgLen = strOrg.GetLength();
	int iNoReplaced = 0 , iOrgCount = 0, iNoReturn = 0, iCurrentPos = 0, iAddedCount = 0;
	iCurrentPos = iPosition;
	CString strTemp = m_richText.Left(iCurrentPos);
	iNoReturn = strTemp.Replace(L"\r\n",L"\r\n");
	/*if(m_strMask.Find(L'b')>-1)
	{
		for(int i=0;i<m_strMask.GetLength();i++)
		{
			iOrgCount ++;
			if(m_strMask.GetAt(i) == L'b')
				continue;
			if(iOrgCount >= iPosition + iNoReplaced)
				break;
			if(m_strMask.GetAt(i) == L'd')
				iNoReplaced++;
		}
	}
	
	for(int i=0 ;i<iStrOrgLen;i++)
	{
		m_strMask.SetAt(iPosition + i+iNoReplaced - iNoReturn, L'd');
	}
		
	for(int i=0 ;i<iStrDestLen;i++)
	{
		m_strMask.Insert(iPosition + iStrOrgLen+ i+iNoReplaced - iNoReturn, L'b');
		m_strShowText.Insert(iPosition + iStrOrgLen+ i + iNoReplaced, strDest.GetAt(i));
	}*/

	m_strMask.Delete(iPosition - iNoReturn, iStrOrgLen);
	m_strShowText.Delete(iPosition, iStrOrgLen);
	for(int i=0 ;i<iStrDestLen;i++)
	{
		m_strMask.Insert(iPosition + i - iNoReturn, L'd');
		m_strShowText.Insert(iPosition + i , strDest.GetAt(i));
	}


	m_rtcShowMask.SetWindowTextW(m_strShowText);
	m_rtcShowMask.LockWindowUpdate();
	m_rtcShowMask.HideSelection(TRUE, FALSE);
	m_rtcShowMask.HideSelection(FALSE, FALSE);
	m_rtcShowMask.UnlockWindowUpdate();
}


void CWordCheckDlg::MarkColorOnShowRTC()
{
	ClearHightLight();
	m_rtcShowMask.LockWindowUpdate();
	m_rtcShowMask.HideSelection(TRUE, FALSE);
	int iNoReplaced = 0 , iCurrentPos = 0, iLength = 0, iOrgCount = 0, iNoReturn;
	iCurrentPos = m_arrUseWordRecord[0].nPosition;
	iLength = m_arrUseWordRecord[0].strName.GetLength();
	CString strTemp = m_richText.Left(iCurrentPos);
	iNoReturn = strTemp.Replace(L"\r\n",L"\r\n");
	/*if(m_strMask.Find(L'b')>-1)
	{
		for(int i=0;i<m_strMask.GetLength();i++)
		{
			iOrgCount ++;
			if(m_strMask.GetAt(i) == L'b')
				continue;
			if(iOrgCount >= iCurrentPos + iNoReplaced)
				break;
			if(m_strMask.GetAt(i) == L'd')
				iNoReplaced++;
		}
	}*/
	int iFirstPos = iCurrentPos + iNoReplaced - iNoReturn;
	int iEndPos = iCurrentPos + iNoReplaced + iLength - iNoReturn;
	m_rtcShowMask.SetSel(iFirstPos, iEndPos);

	CHARFORMAT2 cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	m_rtcShowMask.GetSelectionCharFormat(cf);
	cf.crTextColor = RGB(255,0,0);
	cf.dwMask= CFM_COLOR;
    cf.dwEffects ^= CFE_AUTOCOLOR;
	m_rtcShowMask.SetSelectionCharFormat(cf);
	m_rtcShowMask.HideSelection(FALSE, FALSE);
	m_rtcShowMask.UnlockWindowUpdate();
	m_rtcShowMask.SetSel(iFirstPos, iEndPos);
	m_rtcShowMask.HideSelection(TRUE, FALSE);
}


void CWordCheckDlg::ClearHightLight()
{
	m_rtcShowMask.LockWindowUpdate();
	m_rtcShowMask.SetSel(0, m_strMask.GetLength()-1);
	CHARFORMAT2 cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	m_rtcShowMask.GetSelectionCharFormat(cf);
	cf.crTextColor = RGB(0,0,0);
	cf.dwMask= CFM_COLOR;
	cf.dwEffects ^= CFE_AUTOCOLOR;
	m_rtcShowMask.SetSelectionCharFormat(cf);
	m_rtcShowMask.UnlockWindowUpdate();
	m_rtcShowMask.SetSel(0,0);
	m_rtcShowMask.HideSelection(TRUE, FALSE);
}

void CWordCheckDlg::OnEnSetfocusRichedit21()
{
	// TODO: Add your control notification handler code here
}



void CWordCheckDlg::OnBnClickedFinish()
{
	// TODO: Add your control notification handler code here
	if( MessageBoxW( L"檢查完畢,是否更新稿件?" , 
			L"字詞檢查系統", MB_YESNO | MB_ICONQUESTION ) == IDYES ){
		//	GetParent()->SendMessage(WM_COTENTCHANGED,0,0);

			CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
// 			// Get the active MDI child window.
 			CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
			editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();
			CDocument* pDoc = pChild->GetActiveDocument(); 
			POSITION pos = pDoc->GetFirstViewPosition(); 
			while (pos != NULL)   
			{   
				pView = (editView_Bottom *)pDoc->GetNextView(pos);   
				if (pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))   
				{   
					//pView->GetParentFrame()->ActivateFrame();   
					break;   
				}   
			}   
			pView->SetWindowText(m_richText);
			pView->WordBankReplace(&m_arrReplaceWord);
			//pView->ReplaceWord(m_arrReplaceWord);
		}
	CDialog::OnOK();
}
