#include "aomodel.h"
#include "scadastudiodefine.h"
#include "delegate.h"
#include "aotable.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "variant.h"
#include "global.h"
#include "scadastudio/imodule.h"

CAOModel::CAOModel(QObject *parent, std::vector <Config::CAnalogOutput *> &arrAOs, Config::CFesData *pFes, IMainModuleInterface *pCore)
	: QAbstractTableModel(parent), m_arrAOs(arrAOs), m_pFes(nullptr)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}
	
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pCore = pCore;

	m_pFes = pFes;

	InitPara();
}

CAOModel::~CAOModel()
{

}

int CAOModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_arrAOs.size();
}

int CAOModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_lstHorizontalHeader.size() > 0)
		return m_lstHorizontalHeader.size();

	//if(0 == arr_row_list.count())
	//	return 0;
	//else if(arr_row_list.count() < 1)
	//	return 0;
	//else
	//	return arr_row_list.at(0).size();

	return 0;
}

QVariant CAOModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == TagName)
		{
			return m_arrAOs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (uint)m_arrAOs[nRow]->m_nID;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_arrAOs[nRow]->GetOccNo();
		}
		else if (index.column() == BlockOccNo)
		{
			return (uint)m_arrAOs[nRow]->m_nBlockOccNo;
		}
		//else if (index.column() == DataType)
		//{
		//	return m_arrAOs[nRow]->m_nDataType;
		//}
		else if (index.column() == Desc)
		{
			return m_arrAOs[nRow]->m_strDescription;
		}
		else if (index.column() == InitQua)
		{
			return m_arrAOs[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_arrAOs[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_arrAOs[nRow]->m_strPinLabel;
		}
		else if (index.column() == InitValue)
		{
			return m_arrAOs[nRow]->m_dbInitValue;
		}
		else if (index.column() == Format)
		{
			return m_arrAOs[nRow]->m_strFormat;
		}
		else if (index.column() == Unit)
		{
			return m_arrAOs[nRow]->m_strUnit;
		}
		else if (index.column() == Enable)
		{
			return m_arrAOs[nRow]->m_bEnable;
		}
		else if (index.column() == MaxRaw)
		{
			return m_arrAOs[nRow]->m_dbMaxRaw;
		}
		else if (index.column() == MinRaw)
		{
			return m_arrAOs[nRow]->m_dbMinRaw;
		}
		else if (index.column() == MaxConvert)
		{
			return m_arrAOs[nRow]->m_dbMaxConvert;
		}
		else if (index.column() == MinConvert)
		{
			return m_arrAOs[nRow]->m_dbMinConvert;
		}
		else if (index.column() == ScaleTagName)
		{
			return m_arrAOs[nRow]->m_strScaleTagName;
		}
		//else if (index.column() == ScaleDesc)
		//{
		//	return m_arrAOs[nRow]->m_strScaleDesc;
		//}
		//else if (index.column() == ScaleType)
		//{
		//	if (m_arrAOs[nRow]->m_nScaleType == ScaleType::LINEAR)
		//	{
		//		return m_mapScaleType[ScaleType::LINEAR];
		//	}
		//	else if(m_arrAOs[nRow]->m_nScaleType == ScaleType::NONLINEAR)
		//	{
		//		return m_mapScaleType[ScaleType::NONLINEAR];
		//	}
		//}
		else if (index.column() == RangeL)
		{
			return (double)m_arrAOs[nRow]->m_dblRangeL;
		}
		else if (index.column() == RangeH)
		{
			return (double)m_arrAOs[nRow]->m_dblRangeH;
		}
		else if (index.column() == HighQua)
		{
			return (double)m_arrAOs[nRow]->m_dblHighQua;
		}
		else if (index.column() == LowQua)
		{
			return (double)m_arrAOs[nRow]->m_dblLowQua;
		}
		else if (index.column() == ReferenceCount)
		{
			return (uint)m_arrAOs[nRow]->m_nReferenceCount;
		}
		else if (index.column() == ChannelOccNo)
		{
			return (uint)m_arrAOs[nRow]->m_nChannelOccNo;
		}
		else if (index.column() == DeviceOccNo)
		{
			return (uint)m_arrAOs[nRow]->m_nDeviceOccNo;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == TagName)
		{
			return m_arrAOs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (uint)m_arrAOs[nRow]->m_nID;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_arrAOs[nRow]->GetOccNo();
		}
		else if (index.column() == BlockOccNo)
		{
			return (uint)m_arrAOs[nRow]->m_nBlockOccNo;
		}
		//else if (index.column() == DataType)
		//{
		//	return m_arrAOs[nRow]->m_nDataType;
		//}
		else if (index.column() == Desc)
		{
			return m_arrAOs[nRow]->m_strDescription;
		}
		else if (index.column() == InitQua)
		{
			return m_arrAOs[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_arrAOs[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_arrAOs[nRow]->m_strPinLabel;
		}
		else if (index.column() == InitValue)
		{
			return m_arrAOs[nRow]->m_dbInitValue;
		}
		else if (index.column() == Format)
		{
			return m_arrAOs[nRow]->m_strFormat;
		}
		else if (index.column() == Unit)
		{
			return m_arrAOs[nRow]->m_strUnit;
		}
		else if (index.column() == Enable)
		{
			return m_arrAOs[nRow]->m_bEnable;
		}
		else if (index.column() == MaxRaw)
		{
			return m_arrAOs[nRow]->m_dbMaxRaw;
		}
		else if (index.column() == MinRaw)
		{
			return m_arrAOs[nRow]->m_dbMinRaw;
		}
		else if (index.column() == MaxConvert)
		{
			return m_arrAOs[nRow]->m_dbMaxConvert;
		}
		else if (index.column() == MinConvert)
		{
			return m_arrAOs[nRow]->m_dbMinConvert;
		}
		else if (index.column() == ScaleTagName)
		{
			return m_arrAOs[nRow]->m_strScaleTagName;
		}
		//else if (index.column() == ScaleDesc)
		//{
		//	return m_arrAOs[nRow]->m_strScaleDesc;
		//}
		//else if (index.column() == ScaleType)
		//{
		//	if (m_arrAOs[nRow]->m_nScaleType == ScaleType::LINEAR)
		//	{
		//		return m_mapScaleType[ScaleType::LINEAR];
		//	}
		//	else if (m_arrAOs[nRow]->m_nScaleType == ScaleType::NONLINEAR)
		//	{
		//		return m_mapScaleType[ScaleType::NONLINEAR];
		//	}
		//}
		else if (index.column() == RangeL)
		{
			return (double)m_arrAOs[nRow]->m_dblRangeL;
		}
		else if (index.column() == RangeH)
		{
			return (double)m_arrAOs[nRow]->m_dblRangeH;
		}
		else if (index.column() == HighQua)
		{
			return (double)m_arrAOs[nRow]->m_dblHighQua;
		}
		else if (index.column() == LowQua)
		{
			return (double)m_arrAOs[nRow]->m_dblLowQua;
		}
		else if (index.column() == ReferenceCount)
		{
			return (uint)m_arrAOs[nRow]->m_nReferenceCount;
		}
		else if (index.column() == ChannelOccNo)
		{
			return (uint)m_arrAOs[nRow]->m_nChannelOccNo;
		}
		else if (index.column() == DeviceOccNo)
		{
			return (uint)m_arrAOs[nRow]->m_nDeviceOccNo;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CAOModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_lstHorizontalHeader.size() > section)
				return m_lstHorizontalHeader[section];
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

void CAOModel::InitPara()
{
	m_lstHorizontalHeader.clear();

	m_mapInfo.clear();

	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	m_mapInfo.insert(ID, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	m_mapInfo.insert(TagName, para);
	m_lstHorizontalHeader.append(para.desc);
	CLineEditDelegate *delegate = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CAOTable *)parent())->setItemDelegateForColumn(TagName, delegate);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_mapInfo.insert(OccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	CReadOnlyDelegate *pReadDelegate = new CReadOnlyDelegate(this);
	((CAOTable *)parent())->setItemDelegateForColumn(OccNo, pReadDelegate);


	para.desc = tr("BlockOccNo");
	para.tagname = "BlockOccNo";
	m_mapInfo.insert(BlockOccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAOTable *)parent())->setItemDelegateForColumn(BlockOccNo, pReadDelegate);

	//para.desc = tr("DataType");
	//para.tagname = "DataType";
	//m_arrInfo.insert(DataType, para);
	//m_horizontal_header_list.append(para.desc);
	//QVector<IDDESC> arrVecDataType;
	//IDDESC tmp;
	//tmp.desc = tr("Boolean");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_BOOLEAN);
	//arrVecDataType.append(tmp);
	//tmp.desc = tr("Char");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_CHAR);
	//arrVecDataType.append(tmp);
	//tmp.desc = tr("Byte");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_BYTE);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("Short");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_SHORT);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("Word");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_WORD);
	//arrVecDataType.append(tmp);
	//tmp.desc = tr("Int");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_INT);
	//arrVecDataType.append(tmp);
	//tmp.desc = tr("DWord");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_DWORD);
	//arrVecDataType.append(tmp);
	//tmp.desc = tr("Long");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_LLONG);
	//arrVecDataType.append(tmp);
	//tmp.desc = tr("LLong");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_LWORD);
	//arrVecDataType.append(tmp);
	//tmp.desc = tr("LWord");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_FLOAT);
	//arrVecDataType.append(tmp);
	//tmp.desc = tr("Float");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_DOUBLE);
	//arrVecDataType.append(tmp);
	//tmp.desc = tr("Double");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_STRING);
	//arrVecDataType.append(tmp);
	//tmp.desc = tr("String");
	//tmp.ID = QString("%1").arg(DATA_TYPE::DT_BLOCK);
	//arrVecDataType.append(tmp);
	//arrVecDataType.append(tmp);
	//CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, arrVecDataType);
	//((CAOTable *)parent())->setItemDelegateForColumn(DataType, pCombo);
	
	para.desc = tr("Desc");
	para.tagname = "Desc";
	m_mapInfo.insert(Desc, para);
	m_lstHorizontalHeader.append(para.desc);
	auto *pDelegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CAOTable *)parent())->setItemDelegateForColumn(Desc, pDelegateDesc);


	para.desc = tr("InitQua");
	para.tagname = "InitQua";
	m_mapInfo.insert(InitQua, para);
	m_lstHorizontalHeader.append(para.desc);
	QVector<IDDESC> arrInitQua;
	arrInitQua.clear();
	IDDESC tmp;
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
	((CAOTable *)parent())->setItemDelegateForColumn(InitQua, pComboInitQua);

	para.desc = tr("Address");
	para.tagname = "Address";
	m_mapInfo.insert(Address, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pDelegateAddress = new CLineEditDelegate(this, LINESTRING, MAX_ADDRESS_LENGTH_SCADASTUDIO);
	((CAOTable *)parent())->setItemDelegateForColumn(Address, pDelegateAddress);

	para.desc = tr("PinLabel");
	para.tagname = "PinLabel";
	m_mapInfo.insert(PinLabel, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAOTable *)parent())->setItemDelegateForColumn(PinLabel, pDelegateDesc);

	para.desc = tr("InitValue");
	para.tagname = "InitValue";
	m_mapInfo.insert(InitValue, para);
	m_lstHorizontalHeader.append(para.desc);
	delegate = new CLineEditDelegate(this, DOUBLETYPE, 6);
	((CAOTable *)parent())->setItemDelegateForColumn(InitValue, delegate);

	//待定
	para.desc = tr("Format");
	para.tagname = "Format";
	m_mapInfo.insert(Format, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAOTable *)parent())->setItemDelegateForColumn(Format, pDelegateDesc);

	para.desc = tr("Unit");
	para.tagname = "Unit";
	m_mapInfo.insert(Unit, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAOTable *)parent())->setItemDelegateForColumn(Unit, pDelegateDesc);


	para.desc = tr("Enable");
	para.tagname = "Enable";
	m_mapInfo.insert(Enable, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pDelegateCheck = new CCheckBoxDelegate(this);
	((CAOTable *)parent())->setItemDelegateForColumn(Enable, pDelegateCheck);

	para.desc = tr("MaxRaw");
	para.tagname = "MaxRaw";
	m_mapInfo.insert(MaxRaw, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("MinRaw");
	para.tagname = "MinRaw";
	m_mapInfo.insert(MinRaw, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("MaxConvert");
	para.tagname = "MaxConvert";
	m_mapInfo.insert(MaxConvert, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("MinConvert");
	para.tagname = "MinConvert";
	m_mapInfo.insert(MinConvert, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("ScaleTagName");
	para.tagname = "ScaleTagName";
	m_mapInfo.insert(ScaleTagName, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAOTable *)parent())->setItemDelegateForColumn(ScaleTagName, pReadDelegate);

	//para.desc = tr("ScaleType");
	//para.tagname = "ScaleType";
	//m_mapInfo.insert(ScaleType, para);
	//m_lstHorizontalHeader.append(para.desc);
	//m_mapScaleType.clear();
	//tmp.desc = tr("linear");
	////tmp.ID = QString("%1").arg(LINEAR);
	//m_mapScaleType.insert(LINEAR, tmp.desc);
	//tmp.desc = tr("nonlinear");
	////tmp.ID = QString("%1").arg(NONLINEAR);
	//m_mapScaleType.insert(NONLINEAR, tmp.desc);

	para.desc = tr("RangeL");
	para.tagname = "RangeL";
	m_mapInfo.insert(RangeL, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("RangeH");
	para.tagname = "RangeH";
	m_mapInfo.insert(RangeH, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("HighQua");
	para.tagname = "HighQua";
	m_mapInfo.insert(HighQua, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("LowQua");
	para.tagname = "LowQua";
	m_mapInfo.insert(LowQua, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("ReferenceCount");
	para.tagname = "ReferenceCount";
	m_mapInfo.insert(ReferenceCount, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("ChannelOccNo");
	para.tagname = "ChannelOccNo";
	m_mapInfo.insert(ChannelOccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAOTable *)parent())->setItemDelegateForColumn(ChannelOccNo, pReadDelegate);

	para.desc = tr("DeviceOccNo");
	para.tagname = "DeviceOccNo";
	m_mapInfo.insert(DeviceOccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAOTable *)parent())->setItemDelegateForColumn(DeviceOccNo, pReadDelegate);
}

bool CAOModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			//Q_ASSERT(m_pChannel);

			if (index.column() == TagName)
			{
				if (m_arrAOs[nRow]->m_szTagName != value.toString())
				{
					Q_ASSERT(m_pFes);
					if (!m_pFes)
					{
						return false;
					}

					auto strTagName = value.toString();
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						return false;
					}

					if (m_pFes->CheckTagNameIsExist(value.toString()))
					{
						auto strError = tr("ao tagname is exist!!!");
						Q_ASSERT(m_pCore);
						if (!m_pCore)
						{
							return false;
						}
						
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}
					
					if (!m_pFes->ModifyFesHashMap(AOHASHID, value.toString(), m_arrAOs[nRow]->m_szTagName))
					{
						auto strError = tr("ao tagname modify fail!!!");
						Q_ASSERT(m_pCore);
						if (!m_pCore)
						{
							return false;
						}

						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
						
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
					auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(m_pFes->m_szTagName));
					if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
					{
						return false;
					}

					//Source hash tagname 
					std::string strLastTagName = strNodeTagName + "." + m_arrAOs[nRow]->m_szTagName;
					auto strSourceTagName = QString("%1.%2").arg(strNodeTagName.c_str()).arg(strTagName);

					////////////////////////////////////////////////////通知外部模块scada variable module/////////////////////////////////////////////////////
					//scada variable
					std::vector<QString>  vec;
					m_pCore->GetModulesName(vec);
					auto pScadaVariableModule = m_pCore->GetModule("scada_variable");
					Q_ASSERT(pScadaVariableModule);
					Q_ASSERT(pScadaVariableModule);
					if (!pScadaVariableModule)
					{
						auto strError = tr("Get scada variable module poiter failed!!!");
						m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}
					pScadaVariableModule->ChangeTagNameNodify(strSourceTagName, QString(strLastTagName.c_str()), Module_ID::DEVICE_AO);
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					if (!m_pFes->ModifyUserVariableSourceTagNameArr(strSourceTagName.toStdString(), strLastTagName))
					{
						auto strError = tr("User variable sourcetagname hash %1 modify failed!!!").arg(strLastTagName.c_str());
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}


					if (!m_pFes->ModifyForwardingAOSourceTagNameArr(strTagName.toStdString(), std::string(m_arrAOs[nRow]->m_szTagName)))
					{
						auto strError = tr("Modify Forwarding AO sourcetagname hash %1 modify failed!!!").arg(strLastTagName.c_str());
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					//m_arrAOs[nRow]->m_szTagName = value.toString();
					memset(m_arrAOs[nRow]->m_szTagName, 0, sizeof(m_arrAOs[nRow]->m_szTagName));
					strncpy(m_arrAOs[nRow]->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));
				}
			}
			else if (index.column() == ID)
			{
				if (m_arrAOs[nRow]->m_nID != value.toInt())
				{
					m_arrAOs[nRow]->m_nID = value.toInt();
				}
			}
			else if (index.column() == BlockOccNo)
			{
				if (m_arrAOs[nRow]->m_nBlockOccNo != value.toInt())
				{
					m_arrAOs[nRow]->m_nBlockOccNo = value.toInt();
				}
			}
			//else if (index.column() == DataType)
			//{
			//	if (m_arrAOs[nRow]->m_nDataType != value.toInt())
			//	{
			//		m_arrAOs[nRow]->m_nDataType = value.toInt();
			//	}
			//}
			else if (index.column() == Desc)
			{
				if (m_arrAOs[nRow]->m_strDescription.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrAOs[nRow]->m_strDescription = strDesc;

					//m_pFes->ModifyAOsRelationScaleDescArr(m_arrAOs[nRow]->m_strScaleTagName.toStdString(), value.toString().toStdString());
				}
			}
			else if (index.column() == InitQua)
			{
				if (m_arrAOs[nRow]->m_nInitialQua != value.toDouble())
				{
					m_arrAOs[nRow]->m_nInitialQua = value.toDouble();
				}
			}
			else if (index.column() == Address)
			{
				if (m_arrAOs[nRow]->m_strAddress.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC, MAX_ADDRESS_LENGTH_SCADASTUDIO))
					{
						return false;
					}
					
					m_arrAOs[nRow]->m_strAddress = strDesc;
				}
			}
			else if (index.column() == PinLabel)
			{
				if (m_arrAOs[nRow]->m_strPinLabel.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrAOs[nRow]->m_strPinLabel = strDesc;
				}
			}
			else if (index.column() == InitValue)
			{
				if (m_arrAOs[nRow]->m_dbInitValue != value.toDouble())
				{
					m_arrAOs[nRow]->m_dbInitValue = value.toDouble();
				}
			}
			else if (index.column() == Format)
			{
				if (m_arrAOs[nRow]->m_strFormat != value.toString())
				{
					m_arrAOs[nRow]->m_strFormat = value.toString();
				}
			}
			else if (index.column() == Unit)
			{
				if (m_arrAOs[nRow]->m_strUnit.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrAOs[nRow]->m_strUnit = strDesc;
				}
			}
			else if (index.column() == Enable)
			{
				if (m_arrAOs[nRow]->m_bEnable != value.toBool())
				{
					m_arrAOs[nRow]->m_bEnable = value.toBool();
				}
			}
			else if (index.column() == MaxRaw)
			{
				if (m_arrAOs[nRow]->m_dbMaxRaw != value.toDouble())
				{
					m_arrAOs[nRow]->m_dbMaxRaw = value.toDouble();
				}
			}
			else if (index.column() == MinRaw)
			{
				if (m_arrAOs[nRow]->m_dbMinRaw != value.toDouble())
				{
					m_arrAOs[nRow]->m_dbMinRaw = value.toDouble();
				}
			}
			else if (index.column() == MaxConvert)
			{
				if (m_arrAOs[nRow]->m_dbMaxConvert != value.toDouble())
				{
					m_arrAOs[nRow]->m_dbMaxConvert = value.toDouble();
				}
			}
			else if (index.column() == MinConvert)
			{
				if (m_arrAOs[nRow]->m_dbMinConvert != value.toDouble())
				{
					m_arrAOs[nRow]->m_dbMinConvert = value.toDouble();
				}
			}
			else if (index.column() == ScaleTagName)
			{
				if (m_arrAOs[nRow]->m_strScaleTagName.isEmpty())
				{
					return true;
				}

				//删除关联关系
				if (!m_pFes->DeleteAOsRelationScaleArr(m_arrAOs[nRow]->m_strScaleTagName.toStdString(), m_arrAOs[nRow]))
				{
					auto strTmp = QObject::tr("[Fes %1]  Delete AO TagName %2 Relation Scale failed!!!").arg(m_pFes->m_szTagName).arg(m_arrAOs[nRow]->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);

					return false;
				}

				m_arrAOs[nRow]->m_strScaleTagName.clear();

				return true;
			}
			//else if (index.column() == ScaleDesc)
			//{
			//	if (m_arrAOs[nRow]->m_strScaleDesc != value.toString())
			//	{
			//		m_arrAOs[nRow]->m_strScaleDesc = value.toString();
			//	}
			//}
			//else if (index.column() == ScaleType)
			//{
			//	if (m_arrAOs[nRow]->m_nScaleType != value.toInt())
			//	{
			//		m_arrAOs[nRow]->m_nScaleType = value.toInt();
			//	}
			//}
			else if (index.column() == RangeL)
			{
				if (m_arrAOs[nRow]->m_dblRangeL != value.toDouble())
				{
					m_arrAOs[nRow]->m_dblRangeL = value.toDouble();
				}
			}
			else if (index.column() == RangeH)
			{
				if (m_arrAOs[nRow]->m_dblRangeH != value.toDouble())
				{
					m_arrAOs[nRow]->m_dblRangeH = value.toDouble();
				}
			}
			else if (index.column() == HighQua)
			{
				if (m_arrAOs[nRow]->m_dblHighQua != value.toDouble())
				{
					m_arrAOs[nRow]->m_dblHighQua = value.toDouble();
				}
			}
			else if (index.column() == LowQua)
			{
				if (m_arrAOs[nRow]->m_dblLowQua != value.toDouble())
				{
					m_arrAOs[nRow]->m_dblLowQua = value.toDouble();
				}
			}
			else if (index.column() == ReferenceCount)
			{
				if (m_arrAOs[nRow]->m_nReferenceCount != value.toInt())
				{
					m_arrAOs[nRow]->m_nReferenceCount = value.toInt();
				}
			}
			//if (index.column() == ChannelOccNo)
			//{
			//	if (m_arrAOs[nRow]->m_nChannelOccNo != value.toInt())
			//	{
			//		m_arrAOs[nRow]->m_nChannelOccNo = value.toInt();
			//	}
			//}
			//else if (index.column() == DeviceOccNo)
			//{
			//	if (m_arrAOs[nRow]->m_nDeviceOccNo != value.toInt())
			//	{
			//		m_arrAOs[nRow]->m_nDeviceOccNo = value.toInt();
			//	}
			//}

			emit dataChanged(index, index);
			
			return true;
		}

		return false;
	}

	return false;
}

