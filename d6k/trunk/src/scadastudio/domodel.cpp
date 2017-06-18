#include "domodel.h"
#include "dotable.h"
#include "delegate.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "variant.h"
#include "global.h"
#include "scadastudio/imodule.h"

CDOModel::CDOModel(QObject *parent, std::vector <Config::CDigitalOutput *> &arrDOs, Config::CFesData *pFes, IMainModuleInterface *pCore)
	: QAbstractTableModel(parent), m_arrDOs(arrDOs), m_pFes(nullptr)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}
	m_pFes = pFes;

	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}
	m_pCore = pCore;

	InitPara();
}

CDOModel::~CDOModel()
{

}

int CDOModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_arrDOs.size();
}

int CDOModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_lstHorizontalHeader.size() > 0)
		return m_lstHorizontalHeader.size();

	return 0;
}

QVariant CDOModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == TagName)
		{
			return m_arrDOs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_arrDOs[nRow]->m_nID;
		}
		else if (index.column() == BlockOccNo)
		{
			return (uint)m_arrDOs[nRow]->m_nBlockOccNo;
		}
		//else if (index.column() == DataType)
		//{
		//	return (uint)m_arrDOs[nRow]->m_nDataType;
		//}
		else if (index.column() == Desc)
		{
			return m_arrDOs[nRow]->m_strDescription;
		}
		else if (index.column() == InitValue)
		{
			return m_arrDOs[nRow]->m_bInitValue;
		}
		else if (index.column() == InitQua)
		{
			return m_arrDOs[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_arrDOs[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_arrDOs[nRow]->m_strPinLabel;
		}
		else if (index.column() == IsSBO)
		{
			return m_arrDOs[nRow]->m_bIsSBO;
		}
		else if (index.column() == TelCtlWaitTime)
		{
			return m_arrDOs[nRow]->m_dblTelCtlWaitTime;
		}
		else if (index.column() == TelCtlSendTime)
		{
			return m_arrDOs[nRow]->m_dblTelCtlSendTime;
		}
		else if (index.column() == TelCtlExecTime)
		{
			return m_arrDOs[nRow]->m_dblTelCtlExecTime;
		}
		else if (index.column() == AssociatedDI)
		{
			return m_arrDOs[nRow]->m_szAssociatedDI;
		}
		else if (index.column() == AssociatedDIValType)
		{
			return m_arrDOs[nRow]->m_nAssociatedDIValType;
		}
		else if (index.column() == BlockingSignalTag)
		{
			return m_arrDOs[nRow]->m_szBlockingSignalTag;
		}
		else if (index.column() == BlockingProgram)
		{
			return m_arrDOs[nRow]->m_strBlockingProgram;
		}
		else if (index.column() == StartProgram)
		{
			return m_arrDOs[nRow]->m_strStartProgram;
		}
		else if (index.column() == ReferenceCount)
		{
			return (uint)m_arrDOs[nRow]->m_nReferenceCount;
		}
		else if (index.column() == ChannelOccNo)
		{
			return (uint)m_arrDOs[nRow]->m_nChannelOccNo;
		}
		else if (index.column() == DeviceOccNo)
		{
			return (uint)m_arrDOs[nRow]->m_nDeviceOccNo;
		}
		else if (index.column() == OccNo)
		{
			return uint(m_arrDOs[nRow]->GetOccNo());
		}
		
		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			return m_arrDOs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_arrDOs[nRow]->m_nID;
		}
		else if (index.column() == BlockOccNo)
		{
			return (uint)m_arrDOs[nRow]->m_nBlockOccNo;
		}
		//else if (index.column() == DataType)
		//{
		//	return (uint)m_arrDOs[nRow]->m_nDataType;
		//}
		else if (index.column() == Desc)
		{
			return m_arrDOs[nRow]->m_strDescription;
		}
		else if (index.column() == InitValue)
		{
			return m_arrDOs[nRow]->m_bInitValue;
		}
		else if (index.column() == InitQua)
		{
			return m_arrDOs[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_arrDOs[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_arrDOs[nRow]->m_strPinLabel;
		}
		else if (index.column() == IsSBO)
		{
			return m_arrDOs[nRow]->m_bIsSBO;
		}
		else if (index.column() == TelCtlWaitTime)
		{
			return m_arrDOs[nRow]->m_dblTelCtlWaitTime;
		}
		else if (index.column() == TelCtlSendTime)
		{
			return m_arrDOs[nRow]->m_dblTelCtlSendTime;
		}
		else if (index.column() == TelCtlExecTime)
		{
			return m_arrDOs[nRow]->m_dblTelCtlExecTime;
		}
		else if (index.column() == AssociatedDI)
		{
			return m_arrDOs[nRow]->m_szAssociatedDI;
		}
		else if (index.column() == AssociatedDIValType)
		{
			return m_arrDOs[nRow]->m_nAssociatedDIValType;
		}
		else if (index.column() == BlockingSignalTag)
		{
			return m_arrDOs[nRow]->m_szBlockingSignalTag;
		}
		else if (index.column() == BlockingProgram)
		{
			return m_arrDOs[nRow]->m_strBlockingProgram;
		}
		else if (index.column() == StartProgram)
		{
			return m_arrDOs[nRow]->m_strStartProgram;
		}
		else if (index.column() == ReferenceCount)
		{
			return (uint)m_arrDOs[nRow]->m_nReferenceCount;
		}
		else if (index.column() == ChannelOccNo)
		{
			return (uint)m_arrDOs[nRow]->m_nChannelOccNo;
		}
		else if (index.column() == DeviceOccNo)
		{
			return (uint)m_arrDOs[nRow]->m_nDeviceOccNo;
		}
		else if (index.column() == OccNo)
		{
			return uint(m_arrDOs[nRow]->GetOccNo());
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CDOModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags CDOModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CDOModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			//Q_ASSERT(m_pChannel);

			//if (index.column() == OccNo)
			//{
			//	if (m_arrDOs[nRow]->m_nOccNo != value.toInt())
			//	{
			//		m_arrDOs[nRow]->m_nOccNo = value.toInt();
			//	}
			//}
			if (index.column() == TagName)
			{
				if (m_arrDOs[nRow]->m_szTagName != value.toString())
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

					if (m_pFes->CheckTagNameIsExist(strTagName))
					{
						auto strError = tr("do tagname is exist!!!");
						Q_ASSERT(m_pCore);
						if (!m_pCore)
						{
							return false;
						}

						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					if (!m_pFes->ModifyFesHashMap(DOHASHID, strTagName, m_arrDOs[nRow]->m_szTagName))
					{
						auto strError = tr("do tagname modify fail!!!");
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
					std::string strLastTagName = strNodeTagName + "." + m_arrDOs[nRow]->m_szTagName;
					auto strSourceTagName = QString("%1.%2").arg(strNodeTagName.c_str()).arg(strTagName);

					if (!m_pFes->ModifyUserVariableSourceTagNameArr(strSourceTagName.toStdString(), strLastTagName))
					{
						auto strError = tr("User variable sourcetagname hash %1 modify failed!!!").arg(strLastTagName.c_str());
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

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
					pScadaVariableModule->ChangeTagNameNodify(strSourceTagName, QString(strLastTagName.c_str()), Module_ID::DEVICE_DO);
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//转发通道do sourcetagname modify
					if (!m_pFes->ModifyForwardingDOSourceTagNameArr(strTagName.toStdString(), std::string(m_arrDOs[nRow]->m_szTagName)))
					{
						auto strError = tr("Modify forwarding do sourcetagname hash %1 modify failed!!!").arg(m_arrDOs[nRow]->m_szTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					memset(m_arrDOs[nRow]->m_szTagName, 0, sizeof(m_arrDOs[nRow]->m_szTagName));
					int nSize = strTagName.size();
					strncpy(m_arrDOs[nRow]->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					emit dataChanged(index, index);
				}
			}
			else if (index.column() == ID)
			{
				if (m_arrDOs[nRow]->m_nID != value.toInt())
				{
					m_arrDOs[nRow]->m_nID = value.toInt();
				}
			}
			else if (index.column() == BlockOccNo)
			{
				if (m_arrDOs[nRow]->m_nBlockOccNo != value.toInt())
				{
					m_arrDOs[nRow]->m_nBlockOccNo = value.toInt();
				}
			}
			//else if (index.column() == DataType)
			//{
			//	if (m_arrDOs[nRow]->m_nDataType != value.toInt())
			//	{
			//		m_arrDOs[nRow]->m_nDataType = value.toInt();
			//	}
			//}
			else if (index.column() == Desc)
			{
				if (m_arrDOs[nRow]->m_strDescription.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrDOs[nRow]->m_strDescription = strDesc;
				}
			}
			else if (index.column() == InitValue)
			{
				if (m_arrDOs[nRow]->m_bInitValue != value.toBool())
				{
					m_arrDOs[nRow]->m_bInitValue = value.toBool();
				}
			}
			else if (index.column() == InitQua)
			{
				if (m_arrDOs[nRow]->m_nInitialQua != value.toDouble())
				{
					m_arrDOs[nRow]->m_nInitialQua = value.toDouble();
				}
			}
			else if (index.column() == Address)
			{
				if (m_arrDOs[nRow]->m_strAddress.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC, MAX_ADDRESS_LENGTH_SCADASTUDIO))
					{
						return false;
					}
					
					m_arrDOs[nRow]->m_strAddress = strDesc;
				}
			}
			else if (index.column() == PinLabel)
			{
				if (m_arrDOs[nRow]->m_strPinLabel.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrDOs[nRow]->m_strPinLabel = strDesc;
				}
			}
			else if (index.column() == IsSBO)
			{
				if (m_arrDOs[nRow]->m_bIsSBO != value.toBool())
				{
					m_arrDOs[nRow]->m_bIsSBO = value.toBool();
				}
			}
			else if (index.column() == TelCtlWaitTime)
			{
				if (m_arrDOs[nRow]->m_dblTelCtlWaitTime != value.toDouble())
				{
					m_arrDOs[nRow]->m_dblTelCtlWaitTime = value.toDouble();
				}
			}
			else if (index.column() == TelCtlSendTime)
			{
				if (m_arrDOs[nRow]->m_dblTelCtlSendTime != value.toDouble())
				{
					m_arrDOs[nRow]->m_dblTelCtlSendTime = value.toDouble();
				}
			}
			else if (index.column() == TelCtlExecTime)
			{
				if (m_arrDOs[nRow]->m_dblTelCtlExecTime != value.toDouble())
				{
					m_arrDOs[nRow]->m_dblTelCtlExecTime = value.toDouble();
				}
			}
			else if (index.column() == AssociatedDI)
			{
				if (value.toString().compare(m_arrDOs[nRow]->m_szAssociatedDI) != 0)
				{
					auto strTmp = value.toString();
					if (!CheckTagNameIsValid(strTmp, FES_DESC))
					{
						return false;
					}
					memset(m_arrDOs[nRow]->m_szAssociatedDI, 0, sizeof(m_arrDOs[nRow]->m_szAssociatedDI));
					auto nSize = strTmp.size();
					strncpy(m_arrDOs[nRow]->m_szAssociatedDI, strTmp.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
				}
			}
			else if (index.column() == AssociatedDIValType)
			{
				if (m_arrDOs[nRow]->m_nAssociatedDIValType != value.toInt())
				{
					m_arrDOs[nRow]->m_nAssociatedDIValType = value.toInt();
				}
			}
			else if (index.column() == BlockingSignalTag)
			{
				if (value.toString().compare(m_arrDOs[nRow]->m_szBlockingSignalTag) != 0)
				{
					auto strTmp = value.toString();
					if (!CheckTagNameIsValid(strTmp, FES_DESC))
					{
						return false;
					}
					memset(m_arrDOs[nRow]->m_szBlockingSignalTag, 0, sizeof(m_arrDOs[nRow]->m_szBlockingSignalTag));
					auto nSize = strTmp.size();
					strncpy(m_arrDOs[nRow]->m_szBlockingSignalTag, strTmp.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
				}
			}
			else if (index.column() == BlockingProgram)
			{
				if (m_arrDOs[nRow]->m_strBlockingProgram != value.toString())
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrDOs[nRow]->m_strBlockingProgram = strDesc;
				}
			}
			else if (index.column() == StartProgram)
			{
				if (m_arrDOs[nRow]->m_strStartProgram != value.toString())
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrDOs[nRow]->m_strStartProgram = strDesc;
				}
			}
			else if (index.column() == ReferenceCount)
			{
				if (m_arrDOs[nRow]->m_nReferenceCount != value.toInt())
				{
					m_arrDOs[nRow]->m_nReferenceCount = value.toInt();
				}
			}
			else if (index.column() == ChannelOccNo)
			{
				if (m_arrDOs[nRow]->m_nChannelOccNo != value.toInt())
				{
					m_arrDOs[nRow]->m_nChannelOccNo = value.toInt();
				}
			}
			else if (index.column() == DeviceOccNo)
			{
				if (m_arrDOs[nRow]->m_nDeviceOccNo != value.toInt())
				{
					m_arrDOs[nRow]->m_nDeviceOccNo = value.toInt();
				}
			}

			emit dataChanged(index, index);

			return true;
		}
	}
		
	return false;
}

void CDOModel::InitPara()
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
	((CDOTable *)parent())->setItemDelegateForColumn(TagName, delegate);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_mapInfo.insert(OccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pDelegateRead = new CReadOnlyDelegate(this);
	((CDOTable *)parent())->setItemDelegateForColumn(OccNo, pDelegateRead);

	para.desc = tr("BlockOccNo");
	para.tagname = "BlockOccNo";
	m_mapInfo.insert(BlockOccNo, para);
	m_lstHorizontalHeader.append(para.desc);

	//para.desc = tr("DataType");
	//para.tagname = "DataType";
	//m_arrInfo.insert(DataType, para);
	//horizontal_header_list.append(para.desc);
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
	//CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, arrVecDataType);
	//((CDOTable *)parent())->setItemDelegateForColumn(DataType, pCombo);

	para.desc = tr("Desc");
	para.tagname = "Desc";
	m_mapInfo.insert(Desc, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pDelegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CDOTable *)parent())->setItemDelegateForColumn(Desc, pDelegateDesc);

	para.desc = tr("InitValue");
	para.tagname = "InitValue";
	m_mapInfo.insert(InitValue, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pCheckDelegate = new CCheckBoxDelegate(this);
	((CDOTable *)parent())->setItemDelegateForColumn(InitValue, pCheckDelegate);

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
	((CDOTable *)parent())->setItemDelegateForColumn(InitQua, pComboInitQua);

	para.desc = tr("Address");
	para.tagname = "Address";
	m_mapInfo.insert(Address, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pDelegateAddress = new CLineEditDelegate(this, LINESTRING, MAX_ADDRESS_LENGTH_SCADASTUDIO);
	((CDOTable *)parent())->setItemDelegateForColumn(Address, pDelegateAddress);

	para.desc = tr("PinLabel");
	para.tagname = "PinLabel";
	m_mapInfo.insert(PinLabel, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDOTable *)parent())->setItemDelegateForColumn(PinLabel, pDelegateDesc);

	para.desc = tr("IsSBO");
	para.tagname = "IsSBO";
	m_mapInfo.insert(IsSBO, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pDelegateCheck = new CCheckBoxDelegate(this);
	((CDOTable *)parent())->setItemDelegateForColumn(IsSBO, pDelegateCheck);

	para.desc = tr("TelCtlWaitTime");
	para.tagname = "TelCtlWaitTime";
	m_mapInfo.insert(TelCtlWaitTime, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("TelCtlSendTime");
	para.tagname = "TelCtlSendTime";
	m_mapInfo.insert(TelCtlSendTime, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("TelCtlExecTime");
	para.tagname = "TelCtlExecTime";
	m_mapInfo.insert(TelCtlExecTime, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("AssociatedDI");
	para.tagname = "AssociatedDI";
	m_mapInfo.insert(AssociatedDI, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDOTable *)parent())->setItemDelegateForColumn(AssociatedDI, pDelegateRead);


	para.desc = tr("AssociatedDIValType");
	para.tagname = "AssociatedDIValType";
	m_mapInfo.insert(AssociatedDIValType, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDOTable *)parent())->setItemDelegateForColumn(AssociatedDIValType, pCheckDelegate);

	para.desc = tr("BlockingSignalTag");
	para.tagname = "BlockingSignalTag";
	m_mapInfo.insert(BlockingSignalTag, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDOTable *)parent())->setItemDelegateForColumn(BlockingSignalTag, pDelegateRead);

	para.desc = tr("BlockingProgram");
	para.tagname = "BlockingProgram";
	m_mapInfo.insert(BlockingProgram, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDOTable *)parent())->setItemDelegateForColumn(BlockingProgram, pDelegateDesc);

	para.desc = tr("StartProgram");
	para.tagname = "StartProgram";
	m_mapInfo.insert(StartProgram, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDOTable *)parent())->setItemDelegateForColumn(StartProgram, pDelegateDesc);

	para.desc = tr("ReferenceCount");
	para.tagname = "ReferenceCount";
	m_mapInfo.insert(ReferenceCount, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("ChannelOccNo");
	para.tagname = "ChannelOccNo";
	m_mapInfo.insert(ChannelOccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDOTable *)parent())->setItemDelegateForColumn(ChannelOccNo, pDelegateRead);

	para.desc = tr("DeviceOccNo");
	para.tagname = "DeviceOccNo";
	m_mapInfo.insert(DeviceOccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDOTable *)parent())->setItemDelegateForColumn(DeviceOccNo, pDelegateRead);
}

/*! \fn QMap<int32s, CONFIG_PARA> CDOModel::GetColumInfo()
********************************************************************************************************* 
** \brief CDOModel::GetColumInfo 
** \details 
** \return QMap<int32s, CONFIG_PARA> 
** \author gw
** \date 2016年5月26日 
** \note 
********************************************************************************************************/
QMap<int32s, CONFIG_PARA> &CDOModel::GetColumnInfo()
{
	return m_mapInfo;
}

bool CDOModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_arrDOs.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{
		auto pDigtalPoint = m_pFes->AddNewDigitalOutput();

		pDigtalPoint->SetOccNo(i + 1);
		pDigtalPoint->m_nID = i + 1;

		pDigtalPoint->m_strAddress = QString("%1").arg(pDigtalPoint->m_nID);

		//auto strNewTagName = QString("DigitalOutput%1").arg(pDigtalPoint->m_nID);
		//auto strNewDesc = QString("DigitalOutput%1").arg(pDigtalPoint->m_nID);

		////是否校验

		////pDigtalPoint->m_szTagName = strNewTagName;
		//memset(pDigtalPoint->m_szTagName, 0, sizeof(pDigtalPoint->m_szTagName));
		//int nSize = strNewTagName.size();
		//strncpy(pDigtalPoint->m_szTagName, strNewTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, nSize));

		//pDigtalPoint->m_strDescription = strNewDesc;
		//pDigtalPoint->m_bNewPointAlarm = true;

		m_arrDOs.push_back(pDigtalPoint);
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
bool CDOModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	//auto nCount = m_arrDOs.size();

	auto pDigital = m_arrDOs[position];

	auto it = std::find(m_arrDOs.begin(), m_arrDOs.end(), pDigital);
	if (it != m_arrDOs.end())
	{
		m_arrDOs.erase(it);
	}

	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}


	//note 删除关联关系
	QString strTmp = pDigital->m_szTagName;
	if (!CheckTagNameIsValid(strTmp, FES_DESC))
	{
		return false;
	}

	if (!m_pFes->DeleteFesHashItem(DOHASHID, strTmp))
	{
		return false;
	}


	//开出量删除，如果用户变量绑定关联的开出量存在，那么关联开出量关系也跟着删除
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


	//前置用户变量Source hash tagname 
	std::string strLastTagName = strNodeTagName + "." + pDigital->m_szTagName;

	if (!m_pFes->DeleteUserVariableSourceTagNameArr(strLastTagName))
	{
		return false;
	}
	
	//前置转发通道sourcetagname关联Do tagname关系
	if (!m_pFes->DeleteForwardingDOSourceTagNameArr(strTmp.toStdString()))
	{
		auto strError = QObject::tr("Delete ForwardingDO SourceTagName %1 hash fail!!!").arg(strTmp);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

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
	pScadaVariableModule->ChangeTagNameNodify(QString(""), QString(strLastTagName.c_str()), Module_ID::DEVICE_DO);

	//删除前置DO，如果前置do AssociatedDI有关联，那么关联关系删除
	if (strlen(pDigital->m_szAssociatedDI) > 0)
	{
		if (!m_pFes->DeleteDOsRelationAssociatedDIArr(pDigital->m_szAssociatedDI, pDigital))
		{
			auto strError = QObject::tr("Delete DOs %1 Relation AssociatedDI %2 hash fail!!!").arg(strTmp).arg(pDigital->m_szAssociatedDI);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}


	//删除前置DO，如果前置do BlockingSignal有关联，那么关联关系删除
	if (strlen(pDigital->m_szBlockingSignalTag) > 0)
	{
		if (!m_pFes->DeleteDOsRelationBlockingSignalTagArr(pDigital->m_szBlockingSignalTag, pDigital))
		{
			auto strError = QObject::tr("Delete DOs %1 Relation AssociatedDI %2 hash fail!!!").arg(strTmp).arg(pDigital->m_szBlockingSignalTag);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}


	delete pDigital;
	pDigital = nullptr;


	endRemoveRows();

	if (m_arrDOs.size() == 0)
	{
		RefrushModel();
	}
	

	return true;
}