// _MSC_VER >= 1000
// ReplaceView.h : header file
//
class CReplaceView : public CEditProReplaceView
{
protected: // create from serialization only
	CReplaceView();
	DECLARE_DYNCREATE(CReplaceView)

// Attributes
public:
	CEditroomDoc* GetDocument();
// Operations
public:
	CEditroomDoc *m_pDoc ;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReplaceView)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReplaceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CReplaceView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


#ifndef _DEBUG  // debug version in editView.cpp
inline CEditroomDoc* CReplaceView::GetDocument()
   { return (CEditroomDoc*)m_pDocument; }
#endif



/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