/*! \fn Qt::ItemFlags CAOModel::flags(const QModelIndex &index) const
********************************************************************************************************* 
** \brief CAOModel::flags 
** \details 
** \param index 
** \return Qt::ItemFlags 
** \author gw
** \date 2016年8月12日 
** \note 
********************************************************************************************************/
Qt::ItemFlags CAOModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CAOModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_arrAOs.size();

	beginInsertRows(parent, position, position + rows - 1);

	for (int i = position; i < position + rows; i++)
	{
		auto pAnalogPoint = m_pFes->AddNewAnalogOutput();

		pAnalogPoint->SetOccNo(i + 1);
		pAnalogPoint->m_nID = i + 1;

		pAnalogPoint->m_strAddress = QString("%1").arg(pAnalogPoint->m_nID);

		//auto strNewTagName = QString("AnalogOutput%1").arg(pAnalogPoint->m_nID);
		//auto strNewDesc = QString("AnalogOutput%1").arg(pAnalogPoint->m_nID);

		////是否校验

		////pAnalogPoint->m_szTagName = strNewTagName;
		//memset(pAnalogPoint->m_szTagName, 0, sizeof(pAnalogPoint->m_szTagName));
		//strncpy(pAnalogPoint->m_szTagName, strNewTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, strNewTagName.size()));

		//pAnalogPoint->m_strDescription = strNewDesc;
		//pAnalogPoint->m_bNewPointAlarm = true;

		m_arrAOs.push_back(pAnalogPoint);
	}

	endInsertRows();

	return true;
}

