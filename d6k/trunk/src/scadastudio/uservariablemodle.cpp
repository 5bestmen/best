#include "uservariablemodle.h"
#include "delegate.h"
#include "uservariabletable.h"
#include "config_data.h"
#include "variant.h"
#include "global.h"
#include "scadastudio/icore.h"
#include "scadastudio/imodule.h"

CUserVariableModel::CUserVariableModel(QObject *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData, Config::CUserVariableGroup *pVariable)
	: QAbstractTableModel(parent), m_pCore(nullptr), m_pVariable(nullptr), m_pFesData(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pVariable);

	if (!pCore)
	{
		return;
	}

	if (!pVariable)
	{
		return;
	}

	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		return;
	}
	m_pFesData = pFesData;

	m_pCore = pCore;
	m_pVariable = pVariable;

	InitPara();
}

CUserVariableModel::~CUserVariableModel()
{

}

int CUserVariableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pVariable->m_arrItem.size();
}

int CUserVariableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	return 0;
}

QVariant CUserVariableModel::data(const QModelIndex &index, int role) const
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

bool CUserVariableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		auto nRow = index.row();
		
		if (index.column() == ID)
		{
			if (m_pVariable->m_arrItem[nRow]->m_nID != value.toInt())
			{
				m_pVariable->m_arrItem[nRow]->m_nID = value.toInt();
			}
		}
		else if (index.column() == TagName)
		{
			if (value.toString().compare(m_pVariable->m_arrItem[nRow]->m_szTagName) != 0)
			{
				if (m_pFesData->CheckTagNameIsExist(value.toString()))
				{
					return false;
				}

				if (!CheckTagNameIsValid(value.toString(), FES_DESC))
				{
					return false;
				}
				
				//note
				//模拟量修改，如果用户变量绑定关联的模拟量存在，那么关联模拟量也跟着修改
				//为了方便查找,hash tagname是node tagname + . + 点tagname，在以后修改或者删除的node tagname或者点tagname就可以直接找到，属性tagname不修改
				auto pNodeModule = m_pCore->GetModule("NODE_CONFIG");
				Q_ASSERT(pNodeModule);
				if (!pNodeModule)
				{
					auto strError = tr("Get node module poiter failed!!!");
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

					return false;
				}
				auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(m_pFesData->m_szTagName));
				if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
				{
					return false;
				}

				//Source hash tagname 
				std::string strLastTagName = strNodeTagName + "." + m_pVariable->m_arrItem[nRow]->m_szTagName;
				auto strSourceTagName = QString("%1.%2").arg(strNodeTagName.c_str()).arg(value.toString());

				if (!m_pFesData->ModifyUserVariableSourceTagNameArr(strSourceTagName.toStdString(), strLastTagName))
				{
					auto strError = QObject::tr("SourceTagName %1 modify error!").arg(m_pVariable->m_arrItem[nRow]->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return false;
				}
				

				memset(m_pVariable->m_arrItem[nRow]->m_szTagName, 0, sizeof(m_pVariable->m_arrItem[nRow]->m_szTagName));
				int nSize = value.toString().size();
				strncpy(m_pVariable->m_arrItem[nRow]->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			}
		}
		else if (index.column() == Description)
		{
			if (m_pVariable->m_arrItem[nRow]->m_strDescription == value.toString())
			{
				if (!CheckDescIsValid(value.toString(), FES_DESC))
				{
					return false;
				}
				
				m_pVariable->m_arrItem[nRow]->m_strDescription = value.toString();
			}
		}
		else if (index.column() == DataType)
		{
			if (m_pVariable->m_arrItem[nRow]->m_nType != value.toInt())
			{
				m_pVariable->m_arrItem[nRow]->m_nType = value.toInt();
			}
		}
		else if (index.column() == InitValue)
		{
			if (m_pVariable->m_arrItem[nRow]->m_dbInitValue != value.toDouble())
			{
				m_pVariable->m_arrItem[nRow]->m_dbInitValue = value.toDouble();
			}
		}
		else if (index.column() == SourceTagName)
		{
			if (m_pVariable->m_arrItem[nRow]->m_strSourceTagName != value.toString())
			{
				m_pVariable->m_arrItem[nRow]->m_strSourceTagName = value.toString();
			}
		}
		else if (index.column() == Address)
		{
			if (m_pVariable->m_arrItem[nRow]->m_strAddress != value.toString())
			{
				if (!CheckDescIsValid(value.toString(), FES_DESC))
				{
					return false;
				}
				
				m_pVariable->m_arrItem[nRow]->m_strAddress = value.toString();
			}
		}
		else if (index.column() == PinLabel)
		{
			if (!CheckDescIsValid(value.toString(), FES_DESC))
			{
				return false;
			}
			
			if (m_pVariable->m_arrItem[nRow]->m_strPinLabel != value.toString())
			{
				m_pVariable->m_arrItem[nRow]->m_strPinLabel = value.toString();
			}
		}

		return true;
	}

	return false;
}

