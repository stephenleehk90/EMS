// RebuildHistory.cpp : implementation file
//

#include "stdafx.h"
#include "FileManage.h"
#include "RebuildHistory.h"


// CRebuildHistory

IMPLEMENT_DYNAMIC(CRebuildHistory, CWnd)

CRebuildHistory::CRebuildHistory()
{
	
}

CRebuildHistory::~CRebuildHistory()
{
}

BEGIN_MESSAGE_MAP(CRebuildHistory, CWnd)
END_MESSAGE_MAP()



// CRebuildHistory message handlers
BOOL CRebuildHistory::CompareDiff()
{
	m_strOldMask = m_pDoc->strTempVersionMark;
	m_strOldVersion = m_pDoc->strTempVersion;
	
	CString strNewMask = L"", strNewVersion = L"";
	if(m_pDoc->m_str_Current_Version == L"a")	
	{
		for(int i=0;i<m_strText.GetLength();i++)
		{
			strNewMask += L"a";
		}
		strNewVersion = m_strText;
		m_strOldVersion = strNewVersion;
		m_strOldMask = strNewMask;
	}else
	{
		DiffProcess(m_strText, m_pDoc->strTempOrgText, m_strOldVersion,m_strOldMask );
	}
	
	return true;
}

BOOL CRebuildHistory::DiffProcess(CString strText, CString strOrg, CString &strVersion, CString &strMask)
{
	diff_match_patch dmp;
	Diffs diffs = diffList();
	diffs = diffList(Diff(DELETEA, (wstring)strOrg), Diff(INSERT, (wstring)strText));
	Diffs list2 = dmp.diff_main((wstring)strOrg, (wstring)strText, false); 
	CString m_strResult = L"", m_strOperation = L"", m_strFullText = L"";

	bool first = true;
	int index = 0;
	strVersion.Replace(L"\r\n", L"\r");
	strVersion.Replace(L"\n", L"\r");
	//strVersion.Replace(L"\r", L"æ");
    for (Diffs::const_iterator d2 = list2.begin(); d2 != list2.end(); ++d2) 
	{
		if (!first) 
		{
			m_strOperation += L"\r\nL";
		}
		CString strOper, strText;
		strOper = (*d2).strOperation((*d2).operation).c_str();
		strText = (*d2).text.c_str();
		m_strOperation += (*d2).toString().c_str();
		strText.Replace(L"\r\n", L"\r");
		strText.Replace(L"\n", L"\r");
		//strText.Replace(L"\r", L"æ");
		//strText.Replace(L"\r\n", L"\n");
		m_strFullText += strText;
		//strText.Replace(L"\n", L"æ");
		if(strOper == "INSERT")
		{
			for(int i=0;i<strText.GetLength();i++)
			{
				do
				{
					if(index >= strMask.GetLength())
						break;
					if(m_pDoc->p_str_Ver_Del.Find(strMask[index]) == -1)
						break;
					index++;
				}while(true);
				int iAddIndex = index;
				CString strTemp = strText[i];
				//if(strTemp == L"æ")
				//	strTemp = L"\r\n";
				strVersion.Insert(iAddIndex,strTemp); 
				strMask.Insert(index, m_pDoc->m_str_Current_Version);
				index++;
			}
		}
		else if(strOper == "DELETE")
		{
			for(int i=0;i<strText.GetLength();i++)
			{
				do
				{
					if(index >= strMask.GetLength() || index >= strVersion.GetLength())
						break;
					if(m_pDoc->p_str_Ver_Del.Find(strMask[index]) == -1 &&
						strVersion[index] == strText[i])
						break;
					index++;
				}while(true);
				if (index >= strMask.GetLength())
					break;
				int iDelIndex = index;
				CString str_Ver_Del= m_pDoc->p_str_Ver_Del[m_pDoc->p_str_Ver_Del.Find(m_pDoc->m_str_Current_Ver_Del) + m_pDoc->p_str_Ver.Find(strMask[index])-1];
				if(index < strMask.GetLength())
					strMask.Delete(iDelIndex, 1);
				strMask.Insert(index, str_Ver_Del);
				index++;
			}
		}else
		{
			int itest = strText.GetLength();
			for(int i=0;i<strText.GetLength();i++)
			{
				do
				{
					if(index >= strMask.GetLength())
						break;
					if(m_pDoc->p_str_Ver_Del.Find(strMask[index]) == -1)
						break;
					index++;
				}while(true);
				index++;
			}
		}
		first = false;
    }
	//strVersion.Replace(L"æ", L"\r\n");
	return true;
}


Diffs CRebuildHistory::diffList(Diff d1, Diff d2, Diff d3, Diff d4, Diff d5,
  Diff d6, Diff d7, Diff d8, Diff d9, Diff d10) 
{
  // Diff(INSERT, NULL) is invalid and thus is used as the default argument.
  Diffs listRet;
  if (d1.operation == INSERT && d1.text.empty()) {
    return listRet;
  }
  listRet.push_back(d1);

  if (d2.operation == INSERT && d2.text.empty()) {
    return listRet;
  }
  listRet.push_back(d2);

  if (d3.operation == INSERT && d3.text.empty()) {
    return listRet;
  }
  listRet.push_back(d3);

  if (d4.operation == INSERT && d4.text.empty()) {
    return listRet;
  }
  listRet.push_back(d4);

  if (d5.operation == INSERT && d5.text.empty()) {
    return listRet;
  }
  listRet.push_back(d5);

  if (d6.operation == INSERT && d6.text.empty()) {
    return listRet;
  }
  listRet.push_back(d6);

  if (d7.operation == INSERT && d7.text.empty()) {
    return listRet;
  }
  listRet.push_back(d7);

  if (d8.operation == INSERT && d8.text.empty()) {
    return listRet;
  }
  listRet.push_back(d8);

  if (d9.operation == INSERT && d9.text.empty()) {
    return listRet;
  }
  listRet.push_back(d9);

  if (d10.operation == INSERT && d10.text.empty()) {
    return listRet;
  }
  listRet.push_back(d10);

  return listRet;
}