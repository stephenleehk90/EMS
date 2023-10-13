#if !defined(AFX_LEADSCROLLINFO_H__035B4B0C_65FC_4EDB_B24A_EDA7C237E0A7__INCLUDED_)
#define AFX_LEADSCROLLINFO_H__035B4B0C_65FC_4EDB_B24A_EDA7C237E0A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CLEADScrollInfo wrapper class

class CLEADScrollInfo : public COleDispatchDriver
{
public:
	CLEADScrollInfo() {}		// Calls COleDispatchDriver default constructor
	CLEADScrollInfo(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CLEADScrollInfo(const CLEADScrollInfo& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetMin();
	void SetMin(long);
	long GetMax();
	void SetMax(long);
	long GetPage();
	void SetPage(long);
	long GetLine();
	void SetLine(long);
	long GetPos();
	void SetPos(long);
	BOOL GetVisible();
	void SetVisible(BOOL);
	float GetWidth();
	void SetWidth(float);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEADSCROLLINFO_H__035B4B0C_65FC_4EDB_B24A_EDA7C237E0A7__INCLUDED_)