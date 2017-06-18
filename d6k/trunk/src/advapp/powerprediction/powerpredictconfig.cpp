#include "powerpredictconfig.h"

#include "scadastudio/icore.h"
#include "powerpredictdefine.h"

#include <QObject>

extern IMainModuleInterface *s_pGlobleCore;

namespace PPConfig
{

	//IMainModuleInterface *s_pGlobleCore = nullptr;  //临时用


	CPowerPredictGroup::CPowerPredictGroup()
	{


	}

	CPowerPredictGroup::~CPowerPredictGroup()
	{



	}


	/*! \fn CPowerPredictGroup *CPowerPredictGroup::CreateGroup()
	***************************************************************************************************************************************
	** \brief CPowerPredictGroup::CreateGroup
	** \details  创建预测组
	** \param 
	** \return  CPowerPredictGroup *
	** \author ghj
	** \date 2016年11月24日
	** \note
	***************************************************************************************************************************************/
	CPowerPredictGroup *CPowerPredictGroup::CreateGroup()
	{
		int nIndex = m_arrPowerPredictGroup.size();
		QString strName = "";

		while (true)
		{
			strName = QString("%1%2").arg(GROUP_NAME).arg(nIndex);

			auto it = m_arrPowerPredictGroup.find(strName.toStdString());
			if ( it == m_arrPowerPredictGroup.end())
			{
				auto pGroup = new CPowerPredictGroup;

				pGroup->m_strGroup = strName;

				m_arrPowerPredictGroup.insert(std::make_pair(strName.toStdString(), pGroup));

				return pGroup;
			} 
			else
			{
				nIndex++;
			}
		}

		return nullptr;
	}


	bool CPowerPredictGroup::ModifyGroup(const QString &strTagName, const QString & strLastTagName)
	{

		return true;
	}

	/*! \fn bool CPowerPredictGroup::DeleteGroup(CPowerPredictGroup *pPowerPredictGroup, const QString &strTagGroup)
	***************************************************************************************************************************************
	** \brief  CPowerPredictGroup::DeleteGroup
	** \details  删除预测组
	** \param   pPowerPredictGroup
	** \param   strTagGroup
	** \return  bool
	** \author ghj
	** \date 2016年11月24日
	** \note
	***************************************************************************************************************************************/
	bool CPowerPredictGroup::DeleteGroup(CPowerPredictGroup *pPowerPredictGroup, const QString &strTagGroup)
	{

		return true;

	}

	/*! \fn bool CPowerPredictGroup::DeleteItem(CPowerPredictData *pPowerPredict, const QString &strTagGroup)
	***************************************************************************************************************************************
	** \brief CPowerPredictGroup::DeleteItem
	** \details  删除预测项
	** \param   pPowerPredict
	** \param   strTagGroup
	** \return  bool
	** \author ghj
	** \date 2016年11月24日
	** \note
	***************************************************************************************************************************************/
	bool CPowerPredictGroup::DeleteItem(CPowerPredictData *pPowerPredict, const QString &strTagGroup)
	{
		auto it = m_arrPowerPredictItem.find(strTagGroup.toStdString());
		if (it == m_arrPowerPredictItem.end())
		{
			return false;
		}

		delete it->second;

		m_arrPowerPredictItem.erase(it);

		return true;
	}




	CPowerPredictData::CPowerPredictData(IMainModuleInterface *pCore)
		:m_pCore(pCore)
	{
		

	}

	CPowerPredictData::~CPowerPredictData()
	{


	}

