#include "forwarding_select_aomodel.h"
#include "delegate.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "global.h"

CForwardingAOSelectDataModel::CForwardingAOSelectDataModel(QObject *parent, Config::CDevice *pDevice)
	: QAbstractTableModel(parent), m_pDevice(nullptr)
{
	Q_ASSERT(pDevice);
	if (!pDevice)
	{
		return;
	}
	m_pDevice = pDevice;

	InitPara();
}

CForwardingAOSelectDataModel::~CForwardingAOSelectDataModel()
{

}

void CForwardingAOSelectDataModel::SetHorizontalHeaderList(QStringList list)
{
	if (!horizontal_header_list.isEmpty())
	{
		horizontal_header_list.clear();
	}

	horizontal_header_list = list;
}

void CForwardingAOSelectDataModel::SetVerticalHeaderList(QStringList verticalHeaderList)
{
	//vertical_header_list = verticalHeaderList;
}

int CForwardingAOSelectDataModel::rowCount(const QModelIndex &parent) const
{
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	Q_UNUSED(parent);

	return (int)m_pDevice->m_arrAOs.size();
}

int CForwardingAOSelectDataModel::columnCount(const QModelIndex &parent) const
{	
	Q_UNUSED(parent);
	
	if (horizontal_header_list.size() > 0)
		return horizontal_header_list.size();

	//if(0 == arr_row_list.count())
	//	return 0;
	//else if(arr_row_list.count() < 1)
	//	return 0;
	//else
	//	return arr_row_list.at(0).size();

	return true;
}

QVariant CForwardingAOSelectDataModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		
		if (index.column() == ID)
		{
			return static_cast<uint>(m_pDevice->m_arrAOs[nRow]->m_nID);
		}
		else if (index.column() == TagName)
		{
			return m_pDevice->m_arrAOs[nRow]->m_szTagName;
		}
		else if (index.column() == Desc)
		{
			return m_pDevice->m_arrAOs[nRow]->m_strDescription;
		}
		else if (index.column() == Address)
		{
			return m_pDevice->m_arrAOs[nRow]->m_strAddress;
		}
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == ID)
		{
			return static_cast<uint>(m_pDevice->m_arrAOs[nRow]->m_nID);
		}
		else if (index.column() == TagName)
		{
			return m_pDevice->m_arrAOs[nRow]->m_szTagName;
		}
		else if (index.column() == Desc)
		{
			return m_pDevice->m_arrAOs[nRow]->m_strDescription;
		}
		else if (index.column() == Address)
		{
			return m_pDevice->m_arrAOs[nRow]->m_strAddress;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CForwardingAOSelectDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (horizontal_header_list.size() > section && section > -1)
				return horizontal_header_list[section];
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

Qt::ItemFlags CForwardingAOSelectDataModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	//flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CForwardingAOSelectDataModel::emitDataChanged(const QModelIndex &index)
{
	emit dataChanged(index, index);
}

/*! \fn void CAIModel::InitPara()
********************************************************************************************************* 
** \brief CAIModel::InitPara 
** \details 初始化参数
** \return void 
** \author gw
** \date 2016年5月17日 
** \note 
********************************************************************************************************/
void CForwardingAOSelectDataModel::InitPara()
{
	horizontal_header_list.clear();

	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	horizontal_header_list.append(para.desc);

	para.desc = tr("Desc");
	para.tagname = "Desc";
	horizontal_header_list.append(para.desc);

	para.desc = tr("Address");
	para.tagname = "Address";
	horizontal_header_list.append(para.desc);

	SetHorizontalHeaderList(horizontal_header_list);
}

void CForwardingAOSelectDataModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}