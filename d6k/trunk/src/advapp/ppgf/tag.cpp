/*! @file tag.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  tag.h
�ļ�ʵ�ֹ��� :  ��㶨��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��㶨��
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.24
*******************************************************************************/

#include "tag.h" 
#include <QDebug>

#include "scadaapi/scdsvcapi.h"
#include "fesapi/fesdatadef.h"
 

CDinData::CDinData()
{

}

CDinData::~CDinData()
{

}
/*! \fn bool CDinData::GetOccNoByTagName()
********************************************************************************************************* 
** \brief CDinData::GetOccNoByTagName 
** \details ����OccNo
** \return bool 
** \author LiJin 
** \date 2017��3��24�� 
** \note 
********************************************************************************************************/
bool CDinData::GetOccNoByTagName()
{
	QString szLog;

	Q_ASSERT(m_szTagName.empty() == false);
	if (m_szTagName.empty() == true)
	{// TODO:  LOG
		szLog = QString(QObject::tr("Find empty TagName."));
		LogMsg(szLog.toStdString().c_str(),0);
		return false;
	}

	if (m_pOccNoItem == nullptr)
	{
		m_pOccNoItem = std::make_shared<struct TAGITEM>();
	}

	bool bRet = false;

	bRet = ::GetOccNoByTagName(m_szTagName.c_str(), &m_pOccNoItem->NodeOccNo, &m_pOccNoItem->IddType, &m_pOccNoItem->TagOccNo, &m_pOccNoItem->FiledID);
	if (bRet == false)
	{
		szLog = QString(QObject::tr("[%1] convert OccNo failed.")).arg(m_szTagName.c_str());
		LogMsg(szLog.toStdString().c_str(),0);
	}

	return bRet;
}

bool CDinData::GetRealTimeData()
{
	bool bRet = false;
	if (m_pOccNoItem == nullptr)
	{
		bRet = GetOccNoByTagName();
		if (bRet == false)
		{			
			return false;
		}	
	}
	if (m_pOccNoItem == nullptr)
	{
		return false;
	}

	IO_VARIANT var;
	bRet = ::GetRTData(m_pOccNoItem->IddType, m_pOccNoItem->NodeOccNo, m_pOccNoItem->TagOccNo, m_pOccNoItem->FiledID, &var);
	if (bRet == false)
	{// TODO: log
		QString szLog;
		szLog = QString(QObject::tr("[%1] GetRTData failed.")).arg(m_szTagName.c_str());
		LogMsg(szLog.toStdString().c_str(), 0);
	}
	 
	m_Value = ConvertVariant<int8u>(var);	

	return bRet;
}


CAinData::CAinData()
{

}

CAinData::~CAinData()
{

}

bool CAinData::GetOccNoByTagName()
{
	QString szLog;

	Q_ASSERT(m_szTagName.empty() == false);
	if (m_szTagName.empty() == true)
	{// TODO:  LOG
		szLog = QString(QObject::tr("Find empty TagName."));
		LogMsg(szLog.toStdString().c_str(), 0);
		return false;
	}

	if (m_pOccNoItem == nullptr)
	{
		m_pOccNoItem = std::make_shared<struct TAGITEM>();
	}

	bool bRet = false;

	bRet = ::GetOccNoByTagName(m_szTagName.c_str(), &m_pOccNoItem->NodeOccNo, &m_pOccNoItem->IddType, &m_pOccNoItem->TagOccNo, &m_pOccNoItem->FiledID);
	if (bRet == false)
	{
		szLog = QString(QObject::tr("[%1] convert OccNo failed.")).arg(m_szTagName.c_str());
		LogMsg(szLog.toStdString().c_str(), 0);
	}

	return bRet;
}

bool CAinData::GetRealTimeData()
{
	bool bRet = false;
	if (m_pOccNoItem == nullptr)
	{
		bRet = GetOccNoByTagName();
		if (bRet == false)
		{
			return false;
		}
	}
	if (m_pOccNoItem == nullptr)
	{
		return false;
	}

	IO_VARIANT var;

	bRet = ::GetRTData(m_pOccNoItem->IddType, m_pOccNoItem->NodeOccNo, m_pOccNoItem->TagOccNo, m_pOccNoItem->FiledID, &var);
	if (bRet == false)
	{// TODO: log
		QString szLog;
		szLog = QString(QObject::tr("[%1] GetRTData failed.")).arg(m_szTagName.c_str());
		LogMsg(szLog.toStdString().c_str(), 0);
	}

	m_Value = ConvertVariant<fp64>(var);

	return bRet;
}

/** @}*/

