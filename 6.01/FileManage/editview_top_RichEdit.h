#if !defined(AFX_editView_Top_RichEdit_H__98EBC5C4_8A0D_450B_B0DF_7F0E12E97470__INCLUDED_)
#define AFX_editView_Top_RichEdit_H__98EBC5C4_8A0D_450B_B0DF_7F0E12E97470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// editView_Top.h : header file
//
#include <afxrich.h>
#include "editframenew.h"
#include "editdoc.h"
#include "C_ColorToolTip.h"
/////////////////////////////////////////////////////////////////////////////
// editView_Top view

class version_pos
{
public:
	int start;
	int end;
};
typedef CArray<version_pos> ARRAY_version_pos_1;

class CHAR_POS_1
{
public:
	CPoint pt;
	int i_pos;
};
typedef CArray<CHAR_POS_1,CHAR_POS_1> ARRAY_CHAR_POS_1;


//class editView_Top : public CScrollView
class editView_Top_RichEdit : public CRichEditView
{
protected:
	editView_Top_RichEdit();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(editView_Top_RichEdit)
	CBrush sBRUSH;
	HBRUSH sHBR;

	C_ColorToolTip m_Tips;

	int GetVersion(int i_pos);
	int GetLineNumber(int i_pos, int &i_line_pos); 
	bool IsDigit_Character(CString strTemp);

	bool IsSelectedChar(int i_temp); 	

	bool IsCapLetter_Character(CString strTemp);
	long SelectionPoint(CPoint point);
	CPoint GetExtent(CDC *pDC, const CString& string, long xoffset = 0, long line = 1);
	void SetSelectText();
	CString GetActualSelectedText();  
	int GetVersion_New(int i_pos, bool &b_Del, int &i_old);
	void InitModHistArray(); 
	int GetActualPos(int i_Pos);

	CFont* m_pFont;
	CStatusBar* m_pStatusBar;
	CString m_str_Text, m_str_Text_Temp;
	CString m_str_Text_Version;
	CString m_str_Mod_History;
	CString m_p_str_Text, m_p_str_Text_Version, m_p_str_Mod_History;
	CRect rc_org;

	CFont	*m_ppFont[MAX_TEXTSTYLE_NUM];
	int		m_pnLineSpace[MAX_TEXTSTYLE_NUM];
	int		m_pnCharSpace[MAX_TEXTSTYLE_NUM];
	int		m_nStyleNum;	// 1 ... MAX_STYLE_NUM
	bool m_bFontCreated;

	int m_max_height;
	int m_max_width;
	CMenu*		m_pPopupMenu;
	bool m_b_UpdateScroll;
	BOOL						m_mouseDown;
	BOOL						m_mouseMove;
	long						m_caret;
	long						m_selectStart_x;
	long						m_selectEnd_x;
	long						m_selectStart_y;
	long						m_selectEnd_y;
	long						m_previous_x;
	long						m_previous_y;

	long						m_indent;
	long						m_tabSize;
	CFont						m_font;
	CPoint m_mousePosition;
	CRect rc_standard;
	
	ARRAY_CHAR_POS_1 m_arrCharPos;
	CStringArray m_arrModHist;
	long m_max_select_pos;
	long m_min_select_pos;
	HINSTANCE m_hinstRE41; 
	CPoint ptLog;
	BOOL b_IsFirst;
	int iFontHeight;
	CTime iPastCaretTime;
	BOOL bFocus;
// Attributes
public:
	BOOL bIsCommentMode;
	BOOL bThreadInProcess;
//	void SetContentText(CString strText, CString strTextVersion);
	void SetContentText(CString strText, CString strTextVersion, CString strModHistory);
	void UpdateFontSet();
	void UpdateFontSetTemp(CString strFontName);
	void OnShowcomment();
	editFrameNew* pFrame;
	editDoc *m_pDoc;
	BOOL bHasComment;
	BOOL bStartFlag;
	BOOL bViewChanged;
	BOOL GetDebugFlag();
	void OnLogFile(CString strMessage);
	void KillToopTips();
	void editView_Top_RichEdit::SetVSrcollBar();

	static UINT StartThread (LPVOID param);	//controlling function header
	
	typedef struct THREADSTRUCT				//structure for passing to the controlling function
	{
		editView_Top_RichEdit*	_this;
	} THREADSTRUCT;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(editView_Top)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnScroll( UINT nScrollCode, UINT nPos, BOOL bDoScroll= TRUE );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void AppendStrokeOut(int &iPos, BOOL &bStop);
	void AppendUnderLight(int &iPos, BOOL &bStop);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~editView_Top_RichEdit();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(editView_Top)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnCopy();
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdate();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//fx_msg void OnUpdateDoCommit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDoModify(CCmdUI* pCmdUI);
	afx_msg void OnEditShowModHistory();
	afx_msg void OnOptionModifyHistory();
	void SwitchMode();
	void setHistoryTextColor();
	void OnEditAddcomment();
	BOOL CheckRangeOnComment();
	void ShowCommentInToolTip();
	void OnDeletecomment();
	afx_msg void OnUpdateSwitchMode(CCmdUI* pCmdUI);
	afx_msg void OnSize( UINT nType, int cx, int cy);
	void OnUpdateCommand(CCmdUI* pCmdUI);
	int GetCurVerEndPos(int iCurIndex) ;
	//afx_msg void OnSetFocus( CWnd* );
	//void OnKillFocus(CWnd* pNewWnd) ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_editView_Top_H__98EBC5C4_8A0D_450B_B0DF_7F0E12E97470__INCLUDED_)
