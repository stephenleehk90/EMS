// editroomDoc.h : interface of the CEditroomDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITROOMDOC_H__30DD0185_3F59_4C16_83F0_750D2658B508__INCLUDED_)
#define AFX_EDITROOMDOC_H__30DD0185_3F59_4C16_83F0_750D2658B508__INCLUDED_

#include "..\editpro\editpro.h"
#include "..\commkits\commkits.h"
#include "ListItem.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifdef _MAC
#undef AFX_EXT_API
#undef AFX_EXT_CLASS
#define AFX_EXT_API
#define AFX_EXT_CLASS
#endif

#ifdef _DLLIMPL
   #define CLASS_DECL_B	__declspec(dllexport)
#else
   #define CLASS_DECL_B	__declspec(dllimport)
#endif

const CString DOFILE_LIST = _T("FM_DOFILE_LIST");
const CString DOCOL_LIST  = _T("FM_DOCOL_LIST");
const CString DOFILE_RECORDLIST  = _T("FM_DOFILE_RECORDLIST");
const CString FILERELA_RECORDLIST  = _T("FM_FILERELA_RECORDLIST");
const CString FILESEARCH_LIST  = _T("GL_FILESEARCH_LIST");

// 以下是 List Ctrl Column Map 文件 版本號, 變更此號碼,會導致原有 .dat 文件失效
const int DOFILE_LIST_VER			= 0;
const int DOCOL_LIST_VER			= 0;
const int DOFILE_RECORDLIST_VER		= 0;
const int FILERELA_RECORDLIST_VER	= 0;
const int FILESEARCH_LIST_VER		= 0;

struct EDITFILEBAK
{
	int cbSize;
	FILEPARAM FileParm;
	TCHAR szDisplayTitle[254];
	TCHAR szTmpFileCode[254];
	TCHAR szPathFile[254];
	CWiresParam WiresParm;
	BOOL bHasCommit;
	BOOL bNeedSaveDB;
	BOOL bPending;
	TCHAR szReserved0[128];
	TCHAR szReserved1[128];
	TCHAR szReserved2[128];
};
struct CAttach
{
	CString strFileCode;
	CString strName;
	CString strType;
	int		nSize;
	CString strUploadDate;
	CString strUser;
};
class CSearchParam
{
public:
	short	m_iType;		//-1:not by position,0:by lib ID,1:by column and paper
	CString m_strFileCode;	//
	int		m_nLibID;
	int		m_nColumnID;
	//CString m_strUser;
	CString m_strUserCode;
	CString m_strKey;
	CString	m_strBeginDate;
	CString m_strEndDate;
	int		m_nTotalOnPaper;//receive value
	double	m_dblWordsOnPaper;//receive value
public:	
	CSearchParam(){
		m_dblWordsOnPaper=0.0;
		m_nTotalOnPaper = 0;
	}
	BOOL SearchFile(CListCtrlEx *pCtrl,CADORecordset *rs,CMapStringToOb *pMap);
private:
	CString GetRecordDateFilter();
	CString GetColPermFilter();
	CString GetLibPermFilter();
	
	void	GetSQL(CString &strSQL);
	CString GetPaperFilter();
	CString GetLibFilter();
	CString GetPaperBaseSQL();
	CString GetLibBaseSQL();
	CString GetKeyFilter();
	CString GetUserFilter();
	CString GetFileCodeFilter();
	CString GetDateFilter();
};
//define DoCol list control columns
const short DOCOL_FILENAME		=0;
const short DOCOL_FILEFORMAT	=1;
const short DOCOL_TITLE			=2;
const short DOCOL_CURSTATUS		=3;
const short DOCOL_HAVERELATE	=4;
const short DOCOL_WORDCOUNT		=5;
const short DOCOL_FILEAUTHOR	=6;
const short DOCOL_TRANSDATE		=7;
const short DOCOL_SIGNEDFILE	=8;
const short DOCOL_UPDATEDATE	=9;
const short DOCOL_ASSIGNEDUSER	=10;
const short DOCOL_LOCKUSER		=11;

//define DoFile list control columns
const short DOFILE_FILENAME		=0;
const short DOFILE_FILEFORMAT	=1;
const short DOFILE_TITLE		=2;
const short DOFILE_CURSTATUS	=3;
const short DOFILE_HAVERELATE	=4;
const short DOFILE_WORDCOUNT	=5;
const short DOFILE_FILEAUTHOR	=6;
const short DOFILE_ASSIGNEDUSER	=7;
const short DOFILE_LOCKUSER		=8;
const short DOFILE_COMMENT		=9;
const short DOFILE_UPDATEDATE	=10;

