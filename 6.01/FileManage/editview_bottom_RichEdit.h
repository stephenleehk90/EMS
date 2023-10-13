#if !defined(AFX_editView_Bottom_RichEdit_H__339F7979_7539_4647_B17E_6769034295BB__INCLUDED_)
#define AFX_editView_Bottom_RichEdit_H__339F7979_7539_4647_B17E_6769034295BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// editView_Bottom_RichEdit.h : header file
//
#include "editdoc.h"
#include <afxrich.h>
#include "C_ColorToolTip.h"
/////////////////////////////////////////////////////////////////////////////
// editView_Bottom_RichEdit view

#define		MAX_TEXTSTYLE_NUM	0xfe	// 1 ... 0xff

class editView_Bottom_RichEdit : public CRichEditView
{
protected:
	editView_Bottom_RichEdit();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(editView_Bottom_RichEdit)

// Attributes
public:
	editDoc* GetDocument();
	CString m_fileName;
	editDoc *m_pDoc;  
	COleDateTime dt;
	int m_nSpanSec;
protected:
	BOOL		m_bCreateFontSet;
	CFont*		m_pFont;
	CFontSet*	m_pFontSet;
	CStatusBar* m_pStatusBar;
	  
	CBrush m_BkBrush;
	CMenu*		m_pPopupMenu;
	//Added by Ronald Saliman on 20120910
	CString m_isAutoSave;
	int m_autoSaveDuration;
	int actualWordCount;
	//end 20120910

	CFont	*m_ppFont[MAX_TEXTSTYLE_NUM];
	int		m_pnLineSpace[MAX_TEXTSTYLE_NUM];
	int		m_pnCharSpace[MAX_TEXTSTYLE_NUM];
	int		m_nStyleNum;	// 1 ... MAX_STYLE_NUM
	bool m_bFontCreated;

	bool m_b_on_paste;
	bool m_b_allow_edit;
	bool m_bShowComment;
	bool m_bOpenFindDialog; 
	BOOL bFirstType;
	BOOL bUpperStartedFlag;

	BOOL m_bUpdateUpper;
	C_ColorToolTip m_Tips;
	CPoint ptLog;
	int iFontHeight;
	CTime iPastCaretTime;
// Operations
public:
	void	SetTextToUpperView(); 
	void	SetReadOnly(BOOL bReadOnly);
	bool	m_b_disable_modify;
	void UpdateFontSetTemp(CString strFontName);
	void WordBankReplace(ARRAY_REPLACEWORD *arrList);
	void AutoSaveFileToLocal();
	void OnChangeUpperView();
	void KillUpdateUpperTimers();
	void StartUpdateUpperTimer();

protected:

	void ChangeUpperViewFont(); 
	void InitStatusBar();
	void ReleaseStatusBar();
	void OnEditDelete(int i_select_text_len);
	int GetActualPos(int i_Pos);
	CString GetTextFromPoint(int i_Pos, bool b_start);
	CString GetTextFromPoint_Len(int i_Pos, int &i_len, bool &b_new_line, bool b_backspace, bool b_select_text);
	void	DisplayDocInfo();
	void GetSelectPosAndLength(int &pos,  int &i_Select_Text_Len); 
	void SetTextStyle(int i_Style);
	int GetTextStyle();
	void GetPrintInfo(int iPageIndex, int &iStartPos, int &iEndPos); 
	CString GetActualSelectText(int &pos,  int &i_Select_Text_Len, int &nLen); 
	void UpdateFontSet();
	void OnTimer(UINT nIDEvent);
	void OnInsertSpaceOnFront();
	int GetActualDeletePosCount(int i_Pos);
	
	CFindReplaceDialog* m_pFindDlg;
	CFindReplaceDialog* pDlg_Test;
	afx_msg LONG OnFindMsg(UINT wParam, LONG lParam);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(editView_Bottom_RichEdit)
	public:
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~editView_Bottom_RichEdit();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(editView_Bottom_RichEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnPaste(WPARAM, LPARAM);
	afx_msg LRESULT OnCopy(WPARAM, LPARAM);
	afx_msg LRESULT OnCut(WPARAM, LPARAM);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCommand(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionEdit(CCmdUI* pCmdUI);
	afx_msg void OnOptionEdit();
	afx_msg void OnOptionChange();

	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateDoCommit(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateDoModify(CCmdUI* pCmdUI);

	afx_msg void OnUpdateEditReadOnly(CCmdUI* pCmdUI);
	afx_msg void OnEditReadOnly();

	afx_msg void OnUpdateEditShowcharcode(CCmdUI* pCmdUI);
	afx_msg void OnEditShowcharcode();
	afx_msg void OnUpdatePrintView(CCmdUI* pCmdUI);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);	
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnEditAddcomment();
	afx_msg void OnEditShowcomment();
	afx_msg void OnDeletecomment();
	afx_msg void OnEditGotoline(); 
	afx_msg void OnEditReplace(); 
	afx_msg void OnEditFind(); 
	afx_msg LONG OnFindReplace(WPARAM wParam,LPARAM lParam);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileReplace(CCmdUI* pCmdUI);
//	afx_msg LRESULT OnReplace(WPARAM, LPARAM);
	afx_msg void OnFind();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnTsStyle1();
	afx_msg void OnTsStyle10();
	afx_msg void OnTsStyle11();
	afx_msg void OnTsStyle12();
	afx_msg void OnTsStyle13();
	afx_msg void OnTsStyle14();
	afx_msg void OnTsStyle15();
	afx_msg void OnTsStyle2();
	afx_msg void OnTsStyle3();
	afx_msg void OnTsStyle4();
	afx_msg void OnTsStyle5();
	afx_msg void OnTsStyle6();
	afx_msg void OnTsStyle7();
	afx_msg void OnTsStyle8();
	afx_msg void OnTsStyle9();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	BOOL CheckRangeOnComment();
	void ShowCommentInToolTip();
	afx_msg void OnUpdateSwitchMode(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnInsertWords(UINT nID );
	CFont* GetPrintFontSize();
//	afx_msg LONG OnFindMsg(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()
	void ChangLineSpace();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_editView_Bottom_RichEdit_H__339F7979_7539_4647_B17E_6769034295BB__INCLUDED_)
