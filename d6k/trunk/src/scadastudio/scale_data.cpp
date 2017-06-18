/*! @file
<PRE>
********************************************************************************
ģ����       :  ���ýṹ
�ļ���       :  scale_data.cpp
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
*  @date	2016.1.4
*/

#include "scale_data.h"
#include "scadastudiodefine.h"
#include "stl_util-inl.h"

#include <QObject>

namespace Config
{
	/*! \fn CLinearTransform::CLinearTransform()
	*********************************************************************************************************
	** \brief CLinearTransform::CLinearTransform
	** \details
	** \return
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	CLinearTransform::CLinearTransform()
	{
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_nType = LINEAR;
		m_strDescription = "";
		m_nID = 0;
		m_bEnableFactor = true;
		m_bInverseFactor = false;
		m_dbMaxRaw = 1;
		m_dbMinRaw = 1;
		m_dbMaxScale = 1;
		m_dbMinScale = 1;
		m_dbGain = 0;
		m_dbOffset = 0;
		m_strExtention = "";
		m_strGroupName = "";
		m_bEnableInverse = false;
		//m_nOccNo = 0;
	}

	/*! \fn CLinearTransform::~CLinearTransform()
	*********************************************************************************************************
	** \brief CLinearTransform::~CLinearTransform
	** \details
	** \return
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	CLinearTransform::~CLinearTransform()
	{

	}

	/*! \fn void CLinearTransform::Save()
	*********************************************************************************************************
	** \brief CLinearTransform::Save
	** \details
	** \return void
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	void CLinearTransform::Save()
	{

	}

	/*! \fn CNonlinearTransform::CNonlinearTransform()
	*********************************************************************************************************
	** \brief CNonlinearTransform::CNonlinearTransform
	** \details
	** \return
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	CNonlinearTransform::CNonlinearTransform()
	{
		m_nOccNo = 0;
	}

	/*! \fn CNonlinearTransform::~CNonlinearTransform()
	*********************************************************************************************************
	** \brief CNonlinearTransform::~CNonlinearTransform
	** \details
	** \return
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	CNonlinearTransform::~CNonlinearTransform()
	{
		Clear();
	}

	/*! \fn void CNonlinearTransform::Save()
	*********************************************************************************************************
	** \brief CNonlinearTransform::Save
	** \details
	** \return void
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	void CNonlinearTransform::Save()
	{

	}

	void CNonlinearTransform::Clear()
	{
		for (size_t i = 0; i < m_arrPNonliear.size(); i++)
		{
			delete m_arrPNonliear[i];
		}
		m_arrPNonliear.clear();
	}


	/*! \fn CTransforGroup::CTransforGroup()
	********************************************************************************************************* 
	** \brief Config::CTransforGroup::CTransforGroup 
	** \details ת����
	** \return  
	** \author gw
	** \date 2016��10��31�� 
	** \note 
	********************************************************************************************************/
	CTransformGroup::CTransformGroup()
		:CBaseData()
	{

	}

	CTransformGroup::~CTransformGroup()
	{
		STLDeleteElements(&m_arrTransformGroup);
		STLDeleteElements(&m_arrLinearTransformItem);
	}

	/*! \fn bool CTransformGroup::AddTransformGroup()
	********************************************************************************************************* 
	** \brief Config::CTransformGroup::AddTransformGroup 
	** \details push back transform ��ӵ�vector
	** \return bool 
	** \author gw
	** \date 2016��10��31�� 
	** \note AddTransformGroup��CreateGroup����ͬʱʹ��
	********************************************************************************************************/
	bool CTransformGroup::AddTransformGroup(CTransformGroup *pGroup)
	{
		m_arrTransformGroup.push_back(pGroup);

		return true;
	}


	/*! \fn bool CTransformGroup::AddTransformItem(CTransform *pItem)
	********************************************************************************************************* 
	** \brief Config::CTransformGroup::AddTransformItem 
	** \details �������ת����
	** \param pItem 
	** \return bool 
	** \author gw
	** \date 2016��10��31�� 
	** \note 
	********************************************************************************************************/
	bool CTransformGroup::AddTransformItem(CTransform *pItem)
	{
		m_arrLinearTransformItem.push_back(pItem);

		return true;
	}

	/*! \fn bool CTransformGroup::CreateGroup()
	********************************************************************************************************* 
	** \brief Config::CTransformGroup::CreateGroup 
	** \details ������
	** \return bool 
	** \author gw
	** \date 2016��10��31�� 
	** \note 
	********************************************************************************************************/
	CTransformGroup *CTransformGroup::CreateGroup()
	{
		int nIndex = (int)m_arrTransformGroup.size();
		QString strName = "";
		while (true)
		{
			strName = QString("%1%2").arg(GROUP_NAME).arg(nIndex);

			auto it = Find(strName);
			if (it == nullptr)
			{
				CTransformGroup *pGroup = new CTransformGroup;
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

	/*! \fn CTransformGroup *CTransformGroup::Find(QString &strName)
	********************************************************************************************************* 
	** \brief Config::CTransformGroup::Find 
	** \details �������ֲ�����
	** \param strName 
	** \return CTransformGroup * 
	** \author gw
	** \date 2017��1��17�� 
	** \note 
	********************************************************************************************************/
	CTransformGroup *CTransformGroup::Find(QString &strName)
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

	/*! \fn int32u CTransformGroup::GetItemCount()
	********************************************************************************************************* 
	** \brief Config::CTransformGroup::GetItemCount 
	** \details ���µ�item����
	** \return int32u 
	** \author gw
	** \date 2016��11��1�� 
	** \note 
	********************************************************************************************************/
	int32u CTransformGroup::GetItemCount()
	{
		auto nCount = m_arrLinearTransformItem.size();
		for each (auto var in m_arrTransformGroup)
		{
			nCount += var->GetItemCount();
		}

		return (int32u)nCount;
	}


	/*! \fn bool CTransformGroup::CheckModifyGroupNameExist(CTransformGroup *pGroup, QString strGroupName)
	********************************************************************************************************* 
	** \brief Config::CTransformGroup::CheckModifyGroupNameExist 
	** \details ���parent���޸ĵ��������Ƿ����
	** \param pGroup 
	** \param strGroupName 
	** \return bool 
	** \author gw
	** \date 2016��11��7�� 
	** \note 
	********************************************************************************************************/
	bool CTransformGroup::CheckModifyGroupNameExist(CTransformGroup *pGroup, QString strGroupName)
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

	/*! \fn bool CTransformGroup::DeletGroup(CTransformGroup *pChildGroup)
	********************************************************************************************************* 
	** \brief Config::CTransformGroup::DeletGroup 
	** \details ɾ����
	** \param pChildGroup 
	** \return bool 
	** \author gw
	** \date 2016��11��8�� 
	** \note 
	********************************************************************************************************/
	bool CTransformGroup::DeletGroup(CTransformGroup *pChildGroup)
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


	/*! \fn bool CTransformGroup::DeleteItem(CTransform *pChildItem)
	********************************************************************************************************* 
	** \brief Config::CTransformGroup::DeleteItem 
	** \details ɾ����
	** \param pChildItem 
	** \return bool 
	** \author gw
	** \date 2016��11��8�� 
	** \note 
	********************************************************************************************************/
	bool CTransformGroup::DeleteItem(CTransform *pChildItem)
	{
		auto it = std::find(m_arrLinearTransformItem.begin(), m_arrLinearTransformItem.end(), pChildItem);
		if (it != m_arrLinearTransformItem.end())
		{
			delete *it;

			m_arrLinearTransformItem.erase(it);

			return true;
		}

		return false;
	}
}



