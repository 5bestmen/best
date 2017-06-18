#include "scadauservariablemodel.h"
#include "scadastudio/icore.h"
#include "delegate.h"
#include "scadauservariabletable.h"
#include "scadastudio/define.h"
#include "scadavirable_data.h"
#include "variant.h"
#include "global.h"

CScadaUserVariableModel::CScadaUserVariableModel(QObject *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pScadaVirableConfig
	, Config::CScadaUserVariableGroup *pGroup, Config::CScadaData *pScada)
	: QAbstractTableModel(parent), m_pCore(nullptr), m_pGroup(nullptr), m_pScadaVirableConfig(nullptr), m_pScada(nullptr), m_pGlobleCore(nullptr)
{
	Q_ASSERT(pScadaVirableConfig);
	if (!pScadaVirableConfig)
	{
		return;
	}

	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return;
	}

	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}

	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pScadaVirableConfig = pScadaVirableConfig;
	m_pCore = pCore;
	m_pGroup = pGroup;
	m_pScada = pScada;

	InitPara();
}

CScadaUserVariableModel::~CScadaUserVariableModel()
{

}

void CScadaUserVariableModel::InitPara()
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
	((CScadaUserVariableTable *)parent())->setItemDelegateForColumn(TagName, delegate);

	para.desc = tr("Desc");
	para.tagname = "Desc";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CScadaUserVariableTable *)parent())->setItemDelegateForColumn(Description, delegateDesc);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	auto pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CScadaUserVariableTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);

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
	((CScadaUserVariableTable *)parent())->setItemDelegateForColumn(DataType, pCombo);


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
	((CScadaUserVariableTable *)parent())->setItemDelegateForColumn(InitialQua, pComboInitQua);

	para.desc = tr("Address");
	para.tagname = "Address";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	auto pDelegateAddress = new CLineEditDelegate(this, LINESTRING, MAX_ADDRESS_LENGTH_SCADASTUDIO);
	((CScadaUserVariableTable *)parent())->setItemDelegateForColumn(Address, pDelegateAddress);


	para.desc = tr("PinLabel");
	para.tagname = "PinLabel";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("SourceTagName");
	para.tagname = "SourceTagName";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CScadaUserVariableTable *)parent())->setItemDelegateForColumn(SourceTagName, pReadOnlyDelegate);

	para.desc = tr("Scale");
	para.tagname = "Scale";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaUserVariableTable *)parent())->setItemDelegateForColumn(Scale, pReadOnlyDelegate);

	para.desc = tr("Alarm");
	para.tagname = "Alarm";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaUserVariableTable *)parent())->setItemDelegateForColumn(Alarm, pReadOnlyDelegate);
}

int CScadaUserVariableModel::rowCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);
	
	return (int)m_pGroup->m_arrItem.size();
}

int CScadaUserVariableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	return 0;
}

