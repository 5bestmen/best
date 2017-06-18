#include "fes_di_select_doubledi_model.h"
#include "delegate.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "global.h"

CFes_DI_Select_DoubleDI_DataModel::CFes_DI_Select_DoubleDI_DataModel(QObject *parent, Config::CDevice *pDevice)
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

CFes_DI_Select_DoubleDI_DataModel::~CFes_DI_Select_DoubleDI_DataModel()
{

}

void CFes_DI_Select_DoubleDI_DataModel::SetHorizontalHeaderList(QStringList list)
{
	if (!horizontal_header_list.isEmpty())
	{
		horizontal_header_list.clear();
	}

	horizontal_header_list = list;
}

void CFes_DI_Select_DoubleDI_DataModel::SetVerticalHeaderList(QStringList verticalHeaderList)
{
	//vertical_header_list = verticalHeaderList;
}

int CFes_DI_Select_DoubleDI_DataModel::rowCount(const QModelIndex &parent) const
{
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	Q_UNUSED(parent);

	return (int)m_pDevice->m_arrDIs.size();
}

int CFes_DI_Select_DoubleDI_DataModel::columnCount(const QModelIndex &parent) const
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

QVariant CFes_DI_Select_DoubleDI_DataModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		
		if (index.column() == ID)
		{
			return static_cast<uint>(m_pDevice->m_arrDIs[nRow]->m_nID);
		}
		else if (index.column() == TagName)
		{
			return m_pDevice->m_arrDIs[nRow]->m_szTagName;
		}
		else if (index.column() == Desc)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strDescription;
		}
		else if (index.column() == Address)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strAddress;
		}
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == ID)
		{
			return static_cast<uint>(m_pDevice->m_arrDIs[nRow]->m_nID);
		}
		else if (index.column() == TagName)
		{
			return m_pDevice->m_arrDIs[nRow]->m_szTagName;
		}
		else if (index.column() == Desc)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strDescription;
		}
		else if (index.column() == Address)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strAddress;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CFes_DI_Select_DoubleDI_DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // ˮƽ��ͷ  
		{
			if (horizontal_header_list.size() > section && section > -1)
				return horizontal_header_list[section];
			else
				return QVariant();
		}
		else
		{
			//if(vertical_header_list.size() > section)
			//	return vertical_header_list[section]; // ��ֱ��ͷ  
			//else
			//return QVariant();

			return QString(" ");
		}
	}

	return QVariant();
}

Qt::ItemFlags CFes_DI_Select_DoubleDI_DataModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	//flag |= Qt::ItemIsEditable; // ���õ�Ԫ��ɱ༭,�˴�ע��,��Ԫ���޷����༭  
	return flag;
}

void CFes_DI_Select_DoubleDI_DataModel::emitDataChanged(const QModelIndex &index)
{
	emit dataChanged(index, index);
}

/*! \fn void CAIModel::InitPara()
********************************************************************************************************* 
** \brief CAIModel::InitPara 
** \details ��ʼ������
** \return void 
** \author gw
** \date 2016��5��17�� 
** \note 
********************************************************************************************************/
void CFes_DI_Select_DoubleDI_DataModel::InitPara()
{
	horizontal_header_list.clear();

	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegate = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	//((CAITable *)parent())->setItemDelegateForColumn(TagName, delegate);


	para.desc = tr("Desc");
	para.tagname = "Desc";
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);

	para.desc = tr("Address");
	para.tagname = "Address";
	horizontal_header_list.append(para.desc);


	SetHorizontalHeaderList(horizontal_header_list);

}

void CFes_DI_Select_DoubleDI_DataModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}