/*! \fn bool CDIModel::RemoveRows(int position, int rows, const QModelIndex &parent)
*********************************************************************************************************
** \brief CDIModel::RemoveRows
** \details 删除数据
** \param position
** \param rows
** \param parent
** \return bool
** \author gw
** \date 2016年8月11日
** \note
********************************************************************************************************/
bool CAOModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	//auto nCount = m_arrAOs.size();

	auto pAnalog = m_arrAOs[position];

	auto it = std::find(m_arrAOs.begin(), m_arrAOs.end(), pAnalog);
	if (it != m_arrAOs.end())
	{
		m_arrAOs.erase(it);
	}
	
	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	//note 删除关联关系
	QString strTmp = pAnalog->m_szTagName;
	if (!m_pFes->DeleteFesHashItem(AOHASHID, strTmp))
	{
		return false;
	}


	//模拟量删除，如果用户变量绑定关联的模拟量存在，那么关联模拟量也跟着删除
	//为了方便查找,hash tagname是node tagname + . + 点tagname，在以后修改或者删除的node tagname或者点tagname就可以直接找到，属性tagname不修改
	auto pNodeModule = m_pCore->GetModule("NODE_CONFIG");
	Q_ASSERT(pNodeModule);
	if (!pNodeModule)
	{
		auto strError = tr("Get node module poiter failed!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
		
		return false;
	}
	auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(m_pFes->m_szTagName));
	if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
	{
		return false;
	}

	//Source hash tagname 
	std::string strLastTagName = strNodeTagName + "." + pAnalog->m_szTagName;
	if (!m_pFes->DeleteUserVariableSourceTagNameArr(strLastTagName))
	{
		return false;
	}

	////////////////////////////////////////////////////通知外部模块scada variable module/////////////////////////////////////////////////////
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
	pScadaVariableModule->ChangeTagNameNodify(QString(""), QString(strLastTagName.c_str()), Module_ID::DEVICE_AO);

	//AO删除，如果前置转发AO关联前置AO tagname，那么关联关系删除
	if (!m_pFes->DeleteForwardingAOSourceTagNameArr(strTmp.toStdString()))
	{
		auto strError = QObject::tr("Delete ForwardingAO SourceTagName %1 hash fail!!!").arg(strTmp);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}


	//AO删除，如果AO scale关联绑定转换，那么关联绑定关系也删除
	if (!pAnalog->m_strScaleTagName.isEmpty())
	{
		if (!m_pFes->DeleteAOsRelationScaleArr(pAnalog->m_strScaleTagName.toStdString(), pAnalog))
		{
			auto strError = QObject::tr("Delete AOs %1 Relation Scale %2 hash fail!!!").arg(pAnalog->m_szTagName).arg(pAnalog->m_strScaleTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}




	delete pAnalog;
	pAnalog = nullptr;

	endRemoveRows();

	if (m_arrAOs.size() == 0)
	{
		RefrushModel();
	}

	return true;
}

void CAOModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}