//file record list contrl columns	
const short FILERECORD_DOTIME	=0;
const short FILERECORD_OPERATION=1;
const short FILERECORD_OPERATOR	=2;
const short FILERECORD_WORDCOUNT=3;
const short FILERECORD_POSITION	=4;
const short FILERECORD_COMMENT	=5;

const int	PIC_SIZE = 200;

class  CEditroomDoc : public CEditProDoc
{
protected: // create from serialization only
	CEditroomDoc();
	DECLARE_DYNCREATE(CEditroomDoc)
	friend class CEditroomView;
// Attributes
public:
	
	CWiresParam m_WiresParam;
	CDMParam m_DMParam;
	FILEPARAM m_FileParam;
// Operations
public:
	CString GetTempFileID();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditroomDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bCreate;
	BOOL InsertText(LPCTSTR lpText);
	BOOL UnLockFile();
	BOOL m_bTransfer;
	CCaptionParam m_CaptionParam;
	void BeforeAutosave();
	CString m_strBakFile;
	BOOL SaveBkFile();
	BOOL SetBkEnv(EDITFILEBAK *pEditBak);
	BOOL m_bHasCommit;
	CString m_strDisplayTitle;
	BOOL m_bNeedSaveDB;
	HMENU m_hDefaultMenu;

	void GetTitles(TCHAR *szTitle1,TCHAR *szTitle2,TCHAR *szTitle3,TCHAR *szTitle4);
	void SetReadOnly(BOOL bReadOnly);
	CString m_strTmpFileCode;
	void InitChildWnd();
	virtual ~CEditroomDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL  DoCommit(BOOL &bClose );
protected:

// Generated message map functions
protected:
	virtual void AfterSaveDocumnet();
	//{{AFX_MSG(CEditroomDoc)
	afx_msg void OnDoCommit();
	afx_msg void OnUpdateDoCommit(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnDoModify();
	afx_msg void OnUpdateDoModify(CCmdUI* pCmdUI);
	afx_msg void OnDoSel();
	afx_msg void OnUpdateDoSel(CCmdUI* pCmdUI);
	afx_msg void OnDoSource();
	afx_msg void OnUpdateDoSource(CCmdUI* pCmdUI);
	afx_msg void OnDoSign();
	afx_msg void OnUpdateDoSign(CCmdUI* pCmdUI);
	afx_msg void OnDoSigndirect();
	afx_msg void OnUpdateDoSigndirect(CCmdUI* pCmdUI);
	afx_msg void OnRdoRecord();
	afx_msg void OnUpdateRdoRecord(CCmdUI* pCmdUI);
	afx_msg void OnRdoComment();
	afx_msg void OnUpdateRdoComment(CCmdUI* pCmdUI);
	afx_msg void OnRdoCopy();
	afx_msg void OnUpdateRdoCopy(CCmdUI* pCmdUI);
	afx_msg void OnDoSendcol();
	afx_msg void OnUpdateDoSendcol(CCmdUI* pCmdUI);
	afx_msg void OnDoBackfile();
	afx_msg void OnUpdateDoBackfile(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnFileSave();
	DECLARE_MESSAGE_MAP()
private:
	
	BOOL m_bPending;
};

/////////////////////////////////////////////////////////////////////////////

class  CEditroomView : public CEditProView
{
protected: // create from serialization only
	CEditroomView();
	DECLARE_DYNCREATE(CEditroomView)

// Attributes
public:
	CEditroomDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditroomView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditroomView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEditroomView)
	afx_msg void OnUpdateEditReadonly(CCmdUI* pCmdUI);
	afx_msg void OnEditReadonly();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//#ifndef _DEBUG  // debug version in editroomView.cpp
//inline CEditroomDoc* CEditroomView::GetDocument()
//   { return (CEditroomDoc*)m_pDocument; }
//#endif

class  CChildFrame : public CEditProFrame
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void SetToolBarBitmap();
	CImageList m_ToolBarImageList;
	CImageList m_ToolBarDisableImageList;
	int CreateToolBarOnMainFrm(UINT nID,CWnd *pParent=NULL);
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	BOOL m_bCreateToolbar;
	CToolBar    m_wndToolBar;
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CDlgDisplyPicture;
class  CEditFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CEditFrame)
protected:
	CEditFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CDlgDisplyPicture *m_pDlgDisplyPicture;
	BOOL  m_bCaptionPicShowed;
// Operations
public:
	void SetToolBarBitmap();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditFrame)
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	virtual ~CEditFrame();
	virtual BOOL PreTranslateMessage(MSG* pMsg) ;
	// Generated message map functions
	//{{AFX_MSG(CEditFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	afx_msg void OnDestroy();
	afx_msg void OnCaptionPicture();
	afx_msg void OnUpdateCaptionPicture(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITROOMDOC_H__30DD0185_3F59_4C16_83F0_750D2658B508__INCLUDED_)
