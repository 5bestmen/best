/*! @file
<PRE>
********************************************************************************
模块名       :  scada variable 模块
文件名       :  scadavirable_data.cpp
文件实现功能 :  scada variable模块
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2016.12.23
*/

#include "scadavirable_data.h"
#include "base_data.h"
#include "stl_util-inl.h"
#include "scadastudio/icore.h"
#include "scadavirable_data.h"
#include "scale_data.h"
#include "scadastudio/iprojmgr.h"
#include "variant.h"
#include "global.h"
#include "log/log2file.h"
#include "scadastudio/imodule.h"

#include <QDomDocument>
#include <QIODevice>
#include <QFile>
#include <QXmlStreamWriter>
#include <qobject.h>
#include <qmessagebox.h>

extern IMainModuleInterface *s_pGlobleCore;

namespace Config
{
	CScadaSystemVaraible::CScadaSystemVaraible()
	{
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_nID = 0;
		m_strDescription = "";
		m_nType = 0;
		m_nOccNo = 0;
	}

	CScadaSystemVaraible::~CScadaSystemVaraible()
	{

	}

	CScadaUserVariable::CScadaUserVariable()
	{
		//auto nTest = sizeof(m_szTagName);
		memset(m_szTagName, 0, sizeof(m_szTagName));
		
		m_strDescription = "";	//描述
		m_nID = 0;
		m_nType = DATA_TYPE::DT_DOUBLE;		//变量类型
		m_nInitialQua = InitialQua::NormalQua;
		m_dbInitValue = 0;
		m_nOccNo = 0;
		m_strAlarmTagName = "";
		m_nAlarmOccNo = 0;
		m_strScaleTagName = "";
		m_nAlarmType = 0;

		m_strSourceOccNo = "";
		m_strSourceTagName = "";
	}

	CScadaUserVariable::~CScadaUserVariable()
	{
		

	}


	CScadaUserVariableGroup::CScadaUserVariableGroup()
	{

	}

	CScadaUserVariableGroup::~CScadaUserVariableGroup()
	{
		STLDeleteElements(&m_arrGroup);
		STLDeleteElements(&m_arrItem);
	}

	/*! \fn bool CScadaUserVirableGroup::AddUserVariableGroup(CScadaUserVirableGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaUserVirableGroup::AddUserVariableGroup 
	** \details 添加用户变量组
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016年12月26日 
	** \note 
	********************************************************************************************************/
	bool CScadaUserVariableGroup::AddUserVariableGroup(CScadaUserVariableGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		m_arrGroup.push_back(pGroup);
		return true;
	}

	/*! \fn bool CScadaUserVirableGroup::AddUserVariableItem(CScadaUserVirable *pItem)
	********************************************************************************************************* 
	** \brief Config::CScadaUserVirableGroup::AddUserVariableItem 
	** \details 添加用户项
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2016年12月26日 
	** \note 
	********************************************************************************************************/
	bool CScadaUserVariableGroup::AddUserVariableItem(CScadaUserVariable *pItem)
	{
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return false;
		}