QVariant CUserVariableModel::headerData(int section, Qt::Orientation orientation, int role) const
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


Qt::ItemFlags CUserVariableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CUserVariableModel::InitPara()
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
	CLineEditDelegate *delegate = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CUserVariableTable *)parent())->setItemDelegateForColumn(TagName, delegate);

	para.desc = tr("Desc");
	para.tagname = "Desc";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CUserVariableTable *)parent())->setItemDelegateForColumn(Description, delegateDesc);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	auto pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CUserVariableTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);

	para.desc = tr("DataType");
	para.tagname = "DataType";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	QVector<IDDESC> arrVecDataType;
	IDDESC tmp;
	tmp.desc = tr("Boolean");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_BOOLEAN);
	arrVecDataType.append(tmp);
	tmp.desc = tr("Char");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_CHAR);
	arrVecDataType.append(tmp);
	tmp.desc = tr("Byte");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_BYTE);
	arrVecDataType.append(tmp);
	tmp.desc = QStringLiteral("Short");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_SHORT);
	arrVecDataType.append(tmp);
	tmp.desc = QStringLiteral("Word");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_WORD);
	arrVecDataType.append(tmp);
	tmp.desc = tr("Int");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_INT);
	arrVecDataType.append(tmp);
	tmp.desc = tr("DWord");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_DWORD);
	arrVecDataType.append(tmp);
	tmp.desc = tr("Long");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_LLONG);
	arrVecDataType.append(tmp);
	tmp.desc = tr("LLong");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_LWORD);
	arrVecDataType.append(tmp);
	tmp.desc = tr("LWord");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_FLOAT);
	arrVecDataType.append(tmp);
	tmp.desc = tr("Float");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_DOUBLE);
	arrVecDataType.append(tmp);
	tmp.desc = tr("Double");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_STRING);
	arrVecDataType.append(tmp);
	tmp.desc = tr("String");
	tmp.ID = QString("%1").arg(DATA_TYPE::DT_BLOCK);
	arrVecDataType.append(tmp);
	CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, arrVecDataType);
	((CUserVariableTable *)parent())->setItemDelegateForColumn(DataType, pCombo);


	para.desc = tr("InitValue");
	para.tagname = "InitValue";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("InitialQua");
	para.tagname = "InitialQua";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	//
	QVector<IDDESC> arrInitQua;
	arrInitQua.clear();
	tmp.desc = tr("NormalQua");
	tmp.ID = QString("%1").arg(NormalQua);
	arrInitQua.append(tmp);
	tmp.desc = tr("UndefiedQua");
	tmp.ID = QString("%1").arg(UndefiedQua);
	arrInitQua.append(tmp);
	tmp.desc = tr("UnnormalQua");
	tmp.ID = QString("%1").arg(UnnormalQua);
	arrInitQua.append(tmp);
	tmp.desc = tr("UnconnectQua");
	tmp.ID = QString("%1").arg(UnconnectQua);
	arrInitQua.append(tmp);
	CComboBoxDelegate *pComboInitQua = new CComboBoxDelegate(this, arrInitQua);
	((CUserVariableTable *)parent())->setItemDelegateForColumn(InitialQua, pComboInitQua);

	para.desc = tr("Address");
	para.tagname = "Address";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	auto pDelegateAddress = new CLineEditDelegate(this, LINESTRING, MAX_ADDRESS_LENGTH_SCADASTUDIO);
	((CUserVariableTable *)parent())->setItemDelegateForColumn(Address, pDelegateAddress);

	para.desc = tr("PinLabel");
	para.tagname = "PinLabel";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	((CUserVariableTable *)parent())->setItemDelegateForColumn(PinLabel, delegateDesc);

	para.desc = tr("SourceTagName");
	para.tagname = "SourceTagName";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CUserVariableTable *)parent())->setItemDelegateForColumn(SourceTagName, pReadOnlyDelegate);

	para.desc = tr("Scale");
	para.tagname = "Scale";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	((CUserVariableTable *)parent())->setItemDelegateForColumn(Scale, pReadOnlyDelegate);

	para.desc = tr("Alarm");
	para.tagname = "Alarm";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	((CUserVariableTable *)parent())->setItemDelegateForColumn(Alarm, pReadOnlyDelegate);
}

/*! \fn bool CUserVariableModel::InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex())
********************************************************************************************************* 
** \brief CUserVariableModel::InsertRows 
** \details 
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2016年12月5日 
** \note 
********************************************************************************************************/
bool CUserVariableModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	Q_UNUSED(parent);
	
	//auto nCount = m_pVariable->m_arrItem.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pFesData);
	if (!m_pFesData)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{
		auto pPoint = m_pFesData->AddUserVariable();

		pPoint->SetOccNo(i + 1);
		pPoint->m_nID = i + 1;

		m_pVariable->m_arrItem.push_back(pPoint);
	}

	endInsertRows();

	return true;
}

