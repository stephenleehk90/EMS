#if !defined(AFX_editView_Top_H__98EBC5C4_8A0D_450B_B0DF_7F0E12E97470__INCLUDED_)
#define AFX_editView_Top_H__98EBC5C4_8A0D_450B_B0DF_7F0E12E97470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// editView_Top.h : header file
//
#include <afxrich.h>
#include "editframenew.h"
#include "C_ColorToolTip.h"
/////////////////////////////////////////////////////////////////////////////
// editView_Top view

class CHAR_POS
{
public:
	CPoint pt;
	int i_pos;
};
typedef CArray<CHAR_POS,CHAR_POS> ARRAY_CHAR_POS;

class CHAR_WIDTH
{
public:
	char c;
	int iWidth;
};
typedef CArray<CHAR_WIDTH,CHAR_WIDTH> ARRAY_CHAR_WIDTH;

class TEXT_LOCATION
{
public:
	CString strText;
	int iWidth;
	int iHeight;
	int iX;
	int iY;
};
typedef CArray<TEXT_LOCATION,TEXT_LOCATION> ARRAY_TEXT_LOCATION;


class editView_Top : public CScrollView
{
protected:
	editView_Top();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(editView_Top)
	CBrush sBRUSH;
	HBRUSH sHBR;
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

	CFont* m_pFont;
	CStatusBar* m_pStatusBar;
	CString m_str_Text, m_str_Text_Temp;
	CString m_str_Text_Version;
	CString m_str_Mod_History;

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
	
	ARRAY_CHAR_POS m_arrCharPos;
	CStringArray m_arrModHist;
	int m_max_select_pos;
	int m_min_select_pos;

	int iCurLine;
	int iLineSpaceConstant;
	int iTextSpaceConstant;
	int iNextStartLineStrPos;
	ARRAY_CHAR_WIDTH m_arrCharWidth;
	ARRAY_TEXT_LOCATION m_arrTextLocation;
	CString str_Ver_Comment;
	CString p_str_Ver_Comment;
	C_ColorToolTip m_Tips;
// Attributes
public:
//	void SetContentText(CString strText, CString strTextVersion);
	void SetContentText(CString strText, CString strTextVersion, CString strModHistory);
	void UpdateFontSet();
	void UpdateFontSetTemp(CString strFontName);
	BOOL bReDraw;
	BOOL bIsCommentMode;
	editFrameNew* pFrame;
	BOOL bViewChanged;
	editDoc* m_pDoc;
	void KillToopTips();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(editView_Top)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnScroll( UINT nScrollCode, UINT nPos, BOOL bDoScroll= TRUE );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~editView_Top();
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
//fx_msg void OnUpdateDoCommit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDoModify(CCmdUI* pCmdUI);
	afx_msg void OnEditShowModHistory();
	afx_msg void OnOptionModifyHistory();
	int iFontHeight, m_iFontWidth;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	BOOL HasScrollToBottom();
	int FindNextLineStrPos(int iCurLine);
	afx_msg void OnSize( UINT nType, int cx, int cy);
	void TestOnDraw();
	void editView_Top::GetASCIIActualWidth();
	CEdit m_Edit;
	CString GetTextByLine(int iRelativeLine);
	int GetSkippedTextLength(int iRelativeLine, int &iEnterCount);
	int GetWidthByChar(wchar_t c, int iCheckedLength, int iEnterCount);
	int iPrevScrollPos, iAutoScrollLine;
	BOOL bSelText, bAutoLineScrolledDown, bAutoLineScrolledUp ,bKeyDown;
	CString str_SelText; //for copy function
	int iSelTextPos; // for modify history
	void SetTextCaret();
	void MarkDownHightLightTextPosition();
	int iStartSelPos, iEndSelPos, m_Actual_StartX,m_Actual_StartY , m_CurMouse_X, m_CurMouse_Y, iPrevTotalLine;
	void ScrollDownOneLine(int iLine=1);
	void ScrollUpOneLine(int iLine=1);
	clock_t cktPrevScroll;
	void MoveCaretHorizontialLeft();
	void MoveCaretHorizontialRight();
	int GetLineMaxX(int iLineNo);
	void SetScrollBarOnText();
	void SwitchMode();
	void OnUpdateSwitchMode(CCmdUI* pCmdUI);
	void OnEditAddcomment() ;
	void ShowCommentInToolTip();
	void OnDeletecomment() ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_editView_Top_H__98EBC5C4_8A0D_450B_B0DF_7F0E12E97470__INCLUDED_)
