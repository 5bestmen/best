/*! @file
<PRE>
********************************************************************************
ģ����       :  �澯�ṹ
�ļ���       :  alarm_data.cpp
�ļ�ʵ�ֹ��� :  ת�����ýṹ
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date	2016.11.9
*/

#include "alarm_data.h"
#include "scadastudiodefine.h"
#include "stl_util-inl.h"

#include <QObject>


namespace Config
{
	/*! \fn CAnalogAlarm::CAnalogAlarm()
	*********************************************************************************************************
	** \brief CAnalogAlarm::CAnalogAlarm
	** \details
	** \return
	** \author gw
	** \date 2016��1��11��
	** \note
	********************************************************************************************************/
	CAnalogAlarm::CAnalogAlarm()
	{
		m_nID = 0;
		m_nAlarmType = AIALARM;
		m_nOccNo = 0;
		m_bEnable = true;
		m_nDeadType = DeadType::Range;			//��������
		m_dbDeadArea = 0;			//����
		m_nDelayAlarm = 0;
		m_bOnQualityGood = true;
	}

	/*! \fn CAnalogAlarm::~CAnalogAlarm()
	*********************************************************************************************************
	** \brief CNonlinearTransform::~CNonlinearTransform
	** \details
	** \return
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	CAnalogAlarm::~CAnalogAlarm()
	{
		Clear();
	}

	/*! \fn void CAnalogAlarm::Save()
	*********************************************************************************************************
	** \brief CAnalogAlarm::Save
	** \details
	** \return void
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	void CAnalogAlarm::Save()
	{

	}

	void CAnalogAlarm::Clear()
	{
		for (size_t i = 0; i < m_arrAlarmOfflimit.size(); i++)
		{
			delete m_arrAlarmOfflimit[i];
		}
		m_arrAlarmOfflimit.clear();
	}

	/*! \fn bool CAnalogAlarm::AddAinAlarmLimit(CAinAlarmLimit *pAinAlarmLimit)
	********************************************************************************************************* 
	** \brief Config::CAnalogAlarm::AddAinAlarmLimit 
	** \details ģ�����澯��ֵ��ӵ�ģ�����澯
	** \param pAinAlarmLimit 
	** \return bool 
	** \author gw
	** \date 2016��11��11�� 
	** \note 
	********************************************************************************************************/
	bool CAnalogAlarm::AddAinAlarmLimit(CAinAlarmLimit *pAinAlarmLimit)
	{
		Q_ASSERT(pAinAlarmLimit);
		if (!pAinAlarmLimit)
		{
			return false;
		}

		m_arrAlarmOfflimit.push_back(pAinAlarmLimit);

		return true;
	}


	/*! \fn 
	********************************************************************************************************* 
	** \brief Config::CAnalogAlarm::DeleteItem 
	** \details 
	** \param pAinAlarmLimit 
	** \return bool 
	** \author gw
	** \date 2016��11��15�� 
	** \note 
	********************************************************************************************************/
	bool CAnalogAlarm::DeleteItem(CAinAlarmLimit *pAinAlarmLimit)
	{
		Q_ASSERT(pAinAlarmLimit);
		if (!pAinAlarmLimit)
		{
			return false;
		}

		auto it = std::find(m_arrAlarmOfflimit.begin(), m_arrAlarmOfflimit.end(), pAinAlarmLimit);
		if (it != m_arrAlarmOfflimit.end())
		{
			m_arrAlarmOfflimit.erase(it);

			delete pAinAlarmLimit;
			pAinAlarmLimit = nullptr;

			return true;
		}

		return false;
	}

	CAnalogAlarmGroup::CAnalogAlarmGroup()
	{

	}

	CAnalogAlarmGroup::~CAnalogAlarmGroup()
	{
		STLDeleteElements(&m_arrAnalogGroup);
		STLDeleteElements(&m_arrAnalogItem);
	}

	/*! \fn bool CAnalogGroup::AddAnalogGroup(CAnalogGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CAnalogGroup::AddAnalogGroup 
	** \details ��Ӹ澯��
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016��11��9�� 
	** \note 
	********************************************************************************************************/
	bool CAnalogAlarmGroup::AddAnalogGroup(CAnalogAlarmGroup *pGroup)
	{
		m_arrAnalogGroup.push_back(pGroup);
		return true;
	}