QVariant CScadaUserVariableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		if (index.column() == ID)
		{
			return (uint)m_pGroup->m_arrItem[nRow]->m_nID;
		}
		else if (index.column() == TagName)
		{
			return m_pGroup->m_arrItem[nRow]->m_szTagName;
		}
		else if (index.column() == Description)
		{
			return m_pGroup->m_arrItem[nRow]->m_strDescription;
		}
		else if (index.column() == DataType)
		{
			return (uint)m_pGroup->m_arrItem[nRow]->m_nType;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pGroup->m_arrItem[nRow]->GetOccNo();
		}
		else if (index.column() == InitValue)
		{
			return m_pGroup->m_arrItem[nRow]->m_dbInitValue;
		}
		else if (index.column() == InitialQua)
		{
			return m_pGroup->m_arrItem[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_pGroup->m_arrItem[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_pGroup->m_arrItem[nRow]->m_strPinLabel;
		}
		else if (index.column() == SourceTagName)
		{
			return m_pGroup->m_arrItem[nRow]->m_strSourceTagName;
		}
		else if (index.column() == Scale)
		{
			return m_pGroup->m_arrItem[nRow]->m_strScaleTagName;
		}
		else if (index.column() == Alarm)
		{
			return m_pGroup->m_arrItem[nRow]->m_strAlarmTagName;
		}


		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == ID)
		{
			return (uint)m_pGroup->m_arrItem[nRow]->m_nID;
		}
		else if (index.column() == TagName)
		{
			return m_pGroup->m_arrItem[nRow]->m_szTagName;
		}
		else if (index.column() == Description)
		{
			return m_pGroup->m_arrItem[nRow]->m_strDescription;
		}
		else if (index.column() == DataType)
		{
			return (uint)m_pGroup->m_arrItem[nRow]->m_nType;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pGroup->m_arrItem[nRow]->GetOccNo();
		}
		else if (index.column() == InitValue)
		{
			return m_pGroup->m_arrItem[nRow]->m_dbInitValue;
		}
		else if (index.column() == InitialQua)
		{
			return m_pGroup->m_arrItem[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_pGroup->m_arrItem[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_pGroup->m_arrItem[nRow]->m_strPinLabel;
		}
		else if (index.column() == SourceTagName)
		{
			return m_pGroup->m_arrItem[nRow]->m_strSourceTagName;
		}
		else if (index.column() == Scale)
		{
			return m_pGroup->m_arrItem[nRow]->m_strScaleTagName;
		}
		else if (index.column() == Alarm)
		{
			return m_pGroup->m_arrItem[nRow]->m_strAlarmTagName;
		}

		return QVariant();
	}

	return QVariant();
}

bool CScadaUserVariableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		auto nRow = index.row();

		if (index.column() == ID)
		{
			if (m_pGroup->m_arrItem[nRow]->m_nID != value.toInt())
			{
				m_pGroup->m_arrItem[nRow]->m_nID = value.toInt();
			}
		}
		else if (index.column() == TagName)
		{
			if (value.toString().compare(m_pGroup->m_arrItem[nRow]->m_szTagName) != 0)
			{
				auto bFlag = m_pScada->CheckTagNameIsExist(value.toString());
				if (bFlag)
				{
					auto strError = QString("User variable tagname %1 is exist!!!").arg(value.toString().toStdString().c_str());

					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
					return false;
				}

				if (!CheckTagNameIsValid(value.toString(), SCADAVARIABLE_DESC))
				{
					auto strError = QString("User variable tagname %1 length is > %2!!!").arg(value.toString()).arg(MAX_TAGNAME_LEN_SCADASTUDIO);

					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
					return false;
				}
				
				bFlag = m_pScada->ModifyHashMap(SCADA_VARIABLE_HASH_ID::USERID, value.toString(), m_pGroup->m_arrItem[nRow]->m_szTagName);
				Q_ASSERT(bFlag);
				if (!bFlag)
				{
					auto strError = QObject::tr("Modify user variable tagname %1 fail!!!").arg(m_pGroup->m_arrItem[nRow]->m_szTagName);
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
					return false;
				}

				memset(m_pGroup->m_arrItem[nRow]->m_szTagName, 0, sizeof(m_pGroup->m_arrItem[nRow]->m_szTagName));
				int nSize = value.toString().size();
				strncpy(m_pGroup->m_arrItem[nRow]->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			}
		}
		else if (index.column() == Description)
		{
			if (m_pGroup->m_arrItem[nRow]->m_strDescription != value.toString())
			{
				if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
				{
					auto strError = QString("User variable desc %1 length is > %2!!!").arg(value.toString()).arg(MAX_NAME_LENGTH_SCADASTUDIO);

					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
					return false;
				}
				
				m_pGroup->m_arrItem[nRow]->m_strDescription = value.toString();
			}
		}
		else if (index.column() == DataType)
		{
			if (m_pGroup->m_arrItem[nRow]->m_nType != value.toInt())
			{
				m_pGroup->m_arrItem[nRow]->m_nType = value.toInt();
			}
		}
		else if (index.column() == InitValue)
		{
			if (m_pGroup->m_arrItem[nRow]->m_dbInitValue != value.toDouble())
			{
				m_pGroup->m_arrItem[nRow]->m_dbInitValue = value.toDouble();
			}
		}
		else if (index.column() == SourceTagName)
		{
			if (m_pGroup->m_arrItem[nRow]->m_strSourceTagName != value.toString())
			{
				m_pGroup->m_arrItem[nRow]->m_strSourceTagName = value.toString();
			}
		}
		else if (index.column() == Address)
		{
			if (m_pGroup->m_arrItem[nRow]->m_nInitialQua != value.toInt())
			{
				if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC, MAX_ADDRESS_LENGTH_SCADASTUDIO))
				{
					return false;
				}
				
				m_pGroup->m_arrItem[nRow]->m_nInitialQua = value.toInt();
			}
		}
		else if (index.column() == PinLabel)
		{
			if (m_pGroup->m_arrItem[nRow]->m_strPinLabel != value.toString())
			{
				if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
				{
					return false;
				}
				
				m_pGroup->m_arrItem[nRow]->m_strPinLabel = value.toString();
			}
		}

		return true;
	}

	return false;
}

