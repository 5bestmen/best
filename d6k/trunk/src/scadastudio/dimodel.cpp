#include "dimodel.h"
#include "ditable.h"
#include "delegate.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/icore.h"
#include "global.h"
#include "scadastudio/imodule.h"

CDIModel::CDIModel(QObject *parent, std::vector <Config::CDigitalInput *> &arrDIs, Config::CFesData *pFes, IMainModuleInterface *pCore)
	: QAbstractTableModel(parent), m_arrDIs(arrDIs)
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

CDIModel::~CDIModel()
{

}

int CDIModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_arrDIs.size();
}

int CDIModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_lstHorizontalHeader.size() > 0)
		return m_lstHorizontalHeader.size();

	return 0;
}

QVariant CDIModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == OccNo)
		{
			return uint(m_arrDIs[nRow]->GetOccNo());
		}
		else if (index.column() == TagName)
		{
			return m_arrDIs[nRow]->m_szTagName;
		}
		else if (index.column() == DoubleDI)
		{
			return m_arrDIs[nRow]->m_szDoubleDI;
		}
		else if (index.column() == ID)
		{
			return (uint)m_arrDIs[nRow]->m_nID;
		}
		else if (index.column() == BlockOccNo)
		{
			return (uint)m_arrDIs[nRow]->m_nBlockOccNo;
		}
		//else if (index.column() == DataType)
		//{
		//	return (uint)m_arrDIs[nRow]->m_nDataType;
		//}
		else if (index.column() == Desc)
		{
			return m_arrDIs[nRow]->m_strDescription;
		}
		else if (index.column() == InitValue)
		{
			return m_arrDIs[nRow]->m_bInitValue;
		}
		else if (index.column() == InitQua)
		{
			return m_arrDIs[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_arrDIs[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_arrDIs[nRow]->m_strPinLabel;
		}
		else if (index.column() == SignalType)
		{
			return (uint)m_arrDIs[nRow]->m_nSignalType;
		}
		else if (index.column() == SOE)
		{
			return m_arrDIs[nRow]->m_bSOE;
		}
		else if (index.column() == BitCloseString)
		{
			return m_arrDIs[nRow]->m_strBitCloseString;
		}
		else if (index.column() == BitOpenString)
		{
			return m_arrDIs[nRow]->m_strBitOpenString;
		}
		else if (index.column() == AlarmTagName)
		{
			return m_arrDIs[nRow]->m_strAlarmTag;
		}
		//else if (index.column() == AlarmType)
		//{
		//	if (m_arrDIs[nRow]->m_nAlarmType == AIALARM)
		//	{
		//		return m_mapAlarmType[AIALARM];
		//	}
		//	else if (m_arrDIs[nRow]->m_nAlarmType == DIALARM)
		//	{
		//		return m_mapAlarmType[DIALARM];
		//	}
		//}
		else if (index.column() == SaveDisk)
		{
			return m_arrDIs[nRow]->m_bSaveDisk;
		}
		else if (index.column() == SaveDiskPeriod)
		{
			return (uint)m_arrDIs[nRow]->m_nSaveDiskPeriod;
		}
		else if (index.column() == SaveDB)
		{
			return m_arrDIs[nRow]->m_bSaveDB;
		}
		else if (index.column() == SaveDBPeriod)
		{
			return (uint)m_arrDIs[nRow]->m_nSaveDBPeriod;
		}
		else if (index.column() == ReferenceCount)
		{
			return (uint)m_arrDIs[nRow]->m_nReferenceCount;
		}
		else if (index.column() == DataSource)
		{
			return (uint)m_arrDIs[nRow]->m_nDataSource;
		}
		else if (index.column() == ChannelOccNo)
		{
			return (uint)m_arrDIs[nRow]->m_nChannelOccNo;
		}
		else if (index.column() == DeviceOccNo)
		{
			return (uint)m_arrDIs[nRow]->m_nDeviceOccNo;
		}
		else if (index.column() == Express)
		{
			return m_arrDIs[nRow]->m_strExpress;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == OccNo)
		{
			return uint(m_arrDIs[nRow]->GetOccNo());
		}
		else if (index.column() == TagName)
		{
			return m_arrDIs[nRow]->m_szTagName;
		}
		else if (index.column() == DoubleDI)
		{
			return m_arrDIs[nRow]->m_szDoubleDI;
		}
		else if (index.column() == ID)
		{
			return (uint)m_arrDIs[nRow]->m_nID;
		}
		else if (index.column() == BlockOccNo)
		{
			return (uint)m_arrDIs[nRow]->m_nBlockOccNo;
		}
		//else if (index.column() == DataType)
		//{
		//	return (uint)m_arrDIs[nRow]->m_nDataType;
		//}
		else if (index.column() == Desc)
		{
			return m_arrDIs[nRow]->m_strDescription;
		}
		else if (index.column() == InitValue)
		{
			return m_arrDIs[nRow]->m_bInitValue;
		}
		else if (index.column() == InitQua)
		{
			return m_arrDIs[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_arrDIs[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_arrDIs[nRow]->m_strPinLabel;
		}
		else if (index.column() == SignalType)
		{
			return (uint)m_arrDIs[nRow]->m_nSignalType;
		}
		else if (index.column() == SOE)
		{
			return m_arrDIs[nRow]->m_bSOE;
		}
		else if (index.column() == BitCloseString)
		{
			return m_arrDIs[nRow]->m_strBitCloseString;
		}
		else if (index.column() == BitOpenString)
		{
			return m_arrDIs[nRow]->m_strBitOpenString;
		}
		else if (index.column() == AlarmTagName)
		{
			return m_arrDIs[nRow]->m_strAlarmTag;
		}
		//else if (index.column() == AlarmType)
		//{
		//	return (uint)m_arrDIs[nRow]->m_nAlarmType;
		//}
		else if (index.column() == SaveDisk)
		{
			return m_arrDIs[nRow]->m_bSaveDisk;
		}
		else if (index.column() == SaveDiskPeriod)
		{
			return (uint)m_arrDIs[nRow]->m_nSaveDiskPeriod;
		}
		else if (index.column() == SaveDB)
		{
			return m_arrDIs[nRow]->m_bSaveDB;
		}
		else if (index.column() == SaveDBPeriod)
		{
			return (uint)m_arrDIs[nRow]->m_nSaveDBPeriod;
		}
		else if (index.column() == ReferenceCount)
		{
			return (uint)m_arrDIs[nRow]->m_nReferenceCount;
		}
		else if (index.column() == DataSource)
		{
			return (uint)m_arrDIs[nRow]->m_nDataSource;
		}
		else if (index.column() == ChannelOccNo)
		{
			return (uint)m_arrDIs[nRow]->m_nChannelOccNo;
		}
		else if (index.column() == DeviceOccNo)
		{
			return (uint)m_arrDIs[nRow]->m_nDeviceOccNo;
		}
		else if (index.column() == Express)
		{
			return m_arrDIs[nRow]->m_strExpress;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CDIModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags CDIModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CDIModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			//Q_ASSERT(m_pChannel);

			/*if (index.column() == OccNo)
			{
				if (m_arrDIs[nRow]->m_nOccNo != value.toInt())
				{
					m_arrDIs[nRow]->m_nOccNo = value.toInt();
				}
			}*/
			if (index.column() == TagName)
			{
				if (m_arrDIs[nRow]->m_szTagName != value.toString())
				{
					Q_ASSERT(m_pFes);
					if (!m_pFes)
					{
						return false;
					}
					
					auto strTagName = value.toString();
					//校验tagname长度是否有效
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						return false;
					}

					if (m_pFes->CheckTagNameIsExist(strTagName))
					{
						auto strError = tr("di tagname %1 is exist!!!").arg(strTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					if (!m_pFes->ModifyFesHashMap(DIHASHID, strTagName, m_arrDIs[nRow]->m_szTagName))
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
						return false;
					}
					auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(m_pFes->m_szTagName));
					if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
					{
						return false;
					}

					//Source hash tagname 
					std::string strLastTagName = strNodeTagName + "." + m_arrDIs[nRow]->m_szTagName;
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
					pScadaVariableModule->ChangeTagNameNodify(strSourceTagName, QString(strLastTagName.c_str()), Module_ID::DEVICE_DI);
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


					//开关量修改，转发开关量的source tagname也跟着修改
					bool bFlag = m_pFes->ModifyForwardingDISourceTagNameArr(strTagName.toStdString(), std::string(m_arrDIs[nRow]->m_szTagName));
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = tr("Forwarding di sourcetagname hash %1 modify failed!!!").arg(strLastTagName.c_str());
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}


					//遥信次遥信关联修改
					if (!m_pFes->ModifyDIDoubleDIArr(strTagName.toStdString(), m_arrDIs[nRow]->m_szTagName))
					{
						auto strError = tr("Modify di DoubleDI hash %1 failed!!!").arg(m_arrDIs[nRow]->m_szTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}
					
					//前置DI修改，如果前置DO AssociatedDI关联此遥信，那么关联关系修改 1对多关系
					if (!m_pFes->ModifyDOAssociatedDIArr(strTagName.toStdString(), m_arrDIs[nRow]->m_szTagName))
					{
						auto strError = tr("Modify DO AssociatedDI hash %1 failed!!!").arg(m_arrDIs[nRow]->m_szTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					//前置DI修改，如果前置DO BlockingSignalTag关联此遥信，那么关联关系修改 1对多关系
					if (!m_pFes->ModifyDOBlockingSignalTagArr(strTagName.toStdString(), m_arrDIs[nRow]->m_szTagName))
					{
						auto strError = tr("Modify DO BlockingSignalTag hash %1 failed!!!").arg(m_arrDIs[nRow]->m_szTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					//模拟量删除，如果转发AI source tagname关联该模拟量，那么转发AI source tagname清空
					if (!m_pFes->DeleteForwardingAISourceTagNameArr(std::string(m_arrDIs[nRow]->m_szTagName)))
					{
						auto strError = QObject::tr("Delete Forwarding AI Source TagName %1 hash fail!!!").arg(m_arrDIs[nRow]->m_szTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					memset(m_arrDIs[nRow]->m_szTagName, 0, sizeof(m_arrDIs[nRow]->m_szTagName));
					int nSize = strTagName.size();
					strncpy(m_arrDIs[nRow]->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					//刷新index
					emit dataChanged(index, index);
				}
			}
			else if (index.column() == DoubleDI)
			{
				if (m_arrDIs[nRow]->m_szDoubleDI != value.toString())
				{
					auto strTagName = value.toString();
					//校验tagname长度是否有效
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						return false;
					}
					
					memset(m_arrDIs[nRow]->m_szDoubleDI, 0, sizeof(m_arrDIs[nRow]->m_szDoubleDI));
					int nSize = strTagName.size();
					strncpy(m_arrDIs[nRow]->m_szDoubleDI, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					//刷新index
					emit dataChanged(index, index);
				}
			}
			else if (index.column() == ID)
			{
				if (m_arrDIs[nRow]->m_nID != value.toInt())
				{
					m_arrDIs[nRow]->m_nID = value.toInt();
				}
			}
			else if (index.column() == BlockOccNo)
			{
				if (m_arrDIs[nRow]->m_nBlockOccNo != value.toInt())
				{
					m_arrDIs[nRow]->m_nBlockOccNo = value.toInt();
				}
			}
			//else if (index.column() == DataType)
			//{
			//	if (m_arrDIs[nRow]->m_nDataType != value.toInt())
			//	{
			//		m_arrDIs[nRow]->m_nDataType = value.toInt();
			//	}
			//}
			else if (index.column() == Desc)
			{
				if (m_arrDIs[nRow]->m_strDescription.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrDIs[nRow]->m_strDescription = strDesc;
				}
			}
			else if (index.column() == InitValue)
			{
				if (m_arrDIs[nRow]->m_bInitValue != value.toBool())
				{
					m_arrDIs[nRow]->m_bInitValue = value.toBool();
				}
			}
			else if (index.column() == InitQua)
			{
				if (m_arrDIs[nRow]->m_nInitialQua != value.toInt())
				{
					m_arrDIs[nRow]->m_nInitialQua = value.toInt();
				}
			}
			else if (index.column() == Address)
			{
				if (m_arrDIs[nRow]->m_strAddress.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC, MAX_ADDRESS_LENGTH_SCADASTUDIO))
					{
						return false;
					}
					
					m_arrDIs[nRow]->m_strAddress = strDesc;
				}
			}
			else if (index.column() == PinLabel)
			{
				if (m_arrDIs[nRow]->m_strPinLabel.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrDIs[nRow]->m_strPinLabel = strDesc;
				}
			}
			else if (index.column() == SignalType)
			{
				if (m_arrDIs[nRow]->m_nSignalType != value.toInt())
				{
					m_arrDIs[nRow]->m_nSignalType = value.toInt();
				}
			}
			else if (index.column() == SOE)
			{
				if (m_arrDIs[nRow]->m_bSOE != value.toBool())
				{
					m_arrDIs[nRow]->m_bSOE = value.toBool();
				}
			}
			else if (index.column() == BitCloseString)
			{
				if (m_arrDIs[nRow]->m_strBitCloseString.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrDIs[nRow]->m_strBitCloseString = strDesc;
				}
			}
			else if (index.column() == BitOpenString)
			{
				if (m_arrDIs[nRow]->m_strBitOpenString.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrDIs[nRow]->m_strBitOpenString = strDesc;
				}
			}
			else if (index.column() == AlarmTagName)
			{
				if (m_arrDIs[nRow]->m_strAlarmTag.isEmpty())
				{
					return true;
				}
				
				//删除关联关系
				if (!m_pFes->DeleteDIsRelationAlarmArr(m_arrDIs[nRow]->m_strAlarmTag.toStdString(), m_arrDIs[nRow]))
				{
					auto strTmp = QObject::tr("[Fes %1]  Delete AI TagName %2 Relation Alarm failed!!!").arg(m_pFes->m_szTagName).arg(m_arrDIs[nRow]->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);

					return false;
				}

				m_arrDIs[nRow]->m_strAlarmTag.clear();
			}
			//else if (index.column() == AlarmType)
			//{
			//	if (m_arrDIs[nRow]->m_nAlarmType != value.toInt())
			//	{
			//		m_arrDIs[nRow]->m_nAlarmType = value.toInt();
			//	}
			//}
			else if (index.column() == SaveDisk)
			{
				if (m_arrDIs[nRow]->m_bSaveDisk != value.toBool())
				{
					m_arrDIs[nRow]->m_bSaveDisk = value.toBool();

					emit dataChanged(index, index);
				}
			}
			else if (index.column() == SaveDiskPeriod)
			{
				if (m_arrDIs[nRow]->m_nSaveDiskPeriod != value.toInt())
				{
					m_arrDIs[nRow]->m_nSaveDiskPeriod = value.toInt();
				}
			}
			else if (index.column() == SaveDB)
			{
				if (m_arrDIs[nRow]->m_bSaveDB != value.toBool())
				{
					m_arrDIs[nRow]->m_bSaveDB = value.toBool();

					emit dataChanged(index, index);
				}
			}
			else if (index.column() == SaveDBPeriod)
			{
				if (m_arrDIs[nRow]->m_nSaveDBPeriod != value.toInt())
				{
					m_arrDIs[nRow]->m_nSaveDBPeriod = value.toInt();
				}
			}
			else if (index.column() == ReferenceCount)
			{
				if (m_arrDIs[nRow]->m_nReferenceCount != value.toInt())
				{
					m_arrDIs[nRow]->m_nReferenceCount = value.toInt();
				}
			}
			else if (index.column() == DataSource)
			{
				if (m_arrDIs[nRow]->m_nDataSource != value.toInt())
				{
					//待定
					m_arrDIs[nRow]->m_nDataSource = value.toInt();
				}
			}
			//if (index.column() == ChannelOccNo)
			//{
			//	if (m_arrDIs[nRow]->m_nChannelOccNo != value.toInt())
			//	{
			//		m_arrDIs[nRow]->m_nChannelOccNo = value.toInt();
			//	}
			//}
			//else if (index.column() == DeviceOccNo)
			//{
			//	if (m_arrDIs[nRow]->m_nDeviceOccNo != value.toInt())
			//	{
			//		m_arrDIs[nRow]->m_nDeviceOccNo = value.toInt();
			//	}
			//}
			else if (index.column() == Express)
			{
				if (m_arrDIs[nRow]->m_strExpress.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrDIs[nRow]->m_strExpress = value.toString();
				}
			}

			emit dataChanged(index, index);

			return true;
		}
	}
		
	return false;
}

void CDIModel::InitPara()
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
	((CDITable *)parent())->setItemDelegateForColumn(TagName, delegate);

	para.desc = tr("DoubleDI");
	para.tagname = "DoubleDI";
	m_mapInfo.insert(DoubleDI, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CDITable *)parent())->setItemDelegateForColumn(DoubleDI, pReadOnlyDelegate);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_mapInfo.insert(OccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	//auto pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CDITable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);

	para.desc = tr("BlockOccNo");
	para.tagname = "BlockOccNo";
	m_mapInfo.insert(BlockOccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDITable *)parent())->setItemDelegateForColumn(BlockOccNo, pReadOnlyDelegate);

	////
	//para.desc = tr("DataType");
	//para.tagname = "DataType";
	//m_arrInfo.insert(DataType, para);
	//horizontal_header_list.append(para.desc);
	//QVector<IDDESC> arrVecDataType;
	IDDESC tmp;
	//tmp.desc = tr("Boolean");
	//tmp.ID = QString("%1").arg(VariableDataType::BooleanType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("Char");
	//tmp.ID = QString("%1").arg(VariableDataType::CharType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("Byte");
	//tmp.ID = QString("%1").arg(VariableDataType::ByteType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("Short");
	//tmp.ID = QString("%1").arg(VariableDataType::ShortType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("Word");
	//tmp.ID = QString("%1").arg(VariableDataType::WordType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("Long");
	//tmp.ID = QString("%1").arg(VariableDataType::LongType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("DWord");
	//tmp.ID = QString("%1").arg(VariableDataType::DWordType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("LLong");
	//tmp.ID = QString("%1").arg(VariableDataType::LLongType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("LWord");
	//tmp.ID = QString("%1").arg(VariableDataType::LWordType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("Float");
	//tmp.ID = QString("%1").arg(VariableDataType::FloatType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("Double");
	//tmp.ID = QString("%1").arg(VariableDataType::DoubleType);
	//arrVecDataType.append(tmp);
	//tmp.desc = QStringLiteral("String");
	//tmp.ID = QString("%1").arg(VariableDataType::StringType);
	//arrVecDataType.append(tmp);
	//CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, arrVecDataType);
	//((CDITable *)parent())->setItemDelegateForColumn(DataType, pCombo);

	para.desc = tr("Desc");
	para.tagname = "Desc";
	m_mapInfo.insert(Desc, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pDelegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CDITable *)parent())->setItemDelegateForColumn(Desc, pDelegateDesc);


	para.desc = tr("InitValue");
	para.tagname = "InitValue";
	m_mapInfo.insert(InitValue, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pDelegateCheck = new CCheckBoxDelegate(this);
	((CDITable *)parent())->setItemDelegateForColumn(InitValue, pDelegateCheck);

	para.desc = tr("InitQua");
	para.tagname = "InitQua";
	m_mapInfo.insert(InitQua, para);
	m_lstHorizontalHeader.append(para.desc);
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
	((CDITable *)parent())->setItemDelegateForColumn(InitQua, pComboInitQua);

	para.desc = tr("Address");
	para.tagname = "Address";
	m_mapInfo.insert(Address, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pDelegateAddress = new CLineEditDelegate(this, LINESTRING, MAX_ADDRESS_LENGTH_SCADASTUDIO);
	((CDITable *)parent())->setItemDelegateForColumn(Address, pDelegateAddress);

	para.desc = tr("PinLabel");
	para.tagname = "PinLabel";
	m_mapInfo.insert(PinLabel, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDITable *)parent())->setItemDelegateForColumn(PinLabel, pDelegateDesc);

	para.desc = tr("SignalType");
	para.tagname = "SignalType";
	m_mapInfo.insert(SignalType, para);
	QVector<IDDESC> arrSignalType;
	arrSignalType.clear();
	tmp.desc = tr("NormallyOpen");
	tmp.ID = QString("%1").arg(NormallyOpen);
	arrSignalType.append(tmp);
	tmp.desc = tr("NormallyClosed");
	tmp.ID = QString("%1").arg(NormallyClosed);
	arrSignalType.append(tmp);
	m_lstHorizontalHeader.append(para.desc);
	CComboBoxDelegate *pComboSignalType = new CComboBoxDelegate(this, arrSignalType);
	((CDITable *)parent())->setItemDelegateForColumn(SignalType, pComboSignalType);

	para.desc = tr("SOE");
	para.tagname = "SOE";
	m_mapInfo.insert(SOE, para);
	m_lstHorizontalHeader.append(para.desc);
	pDelegateCheck = new CCheckBoxDelegate(this);
	((CDITable *)parent())->setItemDelegateForColumn(SOE, pDelegateCheck);

	para.desc = tr("BitCloseString");
	para.tagname = "BitCloseString";
	m_mapInfo.insert(BitCloseString, para);
	m_lstHorizontalHeader.append(para.desc);
	delegate = new CLineEditDelegate(this, TAGNAMELINE, MAX_NAME_LENGTH_SCADASTUDIO);
	((CDITable *)parent())->setItemDelegateForColumn(BitCloseString, delegate);

	para.desc = tr("BitOpenString");
	para.tagname = "BitOpenString";
	m_mapInfo.insert(BitOpenString, para);
	m_lstHorizontalHeader.append(para.desc);
	delegate = new CLineEditDelegate(this, TAGNAMELINE, MAX_NAME_LENGTH_SCADASTUDIO);
	((CDITable *)parent())->setItemDelegateForColumn(BitOpenString, delegate);

	para.desc = tr("AlarmTagName");
	para.tagname = "AlarmTagName";
	m_mapInfo.insert(AlarmTagName, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDITable *)parent())->setItemDelegateForColumn(AlarmTagName, pReadOnlyDelegate);

	//para.desc = tr("AlarmType");
	//para.tagname = "AlarmType";
	//m_mapInfo.insert(AlarmType, para);
	//m_lstHorizontalHeader.append(para.desc);
	//m_mapAlarmType.clear();
	//tmp.desc = tr("ai alarm");
	//tmp.ID = AIALARM;		//AIALARM
	//m_mapAlarmType.insert(AIALARM, tmp.desc);
	//tmp.desc = tr("di alarm");
	////tmp.ID = DIALARM;		//DIALARM
	//m_mapAlarmType.insert(DIALARM, tmp.desc);
	//((CDITable *)parent())->setItemDelegateForColumn(AlarmType, pReadOnlyDelegate);
	

	para.desc = tr("SaveDisk");
	para.tagname = "SaveDisk";
	m_mapInfo.insert(SaveDisk, para);
	m_lstHorizontalHeader.append(para.desc);
	pDelegateCheck = new CCheckBoxDelegate(this);
	((CDITable *)parent())->setItemDelegateForColumn(SaveDisk, pDelegateCheck);

	para.desc = tr("SaveDiskPeriod");
	para.tagname = "SaveDiskPeriod";
	m_mapInfo.insert(SaveDiskPeriod, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("SaveDB");
	para.tagname = "SaveDB";
	m_mapInfo.insert(SaveDB, para);
	m_lstHorizontalHeader.append(para.desc);
	pDelegateCheck = new CCheckBoxDelegate(this);
	((CDITable *)parent())->setItemDelegateForColumn(SaveDB, pDelegateCheck);

	para.desc = tr("SaveDBPeriod");
	para.tagname = "SaveDBPeriod";
	m_mapInfo.insert(SaveDBPeriod, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("ReferenceCount");
	para.tagname = "ReferenceCount";
	m_mapInfo.insert(ReferenceCount, para);
	m_lstHorizontalHeader.append(para.desc);

	//待定
	para.desc = tr("DataSource");
	para.tagname = "DataSource";
	m_mapInfo.insert(DataSource, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("ChannelOccNo");
	para.tagname = "ChannelOccNo";
	m_mapInfo.insert(ChannelOccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDITable *)parent())->setItemDelegateForColumn(ChannelOccNo, pReadOnlyDelegate);

	para.desc = tr("DeviceOccNo");
	para.tagname = "DeviceOccNo";
	m_mapInfo.insert(DeviceOccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDITable *)parent())->setItemDelegateForColumn(ChannelOccNo, pReadOnlyDelegate);

	para.desc = tr("Express");
	para.tagname = "Express";
	m_mapInfo.insert(Express, para);
	m_lstHorizontalHeader.append(para.desc);
	((CDITable *)parent())->setItemDelegateForColumn(Express, pDelegateDesc);
}

/*! \fn QMap<int32s, CONFIG_PARA> CDIModel::GetColumInfo()
********************************************************************************************************* 
** \brief CDIModel::GetColumInfo 
** \details 
** \return QMap<int32s, CONFIG_PARA> 
** \author gw
** \date 2016年5月26日 
** \note 
********************************************************************************************************/
QMap<int32s, CONFIG_PARA> &CDIModel::GetColumnInfo()
{
	return m_mapInfo;
}

/*! \fn bool CDIModel::InsertRows(int position, int rows, const QModelIndex &parent)
********************************************************************************************************* 
** \brief CDIModel::InsertRows 
** \details 添加数据
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
bool CDIModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_arrDIs.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{
		auto pDigtalPoint = m_pFes->AddNewDigitalInput();

		pDigtalPoint->SetOccNo(i + 1);
		pDigtalPoint->m_nID = i + 1;

		pDigtalPoint->m_strAddress = QString("%1").arg(pDigtalPoint->m_nID);

		//auto strNewTagName = QString("AnalogInput%1").arg(pDigtalPoint->m_nID);
		//auto strNewDesc = QString("AnalogInput%1").arg(pDigtalPoint->m_nID);

		////是否校验
		//memset(pDigtalPoint->m_szTagName, 0, sizeof(pDigtalPoint->m_szTagName));
		//int nSize = strNewTagName.size();
		//strncpy(pDigtalPoint->m_szTagName, strNewTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, nSize));
		//pDigtalPoint->m_strDescription = strNewDesc;
		//pDigtalPoint->m_bNewPointAlarm = true;

		m_arrDIs.push_back(pDigtalPoint);
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
bool CDIModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	//auto nCount = m_arrDIs.size();

	auto pDigital = m_arrDIs[position];

	auto it = std::find(m_arrDIs.begin(), m_arrDIs.end(), pDigital);
	if (it != m_arrDIs.end())
	{
		m_arrDIs.erase(it);
	}

	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	//note 删除关联关系

	QString strTmp = pDigital->m_szTagName;
	if (!m_pFes->DeleteFesHashItem(DIHASHID, strTmp))
	{
		return false;
	}

	//删除关联组合关系
	if (!m_pFes->DeleteFesDeviceDICombineRelaton(pDigital->m_szTagName, pDigital))
	{
		return false;
	}


	////note
	////模拟量修改，如果用户变量绑定关联的模拟量存在，那么关联模拟量也跟着修改
	////为了方便查找,hash tagname是node tagname + . + 点tagname，在以后修改或者删除的node tagname或者点tagname就可以直接找到，属性tagname不修改
	//auto pNodeModule = m_pCore->GetModule("NODE_CONFIG");
	//Q_ASSERT(pNodeModule);
	//if (!pNodeModule)
	//{
	//	auto strError = tr("Get node module poiter failed!!!");
	//	m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
	//	
	//	return false;
	//}
	//auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(m_pFes->m_szTagName));
	//if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
	//{
	//	return false;
	//}

	////Source hash tagname 
	//std::string strLastTagName = strNodeTagName + "." + pDigital->m_szTagName;
	//if (!m_pFes->DeleteUserVariableSourceTagNameArr(strLastTagName))
	//{
	//	return false;
	//}

	//////////////////////////////////////////////////////通知外部模块scada variable module/////////////////////////////////////////////////////
	////scada variable
	//std::vector<QString>  vec;
	//m_pCore->GetModulesName(vec);
	//auto pScadaVariableModule = m_pCore->GetModule("scada_variable");
	//Q_ASSERT(pScadaVariableModule);
	//if (!pScadaVariableModule)
	//{
	//	auto strError = tr("Get scada variable module poiter failed!!!");
	//	m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

	//	return false;
	//}
	//pScadaVariableModule->ChangeTagNameNodify(QString(""), QString(strLastTagName.c_str()), Module_ID::DEVICE_DI);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////转换DI source tagname如果关联DI tagname，那么删除关联关系
	//m_pFes->DeleteForwardingDISourceTagNameArr(strTmp.toStdString());


	////如果DI alram关联告警，那么关联关系删除
	//if (!pDigital->m_strAlarmTag.isEmpty())
	//{
	//	if (!m_pFes->DeleteDIsRelationAlarmArr(pDigital->m_strAlarmTag.toStdString(), pDigital))
	//	{
	//		auto strError = QObject::tr("Delete DIs %1 Relation Alarm %2 hash fail!!!").arg(pDigital->m_szTagName).arg(pDigital->m_strAlarmTag);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	////如果次遥信有关联，删除关联关系
	//if (strlen(pDigital->m_szDoubleDI) > 0)
	//{
	//	if (!m_pFes->DeleteDIRelationDoubleDIArr(pDigital->m_szDoubleDI, pDigital))
	//	{
	//		auto strError = QObject::tr("Delete DIs %1 Relation DoubleDI %2 hash fail!!!").arg(pDigital->m_szTagName).arg(pDigital->m_szDoubleDI);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	////删除遥信，如果有遥信的次遥信关联该遥信，那么关联关系删除
	//if (!m_pFes->DeleteDIDoubleDIArr(pDigital->m_szTagName, pDigital))
	//{
	//	auto strError = QObject::tr("Delete DIs Relation DoubleDI %1 hash fail!!!").arg(pDigital->m_szTagName);
	//	m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//	return false;
	//}

	////删除遥信，如果fes do AssociatedDI关联该遥信，那么关联关系删除
	//if (!m_pFes->DeleteDOsAssociatedDIArr(pDigital->m_szTagName))
	//{
	//	auto strError = QObject::tr("Delete DOs AssociatedDI %1 hash fail!!!").arg(pDigital->m_szTagName);
	//	m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//	return false;
	//}

	////删除遥信，如果fes do BlockingSignalTag关联该遥信，那么关联关系删除
	////BlockingSignalTag还可以从前置用户变量关联(bool类型)
	//if (!m_pFes->DeleteDOsBlockingSignalTagArr(pDigital->m_szTagName))
	//{
	//	auto strError = QObject::tr("Delete DOs BlockingSignalTag %1 hash fail!!!").arg(pDigital->m_szTagName);
	//	m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//	return false;
	//}

	delete pDigital;
	pDigital = nullptr;

	endRemoveRows();

	if (m_arrDIs.size() == 0)
	{
		RefrushModel();
	}

	return true;
}

