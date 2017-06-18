/*! @file
<PRE>
********************************************************************************
模块名       :	前置选点模块model
文件名       :	fesselectpointmodel.cpp
文件实现功能 :  表格model
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
*  @date    2016.9.29
*/

#include "fesselectpointmodel.h"
#include "scadastudiodefine.h"
#include "config_data.h"

CFesSelectPointModel::CFesSelectPointModel(QObject *parent, Config::CDevice *pDevice, int nType)
	: QAbstractTableModel(parent), m_pDevice(nullptr), m_pUserContainer(nullptr), m_pFes(nullptr)
{
	Q_ASSERT(pDevice);
	if (!pDevice)
	{
		return;
	}
	
	auto strTmp = tr("ID");
	m_horizontal_header_list.append(strTmp);

	strTmp = tr("TagName");
	m_horizontal_header_list.append(strTmp);

	strTmp = tr("Description");
	m_horizontal_header_list.append(strTmp);

	strTmp = tr("OccNo");
	m_horizontal_header_list.append(strTmp);

	m_nType = nType;

	m_pDevice = pDevice;
}

/*! \fn CFesSelectPointModel::CFesSelectPointModel(QObject *parent, Config::CUserVariableContainer *pUser, int nType)
********************************************************************************************************* 
** \brief CFesSelectPointModel::CFesSelectPointModel 
** \details 用户变量组
** \param parent 
** \param pUser 
** \param nType 
** \return  
** \author gw
** \date 2016年10月12日 
** \note 
********************************************************************************************************/
CFesSelectPointModel::CFesSelectPointModel(QObject *parent, Config::CUserVariableGroup *pUser, int nType)
	:QAbstractTableModel(parent), m_pDevice(nullptr), m_pUserContainer(nullptr), m_pFes(nullptr)
{
	Q_ASSERT(pUser);
	if (!pUser)
	{
		return;
	}

	m_nType = nType;
	m_pUserContainer = pUser;

	auto strTmp = tr("ID");
	m_horizontal_header_list.append(strTmp);

	strTmp = tr("TagName");
	m_horizontal_header_list.append(strTmp);

	strTmp = tr("Description");
	m_horizontal_header_list.append(strTmp);

	strTmp = tr("OccNo");
	m_horizontal_header_list.append(strTmp);
}

/*! \fn CFesSelectPointModel::CFesSelectPointModel(QObject *parent, Config::CFesData *pFes, int nType)
********************************************************************************************************* 
** \brief CFesSelectPointModel::CFesSelectPointModel 
** \details 前置系统变量
** \param parent 
** \param pFes 
** \param nType 
** \return  
** \author gw
** \date 2016年10月12日 
** \note 
********************************************************************************************************/
CFesSelectPointModel::CFesSelectPointModel(QObject *parent, Config::CFesData *pFes, int nType)
	:QAbstractTableModel(parent), m_pDevice(nullptr), m_pUserContainer(nullptr), m_pFes(nullptr)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}

	m_nType = nType;
	m_pFes = pFes;

	auto strTmp = tr("ID");
	m_horizontal_header_list.append(strTmp);

	strTmp = tr("TagName");
	m_horizontal_header_list.append(strTmp);

	strTmp = tr("Description");
	m_horizontal_header_list.append(strTmp);

	strTmp = tr("OccNo");
	m_horizontal_header_list.append(strTmp);
}

CFesSelectPointModel::~CFesSelectPointModel()
{

}

int CFesSelectPointModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();
	if (m_nType == FES_DEVICE_AI)
	{
		return (int)m_pDevice->m_arrAIs.size();
	}
	else if (m_nType == FES_DEVICE_DI)
	{
		return (int)m_pDevice->m_arrDIs.size();
	}
	else if (m_nType == FES_DEVICE_AO)
	{
		return (int)m_pDevice->m_arrAOs.size();
	}
	else if (m_nType == FES_DEVICE_DO)
	{
		return (int)m_pDevice->m_arrDOs.size();
	}
	else if (m_nType == FES_USER_VARIBALE_GROUP_ITEM)
	{
		return (int)m_pUserContainer->m_arrItem.size();
	}
	else if (m_nType == FES_SYSTEM_VARIABLE_ITEM)
	{
		return (int)m_pFes->m_arrSystemVariable.size();
	}

	return 0;
}

int CFesSelectPointModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	//if(0 == arr_row_list.count())
	//	return 0;
	//else if(arr_row_list.count() < 1)
	//	return 0;
	//else
	//	return arr_row_list.at(0).size();

	return 0;
}