	bool CAnalogAlarmGroup::AddAnalogItem(CAnalogAlarm *pItem)
	{
		m_arrAnalogItem.push_back(pItem);
		return true;
	}


	/*! \fn bool CAnalogGroup::CheckModifyGroupNameExist(CAnalogGroup *pGroup, QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CAnalogGroup::CheckModifyGroupNameExist 
	** \details ��������Ƿ����
	** \param pGroup 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016��11��10�� 
	** \note 
	********************************************************************************************************/
	bool CAnalogAlarmGroup::CheckModifyGroupNameExist(CAnalogAlarmGroup *pGroup, QString &strTagName)
	{
		for each (auto var in m_arrAnalogGroup)
		{
			if (var != pGroup)
			{
				if (strTagName.compare(var->m_strGroupName, Qt::CaseSensitive) == 0)
				{
					return true;
				}
			}
		}

		return false;
	}


	/*! \fn bool CAnalogAlarmGroup::CheckGroupNameExist(QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CAnalogAlarmGroup::CheckGroupNameExist 
	** \details ���group name�Ƿ����
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016��11��14�� 
	** \note 
	********************************************************************************************************/
	bool CAnalogAlarmGroup::CheckGroupNameExist(QString &strTagName)
	{
		for each (auto var in m_arrAnalogGroup)
		{
				if (strTagName.compare(var->m_strGroupName, Qt::CaseSensitive) == 0)
				{
					return true;
				}
		}

		return false;
	}


	/*! \fn bool CAnalogAlarmGroup::DeleteGroup(QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CAnalogAlarmGroup::DeleteGroup 
	** \details ɾ�����������
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016��11��14�� 
	** \note 
	********************************************************************************************************/
	bool CAnalogAlarmGroup::DeleteGroup(CAnalogAlarmGroup *pChildGroup)
	{
		auto it = std::find(m_arrAnalogGroup.begin(), m_arrAnalogGroup.end(), pChildGroup);
		if (it != m_arrAnalogGroup.end())
		{
			m_arrAnalogGroup.erase(it);

			delete pChildGroup;
			pChildGroup = nullptr;

			return true;
		}

		return false;
	}


	/*! \fn bool CAnalogAlarmGroup::DeleteItem(CAnalogAlarm *pItem)
	********************************************************************************************************* 
	** \brief Config::CAnalogAlarmGroup::DeleteItem 
	** \details 
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2016��11��14�� 
	** \note 
	********************************************************************************************************/
	bool CAnalogAlarmGroup::DeleteItem(CAnalogAlarm *pItem)
	{
		auto it = std::find(m_arrAnalogItem.begin(), m_arrAnalogItem.end(), pItem);
		if (it != m_arrAnalogItem.end())
		{
			m_arrAnalogItem.erase(it);

			delete pItem;
			pItem = nullptr;

			return true;
		}

		return false;
	}

	/*! \fn CAnalogGroup *CAnalogGroup::CreateGroup()
	********************************************************************************************************* 
	** \brief Config::CAnalogGroup::CreateGroup 
	** \details ������
	** \return CAnalogGroup * 
	** \author gw
	** \date 2016��11��10�� 
	** \note 
	********************************************************************************************************/
	CAnalogAlarmGroup *CAnalogAlarmGroup::CreateGroup()
	{
		int nIndex = (int)m_arrAnalogGroup.size();
		QString strName = "";
		bool bFlag = false;
		while (!bFlag)
		{
			strName = QString("%1%2").arg(GROUP_NAME).arg(nIndex);

			if (CheckGroupNameExist(strName))
			{
				nIndex++;
				continue;
			}

			auto pGroup = new CAnalogAlarmGroup;
			pGroup->SetGroupName(strName);

			m_arrAnalogGroup.push_back(pGroup);

			return pGroup;
		}

		return nullptr;
	}


