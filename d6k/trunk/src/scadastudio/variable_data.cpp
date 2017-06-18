#include "variable_data.h"
#include "stl_util-inl.h"
#include "variant.h"

namespace Config
{
	CUserVariable::CUserVariable()
	{
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_strDescription = "";
		m_nID = 0;
		m_nType = DATA_TYPE::DT_DOUBLE;
		
		m_dbInitValue = 0;
		m_nInitialQua = InitialQua::NormalQua;
		m_strAddress = "";
		m_strPinLabel = "";

		m_nOccNo = 0;

		m_strSourceTagName = "";
		m_strSourceOccNo = "";

		m_strFesTagName = "";
		m_strDataTagName = "";
		m_strAttrTagName = "";

		m_strScaleTagName = "";
		//m_strScaleDesc = "";
		m_nScaleOccNo = 0;
		m_nScaleType = ScaleType::SCALETYPENULL;

		m_strAlarmTagName = "";
		//m_strAlarmDesc = "";
		m_nAlarmOccNo = 0;
		m_nAlarmType = AlarmType::ALARMTYPENULL;

		m_bNewScalePoint = true;
	}
	
	/*! \fn CSystemVariable::CSystemVariable()
	*********************************************************************************************************
	** \brief Config::CSystemVariable::CSystemVariable
	** \details 系统变量
	** \return
	** \author gw
	** \date 2016年10月9日
	** \note
	********************************************************************************************************/
	CSystemVariable::CSystemVariable()
	{
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_nID = 0;
		m_strDescription = "";
		m_nType = 0;
		m_nOccNo = 0;
	}

	CSystemVariable::~CSystemVariable()
	{
		
	}


	CUserVariableGroup::CUserVariableGroup()
	{

	}

	CUserVariableGroup::~CUserVariableGroup()
	{
		STLDeleteElements(&m_arrGroup);
		m_arrGroup.clear();
		STLDeleteElements(&m_arrItem);
		m_arrItem.clear();
	}


	/*! \fn CUserVariableGroup *CUserVariableGroup::CreateGroup()
	********************************************************************************************************* 
	** \brief Config::CUserVariableGroup::CreateGroup 
	** \details 
	** \return CUserVariableGroup * 
	** \author gw
	** \date 2016年11月29日 
	** \note 
	********************************************************************************************************/
	CUserVariableGroup *CUserVariableGroup::CreateGroup()
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

			auto pGroup = new CUserVariableGroup;
			pGroup->SetGroupName(strName);

			m_arrGroup.push_back(pGroup);

			return pGroup;
		}

		return nullptr;
	}


	/*! \fn bool CUserVariableGroup::CheckGroupNameExist(QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CUserVariableGroup::CheckGroupNameExist 
	** \details 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016年11月29日 
	** \note 
	********************************************************************************************************/
	bool CUserVariableGroup::CheckGroupNameExist(QString &strTagName)
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


	/*! \fn bool CUserVariableGroup::DeleteGroup(CUserVariableGroup *pChildGroup)
	********************************************************************************************************* 
	** \brief Config::CUserVariableGroup::DeleteGroup 
	** \details 删除组
	** \param pChildGroup 
	** \return bool 
	** \author gw
	** \date 2016年12月1日 
	** \note 
	********************************************************************************************************/
	bool CUserVariableGroup::DeleteGroup(CUserVariableGroup *pChildGroup)
	{
		auto it = std::find(m_arrGroup.begin(), m_arrGroup.end(), pChildGroup);
		if (it == m_arrGroup.end())
		{
			return false;
		}

		m_arrGroup.erase(it);
		delete pChildGroup;
		pChildGroup = nullptr;

		return true;
	}

	/*! \fn bool CUserVariableGroup::AddUserVariableGroup(CUserVariableGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CUserVariableGroup::AddUserVariableGroup 
	** \details 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016年11月29日 
	** \note 
	********************************************************************************************************/
	bool CUserVariableGroup::AddUserVariableGroup(CUserVariableGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		m_arrGroup.push_back(pGroup);
		return true;
	}

	/*! \fn bool CUserVariableGroup::AddUserVariableItem(CUserVariable *pItem)
	********************************************************************************************************* 
	** \brief Config::CUserVariableGroup::AddUserVariableItem 
	** \details 
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2016年11月29日 
	** \note 
	********************************************************************************************************/
	bool CUserVariableGroup::AddUserVariableItem(CUserVariable *pItem)
	{
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return false;
		}

		m_arrItem.push_back(pItem);
		return true;
	}
}


