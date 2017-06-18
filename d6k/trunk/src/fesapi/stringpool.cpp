#include "stringpool.h"
#include "fesapi/fescnst.h"
#include "../fespub/sharemem.h"
#include "log/log.h"
#include <qglobal.h>
#include <QObject>
#include <QString>

CStringPool::CStringPool()
{
	m_pMem = std::make_shared<CShareMem>();
}


CStringPool::~CStringPool()
{
}

bool CStringPool::Initialize(const char* pszProjName, int32u nMode)
{
	Q_ASSERT(pszProjName && strlen(pszProjName)>0);
	if (!pszProjName || strlen(pszProjName)==0)
	{
		return false;
	}
	if (m_bInitialized == true)
	{
		return true;
	}

	QString szLog;
	QString szStringName = QString(pszProjName)+"-stringpool";
	char* pData = (char*)(m_pMem->GetShareMem(szStringName.toStdString().c_str()));
	//m_pMem->Lock();
	Q_ASSERT(pData);
	if (pData == nullptr)
	{
		szLog = QString(QObject::tr("StringPool:Open share memory [ %1 ] failed")).arg(szStringName);
		LogMsg(szLog.toStdString().c_str(), 0);
		//m_pMem->UnLock();
		return false;
	}

	m_pStringHead = reinterpret_cast<HEAD_STRING*>(pData);
	
	Q_ASSERT(pData);
	if (!pData)
	{
		//m_pMem->UnLock();
		return false;
	}

	Q_ASSERT(m_pStringHead->MagicHead1 == MAGIC_HEAD);
	Q_ASSERT(m_pStringHead->MagicHead2 == MAGIC_HEAD);
	m_nStringsCount = m_pStringHead->TotalCount;
	Q_ASSERT(m_nStringsCount !=INVALID_OCCNO && m_nStringsCount <=MAX_OCCNO);
	if (m_nStringsCount==INVALID_OCCNO || m_nStringsCount >MAX_OCCNO)
	{
		//m_pMem->UnLock();
		return false;
	}

	StringUnit* m_pData = reinterpret_cast<StringUnit* >(pData + sizeof HEAD_STRING);

	for (int i=0;i<m_nStringsCount;++i)
	{
		Q_ASSERT(m_pData[i].OccNo != INVALID_OCCNO && m_pData[i].OccNo <= MAX_OCCNO);
		if (m_pData[i].OccNo == INVALID_OCCNO || m_pData[i].OccNo > MAX_OCCNO )
		{
			szLog = QString(QObject::tr("OccNo [ %1 ] stringpool's length is wrong ")).arg(m_pData[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		Q_ASSERT(strlen(m_pData[i].Desc) > 0 && strlen(m_pData[i].Desc) <= MAX_NAME_LENGTH);
		if (strlen(m_pData[i].Desc) <= 0 || strlen(m_pData[i].Desc) > MAX_NAME_LENGTH)
		{
			szLog = QString(QObject::tr("OccNo [ %1 ] stringpool's length is wrong ")).arg(m_pData[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		auto bRet=m_MapStrings.insert(std::make_pair(m_pData[i].OccNo,&m_pData[i]));
		Q_ASSERT(bRet.second==true);
	}

	return true;
}

void CStringPool::Run()
{

}

void CStringPool::Shutdown()
{

}

bool CStringPool::GetDescByOccno(int nOccNo, bool bIsTranslate, char* pBuff)
{
	Q_ASSERT(nOccNo !=INVALID_OCCNO && nOccNo <=MAX_OCCNO);
	if (nOccNo ==INVALID_OCCNO || nOccNo >MAX_OCCNO)
	{
		return false;
	}
	Q_ASSERT(pBuff && strlen(pBuff) >0);
	if (!pBuff || strlen(pBuff)==0)
	{
		return false;
	}
	StringUnit stringUint;
	bool bRet = GetStringPoolByOccNo(nOccNo, &stringUint);
	Q_ASSERT(bRet);
	if (!bRet)
	{
		return false;
	}
	if (bIsTranslate)
	{
		strncpy(pBuff,stringUint.Language,qMin(MAX_NAME_LENGTH + STRING_PAD,strlen(pBuff)));
	}
	else
	{
		strncpy(pBuff, stringUint.Desc, qMin(MAX_NAME_LENGTH + STRING_PAD, strlen(pBuff)));
	}
	return true;
}


bool CStringPool::GetStringPoolByOccNo(int32u nOccNo, StringUnit* pStringUnit)
{
	Q_ASSERT(nOccNo !=INVALID_OCCNO && nOccNo <=MAX_OCCNO);
	if (nOccNo ==INVALID_OCCNO || nOccNo >MAX_OCCNO)
	{
		return false;
	}
	auto iter = m_MapStrings.find(nOccNo);
	if (iter== m_MapStrings.end())
	{
		return false;
	}
	else
	{
		pStringUnit = iter->second;
		return true;
	}

	return false;
}

void CStringPool::LogMsg(const char * pszText, int nLevel)
{
	::LogMsg("StringPool", pszText, nLevel, nullptr);
}

