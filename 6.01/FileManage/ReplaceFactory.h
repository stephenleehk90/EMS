// ReplaceFactory.h: interface for the CReplaceFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPLACEFACTORY_H__216B1721_9FE2_11D6_963B_0050BAF27B24__INCLUDED_)
#define AFX_REPLACEFACTORY_H__216B1721_9FE2_11D6_963B_0050BAF27B24__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CRepFactory
{
public:
	void UnLoad();
	CMapStringToSymbol *m_pmapSymbol;
	CMapStringToString *m_pmapSpellingOverAll;
	CMapStringToString *m_pmapSpellingByCase;
	CMapStringToMyObject *m_pmapSynonymns;
	static CRepFactory* Instance();
	BOOL Init(int nCountryCode);
protected:
	CRepFactory();
	~CRepFactory();
	void Release();
	void RemoveAll();
private:
	static CRepFactory *_instance;
	int m_nCountryCode;
	BOOL m_bInit;
};


#endif // !defined(AFX_REPLACEFACTORY_H__216B1721_9FE2_11D6_963B_0050BAF27B24__INCLUDED_)