QVariant CFesSelectPointModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	//int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);
		if (m_nType == FES_DEVICE_AI)
		{
			if (index.column() == ID)
			{
				return (uint)m_pDevice->m_arrAIs[index.row()]->m_nID;
			}
			else if (index.column() == TagName)
			{
				return m_pDevice->m_arrAIs[index.row()]->m_szTagName;
			}
			else if (index.column() == Description)
			{
				return m_pDevice->m_arrAIs[index.row()]->m_strDescription;
			}
			else if (index.column() == OccNo)
			{
				return (uint)m_pDevice->m_arrAIs[index.row()]->GetOccNo();
			}
			//else if (index.column() == BlockNo)
			//{
			//	return (uint)m_pDevice->m_arrAIs[index.row()]->m_nBlockOccNo;
			//}
			//else if (index.column() == DataType)
			//{
			//	return (uint)m_pDevice->m_arrAIs[index.row()]->m_nDataType;
			//}
		}
		else if (m_nType == FES_DEVICE_DI)
		{
			if (index.column() == ID)
			{
				return (uint)m_pDevice->m_arrDIs[index.row()]->m_nID;
			}
			else if (index.column() == TagName)
			{
				return m_pDevice->m_arrDIs[index.row()]->m_szTagName;
			}
			else if (index.column() == Description)
			{
				return m_pDevice->m_arrDIs[index.row()]->m_strDescription;
			}
			else if (index.column() == OccNo)
			{
				return (uint)m_pDevice->m_arrDIs[index.row()]->GetOccNo();
			}
			//else if (index.column() == BlockNo)
			//{
			//	return (uint)m_pDevice->m_arrDIs[index.row()]->m_nBlockOccNo;
			//}
			//else if (index.column() == DataType)
			//{
			//	return (uint)m_pDevice->m_arrDIs[index.row()]->m_nDataType;
			//}
		}
		else if (m_nType == FES_DEVICE_AO)
		{
			if (index.column() == ID)
			{
				return (uint)m_pDevice->m_arrAOs[index.row()]->m_nID;
			}
			else if (index.column() == TagName)
			{
				return m_pDevice->m_arrAOs[index.row()]->m_szTagName;
			}
			else if (index.column() == Description)
			{
				return m_pDevice->m_arrAOs[index.row()]->m_strDescription;
			}
			else if (index.column() == OccNo)
			{
				return (uint)m_pDevice->m_arrAOs[index.row()]->GetOccNo();
			}
			//else if (index.column() == BlockNo)
			//{
			//	return (uint)m_pDevice->m_arrAOs[index.row()]->m_nBlockOccNo;
			//}
			else if (index.column() == DataType)
			{
				return (uint)m_pDevice->m_arrAOs[index.row()]->m_nDataType;
			}
		}
		else if (m_nType == FES_DEVICE_DO)
		{
			if (index.column() == ID)
			{
				return (uint)m_pDevice->m_arrDOs[index.row()]->m_nID;
			}
			else if (index.column() == TagName)
			{
				return m_pDevice->m_arrDOs[index.row()]->m_szTagName;
			}
			else if (index.column() == Description)
			{
				return m_pDevice->m_arrDOs[index.row()]->m_strDescription;
			}
			else if (index.column() == OccNo)
			{
				return (uint)m_pDevice->m_arrDOs[index.row()]->GetOccNo();
			}
			//else if (index.column() == BlockNo)
			//{
			//	return (uint)m_pDevice->m_arrDOs[index.row()]->m_nBlockOccNo;
			//}
			else if (index.column() == DataType)
			{
				return (uint)m_pDevice->m_arrDOs[index.row()]->m_nDataType;
			}
		}
		else if (m_nType == FES_USER_VARIBALE_GROUP_ITEM)
		{
			if (index.column() == ID)
			{
				return (uint)m_pUserContainer->m_arrItem[index.row()]->m_nID;
			}
			else if (index.column() == TagName)
			{
				return m_pUserContainer->m_arrItem[index.row()]->m_szTagName;
			}
			else if (index.column() == Description)
			{
				return m_pUserContainer->m_arrItem[index.row()]->m_strDescription;
			}
			else if (index.column() == OccNo)
			{
				return (uint)m_pUserContainer->m_arrItem[index.row()]->GetOccNo();
			}
			else if (index.column() == DataType)
			{
				return (uint)m_pUserContainer->m_arrItem[index.row()]->m_nType;
			}
		}
		else if (m_nType == FES_SYSTEM_VARIABLE_ITEM)
		{
			if (index.column() == ID)
			{
				return (uint)m_pFes->m_arrSystemVariable[index.row()]->m_nID;
			}
			else if (index.column() == TagName)
			{
				return m_pFes->m_arrSystemVariable[index.row()]->m_szTagName;
			}
			else if (index.column() == Description)
			{
				return m_pFes->m_arrSystemVariable[index.row()]->m_strDescription;
			}
			else if (index.column() == OccNo)
			{
				return (uint)m_pFes->m_arrSystemVariable[index.row()]->GetOccNo();
			}
			else if (index.column() == DataType)
			{
				return (uint)m_pFes->m_arrSystemVariable[index.row()]->m_nType;
			}
		}


		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (m_nType == FES_DEVICE_AI)
		{
			if (index.column() == ID)
			{
				return (uint)m_pDevice->m_arrAIs[index.row()]->m_nID;
			}
			else if (index.column() == TagName)
			{
				return m_pDevice->m_arrAIs[index.row()]->m_szTagName;
			}
			else if (index.column() == Description)
			{
				return m_pDevice->m_arrAIs[index.row()]->m_strDescription;
			}
			else if (index.column() == OccNo)
			{
				return (uint)m_pDevice->m_arrAIs[index.row()]->GetOccNo();
			}
			//else if (index.column() == BlockNo)
			//{
			//	return (uint)m_pDevice->m_arrAIs[index.row()]->m_nBlockOccNo;
			//}
			//else if (index.column() == DataType)
			//{
			//	return (uint)m_pDevice->m_arrAIs[index.row()]->m_nDataType;
			//}
		}
		else if (m_nType == FES_DEVICE_DI)
		{
			if (index.column() == ID)
			{
				return (uint)m_pDevice->m_arrDIs[index.row()]->m_nID;
			}
			else if (index.column() == TagName)
			{
				return m_pDevice->m_arrDIs[index.row()]->m_szTagName;
			}
			else if (index.column() == Description)
			{
				return m_pDevice->m_arrDIs[index.row()]->m_strDescription;
			}
			else if (index.column() == OccNo)
			{
				return (uint)m_pDevice->m_arrDIs[index.row()]->GetOccNo();
			}
			//else if (index.column() == BlockNo)
			//{
			//	return (uint)m_pDevice->m_arrDIs[index.row()]->m_nBlockOccNo;
			//}
			//else if (index.column() == DataType)
			//{
			//	return (uint)m_pDevice->m_arrDIs[index.row()]->m_nDataType;
			//}
		}
		else if (m_nType == FES_DEVICE_AO)
		{
			if (index.column() == ID)
			{
				return (uint)m_pDevice->m_arrAOs[index.row()]->m_nID;
			}
			else if (index.column() == TagName)
			{
				return m_pDevice->m_arrAOs[index.row()]->m_szTagName;
			}
			else if (index.column() == Description)
			{
				return m_pDevice->m_arrAOs[index.row()]->m_strDescription;
			}
			else if (index.column() == OccNo)
			{
				return (uint)m_pDevice->m_arrAOs[index.row()]->GetOccNo();
			}
			//else if (index.column() == BlockNo)
			//{
			//	return (uint)m_pDevice->m_arrAOs[index.row()]->m_nBlockOccNo;
			//}
			else if (index.column() == DataType)
			{
				return (uint)m_pDevice->m_arrAOs[index.row()]->m_nDataType;
			}
		}
		else if (m_nType == FES_DEVICE_DO)
		{
			if (index.column() == ID)
			{
				return (uint)m_pDevice->m_arrDOs[index.row()]->m_nID;
			}
			else if (index.column() == TagName)
			{
				return m_pDevice->m_arrDOs[index.row()]->m_szTagName;
			}
			else if (index.column() == Description)
			{
				return m_pDevice->m_arrDOs[index.row()]->m_strDescription;
			}
			else if (index.column() == OccNo)
			{
				return (uint)m_pDevice->m_arrDOs[index.row()]->GetOccNo();
			}
			//else if (index.column() == BlockNo)
			//{
			//	return (uint)m_pDevice->m_arrDOs[index.row()]->m_nBlockOccNo;
			//}
			else if (index.column() == DataType)
			{
				return (uint)m_pDevice->m_arrDOs[index.row()]->m_nDataType;
			}
		}


		return QVariant();
	}

	return QVariant();
}

QVariant CFesSelectPointModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_horizontal_header_list.size() > section && m_horizontal_header_list.size() > -1)
				return m_horizontal_header_list[section];
			else
				return QVariant();
		}
		else
		{
			//if(vertical_header_list.size() > section)
			//	return vertical_header_list[section]; // 垂直表头  
			//else
			//return QVariant();

			return QString(" ");
		}
	}

	return QVariant();
}

bool CFesSelectPointModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	Q_UNUSED(value);
	
	//int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return true;
}

Qt::ItemFlags CFesSelectPointModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	//flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}