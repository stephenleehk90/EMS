#pragma once

#include "diff_match_patch.h"
#include "editdoc.h"
#include "Global.h"
// CRebuildHistory

class CRebuildHistory : public CWnd
{
	DECLARE_DYNAMIC(CRebuildHistory)

public:
	CRebuildHistory();
	virtual ~CRebuildHistory();

	editDoc* m_pDoc;
	CString m_strOldMask, m_strOldVersion, m_strText;

	BOOL DiffProcess(CString strText, CString strOrg, CString &strVersion, CString &strMask);
	BOOL CompareDiff();
	static Diffs diffList(
      // Diff(INSERT, NULL) is invalid and thus is used as the default argument.
      Diff d1 = Diff(INSERT, wstring()), Diff d2 = Diff(INSERT, wstring()),
      Diff d3 = Diff(INSERT, wstring()), Diff d4 = Diff(INSERT, wstring()),
      Diff d5 = Diff(INSERT, wstring()), Diff d6 = Diff(INSERT, wstring()),
      Diff d7 = Diff(INSERT, wstring()), Diff d8 = Diff(INSERT, wstring()),
      Diff d9 = Diff(INSERT, wstring()), Diff d10 = Diff(INSERT, wstring()));

protected:
	DECLARE_MESSAGE_MAP()
};


