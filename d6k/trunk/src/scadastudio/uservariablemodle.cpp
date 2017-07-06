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
				//ģ�����޸ģ�����û������󶨹�����ģ�������ڣ���ô����ģ����Ҳ�����޸�
				//Ϊ�˷������,hash tagname��node tagname + . + ��tagname�����Ժ��޸Ļ���ɾ����node tagname���ߵ�tagname�Ϳ���ֱ���ҵ�������tagname���޸�
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
				

				////////////////////////////////////////////////////֪ͨ�ⲿģ��scada variable module/////////////////////////////////////////////////////
				//scada variable
				std::vector<QString>  vec;
				m_pCore->GetModulesName(vec);
				auto pScadaVariableModule = m_pCore->GetModule("scada_variable");
				Q_ASSERT(pScadaVariableModule);
				if (!pScadaVariableModule)
				{
					auto strError = tr("Get scada variable module poiter failed!!!");
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

					return false;
				}
				bool bFlag = pScadaVariableModule->ChangeTagNameNodify(strSourceTagName, QString(strLastTagName.c_str()), Module_ID::USERVARAIBLE);
				Q_ASSERT(bFlag);
				if (!bFlag)
				{
					auto strError = QObject::tr("ScadaVariable relation SourceTagName %1 modify error!").arg(m_pVariable->m_arrItem[nRow]->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return false;
				}
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				
				//HASH ID�޸�
				if (m_pFesData->ModifyFesHashMap(USERVIRIABLEID, value.toString(), m_pVariable->m_arrItem[nRow]->m_szTagName))
				{
					memset(m_pVariable->m_arrItem[nRow]->m_szTagName, 0, sizeof(m_pVariable->m_arrItem[nRow]->m_szTagName));
					int nSize = value.toString().size();
					strncpy(m_pVariable->m_arrItem[nRow]->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
				}
				else
				{
					auto strError = tr("User varaible hash tagname %1 modify failed!!!").arg(value.toString());
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return false;
				}


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
			if (m_pVariable->m_arrItem[nRow]->m_strSourceTagName.isEmpty())
			{
				return true;
			}

			auto list = m_pVariable->m_arrItem[nRow]->m_strSourceTagName.split(".");
			if (list.count() != 3)
			{
				auto strTmp = QObject::tr("[Fes %1]  Delete UserVariable TagName %2 Relation SourceTagName %3 is invalid!!!").arg(m_pFesData->m_szTagName).arg(m_pVariable->m_arrItem[nRow]->m_szTagName).arg(m_pVariable->m_arrItem[nRow]->m_strSourceTagName);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);

				return false;
			}

			QString strTmp = QString("%1.%2").arg(list[0]).arg(list[1]);

			//ɾ��������ϵ
			if (!m_pFesData->DeleteUserVariableRelationSourceTagNameArr(strTmp.toStdString(), m_pVariable->m_arrItem[nRow]))
			{
				auto strTmp = QObject::tr("[Fes %1]  Delete UserVariable TagName %2 Relation SourceTagName failed!!!").arg(m_pFesData->m_szTagName).arg(m_pVariable->m_arrItem[nRow]->m_szTagName);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);

				return false;
			}

			m_pVariable->m_arrItem[nRow]->m_strSourceTagName.clear();

			return true;
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
		else if (index.column() == Alarm)
		{
			if (m_pVariable->m_arrItem[nRow]->m_strAlarmTagName.isEmpty())
			{
				return true;
			}

			//ɾ��������ϵ
			if (!m_pFesData->DeleteUserVariableRelationAlarmArr(m_pVariable->m_arrItem[nRow]->m_strAlarmTagName.toStdString(), m_pVariable->m_arrItem[nRow]))
			{
				auto strTmp = QObject::tr("[Fes %1]  Delete AI TagName %2 Relation Alarm failed!!!").arg(m_pFesData->m_szTagName).arg(m_pVariable->m_arrItem[nRow]->m_szTagName);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);

				return false;
			}

			m_pVariable->m_arrItem[nRow]->m_strAlarmTagName.clear();

			return true;
		}
		else if (index.column() == Scale)
		{
			if (m_pVariable->m_arrItem[nRow]->m_strScaleTagName.isEmpty())
			{
				return true;
			}

			//ɾ��������ϵ
			if (!m_pFesData->DeleteUserVariableRelationScaleArr(m_pVariable->m_arrItem[nRow]->m_strScaleTagName.toStdString(), m_pVariable->m_arrItem[nRow]))
			{
				auto strTmp = QObject::tr("[Fes %1]  Delete AI TagName %2 Relation Scale failed!!!").arg(m_pFesData->m_szTagName).arg(m_pVariable->m_arrItem[nRow]->m_szTagName);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);

				return false;
			}

			m_pVariable->m_arrItem[nRow]->m_strScaleTagName.clear();

			return true;
		}

		return true;
	}

	return false;
}