/*! \fn std::vector<Config::CUserVariable *> &CUserVariableTable::GetArr()
*********************************************************************************************************
** \brief CUserVariableTable::GetArr
** \details
** \return std::vector<Config::CUserVariable *> &
** \author gw
** \date 2016年12月5日
** \note
********************************************************************************************************/
std::vector<Config::CUserVariable *> &CUserVariableModel::GetArr()
{
	return m_pVariable->m_arrItem;
}

/*! \fn bool CUserVariableModel::RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex())
********************************************************************************************************* 
** \brief CUserVariableModel::RemoveRows 
** \details 删除数据
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2016年12月5日 
** \note 
********************************************************************************************************/
bool CUserVariableModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	Q_UNUSED(parent);
	
	beginRemoveRows(parent, rows, rows);

	//auto nCount = m_pVariable->GetItem().size();

	auto pUser = m_pVariable->GetItem()[position];

	auto it = std::find(m_pVariable->GetItem().begin(), m_pVariable->GetItem().end(), pUser);
	if (it != m_pVariable->GetItem().end())
	{
		m_pVariable->GetItem().erase(it);
	}

	//note 删除关联关系
	QString strTmp = pUser->m_szTagName;
	//Scada变量模块关联用户变量
	//前置用户变量删除，关联改前置用户变量的关联关系也删除
	//note
	//模拟量修改，如果用户变量绑定关联的模拟量存在，那么关联模拟量也跟着修改
	//为了方便查找,hash tagname是node tagname + . + 点tagname，在以后修改或者删除的node tagname或者点tagname就可以直接找到，属性tagname不修改
	auto pNodeModule = m_pCore->GetModule("NODE_CONFIG");
	Q_ASSERT(pNodeModule);
	if (!pNodeModule)
	{
		auto strError = QObject::tr("Get node module poiter failed!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}
	auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(m_pFesData->m_szTagName));
	if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
	{
		return false;
	}

	//Source hash tagname 
	auto strSourceTagName = QString("%1.%2").arg(strNodeTagName.c_str()).arg(pUser->m_szTagName);
	if (!m_pFesData->DeleteFesUserVaraibleCombineRelaton(strTmp.toStdString(), pUser))
	{
		auto strError = QObject::tr("Delete FesUserVaraible %1 Combine Relaton failed!!!").arg(strTmp);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	if (!m_pFesData->DeleteFesHashItem(USERVIRIABLEID, strTmp))
	{
		return false;
	}

	////前置用户变量删除，如果前置用户变量有关联sourcetagname，那么关联hash也删除绑定关系
	//if (!pUser->m_strSourceTagName.isEmpty())
	//{
	//	auto list = pUser->m_strSourceTagName.split(".");
	//	//暂定判定方法
	//	if (list.count() == 3)
	//	{
	//		auto strTmp = list[0] + "." + list[1];
	//		if (!m_pFesData->DeleteUserVariableRelationSourceTagNameArr(strTmp.toStdString(), pUser))
	//		{
	//			auto strError = QObject::tr("Delete User Variable Relation SourceTagName Arr %1 delete failed!!!").arg(pUser->m_strSourceTagName);
	//			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//			return false;
	//		}
	//	}
	//	else
	//	{
	//		auto strError = QObject::tr("User Variable SourceTagName %1 of User Variable Arr %2 is invalid!!!").arg(pUser->m_strSourceTagName).arg(pUser->m_szTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	////告警关联删除
	//if (!pUser->m_strAlarmTagName.isEmpty())
	//{
	//	if (!m_pFesData->DeleteUserVariableRelationAlarmArr(pUser->m_strAlarmTagName.toStdString(), pUser))
	//	{
	//		auto strError = QObject::tr("User Variable Relation Alarm %1 of User Variable  %2 delete failed!!!").arg(pUser->m_strAlarmTagName).arg(pUser->m_szTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	////转换删除关联
	//if (!pUser->m_strScaleTagName.isEmpty())
	//{
	//	if (!m_pFesData->DeleteUserVariableRelationScaleArr(pUser->m_strScaleTagName.toStdString(), pUser))
	//	{
	//		auto strError = QObject::tr("User Variable Relation Scale %1 of User Variable  %2 delete failed!!!").arg(pUser->m_strScaleTagName).arg(pUser->m_szTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	////前置用户变量删除，前置do BlockingSignalTag如果关联此用户变量，那么关联关系删除 一对多关系
	//if (!m_pFesData->DeleteDOsBlockingSignalTagArr(pUser->m_szTagName))
	//{
	//	auto strError = QObject::tr("Delete DOs BlockingSignalTag %1 hash failed!!!").arg(pUser->m_szTagName);
	//	m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//	return false;
	//}

	delete pUser;
	pUser = nullptr;

	endRemoveRows();

	if (m_pVariable->GetItem().size() == 0)
	{
		RefrushModel();
	}

	return true;
}

/*! \fn void CUserVariableModel::RefrushModel()
********************************************************************************************************* 
** \brief CUserVariableModel::RefrushModel 
** \details 刷新
** \return void 
** \author gw
** \date 2016年12月5日 
** \note 
********************************************************************************************************/
void CUserVariableModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}