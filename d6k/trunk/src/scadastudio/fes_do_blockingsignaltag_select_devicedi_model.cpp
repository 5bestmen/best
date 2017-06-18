#include "fes_Do_BlockingSignalTag_Select_DeviceDI_Model.h"
#include "delegate.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "global.h"
#include "scadastudio/quoteItem.h"

#include <qstandarditemmodel.h>

CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::CFes_Do_BlockingSignalTag_Select_DeviceDI_Model(QObject *parent, Config::CDevice *pDevice)
	: QAbstractTableModel(parent)
{
	Q_ASSERT(pDevice);
	if (!pDevice)
	{
		return;
	}
	m_pDevice = pDevice;

	InitPara();
}

CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::~CFes_Do_BlockingSignalTag_Select_DeviceDI_Model()
{

}

void CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::SetHorizontalHeaderList(QStringList list)
{
	if (!horizontal_header_list.isEmpty())
	{
		horizontal_header_list.clear();
	}

	horizontal_header_list = list;
}

void CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::SetVerticalHeaderList(QStringList verticalHeaderList)
{
	//vertical_header_list = verticalHeaderList;
}

int CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::rowCount(const QModelIndex &parent) const
{
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	Q_UNUSED(parent);

	return (int)m_pDevice->m_arrDIs.size();
}

int CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::columnCount(const QModelIndex &parent) const
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

QVariant CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::data(const QModelIndex &index, int role) const
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
		else if (index.column() == DoubleDI)
		{
			return m_pDevice->m_arrDIs[nRow]->m_szDoubleDI;
		}
		else if (index.column() == Desc)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strDescription;
		}
		else if (index.column() == InitValue)
		{
			return m_pDevice->m_arrDIs[nRow]->m_bInitValue;
		}
		else if (index.column() == PinLabel)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strPinLabel;
		}
		else if (index.column() == Address)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strAddress;
		}
		else if (index.column() == InitQua)
		{
			return m_arrInitQua[m_pDevice->m_arrDIs[nRow]->m_nInitialQua].desc;
		}
		else if (index.column() == SignalType)
		{
			return m_arrSignalType[m_pDevice->m_arrDIs[nRow]->m_nInitialQua].desc;
		}
		else if (index.column() == Express)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strExpress;
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
		else if (index.column() == DoubleDI)
		{
			return m_pDevice->m_arrDIs[nRow]->m_szDoubleDI;
		}
		else if (index.column() == Desc)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strDescription;
		}
		else if (index.column() == InitValue)
		{
			return m_pDevice->m_arrDIs[nRow]->m_bInitValue;
		}
		else if (index.column() == PinLabel)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strPinLabel;
		}
		else if (index.column() == Address)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strAddress;
		}
		else if (index.column() == InitQua)
		{
			return m_arrInitQua[m_pDevice->m_arrDIs[nRow]->m_nInitialQua].desc;
		}
		else if (index.column() == SignalType)
		{
			return m_arrSignalType[m_pDevice->m_arrDIs[nRow]->m_nInitialQua].desc;
		}
		else if (index.column() == Express)
		{
			return m_pDevice->m_arrDIs[nRow]->m_strExpress;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	//flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::emitDataChanged(const QModelIndex &index)
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
void CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::InitPara()
{
	horizontal_header_list.clear();

	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	horizontal_header_list.append(para.desc);

	para.desc = tr("DoubleDI");
	para.tagname = "DoubleDI";
	horizontal_header_list.append(para.desc);

	para.desc = tr("Desc");
	para.tagname = "Desc";
	horizontal_header_list.append(para.desc);

	para.desc = tr("InitValue");
	para.tagname = "InitValue";
	horizontal_header_list.append(para.desc);

	para.desc = tr("InitQua");
	para.tagname = "InitQua";
	horizontal_header_list.append(para.desc);
	m_arrInitQua.clear();
	IDDESC tmp;
	tmp.desc = tr("NormalQua");
	tmp.ID = QString("%1").arg(NormalQua);
	m_arrInitQua.append(tmp);
	tmp.desc = tr("UndefiedQua");
	tmp.ID = QString("%1").arg(UndefiedQua);
	m_arrInitQua.append(tmp);
	tmp.desc = tr("UnnormalQua");
	tmp.ID = QString("%1").arg(UnnormalQua);
	m_arrInitQua.append(tmp);
	tmp.desc = tr("UnconnectQua");
	tmp.ID = QString("%1").arg(UnconnectQua);
	m_arrInitQua.append(tmp);


	para.desc = tr("Address");
	para.tagname = "Address";
	horizontal_header_list.append(para.desc);

	para.desc = tr("PinLabel");
	para.tagname = "PinLabel";
	horizontal_header_list.append(para.desc);

	para.desc = tr("SignalType");
	para.tagname = "SignalType";
	horizontal_header_list.append(para.desc);
	para.desc = tr("SignalType");
	para.tagname = "SignalType";
	m_arrSignalType.clear();
	tmp.desc = tr("NormallyOpen");
	tmp.ID = QString("%1").arg(NormallyOpen);
	m_arrSignalType.append(tmp);
	tmp.desc = tr("NormallyClosed");
	tmp.ID = QString("%1").arg(NormallyClosed);
	m_arrSignalType.append(tmp);

	para.desc = tr("Express");
	para.tagname = "Express";
	horizontal_header_list.append(para.desc);

	SetHorizontalHeaderList(horizontal_header_list);
}

void CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::RefrushModel()
{
	beginResetModel();
	endResetModel();
}
