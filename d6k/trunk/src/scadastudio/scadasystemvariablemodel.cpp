#include "scadasystemvariablemodel.h"
#include "scadastudiodefine.h"
#include "scadastudio/icore.h"
#include "scadavirable_data.h"
#include "variant.h"

extern IMainModuleInterface *s_pGlobleCore;

CScadaSystemVariableModel::CScadaSystemVariableModel(QObject *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pConfigData
	, Config::CScadaData *pData)
	: QAbstractTableModel(parent), m_pCore(nullptr), m_pConfigData(nullptr), m_pData(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pConfigData);

	if(!pCore)
	{
		return;
	}

	if (!pConfigData)
	{
		return;
	}

	m_pCore = pCore;
	m_pConfigData = pConfigData;

	Q_ASSERT(pData);
	if (!pData)
	{
		return;
	}
	m_pData = pData;


	InitPara();

	InitData();
}

CScadaSystemVariableModel::~CScadaSystemVariableModel()
{

}

int CScadaSystemVariableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pData->m_arrSystemVariable.size();
}

int CScadaSystemVariableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	return 0;
}

QVariant CScadaSystemVariableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		if (index.column() == ID)
		{
			return (uint)m_pData->m_arrSystemVariable[nRow]->m_nID;
		}
		else if (index.column() == TagName)
		{
			return m_pData->m_arrSystemVariable[nRow]->m_szTagName;
		}
		else if (index.column() == Desc)
		{
			return m_pData->m_arrSystemVariable[nRow]->m_strDescription;
		}
		else if (index.column() == DataType)
		{
			return m_arrDataType[m_pData->m_arrSystemVariable[nRow]->m_nType];
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pData->m_arrSystemVariable[nRow]->GetOccNo();
		}


		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == ID)
		{
			return (uint)m_pData->m_arrSystemVariable[nRow]->m_nID;
		}
		else if (index.column() == TagName)
		{
			return m_pData->m_arrSystemVariable[nRow]->m_szTagName;
		}
		else if (index.column() == Desc)
		{
			return m_pData->m_arrSystemVariable[nRow]->m_strDescription;
		}
		else if (index.column() == DataType)
		{
			return m_arrDataType[m_pData->m_arrSystemVariable[nRow]->m_nType];
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pData->m_arrSystemVariable[nRow]->GetOccNo();
		}


		return QVariant();
	}

	return QVariant();
}

QVariant CScadaSystemVariableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_horizontal_header_list.size() > section && section > -1)
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


Qt::ItemFlags CScadaSystemVariableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	//flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CScadaSystemVariableModel::InitPara()
{
	//m_arrInfo.clear();

	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	//m_arrInfo.insert(ID, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	//m_arrInfo.insert(TagName, para);
	m_horizontal_header_list.append(para.desc);


	para.desc = tr("Desc");
	para.tagname = "Desc";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("DataType");
	para.tagname = "DataType";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);

	m_arrDataType.clear();
	m_arrDataType[DATA_TYPE::DT_BOOLEAN] = tr("Boolean");
	m_arrDataType[DT_CHAR] = tr("Char");
	m_arrDataType[DT_BYTE] = tr("Byte");
	m_arrDataType[DT_SHORT] = tr("Short");
	m_arrDataType[DT_WORD] = tr("Word");
	m_arrDataType[DT_INT] = tr("Int");
	m_arrDataType[DT_DWORD] = tr("DWord");
	m_arrDataType[DT_LLONG] = tr("LLong");
	m_arrDataType[DT_LWORD] = tr("LWord");
	m_arrDataType[DT_FLOAT] = tr("Float");
	m_arrDataType[DT_DOUBLE] = tr("Double");
	m_arrDataType[DT_STRING] = tr("String");
	m_arrDataType[DT_BLOCK] = tr("Block");
}


/*! \fn 
********************************************************************************************************* 
** \brief CSystemVariableModel::InitData 
** \details 
** \return void 
** \author gw
** \date 2016年11月28日 
** \note 
********************************************************************************************************/
void CScadaSystemVariableModel::InitData()
{

}