QVariant CScadaUserVariableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags CScadaUserVariableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CScadaUserVariableModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_pGroup->m_arrItem.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{
		//scada唯一
		auto pPoint = m_pScada->AddScadaUserVariable();

		pPoint->SetOccNo(i + 1);
		pPoint->m_nID = i + 1;

		m_pGroup->m_arrItem.push_back(pPoint);
	}

	endInsertRows();

	return true;
}

std::vector<Config::CScadaUserVariable *> &CScadaUserVariableModel::GetArr()
{
	return m_pGroup->m_arrItem;
}

bool CScadaUserVariableModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, position, position + rows - 1);

	//auto nCount = m_pGroup->GetItem().size();

	auto pUser = m_pGroup->GetItem()[position];

	auto it = std::find(m_pGroup->GetItem().begin(), m_pGroup->GetItem().end(), pUser);
	if (it != m_pGroup->GetItem().end())
	{
		m_pGroup->GetItem().erase(it);
	}
	else
	{
		endRemoveRows();
		
		return false;
	}

	//note 删除关联关系
	QString strTmp = pUser->m_szTagName;
	//删除用户变量的scale、alarm和sourcetagname关联的关系
	if (!m_pScada->DeleteUserVariableCombineRelation(strTmp.toStdString(), pUser))
	{
		auto strError = QObject::tr("Delete UserVariable CombineRelation %1 fail!!!").arg(strTmp);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	if (!m_pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::USERID, strTmp))
	{
		endRemoveRows();
		
		return false;
	}

	////用户变量删除，如果用户变量sourcetagname有关联关系，那么关联关系删除
	//if (!pUser->m_strSourceTagName.isEmpty())
	//{
	//	auto list = pUser->m_strSourceTagName.split(".");
	//	//拼接tagname暂定判定方法
	//	if (list.count() == 3)
	//	{
	//		//拼接tagname node.data
	//		auto strTagNameCombine = QString("%1.%2").arg(list[0]).arg(list[1]);
	//		
	//		if (!m_pScada->DeleteUserVariableRelationSourceTagNameArr(strTagNameCombine.toStdString(), pUser))
	//		{
	//			auto strError = QObject::tr("Delete Scada variable User Variable delete RelationSourceTagName %1 fail!!!").arg(strTagNameCombine);
	//			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

	//			return false;
	//		}
	//	}
	//}

	////用户变量删除，如果用户变量的ALARM有关联关系，那么关联关系删除
	//if (!pUser->m_strAlarmTagName.isEmpty())
	//{
	//	if (!m_pScada->DeleteUserVariableRelationAlarmArr(pUser->m_strAlarmTagName.toStdString(), pUser))
	//	{
	//		auto strError = QObject::tr("Delete Scada variable User Variable delete RelationSourceTagName %1 fail!!!").arg(pUser->m_strAlarmTagName);
	//		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	} 
	//}

	////用户变量删除，如果用户变量的scale有关联，那么关联管也删除
	//if (!pUser->m_strScaleTagName.isEmpty())
	//{
	//	if (!m_pScada->DeleteUserVariableRelationScaleArr(pUser->m_strScaleTagName.toStdString(), pUser))
	//	{
	//		auto strError = QObject::tr("Delete UserVariable Relation Scale %1 fail!!!").arg(pUser->m_strScaleTagName);
	//		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}


	delete pUser;
	pUser = nullptr;

	endRemoveRows();

	if (m_pGroup->GetItem().size() == 0)
	{
		RefrushModel();
	}

	return true;
}

void CScadaUserVariableModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}
