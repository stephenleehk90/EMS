// ReplaceFactory.cpp: implementation of the CReplaceFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReplaceFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRepFactory *CRepFactory::_instance = 0;
//CRepFactory 
CRepFactory *CRepFactory::Instance()
{
	if(_instance==0)
		_instance = new CRepFactory;
	return _instance;
}
CRepFactory::CRepFactory()
{
	m_pmapSymbol = new CMapStringToSymbol;
	m_pmapSpellingOverAll = new CMapStringToString;
	m_pmapSpellingByCase = new CMapStringToString;
	m_pmapSynonymns = new CMapStringToMyObject;
	m_nCountryCode = -1;
	m_bInit = FALSE;
}
BOOL CRepFactory::Init(int nCountryCode)
{
	if(nCountryCode == m_nCountryCode)
		return TRUE;
	if(m_bInit)
		RemoveAll();
	CGlobal *pGL=CGlobal::GetInstance();
	if(pGL->LoadSymbol(m_pmapSymbol,nCountryCode)&&
		pGL->LoadSpellingOverAll(m_pmapSpellingOverAll,nCountryCode)&&
		pGL->LoadSpellingByCase(m_pmapSpellingByCase,nCountryCode)&&
		pGL->LoadSynonymns(m_pmapSynonymns,nCountryCode))
	{
		m_nCountryCode = nCountryCode;
		m_bInit = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}
void CRepFactory::Release()
{
	if(m_pmapSymbol)
		delete m_pmapSymbol;
	if(m_pmapSpellingOverAll)
		delete m_pmapSpellingOverAll;
	if(m_pmapSpellingByCase)
		delete m_pmapSpellingByCase;
	if(m_pmapSynonymns)
		delete m_pmapSynonymns;
}
void CRepFactory::RemoveAll()
{
	if(m_pmapSymbol)
	{
		POSITION pos = m_pmapSymbol->GetStartPosition();
		while (pos != NULL)
		{
			CString str;
			SYMBOL* pMyObject;
			m_pmapSymbol->GetNextAssoc(pos, str, pMyObject);
			delete pMyObject;
		}
		m_pmapSymbol->RemoveAll();
		
	}
	if(m_pmapSpellingOverAll)
	{
		m_pmapSpellingOverAll->RemoveAll();
	}
	if(m_pmapSpellingByCase)
	{
		m_pmapSpellingByCase->RemoveAll();
	}
	if(m_pmapSynonymns)
	{
		POSITION pos = m_pmapSynonymns->GetStartPosition();
		while (pos != NULL)
		{
			CString str;
			CStringList* pMyObject;
			m_pmapSynonymns->GetNextAssoc(pos, str, pMyObject);
			delete pMyObject;
		}
		m_pmapSynonymns->RemoveAll();
	}
}
CRepFactory::~CRepFactory()
{
	RemoveAll();
	Release();
}

void CRepFactory::UnLoad()
{
	if(this)
		delete this;
}
