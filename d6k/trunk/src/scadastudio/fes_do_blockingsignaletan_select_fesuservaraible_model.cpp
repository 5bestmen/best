#include "fes_do_blockingsignaletan_select_fesuservaraible_model.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "global.h"
#include "scadastudio/quoteItem.h"
#include "delegate.h"
#include "variant.h"

#include <QTableView>

CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model::CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model(QObject *parent, Config::CUserVariableGroup *pVariable)
	: QAbstractTableModel(parent)
{
	if (!pVariable)
	{
		return;
	}
	m_pVariable = pVariable;

	InitPara();
}

CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model::~CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model()
{

}

int CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pVariable->m_arrItem.size();
}

int CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	return 0;
}

QVariant CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		if (index.column() == ID)
		{
			return (uint)m_pVariable->m_arrItem[nRow]->m_nID;
		}
		else if (index.column() == TagName)
		{
			return m_pVariable->m_arrItem[nRow]->m_szTagName;
		}
		else if (index.column() == Description)
		{
			return m_pVariable->m_arrItem[nRow]->m_strDescription;
		}
		else if (index.column() == DataType)
		{
			return m_arrVecDataType[m_pVariable->m_arrItem[nRow]->m_nType].desc;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pVariable->m_arrItem[nRow]->GetOccNo();
		}
		else if (index.column() == InitValue)
		{
			return m_pVariable->m_arrItem[nRow]->m_dbInitValue;
		}
		else if (index.column() == InitialQua)
		{
			return m_arrInitQua[m_pVariable->m_arrItem[nRow]->m_nInitialQua].desc;
		}
		else if (index.column() == Address)
		{
			return m_pVariable->m_arrItem[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_pVariable->m_arrItem[nRow]->m_strPinLabel;
		}
		else if (index.column() == SourceTagName)
		{
			return m_pVariable->m_arrItem[nRow]->m_strSourceTagName;
		}
		else if (index.column() == Scale)
		{
			return m_pVariable->m_arrItem[nRow]->m_strScaleTagName;
		}
		else if (index.column() == Alarm)
		{
			return m_pVariable->m_arrItem[nRow]->m_strAlarmTagName;
		}


		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == ID)
		{
			return (uint)m_pVariable->m_arrItem[nRow]->m_nID;
		}
		else if (index.column() == TagName)
		{
			return m_pVariable->m_arrItem[nRow]->m_szTagName;
		}
		else if (index.column() == Description)
		{
			return m_pVariable->m_arrItem[nRow]->m_strDescription;
		}
		else if (index.column() == DataType)
		{
			return (uint)m_pVariable->m_arrItem[nRow]->m_nType;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pVariable->m_arrItem[nRow]->GetOccNo();
		}
		else if (index.column() == InitValue)
		{
			return m_pVariable->m_arrItem[nRow]->m_dbInitValue;
		}
		else if (index.column() == InitialQua)
		{
			return m_pVariable->m_arrItem[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_pVariable->m_arrItem[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_pVariable->m_arrItem[nRow]->m_strPinLabel;
		}
		else if (index.column() == SourceTagName)
		{
			return m_pVariable->m_arrItem[nRow]->m_strSourceTagName;
		}
		else if (index.column() == Scale)
		{
			return m_pVariable->m_arrItem[nRow]->m_strScaleTagName;
		}
		else if (index.column() == Alarm)
		{
			return m_pVariable->m_arrItem[nRow]->m_strAlarmTagName;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model::headerData(int section, Qt::Orientation orientation, int role) const
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


Qt::ItemFlags CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	//flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model::InitPara()
{
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

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	auto pReadOnlyDelegate = new CReadOnlyDelegate(this);

	para.desc = tr("DataType");
	para.tagname = "DataType";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	m_arrVecDataType.clear();
	IDDESC tmp;
	tmp.desc = tr("Boolean");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_BOOLEAN);
	m_arrVecDataType.append(tmp);
	tmp.desc = tr("Char");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_CHAR);
	m_arrVecDataType.append(tmp);
	tmp.desc = tr("Byte");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_BYTE);
	m_arrVecDataType.append(tmp);
	tmp.desc = QStringLiteral("Short");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_SHORT);
	m_arrVecDataType.append(tmp);
	tmp.desc = QStringLiteral("Word");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_WORD);
	m_arrVecDataType.append(tmp);
	tmp.desc = tr("Int");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_INT);
	m_arrVecDataType.append(tmp);
	tmp.desc = tr("DWord");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_DWORD);
	m_arrVecDataType.append(tmp);
	tmp.desc = tr("Long");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_LLONG);
	m_arrVecDataType.append(tmp);
	tmp.desc = tr("LLong");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_LWORD);
	m_arrVecDataType.append(tmp);
	tmp.desc = tr("LWord");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_FLOAT);
	m_arrVecDataType.append(tmp);
	tmp.desc = tr("Float");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_DOUBLE);
	m_arrVecDataType.append(tmp);
	tmp.desc = tr("Double");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_STRING);
	m_arrVecDataType.append(tmp);
	tmp.desc = tr("String");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_BLOCK);
	m_arrVecDataType.append(tmp);

	para.desc = tr("InitValue");
	para.tagname = "InitValue";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("InitialQua");
	para.tagname = "InitialQua";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	//
	m_arrInitQua.clear();
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
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("PinLabel");
	para.tagname = "PinLabel";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("SourceTagName");
	para.tagname = "SourceTagName";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);


	para.desc = tr("Scale");
	para.tagname = "Scale";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("Alarm");
	para.tagname = "Alarm";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
}