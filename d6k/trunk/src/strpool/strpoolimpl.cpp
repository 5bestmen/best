
#include <ace/ACE.h>
#include "ace\OS_NS_unistd.h"
#include "ace/Thread_Manager.h"
#include "ace/CDR_Stream.h"

#include "Utils/misc.h"
#include "public.h"

#include "StrPoolImpl.h"

CStrPoolImpl g_StrPoolImpl;

CStrPoolImpl::CStrPoolImpl(void)
{
	ACE::init();

	m_nActiveLang = LANG_zh_CN;
}

CStrPoolImpl::~CStrPoolImpl(void)
{
	for (mapStrPool::iterator b = m_mapStringPool.begin(); b != m_mapStringPool.end(); b++)
	{
		mapStrPool::iterator itr = b;
		CArray<STRINGDATA>* pPool = (CArray<STRINGDATA>*)itr->second;
		if (pPool != NULL)
		{
			delete pPool;
			pPool = NULL;
		}
	}
	m_mapStringPool.clear();

	ACE::fini();
}

bool CStrPoolImpl::CreateStringPool(TiXmlElement* pStringElement)
{
	int nCount = GetElementChildCount(pStringElement, "s");
	if (nCount <= 0)
	{
		return false;
	}

	//记录所有字符串池langid的数组
	if (!m_arrayStringPoolIndex.CreateArray(SHAREMEM_BASE_ID_STRPOOL, nCount))
		return false;

	//创建所有字符串池
	TiXmlNode *pChild;
	int nIndex = 0;
	for (pChild = pStringElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		std::string strChildName = pChild->Value();

		if (!Utils::IsEqualIgnoreCase(strChildName, "s"))
			continue;

		TiXmlElement* pLangElement = pChild->ToElement();
		if (pLangElement == NULL)
			continue;

		InnerCreateStringPool(pLangElement,nIndex++);
	}

	return true;
}

bool CStrPoolImpl::InnerCreateStringPool(TiXmlElement* pLangElement, int nIndex)
{
	std::string strLang;
	pLangElement->QueryStringAttribute("lang", &strLang);

	int nLang = LangStringToID(strLang);
	if (nLang == LANG_NONE)
		return false;
	
	CArray<STRINGDATA>* pLangArray = m_mapStringPool[nLang];
	if (pLangArray == NULL)
	{
		pLangArray = new CArray<STRINGDATA>;
		m_mapStringPool[nLang] = pLangArray;
	}

	int nStringCount = GetElementChildCount(pLangElement,"l");

	if (!pLangArray->CreateArray(SHAREMEM_BASE_ID_STRPOOL + nLang, nStringCount))
	{
		return false;
	}

	TiXmlNode* pChild;
	for (pChild = pLangElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		std::string strChildName = pChild->Value();

		if (!Utils::IsEqualIgnoreCase(strChildName, "l"))
			continue;

		//获取string信息
		TiXmlElement* pStrElement = pChild->ToElement();

		//获取属性值
		int nOccNO = 0;
		std::string strValue;

		pStrElement->QueryIntAttribute("OccNO", &nOccNO);
		pStrElement->QueryStringAttribute("Name", &strValue);

		STRINGDATA* pStrData = pLangArray->GetObject(nOccNO);//occNo从1开始
		if (pStrData == NULL)
		{
			return false;
		}

		//初始化STRINGDATA对象
		STRING_ITEM strItem;
		strItem.m_nOccNo = nOccNO;
		strItem.m_string.SetMsg(strValue.c_str());

		ACE_OutputCDR payload(sizeof(STRINGDATA));
		payload << strItem;

		::memcpy(((char*)pStrData), payload.begin()->rd_ptr(), payload.total_length());
	}


	//将当前字符串池记录进m_arrayStringPoolIndex;
	int* pLang =  m_arrayStringPoolIndex.GetObject(nIndex);
	*pLang = nLang;

	return true;
}

int CStrPoolImpl::LangStringToID(std::string strLang)
{
	if (Utils::IsEqualIgnoreCase(strLang, "en"))
		return LANG_EN;

	if (Utils::IsEqualIgnoreCase(strLang, "zh-CN"))
		return LANG_zh_CN;

	return LANG_NONE;
}

bool CStrPoolImpl::DestroyStringPool()
{
	CloseStringPool();

	return true;
}

bool CStrPoolImpl::OpenStringPool()
{
	if (!m_arrayStringPoolIndex.OpenArray(SHAREMEM_BASE_ID_STRPOOL))
		return false;

	int nSize = m_arrayStringPoolIndex.GetArraySize();

	for (int i = 0; i < nSize; i++)
	{
		int nLang = *(m_arrayStringPoolIndex.GetObject(i));

		CArray<STRINGDATA>* pStringPool = m_mapStringPool[nLang];
		if (pStringPool == NULL)
		{
			pStringPool = new CArray<STRINGDATA>;
			m_mapStringPool[nLang] = pStringPool;
			if (!pStringPool->OpenArray(SHAREMEM_BASE_ID_STRPOOL + nLang))
				return false;
		}
	}

	return true;
}

bool CStrPoolImpl::CloseStringPool()
{
	for (mapStrPool::iterator b = m_mapStringPool.begin(); b != m_mapStringPool.end(); b++)
	{
		CArray<STRINGDATA>* pPool = (CArray<STRINGDATA>*)b->second;
		if (pPool != NULL)
		{
			pPool->CloseArray();
		}
	}

	m_arrayStringPoolIndex.CloseArray();

	return true;
}

bool CStrPoolImpl::SetActiveStringPool(int lang)
{
	m_nActiveLang = lang;

	return true;
}

bool CStrPoolImpl::LoadStringFromPool(std::string& str, int nStrOccNo, int nLang)
{
	int nFromLang = m_nActiveLang;
	if (nLang != LANG_NONE)
		nFromLang = nLang;

	CArray<STRINGDATA>* pStringPool = m_mapStringPool[nFromLang];
	if (pStringPool == NULL)
		return false;

	STRINGDATA* pStrData = pStringPool->GetObject(nStrOccNo);

	ACE_InputCDR cdr((char*)pStrData, sizeof(STRINGDATA));
	STRING_ITEM strItem;
	cdr >> strItem;

	str = (char*)(strItem.m_string.GetMsg());

	if (strItem.m_nOccNo != nStrOccNo)
		return false;
	
	return true;
}

int CStrPoolImpl::GetElementChildCount(TiXmlElement* pElement, std::string strTag)
{
	int nCount = 0;
	TiXmlNode *pTemp = NULL;
	for (pTemp = pElement->FirstChild(); pTemp != NULL; pTemp = pTemp->NextSibling())
	{
		std::string strChildName = pTemp->Value();

		if (!Utils::IsEqualIgnoreCase(strChildName, strTag))
			continue;

		nCount++;
	}
	return nCount;
}