	/*! \fn bool CPowerPredictData::PushPowerPredictTagNameHashMap(int nIDD, const QString &strTagName, CBaseData *pData)
	***************************************************************************************************************************************
	** \brief  CPowerPredictData::PushPowerPredictTagNameHashMap
	** \details  powerpredict hash
	** \param   nIDD
	** \param   strTagName
	** \param   pData
	** \return  bool
	** \author ghj
	** \date 2016年11月24日
	** \note
	***************************************************************************************************************************************/
	bool CPowerPredictData::PushPowerPredictTagNameHashMap(int nIDD, const QString &strTagName, CBaseData *pData)
	{
		auto bFlag = nIDD < 0 || nIDD > POWERPREDICTHASHTYPENUM::MAX_IDD_NUM;
		Q_ASSERT(!bFlag);
		if (nIDD < 0 || nIDD > POWERPREDICTHASHTYPENUM::MAX_IDD_NUM)
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



	void CPowerPredictData::Save()
	{

	}






	/*! \fn CBaseData::CBaseData()
	*********************************************************************************************************
	** \brief CBaseData::CBaseData
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CBaseData::CBaseData()
		:m_bModifyFlag(false)
	{

	}

	/*! \fn CBaseData::~CBaseData()
	*********************************************************************************************************
	** \brief CBaseData::~CBaseData
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CBaseData::~CBaseData()
	{

	}







	/*CPowerPredictConfigData::CPowerPredictConfigData()
		:m_pPowerPredictGroup(nullptr)
	{
		m_pPowerPredictGroup = new CPowerPredictGroup;

	}

	bool CPowerPredictConfigData::CheckModifyPowerPredictTagNameExist(const QString &strTagName)
	{
		auto it = m_arrHashTagNameMap.find(strTagName.toStdString());
		
		if (it != m_arrHashTagNameMap.end())
		{
			return true;
		}

		return false;
	}*/


	/*! \fn CPowerPredictData *CPowerPredictConfigData::AddNewPowerPredictData(CPowerPredictGroup *pPowerPredictGroup)
	***************************************************************************************************************************************
	** \brief CPowerPredictConfigData::AddNewPowerPredictData
	** \details  添加预测项数据
	** \param   pPowerPredictGroup
	** \return  CPowerPredictData *
	** \author ghj
	** \date 2016年11月24日
	** \note
	***************************************************************************************************************************************/
	/*CPowerPredictData *CPowerPredictConfigData::AddNewPowerPredictData(CPowerPredictGroup *pPowerPredictGroup)
	{
		Q_ASSERT(s_pGlobleCore);
		if (!s_pGlobleCore)
		{
			return nullptr;
		}

		CPowerPredictData *pData = new CPowerPredictData(s_pGlobleCore);
		Q_ASSERT(pData);
		if (!pData)
		{
			return nullptr;
		}

		auto index = m_arrHashTagNameMap.size();
		while (true)
		{
			QString strPowerPredict = QString("powerprediction%1").arg(index);
			QString strDesc = QString(QObject::tr("powerprediction%1")).arg(index);

			std::string str = strPowerPredict.toStdString();

			std::unordered_map<std::string, CPowerPredictData *>::const_iterator it = m_arrHashTagNameMap.find(str);
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
			pData->m_strDescription = strDesc;

			memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
			int nSize = strPowerPredict.size();
			strncpy(pData->m_szTagName, strPowerPredict.toStdString().c_str(), qMin(MAX_NAME_LENGTH, nSize));

			pPowerPredictGroup->m_arrPowerPredictItem.insert(std::make_pair(str, pData));
			break;
		}

		QString str = QString("%1").arg(pData->m_szTagName);
		m_arrPowerPredictDatas.push_back(pData);
		pData->PushPowerPredictTagNameHashMap(POWERPREDICTHASHID, str, pData);

		return pData;
	}*/


	/*! \fn bool CPowerPredictConfigData::DeleteHashPowerPredictItem(std::string &szPowerPredictTag)
	***************************************************************************************************************************************
	** \brief  CPowerPredictConfigData::DeleteHashPowerPredictItem
	** \details  添加预测项数据
	** \param   szPowerPredictTag
	** \return  bool
	** \author ghj
	** \date 2016年11月24日
	** \note
	***************************************************************************************************************************************/
	/*bool CPowerPredictConfigData::DeleteHashPowerPredictItem(std::string &szPowerPredictTag)
	{
		auto it = m_arrHashTagNameMap.find(szPowerPredictTag);
		if (it != m_arrHashTagNameMap.end())
		{
			m_arrHashTagNameMap.erase(it);

			return true;
		}

		return false;
	}*/



};





/*
CDiInput::CDiInput()
{

}

CDiInput::~CDiInput()
{
	
}


CAiInput::CAiInput()
{

}


CAiInput::~CAiInput()
{

}



CPredictDevice::CPredictDevice(IMainModuleInterface *pCore)
{


}


CPredictDevice::~CPredictDevice()
{


}*/