		m_arrItem.push_back(pItem);
		return true;
	}

	CScadaUserVariableGroup *CScadaUserVariableGroup::CreateGroup()
	{
		int nIndex = (int)m_arrGroup.size();
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

			auto pGroup = new CScadaUserVariableGroup;
			pGroup->SetGroupName(strName);

			m_arrGroup.push_back(pGroup);

			return pGroup;
		}

		return nullptr;
	}

	/*! \fn bool CScadaUserVariableGroup::CheckGroupNameExist(QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaUserVariableGroup::CheckGroupNameExist 
	** \details 检查组内的组名是否存在
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016年12月27日 
	** \note 
	********************************************************************************************************/
	bool CScadaUserVariableGroup::CheckGroupNameExist(QString &strTagName)
	{
		for each (auto var in m_arrGroup)
		{
			if (strTagName.compare(var->m_strGroupName, Qt::CaseSensitive) == 0)
			{
				return true;
			}
		}

		return false;
	}

	/*! \fn bool CScadaUserVariableGroup::DeleteGroup(CScadaUserVariableGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaUserVariableGroup::DeleteGroup 
	** \details 删除组
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016年12月29日 
	** \note 
	********************************************************************************************************/
	bool CScadaUserVariableGroup::DeleteGroup(CScadaUserVariableGroup *pGroup)
	{
		auto it = std::find(m_arrGroup.begin(), m_arrGroup.end(), pGroup);
		if (it == m_arrGroup.end())
		{
			return false;
		}

		m_arrGroup.erase(it);
		delete pGroup;
		pGroup = nullptr;

		return true;
	}


	CScadaAnalogAlarm::CScadaAnalogAlarm()
	{
		m_nID = 0;
		m_nAlarmType = AIALARM;
		m_nOccNo = 0;
		m_bEnable = true;
		m_nDeadType = DeadType::Range;			//死区类型
		m_dbDeadArea = 0;			//死区
		m_nDelayAlarm = 0;
		m_bOnQualityGood = true;

		m_arrAlarmOfflimit.clear();
	}


	CScadaAnalogAlarm::~CScadaAnalogAlarm()
	{
		Clear();
	}


	void CScadaAnalogAlarm::Save()
	{

	}

	void CScadaAnalogAlarm::Clear()
	{
		STLDeleteElements(&m_arrAlarmOfflimit);
	}


	/*! \fn CScadaAnalogAlarmGroup::CScadaAnalogAlarmGroup()
	********************************************************************************************************* 
	** \brief Config::CScadaAnalogAlarmGroup::CScadaAnalogAlarmGroup 
	** \details 
	** \return  
	** \author gw
	** \date 2017年1月9日 
	** \note 
	********************************************************************************************************/
	CScadaAnalogAlarmGroup::CScadaAnalogAlarmGroup()
	{

	}

	/*! \fn CScadaAnalogAlarmGroup::~CScadaAnalogAlarmGroup()
	********************************************************************************************************* 
	** \brief Config::CScadaAnalogAlarmGroup::~CScadaAnalogAlarmGroup 
	** \details 
	** \return  
	** \author gw
	** \date 2017年1月9日 
	** \note 
	********************************************************************************************************/
	CScadaAnalogAlarmGroup::~CScadaAnalogAlarmGroup()
	{
		STLDeleteElements(&m_arrAnalogGroup);
		STLDeleteElements(&m_arrAnalogItem);
	}

	/*! \fn bool CScadaAnalogAlarmGroup::AddAnalogGroup(CScadaAnalogAlarmGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaAnalogAlarmGroup::AddAnalogGroup 
	** \details 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	bool CScadaAnalogAlarmGroup::AddAnalogGroup(CScadaAnalogAlarmGroup *pGroup)
	{
		m_arrAnalogGroup.push_back(pGroup);
		return true;
	}

	/*! \fn bool CScadaAnalogAlarmGroup::AddAnalogItem(CScadaAnalogAlarm *pItem)
	********************************************************************************************************* 
	** \brief Config::CScadaAnalogAlarmGroup::AddAnalogItem 
	** \details 
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	bool CScadaAnalogAlarmGroup::AddAnalogItem(CScadaAnalogAlarm *pItem)
	{
		m_arrAnalogItem.push_back(pItem);
		return true;
	}

	/*! \fn CScadaAnalogAlarmGroup *CScadaAnalogAlarmGroup::CreateGroup()
	********************************************************************************************************* 
	** \brief Config::CScadaAnalogAlarmGroup::CreateGroup 
	** \details 
	** \return CScadaAnalogAlarmGroup * 
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	CScadaAnalogAlarmGroup *CScadaAnalogAlarmGroup::CreateGroup()
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

			auto pGroup = new CScadaAnalogAlarmGroup;
			pGroup->SetGroupName(strName);

			m_arrAnalogGroup.push_back(pGroup);

			return pGroup;
		}

		return nullptr;
	}

	/*! \fn bool CScadaAnalogAlarmGroup::CheckGroupNameExist(QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaAnalogAlarmGroup::CheckGroupNameExist 
	** \details 检查组名是否重复
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	bool CScadaAnalogAlarmGroup::CheckGroupNameExist(QString &strTagName)
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

	/*! \fn bool CScadaAnalogAlarmGroup::DeleteItem(CScadaAnalogAlarm *pItem)
	********************************************************************************************************* 
	** \brief Config::CScadaAnalogAlarmGroup::DeleteItem 
	** \details 
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2017年1月11日 
	** \note 
	********************************************************************************************************/
	bool CScadaAnalogAlarmGroup::DeleteItem(CScadaAnalogAlarm *pItem)
	{
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return false;
		}

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

	/*! \fn bool CScadaAnalogAlarmGroup::DeleteGroup(CScadaAnalogAlarmGroup *pChildGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaAnalogAlarmGroup::DeleteGroup 
	** \details 
	** \param pChildGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月11日 
	** \note 
	********************************************************************************************************/
	bool CScadaAnalogAlarmGroup::DeleteGroup(CScadaAnalogAlarmGroup *pChildGroup)
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

	/*! \fn bool CScadaAnalogAlarmGroup::CheckModifyGroupNameExist(CScadaAnalogAlarmGroup *pGroup, QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaAnalogAlarmGroup::CheckModifyGroupNameExist 
	** \details 
	** \param pGroup 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	bool CScadaAnalogAlarmGroup::CheckModifyGroupNameExist(CScadaAnalogAlarmGroup *pGroup, QString &strTagName)
	{
		for each (auto var in m_arrAnalogGroup)
		{
			if (pGroup != var)
			{
				if (strTagName.compare(var->m_strGroupName, Qt::CaseSensitive) == 0)
				{
					return true;
				}
			}
		}

		return false;
	}

	/*! \fn CScadaDigitalAlarm::CScadaDigitalAlarm()
	********************************************************************************************************* 
	** \brief Config::CScadaDigitalAlarm::CScadaDigitalAlarm 
	** \details 
	** \return  
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	CScadaDigitalAlarm::CScadaDigitalAlarm()
	{
		m_nOccNo = 0;
		m_bEnable = true;
		m_nID = 0;
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_nDelayAlarm = 0;
		m_bOnQualityGood = true;

		m_arrDigtallimit.clear();
	}

	/*! \fn CScadaDigitalAlarm::~CScadaDigitalAlarm()
	********************************************************************************************************* 
	** \brief Config::CScadaDigitalAlarm::~CScadaDigitalAlarm 
	** \details 
	** \return  
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	CScadaDigitalAlarm::~CScadaDigitalAlarm()
	{
		STLDeleteElements(&m_arrDigtallimit);
	}

	/*! \fn void CScadaDigitalAlarm::Save()
	********************************************************************************************************* 
	** \brief Config::CScadaDigitalAlarm::Save 
	** \details 
	** \return void 
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	void CScadaDigitalAlarm::Save()
	{

	}

	CScadaDigitalAlarmGroup::CScadaDigitalAlarmGroup()
	{

	}
	CScadaDigitalAlarmGroup::~CScadaDigitalAlarmGroup()
	{
		STLDeleteElements(&m_arrDigitalGroup);
		STLDeleteElements(&m_arrDigitalItem);
	}

	/*! \fn CScadaDigitalAlarmGroup *CScadaDigitalAlarmGroup::CreateGroup()
	********************************************************************************************************* 
	** \brief Config::CScadaDigitalAlarmGroup::CreateGroup 
	** \details 
	** \return CScadaDigitalAlarmGroup * 
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	CScadaDigitalAlarmGroup *CScadaDigitalAlarmGroup::CreateGroup()
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

			auto pGroup = new CScadaDigitalAlarmGroup;
			pGroup->SetGroupName(strName);

			m_arrDigitalGroup.push_back(pGroup);

			return pGroup;
		}

		return nullptr;
	}

	/*! \fn bool CScadaDigitalAlarmGroup::AddDigitalGroup(CScadaDigitalAlarmGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaDigitalAlarmGroup::AddDigitalGroup 
	** \details 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	bool CScadaDigitalAlarmGroup::AddDigitalGroup(CScadaDigitalAlarmGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		m_arrDigitalGroup.push_back(pGroup);
		return true;
	}

	/*! \fn bool CScadaDigitalAlarmGroup::AddDigitalItem(CScadaDigitalAlarm *pItem)
	********************************************************************************************************* 
	** \brief Config::CScadaDigitalAlarmGroup::AddDigitalItem 
	** \details 
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	bool CScadaDigitalAlarmGroup::AddDigitalItem(CScadaDigitalAlarm *pItem)
	{
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return false;
		}

		m_arrDigitalItem.push_back(pItem);
		return true;
	}

	/*! \fn bool CScadaDigitalAlarmGroup::CheckGroupNameExist(QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaDigitalAlarmGroup::CheckGroupNameExist 
	** \details 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2017年1月10日 
	** \note 
	********************************************************************************************************/
	bool CScadaDigitalAlarmGroup::CheckGroupNameExist(QString &strTagName)
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

	/*! \fn bool CScadaDigitalAlarmGroup::DeleteGroup(CScadaDigitalAlarmGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaDigitalAlarmGroup::DeleteGroup 
	** \details 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月14日 
	** \note 
	********************************************************************************************************/
	bool CScadaDigitalAlarmGroup::DeleteGroup(CScadaDigitalAlarmGroup *pGroup)
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

	/*! \fn bool CScadaDigitalAlarmGroup::DeleteItem(CScadaDigitalAlarm *pItem)
	********************************************************************************************************* 
	** \brief Config::CScadaDigitalAlarmGroup::DeleteItem 
	** \details 
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2017年1月14日 
	** \note 
	********************************************************************************************************/
	bool CScadaDigitalAlarmGroup::DeleteItem(CScadaDigitalAlarm *pItem)
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

	/*! \fn bool CScadaDigitalAlarmGroup::CheckModifyGroupNameExist(QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaDigitalAlarmGroup::CheckModifyGroupNameExist 
	** \details 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2017年1月11日 
	** \note 
	********************************************************************************************************/
	bool CScadaDigitalAlarmGroup::CheckModifyGroupNameExist(CScadaDigitalAlarmGroup *pGroup, QString &strTagName)
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



	CScadaLinearTransform::CScadaLinearTransform()
	{
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_nType = LINEAR;
		m_strDescription = "";
		m_nID = 0;
		m_bEnableFactor = true;
		m_bEnableInverseFactor = false;
		m_dbMaxRaw = 1;
		m_dbMinRaw = 1;
		m_dbMaxScale = 1;
		m_dbMinScale = 1;
		m_dbGain = 0;
		m_dbOffset = 0;
		m_strExtention = "";
		m_strGroupName = "";
		m_bEnableInverse = false;
	}

	CScadaLinearTransform::~CScadaLinearTransform()
	{

	}

	CScadaNonlinearTransform::CScadaNonlinearTransform()
	{
		//m_nOccNo = 0;
	}

	CScadaNonlinearTransform::~CScadaNonlinearTransform()
	{

	}

	/*! \fn CScadaTransformGroup::CScadaTransformGroup()
	*********************************************************************************************************
	** \brief Config::CScadaTransformGroup::CScadaTransformGroup
	** \details
	** \return
	** \author gw
	** \date 2017年1月17日
	** \note
	********************************************************************************************************/
	CScadaTransformGroup::CScadaTransformGroup()
	{
		m_arrTransformItem.clear();
		m_arrTransformGroup.clear();
	}

	/*! \fn CScadaTransformGroup::~CScadaTransformGroup()
	*********************************************************************************************************
	** \brief Config::CScadaTransformGroup::~CScadaTransformGroup
	** \details
	** \return
	** \author gw
	** \date 2017年1月17日
	** \note
	********************************************************************************************************/
	CScadaTransformGroup::~CScadaTransformGroup()
	{
		STLDeleteElements(&m_arrTransformItem);
		STLDeleteElements(&m_arrTransformGroup);
	}

	/*! \fn bool CScadaTransformGroup::AddTransformItem(CScadaTransform *pItem)
	********************************************************************************************************* 
	** \brief Config::CScadaTransformGroup::AddTransformItem 
	** \details 新的项是config创建的,然后添加到vec里,析构的时候统一在组里删除
				线性转换和非先行转换
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2017年1月17日 
	** \note 
	********************************************************************************************************/
	bool CScadaTransformGroup::AddTransformItem(CScadaTransform *pItem)
	{
		m_arrTransformItem.push_back(pItem);

		return true;
	}

	/*! \fn bool CScadaTransformGroup::AddTransformGroup(CScadaTransformGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaTransformGroup::AddTransformGroup 
	** \details 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月28日 
	** \note 
	********************************************************************************************************/
	bool CScadaTransformGroup::AddTransformGroup(CScadaTransformGroup *pGroup)
	{
		m_arrTransformGroup.push_back(pGroup);

		return true;
	}

	/*! \fn CScadaTransformGroup *CScadaTransformGroup::CreateGroup()
	********************************************************************************************************* 
	** \brief Config::CScadaTransformGroup::CreateGroup 
	** \details 新组是老组创建的
	** \return CScadaTransformGroup * 
	** \author gw
	** \date 2017年1月17日 
	** \note 
	********************************************************************************************************/
	CScadaTransformGroup *CScadaTransformGroup::CreateGroup()
	{
		int nIndex = (int)m_arrTransformGroup.size();
		QString strName = "";
		while (true)
		{
			strName = QString("%1%2").arg(GROUP_NAME).arg(nIndex);

			auto it = Find(strName);
			if (it == nullptr)
			{
				CScadaTransformGroup *pGroup = new CScadaTransformGroup;
				if (!pGroup->SetGroupName(strName))
				{
					return false;
				}

				m_arrTransformGroup.push_back(pGroup);

				return pGroup;
			}
			else
			{
				nIndex++;
			}
		}

		return nullptr;
	}

	/*! \fn CScadaTransformGroup *CScadaTransformGroup::Find(QString &strName)
	********************************************************************************************************* 
	** \brief Config::CScadaTransformGroup::Find 
	** \details 根据名字查找组
	** \param strName 
	** \return CScadaTransformGroup * 
	** \author gw
	** \date 2017年1月17日 
	** \note 
	********************************************************************************************************/
	CScadaTransformGroup *CScadaTransformGroup::Find(QString &strName)
	{
		for each (auto var in m_arrTransformGroup)
		{
			if (var->GetGroupName().compare(strName, Qt::CaseSensitive) == 0)
			{
				return var;
			}
		}

		return nullptr;
	}

	/*! \fn bool CScadaTransformGroup::DeleteItem(CScadaTransform *pChildItem)
	********************************************************************************************************* 
	** \brief Config::CScadaTransformGroup::DeleteItem 
	** \details 线性非线性删除项
	** \param pChildItem 
	** \return bool 
	** \author gw
	** \date 2017年1月19日 
	** \note 
	********************************************************************************************************/
	bool CScadaTransformGroup::DeleteItem(CScadaTransform *pChildItem)
	{
		auto it = std::find(m_arrTransformItem.begin(), m_arrTransformItem.end(), pChildItem);
		if (it != m_arrTransformItem.end())
		{
			delete *it;

			m_arrTransformItem.erase(it);

			return true;
		}

		return false;
	}

	/*! \fn bool CScadaTransformGroup::DeletGroup(CScadaTransformGroup *pChildGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaTransformGroup::DeletGroup 
	** \details 
	** \param pChildGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月19日 
	** \note 
	********************************************************************************************************/
	bool CScadaTransformGroup::DeletGroup(CScadaTransformGroup *pChildGroup)
	{
		auto it = std::find(m_arrTransformGroup.begin(), m_arrTransformGroup.end(), pChildGroup);
		if (it != m_arrTransformGroup.end())
		{
			delete *it;

			m_arrTransformGroup.erase(it);

			return true;
		}

		return false;
	}

	/*! \fn bool CScadaTransformGroup::CheckModifyGroupNameExist(CScadaTransformGroup *pGroup, QString strGroupName)
	********************************************************************************************************* 
	** \brief Config::CScadaTransformGroup::CheckModifyGroupNameExist 
	** \details 
	** \param pGroup 
	** \param strGroupName 
	** \return bool 
	** \author gw
	** \date 2017年1月19日 
	** \note 
	********************************************************************************************************/
	bool CScadaTransformGroup::CheckModifyGroupNameExist(CScadaTransformGroup *pGroup, QString strGroupName)
	{
		for (auto it = m_arrTransformGroup.begin(); it != m_arrTransformGroup.end(); ++it)
		{
			if (*it != pGroup)
			{
				if ((*it)->GetGroupName() == strGroupName)
				{
					return true;
				}
			}
		}

		return false;
	}

	/*! \fn int32u CTransformGroup::GetItemCount()
	********************************************************************************************************* 
	** \brief Config::CTransformGroup::GetItemCount 
	** \details 
	** \return int32u 
	** \author gw
	** \date 2017年1月23日 
	** \note 
	********************************************************************************************************/
	int32u CScadaTransformGroup::GetItemCount()
	{
		auto nCount = m_arrTransformItem.size();
		for each (auto var in m_arrTransformGroup)
		{
			nCount += var->GetItemCount();
		}

		return (int32u)nCount;
	}


	//
	CScadaData::CScadaData(IMainModuleInterface *pCore)
		:m_pCore(nullptr), m_pScadaUserGroup(nullptr), m_pScadaAnalogAlarmGroup(nullptr), m_pScadaLinearGroup(nullptr)
		, m_pScadaNonLinearGroup(nullptr), m_pScadaDigitalAlarmGroup(nullptr)
	{
		Q_ASSERT(pCore);
		if (!pCore)
		{
			return;
		}

		m_pCore = pCore;

		memset(m_szTagName, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		m_nID = 0;

		m_arrSystemVariable.clear();

		if (!m_pScadaUserGroup)
		{
			m_pScadaUserGroup = new CScadaUserVariableGroup;
		}

		if (!m_pScadaAnalogAlarmGroup)
		{
			m_pScadaAnalogAlarmGroup = new CScadaAnalogAlarmGroup;
		}

		if (!m_pScadaDigitalAlarmGroup)
		{
			m_pScadaDigitalAlarmGroup = new CScadaDigitalAlarmGroup;
		}

		if (!m_pScadaLinearGroup)
		{
			m_pScadaLinearGroup = new CScadaTransformGroup;
		}

		if (!m_pScadaNonLinearGroup)
		{
			m_pScadaNonLinearGroup = new CScadaTransformGroup;
		}

		InitScadaSystemVariable();

		m_arrAlarmToUserVariableMap.clear();
		m_arrSourceTagNameToUserVariableMap.clear();
		m_arrScaleToUserVariableMap.clear();

	}

	CScadaData::~CScadaData()
	{
		STLDeleteElements(&m_arrSystemVariable);

		if (m_pScadaUserGroup)
		{
			delete m_pScadaUserGroup;
			m_pScadaUserGroup = nullptr;
		}

		if (m_pScadaAnalogAlarmGroup)
		{
			delete m_pScadaAnalogAlarmGroup;
			m_pScadaAnalogAlarmGroup = nullptr;
		}

		if (m_pScadaDigitalAlarmGroup)
		{
			delete m_pScadaDigitalAlarmGroup;
			m_pScadaDigitalAlarmGroup = nullptr;
		}

		if (m_pScadaLinearGroup)
		{
			delete m_pScadaLinearGroup;
			m_pScadaLinearGroup = nullptr;
		}

		if (m_pScadaNonLinearGroup)
		{
			delete m_pScadaNonLinearGroup;
			m_pScadaNonLinearGroup = nullptr;
		}
	}

	void CScadaData::Save()
	{
		//接口 保存此前置数据
		//



		//接口 保存通道数据
	}

	bool CScadaData::SaveScadaData(const QString &szRoot, int nOccNo, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		QString fileName = QString("%1%2").arg(m_szTagName).arg(".xml");

		QString strPath = szRoot + "/scadavariable/";
		strPath = strPath + fileName;
		QFile file(strPath);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QString strTmp = QString(QObject::tr("===========open %1 fail!!!==========")).arg(strPath);
			s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);

			return false;
		}

		auto strTmp = QObject::tr("=============Open %1 success!!!===============").arg(strPath);
		MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);

		QXmlStreamWriter writer(&file);
		writer.setAutoFormatting(true);
		writer.writeStartDocument();

		writer.writeStartElement("scada");
		writer.writeAttribute("Description", m_strDescription);
		const auto it = pHash->find(m_strDescription.toStdString());
		if (it == pHash->end())
		{
			//字符串池
			*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
			pHash->insert(std::make_pair(m_strDescription.toStdString(), *pDescStringPoolOccNo));
			pStringPoolVec->push_back(m_strDescription.toStdString());
			//DescriptionOccNo

			writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
		}
		else
		{
			writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
		}

		writer.writeAttribute("TagName", m_szTagName);
		if (!CheckTagNameIsValid(m_szTagName, SCADAVARIABLE_DESC))
		{
			auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1 is invalid!!!").arg(m_szTagName);
			MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
		}

		writer.writeAttribute("version", VERSION);
		writer.writeAttribute("ID", QString("%1").arg(m_nID));
		writer.writeAttribute("Extention", m_strExtention);
		writer.writeAttribute("GroupName", m_strGroupName);

		//大排行号
		m_nOccNo = nOccNo;
		writer.writeAttribute("OccNo", QString("%1").arg(m_nOccNo));

		//Save alarm
		SaveAlarm(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		//Save scale
		SaveTransform(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		//Save variable
		SaveVariable(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		writer.writeEndDocument();

		return true;
	}

	bool CScadaData::SaveAlarm(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("alarm");
		auto strTmp = QObject::tr("-->Scada Varaible TagName %1  alarm log start!!!").arg(m_szTagName);
		MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);


		Q_ASSERT(m_pScadaAnalogAlarmGroup);
		if (!m_pScadaAnalogAlarmGroup)
		{
			return false;
		}

		int nCounts = (int)m_arrMaps[SCADA_VARIABLE_HASH_ID::ANALOGALARMID].size();
		writer.writeStartElement("ain_alarm");
		writer.writeAttribute("Count", QString("%1").arg(nCounts));
		strTmp = QObject::tr("-->Scada Varaible TagName %1  analog alarm count %1!!!").arg(m_szTagName).arg(nCounts);
		MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);

		int nAnalogAlarmOccNo = 0, nAanalogAlarmBlockNo = 0;
		SaveAnalogAlarm(writer, m_pScadaAnalogAlarmGroup, nAnalogAlarmOccNo, nAanalogAlarmBlockNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		Q_ASSERT(m_pScadaDigitalAlarmGroup);
		if (!m_pScadaDigitalAlarmGroup)
		{
			return false;
		}

		writer.writeStartElement("din_alarm");
		nCounts = (int)m_arrMaps[SCADA_VARIABLE_HASH_ID::DIGITALALARMID].size();


		writer.writeAttribute("Count", QString("%1").arg(nCounts));
		strTmp = QObject::tr("-->Scada Varaible TagName %1  digital alarm count %1!!!").arg(m_szTagName).arg(nCounts);
		MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);

		int nDigitalAlarmOccNo = 0, nDigitalAlarmLimitOccNo = 0;
		SaveDigtalAlarm(writer, m_pScadaDigitalAlarmGroup, nDigitalAlarmOccNo, nDigitalAlarmLimitOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);
		writer.writeEndElement();

		writer.writeEndElement();
		return true;
	}

	bool CScadaData::SaveAnalogAlarm(QXmlStreamWriter &writer, CScadaAnalogAlarmGroup *pGroup, int &nAanalogAlarmOccNo, int &nAanalogAlarmLimitOccNo, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto &arrGroup = pGroup->GetGroup();
		for each (auto var in arrGroup)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", QString("%1").arg(var->GetGroupName()));

			SaveAnalogAlarm(writer, var, nAanalogAlarmOccNo, nAanalogAlarmLimitOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}


		auto &arrItem = pGroup->GetItem();

		Q_ASSERT(nAanalogAlarmOccNo + 1 > 0);
		Q_ASSERT(nAanalogAlarmLimitOccNo + 1 > 0);
		for each (auto var in arrItem)
		{
			writer.writeStartElement("ain_alarm_node");

			nAanalogAlarmOccNo++;
			var->SetOccNo(nAanalogAlarmOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nAanalogAlarmOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			if (!CheckTagNameIsValid(var->m_szTagName, SCADAVARIABLE_DESC))
			{
				auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  analog alarm %2 is invalid!!!").arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(var->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				//字符串池
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(var->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(var->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}


			writer.writeAttribute("AlarmType", QString("%1").arg(var->m_nAlarmType));
			writer.writeAttribute("Enable", QString("%1").arg(var->m_bEnable));
			writer.writeAttribute("OnQualityGood", QString("%1").arg(var->m_bOnQualityGood));
			writer.writeAttribute("DeadType", QString("%1").arg(var->m_nDeadType));
			auto tmp = QString::number(var->m_dbDeadArea, 2, 6);
			writer.writeAttribute("DeadArea", tmp);
			writer.writeAttribute("DelayAlarm", QString("%1").arg(var->m_nDelayAlarm));
			writer.writeAttribute("GroupName", QString("%1").arg(var->m_strGroupName));

			auto const &analogOfflimit = var->m_arrAlarmOfflimit;
			writer.writeAttribute("Count", QString("%1").arg(analogOfflimit.size()));
			if (analogOfflimit.size() == 0)
			{
				auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  analog alarm %2  Limit count is 0!!!").arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			else
			{
				auto strTmp = QObject::tr("-->Scada Varaible TagName %1  analog alarm %2  Limit count is %3!!!").arg(m_szTagName).arg(var->m_szTagName).arg(analogOfflimit.size());
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			int nBlockNo = 0;
			for (auto const &tmp : analogOfflimit)
			{
				writer.writeStartElement("aa");

				nAanalogAlarmLimitOccNo++;
				tmp->SetOccNo(nAanalogAlarmLimitOccNo);
				writer.writeAttribute("OccNo", QString("%1").arg(nAanalogAlarmLimitOccNo));
				nBlockNo++;
				tmp->SetBlockNo(nBlockNo);
				writer.writeAttribute("BlockNo", QString("%1").arg(nBlockNo));
				writer.writeAttribute("TagName", QString("%1").arg(tmp->m_szTagName));
				if (!CheckTagNameIsValid(tmp->m_szTagName, SCADAVARIABLE_DESC))
				{
					auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  analog alarm %2  Limit %3 is invalid!!!").arg(m_szTagName).arg(var->m_szTagName).arg(tmp->m_szTagName);
					MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
					s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}

				writer.writeAttribute("ID", QString("%1").arg(tmp->m_nID));
				writer.writeAttribute("Description", QString("%1").arg(tmp->m_strDescription));
				//字符串内存池
				const auto it = pHash->find(tmp->m_strDescription.toStdString());
				if (it == pHash->end())
				{
					//字符串池
					*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
					pHash->insert(std::make_pair(tmp->m_strDescription.toStdString(), *pDescStringPoolOccNo));
					pStringPoolVec->push_back(tmp->m_strDescription.toStdString());

					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
				}
				else
				{
					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
				}

				//writer.writeAttribute("TimeFormat", QString("%1").arg(tmp->TimeFormat));
				writer.writeAttribute("Condition", QString("%1").arg(tmp->m_nCondition));
				writer.writeAttribute("Category", QString("%1").arg(tmp->m_nCategory));
				writer.writeAttribute("Value", QString("%1").arg(tmp->m_dbValue));
				writer.writeAttribute("LowValue", QString("%1").arg(tmp->m_dbLowValue));
				writer.writeAttribute("DelayAlarm", QString("%1").arg(tmp->m_nDelayAlarm));
				writer.writeAttribute("Priority", QString("%1").arg(tmp->m_nPriority));
				writer.writeAttribute("SupportAck", QString("%1").arg(tmp->m_bSupportAck));
				writer.writeAttribute("SupportDelete", QString("%1").arg(tmp->m_bSupportDelete));
				writer.writeAttribute("AckType", QString("%1").arg(tmp->m_nAckType));
				writer.writeAttribute("SoundFile", QString("%1").arg(tmp->m_strSoundFile));
				writer.writeAttribute("PlaySoundTimes", QString("%1").arg(tmp->m_nPlaySoundTimes));
				writer.writeAttribute("SpeechAlarmText", QString("%1").arg(tmp->m_strSpeechAlarmText));
				writer.writeAttribute("Beep", QString("%1").arg(tmp->m_bBeep));
				writer.writeAttribute("PushGraph", QString("%1").arg(tmp->m_strPushGraph));
				writer.writeAttribute("BlinkGraph", QString("%1").arg(tmp->m_bBlinkGraph));
				writer.writeAttribute("Log", QString("%1").arg(tmp->m_bLog));
				writer.writeAttribute("BackColor", QString("%1").arg(tmp->m_nBackColor));
				writer.writeAttribute("TextColor", QString("%1").arg(tmp->m_nTextColor));
				writer.writeAttribute("BlinkBackColor", QString("%1").arg(tmp->m_nBlinkBackColor));
				writer.writeAttribute("BlinkTextColor", QString("%1").arg(tmp->m_nBlinkTextColor));
				writer.writeAttribute("LightBoard", QString("%1").arg(tmp->m_bLightBoard));
				writer.writeAttribute("DispGuide", QString("%1").arg(tmp->m_strDispGuide));

				writer.writeAttribute("Commands", QString("%1").arg(tmp->m_strCommands));

				writer.writeEndElement();
			}

			writer.writeEndElement();
		}

		return true;
	}

	bool CScadaData::SaveDigtalAlarm(QXmlStreamWriter &writer, CScadaDigitalAlarmGroup *pGroup, int &nDigitalAlarmOccNo, int &nDigitalAlarmLimitOccNo
		, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto &arrGroup = pGroup->GetGroup();
		for each (auto var in arrGroup)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", QString("%1").arg(var->GetGroupName()));

			SaveDigtalAlarm(writer, var, nDigitalAlarmOccNo, nDigitalAlarmLimitOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}



		auto &arrItem = pGroup->GetItem();
		Q_ASSERT(nDigitalAlarmOccNo + 1 > 0);
		Q_ASSERT(nDigitalAlarmLimitOccNo + 1 > 0);
		for each (auto var in arrItem)
		{
			writer.writeStartElement("din_alarm_node");

			nDigitalAlarmOccNo++;

			var->SetOccNo(nDigitalAlarmOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(var->GetOccNo()));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			if (!CheckTagNameIsValid(var->m_szTagName, SCADAVARIABLE_DESC))
			{
				auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  digital alarm %2 is invalid!!!").arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(var->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(var->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(var->m_strDescription.toStdString());

				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//字符串存在
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("AlarmType", QString("%1").arg(var->m_nAlarmType));
			writer.writeAttribute("DelayAlarm", QString("%1").arg(var->m_nDelayAlarm));
			writer.writeAttribute("Enable", QString("%1").arg(var->m_bEnable));
			writer.writeAttribute("OnQualityGood", QString("%1").arg(var->m_bOnQualityGood));
			writer.writeAttribute("GroupName", QString("%1").arg(var->m_strGroupName));

			auto const &digtalOfflimit = var->m_arrDigtallimit;
			writer.writeAttribute("Count", QString("%1").arg(digtalOfflimit.size()));
			if (digtalOfflimit.size() == 0)
			{
				auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  digtal alarm %2  Limit count is 0!!!").arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			else
			{
				auto strTmp = QObject::tr("-->Scada Varaible TagName %1  digtal alarm %2  Limit count is %3!!!").arg(m_szTagName).arg(var->m_szTagName).arg(digtalOfflimit.size());
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			int nBolckNo = 0;
			for (auto const &tmp : digtalOfflimit)
			{
				writer.writeStartElement("da");

				nDigitalAlarmLimitOccNo++;
				tmp->SetOccNo(nDigitalAlarmLimitOccNo);
				writer.writeAttribute("OccNo", QString("%1").arg(tmp->GetOccNo()));

				nBolckNo++;
				tmp->BlockNo = nBolckNo;
				writer.writeAttribute("BlockNo", QString("%1").arg(nBolckNo));

				writer.writeAttribute("TagName", QString("%1").arg(tmp->TagName));
				if (!CheckTagNameIsValid(tmp->TagName, SCADAVARIABLE_DESC))
				{
					auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  digtal alarm %2  Limit %3 is invalid!!!").arg(m_szTagName).arg(var->m_szTagName).arg(tmp->TagName);
					MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
					s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}

				writer.writeAttribute("ID", QString("%1").arg(tmp->ID));
				writer.writeAttribute("Description", QString("%1").arg(tmp->Description));
				//字符串内存池
				const auto it = pHash->find(tmp->Description.toStdString());
				if (it == pHash->end())
				{
					*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;

					pHash->insert(std::make_pair(tmp->Description.toStdString(), *pDescStringPoolOccNo));
					pStringPoolVec->push_back(tmp->Description.toStdString());

					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
				}
				else
				{
					//字符串存在
					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
				}


				writer.writeAttribute("Condition", QString("%1").arg(tmp->Condition));
				writer.writeAttribute("Category", QString("%1").arg(tmp->Category));
				writer.writeAttribute("DelayAlarm", QString("%1").arg(tmp->DelayAlarm));
				writer.writeAttribute("Priority", QString("%1").arg(tmp->Priority));
				writer.writeAttribute("SupportAck", QString("%1").arg(tmp->SupportAck));
				writer.writeAttribute("SupportDelete", QString("%1").arg(tmp->SupportDelete));
				writer.writeAttribute("AckType", QString("%1").arg(tmp->AckType));
				writer.writeAttribute("SoundFile", QString("%1").arg(tmp->SoundFile));
				writer.writeAttribute("PlaySoundTimes", QString("%1").arg(tmp->PlaySoundTimes));
				writer.writeAttribute("SpeechAlarmText", QString("%1").arg(tmp->SpeechAlarmText));
				writer.writeAttribute("Beep", QString("%1").arg(tmp->Beep));
				writer.writeAttribute("PushGraph", QString("%1").arg(tmp->PushGraph));
				writer.writeAttribute("BlinkGraph", QString("%1").arg(tmp->BlinkGraph));
				writer.writeAttribute("Log", QString("%1").arg(tmp->Log));

				writer.writeAttribute("BackColor", QString("%1").arg(tmp->BackColor));
				writer.writeAttribute("TextColor", QString("%1").arg(tmp->TextColor));
				writer.writeAttribute("BlinkBackColor", QString("%1").arg(tmp->BlinkBackColor));
				writer.writeAttribute("BlinkTextColor", QString("%1").arg(tmp->BlinkTextColor));

				writer.writeAttribute("DispGuide", QString("%1").arg(tmp->DispGuide));
				writer.writeAttribute("Commands", QString("%1").arg(tmp->Commands));

				writer.writeEndElement();
			}

			writer.writeEndElement();
		}

		return true;
	}

	/*! \fn 
	********************************************************************************************************* 
	** \brief Config::CScadaData::SaveTransform 
	** \details 
	** \param writer 
	** \param  
	** \param pHash 
	** \param pStringPoolVec 
	** \param pDescStringPoolOccNo 
	** \return bool 
	** \author xingzhibing
	** \date 2017年6月29日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::SaveTransform(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("scale");
		auto strTmp = QObject::tr("Transform log start!!!");
		MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);

		SaveLinear(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		SaveNonlinear(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}

	/*! \fn bool CScadaData::SaveLinear(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CScadaData::SaveLinear 
	** \details 
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2017年3月15日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::SaveLinear(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(m_pScadaLinearGroup);
		if (!m_pScadaLinearGroup)
		{
			return false;
		}

		writer.writeStartElement("linear");
		auto nCount = m_pScadaLinearGroup->GetItemCount();
		writer.writeAttribute("Count", QString("%1").arg(nCount));
		auto strTmp = QObject::tr("-->Scada Varaible TagName %1  Linear scale count is %2!!!").arg(m_szTagName).arg(nCount);
		MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);

		int nOccNo = 0;
		SaveLinearTransformChildNode(writer, m_pScadaLinearGroup, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}

	bool CScadaData::SaveLinearTransformChildNode(QXmlStreamWriter &writer, Config::CScadaTransformGroup *pGroup, int &nOccNo, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		auto &arr = pGroup->GetGroup();
		for each (auto var in arr)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", var->GetGroupName());

			SaveLinearTransformChildNode(writer, var, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}


		auto &arrLinear = pGroup->GetItem();
		for (auto const &linear : arrLinear)
		{
			Q_ASSERT(linear);
			auto pTmp = linear->GetLinear();
			Q_ASSERT(pTmp);

			writer.writeStartElement("l");

			nOccNo++;
			linear->SetOccNo(nOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(pTmp->m_szTagName));
			if (!CheckTagNameIsValid(pTmp->m_szTagName, SCADAVARIABLE_DESC))
			{
				auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  Linear TagName %2 is invalid!!!").arg(m_szTagName).arg(pTmp->m_szTagName);
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
			}

			writer.writeAttribute("ID", QString("%1").arg(pTmp->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(pTmp->m_strDescription));
			const auto it = pHash->find(pTmp->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				//字符串池
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(pTmp->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(pTmp->m_strDescription.toStdString());
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("ScaleType", QString("%1").arg(pTmp->m_nType));
			writer.writeAttribute("MaxRaw", QString::number(pTmp->m_dbMaxRaw, 10, 6));
			writer.writeAttribute("MinRaw", QString::number(pTmp->m_dbMinRaw, 10, 6));
			writer.writeAttribute("MaxScale", QString::number(pTmp->m_dbMaxScale, 10, 6));
			writer.writeAttribute("MinScale", QString::number(pTmp->m_dbMinScale, 10, 6));
			writer.writeAttribute("Inverse", QString("%1").arg(pTmp->m_bEnableInverse));;
			writer.writeAttribute("EnaleFactor", QString("%1").arg(pTmp->m_bEnableFactor));
			writer.writeAttribute("Gain", QString("%1").arg(pTmp->m_dbGain));
			writer.writeAttribute("Offset", QString("%1").arg(pTmp->m_dbOffset));
			writer.writeAttribute("GroupName", QString("%1").arg(pTmp->m_strGroupName));

			writer.writeEndElement();
		}


		return true;
	}

	bool CScadaData::SaveNonlinear(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(m_pScadaNonLinearGroup);
		if (!m_pScadaNonLinearGroup)
		{
			return false;
		}

		writer.writeStartElement("nonlinear");

		auto nCount = m_pScadaNonLinearGroup->GetItemCount();
		writer.writeAttribute("Count", QString("%1").arg(nCount));
		auto strTmp = QObject::tr("-->Scada Varaible TagName %1  NonLinear scale count is %2!!!").arg(m_szTagName).arg(nCount);
		MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);

		int nOccNo = 0;
		SaveNonLinearTransformChildNode(writer, m_pScadaNonLinearGroup, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}

	bool CScadaData::SaveNonLinearTransformChildNode(QXmlStreamWriter &writer, Config::CScadaTransformGroup *pGroup, int &nOccNo, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		auto &arr = pGroup->GetGroup();
		for each (auto var in arr)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", var->GetGroupName());

			SaveNonLinearTransformChildNode(writer, var, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}

		auto &arrNonLinear = pGroup->GetItem();
		for (auto const &nonlinear : arrNonLinear)
		{
			Q_ASSERT(nonlinear);
			auto pTmp = nonlinear->GetNonLinear();
			Q_ASSERT(pTmp);
			auto bFlag = QString("%1").arg(pTmp->m_szTagName).isEmpty();
			Q_ASSERT(!bFlag);

			writer.writeStartElement("nl");

			nOccNo++;
			nonlinear->SetOccNo(nOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(pTmp->m_szTagName));
			if (!CheckTagNameIsValid(pTmp->m_szTagName, SCADAVARIABLE_DESC))
			{
				auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  NonLinear TagName %2 is invalid!!!").arg(m_szTagName).arg(pTmp->m_szTagName);
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
			}

			writer.writeAttribute("ID", QString("%1").arg(pTmp->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(pTmp->m_strDescription));
			const auto it = pHash->find(pTmp->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				//字符串池
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(pTmp->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(pTmp->m_strDescription.toStdString());
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}



			writer.writeAttribute("ScaleType", QString("%1").arg(nonlinear->m_nType));
			writer.writeAttribute("Count", QString("%1").arg(pTmp->m_arrPNonliear.size()));
			if (pTmp->m_arrPNonliear.size() == 0)
			{
				auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  NonLinear TagName %2 Point count is 0!!!").arg(m_szTagName).arg(pTmp->m_szTagName);
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			else
			{
				auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  NonLinear TagName %2 Point count is %3!!!").arg(m_szTagName).arg(pTmp->m_szTagName).arg(pTmp->m_arrPNonliear.size());
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
			}
			
			writer.writeAttribute("GroupName", QString("%1").arg(pTmp->m_strGroupName));


			for (auto const &p : pTmp->m_arrPNonliear)
			{
				writer.writeStartElement("p");

				writer.writeAttribute("RawValue", QString("%1").arg(p->RawValue));
				writer.writeAttribute("ScaleValue", QString("%1").arg(p->ScaleValue));

				writer.writeEndElement();
			}


			writer.writeEndElement();
		}


		return true;
	}

	bool CScadaData::SaveVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("variable");

		SaveScadaSystemVariable(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		SaveScadaUserVariable(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}

	bool CScadaData::SaveScadaSystemVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("scadasystem");
		writer.writeAttribute("Count", QString("%1").arg(m_arrSystemVariable.size()));

		int nOccNo = 0;
		for each (auto var in m_arrSystemVariable)
		{
			writer.writeStartElement("s");

			nOccNo++;
			var->SetOccNo(nOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			if (!CheckTagNameIsValid(var->m_szTagName, SCADAVARIABLE_DESC))
			{
				auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  system varaible %2 is invalid!!!").arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			const auto it = pHash->find(var->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(var->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(var->m_strDescription.toStdString());

				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}


			writer.writeAttribute("DataType", QString("%1").arg(var->m_nType));

			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}

	bool CScadaData::SaveScadaUserVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("scadauser");

		SaveScadaUserVariableChildNode(writer, m_pScadaUserGroup, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}

	bool CScadaData::SaveScadaUserVariableChildNode(QXmlStreamWriter &writer, Config::CScadaUserVariableGroup *pGroup, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto ite = pGroup->m_arrGroup.begin();
		for (; ite != pGroup->m_arrGroup.end(); ++ite)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", (*ite)->GetGroupName());
			writer.writeAttribute("count", QString("%1").arg((*ite)->m_arrItem.size()));

			SaveScadaUserVariableChildNode(writer, (*ite), pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}

		auto it = pGroup->m_arrItem.begin();
		for each (auto var in pGroup->m_arrItem)
		{
			writer.writeStartElement("u");

			writer.writeAttribute("OccNo", QString("%1").arg(var->GetOccNo()));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			if (!CheckTagNameIsValid(var->m_szTagName, SCADAVARIABLE_DESC))
			{
				auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  user varaible %2 is invalid!!!").arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			const auto it = pHash->find(var->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(var->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(var->m_strDescription.toStdString());

				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}


			writer.writeAttribute("DataType", QString("%1").arg(var->m_nType));
			writer.writeAttribute("InitValue", QString("%1").arg(var->m_dbInitValue));
			writer.writeAttribute("InitialQua", QString("%1").arg(var->m_nInitialQua));
			writer.writeAttribute("Address", QString("%1").arg(var->m_strAddress));
			writer.writeAttribute("PinLabel", QString("%1").arg(var->m_strPinLabel));
			
			writer.writeAttribute("SourceTagName", QString("%1").arg(var->m_strSourceTagName));
			if (!var->m_strSourceTagName.isEmpty())
			{
				//根据tagname找OccNo
				QString strTmp = QString("%1").arg(var->m_szTagName);
				var->m_strSourceOccNo = "";
				if (!GetUserVariableRelatedSourceOccNo(strTmp, var->m_strSourceTagName, var->m_strSourceOccNo))
				{
					auto strError = QObject::tr("User variable tagname %1 relate sourcetagname failed!!!").arg(var->m_szTagName);
					LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					writer.writeEndElement();

					break;
				}
				writer.writeAttribute("SourceOccNo", QString("%1").arg(var->m_strSourceOccNo));
				//check
				if (var->m_strSourceOccNo.isEmpty())
				{
					auto strTmp = QObject::tr("Error-->Scada Varaible TagName %1  user varaible %2   SourceTagName %3 format is wrong!!!").arg(m_szTagName).arg(var->m_szTagName).arg(var->m_strSourceTagName);
					MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), true);
					s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}
			}




			//关联的转换大排行号
			//判断转换tagname是否为空
			if (var->m_strScaleTagName.isEmpty())
			{
				writer.writeAttribute("ScaleOccNo", QString("%1").arg(0));
				writer.writeAttribute("ScaleType", QString("%1").arg(LINEAR));
				writer.writeAttribute("ScaleTagName", QString("%1").arg(""));
			}
			else
			{
				Config::CScadaTransform *pTransform = nullptr;
				if (ScaleType::LINEAR == var->m_nScaleType)
				{
					pTransform = (Config::CScadaTransform *)GetData(var->m_strScaleTagName, SCADA_VARIABLE_HASH_ID::LINEARTRANSFORMID);
				}
				else if (ScaleType::NONLINEAR == var->m_nScaleType)
				{
					pTransform = (Config::CScadaTransform *)GetData(var->m_strScaleTagName, SCADA_VARIABLE_HASH_ID::NONLINEARTRANSFORMID);
				}

				Q_ASSERT(pTransform);
				if (!pTransform)
				{
					auto strError = QObject::tr("scale tagname %1 is isvalid!!!").arg(var->m_strScaleTagName);
					s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					writer.writeEndElement();

					break;
				}

				writer.writeAttribute("ScaleOccNo", QString("%1").arg(pTransform->GetOccNo()));
				writer.writeAttribute("ScaleType", QString("%1").arg(var->m_nScaleType));
				writer.writeAttribute("ScaleTagName", QString("%1").arg(var->m_strScaleTagName));
			}

			if (var->m_strAlarmTagName.isEmpty())
			{
				writer.writeAttribute("AlarmOccNo", QString("%1").arg(0));
				writer.writeAttribute("AlarmType", QString("%1").arg(AlarmType::AIALARM));
				writer.writeAttribute("AlarmTagName", QString("%1").arg(""));
			}
			else
			{
				if (AlarmType::AIALARM == var->m_nAlarmType)
				{
					auto pAlarm = (Config::CScadaAnalogAlarm *)GetData(var->m_strAlarmTagName, SCADA_VARIABLE_HASH_ID::ANALOGALARMID);
					Q_ASSERT(pAlarm);
					if (!pAlarm)
					{
						auto strError = QObject::tr("alarm tagname %1 is isvalid!!!").arg(var->m_strAlarmTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						writer.writeEndElement();

						break;
					}

					writer.writeAttribute("AlarmOccNo", QString("%1").arg(pAlarm->GetOccNo()));
					writer.writeAttribute("AlarmType", QString("%1").arg(AlarmType::AIALARM));
					writer.writeAttribute("AlarmTagName", QString("%1").arg(var->m_strAlarmTagName));
				}
				else if (AlarmType::DIALARM == var->m_nAlarmType)
				{
					auto pAlarm = (Config::CScadaDigitalAlarm *)GetData(var->m_strAlarmTagName, SCADA_VARIABLE_HASH_ID::DIGITALALARMID);
					Q_ASSERT(pAlarm);
					if (!pAlarm)
					{
						auto strError = QObject::tr("alarm tagname %1 is isvalid!!!").arg(var->m_strAlarmTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						writer.writeEndElement();

						break;
					}

					writer.writeAttribute("AlarmOccNo", QString("%1").arg(pAlarm->GetOccNo()));
					writer.writeAttribute("AlarmType", QString("%1").arg(AlarmType::DIALARM));
					writer.writeAttribute("AlarmTagName", QString("%1").arg(var->m_strAlarmTagName));
				}
			}


			writer.writeEndElement();
		}

		return true;
	}

	/*! \fn bool CScadaData::LoadScadaData(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CScadaData::LoadScadaData 
	** \details 加载scada数据
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2017年3月15日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::LoadScadaData(QXmlStreamReader &reader)
	{
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "channel")
				{
					//ReadChannel(reader, m_pChannelGroup);
				}
				else if (strTmp == "scale")
				{
					//ReadTransform(reader);
				}
				else if (strTmp == "linear")
				{
					Q_ASSERT(m_pScadaLinearGroup);
					if (!m_pScadaLinearGroup)
					{
						continue;
					}
					
					LoadLinear(reader, m_pScadaLinearGroup);
				}
				else if (strTmp == "nonlinear")
				{
					Q_ASSERT(m_pScadaNonLinearGroup);
					if (!m_pScadaNonLinearGroup)
					{
						continue;
					}

					LoadNonlinear(reader, m_pScadaNonLinearGroup);
				}
				else if (strTmp == "variable")
				{
					LoadVariable(reader);
				}
				else if (strTmp == "ain_alarm")
				{
					Q_ASSERT(m_pScadaAnalogAlarmGroup);
					if (!m_pScadaAnalogAlarmGroup)
					{
						continue;
					}
					
					LoadAINAlarm(reader, m_pScadaAnalogAlarmGroup);
				}
				else if (strTmp == "din_alarm")
				{
					Q_ASSERT(m_pScadaDigitalAlarmGroup);
					if (!m_pScadaDigitalAlarmGroup)
					{
						continue;
					}

					LoadDINAlarm(reader, m_pScadaDigitalAlarmGroup);
				}
			}
			else if (reader.isEndElement() && strTmp == "scada")
			{
				break;
			}
		}

		return true;
	}

	bool CScadaData::LoadVariable(QXmlStreamReader &reader)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "scadasystem")
				{
					LoadSystemVarialbeNode(reader);
				}
				else if (strTmp == "scadauser")
				{
					Q_ASSERT(m_pScadaUserGroup);
					if (!m_pScadaUserGroup)
					{
						reader.readNext();

						continue;
					}

					LoadUserVarialbeNode(reader, m_pScadaUserGroup);
				}
			}
			else if (reader.isEndElement() && strTmp == "variable")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	bool CScadaData::LoadSystemVarialbeNode(QXmlStreamReader &xml)
	{
		while (!xml.atEnd())
		{
			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();
				if (strTmp == "s")
				{
					//auto pSystemVariable = new CScadaSystemVaraible;
					//memset(pSystemVariable->m_szTagName, 0, sizeof(pSystemVariable->m_szTagName));
					//auto strTagName = xml.attributes().value("TagName").toString();
					//auto nSize = strTagName.size();
					//strncpy(pSystemVariable->m_szTagName, strTagName.toStdString().c_str(), qMin(nSize, MAX_NAME_LENGTH_SCADASTUDIO));

					//auto nOccNo = xml.attributes().value("OccNo").toInt();
					//Q_ASSERT(nOccNo > 0 && nOccNo < MAX_OCCNO_SCADASTUDIO);
					//pSystemVariable->SetOccNo(nOccNo);

					//pSystemVariable->m_nID = xml.attributes().value("ID").toInt();
					//pSystemVariable->m_strDescription = xml.attributes().value("Description").toString();
					//pSystemVariable->m_nType = xml.attributes().value("DataType").toInt();

					/************************************************************************/
					/* 系统变量是程序写死的,所以读取xml文件可以忽略
					在Create函数里加载数据
					/************************************************************************/
					//m_arrSystemVariable.push_back(pSystemVariable);

					////Push data to hash table
					//PushTagNameHashMap(SCADAVIRABLEHASHID::SYSTEMID, pSystemVariable->m_szTagName, pSystemVariable);
				}
			}
			else if (xml.isEndElement() && xml.name() == "scadasystem")
			{
				break;
			}

			xml.readNext();
		}

		return true;
	}

	bool CScadaData::LoadUserVarialbeNode(QXmlStreamReader &xml, Config::CScadaUserVariableGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			auto strError = QObject::tr("read user variable error!!!");
			s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		while (!xml.atEnd())
		{
			xml.readNext();

			QString strTmp = xml.name().toString();

			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();

				if (strTmp == "u")
				{
					//if (strTmp == "s")
					{


						auto pUser = new CScadaUserVariable();

						pUser->SetOccNo(xml.attributes().value("OccNo").toInt());
						pUser->m_nID = xml.attributes().value("ID").toInt();

						QString strTagName = xml.attributes().value("TagName").toString();
						//校验tagname长度
						if (!CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC))
						{
							xml.readNext();

							return false;
						}
						memset(pUser->m_szTagName, 0, sizeof(pUser->m_szTagName));
						int nSize = strTagName.size();
						strncpy(pUser->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

						pUser->m_strDescription = xml.attributes().value("Description").toString();
						pUser->m_nType = xml.attributes().value("DataType").toInt();
						pUser->m_dbInitValue = xml.attributes().value("InitValue").toDouble();
						pUser->m_nInitialQua = xml.attributes().value("InitialQua").toInt();
						pUser->m_strSourceTagName = xml.attributes().value("SourceTagName").toString();
						
						pUser->m_strScaleTagName = xml.attributes().value("ScaleTagName").toString();
						pUser->m_nScaleType = xml.attributes().value("ScaleType").toInt();
						pUser->m_nScaleOccNo = xml.attributes().value("ScaleOccNo").toInt();

						pUser->m_strAlarmTagName = xml.attributes().value("AlarmTagName").toString();
						pUser->m_nAlarmType = xml.attributes().value("AlarmType").toInt();
						pUser->m_nAlarmOccNo = xml.attributes().value("AlarmOccNo").toInt();
						if (!CheckTagNameIsExist(strTagName))
						{
							if (!PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::USERID, strTagName, pUser))
							{
								auto strError = QObject::tr("user variable read error!!!");
								s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

								continue;
							}

							pGroup->AddUserVariableItem(pUser);
						}
						else
						{
							auto strError = QObject::tr("user variable read error!!!");
							s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

							continue;
						}

						//Note
						//添加用户变量绑定关联的转换到关联转换hash里
						if (!pUser->m_strScaleTagName.isEmpty())
						{
							AddScadaVariableRelationScaleArr(pUser->m_strScaleTagName.toStdString(), pUser);
						}

						//Note
						//添加用户变量绑定关联的到关联告警hash里
						if (!pUser->m_strAlarmTagName.isEmpty())
						{
							AddScadaVariableRelationAlarmArr(pUser->m_strAlarmTagName.toStdString(), pUser);
						}

						//用户变量sourcetagname添加关联关系
						if (!pUser->m_strSourceTagName.isEmpty())
						{
							auto list = pUser->m_strSourceTagName.split(".");
							//
							if (list.count() == 3)
							{
								QString strTagNameCombine = QString("%1.%2").arg(list[0]).arg(list[1]);

								if (!AddUserVariableRelationSourceTagNameArr(strTagNameCombine.toStdString(), pUser))
								{
									auto strError = QObject::tr("Add UserVariable %1 RelationSourceTagName %2 failed!!!").arg(pUser->m_szTagName).arg(pUser->m_strSourceTagName);
									s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
								}
							}
							else
							{
								auto strError = QObject::tr("SourceTagName %1 of UserVariable %2 !!!").arg(pUser->m_strSourceTagName).arg(pUser->m_szTagName);
								s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
							}
						}
					}
				}
				else if (strTmp == "group")
				{


					QString strName = xml.attributes().value("name").toString();
					//if (!CheckTagNameIsExist(strName))
					{
						auto *pTmp = new CScadaUserVariableGroup;
						Q_ASSERT(pTmp);
						if (!pTmp)
						{
							auto strError = QObject::tr("read uservariablegroup error!!!");
							s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

							continue;
						}

						//if (!PushTagNameHashMap(SCADAVIRABLEHASHID::USERID, strName, pTmp))
						//{
						//	auto strError = QObject::tr("user variable group name %1 is exist!!!").arg(strName);
						//	s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						//	continue;
						//}

						pTmp->SetGroupName(strName);
						pGroup->AddUserVariableGroup(pTmp);
						LoadUserVarialbeNode(xml, pTmp);
					}
					//else
					//{
					// auto strError = QObject::tr("user variable read error!!!");
					// s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					// continue;
					//}



				}
			}
			else if (xml.isEndElement() && strTmp == "group")
			{
				break;
			}

			//xml.readNext();
		}

		return true;
	}


	bool CScadaData::LoadLinear(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "l")
				{
					LoadL(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CScadaTransformGroup *pGroupChild = pGroup->Find(strTmp);
					if (!pGroupChild)
					{
						pGroupChild = new Config::CScadaTransformGroup;
						pGroupChild->SetGroupName(reader.attributes().value("name").toString());

						pGroup->AddTransformGroup(pGroupChild);
					}


					//组group的子组group
					reader.readNext();

					LoadLinearGroupNode(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement() && strTmp == "linear")
			{
				break;
			}
		}

		return true;
	}

	bool CScadaData::LoadLinearGroupNode(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();
				if (strTmp == "l")
				{
					LoadL(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					auto strTmp = reader.attributes().value("name").toString();
					auto *pTransformChildGroup = pGroup->Find(strTmp);
					if (!pTransformChildGroup)
					{
						pTransformChildGroup = new CScadaTransformGroup;
						pTransformChildGroup->SetGroupName(reader.attributes().value("name").toString());

						pGroup->AddTransformGroup(pTransformChildGroup);
					}


					//组group的子组group
					reader.readNext();

					LoadLinearGroupNode(reader, pTransformChildGroup);

					continue;
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	bool CScadaData::LoadL(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "l")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("linear OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(LINEAR_SCALE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("linear Tag name is empty!!!");
						s_pGlobleCore->LogMsg(LINEAR_SCALE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaLinearTransform *pLinearTransform = new CScadaLinearTransform();

					//pLinearTransform->SetOccNo(nOccNo);
					memset(pLinearTransform->m_szTagName, 0, sizeof(pLinearTransform->m_szTagName));
					strncpy(pLinearTransform->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));

					int32u nID = reader.attributes().value("ID").toUInt();
					pLinearTransform->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pLinearTransform->m_strDescription = strDescription;

					fp64 dbMaxRaw = reader.attributes().value("MaxRaw").toDouble();
					pLinearTransform->m_dbMaxRaw = dbMaxRaw;

					fp64 dbMinRaw = reader.attributes().value("MinRaw").toDouble();
					pLinearTransform->m_dbMinRaw = dbMinRaw;

					fp64 dbMaxScale = reader.attributes().value("MaxScale").toDouble();
					pLinearTransform->m_dbMaxScale = dbMaxScale;

					fp64 dbMinScale = reader.attributes().value("MinScale").toDouble();
					pLinearTransform->m_dbMinScale = dbMinScale;

					int8u nInverse = reader.attributes().value("Inverse").toInt();
					pLinearTransform->m_bEnableInverse = nInverse;

					int8u nEnaleFactor = reader.attributes().value("EnaleFactor").toInt();
					pLinearTransform->m_bEnableFactor = nEnaleFactor;

					fp64 dbGain = reader.attributes().value("Gain").toDouble();
					pLinearTransform->m_dbGain = dbGain;

					fp64 dbOffset = reader.attributes().value("Offset").toDouble();
					pLinearTransform->m_dbOffset = dbOffset;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pLinearTransform->m_strGroupName = strGroupName;

					pLinearTransform->m_nType = reader.attributes().value("ScaleType").toInt();


					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of linear scale %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(LINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString(QObject::tr("linear scale %1 is empty")).arg(strTagName);
						s_pGlobleCore->LogMsg(LINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					else if (strTagName.count() > MAX_TAGNAME_LEN_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("linear scale %1 size is more than %2 ")).arg(strTagName).arg(MAX_TAGNAME_LEN_SCADASTUDIO);
						s_pGlobleCore->LogMsg(LINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					CScadaTransform *pTmp = new CScadaTransform;
					pTmp->SetOccNo(nOccNo);

					pTmp->m_nType = pLinearTransform->m_nType;
					memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
					auto nSize = strlen(pLinearTransform->m_szTagName);
					strncpy(pTmp->m_szTagName, pLinearTransform->m_szTagName, qMin(MAX_TAGNAME_LEN_SCADASTUDIO, (int)nSize));

					pTmp->SetLinear(pLinearTransform);

					//添加到前置转换hash表
					PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::LINEARTRANSFORMID, strTagName, pTmp);
					pGroup->AddTransformItem(pTmp);
				}

				reader.readNext();
			}
			else if (reader.isEndElement())
			{
				break;
			}
		}

		return true;
	}

	/*! \fn bool CScadaData::LoadNonlinear(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaData::LoadNonlinear 
	** \details 
	** \param reader 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年3月16日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::LoadNonlinear(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "nl")
				{
					LoadNl(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					auto strTmp = reader.attributes().value("name").toString();
					CScadaTransformGroup *pChildGroup = pGroup->Find(strTmp);
					if (!pChildGroup)
					{
						pChildGroup = new CScadaTransformGroup;
						pChildGroup->SetGroupName(reader.attributes().value("name").toString());
						pGroup->AddTransformGroup(pChildGroup);
					}


					reader.readNext();

					LoadNonLinearGroupNode(reader, pChildGroup);
				}
			}
			else if (reader.isEndElement() && strTmp == "nonlinear")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	bool CScadaData::LoadNonLinearGroupNode(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();
				if (strTmp == "nl")
				{
					LoadNl(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					auto strTmp = reader.attributes().value("name").toString();
					auto *pTransformChildGroup = pGroup->Find(strTmp);
					if (!pTransformChildGroup)
					{
						pTransformChildGroup = new CScadaTransformGroup;
						pTransformChildGroup->SetGroupName(reader.attributes().value("name").toString());

						pGroup->AddTransformGroup(pTransformChildGroup);
					}

					//组group的子组group
					reader.readNext();

					LoadNonLinearGroupNode(reader, pTransformChildGroup);

					continue;
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	bool CScadaData::LoadNl(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "nl")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("nl OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("Tag name is empty!!!");
						s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaNonlinearTransform *pNonliear = new CScadaNonlinearTransform();

					//pNonliear->SetOccNo(nOccNo);
					memset(pNonliear->m_szTagName, 0, sizeof(pNonliear->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pNonliear->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pNonliear->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pNonliear->m_strDescription = strDescription;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pNonliear->m_strGroupName = strGroupName;

					pNonliear->m_nType = reader.attributes().value("ScaleType").toInt();

					bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of nonlinear scale %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(NONLINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					Loadnl(reader, pNonliear);

					CScadaTransform *pTmp = new CScadaTransform;

					pTmp->m_nType = pNonliear->m_nType;
					memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
					nSize = sizeof(pNonliear->m_szTagName);
					strncpy(pTmp->m_szTagName, pNonliear->m_szTagName, qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
					pTmp->SetOccNo(nOccNo);

					pTmp->SetNonLinear(pNonliear);

					PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::NONLINEARTRANSFORMID, strTagName, pTmp);
					pGroup->AddTransformItem(pTmp);
				}

				strTmp = reader.name().toString();
			}
			else if (reader.isEndElement())
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	bool CScadaData::Loadnl(QXmlStreamReader &reader, Config::CScadaNonlinearTransform *pData)
	{
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "p")
				{
					LoadP(reader, pData);
				}
			}
			else if (reader.isEndElement())
			{
				//reader.skipCurrentElement();

				break;
			}
		}

		return true;
	}

	bool CScadaData::LoadP(QXmlStreamReader &reader, Config::CScadaNonlinearTransform *pData)
	{
		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "p")
				{
					SCADA_NONLINEAR_SCALE *pNonScale = new SCADA_NONLINEAR_SCALE;

					fp64 dbMaxRaw = reader.attributes().value("RawValue").toDouble();
					pNonScale->RawValue = dbMaxRaw;

					fp64 dbMaxScale = reader.attributes().value("ScaleValue").toDouble();
					pNonScale->ScaleValue = dbMaxScale;

					pData->m_arrPNonliear.push_back(pNonScale);
				}

				reader.readNext();
			}
			else if (reader.isEndElement())
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	bool CScadaData::LoadAINAlarm(QXmlStreamReader &reader, CScadaAnalogAlarmGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		using namespace Config;

		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "ain_alarm_node")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					auto bFlag = nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO;
					Q_ASSERT(!bFlag);
					if (bFlag)
					{// LOG
						auto strError = QObject::tr("ain alarm OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("ain alarm Tag name is empty!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaAnalogAlarm *pAlarm = new CScadaAnalogAlarm;
					//模拟量报警
					pAlarm->SetOccNo(nOccNo);
					memset(pAlarm->m_szTagName, 0, sizeof(pAlarm->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pAlarm->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pAlarm->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pAlarm->m_strDescription = strDescription;

					int8u nEnable = reader.attributes().value("Enable").toInt();
					pAlarm->m_bEnable = nEnable;

					int8u nOnQualityGood = reader.attributes().value("OnQualityGood").toUInt();
					pAlarm->m_bOnQualityGood = nOnQualityGood;

					int8u nDeadType = reader.attributes().value("DeadType").toUInt();
					pAlarm->m_nDeadType = nDeadType;

					fp64 dbDeadArea = reader.attributes().value("DeadArea").toDouble();
					pAlarm->m_dbDeadArea = dbDeadArea;

					auto nAlarmType = reader.attributes().value("AlarmType").toInt();
					pAlarm->m_nAlarmType = nAlarmType;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pAlarm->m_strGroupName = strGroupName;

					bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of ai alarm %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString(QObject::tr("ai alarm %1 is empty")).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					else if (strTagName.count() > MAX_TAGNAME_LEN_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("ai alarm %1 size is more than %2 ")).arg(strTagName).arg(MAX_TAGNAME_LEN_SCADASTUDIO);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					LoadAINAlarmLimit(reader, pAlarm);

					pGroup->AddAnalogItem(pAlarm);
					PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMID, strTagName, pAlarm);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CScadaAnalogAlarmGroup *pGroupChild = new Config::CScadaAnalogAlarmGroup;

					pGroupChild->SetGroupName(reader.attributes().value("name").toString());

					pGroup->AddAnalogGroup(pGroupChild);


					//组group的子组group
					reader.readNext();

					LoadAINAlarm(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}
		}

		return true;
	}

	bool CScadaData::LoadAINAlarmLimit(QXmlStreamReader &reader, Config::CScadaAnalogAlarm *pAlarm)
	{
		Q_ASSERT(pAlarm);
		if (!pAlarm)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "aa")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ain alarm offlimit OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					int32u nBlockNo = reader.attributes().value("BlockNo").toInt();
					Q_ASSERT(nBlockNo >= MIN_BLOCKNO_SCADASTUDIO && nBlockNo <= MAX_BLOCKNO_SCADASTUDIO);
					if (nBlockNo <= MIN_BLOCKNO_SCADASTUDIO || nBlockNo > MAX_BLOCKNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ain alarm offlimit BlockNo is invalid!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (!CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC))
					{
						auto strError = QObject::tr("ain alarm offlimit Tag name is invalid!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaAinAlarmLimit *pAlarmOfflimit = new CScadaAinAlarmLimit;
					pAlarmOfflimit->SetOccNo(nOccNo);

					memset(pAlarmOfflimit->m_szTagName, 0, sizeof(pAlarmOfflimit->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pAlarmOfflimit->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					QString strDescription = reader.attributes().value("Description").toString();
					pAlarmOfflimit->m_strDescription = strDescription;

					int32u nID = reader.attributes().value("ID").toUInt();
					pAlarmOfflimit->m_nID = nID;



					//QString strTimeFormat = reader.attributes().value("TimeFormat").toString();
					//pAlarmOfflimit->TimeFormat = strTimeFormat;

					int8u nCondition = reader.attributes().value("Condition").toUInt();
					pAlarmOfflimit->m_nCondition = nCondition;

					pAlarmOfflimit->m_nCategory = reader.attributes().value("Category").toUInt();

					fp64 dbValue = reader.attributes().value("Value").toDouble();
					pAlarmOfflimit->m_dbValue = dbValue;

					fp64 dbLowValue = reader.attributes().value("LowValue").toDouble();
					pAlarmOfflimit->m_dbLowValue = dbLowValue;

					int32u nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();
					pAlarmOfflimit->m_nDelayAlarm = nDelayAlarm;

					pAlarmOfflimit->m_nPriority = reader.attributes().value("Priority").toUInt();

					pAlarmOfflimit->m_bSupportAck = reader.attributes().value("SupportAck").toUInt();
					pAlarmOfflimit->m_bSupportDelete = reader.attributes().value("SupportDelete").toUInt();

					pAlarmOfflimit->m_nAckType = reader.attributes().value("AckType").toInt();

					QString strSoundFile = reader.attributes().value("SoundFile").toString();
					pAlarmOfflimit->m_strSoundFile = strSoundFile;
					int32u nPlaySoundTimes = reader.attributes().value("PlaySoundTimes").toUInt();
					pAlarmOfflimit->m_nPlaySoundTimes = nPlaySoundTimes;

					pAlarmOfflimit->m_strSpeechAlarmText = reader.attributes().value("SpeechAlarmText").toString();

					pAlarmOfflimit->m_bBeep = reader.attributes().value("Beep").toUInt();

					pAlarmOfflimit->m_strPushGraph = reader.attributes().value("PushGraph").toString();

					pAlarmOfflimit->m_bBlinkGraph = reader.attributes().value("BlinkGraph").toInt();

					pAlarmOfflimit->m_bLog = reader.attributes().value("Log").toInt();

					pAlarmOfflimit->m_nBackColor = reader.attributes().value("BackColor").toInt();
					pAlarmOfflimit->m_nTextColor = reader.attributes().value("TextColor").toInt();
					pAlarmOfflimit->m_nBlinkBackColor = reader.attributes().value("BlinkBackColor").toInt();
					pAlarmOfflimit->m_nBlinkTextColor = reader.attributes().value("BlinkTextColor").toInt();

					pAlarmOfflimit->m_bLightBoard = reader.attributes().value("LightBoard").toInt();
					pAlarmOfflimit->m_strDispGuide = reader.attributes().value("DispGuide").toString();
					pAlarmOfflimit->m_strCommands = reader.attributes().value("Commands").toString();

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of ai alarm limit %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty() || strTagName.count() > MAX_TAGNAME_LEN_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("ai alarm limit %1 is empty")).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					else if (strTagName.count() > MAX_TAGNAME_LEN_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("ai alarm limit %1 size is more than %2 ")).arg(strTagName).arg(MAX_TAGNAME_LEN_SCADASTUDIO);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					pAlarm->m_arrAlarmOfflimit.push_back(pAlarmOfflimit);
					PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMLIMITID, strTagName, pAlarmOfflimit);
				}

				reader.readNext();
			}
			else if (reader.isEndElement() && strTmp == "ain_alarm_node")
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	bool CScadaData::LoadDINAlarm(QXmlStreamReader &reader, CScadaDigitalAlarmGroup *pGroup)
	{
		using namespace Config;
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "din_alarm_node")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("di alarm OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("di alarm Tag name %1 is invalid!!!").arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaDigitalAlarm *pDigitalAlarm = new CScadaDigitalAlarm;

					//数字量报警
					pDigitalAlarm->SetOccNo(nOccNo);
					memset(pDigitalAlarm->m_szTagName, 0, sizeof(pDigitalAlarm->m_szTagName));
					strncpy(pDigitalAlarm->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigitalAlarm->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pDigitalAlarm->m_strDescription = strDescription;

					int8u nEnable = reader.attributes().value("Enable").toInt();
					pDigitalAlarm->m_bEnable = nEnable;

					int8u nOnQualityGood = reader.attributes().value("OnQualityGood").toUInt();
					pDigitalAlarm->m_bOnQualityGood = nOnQualityGood;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pDigitalAlarm->m_strGroupName = strGroupName;

					pDigitalAlarm->m_nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();

					auto nAlarmType = reader.attributes().value("AlarmType").toInt();
					pDigitalAlarm->m_nAlarmType = nAlarmType;

					bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of di alarm %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString(QObject::tr("di alarm %1 is empty")).arg(strTagName);
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.count() > MAX_TAGNAME_LEN_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("di alarm %1 is more than %2")).arg(strTagName);
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					LoadDINAlarmLimit(reader, pDigitalAlarm);

					pGroup->AddDigitalItem(pDigitalAlarm);
					PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMID, strTagName, pDigitalAlarm);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CScadaDigitalAlarmGroup *pGroupChild = new Config::CScadaDigitalAlarmGroup;

					pGroupChild->SetGroupName(reader.attributes().value("name").toString());

					pGroup->AddDigitalGroup(pGroupChild);


					//组group的子组group
					reader.readNext();

					LoadDINAlarm(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}
		}

		return true;
	}

	bool CScadaData::LoadDINAlarmLimit(QXmlStreamReader &reader, CScadaDigitalAlarm *pAlarm)
	{
		Q_ASSERT(pAlarm);
		if (!pAlarm)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "da")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("da OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					int32u nBlockNo = reader.attributes().value("BlockNo").toInt();
					Q_ASSERT(nBlockNo >= MIN_BLOCKNO_SCADASTUDIO && nBlockNo <= MAX_BLOCKNO_SCADASTUDIO);
					if (nBlockNo <= MIN_BLOCKNO_SCADASTUDIO || nBlockNo > MAX_BLOCKNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("da BlockNo is invalid!!!");
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("Digital alarm limit tagname %1 is invliad!!!").arg(strTagName);
						s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaDinAlarmLimit *pDigitalOfflimit = new CScadaDinAlarmLimit;
					pDigitalOfflimit->SetOccNo(nOccNo);
					memset(pDigitalOfflimit->TagName, 0, sizeof(pDigitalOfflimit->TagName));
					int nSize = strTagName.size();
					strncpy(pDigitalOfflimit->TagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigitalOfflimit->ID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pDigitalOfflimit->Description = strDescription;

					int8u nCondition = reader.attributes().value("Condition").toUInt();
					pDigitalOfflimit->Condition = nCondition;

					pDigitalOfflimit->Category = reader.attributes().value("Category").toUInt();

					int32u nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();
					pDigitalOfflimit->DelayAlarm = nDelayAlarm;

					pDigitalOfflimit->Priority = reader.attributes().value("Priority").toUInt();

					int8u nSupportAck = reader.attributes().value("SupportAck").toUInt();
					pDigitalOfflimit->SupportAck = nSupportAck;

					int8u nSupportDelete = reader.attributes().value("SupportDelete").toUInt();
					pDigitalOfflimit->SupportDelete = nSupportDelete;

					pDigitalOfflimit->AckType = reader.attributes().value("AckType").toUInt();

					QString strSoundFile = reader.attributes().value("SoundFile").toString();
					pDigitalOfflimit->SoundFile = strSoundFile;

					int32u nPlaySoundTimes = reader.attributes().value("PlaySoundTimes").toUInt();
					pDigitalOfflimit->PlaySoundTimes = nPlaySoundTimes;

					pDigitalOfflimit->SpeechAlarmText = reader.attributes().value("SpeechAlarmText").toString();

					pDigitalOfflimit->Beep = reader.attributes().value("Beep").toInt();
					pDigitalOfflimit->PushGraph = reader.attributes().value("PushGraph").toString();
					pDigitalOfflimit->BlinkGraph = reader.attributes().value("BlinkGraph").toInt();
					pDigitalOfflimit->Log = reader.attributes().value("Log").toInt();

					pDigitalOfflimit->BackColor = reader.attributes().value("BackColor").toInt();
					pDigitalOfflimit->TextColor = reader.attributes().value("TextColor").toInt();
					pDigitalOfflimit->BlinkBackColor = reader.attributes().value("BlinkBackColor").toInt();
					pDigitalOfflimit->BlinkTextColor = reader.attributes().value("BlinkTextColor").toInt();

					pDigitalOfflimit->DispGuide = reader.attributes().value("DispGuide").toString();

					QString strCommands = reader.attributes().value("Commands").toString();
					pDigitalOfflimit->Commands = strCommands;

					bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of di alarm limit %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					pAlarm->m_arrDigtallimit.push_back(pDigitalOfflimit);
					PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMLIMITID, strTagName, pDigitalOfflimit);
				}

				reader.readNext();
			}
			else if (reader.isEndElement() && strTmp == "din_alarm_node")
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	bool CScadaData::PushTagNameHashMap(int nIDD, const QString &strTagName, CBaseData *pData)
	{
		auto bFlag = nIDD < 0 || nIDD >= SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM;
		Q_ASSERT(!bFlag);
		if (nIDD < 0 || nIDD >= SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM)
		{
			return false;
		}

		Q_ASSERT(!strTagName.isEmpty());
		if (strTagName.isEmpty())
		{
			return false;
		}

		Q_ASSERT(pData);
		if (!pData)
		{
			return false;
		}

		m_arrMaps[nIDD].insert(make_pair(strTagName.toStdString(), pData));

		return true;
	}

	/*! \fn bool CScadaData::DeleteTagNameHashMap(int nIDD, QString &strChannelTagnname)
	********************************************************************************************************* 
	** \brief Config::CScadaData::DeleteTagNameHashMap 
	** \details 
	** \param nIDD 
	** \param strChannelTagnname 
	** \return bool 
	** \author gw
	** \date 2017年3月7日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::DeleteTagNameHashMap(int nIDD, const QString &strChannelTagnname)
	{
		auto &arr = m_arrMaps[nIDD];

		auto it = arr.find(strChannelTagnname.toStdString());
		if (it != arr.end())
		{
			arr.erase(it);

			return true;
		}

		return false;
	}

	/*! \fn bool CScadaData::CheckTagNameIsExist(const QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaData::CheckTagNameIsExist 
	** \details 检查tagname是否存在
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2017年2月27日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::CheckTagNameIsExist(const QString &strTagName)
	{	
		auto szTmp = strTagName.toStdString();
		for (int i = 0; i < SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM; i++)
		{
			auto it = m_arrMaps[i].find(szTmp);
			if (it != m_arrMaps[i].end())
			{
				return true;
			}
		}

		return false;
	}

	/*! \fn bool CScadaData::ModifyHashMap(int32u nIDD, const QString &strTagName, const QString &strLastTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaData::ModifyHashMap 
	** \details 修改scada的hash内存key
	** \param nIDD 
	** \param strTagName 
	** \param strLastTagName 
	** \return bool 
	** \author gw
	** \date 2017年3月7日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::ModifyHashMap(int32u nIDD, const QString &strTagName, const QString &strLastTagName)
	{
		auto bFlag = nIDD < 0 || nIDD >= SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM;
		Q_ASSERT(!bFlag);
		if (nIDD < 0 || nIDD >= SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM)
		{
			return false;
		}

		bFlag = strTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO || strTagName.isEmpty();
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return false;
		}

		auto &arr = m_arrMaps[nIDD];

		auto it = arr.find(strLastTagName.toStdString());
		if (it != arr.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(arr[strTagName.toStdString()], it->second);
			// Erase old key-value from map
			arr.erase(it);

			return true;
		}

		return false;
	}

	/*! \fn int32u CScadaData::GetArrCount(int nIDD)
	********************************************************************************************************* 
	** \brief Config::CScadaData::GetArrCount 
	** \details 
	** \param nIDD 
	** \return int32u 
	** \author gw
	** \date 2017年3月7日 
	** \note 
	********************************************************************************************************/
	int32u CScadaData::GetArrCount(int nIDD)
	{
		Q_ASSERT(!(nIDD < 0 || nIDD >= SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM));
		if (nIDD < 0 || nIDD >= SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM)
		{
			return -1;
		} 
		return (int32u)m_arrMaps[nIDD].size();
	}

	
	/*! \fn CScadaUserVariable *CScadaData::AddScadaUserVariable()
	********************************************************************************************************* 
	** \brief Config::CScadaData::AddScadaUserVariable 
	** \details 
	** \return CScadaUserVariable * 
	** \author gw
	** \date 2017年3月7日 
	** \note 
	********************************************************************************************************/
	CScadaUserVariable *CScadaData::AddScadaUserVariable()
	{
		auto pUser = new Config::CScadaUserVariable;

		int nCount = GetArrCount(SCADA_VARIABLE_HASH_ID::USERID);

		QString strTagname;
		QString strDesc;
		strTagname.clear();
		strDesc.clear();

		while (true)
		{
			strTagname = QString("%1%2").arg("scadauservariable").arg(nCount + 1);
			strDesc = QString("%1%2").arg(QObject::tr("scadauservariable")).arg(nCount + 1);

			if (CheckTagNameIsExist(strTagname))
			{
				nCount++;

				continue;
			}

			break;
		}


		pUser->m_nID = nCount + 1;

		if (strTagname.size() > MAX_TAGNAME_LEN_SCADASTUDIO || strTagname.size() == 0)
		{
			return nullptr;
		}

		memset(pUser->m_szTagName, 0, sizeof(pUser->m_szTagName));
		int nSize = strTagname.size();
		strncpy(pUser->m_szTagName, strTagname.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		
		if (strDesc.size() > MAX_NAME_LENGTH_SCADASTUDIO)
		{
			pUser->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pUser->m_strDescription = strDesc;
		}

		pUser->m_nType = DATA_TYPE::DT_DOUBLE;
		pUser->m_dbInitValue = 0;
		pUser->m_nInitialQua = InitialQua::NormalQua;
		pUser->SetOccNo(0);

		PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::USERID, strTagname, pUser);

		return pUser;
	}

	/*! \fn CScadaAnalogAlarm * CScadaData::AddAnalogAlarm()
	********************************************************************************************************* 
	** \brief Config::CScadaData::AddAnalogAlarm 
	** \details 添加模拟量告警
	** \return CScadaAnalogAlarm * 
	** \author gw
	** \date 2017年3月7日 
	** \note 
	********************************************************************************************************/
	CScadaAnalogAlarm * CScadaData::AddAnalogAlarm()
	{
		QString strTagName;
		QString strDesc;
		strTagName.clear();
		strDesc.clear();

		auto count = GetArrCount(SCADA_VARIABLE_HASH_ID::ANALOGALARMID);
		while (true)
		{
			strTagName = QString(QObject::tr("analogalarm%1")).arg(count);
			strDesc = QString("analogalarm%1").arg(count);
			

			if (CheckTagNameIsExist(strTagName))
			{
				count++;

				continue;
			}

			break;
		}

		auto pData = new CScadaAnalogAlarm;
		pData->m_nID = count;
		pData->m_nAlarmType = AIALARM;

		if (!CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC))
		{
			return nullptr;
		}
		memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
		auto nSize = strTagName.size();
		strncpy(pData->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));


		if (!CheckDescIsValid(strDesc, SCADAVARIABLE_DESC))
		{
			pData->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pData->m_strDescription = strDesc;
		}

		PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMID, strTagName, pData);

		return pData;
	}

	/*! \fn CScadaAinAlarmLimit * CScadaData::AddAnalogAlarmLimit()
	********************************************************************************************************* 
	** \brief Config::CScadaData::AddAnalogAlarmLimit 
	** \details 
	** \return CScadaAinAlarmLimit * 
	** \author gw
	** \date 2017年3月11日 
	** \note 
	********************************************************************************************************/
	CScadaAinAlarmLimit * CScadaData::AddAnalogAlarmLimit()
	{
		auto pAnalogAlarmOfflimit = new Config::CScadaAinAlarmLimit;

		int nAlarmCount = GetArrCount(SCADA_VARIABLE_HASH_ID::ANALOGALARMLIMITID);
		QString strTagname;
		QString strDesc;
		while (true)
		{
			strTagname = QString("%1%2").arg(QObject::tr("analogalarmofflimit")).arg(nAlarmCount + 1);
			strDesc = QString("%1%2").arg(QObject::tr("analogalarmofflimitdesc")).arg(nAlarmCount + 1);

			if (CheckTagNameIsExist(strTagname))
			{
				nAlarmCount++;

				continue;
			}

			break;
		}




		pAnalogAlarmOfflimit->m_nID = nAlarmCount + 1;

		if (!CheckTagNameIsValid(strTagname, SCADAVARIABLE_DESC))
		{
			return nullptr;
		}
		memset(pAnalogAlarmOfflimit->m_szTagName, 0, sizeof(pAnalogAlarmOfflimit->m_szTagName));
		int nSize = strTagname.size();
		strncpy(pAnalogAlarmOfflimit->m_szTagName, strTagname.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		
		
		if (!CheckDescIsValid(strDesc, SCADAVARIABLE_DESC))
		{
			pAnalogAlarmOfflimit->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pAnalogAlarmOfflimit->m_strDescription = strDesc;
		}

		pAnalogAlarmOfflimit->m_nCondition = GREATEREQUAL;
		pAnalogAlarmOfflimit->m_nCategory = AlarmCategory::COMMON;
		pAnalogAlarmOfflimit->m_nAckType = AlarmAckType::DELETE;

		pAnalogAlarmOfflimit->m_nPriority = 0;
		pAnalogAlarmOfflimit->m_strCommands = "";
		pAnalogAlarmOfflimit->m_strSoundFile = "";
		pAnalogAlarmOfflimit->m_bSupportAck = false;

		PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMLIMITID, strTagname, pAnalogAlarmOfflimit);

		return pAnalogAlarmOfflimit;
	}

	/*! \fn CScadaDinAlarmLimit * CScadaData::AddDigitalAlarmLimit()
	********************************************************************************************************* 
	** \brief Config::CScadaData::AddDigitalAlarmLimit 
	** \details 
	** \return CScadaDinAlarmLimit * 
	** \author gw
	** \date 2017年3月11日 
	** \note 
	********************************************************************************************************/
	CScadaDinAlarmLimit * CScadaData::AddDigitalAlarmLimit()
	{
		auto pDigtalAlarmLimit = new Config::CScadaDinAlarmLimit;

		int nAlarmCount = GetArrCount(SCADA_VARIABLE_HASH_ID::DIGITALALARMLIMITID);

		QString strTagname;
		QString strDesc;
		strTagname.clear();
		strDesc.clear();

		while (true)
		{
			strTagname = QString("%1%2").arg("digtalalarmlimit").arg(nAlarmCount + 1);
			strDesc = QString("%1%2").arg(QObject::tr("digtalalarmlimit")).arg(nAlarmCount + 1);

			//判断tagname是否重复
			if (CheckTagNameIsExist(strTagname))
			{
				nAlarmCount++;

				continue;
			}

			break;
		}

		pDigtalAlarmLimit->ID = nAlarmCount + 1;
		if (!CheckTagNameIsValid(strTagname, SCADAVARIABLE_DESC))
		{
			return nullptr;
		}
		memset(pDigtalAlarmLimit->TagName, 0, sizeof(pDigtalAlarmLimit->TagName));
		int nSize = strTagname.size();
		strncpy(pDigtalAlarmLimit->TagName, strTagname.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		
		
		if (!CheckDescIsValid(strDesc, SCADAVARIABLE_DESC))
		{
			pDigtalAlarmLimit->Description = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pDigtalAlarmLimit->Description = strDesc;
		}

		pDigtalAlarmLimit->Condition = GREATEREQUAL;

		pDigtalAlarmLimit->Priority = 0;
		pDigtalAlarmLimit->Commands = "";
		pDigtalAlarmLimit->SoundFile = "";
		pDigtalAlarmLimit->SupportAck = false;

		PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMLIMITID, strTagname, pDigtalAlarmLimit);

		return pDigtalAlarmLimit;
	}

	CScadaTransform * CScadaData::AddLinearTransform()
	{
		auto pData = new CScadaTransform;
		auto pLinear = new CScadaLinearTransform;

		auto count = GetArrCount(SCADA_VARIABLE_HASH_ID::LINEARTRANSFORMID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strTransformTagName;
		QString strTransformDesc;
		strTransformTagName.clear();
		strTransformDesc.clear();

		while (true)
		{
			strTransformTagName = QString("transform%1").arg(count);
			strTransformDesc = QString(QObject::tr("transform%1")).arg(count);


			if (CheckTagNameIsExist(strTransformTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pLinear->m_nID = count;

		if (!CheckTagNameIsValid(strTransformTagName, SCADAVARIABLE_DESC))
		{
			return nullptr;
		}
		memset(pLinear->m_szTagName, 0, sizeof(pLinear->m_szTagName));
		int nSize = strTransformTagName.size();
		strncpy(pLinear->m_szTagName, strTransformTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		memset(pData->m_szTagName, 0, sizeof(this->m_szTagName));
		strncpy(pData->m_szTagName, strTransformTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		if (!CheckDescIsValid(strTransformDesc, SCADAVARIABLE_DESC))
		{
			pLinear->m_strDescription = strTransformDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pLinear->m_strDescription = strTransformDesc;
		}

		pData->SetLinear(pLinear);
		pData->m_nType = ScaleType::LINEAR;
		//push transform hash
		this->PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::LINEARTRANSFORMID, strTransformTagName, pData);

		return pData;
	}

	CScadaTransform * CScadaData::AddNonLinearTransform()
	{
		auto pData = new CScadaTransform;
		auto pNonLinear = new CScadaNonlinearTransform;

		auto count = GetArrCount(SCADA_VARIABLE_HASH_ID::NONLINEARTRANSFORMID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strTransformTagName;
		QString strTransformDesc;
		strTransformTagName.clear();
		strTransformDesc.clear();

		while (true)
		{
			strTransformTagName = QString("nontransform%1").arg(count);
			strTransformDesc = QString(QObject::tr("nontransform%1")).arg(count);


			if (CheckTagNameIsExist(strTransformTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pNonLinear->m_nID = count;

		if (!CheckTagNameIsValid(strTransformTagName, SCADAVARIABLE_DESC))
		{
			return nullptr;
		}
		memset(pNonLinear->m_szTagName, 0, sizeof(pNonLinear->m_szTagName));
		int nSize = strTransformTagName.size();
		strncpy(pNonLinear->m_szTagName, strTransformTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		memset(pData->m_szTagName, 0, sizeof(this->m_szTagName));
		strncpy(pData->m_szTagName, strTransformTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		if (!CheckDescIsValid(strTransformDesc, SCADAVARIABLE_DESC))
		{
			pNonLinear->m_strDescription = strTransformDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pNonLinear->m_strDescription = strTransformTagName;
		}

		pData->SetNonLinear(pNonLinear);
		pData->m_nType = ScaleType::NONLINEAR;
		//push transform hash
		this->PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::NONLINEARTRANSFORMID, strTransformTagName, pData);

		return pData;
	}

	/*! \fn CScadaDigitalAlarm * CScadaData::AddDigitalAlarm()
	********************************************************************************************************* 
	** \brief Config::CScadaData::AddDigitalAlarm 
	** \details 添加开关量告警
	** \return CScadaDigitalAlarm * 
	** \author gw
	** \date 2017年3月8日 
	** \note 
	********************************************************************************************************/
	CScadaDigitalAlarm * CScadaData::AddDigitalAlarm()
	{
		QString strTagName;
		QString strDesc;
		strTagName.clear();
		strDesc.clear();

		auto count = GetArrCount(SCADA_VARIABLE_HASH_ID::DIGITALALARMID);
		while (true)
		{
			strTagName = QString(QObject::tr("digitalalarm%1")).arg(count);
			strDesc = QString("digitalalarm%1").arg(count);

			if (CheckTagNameIsExist(strTagName))
			{
				count++;

				continue;
			}

			break;
		}

		auto pData = new CScadaDigitalAlarm;
		pData->m_nID = count;
		pData->m_nAlarmType = DIALARM;

		if (!CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC))
		{
			return nullptr;
		}
		memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
		auto nSize = strTagName.size();
		strncpy(pData->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		//desc判断长度
		if (!CheckDescIsValid(strDesc, SCADAVARIABLE_DESC))
		{
			strDesc = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else 
		{
			pData->m_strDescription = strDesc;
		}

		PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMID, pData->m_szTagName, pData);

		return pData;
	}

	/*! \fn bool CScadaData::AddScadaVariableRelationAlarmArr(std::string & tagName, CScadaUserVariable * pVairalbe)
	********************************************************************************************************* 
	** \brief Config::CScadaData::AddScadaVariableRelationAlarmArr 
	** \details 用户变量关联告警
	** \param tagName 
	** \param pVairalbe 
	** \return bool 
	** \author gw
	** \date 2017年3月8日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::AddScadaVariableRelationAlarmArr(std::string & tagName, CScadaUserVariable * pUserVariable)
	{
		auto bFlag = CheckTagNameIsValid(tagName.c_str(), SCADAVARIABLE_DESC);
		
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}
		 
		Q_ASSERT(pUserVariable);
		if (!pUserVariable)
		{
			return false;
		}

		auto it = m_arrAlarmToUserVariableMap.find(tagName);
		if (it != m_arrAlarmToUserVariableMap.end())
		{
			it->second.push_back(pUserVariable);

			return true;
		}

		std::vector<CScadaUserVariable *> vec;
		vec.clear();
		vec.push_back(pUserVariable);
		m_arrAlarmToUserVariableMap.insert(make_pair(tagName, vec));

		return true;
	}

	/*! \fn bool CScadaData::ModifyUserVariableRelationAlarmArr(CScadaUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaData::ModifyUserVariableRelationAlarmArr 
	** \details 用户变量修改关联的告警
	** \param pVairalbe 
	** \param tagName 
	** \param lastTagName 
	** \return bool 
	** \author gw
	** \date 2017年3月8日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::ModifyUserVariableRelationAlarmArr(CScadaUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName)
	{
		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto bFlag = CheckTagNameIsValid(tagName.c_str(), SCADAVARIABLE_DESC) && CheckTagNameIsValid(lastTagName.c_str(), SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}

		auto it = m_arrAlarmToUserVariableMap.find(lastTagName);
		if (it != m_arrAlarmToUserVariableMap.end())
		{
			//hash查找上一次tagname是否存在
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				//hash remove
				arr.erase(ite);

				//先查新选的告警tagname是否存在
				auto itee = m_arrAlarmToUserVariableMap.find(tagName);
				if (itee != m_arrAlarmToUserVariableMap.end())
				{
					itee->second.push_back(pVairalbe);
				}
				else
				{
					//没有关联tagname的用户变量
					std::vector<CScadaUserVariable *> vec;
					vec.clear();
					vec.push_back(pVairalbe);
					m_arrAlarmToUserVariableMap.insert(std::make_pair(tagName, vec));

				}
			}
			else
			{
				//vec未找到
				return false;
			}
		}
		else
		{
			//未查到 error
			return false;
		}

		return true;
	}

	/*! \fn bool CScadaData::ModifyUserVariableAlarmArr(std::string & tagName, std::string & lastTagName, std::string & desc, int type)
	********************************************************************************************************* 
	** \brief Config::CScadaData::ModifyUserVariableAlarmArr 
	** \details 告警修改,用户变量关联的告警也修改
	** \param tagName 
	** \param lastTagName 
	** \param desc 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2017年3月8日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::ModifyUserVariableAlarmArr(std::string & tagName, std::string & lastTagName, std::string & desc, int type)
	{
		bool bFlag = CheckTagNameIsValid(tagName.c_str(), SCADAVARIABLE_DESC) && CheckTagNameIsValid(lastTagName.c_str(), SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}
		
		Q_UNUSED(desc);

		auto it = m_arrAlarmToUserVariableMap.find(lastTagName);
		if (it != m_arrAlarmToUserVariableMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strAlarmTagName = tagName.c_str();
				//var->m_strScaleDesc = desc.c_str();
				var->m_nAlarmType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrAlarmToUserVariableMap[tagName], it->second);
			// Erase old key-value from map
			m_arrAlarmToUserVariableMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CScadaData::DeleteUserVariableAlarmArr(std::string &tagName)
	********************************************************************************************************* 
	** \brief Config::CScadaData::DeleteUserVariableAlarmArr 
	** \details 告警删除,关联的用户变量告警跟着删除
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2017年3月9日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::DeleteUserVariableAlarmArr(std::string &tagName)
	{
		bool bFlag = CheckTagNameIsValid(tagName.c_str(), SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}
		
		auto it = m_arrAlarmToUserVariableMap.find(tagName);
		if (it != m_arrAlarmToUserVariableMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strAlarmTagName = "";
			}

			m_arrAlarmToUserVariableMap.erase(it);
		}

		return true;
	}

	bool CScadaData::DeleteUserVariableRelationAlarmArr(const std::string &szTagName, CScadaUserVariable *pVairalbe)
	{
		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		//没有关联告警，返回true
		if (szTagName.empty())
		{
			return true;
		}


		auto bFlag = CheckTagNameIsValid(szTagName.c_str(), SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}

		auto it = m_arrAlarmToUserVariableMap.find(szTagName);
		if (it != m_arrAlarmToUserVariableMap.end())
		{
			//hash查找上一次tagname是否存在
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{	
				//hash remove
				arr.erase(ite);
			}
			else
			{
				//vec未找到
				return false;
			}
		}
		else
		{
			//未查到 error
			return false;
		}

		return true;
	}

	/*! \fn bool CScadaData::AddScadaVariableRelationScaleArr(std::string &tagName, CScadaUserVariable *pUserVariable)
	********************************************************************************************************* 
	** \brief Config::CScadaData::AddScadaVariableRelationScaleArr 
	** \details 用户变量关联转换tagname时，添加转换tagname对应scada用户变量
	** \param tagName 
	** \param pUserVariable 
	** \return bool 
	** \author gw
	** \date 2017年3月13日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::AddScadaVariableRelationScaleArr(std::string &tagName, CScadaUserVariable *pUserVariable)
	{
		bool bFlag = CheckTagNameIsValid(tagName.c_str(), SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}

		Q_ASSERT(pUserVariable);
		if (!pUserVariable)
		{
			return false;
		}

		auto it = m_arrScaleToUserVariableMap.find(tagName);
		if (it != m_arrScaleToUserVariableMap.end())
		{
			it->second.push_back(pUserVariable);

			return true;
		}

		std::vector<CScadaUserVariable *> vec;
		vec.clear();
		vec.push_back(pUserVariable);
		m_arrScaleToUserVariableMap.insert(make_pair(tagName, vec));

		return true;
	}

	/*! \fn bool CScadaData::ModifyUserVariableRelationScaleArr(CScadaUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName, int type)
	********************************************************************************************************* 
	** \brief Config::CScadaData::ModifyUserVariableRelationScaleArr 
	** \details scada用户变量表格 修改scada用户变量关联的转换
	** \param pVairalbe 
	** \param tagName 
	** \param lastTagName 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2017年3月13日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::ModifyUserVariableRelationScaleArr(CScadaUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName, int type)
	{
		Q_UNUSED(type);

		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		bool bFlag = CheckTagNameIsValid(tagName.c_str(), SCADAVARIABLE_DESC) && CheckTagNameIsValid(lastTagName.c_str(), SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}

		auto it = m_arrScaleToUserVariableMap.find(lastTagName);
		if (it != m_arrScaleToUserVariableMap.end())
		{
			//hash查找上一次tagname是否存在
			auto &arr = it->second;

			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				//hash remove
				arr.erase(ite);

				auto itee = m_arrScaleToUserVariableMap.find(tagName);
				//hash查找tagname是否存在
				if (itee != m_arrScaleToUserVariableMap.end())
				{
					itee->second.push_back(pVairalbe);
				}
				else
				{
					std::vector<CScadaUserVariable *> vec;
					vec.clear();
					vec.push_back(pVairalbe);
					m_arrScaleToUserVariableMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	bool CScadaData::ModifyUserVariableScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	{
		bool bFlag = CheckTagNameIsValid(tagName.c_str(), SCADAVARIABLE_DESC) && CheckTagNameIsValid(lastTagName.c_str(), SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}

		Q_UNUSED(desc);

		auto it = m_arrScaleToUserVariableMap.find(lastTagName);
		if (it != m_arrScaleToUserVariableMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strScaleTagName = tagName.c_str();
				//var->m_strScaleDesc = desc.c_str();
				var->m_nScaleType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrScaleToUserVariableMap[tagName], it->second);
			// Erase old key-value from map
			m_arrScaleToUserVariableMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CScadaData::DeleteUserVariableScaleArr(std::string &tagName)
	********************************************************************************************************* 
	** \brief Config::CScadaData::DeleteUserVariableScaleArr 
	** \details 删除转换 scada用户变量关联的转换也删除
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2017年3月13日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::DeleteUserVariableScaleArr(std::string &tagName)
	{
		bool bFlag = CheckTagNameIsValid(tagName.c_str(), SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}
		
		auto it = m_arrScaleToUserVariableMap.find(tagName);
		if (it != m_arrScaleToUserVariableMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strScaleTagName = "";
			}

			m_arrScaleToUserVariableMap.erase(it);
		}

		return true;
	}

	bool CScadaData::DeleteUserVariableRelationScaleArr(const std::string &szTagName, CScadaUserVariable *pVairalbe)
	{
		//没有关联转换
		if (szTagName.empty())
		{
			return true;
		}

		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		//查找转换key是否存在
		auto it = m_arrScaleToUserVariableMap.find(szTagName);
		//有绑定关系
		if (it != m_arrScaleToUserVariableMap.end())
		{
			auto &arr = it->second;
			//用户变量vec
			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrScaleToUserVariableMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CScadaData::AddUserVariableRelationSourceTagNameArr(const std::string &szTagName, CScadaUserVariable *pVairalbe)
	********************************************************************************************************* 
	** \brief Config::CScadaData::AddUserVariableRelationSourceTagNameArr 
	** \details scada varaible用户变量sourcetagname添加关联
	** \param szTagName node tagname.data tagname
	** \param pVairalbe 
	** \return bool 
	** \author gw
	** \date 2017年5月23日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::AddUserVariableRelationSourceTagNameArr(const std::string &szTagName, CScadaUserVariable *pVairalbe)
	{
		Q_ASSERT(!szTagName.empty());
		if (szTagName.empty())
		{
			return false;
		}

		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		//查找tagname是否存在
		auto it = m_arrSourceTagNameToUserVariableMap.find(szTagName);
		if (it != m_arrSourceTagNameToUserVariableMap.end())
		{
			//若存在,添加到存在的vec中
			it->second.push_back(pVairalbe);

			return true;
		}

		//若不存在，新建一个vec，添加到vec中
		std::vector<CScadaUserVariable *> vec;
		vec.clear();
		vec.push_back(pVairalbe);
		m_arrSourceTagNameToUserVariableMap.insert(make_pair(szTagName, vec));

		return true;
	}

	/*! \fn bool CScadaData::ModifyUserVariableRelationSourceTagNameArr(const std::string &szTagName, const std::string &szLastTagName, CScadaUserVariable *pVairalbe)
	********************************************************************************************************* 
	** \brief Config::CScadaData::ModifyUserVariableRelationSourceTagNameArr 
	** \details scada变量用户变量SourceTagName关联修改
	** \param szTagName 
	** \param szLastTagName 
	** \param pVairalbe 
	** \return bool 
	** \author gw
	** \date 2017年5月23日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::ModifyUserVariableRelationSourceTagNameArr(const std::string &szTagName, const std::string &szLastTagName, CScadaUserVariable *pVairalbe)
	{
		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto bFlag = szTagName.empty() || szLastTagName.empty();
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return false;
		}

		//查找szLastTagName是否存在，若不存在，返回错误
		auto it = m_arrSourceTagNameToUserVariableMap.find(szLastTagName);
		if (it != m_arrSourceTagNameToUserVariableMap.end())
		{
			//hash查找上一次tagname存在
			auto &arr = it->second;

			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				//hash remove
				arr.erase(ite);

				auto itee = m_arrSourceTagNameToUserVariableMap.find(szTagName);
				//hash查找tagname是否存在
				if (itee != m_arrSourceTagNameToUserVariableMap.end())
				{
					itee->second.push_back(pVairalbe);
				}
				else
				{
					std::vector<CScadaUserVariable *> vec;
					vec.clear();
					vec.push_back(pVairalbe);
					m_arrSourceTagNameToUserVariableMap.insert(make_pair(szTagName, vec));
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CScadaData::ModifyUserVariableSourceTagNameArr(const std::string &szTagName, const std::string &szLastTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaData::ModifyUserVariableSourceTagNameArr 
	** \details Scada用户变量的sourcetagname可以从前置AI、DI、AO、DO绑定关联    前置系统变量和用户变量暂时不绑定关联
	** \param szTagName 
	** \param szLastTagName 
	** \return bool 
	** \author gw
	** \date 2017年5月24日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::ModifyUserVariableSourceTagNameArr(const std::string &szTagName, const std::string &szLastTagName)
	{
		//查找szLastTagName是否存在
		auto it = m_arrSourceTagNameToUserVariableMap.find(szLastTagName);
		if (it != m_arrSourceTagNameToUserVariableMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				auto list = var->m_strSourceTagName.split(".");
				//暂定判定方法
				if (list.count() == 3)
				{
					var->m_strSourceTagName = QString("%1.%2").arg(szTagName.c_str()).arg(list[2]);
				}
				else
				{	
					QString strError = QObject::tr("Modify Scada UserVariable SourceTagName fail!!!");
					CBaseData::LogString(SCADASTUDIO_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
					return false;
				}

			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrSourceTagNameToUserVariableMap[szTagName], it->second);
			// Erase old key-value from map
			m_arrSourceTagNameToUserVariableMap.erase(it);
		}
		
		return true;
	}

	/*! \fn bool CScadaData::DeleteUserVariableRelationSourceTagNameArr(const std::string &szTagName, CScadaUserVariable *pVairalbe)
	********************************************************************************************************* 
	** \brief Config::CScadaData::DeleteUserVariableRelationSourceTagNameArr 
	** \details scada变量删除，如果scada变量的SourceTagName有关联，那么关联关系删除
	** \param szTagName 
	** \param pVairalbe 
	** \return bool 
	** \author gw
	** \date 2017年5月23日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::DeleteUserVariableRelationSourceTagNameArr(const std::string &szTagName, CScadaUserVariable *pVairalbe)
	{
		if (szTagName.empty())
		{
			return true;
		}
		
		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto it = m_arrSourceTagNameToUserVariableMap.find(szTagName);
		//有绑定关系
		if (it != m_arrSourceTagNameToUserVariableMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrSourceTagNameToUserVariableMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CScadaData::DeleteUserVariableSourceTagNameArr(const std::string &szTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaData::DeleteUserVariableSourceTagNameArr 
	** \details Scada用户变量的sourcetagname可以从前置AI、DI、AO、DO绑定关联    前置系统变量和用户变量暂时不绑定关联
	** \param szTagName 
	** \return bool 
	** \author gw
	** \date 2017年5月24日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::DeleteUserVariableSourceTagNameArr(const std::string &szTagName)
	{
		auto it = m_arrSourceTagNameToUserVariableMap.find(szTagName);
		if (it != m_arrSourceTagNameToUserVariableMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strSourceTagName = "";
			}

			m_arrSourceTagNameToUserVariableMap.erase(it);
		}
		
		return true;
	}

	/*! \fn 
	********************************************************************************************************* 
	** \brief Config::CScadaData::DeleteUserVariableCombineRelation 
	** \details 用户变量删除，组合关联关系删除
	** \param szTagName 
	** \param pVairalbe 
	** \return bool 
	** \author gw
	** \date 2017年6月15日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::DeleteUserVariableCombineRelation(const std::string &szTagName, CScadaUserVariable *pVairalbe)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), SCADASTUDIO_DESC))
		{
			return false;
		}
		
		//用户变量删除，如果用户变量sourcetagname有关联关系，那么关联关系删除
		if (!pVairalbe->m_strSourceTagName.isEmpty())
		{
			auto list = pVairalbe->m_strSourceTagName.split(".");
			//拼接tagname暂定判定方法
			if (list.count() == 3)
			{
				//拼接tagname node.data
				auto strTagNameCombine = QString("%1.%2").arg(list[0]).arg(list[1]);

				if (!this->DeleteUserVariableRelationSourceTagNameArr(strTagNameCombine.toStdString(), pVairalbe))
				{
					auto strError = QObject::tr("Delete Scada variable User Variable delete RelationSourceTagName %1 fail!!!").arg(strTagNameCombine);
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

					return false;
				}
			}
		}

		//用户变量删除，如果用户变量的ALARM有关联关系，那么关联关系删除
		if (!pVairalbe->m_strAlarmTagName.isEmpty())
		{
			if (!this->DeleteUserVariableRelationAlarmArr(pVairalbe->m_strAlarmTagName.toStdString(), pVairalbe))
			{
				auto strError = QObject::tr("Delete Scada variable User Variable delete RelationSourceTagName %1 fail!!!").arg(pVairalbe->m_strAlarmTagName);
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}
		}

		//用户变量删除，如果用户变量的scale有关联，那么关联管也删除
		if (!pVairalbe->m_strScaleTagName.isEmpty())
		{
			if (!this->DeleteUserVariableRelationScaleArr(pVairalbe->m_strScaleTagName.toStdString(), pVairalbe))
			{
				auto strError = QObject::tr("Delete UserVariable Relation Scale %1 fail!!!").arg(pVairalbe->m_strScaleTagName);
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}
		}

		return true;
	}

	/*! \fn bool CScadaData::ChangeTagNameNodify(const QString &stTagName, const QString &strLastTagName, int nType)
	********************************************************************************************************* 
	** \brief Config::CScadaData::ChangeTagNameNodify 
	** \details 前置DEVICE_AI DEVICE_DI DEVICE_AO DEVICE_DO 点tagname修改,通知绑定点的模块做出相应的修改 删除tagname为空 
	** \param stTagName 
	** \param strLastTagName 
	** \param nType 
	** \return bool 
	** \author gw
	** \date 2017年5月24日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::ChangeTagNameNodify(const QString &stTagName, const QString &strLastTagName, int nType)
	{
		if (Module_ID::DEVICE_AI == nType ||
			Module_ID::DEVICE_DI == nType || 
			Module_ID::DEVICE_AO == nType || 
			Module_ID::DEVICE_DO == nType ||
			Module_ID::USERVARAIBLE)
		{
			if (stTagName.isEmpty())
			{
				//stTagName为空，删除关联关系
				return DeleteUserVariableSourceTagNameArr(strLastTagName.toStdString());
			}
			else
			{
				return ModifyUserVariableSourceTagNameArr(stTagName.toStdString(), strLastTagName.toStdString());
			}
		}
		else if (Module_ID::NODE_MODULE_SERVER == nType)
		{
			return this->ChangeNodeServerTagNameNodifyScadaVariable(stTagName.toStdString(), strLastTagName.toStdString(), nType);
		}

		
		return true;
	}

	bool CScadaData::GetUserVariableRelatedSourceOccNo(QString &strTagName, QString &strRelatedTagName, QString &strOutput)
	{
		auto bFlag = strRelatedTagName.isEmpty() || strRelatedTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			auto strError = QObject::tr("User variable tagname %1 related sourcetagname is invalid!!!").arg(strTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		if (strTagName.isEmpty())
		{
			return false;
		}

		//[].[].[]暂时考虑这个
		auto &list = strRelatedTagName.split(".");
		if (list.size() == 3)
		{
			//
			auto &strNodeTagName = list[0];
			auto &strTagName = list[1];
			auto &strAttrTagName = list[2];
			int nNodeType = -1;

			auto pNode = m_pCore->GetModule("node_config");
			Q_ASSERT(pNode);
			if (!pNode)
			{
				return nullptr;
			}
			//node -> fes
			std::string szFesTagName = pNode->GetFesConfigNameByNodeName(strNodeTagName.toStdString());
			//Q_ASSERT(!szFesTagName.empty());;
			if (szFesTagName.empty())
			{
				QMessageBox box;
				box.setText(QObject::tr("Fes node %1 do not relate to fes!!!").arg(strNodeTagName));
				box.exec();

				return false;
			}

			auto pFesModule = m_pCore->GetModule("fes");
			Q_ASSERT(pNode);
			if (!pNode)
			{
				return nullptr;
			}
			strOutput = pFesModule->GetOccNoFromScadaVariableUserSourceTagName(szFesTagName.c_str(), strRelatedTagName);
		}

		return true;
	}

	/*! \fn void void CScadaData::InitScadaSystemVariable()
	*********************************************************************************************************
	** \brief Config::CScadaVirableConfigData::InitScadaSystemVariable
	** \details 初始化scada变量
	** \return void
	** \author gw
	** \date 2016年12月26日
	** \note
	********************************************************************************************************/
	void CScadaData::InitScadaSystemVariable()
	{
		auto *pTmp = new Config::CScadaSystemVaraible;
		pTmp->m_nID = 1;
		memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
		QString strTmp = "$YEAR";
		int nSize = strTmp.size();
		strncpy(pTmp->m_szTagName, strTmp.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		pTmp->m_strDescription = QObject::tr("system-year");
		if (pTmp->m_strDescription.size() > MAX_NAME_LENGTH_SCADASTUDIO)
		{
			pTmp->m_strDescription = pTmp->m_strDescription.mid(0, MAX_NAME_LENGTH_SCADASTUDIO);
		}


		pTmp->m_nType = DATA_TYPE::DT_WORD;
		if (CheckTagNameIsExist(pTmp->m_szTagName))
		{
			auto strError = QObject::tr("scada system variable tagname %1 is exist!!!").arg(pTmp->m_szTagName);
			s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		else
		{
			if (!PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::SYSTEMID, pTmp->m_szTagName, pTmp))
			{
				auto strError = QObject::tr("scada system variable init error!!!");
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			m_arrSystemVariable.push_back(pTmp);
		}



		pTmp = new Config::CScadaSystemVaraible;
		pTmp->m_nID = 2;
		memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
		strTmp = "$MONTH";
		nSize = strTmp.size();
		strncpy(pTmp->m_szTagName, strTmp.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));
		pTmp->m_strDescription = QObject::tr("system-month");
		if (pTmp->m_strDescription.size() > MAX_NAME_LENGTH_SCADASTUDIO)
		{
			pTmp->m_strDescription = pTmp->m_strDescription.mid(0, MAX_NAME_LENGTH_SCADASTUDIO);
		}

		pTmp->m_nType = DATA_TYPE::DT_WORD;
		if (CheckTagNameIsExist(pTmp->m_szTagName))
		{
			auto strError = QObject::tr("system variable tagname %1 is exist!!!").arg(pTmp->m_szTagName);
			s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		else
		{
			if (!PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::SYSTEMID, pTmp->m_szTagName, pTmp))
			{
				auto strError = QObject::tr("scada system variable init error!!!");
				s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			m_arrSystemVariable.push_back(pTmp);
		}


		pTmp = new Config::CScadaSystemVaraible;
		pTmp->m_nID = 3;
		memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
		strTmp = "$DAY";
		nSize = strTmp.size();
		strncpy(pTmp->m_szTagName, strTmp.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));
		pTmp->m_strDescription = QObject::tr("system-day");
		if (pTmp->m_strDescription.size() > MAX_NAME_LENGTH_SCADASTUDIO)
		{
			pTmp->m_strDescription = pTmp->m_strDescription.mid(0, MAX_NAME_LENGTH_SCADASTUDIO);
		}

		pTmp->m_nType = DATA_TYPE::DT_WORD;
		if (CheckTagNameIsExist(pTmp->m_szTagName))
		{
			auto strError = QObject::tr("scada system variable tagname %1 is exist!!!").arg(pTmp->m_szTagName);
			s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		else
		{
			if (!PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::SYSTEMID, pTmp->m_szTagName, pTmp))
			{
				auto strError = QObject::tr("scada system variable init error!!!");
				s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			m_arrSystemVariable.push_back(pTmp);
		}
	}



	/*! \fn CBaseData * CScadaData::GetData(QString &strTagName, int nType)
	********************************************************************************************************* 
	** \brief Config::CScadaData::GetData 
	** \details 根据tagname得到数据
	** \param strTagName 
	** \param nType 
	** \return CBaseData * 
	** \author gw
	** \date 2017年3月15日 
	** \note 
	********************************************************************************************************/
	CBaseData * CScadaData::GetData(QString &strTagName, int nType)
	{
		Q_ASSERT(!strTagName.isEmpty());
		if (strTagName.isEmpty())
		{
			return nullptr;
		}

		auto bFlag = strTagName.size() > 0 && strTagName.size() < MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return nullptr;
		}

		bFlag = nType > 0 && nType < SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM;
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return nullptr;
		}

		auto it = m_arrMaps[nType].find(strTagName.toStdString());
		if (it != m_arrMaps[nType].end())
		{
			return it->second;
		}

		return nullptr;
	}

	/*! \fn 
	********************************************************************************************************* 
	** \brief Config::CScadaData::ChangeNodeServerTagNameNodifyScadaVariable 
	** \details Node server的tagname修改，通知scada varaible模块
	** \param stTagName 
	** \param strLastTagName 
	** \param nType 
	** \return bool 
	** \author gw
	** \date 2017年6月14日 
	** \note 
	********************************************************************************************************/
	bool CScadaData::ChangeNodeServerTagNameNodifyScadaVariable(const std::string &szNodeTagName, const std::string &szNodeLastTagName, int nType)
	{
		return true;
	}

	//scada变量组
	/*! \fn CScadaVariableGroup::CScadaVariableGroup()
	********************************************************************************************************* 
	** \brief Config::CScadaVariableGroup::CScadaVariableGroup 
	** \details scada变量组 包含变量和组 scada包含变量、转换和告警
	** \return  
	** \author gw
	** \date 2017年2月25日 
	** \note 
	********************************************************************************************************/
	CScadaGroup::CScadaGroup()
	{
		m_arrGroup.clear();
		m_arrItem.clear();
	}

	/*! \fn CScadaGroup::~CScadaGroup()
	********************************************************************************************************* 
	** \brief Config::CScadaGroup::~CScadaGroup 
	** \details 删除组和项
	** \return  
	** \author gw
	** \date 2017年2月25日 
	** \note 
	********************************************************************************************************/
	CScadaGroup::~CScadaGroup()
	{
		//delete group
		for (auto it = m_arrGroup.begin(); it != m_arrGroup.end(); ++it)
		{
			delete it->second;
		}
		m_arrGroup.clear();


		for (auto it = m_arrItem.begin(); it != m_arrItem.end(); ++it)
		{
			delete it->second;
		}
		m_arrItem.clear();
	}

	/*! \fn bool CScadaGroup::CheckModifyGroupNameExist(CScadaGroup *pGroup, QString strGroupName)
	********************************************************************************************************* 
	** \brief Config::CScadaGroup::CheckModifyGroupNameExist 
	** \details 检查修改的组名是否重复
	** \param pGroup 
	** \param strGroupName 
	** \return bool 
	** \author gw
	** \date 2017年3月14日 
	** \note 
	********************************************************************************************************/
	bool CScadaGroup::CheckModifyGroupNameExist(CScadaGroup *pGroup, QString strGroupName)
	{
		for (auto it = m_arrGroup.begin(); it != m_arrGroup.end(); ++it)
		{
			if (it->second != pGroup)
			{
				if (it->second->m_strGroup == strGroupName)
				{
					return true;
				}
			}
		}

		return false;
	}

	CScadaGroup* CScadaGroup::CreateGroup()
	{
		int nIndex = (int)m_arrGroup.size();
		QString strName = "";
		while (true)
		{
			strName = QString("%1%2").arg(GROUP_NAME).arg(nIndex);

			auto it = m_arrGroup.find(strName.toStdString());
			if (it == m_arrGroup.end())
			{
				auto pGroup = new CScadaGroup;
				pGroup->m_strGroup = strName;

				m_arrGroup.insert(std::make_pair(strName.toStdString(), pGroup));

				return pGroup;
			}
			else
			{
				nIndex++;
			}
		}

		return nullptr;
	}











	bool CScadaGroup::ModifyGroup(const QString &strTagName, const QString &strLastTagName)
	{
		auto ite = m_arrGroup.find(strLastTagName.toStdString());
		if (ite != m_arrGroup.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.

			ite->second->m_strGroup = strTagName;
			std::swap(m_arrGroup[strTagName.toStdString()], ite->second);
			// Erase old key-value from map
			m_arrGroup.erase(ite);

			return true;
		}

		return false;
	}

	bool CScadaGroup::DeletGroup(CScadaGroup *pGroup, const QString &strTagGroup)
	{
		Q_UNUSED(pGroup);

		auto it = m_arrGroup.find(strTagGroup.toStdString());
		if (it == m_arrGroup.end())
		{
			return false;
		}

		delete it->second;

		m_arrGroup.erase(it);

		return true;
	}

	/*! \fn bool CScadaGroup::DeletItem(CScadaData *pScada, const QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaGroup::DeletItem 
	** \details 删除组内的项
	** \param pScada 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2017年3月14日 
	** \note 
	********************************************************************************************************/
	bool CScadaGroup::DeletItem(CScadaData *pScada, const QString &strTagName)
	{
		Q_UNUSED(pScada);

		auto it = m_arrItem.find(strTagName.toStdString());
		if (it == m_arrItem.end())
		{
			return false;
		}

		delete it->second;

		m_arrItem.erase(it);

		return true;
	}

	bool CScadaGroup::ModifyItem(const QString &strTagName, const QString &strLastTagName)
	{
		auto ite = m_arrItem.find(strLastTagName.toStdString());
		if (ite != m_arrItem.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.

			memset(ite->second->m_szTagName, 0, sizeof(ite->second->m_szTagName));
			int nSize = strTagName.size();
			strncpy(ite->second->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));
			std::swap(m_arrItem[strTagName.toStdString()], ite->second);
			// Erase old key-value from map
			m_arrItem.erase(ite);

			return true;
		}

		auto it = m_arrGroup.begin();
		for (; it != m_arrGroup.end(); ++it)
		{
			if (it->second->ModifyItem(strTagName, strLastTagName))
			{
				return true;
			}
		}

		return false;
	}

	/*! \fn CScadaVirableConfigData::CScadaVirableConfigData()
	*********************************************************************************************************
	** \brief Config::CScadaVirableConfigData::CScadaVirableConfigData
	** \details
	** \return
	** \author gw
	** \date 2016年12月26日
	** \note
	********************************************************************************************************/
	CScadaVariableConfigData::CScadaVariableConfigData()
		:m_pGroup(nullptr)
	{
		if (!m_pGroup)
		{
			m_pGroup = new CScadaGroup;
		}

		m_arrScaleToUserVariableMap.clear();
		m_arrAlarmToUserVariableMap.clear();

		m_arrHashTagNameMap.clear();
		m_arrScadaDatas.clear();
	}

	CScadaVariableConfigData::~CScadaVariableConfigData()
	{
		Clear();
	}

	/*! \fn CScadaData *CScadaVariableConfigData::AddNewScadaData(CScadaGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::AddNewScadaData 
	** \details config添加scada,同时添加到scada本身的hash管理内存中
	** \param pGroup 
	** \return CScadaData * scada组,管理scada
	** \author gw
	** \date 2017年3月7日 
	** \note 
	********************************************************************************************************/
	CScadaData *CScadaVariableConfigData::AddNewScadaData(CScadaGroup *pGroup)
	{
		Q_ASSERT(s_pGlobleCore);
		if (!s_pGlobleCore)
		{
			return nullptr;
		}

		CScadaData *pData = new CScadaData(s_pGlobleCore);
		auto index = (int)m_arrHashTagNameMap.size();
		while (true)
		{
			QString strScada = QString("Scada%1").arg(index);
			QString strDesc = QString(QObject::tr("Scada%1")).arg(index);

			std::string str = strScada.toStdString();
			std::unordered_map<std::string, CScadaData *>::const_iterator it = m_arrHashTagNameMap.find(str);
			if (it == m_arrHashTagNameMap.end())
			{
				auto &tmp = std::make_pair(str, pData);
				m_arrHashTagNameMap.insert(tmp);
			}
			else
			{
				index++;

				continue;
			}

			pData->m_nID = index;

			memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
			int nSize = strScada.size();
			strncpy(pData->m_szTagName, strScada.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

			if (strDesc.size() > MAX_NAME_LENGTH_SCADASTUDIO)
			{
				pData->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO);
			}
			else
			{
				pData->m_strDescription = strDesc;
			}

			//加入hash
			pGroup->m_arrItem.insert(std::make_pair(str, pData));
			break;
		}

		QString str = QString("%1").arg(pData->m_szTagName);

		//config scada vec
		m_arrScadaDatas.push_back(pData);
		
		if (pData->CheckTagNameIsExist(str))
		{
			auto strError = QObject::tr("Scada tagname is exist!!!");
			s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return nullptr;
		}

		//添加到scada的hash内存管理
		pData->PushTagNameHashMap(SCADA_VARIABLE_HASH_ID::SCADAID, str, pData);

		return pData;
	}

	/*! \fn bool CScadaVirableConfigData::Clear()
	********************************************************************************************************* 
	** \brief Config::CScadaVirableConfigData::Clear 
	** \details clear数据
	** \return bool 
	** \author gw
	** \date 2016年12月26日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::Clear()
	{
		m_arrHashTagNameMap.clear();
	
		for (int32u i = 0; i < SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM; i++)
		{
			m_arrMaps[i].clear();
		}

		if (m_pGroup)
		{
			delete m_pGroup;
			m_pGroup = nullptr;
		}

		return true;
	}

	/*! \fn bool CScadaVirableConfigData::Create()
	********************************************************************************************************* 
	** \brief Config::CScadaVirableConfigData::Create 
	** \details 
	** \return bool 
	** \author gw
	** \date 2016年12月26日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::Create()
	{
		if (!m_pGroup)
		{
			m_pGroup = new CScadaGroup;
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::CheckModifyScadaTagNameExist(const QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::CheckModifyScadaTagNameExist 
	** \details scada config检查修改的scada tagname是否存在
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2017年2月27日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::CheckModifyScadaTagNameExist(const QString &strTagName)
	{
		auto it = m_arrHashTagNameMap.find(strTagName.toStdString());
		if (it != m_arrHashTagNameMap.end())
		{
			return true;
		}

		return false;
	}


	/*! \fn bool CScadaVariableConfigData::ModifyScadaHashMap(const QString &strScadaTagName, const QString &strLastScadaTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::ModifyScadaHashMap 
	** \details Config的scada hash修改
	** \param strScadaTagName 
	** \param strLastScadaTagName 
	** \return bool 
	** \author gw
	** \date 2017年3月9日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::ModifyScadaHashMap(const QString &strScadaTagName, const QString &strLastScadaTagName)
	{
		auto bFlag = strScadaTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO || strLastScadaTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO ||
			strScadaTagName.isEmpty() || strLastScadaTagName.isEmpty();
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			auto strError = QObject::tr("Scada tagname length is invalid,Scada hash modify fail!!!");
			s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		//查找old tagname
		auto it = m_arrHashTagNameMap.find(strLastScadaTagName.toStdString());
		if (it != m_arrHashTagNameMap.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrHashTagNameMap[strScadaTagName.toStdString()], it->second);
			// Erase old key-value from map
			m_arrHashTagNameMap.erase(it);

			return true;
		}

		return false;
	}

	/*! \fn bool CScadaVariableConfigData::DeleteScadaHahsMap(const QString &strScadaTagName)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::DeleteScadaHahsMap 
	** \details 
	** \param strScadaTagName 
	** \return bool 
	** \author gw
	** \date 2017年3月11日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::DeleteScadaHahsMap(const QString &strScadaTagName)
	{
		auto bFlag = strScadaTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO || strScadaTagName.isEmpty();
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			auto strError = QObject::tr("Scada tagname length is invalid,Scada hash delete fail!!!");
			s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		auto it = m_arrHashTagNameMap.find(strScadaTagName.toStdString());
		if (it != m_arrHashTagNameMap.end())
		{
			m_arrHashTagNameMap.erase(it);

			return true;
		}

		return false;
	}

	/*! \fn bool CScadaVirableConfigData::SaveProjectScadavarialbeNode(QDomDocument *pXml, QDomElement &e, const QString &szRoot)
	********************************************************************************************************* 
	** \brief Config::CScadaVirableConfigData::SaveProjectScadavarialbeNode 
	** \details 保存scadavariable模块
	** \param pXml 
	** \param e 
	** \param szRoot 
	** \return bool 
	** \author gw
	** \date 2016年12月31日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveProjectScadavarialbeNode(QDomDocument *pXml, QDomElement &e, const QString &szRoot, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pXml);
		if (!pXml)
		{
			return false;
		}

		Q_ASSERT(pXml);
		if (!pXml)
		{
			return false;
		}

		auto strTmp = QObject::tr("Scada varaible log start!!!");
		MYLIB::Log2File(LOG_SCADA_VARAIBLE_LOG, strTmp.toStdString().c_str(), false);

		SaveChildNode(pXml, e, m_pGroup);

		//大排行号
		int nOccNo = 0;
		SaveChildItem(m_pGroup, szRoot, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

		return true;
	}

	/*! \fn bool CScadaVirableConfigData::SaveChildNode(QDomDocument *pXml, QDomElement &e, CScadaUserVariableGroup *pFesGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaVirableConfigData::SaveChildNode 
	** \details 保存工程文件
	** \param pXml 
	** \param e 
	** \param pFesGroup 
	** \return bool 
	** \author gw
	** \date 2016年12月31日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveChildNode(QDomDocument *pXml, QDomElement &e, CScadaGroup *pGroup)
	{
		Q_ASSERT(pXml);
		if (pXml == nullptr)
		{
			return false;
		}

		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto it = pGroup->m_arrGroup.begin();
		for (; it != pGroup->m_arrGroup.end(); ++it)
		{
			QDomElement fElet = pXml->createElement("group");
			fElet.setAttribute("name", it->second->m_strGroup);

			e.appendChild(fElet);

			SaveChildNode(pXml, fElet, it->second);
		}

		auto ite = pGroup->m_arrItem.begin();
		for (; ite != pGroup->m_arrItem.end(); ++ite)
		{
			QDomElement fElet = pXml->createElement("file");
			auto strFile = QString("%1").arg(ite->second->m_szTagName);
			fElet.setAttribute("name", strFile);

			e.appendChild(fElet);
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::SaveChildItem(CScadaGroup *pFesGroup, const QString &szRoot, int nOccNo, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::SaveChildItem 
	** \details 
	** \param pFesGroup 
	** \param szRoot 
	** \param nOccNo 
	** \param  
	** \param pHash 
	** \param pStringPoolVec 
	** \param pDescStringPoolOccNo 
	** \return bool 
	** \author gw
	** \date 2017年3月14日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveChildItem(CScadaGroup *pFesGroup, const QString &szRoot, int nOccNo, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		auto it = pFesGroup->m_arrItem.begin();
		for (; it != pFesGroup->m_arrItem.end(); ++it)
		{
			nOccNo++;
			it->second->SaveScadaData(szRoot, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);
		}

		auto ite = pFesGroup->m_arrGroup.begin();
		for (; ite != pFesGroup->m_arrGroup.end(); ++ite)
		{
			SaveChildItem(ite->second, szRoot, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);
		}

		return true;
	}

	/*! \fn bool CScadaVirableConfigData::SaveSystemVariable(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CScadaVirableConfigData::SaveSystemVariable 
	** \details 
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2017年1月3日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveScadaSystemVariable(QXmlStreamWriter &writer)
	{
		//writer.writeStartElement("scadasystem");
		//writer.writeAttribute("Count", QString("%1").arg(m_arrSystemVariable.size()));

		//int nOccNo = 0;
		//for each (auto var in m_arrSystemVariable)
		//{
		//	writer.writeStartElement("s");

		//	nOccNo++;
		//	var->SetOccNo(nOccNo);
		//	writer.writeAttribute("OccNo", QString("%1").arg(nOccNo));
		//	writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
		//	writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
		//	writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
		//	writer.writeAttribute("DataType", QString("%1").arg(var->m_nType));

		//	writer.writeEndElement();
		//}

		//writer.writeEndElement();

		return true;
	}

	/*! \fn bool CScadaVirableConfigData::SaveScadaUserVariable(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CScadaVirableConfigData::SaveScadaUserVariable 
	** \details 
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2017年1月3日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveScadaUserVariable(QXmlStreamWriter &writer)
	{
		//writer.writeStartElement("scadauser");

		//SaveScadaUserVariableChildNode(writer, m_pScadaUserGroup);

		//writer.writeEndElement();

		return true;
	}

	/*! \fn bool CScadaVirableConfigData::SaveScadaUserVariableChildNode(QXmlStreamWriter &writer, Config::CScadaUserVariableGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaVirableConfigData::SaveScadaUserVariableChildNode 
	** \details 保存用户变量
	** \param writer 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月3日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveScadaUserVariableChildNode(QXmlStreamWriter &writer, Config::CScadaUserVariableGroup *pGroup)
	{	
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto ite = pGroup->m_arrGroup.begin();
		for (; ite != pGroup->m_arrGroup.end(); ++ite)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", (*ite)->GetGroupName());
			writer.writeAttribute("count", QString("%1").arg((*ite)->m_arrItem.size()));

			SaveScadaUserVariableChildNode(writer, (*ite));

			writer.writeEndElement();
		}

		auto it = pGroup->m_arrItem.begin();
		for each (auto var in pGroup->m_arrItem)
		{
			writer.writeStartElement("u");

			writer.writeAttribute("OccNo", QString("%1").arg(var->GetOccNo()));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			writer.writeAttribute("DataType", QString("%1").arg(var->m_nType));
			writer.writeAttribute("InitValue", QString("%1").arg(var->m_dbInitValue));
			writer.writeAttribute("InitialQua", QString("%1").arg(var->m_nInitialQua));
			writer.writeAttribute("Address", QString("%1").arg(var->m_strAddress));
			writer.writeAttribute("PinLabel", QString("%1").arg(var->m_strPinLabel));
			writer.writeAttribute("SourceTagName", QString("%1").arg(var->m_strSourceTagName));
			//关联的转换大排行号
			//判断转换tagname是否为空
			if (var->m_strScaleTagName.isEmpty())
			{
				writer.writeAttribute("ScaleOccNo", QString("%1").arg(0));
				writer.writeAttribute("ScaleType", QString("%1").arg(LINEAR));
				writer.writeAttribute("ScaleTagName", QString("%1").arg(""));
			}
			else
			{
				Config::CScadaTransform *pTransform = nullptr;
				if (ScaleType::LINEAR == var->m_nScaleType)
				{
					pTransform = (Config::CScadaTransform *)GetData(var->m_strScaleTagName, SCADA_VARIABLE_HASH_ID::LINEARTRANSFORMID);
				}
				else if (ScaleType::NONLINEAR == var->m_nScaleType)
				{
					pTransform = (Config::CScadaTransform *)GetData(var->m_strScaleTagName, SCADA_VARIABLE_HASH_ID::NONLINEARTRANSFORMID);
				}

				Q_ASSERT(pTransform);
				if (!pTransform)
				{
					auto strError = QObject::tr("scale tagname %1 is isvalid!!!").arg(var->m_strScaleTagName);
					s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					writer.writeEndElement();

					break;
				}

				writer.writeAttribute("ScaleOccNo", QString("%1").arg(pTransform->GetOccNo()));
				writer.writeAttribute("ScaleType", QString("%1").arg(var->m_nScaleType));
				writer.writeAttribute("ScaleTagName", QString("%1").arg(var->m_strScaleTagName));
			}

			if (var->m_strAlarmTagName.isEmpty())
			{
				writer.writeAttribute("AlarmOccNo", QString("%1").arg(0));
				writer.writeAttribute("AlarmType", QString("%1").arg(AlarmType::AIALARM));
				writer.writeAttribute("AlarmTagName", QString("%1").arg(""));
			}
			else
			{
				if (AlarmType::AIALARM == var->m_nAlarmType)
				{
					auto pAlarm = (Config::CScadaAnalogAlarm *)GetData(var->m_strAlarmTagName, SCADA_VARIABLE_HASH_ID::ANALOGALARMID);
					Q_ASSERT(pAlarm);
					if (!pAlarm)
					{
						auto strError = QObject::tr("alarm tagname %1 is isvalid!!!").arg(var->m_strAlarmTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						writer.writeEndElement();

						break;
					}

					writer.writeAttribute("AlarmOccNo", QString("%1").arg(pAlarm->GetOccNo()));
					writer.writeAttribute("AlarmType", QString("%1").arg(AlarmType::AIALARM));
					writer.writeAttribute("AlarmTagName", QString("%1").arg(var->m_strAlarmTagName));
				}
				else if (AlarmType::DIALARM == var->m_nAlarmType)
				{
					auto pAlarm = (Config::CScadaDigitalAlarm *)GetData(var->m_strAlarmTagName, SCADA_VARIABLE_HASH_ID::DIGITALALARMID);
					Q_ASSERT(pAlarm);
					if (!pAlarm)
					{
						auto strError = QObject::tr("alarm tagname %1 is isvalid!!!").arg(var->m_strAlarmTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						writer.writeEndElement();

						break;
					}

					writer.writeAttribute("AlarmOccNo", QString("%1").arg(pAlarm->GetOccNo()));
					writer.writeAttribute("AlarmType", QString("%1").arg(AlarmType::DIALARM));
					writer.writeAttribute("AlarmTagName", QString("%1").arg(var->m_strAlarmTagName));
				}
			}


			writer.writeEndElement();
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::SaveAlarm(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::SaveAlarm 
	** \details 
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2017年1月16日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveAlarm(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		////writer.writeStartElement("alarm");

		//Q_ASSERT(m_pScadaAnalogAlarmGroup);
		//if (!m_pScadaAnalogAlarmGroup)
		//{
		//	return false;
		//}

		//int nCounts = (int)m_arrMaps[SCADAVARIABLEHASHID::ANALOGALARMID].size();
		//writer.writeStartElement("ain_alarm");
		//writer.writeAttribute("Count", QString("%1").arg(nCounts));
		//int nAnalogAlarmOccNo = 0, nAanalogAlarmBlockNo = 0;
		//SaveAnalogAlarm(writer, m_pScadaAnalogAlarmGroup, nAnalogAlarmOccNo, nAanalogAlarmBlockNo);
		//writer.writeEndElement();

		//Q_ASSERT(m_pScadaDigitalAlarmGroup);
		//if (!m_pScadaDigitalAlarmGroup)
		//{
		//	return false;
		//}

		//writer.writeStartElement("din_alarm");
		//nCounts = (int)m_arrMaps[SCADAVARIABLEHASHID::DIGITALALARMID].size();
		//writer.writeAttribute("Count", QString("%1").arg(nCounts));
		//int nDigitalAlarmOccNo = 0, nDigitalAlarmLimitOccNo = 0;
		//SaveDigtalAlarm(writer, m_pScadaDigitalAlarmGroup, nDigitalAlarmOccNo, nDigitalAlarmLimitOccNo);
		//writer.writeEndElement();

		////writer.writeEndElement();
		return true;
	}

	/*! \fn bool CScadaVariableConfigData::SaveAnalogAlarm(QXmlStreamWriter &writer, CScadaAnalogAlarmGroup *pGroup, int &nAanalogAlarmOccNo, int &nAanalogAlarmLimitOccNo)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::SaveAnalogAlarm 
	** \details 保存模拟量告警
	** \param writer 
	** \param pGroup 
	** \param nAanalogAlarmOccNo 告警大排行号
	** \param nAanalogAlarmLimitOccNo 告警限值大排行号
	** \return bool 
	** \author gw
	** \date 2017年1月16日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveAnalogAlarm(QXmlStreamWriter &writer, CScadaAnalogAlarmGroup *pGroup, int &nAanalogAlarmOccNo, int &nAanalogAlarmLimitOccNo)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto &arrGroup = pGroup->GetGroup();
		for each (auto var in arrGroup)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", QString("%1").arg(var->GetGroupName()));

			SaveAnalogAlarm(writer, var, nAanalogAlarmOccNo, nAanalogAlarmLimitOccNo);

			writer.writeEndElement();
		}


		auto &arrItem = pGroup->GetItem();
		Q_ASSERT(nAanalogAlarmOccNo + 1 > 0);
		Q_ASSERT(nAanalogAlarmLimitOccNo + 1 > 0);
		for each (auto var in arrItem)
		{
			writer.writeStartElement("ain_alarm_node");

			nAanalogAlarmOccNo++;
			var->SetOccNo(nAanalogAlarmOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nAanalogAlarmOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			writer.writeAttribute("AlarmType", QString("%1").arg(var->m_nAlarmType));
			writer.writeAttribute("Enable", QString("%1").arg(var->m_bEnable));
			writer.writeAttribute("OnQualityGood", QString("%1").arg(var->m_bOnQualityGood));
			writer.writeAttribute("DeadType", QString("%1").arg(var->m_nDeadType));
			auto tmp = QString::number(var->m_dbDeadArea, 2, 6);
			writer.writeAttribute("DeadArea", tmp);
			writer.writeAttribute("DelayAlarm", QString("%1").arg(var->m_nDelayAlarm));
			writer.writeAttribute("GroupName", QString("%1").arg(var->m_strGroupName));

			auto const &analogOfflimit = var->m_arrAlarmOfflimit;
			writer.writeAttribute("Count", QString("%1").arg(analogOfflimit.size()));
			int nBlockNo = 0;
			for (auto const &tmp : analogOfflimit)
			{
				writer.writeStartElement("aa");

				nAanalogAlarmLimitOccNo++;
				tmp->SetOccNo(nAanalogAlarmLimitOccNo);
				writer.writeAttribute("OccNo", QString("%1").arg(nAanalogAlarmLimitOccNo));
				nBlockNo++;
				tmp->SetBlockNo(nBlockNo);
				writer.writeAttribute("BlockNo", QString("%1").arg(nBlockNo));
				writer.writeAttribute("TagName", QString("%1").arg(tmp->m_szTagName));
				writer.writeAttribute("ID", QString("%1").arg(tmp->m_nID));
				writer.writeAttribute("Description", QString("%1").arg(tmp->m_strDescription));
				//writer.writeAttribute("TimeFormat", QString("%1").arg(tmp->TimeFormat));
				writer.writeAttribute("Condition", QString("%1").arg(tmp->m_nCondition));
				writer.writeAttribute("Category", QString("%1").arg(tmp->m_nCategory));
				writer.writeAttribute("Value", QString("%1").arg(tmp->m_dbValue));
				writer.writeAttribute("LowValue", QString("%1").arg(tmp->m_dbLowValue));
				writer.writeAttribute("DelayAlarm", QString("%1").arg(tmp->m_nDelayAlarm));
				writer.writeAttribute("Priority", QString("%1").arg(tmp->m_nPriority));
				writer.writeAttribute("SupportAck", QString("%1").arg(tmp->m_bSupportAck));
				writer.writeAttribute("SupportDelete", QString("%1").arg(tmp->m_bSupportDelete));
				writer.writeAttribute("AckType", QString("%1").arg(tmp->m_nAckType));
				writer.writeAttribute("SoundFile", QString("%1").arg(tmp->m_strSoundFile));
				writer.writeAttribute("PlaySoundTimes", QString("%1").arg(tmp->m_nPlaySoundTimes));
				writer.writeAttribute("SpeechAlarmText", QString("%1").arg(tmp->m_strSpeechAlarmText));
				writer.writeAttribute("Beep", QString("%1").arg(tmp->m_bBeep));
				writer.writeAttribute("PushGraph", QString("%1").arg(tmp->m_strPushGraph));
				writer.writeAttribute("BlinkGraph", QString("%1").arg(tmp->m_bBlinkGraph));
				writer.writeAttribute("Log", QString("%1").arg(tmp->m_bLog));
				writer.writeAttribute("BackColor", QString("%1").arg(tmp->m_nBackColor));
				writer.writeAttribute("TextColor", QString("%1").arg(tmp->m_nTextColor));
				writer.writeAttribute("BlinkBackColor", QString("%1").arg(tmp->m_nBlinkBackColor));
				writer.writeAttribute("BlinkTextColor", QString("%1").arg(tmp->m_nBlinkTextColor));
				writer.writeAttribute("LightBoard", QString("%1").arg(tmp->m_bLightBoard));
				writer.writeAttribute("DispGuide", QString("%1").arg(tmp->m_strDispGuide));

				writer.writeAttribute("Commands", QString("%1").arg(tmp->m_strCommands));

				writer.writeEndElement();
			}

			writer.writeEndElement();
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::SaveDigtalAlarm(QXmlStreamWriter &writer, CScadaDigitalAlarmGroup *pGroup, int &nDigitalAlarmOccNo, int &nDigitalAlarmLimitOccNo)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::SaveDigtalAlarm 
	** \details 保存开关量报警
	** \param writer 
	** \param pGroup 
	** \param nDigitalAlarmOccNo 
	** \param nDigitalAlarmLimitOccNo 
	** \return bool 
	** \author gw
	** \date 2017年1月16日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveDigtalAlarm(QXmlStreamWriter &writer, CScadaDigitalAlarmGroup *pGroup, int &nDigitalAlarmOccNo, int &nDigitalAlarmLimitOccNo)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto &arrGroup = pGroup->GetGroup();
		for each (auto var in arrGroup)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", QString("%1").arg(var->GetGroupName()));

			SaveDigtalAlarm(writer, var, nDigitalAlarmOccNo, nDigitalAlarmLimitOccNo);

			writer.writeEndElement();
		}



		auto &arrItem = pGroup->GetItem();
		Q_ASSERT(nDigitalAlarmOccNo + 1 > 0);
		Q_ASSERT(nDigitalAlarmLimitOccNo + 1 > 0);
		for each (auto var in arrItem)
		{
			writer.writeStartElement("din_alarm_node");

			nDigitalAlarmOccNo++;

			var->SetOccNo(nDigitalAlarmOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(var->GetOccNo()));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			writer.writeAttribute("AlarmType", QString("%1").arg(var->m_nAlarmType));
			writer.writeAttribute("DelayAlarm", QString("%1").arg(var->m_nDelayAlarm));
			writer.writeAttribute("Enable", QString("%1").arg(var->m_bEnable));
			writer.writeAttribute("OnQualityGood", QString("%1").arg(var->m_bOnQualityGood));
			writer.writeAttribute("GroupName", QString("%1").arg(var->m_strGroupName));

			auto const &digtalOfflimit = var->m_arrDigtallimit;
			writer.writeAttribute("Count", QString("%1").arg(digtalOfflimit.size()));
			int nBolckNo = 0;
			for (auto const &tmp : digtalOfflimit)
			{
				writer.writeStartElement("da");

				nDigitalAlarmLimitOccNo++;
				tmp->SetOccNo(nDigitalAlarmLimitOccNo);
				writer.writeAttribute("OccNo", QString("%1").arg(tmp->GetOccNo()));

				nBolckNo++;
				tmp->BlockNo = nBolckNo;
				writer.writeAttribute("BlockNo", QString("%1").arg(nBolckNo));

				writer.writeAttribute("TagName", QString("%1").arg(tmp->TagName));
				writer.writeAttribute("ID", QString("%1").arg(tmp->ID));
				writer.writeAttribute("Description", QString("%1").arg(tmp->Description));
				writer.writeAttribute("Condition", QString("%1").arg(tmp->Condition));
				writer.writeAttribute("Category", QString("%1").arg(tmp->Category));
				writer.writeAttribute("DelayAlarm", QString("%1").arg(tmp->DelayAlarm));
				writer.writeAttribute("Priority", QString("%1").arg(tmp->Priority));
				writer.writeAttribute("SupportAck", QString("%1").arg(tmp->SupportAck));
				writer.writeAttribute("SupportDelete", QString("%1").arg(tmp->SupportDelete));
				writer.writeAttribute("AckType", QString("%1").arg(tmp->AckType));
				writer.writeAttribute("SoundFile", QString("%1").arg(tmp->SoundFile));
				writer.writeAttribute("PlaySoundTimes", QString("%1").arg(tmp->PlaySoundTimes));
				writer.writeAttribute("SpeechAlarmText", QString("%1").arg(tmp->SpeechAlarmText));
				writer.writeAttribute("Beep", QString("%1").arg(tmp->Beep));
				writer.writeAttribute("PushGraph", QString("%1").arg(tmp->PushGraph));
				writer.writeAttribute("BlinkGraph", QString("%1").arg(tmp->BlinkGraph));
				writer.writeAttribute("Log", QString("%1").arg(tmp->Log));

				writer.writeAttribute("BackColor", QString("%1").arg(tmp->BackColor));
				writer.writeAttribute("TextColor", QString("%1").arg(tmp->TextColor));
				writer.writeAttribute("BlinkBackColor", QString("%1").arg(tmp->BlinkBackColor));
				writer.writeAttribute("BlinkTextColor", QString("%1").arg(tmp->BlinkTextColor));

				writer.writeAttribute("DispGuide", QString("%1").arg(tmp->DispGuide));
				writer.writeAttribute("Commands", QString("%1").arg(tmp->Commands));

				writer.writeEndElement();
			}

			writer.writeEndElement();
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::LoadProjectScadaNode(QDomElement &e, const QString &szRoot)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::LoadProjectScadaNode 
	** \details 加载工程文件scada模块
	** \param e 
	** \param szRoot 
	** \return bool 
	** \author gw
	** \date 2017年3月15日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadProjectScadaNode(QDomElement &e, const QString &szRoot)
	{
		QDomNode n = e.firstChild();

		m_arrHashTagNameMap.clear();

		while (!n.isNull())
		{
			QDomElement e = n.toElement(); // try to convert the node to an element.

			auto strTmp = e.tagName();
			if (strTmp.compare("group") == 0)
			{
				// 文件路径
				QString strGroupName = e.attribute("name");
				if (strGroupName.isEmpty() == false)
				{
					CScadaGroup *pScadaGroup = new CScadaGroup;
					pScadaGroup->m_strGroup = strGroupName;
					LoadChildGroup(pScadaGroup, e, szRoot);

					m_pGroup->m_arrGroup.insert(std::make_pair(strGroupName.toStdString(), pScadaGroup));
					//vec.append(strFile);
				}
				else
				{
					//TODO LOG
				}
			}
			else if (strTmp.compare("file") == 0)
			{
				QString strTagName = e.attribute("name");
				CScadaData *pScada = new CScadaData(s_pGlobleCore);
				memset(pScada->m_szTagName, 0, sizeof(pScada->m_szTagName));
				int nSize = strTagName.size();
				strncpy(pScada->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
				m_pGroup->m_arrItem.insert(std::make_pair(strTagName.toStdString(), pScada));

				if (LoadScadaItem(pScada, pScada->m_szTagName))
				{

				}
			}

			n = n.nextSibling();
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::LoadChildGroup(CScadaGroup *pScadaGroup, QDomElement &e, const QString &szRoot)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::LoadChildGroup 
	** \details 递归加载组
	** \param pScadaGroup 
	** \param e 
	** \param szRoot 
	** \return bool 
	** \author gw
	** \date 2017年3月15日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadChildGroup(CScadaGroup *pScadaGroup, QDomElement &e, const QString &szRoot)
	{
		QDomNode n = e.firstChild();

		while (!n.isNull())
		{
			QDomElement e = n.toElement(); // try to convert the node to an element.

			auto strTmp = e.tagName();
			if (strTmp.compare("group") == 0)
			{
				QString strGroupName = e.attribute("name");
				CScadaGroup *pScadaGroupChild = new CScadaGroup;
				pScadaGroupChild->m_strGroup = strGroupName;

				pScadaGroup->m_arrGroup.insert(std::make_pair(strGroupName.toStdString(), pScadaGroupChild));

				LoadChildGroup(pScadaGroupChild, e, szRoot);
			}
			else if (strTmp.compare("file") == 0)
			{
				QString strTagName = e.attribute("name");
				CScadaData *pScada = new CScadaData(s_pGlobleCore);
				memset(pScada->m_szTagName, 0, sizeof(pScada->m_szTagName));
				int nSize = strTagName.size();
				strncpy(pScada->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
				pScadaGroup->m_arrItem.insert(std::make_pair(strTagName.toStdString(), pScada));

				LoadScadaItem(pScada, pScada->m_szTagName);
			}

			n = n.nextSibling();
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::LoadChildItem(CScadaData *pFes, const QString &szRoot)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::LoadChildItem 
	** \details 加载scadavariable文件
	** \param pFes 
	** \param szRoot 
	** \return bool 
	** \author gw
	** \date 2017年3月15日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadScadaItem(CScadaData *pScada, const QString &filePath)
	{
		QXmlStreamReader xml;
		auto strPath = s_pGlobleCore->GetProjMgr()->GetFile()->fileName();
		auto nIndex = strPath.lastIndexOf("/");
		strPath = strPath.mid(0, nIndex);
		strPath += "/scadavariable/";

		QString strTmp = strPath + "/" + filePath + ".xml";
		QFile file(strTmp);

		if (!file.open(QFile::ReadOnly | QFile::Text))
		{
			auto strError = QString(QObject::tr("load scadavariable file %1 failed!!!")).arg(strTmp);
			s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), 1);

			return false;
		}

		xml.setDevice(&file);

		if (xml.readNextStartElement())
		{
			QString strTmp = xml.name().toString();

			QString strTmp1 = xml.attributes().value("version").toString();

			if (xml.name() == "scada" && xml.attributes().value("version") == "1.0")
			{
				QString strTagName = xml.attributes().value("TagName").toString();

				int nSize = strTagName.size();
				strncpy(pScada->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

				QString strDesc = xml.attributes().value("Description").toString();
				pScada->m_strDescription = strDesc;

				QString strGroup = xml.attributes().value("GroupName").toString();
				pScada->m_strGroupName = strGroup;

				int nID = xml.attributes().value("ID").toInt();
				auto bFlag = CHECK_LESS_ZERO_INT(nID);
				if (!bFlag)
				{
					auto strFesError = QString("ID %1 of fes %2 is invalid < 0").arg(nID).arg(strTagName);
					s_pGlobleCore->LogMsg(FES_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);

					delete pScada;
					pScada = nullptr;

					return false;
				}
				if (strTagName.isEmpty())
				{
					auto strFesError = QString("fes %1 is empty").arg(strTagName);
					s_pGlobleCore->LogMsg(FES_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);

					delete pScada;
					pScada = nullptr;

					return false;
				}

				//fes tagname是否存在
				auto it = m_arrHashTagNameMap.find(pScada->m_szTagName);
				if (it != m_arrHashTagNameMap.end())
				{
					file.close();

					return false;
				}

				pScada->LoadScadaData(xml);

				//前置大hash map也要保存前置的tagname
				pScada->PushTagNameHashMap(SCADAID, pScada->m_szTagName, pScada);

				//push到fes hash map
				m_arrHashTagNameMap.insert(std::make_pair(pScada->m_szTagName, pScada));
			}
		}

		file.close();

		return true;
	}

	/*! \fn bool CScadaVirableConfigData::LoadScadaVariableProjectFile(QString &strFile)
	********************************************************************************************************* 
	** \brief Config::CScadaVirableConfigData::LoadScadaVariableProjectFile 
	** \details 加载scada variable文件
	** \param strFile 
	** \return bool 
	** \author gw
	** \date 2017年1月6日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadScadaVariableProjectFile(QString &strFile)
	{
		//Q_ASSERT(m_pScadaUserGroup);
		//if (!m_pScadaUserGroup)
		//{
		//	return false;
		//}

		//QXmlStreamReader xml;
		//auto strPath = s_pGlobleCore->GetProjMgr()->GetFile()->fileName();
		//auto nIndex = strPath.lastIndexOf("/");
		//strPath = strPath.mid(0, nIndex);
		//strPath += "/scadavariable/";

		//auto strTmp = strPath + "/" + strFile;
		//QFile file(strTmp);

		//if (!file.open(QFile::ReadOnly | QFile::Text))
		//{
		//	auto strError = QString(QObject::tr("load scada variable file failed!!!"));
		//	s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), 1);

		//	return false;
		//}

		//xml.setDevice(&file);

		//while (!xml.atEnd())
		//{
		//	auto strName = xml.name().toString();
		//	
		//	if (xml.hasError())
		//	{
		//		auto strError = QObject::tr("xml error!!!");
		//		s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);
		//		return false;
		//	}
		//	else if (xml.isStartElement()) //遇到开始标签，如<city>  
		//	{
		//		if (xml.name() == "scadasystem")
		//		{
		//			LoadSystemVarialbeNode(xml);

		//			xml.readNext();
		//		}
		//		else if (xml.name() == "scadauser")
		//		{
		//			Q_ASSERT(m_pScadaUserGroup);
		//			if (!m_pScadaUserGroup)
		//			{
		//				xml.readNext();

		//				continue;
		//			}

		//			LoadUserVarialbeNode(xml, m_pScadaUserGroup);

		//			xml.readNext();
		//		}
		//		else if (xml.name() == "ain_alarm")
		//		{
		//			Q_ASSERT(m_pScadaAnalogAlarmGroup);
		//			if (!m_pScadaAnalogAlarmGroup)
		//			{
		//				return false;
		//			}
		//			LoadAINAlarm(xml, m_pScadaAnalogAlarmGroup);
		//		}
		//		else if (xml.name() == "din_alarm")
		//		{
		//			Q_ASSERT(m_pScadaDigitalAlarmGroup);
		//			if (!m_pScadaDigitalAlarmGroup)
		//			{
		//				return false;
		//			}

		//			LoadDINAlarm(xml, m_pScadaDigitalAlarmGroup);
		//			xml.readNext();
		//		}
		//		else if (xml.name() == "linear")
		//		{
		//			Q_ASSERT(m_pScadaLinearGroup);
		//			if (!m_pScadaLinearGroup)
		//			{
		//				return false;
		//			}

		//			LoadLinear(xml, m_pScadaLinearGroup);
		//		}
		//		else if (xml.name() == "nonlinear")
		//		{
		//			Q_ASSERT(m_pScadaNonLinearGroup);
		//			if (!m_pScadaNonLinearGroup)
		//			{
		//				return false;
		//			}

		//			LoadNonlinear(xml, m_pScadaNonLinearGroup);
		//		}
		//		else //遇到不需要的标签，如<fengli>和<shidu> 
		//		{
		//			xml.readNext();
		//		}

		//	}
		//	else //遇到结束标签，<span style="font-family: Arial, Helvetica, sans-serif;">如</city></span>  
		//		xml.readNext();
		//}

		//file.close();

		return true;
	}

	/*! \fn bool CScadaVirableConfigData::LoadSystemVarialbeNode(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CScadaVirableConfigData::LoadSystemVarialbeNode 
	** \details 保存scada variable system variable
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2017年1月6日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadSystemVarialbeNode(QXmlStreamReader &xml)
	{
		while (!xml.atEnd())
		{
			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();
				if (strTmp == "s")
				{
					//auto pSystemVariable = new CScadaSystemVaraible;
					//memset(pSystemVariable->m_szTagName, 0, sizeof(pSystemVariable->m_szTagName));
					//auto strTagName = xml.attributes().value("TagName").toString();
					//auto nSize = strTagName.size();
					//strncpy(pSystemVariable->m_szTagName, strTagName.toStdString().c_str(), qMin(nSize, MAX_NAME_LENGTH_SCADASTUDIO));
				
					//auto nOccNo = xml.attributes().value("OccNo").toInt();
					//Q_ASSERT(nOccNo > 0 && nOccNo < MAX_OCCNO_SCADASTUDIO);
					//pSystemVariable->SetOccNo(nOccNo);
				
					//pSystemVariable->m_nID = xml.attributes().value("ID").toInt();
					//pSystemVariable->m_strDescription = xml.attributes().value("Description").toString();
					//pSystemVariable->m_nType = xml.attributes().value("DataType").toInt();

					/************************************************************************/
					/* 系统变量是程序写死的,所以读取xml文件可以忽略   
					   在Create函数里加载数据
					/************************************************************************/
					//m_arrSystemVariable.push_back(pSystemVariable);

					////Push data to hash table
					//PushTagNameHashMap(SCADAVIRABLEHASHID::SYSTEMID, pSystemVariable->m_szTagName, pSystemVariable);
				}
			}
			else if (xml.isEndElement() && xml.name() == "scadasystem")
			{
				break;
			}

			xml.readNext();
		}

		return true;
	}

	/*! \fn bool CScadaVirableConfigData::LoadUserVarialbeNode(QXmlStreamReader &xml)
	********************************************************************************************************* 
	** \brief Config::CScadaVirableConfigData::LoadUserVarialbeNode 
	** \details 保存scada variable user variable
	** \param xml 
	** \return bool 
	** \author gw
	** \date 2017年1月6日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadUserVarialbeNode(QXmlStreamReader &xml, Config::CScadaUserVariableGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			auto strError = QObject::tr("read user variable error!!!");
			s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		while (!xml.atEnd())
		{

			
			xml.readNext();

			QString strTmp = xml.name().toString();

			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();

				if (strTmp == "u")
				{
					//if (strTmp == "s")
					{


						auto pUser = new CScadaUserVariable();

						pUser->SetOccNo(xml.attributes().value("OccNo").toInt());
						pUser->m_nID = xml.attributes().value("ID").toInt();

						memset(pUser->m_szTagName, 0, sizeof(pUser->m_szTagName));
						QString strTagName = xml.attributes().value("TagName").toString();
						int nSize = strTagName.size();
						strncpy(pUser->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

						pUser->m_strDescription = xml.attributes().value("Description").toString();
						pUser->m_nType = xml.attributes().value("DataType").toInt();
						pUser->m_dbInitValue = xml.attributes().value("InitValue").toDouble();
						pUser->m_nInitialQua = xml.attributes().value("InitialQua").toInt();
						
						pUser->m_strSourceTagName = xml.attributes().value("SourceTagName").toString();

						pUser->m_strScaleTagName = xml.attributes().value("ScaleTagName").toString();
						pUser->m_nScaleType = xml.attributes().value("ScaleType").toInt();
						pUser->m_nScaleOccNo = xml.attributes().value("ScaleOccNo").toInt();
						pUser->m_strAlarmTagName = xml.attributes().value("AlarmTagName").toString();
						pUser->m_nAlarmType = xml.attributes().value("AlarmType").toInt();
						pUser->m_nAlarmOccNo = xml.attributes().value("AlarmOccNo").toInt();
						//if (!CheckTagNameIsExist(strTagName))
						//{
						//	if (!PushTagNameHashMap(SCADAVARIABLEHASHID::USERID, strTagName, pUser))
						//	{
						//		auto strError = QObject::tr("user variable read error!!!");
						//		s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						//		continue;
						//	}

						//	pGroup->AddUserVariableItem(pUser);
						//}
						//else
						//{
						//	auto strError = QObject::tr("user variable read error!!!");
						//	s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						//	continue;
						//}
					}
				}
				else if (strTmp == "group")
				{


					QString strName = xml.attributes().value("name").toString();
					//if (!CheckTagNameIsExist(strName))
					{
						auto *pTmp = new CScadaUserVariableGroup;
						Q_ASSERT(pTmp);
						if (!pTmp)
						{
							auto strError = QObject::tr("read uservariablegroup error!!!");
							s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

							continue;
						}

						//if (!PushTagNameHashMap(SCADAVIRABLEHASHID::USERID, strName, pTmp))
						//{
						//	auto strError = QObject::tr("user variable group name %1 is exist!!!").arg(strName);
						//	s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						//	continue;
						//}

						pTmp->SetGroupName(strName);
						pGroup->AddUserVariableGroup(pTmp);
						LoadUserVarialbeNode(xml, pTmp);
					}
					//else
					//{
					// auto strError = QObject::tr("user variable read error!!!");
					// s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					// continue;
					//}



				}
			}
			else if (xml.isEndElement() && strTmp == "group")
			{
				break;
			}

			//xml.readNext();
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::LoadAINAlarm(QXmlStreamReader &reader, CScadaAnalogAlarmGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::LoadAINAlarm 
	** \details 
	** \param reader 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月16日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadAINAlarm(QXmlStreamReader &reader, CScadaAnalogAlarmGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		using namespace Config;

		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "ain_alarm_node")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					auto bFlag = nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO;
					Q_ASSERT(!bFlag);
					if (bFlag)
					{// LOG
						auto strError = QObject::tr("ain alarm OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("ain alarm Tag name is empty!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaAnalogAlarm *pAlarm = new CScadaAnalogAlarm;
					//模拟量报警
					pAlarm->SetOccNo(nOccNo);
					memset(pAlarm->m_szTagName, 0, sizeof(pAlarm->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pAlarm->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pAlarm->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pAlarm->m_strDescription = strDescription;

					int8u nEnable = reader.attributes().value("Enable").toInt();
					pAlarm->m_bEnable = nEnable;

					int8u nOnQualityGood = reader.attributes().value("OnQualityGood").toUInt();
					pAlarm->m_bOnQualityGood = nOnQualityGood;

					int8u nDeadType = reader.attributes().value("DeadType").toUInt();
					pAlarm->m_nDeadType = nDeadType;

					fp64 dbDeadArea = reader.attributes().value("DeadArea").toDouble();
					pAlarm->m_dbDeadArea = dbDeadArea;

					auto nAlarmType = reader.attributes().value("AlarmType").toInt();
					pAlarm->m_nAlarmType = nAlarmType;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pAlarm->m_strGroupName = strGroupName;

					bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of ai alarm %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString(QObject::tr("ai alarm %1 is empty")).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					else if (strTagName.count() > MAX_NAME_LENGTH_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("ai alarm %1 size is more than %2 ")).arg(strTagName).arg(MAX_NAME_LENGTH_SCADASTUDIO);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					LoadAINAlarmLimit(reader, pAlarm);

					pGroup->AddAnalogItem(pAlarm);
					//PushTagNameHashMap(SCADAVARIABLEHASHID::ANALOGALARMID, strTagName, pAlarm);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CScadaAnalogAlarmGroup *pGroupChild = new Config::CScadaAnalogAlarmGroup;

					pGroupChild->SetGroupName(reader.attributes().value("name").toString());

					pGroup->AddAnalogGroup(pGroupChild);


					//组group的子组group
					reader.readNext();

					LoadAINAlarm(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::LoadAINLimit(QXmlStreamReader &reader, Config::CScadaAnalogAlarm *pAlarm)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::LoadAINLimit 
	** \details 
	** \param reader 
	** \param pAlarm 
	** \return bool 
	** \author gw
	** \date 2017年1月16日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadAINAlarmLimit(QXmlStreamReader &reader, Config::CScadaAnalogAlarm *pAlarm)
	{
		Q_ASSERT(pAlarm);
		if (!pAlarm)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "aa")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ain alarm offlimit OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					int32u nBlockNo = reader.attributes().value("BlockNo").toInt();
					Q_ASSERT(nBlockNo >= MIN_BLOCKNO_SCADASTUDIO && nBlockNo <= MAX_BLOCKNO_SCADASTUDIO);
					if (nBlockNo <= MIN_BLOCKNO_SCADASTUDIO || nBlockNo > MAX_BLOCKNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ain alarm offlimit BlockNo is invalid!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("ain alarm offlimit Tag name is empty!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaAinAlarmLimit *pAlarmOfflimit = new CScadaAinAlarmLimit;
					pAlarmOfflimit->SetOccNo(nOccNo);
					memset(pAlarmOfflimit->m_szTagName, 0, sizeof(pAlarmOfflimit->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pAlarmOfflimit->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

					QString strDescription = reader.attributes().value("Description").toString();
					pAlarmOfflimit->m_strDescription = strDescription;

					int32u nID = reader.attributes().value("ID").toUInt();
					pAlarmOfflimit->m_nID = nID;



					//QString strTimeFormat = reader.attributes().value("TimeFormat").toString();
					//pAlarmOfflimit->TimeFormat = strTimeFormat;

					int8u nCondition = reader.attributes().value("Condition").toUInt();
					pAlarmOfflimit->m_nCondition = nCondition;

					pAlarmOfflimit->m_nCategory = reader.attributes().value("Category").toUInt();

					fp64 dbValue = reader.attributes().value("Value").toDouble();
					pAlarmOfflimit->m_dbValue = dbValue;

					fp64 dbLowValue = reader.attributes().value("LowValue").toDouble();
					pAlarmOfflimit->m_dbLowValue = dbLowValue;

					int32u nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();
					pAlarmOfflimit->m_nDelayAlarm = nDelayAlarm;

					pAlarmOfflimit->m_nPriority = reader.attributes().value("Priority").toUInt();

					pAlarmOfflimit->m_bSupportAck = reader.attributes().value("SupportAck").toUInt();
					pAlarmOfflimit->m_bSupportDelete = reader.attributes().value("SupportDelete").toUInt();

					pAlarmOfflimit->m_nAckType = reader.attributes().value("AckType").toInt();

					QString strSoundFile = reader.attributes().value("SoundFile").toString();
					pAlarmOfflimit->m_strSoundFile = strSoundFile;
					int32u nPlaySoundTimes = reader.attributes().value("PlaySoundTimes").toUInt();
					pAlarmOfflimit->m_nPlaySoundTimes = nPlaySoundTimes;

					pAlarmOfflimit->m_strSpeechAlarmText = reader.attributes().value("SpeechAlarmText").toString();

					pAlarmOfflimit->m_bBeep = reader.attributes().value("Beep").toUInt();

					pAlarmOfflimit->m_strPushGraph = reader.attributes().value("PushGraph").toString();

					pAlarmOfflimit->m_bBlinkGraph = reader.attributes().value("BlinkGraph").toInt();

					pAlarmOfflimit->m_bLog = reader.attributes().value("Log").toInt();

					pAlarmOfflimit->m_nBackColor = reader.attributes().value("BackColor").toInt();
					pAlarmOfflimit->m_nTextColor = reader.attributes().value("TextColor").toInt();
					pAlarmOfflimit->m_nBlinkTextColor = reader.attributes().value("BlinkBackColor").toInt();
					pAlarmOfflimit->m_nBlinkTextColor = reader.attributes().value("BlinkTextColor").toInt();

					pAlarmOfflimit->m_bLightBoard = reader.attributes().value("LightBoard").toInt();
					pAlarmOfflimit->m_strDispGuide = reader.attributes().value("DispGuide").toString();
					pAlarmOfflimit->m_strCommands = reader.attributes().value("Commands").toString();

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of ai alarm limit %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty() || strTagName.count() > MAX_NAME_LENGTH_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("ai alarm limit %1 is empty")).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					else if (strTagName.count() > MAX_NAME_LENGTH_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("ai alarm limit %1 size is more than %2 ")).arg(strTagName).arg(MAX_NAME_LENGTH_SCADASTUDIO);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					pAlarm->m_arrAlarmOfflimit.push_back(pAlarmOfflimit);
					//PushTagNameHashMap(SCADAVARIABLEHASHID::ANALOGALARMLIMITID, strTagName, pAlarmOfflimit);
				}

				reader.readNext();
			}
			else if (reader.isEndElement() && strTmp == "ain_alarm_node")
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::ReadDINAlarm(QXmlStreamReader &reader, CScadaDigitalAlarmGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::ReadDINAlarm 
	** \details 
	** \param reader 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月16日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadDINAlarm(QXmlStreamReader &reader, CScadaDigitalAlarmGroup *pGroup)
	{
		using namespace Config;
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "din_alarm_node")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("di alarm OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("di alarm Tag name is empty!!!");
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaDigitalAlarm *pDigitalAlarm = new CScadaDigitalAlarm;

					//数字量报警
					pDigitalAlarm->SetOccNo(nOccNo);
					memset(pDigitalAlarm->m_szTagName, 0, sizeof(pDigitalAlarm->m_szTagName));
					strncpy(pDigitalAlarm->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, strTagName.size()));

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigitalAlarm->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pDigitalAlarm->m_strDescription = strDescription;

					int8u nEnable = reader.attributes().value("Enable").toInt();
					pDigitalAlarm->m_bEnable = nEnable;

					int8u nOnQualityGood = reader.attributes().value("OnQualityGood").toUInt();
					pDigitalAlarm->m_bOnQualityGood = nOnQualityGood;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pDigitalAlarm->m_strGroupName = strGroupName;

					pDigitalAlarm->m_nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();

					auto nAlarmType = reader.attributes().value("AlarmType").toInt();
					pDigitalAlarm->m_nAlarmType = nAlarmType;

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of di alarm %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString(QObject::tr("di alarm %1 is empty")).arg(strTagName);
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.count() > MAX_NAME_LENGTH_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("di alarm %1 is more than %2")).arg(strTagName);
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					LoadDINAlarmLimit(reader, pDigitalAlarm);

					pGroup->AddDigitalItem(pDigitalAlarm);
					//PushTagNameHashMap(SCADAVARIABLEHASHID::DIGITALALARMID, strTagName, pDigitalAlarm);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CScadaDigitalAlarmGroup *pGroupChild = new Config::CScadaDigitalAlarmGroup;

					pGroupChild->SetGroupName(reader.attributes().value("name").toString());

					pGroup->AddDigitalGroup(pGroupChild);


					//组group的子组group
					reader.readNext();

					LoadDINAlarm(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::LoadDINAlarmLimit(QXmlStreamReader &reader, CScadaDigitalAlarm *pAlarm)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::LoadDINAlarmLimit 
	** \details 
	** \param reader 
	** \param pAlarm 
	** \return bool 
	** \author gw
	** \date 2017年1月16日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadDINAlarmLimit(QXmlStreamReader &reader, CScadaDigitalAlarm *pAlarm)
	{
		Q_ASSERT(pAlarm);
		if (!pAlarm)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "da")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("da OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					int32u nBlockNo = reader.attributes().value("BlockNo").toInt();
					Q_ASSERT(nBlockNo >= MIN_BLOCKNO_SCADASTUDIO && nBlockNo <= MAX_BLOCKNO_SCADASTUDIO);
					if (nBlockNo <= MIN_BLOCKNO_SCADASTUDIO || nBlockNo > MAX_BLOCKNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("da BlockNo is invalid!!!");
						s_pGlobleCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("da Tag name is empty!!!");
						s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaDinAlarmLimit *pDigitalOfflimit = new CScadaDinAlarmLimit;
					pDigitalOfflimit->SetOccNo(nOccNo);
					memset(pDigitalOfflimit->TagName, 0, sizeof(pDigitalOfflimit->TagName));
					int nSize = strTagName.size();
					strncpy(pDigitalOfflimit->TagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigitalOfflimit->ID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pDigitalOfflimit->Description = strDescription;

					int8u nCondition = reader.attributes().value("Condition").toUInt();
					pDigitalOfflimit->Condition = nCondition;

					pDigitalOfflimit->Category = reader.attributes().value("Category").toUInt();

					int32u nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();
					pDigitalOfflimit->DelayAlarm = nDelayAlarm;

					pDigitalOfflimit->Priority = reader.attributes().value("Priority").toUInt();

					int8u nSupportAck = reader.attributes().value("SupportAck").toUInt();
					pDigitalOfflimit->SupportAck = nSupportAck;

					int8u nSupportDelete = reader.attributes().value("SupportDelete").toUInt();
					pDigitalOfflimit->SupportDelete = nSupportDelete;

					pDigitalOfflimit->AckType = reader.attributes().value("AckType").toUInt();

					QString strSoundFile = reader.attributes().value("SoundFile").toString();
					pDigitalOfflimit->SoundFile = strSoundFile;

					int32u nPlaySoundTimes = reader.attributes().value("PlaySoundTimes").toUInt();
					pDigitalOfflimit->PlaySoundTimes = nPlaySoundTimes;

					pDigitalOfflimit->SpeechAlarmText = reader.attributes().value("SpeechAlarmText").toString();

					pDigitalOfflimit->Beep = reader.attributes().value("Beep").toInt();
					pDigitalOfflimit->PushGraph = reader.attributes().value("PushGraph").toString();
					pDigitalOfflimit->BlinkGraph = reader.attributes().value("BlinkGraph").toInt();
					pDigitalOfflimit->Log = reader.attributes().value("Log").toInt();

					pDigitalOfflimit->BackColor = reader.attributes().value("BackColor").toInt();
					pDigitalOfflimit->TextColor = reader.attributes().value("TextColor").toInt();
					pDigitalOfflimit->BlinkBackColor = reader.attributes().value("BlinkBackColor").toInt();
					pDigitalOfflimit->BlinkTextColor = reader.attributes().value("BlinkTextColor").toInt();

					pDigitalOfflimit->DispGuide = reader.attributes().value("DispGuide").toString();

					QString strCommands = reader.attributes().value("Commands").toString();
					pDigitalOfflimit->Commands = strCommands;

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of di alarm limit %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString(QObject::tr("di alarm limit %1 is empty")).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.count() > MAX_NAME_LENGTH_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("di alarm limit %1 is more than %2")).arg(strTagName);
						s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					pAlarm->m_arrDigtallimit.push_back(pDigitalOfflimit);
					//PushTagNameHashMap(SCADAVARIABLEHASHID::DIGITALALARMLIMITID, strTagName, pDigitalOfflimit);
				}

				reader.readNext();
			}
			else if (reader.isEndElement() && strTmp == "din_alarm_node")
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	/*! \fn 
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::LoadLinear 
	** \details 
	** \param reader 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月28日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadLinear(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "l")
				{
					LoadL(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CScadaTransformGroup *pGroupChild = pGroup->Find(strTmp);
					if (!pGroupChild)
					{
						pGroupChild = new Config::CScadaTransformGroup;
						pGroupChild->SetGroupName(reader.attributes().value("name").toString());

						pGroup->AddTransformGroup(pGroupChild);
					}


					//组group的子组group
					reader.readNext();

					LoadLinearGroupNode(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement() && strTmp == "linear")
			{
				break;
			}
		}

		return true;
	}

	bool CScadaVariableConfigData::LoadLinearGroupNode(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();
				if (strTmp == "l")
				{
					LoadL(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					auto strTmp = reader.attributes().value("name").toString();
					auto *pTransformChildGroup = pGroup->Find(strTmp);
					if (!pTransformChildGroup)
					{
						pTransformChildGroup = new CScadaTransformGroup;
						pTransformChildGroup->SetGroupName(reader.attributes().value("name").toString());

						pGroup->AddTransformGroup(pTransformChildGroup);
					}


					//组group的子组group
					reader.readNext();

					LoadLinearGroupNode(reader, pTransformChildGroup);

					continue;
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::LoadL(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::LoadL 
	** \details 
	** \param reader 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月28日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadL(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "l")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("linear OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(LINEAR_SCALE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("linear Tag name is empty!!!");
						s_pGlobleCore->LogMsg(LINEAR_SCALE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaLinearTransform *pLinearTransform = new CScadaLinearTransform();

					//pLinearTransform->SetOccNo(nOccNo);
					memset(pLinearTransform->m_szTagName, 0, sizeof(pLinearTransform->m_szTagName));
					strncpy(pLinearTransform->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, strTagName.size()));

					int32u nID = reader.attributes().value("ID").toUInt();
					pLinearTransform->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pLinearTransform->m_strDescription = strDescription;

					fp64 dbMaxRaw = reader.attributes().value("MaxRaw").toDouble();
					pLinearTransform->m_dbMaxRaw = dbMaxRaw;

					fp64 dbMinRaw = reader.attributes().value("MinRaw").toDouble();
					pLinearTransform->m_dbMinRaw = dbMinRaw;

					fp64 dbMaxScale = reader.attributes().value("MaxScale").toDouble();
					pLinearTransform->m_dbMaxScale = dbMaxScale;

					fp64 dbMinScale = reader.attributes().value("MinScale").toDouble();
					pLinearTransform->m_dbMinScale = dbMinScale;

					int8u nInverse = reader.attributes().value("Inverse").toInt();
					pLinearTransform->m_bEnableInverse = nInverse;

					int8u nEnaleFactor = reader.attributes().value("EnaleFactor").toInt();
					pLinearTransform->m_bEnableFactor = nEnaleFactor;

					fp64 dbGain = reader.attributes().value("Gain").toDouble();
					pLinearTransform->m_dbGain = dbGain;

					fp64 dbOffset = reader.attributes().value("Offset").toDouble();
					pLinearTransform->m_dbOffset = dbOffset;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pLinearTransform->m_strGroupName = strGroupName;

					pLinearTransform->m_nType = reader.attributes().value("ScaleType").toInt();


					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of linear scale %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(LINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString(QObject::tr("linear scale %1 is empty")).arg(strTagName);
						s_pGlobleCore->LogMsg(LINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					else if (strTagName.count() > MAX_NAME_LENGTH_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("linear scale %1 size is more than %2 ")).arg(strTagName).arg(MAX_NAME_LENGTH_SCADASTUDIO);
						s_pGlobleCore->LogMsg(LINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					CScadaTransform *pTmp = new CScadaTransform;
					pTmp->SetOccNo(nOccNo);

					pTmp->m_nType = pLinearTransform->m_nType;
					memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
					auto nSize = strlen(pLinearTransform->m_szTagName);
					strncpy(pTmp->m_szTagName, pLinearTransform->m_szTagName, qMin(MAX_NAME_LENGTH_SCADASTUDIO, (int)nSize));

					pTmp->SetLinear(pLinearTransform);

					//添加到前置转换hash表
					//PushTagNameHashMap(SCADAVARIABLEHASHID::LINEARTRANSFORMID, strTagName, pTmp);
					pGroup->AddTransformItem(pTmp);
				}

				reader.readNext();
			}
			else if (reader.isEndElement())
			{
				break;
			}
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::LoadNonlinear(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::LoadNonlinear 
	** \details 
	** \param reader 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2017年1月28日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::LoadNonlinear(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "nl")
				{
					LoadNl(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					auto strTmp = reader.attributes().value("name").toString();
					CScadaTransformGroup *pChildGroup = pGroup->Find(strTmp);
					if (!pChildGroup)
					{
						pChildGroup = new CScadaTransformGroup;
						pChildGroup->SetGroupName(reader.attributes().value("name").toString());
						pGroup->AddTransformGroup(pChildGroup);
					}


					reader.readNext();

					LoadNonLinearGroupNode(reader, pChildGroup);
				}
			}
			else if (reader.isEndElement() && strTmp == "nonlinear")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	bool CScadaVariableConfigData::LoadNl(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "nl")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("nl OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("Nonlinear Tag name %1 is invalid!!!").arg(strTagName);
						s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CScadaNonlinearTransform *pNonliear = new CScadaNonlinearTransform();

					//pNonliear->SetOccNo(nOccNo);
					memset(pNonliear->m_szTagName, 0, sizeof(pNonliear->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pNonliear->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pNonliear->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pNonliear->m_strDescription = strDescription;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pNonliear->m_strGroupName = strGroupName;

					pNonliear->m_nType = reader.attributes().value("ScaleType").toInt();

					bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of nonlinear scale %2 is invalid < 0")).arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(NONLINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					Loadnl(reader, pNonliear);

					CScadaTransform *pTmp = new CScadaTransform;

					pTmp->m_nType = pNonliear->m_nType;
					memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
					nSize = sizeof(pNonliear->m_szTagName);
					strncpy(pTmp->m_szTagName, pNonliear->m_szTagName, qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
					pTmp->SetOccNo(nOccNo);

					pTmp->SetNonLinear(pNonliear);

					//PushTagNameHashMap(SCADAVARIABLEHASHID::NONLINEARTRANSFORMID, strTagName, pTmp);
					pGroup->AddTransformItem(pTmp);
				}

				strTmp = reader.name().toString();
			}
			else if (reader.isEndElement())
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	bool CScadaVariableConfigData::Loadnl(QXmlStreamReader &reader, Config::CScadaNonlinearTransform *pData)
	{
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "p")
				{
					LoadP(reader, pData);
				}
			}
			else if (reader.isEndElement())
			{
				//reader.skipCurrentElement();

				break;
			}
		}

		return true;
	}

	bool CScadaVariableConfigData::LoadP(QXmlStreamReader &reader, Config::CScadaNonlinearTransform *pData)
	{
		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "p")
				{
					SCADA_NONLINEAR_SCALE *pNonScale = new SCADA_NONLINEAR_SCALE;

					fp64 dbMaxRaw = reader.attributes().value("RawValue").toDouble();
					pNonScale->RawValue = dbMaxRaw;

					fp64 dbMaxScale = reader.attributes().value("ScaleValue").toDouble();
					pNonScale->ScaleValue = dbMaxScale;

					pData->m_arrPNonliear.push_back(pNonScale);
				}

				reader.readNext();
			}
			else if (reader.isEndElement())
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	bool CScadaVariableConfigData::LoadNonLinearGroupNode(QXmlStreamReader &reader, CScadaTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();
				if (strTmp == "nl")
				{
					LoadNl(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					auto strTmp = reader.attributes().value("name").toString();
					auto *pTransformChildGroup = pGroup->Find(strTmp);
					if (!pTransformChildGroup)
					{
						pTransformChildGroup = new CScadaTransformGroup;
						pTransformChildGroup->SetGroupName(reader.attributes().value("name").toString());

						pGroup->AddTransformGroup(pTransformChildGroup);
					}

					//组group的子组group
					reader.readNext();

					LoadNonLinearGroupNode(reader, pTransformChildGroup);

					continue;
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::SaveTransform(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::SaveTransform 
	** \details 
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2017年1月23日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveTransform(QXmlStreamWriter &writer)
	{
		writer.writeStartElement("scale");

		SaveLinear(writer);

		SaveNonlinear(writer);

		writer.writeEndElement();

		return true;
	}

	/*! \fn bool CScadaVariableConfigData::SaveLinear(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::SaveLinear 
	** \details 
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2017年1月23日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveLinear(QXmlStreamWriter &writer)
	{
		//Q_ASSERT(m_pScadaLinearGroup);
		//if (!m_pScadaLinearGroup)
		//{
		//	return false;
		//}

		//writer.writeStartElement("linear");
		//auto nCount = m_pScadaLinearGroup->GetItemCount();
		//writer.writeAttribute("Count", QString("%1").arg(nCount));

		//int nOccNo = 0;
		//SaveLinearTransformChildNode(writer, m_pScadaLinearGroup, nOccNo);

		//writer.writeEndElement();

		return true;
	}

	bool CScadaVariableConfigData::SaveLinearTransformChildNode(QXmlStreamWriter &writer, Config::CScadaTransformGroup *pGroup, int &nOccNo)
	{
		auto &arr = pGroup->GetGroup();
		for each (auto var in arr)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", var->GetGroupName());

			SaveLinearTransformChildNode(writer, var, nOccNo);

			writer.writeEndElement();
		}


		auto &arrLinear = pGroup->GetItem();
		for (auto const &linear : arrLinear)
		{
			Q_ASSERT(linear);
			auto pTmp = linear->GetLinear();
			Q_ASSERT(pTmp);

			writer.writeStartElement("l");

			nOccNo++;
			linear->SetOccNo(nOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(pTmp->m_szTagName));
			writer.writeAttribute("ID", QString("%1").arg(pTmp->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(pTmp->m_strDescription));
			writer.writeAttribute("ScaleType", QString("%1").arg(pTmp->m_nType));
			writer.writeAttribute("MaxRaw", QString::number(pTmp->m_dbMaxRaw, 10, 6));
			writer.writeAttribute("MinRaw", QString::number(pTmp->m_dbMinRaw, 10, 6));
			writer.writeAttribute("MaxScale", QString::number(pTmp->m_dbMaxScale, 10, 6));
			writer.writeAttribute("MinScale", QString::number(pTmp->m_dbMinScale, 10, 6));
			writer.writeAttribute("Inverse", QString("%1").arg(pTmp->m_bEnableInverse));;
			writer.writeAttribute("EnaleFactor", QString("%1").arg(pTmp->m_bEnableFactor));
			writer.writeAttribute("Gain", QString("%1").arg(pTmp->m_dbGain));
			writer.writeAttribute("Offset", QString("%1").arg(pTmp->m_dbOffset));
			writer.writeAttribute("GroupName", QString("%1").arg(pTmp->m_strGroupName));

			writer.writeEndElement();
		}


		return true;
	}

	//************************************
	// Method:    SaveNonlinear
	// FullName:  Config::CScadaVariableConfigData::SaveNonlinear
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: QXmlStreamWriter & writer
	//************************************
	bool CScadaVariableConfigData::SaveNonlinear(QXmlStreamWriter &writer)
	{
		//Q_ASSERT(m_pScadaNonLinearGroup);
		//if (!m_pScadaNonLinearGroup)
		//{
		//	return false;
		//}

		//writer.writeStartElement("nonlinear");

		//auto nCount = m_pScadaNonLinearGroup->GetItemCount();
		//writer.writeAttribute("Count", QString("%1").arg(nCount));

		//int nOccNo = 0;
		//SaveNonLinearTransformChildNode(writer, m_pScadaNonLinearGroup, nOccNo);

		//writer.writeEndElement();

		return true;
	}

	/*! \fn 
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::SaveNonLinearTransformChildNode 
	** \details 
	** \param writer 
	** \param pGroup 
	** \param nOccNo 
	** \return bool 
	** \author gw
	** \date 2017年1月28日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::SaveNonLinearTransformChildNode(QXmlStreamWriter &writer, Config::CScadaTransformGroup *pGroup, int &nOccNo)
	{
		auto &arr = pGroup->GetGroup();
		for each (auto var in arr)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", var->GetGroupName());

			SaveNonLinearTransformChildNode(writer, var, nOccNo);

			writer.writeEndElement();
		}

		auto &arrNonLinear = pGroup->GetItem();
		for (auto const &nonlinear : arrNonLinear)
		{
			Q_ASSERT(nonlinear);
			auto pTmp = nonlinear->GetNonLinear();
			Q_ASSERT(pTmp);
			auto bFlag = QString("%1").arg(pTmp->m_szTagName).isEmpty();
			Q_ASSERT(!bFlag);

			writer.writeStartElement("nl");

			nOccNo++;
			nonlinear->SetOccNo(nOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(pTmp->m_szTagName));
			writer.writeAttribute("ID", QString("%1").arg(pTmp->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(pTmp->m_strDescription));
			writer.writeAttribute("ScaleType", QString("%1").arg(nonlinear->m_nType));
			writer.writeAttribute("Count", QString("%1").arg(pTmp->m_arrPNonliear.size()));
			writer.writeAttribute("GroupName", QString("%1").arg(pTmp->m_strGroupName));


			for (auto const &p : pTmp->m_arrPNonliear)
			{
				writer.writeStartElement("p");

				writer.writeAttribute("RawValue", QString("%1").arg(p->RawValue));
				writer.writeAttribute("ScaleValue", QString("%1").arg(p->ScaleValue));

				writer.writeEndElement();
			}


			writer.writeEndElement();
		}


		return true;
	}

	/*! \fn CBaseData *CScadaVariableConfigData::GetData(QString &strTagName, int nType)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::GetData 
	** \details 根据tagname得到数据
	** \param strTagName 
	** \param nType 
	** \return CBaseData * 
	** \author gw
	** \date 2017年2月3日 
	** \note 
	********************************************************************************************************/
	CBaseData *CScadaVariableConfigData::GetData(QString &strTagName, int nType)
	{
		Q_ASSERT(!strTagName.isEmpty());
		if (strTagName.isEmpty())
		{
			return nullptr;
		}

		auto bFlag = strTagName.size() > 0 && strTagName.size() < MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return nullptr;
		}

		bFlag = nType > 0 && nType < SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM;
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return nullptr;
		}

		auto it = m_arrMaps[nType].find(strTagName.toStdString());
		if (it != m_arrMaps[nType].end())
		{
			return it->second;
		}

		return nullptr;
	}

	/*! \fn bool CScadaVariableConfigData::ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType)
	********************************************************************************************************* 
	** \brief Config::CScadaVariableConfigData::ChangeTagNameNodify 
	** \details 前置DEVICE_AI DEVICE_DI DEVICE_AO DEVICE_DO 点tagname修改,通知绑定点的模块做出相应的修改 删除tagname为空 
	** \param tagName 
	** \param lastTagName 
	** \param nType 
	** \return bool 
	** \author gw
	** \date 2017年5月24日 
	** \note 
	********************************************************************************************************/
	bool CScadaVariableConfigData::ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType)
	{
		m_arrHashTagNameMap;

		for each (auto var in m_arrHashTagNameMap)
		{
			var.second->ChangeTagNameNodify(tagName, lastTagName, nType);
		}

		return true;
	}

}