	CDinAlarmLimit::CDinAlarmLimit()
	{
		OccNo = 0;
		BlockNo = 0;
		memset(TagName, 0, sizeof(TagName));
		Description = "";
		ID = 0;
		Condition = ONETOZERO;
		Category = AlarmCategory::COMMON;
		DelayAlarm = 0;
		Priority = 0;
		SupportAck = true;
		SupportDelete = true;
		AckType = AlarmAckType::DELETE;
		SoundFile = "";
		PlaySoundTimes = 0;
		SpeechAlarmText = "";
		Beep = false;
		PushGraph = "";
		BlinkGraph = false;
		Log = false;
		BackColor = 0;
		TextColor = 0;
		BlinkBackColor = 0;
		BlinkTextColor = 0;
		DispGuide = "";
		Commands = "";
	}


	CDigitalAlarm::CDigitalAlarm()
	{
		m_nOccNo = 0;
		m_bEnable = true;
		m_nID = 0;
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_nDelayAlarm = 0;
		m_bOnQualityGood = true;
	}

	/*! \fn CAnalogAlarm::~CAnalogAlarm()
	*********************************************************************************************************
	** \brief CNonlinearTransform::~CNonlinearTransform
	** \details
	** \return
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	CDigitalAlarm::~CDigitalAlarm()
	{
		Clear();
	}

	/*! \fn void CAnalogAlarm::Save()
	*********************************************************************************************************
	** \brief CAnalogAlarm::Save
	** \details
	** \return void
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	void CDigitalAlarm::Save()
	{

	}

	void CDigitalAlarm::Clear()
	{
		for (size_t i = 0; i < m_arrDigtalOfflimit.size(); i++)
		{
			delete m_arrDigtalOfflimit[i];
		}
		m_arrDigtalOfflimit.clear();
	}


	/*! \fn bool CDigitalAlarm::AddDinAlarmLimit(ALARM_OFFLIMIT_DI *pDinAlarmLimit)
	********************************************************************************************************* 
	** \brief Config::CDigitalAlarm::AddDinAlarmLimit 
	** \details 
	** \param pDinAlarmLimit 
	** \return bool 
	** \author gw
	** \date 2016��11��11�� 
	** \note 
	********************************************************************************************************/
	bool CDigitalAlarm::AddDinAlarmLimit(CDinAlarmLimit *pDinAlarmLimit)
	{
		Q_ASSERT(pDinAlarmLimit);
		if (!pDinAlarmLimit)
		{
			return false;
		}

		m_arrDigtalOfflimit.push_back(pDinAlarmLimit);
		
		return true;
	}


	/*! \fn bool CDigitalAlarm::DeleteItem(CDinAlarmLimit *pDinAlarmLimit)
	********************************************************************************************************* 
	** \brief Config::CDigitalAlarm::DeleteItem 
	** \details ɾ����
	** \param pDinAlarmLimit 
	** \return bool 
	** \author gw
	** \date 2016��11��15�� 
	** \note 
	********************************************************************************************************/
	bool CDigitalAlarm::DeleteItem(CDinAlarmLimit *pDinAlarmLimit)
	{
		Q_ASSERT(pDinAlarmLimit);
		if (!pDinAlarmLimit)
		{
			return false;
		}

		auto it = std::find(m_arrDigtalOfflimit.begin(), m_arrDigtalOfflimit.end(), pDinAlarmLimit);
		if (it != m_arrDigtalOfflimit.end())
		{
			m_arrDigtalOfflimit.erase(it);

			delete pDinAlarmLimit;
			pDinAlarmLimit = nullptr;

			return true;
		}

		return false;
	}

	/*! \fn CDigitalGroup::CDigitalGroup()
	********************************************************************************************************* 
	** \brief Config::CDigitalGroup::CDigitalGroup 
	** \details ��������
	** \return  
	** \author gw
	** \date 2016��11��9�� 
	** \note 
	********************************************************************************************************/
	CDigitalAlarmGroup::CDigitalAlarmGroup()
	{

	}
	CDigitalAlarmGroup::~CDigitalAlarmGroup()
	{
		STLDeleteElements(&m_arrDigitalGroup);
		STLDeleteElements(&m_arrDigitalItem);
	}


