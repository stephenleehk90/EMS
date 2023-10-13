#if !defined(AFX_editDoc_H__63BF1D26_13A3_4FC4_8B34_2486E6D85393__INCLUDED_)
#define AFX_editDoc_H__63BF1D26_13A3_4FC4_8B34_2486E6D85393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// editDoc.h : header file
//
#include "afxcmn.h"
#include <vector>
using namespace std;
typedef vector<wstring> Strings;
#include "diff_match_patch.h"

class UNDOREDOLOG
{
public:
	bool	bIsAdd;
	long	lSeq;	
	long	lLen;
	long	lStartPos;
	bool    bIsCurrent;
	CString strRedoUndo;
	bool    bExtra;
	CString strRedoUndo_Extra;
	CString strMark;
	CString strComment;
	CString strStyle;
	CString strRedoUndo_Top;
};
typedef CArray<UNDOREDOLOG,UNDOREDOLOG> ARRAY_UNDOREDOLOG;


class DOC_COMMENT
{
public:
//	int	i;
	int	iIndex;
	CString strComment;
};
typedef CArray<DOC_COMMENT,DOC_COMMENT> ARRAY_DOC_COMMENT;

class REPLACEWORDITEM{
public:
	int nPosition;
	CString strOrgText;
	CString strTrgText;
};
typedef CArray<REPLACEWORDITEM,REPLACEWORDITEM> ARRAY_REPLACEWORD;


class PRINTPAGEINFO
{
public:
	long	lPageNo;	
	long	lStartPos;
	long	lEndPos;
};
typedef CArray<PRINTPAGEINFO,PRINTPAGEINFO> ARRAY_PRINTPAGEINFO;

/////////////////////////////////////////////////////////////////////////////
// editDoc document

class editDoc : public CDocument
{
protected:
	editDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(editDoc)

// Attributes
public:
	FILEPARAM m_FileParam;
	CWiresParam m_WiresParam;
	CDMParam m_DMParam;
	BOOL m_bTransfer;

	BOOL m_bCreate;
	CCaptionParam m_CaptionParam;
	CString m_strDisplayTitle;

	//
	CString m_strVersion;
	CString m_strVersion_Mark;
	CString m_str_Current_Version;
	CString m_str_Current_Ver_Del;
	CString m_strModHistory;
	CString m_strVersion_Comment;
	CString m_strVersion_Style;
	CString m_str_Comment;

	//For History Rebulid usage
	CString strTempOrgText;
	CString strTempVersion;
	CString strTempVersionMark;
	

	CString m_strLastUser;

	ARRAY_UNDOREDOLOG m_arrUndo;
	ARRAY_UNDOREDOLOG m_arrRedo;
	ARRAY_DOC_COMMENT m_arrComment;
	
	ARRAY_PRINTPAGEINFO m_arrPrintPageInfo;

	BOOL m_bEditorLog;
	BOOL m_bClose;
	BOOL m_bIsStoryFile;
	CString p_str_Ver_Del;
	CString p_str_Ver;
	CString p_str_Ver_Style;
	CString p_str_Ver_Comment;
	BOOL m_bNewCheckingApplied;
	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(editDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	 virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~editDoc();

	BOOL m_bNeedSaveDB;
	BOOL  DoCommit(BOOL &bClose );
	CString GetTempFileID();
	BOOL OpenTextFile(const wchar_t *pszPathName);

	void UpdateTextVersionPos(CString strAppend, int i_Pos, bool b_redoundo);
	void DeleteTextVersionPos(int i_Pos, int i_Length);

	void SetTextVersionComment(int i_Pos, int i_Length, int i_comment_index, bool b_Delete);

	CString GetTextVersionComment(int i_Pos, int i_Length);
	
	CString GetCurrentVersion(CString str_lastuser);
	CString GetCurrentVersion_Del(CString str_lastuser);

	void SetTextVersionStyle(int i_Pos, int i_Length, int i_style_index);
	int GetTextVersionStyle(int i_Pos);

	int GetMarkPosition(int i_Pos, bool is_undoredo);
	int GetEnterMarkPosition(int i_Pos, bool is_undoredo);
	int GetMarkLength(int i_Pos, int i_Length);
	void SetReadOnly();
	bool AddComment(CString strComment, int &iCommentIndex);
	void GetComment(CString& strComment, int iStart, int iEnd);
	void InitCommentStringArray();
	void DeleteUnUsedComment();
	int GetActualWordCount();
	void SaveDocumentLocal(); 
	void SetAllowEdit();
	void SetMsg(CString strLockUser);
	void SaveDocumentAs();
	void AutoSaveDocumentLocal();
	HMENU m_hDefaultMenu;
	BOOL m_bHasCommit;
	CString m_strTmpFileCode;
	BOOL m_bPending;
	CString GetPStrVerComment();

	wchar_t	m_szAbstract[ABSTRACT_SIZE];
	wchar_t	m_szAuthor[NAME_SIZE];
	wchar_t	m_szUser[NAME_SIZE];
	wchar_t	m_szPrintCaption[ABSTRACT_SIZE];
	void OnLogFile(CString str_Message);
	BOOL CheckCompareDiff();

private:
	bool m_b_allow_edit;
	CString GetAllCommentString();
	CWiresAttach PrepareAttachment(CString strFileName, CString strWiresCode);
	BOOL CheckTxtIsOnCol(CString strFileName, CString &strCurrUser);
	BOOL ResetCurruser(CString strFileName, CString strCurrUser);
	CString GetEditViewText();
	bool Check_Connect();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(editDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnDoCommit();
	afx_msg void OnWordCheck();
	afx_msg void OnUpdateDoCommit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndoNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedoNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDoup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZeroC(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZeroF(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDivide(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMultiply(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDel(CCmdUI* pCmdUI);
	afx_msg void OnDoModify();
	afx_msg void OnUpdateDoModify(CCmdUI* pCmdUI);
	afx_msg void OnContentChanged(CString strContent);
	afx_msg void OnUpdateWordChk(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchMode(CCmdUI* pCmdUI);
	afx_msg void OnDoSel();
	afx_msg void OnUpdateDoSel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDoSource(CCmdUI* pCmdUI);
	afx_msg void OnDoSource();
	afx_msg void OnDoSendcol();
	afx_msg void OnUpdateDoSendcol(CCmdUI* pCmdUI);
	afx_msg void OnDoSign();
	afx_msg void OnUpdateDoSign(CCmdUI* pCmdUI);
	afx_msg void OnDoSigndirect();
	afx_msg void OnUpdateDoSigndirect(CCmdUI* pCmdUI);
	afx_msg void OnDoBackfile();
	afx_msg void OnUpdateDoBackfile(CCmdUI* pCmdUI);
	afx_msg void OnRdoRecord();
	afx_msg void OnUpdateRdoRecord(CCmdUI* pCmdUI);
	afx_msg void OnRdoComment();
	afx_msg void OnUpdateRdoComment(CCmdUI* pCmdUI);
	afx_msg void OnRdoCopy();
	afx_msg void OnUpdateRdoCopy(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_editDoc_H__63BF1D26_13A3_4FC4_8B34_2486E6D85393__INCLUDED_)