QVariant CUserVariableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // ˮƽ��ͷ  
		{
			if (m_horizontal_header_list.size() > section && section > -1)
				return m_horizontal_header_list[section];
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


Qt::ItemFlags CUserVariableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // ���õ�Ԫ��ɱ༭,�˴�ע��,��Ԫ���޷����༭  
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
** \date 2016��12��5�� 
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
** \date 2016��12��5��
** \note
********************************************************************************************************/
std::vector<Config::CUserVariable *> &CUserVariableModel::GetArr()
{
	return m_pVariable->m_arrItem;
}

/*! \fn bool CUserVariableModel::RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex())
********************************************************************************************************* 
** \brief CUserVariableModel::RemoveRows 
** \details ɾ������
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2016��12��5�� 
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

	//note ɾ��������ϵ
	QString strTmp = pUser->m_szTagName;
	//Scada����ģ������û�����
	//ǰ���û�����ɾ����������ǰ���û������Ĺ�����ϵҲɾ��
	//note
	//ģ�����޸ģ�����û������󶨹�����ģ�������ڣ���ô����ģ����Ҳ�����޸�
	//Ϊ�˷������,hash tagname��node tagname + . + ��tagname�����Ժ��޸Ļ���ɾ����node tagname���ߵ�tagname�Ϳ���ֱ���ҵ�������tagname���޸�
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

	////ǰ���û�����ɾ�������ǰ���û������й���sourcetagname����ô����hashҲɾ���󶨹�ϵ
	//if (!pUser->m_strSourceTagName.isEmpty())
	//{
	//	auto list = pUser->m_strSourceTagName.split(".");
	//	//�ݶ��ж�����
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

	////�澯����ɾ��
	//if (!pUser->m_strAlarmTagName.isEmpty())
	//{
	//	if (!m_pFesData->DeleteUserVariableRelationAlarmArr(pUser->m_strAlarmTagName.toStdString(), pUser))
	//	{
	//		auto strError = QObject::tr("User Variable Relation Alarm %1 of User Variable  %2 delete failed!!!").arg(pUser->m_strAlarmTagName).arg(pUser->m_szTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	////ת��ɾ������
	//if (!pUser->m_strScaleTagName.isEmpty())
	//{
	//	if (!m_pFesData->DeleteUserVariableRelationScaleArr(pUser->m_strScaleTagName.toStdString(), pUser))
	//	{
	//		auto strError = QObject::tr("User Variable Relation Scale %1 of User Variable  %2 delete failed!!!").arg(pUser->m_strScaleTagName).arg(pUser->m_szTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	////ǰ���û�����ɾ����ǰ��do BlockingSignalTag����������û���������ô������ϵɾ�� һ�Զ��ϵ
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
** \details ˢ��
** \return void 
** \author gw
** \date 2016��12��5�� 
** \note 
********************************************************************************************************/
void CUserVariableModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}