	/*! \fn CDigitalAlarmGroup *CDigitalAlarmGroup::CreateGroup()
	********************************************************************************************************* 
	** \brief Config::CDigitalAlarmGroup::CreateGroup 
	** \details �������������
	** \return CDigitalAlarmGroup * 
	** \author gw
	** \date 2016��11��11�� 
	** \note 
	********************************************************************************************************/
	CDigitalAlarmGroup *CDigitalAlarmGroup::CreateGroup()
	{
		int nIndex = (int)m_arrDigitalGroup.size();
		QString strName = "";
		while (true)
		{
			strName = QString("%1%2").arg(GROUP_NAME).arg(nIndex);

			if (CheckGroupNameExist(strName))
			{
				nIndex++;

				continue;
			}

			auto pGroup = new CDigitalAlarmGroup;
			pGroup->SetGroupName(strName);

			m_arrDigitalGroup.push_back(pGroup);

			return pGroup;
		}

		return nullptr;
	}

	/*! \fn bool CDigitalGroup::AddDigitalGroup(CAnalogGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CDigitalGroup::AddDigitalGroup 
	** \details ��ӿ�������
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016��11��9�� 
	** \note 
	********************************************************************************************************/
	bool CDigitalAlarmGroup::AddDigitalGroup(CDigitalAlarmGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		m_arrDigitalGroup.push_back(pGroup);
		return true;
	}

	/*! \fn bool CDigitalAlarmGroup::DeleteGroup(CDigitalAlarmGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CDigitalAlarmGroup::DeleteGroup 
	** \details ɾ����
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016��11��15�� 
	** \note 
	********************************************************************************************************/
	bool CDigitalAlarmGroup::DeleteGroup(CDigitalAlarmGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto it = std::find(m_arrDigitalGroup.begin(), m_arrDigitalGroup.end(), pGroup);
		if (it != m_arrDigitalGroup.end())
		{
			m_arrDigitalGroup.erase(it);
			delete pGroup;
			pGroup = nullptr;

			return true;
		}

		return false;
	}


	/*! \fn bool CDigitalAlarmGroup::DeleteItem(CDigitalAlarm *pItem)
	********************************************************************************************************* 
	** \brief Config::CDigitalAlarmGroup::DeleteItem 
	** \details 
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2016��11��15�� 
	** \note 
	********************************************************************************************************/
	bool CDigitalAlarmGroup::DeleteItem(CDigitalAlarm *pItem)
	{
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return false;
		}

		auto it = std::find(m_arrDigitalItem.begin(), m_arrDigitalItem.end(), pItem);
		if (it != m_arrDigitalItem.end())
		{
			m_arrDigitalItem.erase(it);
			delete pItem;
			pItem = nullptr;

			return true;
		}

		return false;
	}

	/*! \fn bool CDigitalGroup::AddDigitalItem(CAnalogAlarm *pItem)
	********************************************************************************************************* 
	** \brief Config::CDigitalGroup::AddDigitalItem 
	** \details ��ӿ�����
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2016��11��9�� 
	** \note 
	********************************************************************************************************/
	bool CDigitalAlarmGroup::AddDigitalItem(CDigitalAlarm *pItem)
	{
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return false;
		}

		m_arrDigitalItem.push_back(pItem);
		return true;
	}

	/*! \fn bool CDigitalGroup::CheckModifyGroupNameExist(CDigitalGroup *pGroup, QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CDigitalGroup::CheckModifyGroupNameExist 
	** \details ����޸ĵĿ��������������Ƿ����
	** \param pGroup 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016��11��10�� 
	** \note 
	********************************************************************************************************/
	bool CDigitalAlarmGroup::CheckModifyGroupNameExist(CDigitalAlarmGroup *pGroup, QString &strTagName)
	{
		for each (auto var in m_arrDigitalGroup)
		{
			if (var != pGroup)
			{
				if (strTagName.compare(var->m_strGroupName, Qt::CaseSensitive) == 0)
				{
					return true;
				}
			}
		}

		return false;
	}


	/*! \fn bool CDigitalAlarmGroup::CheckGroupNameExist(CDigitalAlarmGroup *pGroup, QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CDigitalAlarmGroup::CheckGroupNameExist 
	** \details ��������Ƿ����
	** \param pGroup 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016��11��14�� 
	** \note 
	********************************************************************************************************/
	bool CDigitalAlarmGroup::CheckGroupNameExist(QString &strTagName)
	{
		for each (auto var in m_arrDigitalGroup)
		{
			if (strTagName.compare(var->m_strGroupName, Qt::CaseSensitive) == 0)
			{
				return true;
			}
		}

		return false;
	}
}
