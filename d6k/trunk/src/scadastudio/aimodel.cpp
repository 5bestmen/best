#include "aimodel.h"
#include "delegate.h"
#include "aitable.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "global.h"
#include "scadastudio/imodule.h"


CAIModel::CAIModel(QObject *parent, std::vector <Config::CAnalogInput *> &arrAIs, Config::CFesData *pFes, IMainModuleInterface *pCore)
	: QAbstractTableModel(parent), m_arrAIs(arrAIs), m_pFes(nullptr)
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

CAIModel::~CAIModel()
{

}

void CAIModel::SetHorizontalHeaderList(QStringList list)
{
	if (!m_lstHorizontalHeader.isEmpty())
	{
		m_lstHorizontalHeader.clear();
	}

	m_lstHorizontalHeader = list;
}

void CAIModel::SetVerticalHeaderList(QStringList verticalHeaderList)
{
	//vertical_header_list = verticalHeaderList;
}

int CAIModel::rowCount(const QModelIndex &parent) const
{
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	Q_UNUSED(parent);

	return (int)m_arrAIs.size();
}

int CAIModel::columnCount(const QModelIndex &parent) const
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

	return true;
}

QVariant CAIModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == ID)
		{
			return static_cast<uint>(m_arrAIs[nRow]->m_nID);
		}
		else if (index.column() == TagName)
		{
			return m_arrAIs[nRow]->m_szTagName;
		}
		else if (index.column() == Desc)
		{
			return m_arrAIs[nRow]->m_strDescription;
		}
		else if (index.column() == BlockOccNo)
		{
			return (uint)m_arrAIs[nRow]->m_nBlockOccNo;
		}
		//else if (index.column() == DataType)
		//{
		//	return m_arrAIs[nRow]->m_nDataType;
		//}
		else if (index.column() == InitQua)
		{
			return m_arrAIs[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_arrAIs[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_arrAIs[nRow]->m_strPinLabel;
		}
		else if (index.column() == InitValue)
		{
			return m_arrAIs[nRow]->m_dbInitValue;
		}
		//else if (index.column() == Format)
		//{
		//	return m_arrAIs[nRow]->m_strFormat;
		//}
		else if (index.column() == Unit)
		{
			return m_arrAIs[nRow]->m_strUnit;
		}
		else if (index.column() == Enable)
		{
			return m_arrAIs[nRow]->m_bEnable;
		}
		else if (index.column() == MaxRaw)
		{
			return m_arrAIs[nRow]->m_dbMaxRaw;
		}
		else if (index.column() == MinRaw)
		{
			return m_arrAIs[nRow]->m_dbMinRaw;
		}
		else if (index.column() == MaxConvert)
		{
			return m_arrAIs[nRow]->m_dbMaxConvert;
		}
		else if (index.column() == MinConvert)
		{
			return m_arrAIs[nRow]->m_dbMinConvert;
		}
		else if (index.column() == ScaleTagName)
		{
			return m_arrAIs[nRow]->m_strScaleTagName;
		}
		//else if (index.column() == ScaleType)
		//{
		//	if (m_arrAIs[nRow]->m_nScaleType == ScaleType::LINEAR)
		//	{
		//		return m_mapScaleType[ScaleType::LINEAR];
		//	}
		//	else
		//	{
		//		return m_mapScaleType[ScaleType::NONLINEAR];
		//	}
		//}
		//else if (index.column() == ScaleDesc)
		//{
		//	return m_arrAIs[nRow]->m_strScaleDesc;
		//}
		else if (index.column() == RangeL)
		{
			return m_arrAIs[nRow]->m_dblRangeL;
		}
		else if (index.column() == RangeH)
		{
			return m_arrAIs[nRow]->m_dblRangeH;
		}
		else if (index.column() == HighQua)
		{
			return m_arrAIs[nRow]->m_dblHighQua;
		}
		else if (index.column() == LowQua)
		{
			return m_arrAIs[nRow]->m_dblLowQua;
		}
		else if (index.column() == SaveDisk)
		{
			return m_arrAIs[nRow]->m_bSaveDisk;
		}
		else if (index.column() == SaveDiskPeriod)
		{
			return (uint)m_arrAIs[nRow]->m_nSaveDiskPeriod;
		}
		else if (index.column() == SaveDB)
		{
			return m_arrAIs[nRow]->m_bSaveDB;
		}
		else if (index.column() == SaveDBPeriod)
		{
			return (uint)m_arrAIs[nRow]->m_nSaveDBPeriod;
		}
		else if (index.column() == Sensitivity)
		{
			return m_arrAIs[nRow]->m_dblSensitivity;
		}
		else if (index.column() == SensitivityType)
		{
			return (uint)m_arrAIs[nRow]->m_nSensitivityType;
		}
		else if (index.column() == AlarmTagName)
		{
			return m_arrAIs[nRow]->m_strAlarmTagName;
		}
		//else if (index.column() == AlarmType)
		//{
		//	if (m_arrAIs[nRow]->m_nAlarmType == AlarmType::AIALARM)
		//	{
		//		return  m_mapAlarmType[AlarmType::AIALARM];
		//	}
		//	else if (m_arrAIs[nRow]->m_nAlarmType == AlarmType::DIALARM)
		//	{
		//		return  m_mapAlarmType[AlarmType::DIALARM];
		//	}
		//	
		//	return QVariant();
		//}
		//else if (index.column() == AlarmDesc)
		//{
		//	return m_arrAIs[nRow]->m_strAlarmDesc;
		//}
		else if (index.column() == ChannelOccNo)
		{
			return (uint)m_arrAIs[nRow]->m_nChannelOccNo;
		}
		else if (index.column() == DeviceOccNo)
		{
			return (uint)m_arrAIs[nRow]->m_nDeviceOccNo;
		}

		else if (index.column() == ReferenceCount)
		{
			return (uint)m_arrAIs[nRow]->m_nReferenceCount;
		}
		else if (index.column() == DataSource)
		{
			return uint(m_arrAIs[nRow]->m_nDataSource);
		}
		else if (index.column() == Express)
		{
			return m_arrAIs[nRow]->m_strExpress;
		}
		else if (index.column() == OccNo)
		{
			return uint(m_arrAIs[nRow]->GetOccNo());
		}
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == ID)
		{
			return static_cast<uint>(m_arrAIs[nRow]->m_nID);
		}
		else if (index.column() == TagName)
		{
			return m_arrAIs[nRow]->m_szTagName;
		}
		else if (index.column() == Desc)
		{
			return m_arrAIs[nRow]->m_strDescription;
		}
		else if (index.column() == BlockOccNo)
		{
			return (uint)m_arrAIs[nRow]->m_nBlockOccNo;
		}
		//else if (index.column() == DataType)
		//{
		//	return m_arrAIs[nRow]->m_nDataType;
		//}
		else if (index.column() == InitQua)
		{
			return m_arrAIs[nRow]->m_nInitialQua;
		}
		else if (index.column() == Address)
		{
			return m_arrAIs[nRow]->m_strAddress;
		}
		else if (index.column() == PinLabel)
		{
			return m_arrAIs[nRow]->m_strPinLabel;
		}
		else if (index.column() == InitValue)
		{
			return m_arrAIs[nRow]->m_dbInitValue;
		}
		//else if (index.column() == Format)
		//{
		//	return m_arrAIs[nRow]->m_strFormat;
		//}
		else if (index.column() == Unit)
		{
			return m_arrAIs[nRow]->m_strUnit;
		}
		else if (index.column() == Enable)
		{
			return m_arrAIs[nRow]->m_bEnable;
		}
		else if (index.column() == MaxRaw)
		{
			return m_arrAIs[nRow]->m_dbMaxRaw;
		}
		else if (index.column() == MinRaw)
		{
			return m_arrAIs[nRow]->m_dbMinRaw;
		}
		else if (index.column() == MaxConvert)
		{
			return m_arrAIs[nRow]->m_dbMaxConvert;
		}
		else if (index.column() == MinConvert)
		{
			return m_arrAIs[nRow]->m_dbMinConvert;
		}
		else if (index.column() == ScaleTagName)
		{
			return m_arrAIs[nRow]->m_strScaleTagName;
		}
		//else if (index.column() == ScaleType)
		//{
		//	return (uint)m_arrAIs[nRow]->m_nScaleType;
		//}
		//else if (index.column() == ScaleDesc)
		//{
		//	return m_arrAIs[nRow]->m_strScaleDesc;
		//}
		else if (index.column() == RangeL)
		{
			return m_arrAIs[nRow]->m_dblRangeL;
		}
		else if (index.column() == RangeH)
		{
			return m_arrAIs[nRow]->m_dblRangeH;
		}
		else if (index.column() == HighQua)
		{
			return m_arrAIs[nRow]->m_dblHighQua;
		}
		else if (index.column() == LowQua)
		{
			return m_arrAIs[nRow]->m_dblLowQua;
		}
		else if (index.column() == SaveDisk)
		{
			return m_arrAIs[nRow]->m_bSaveDisk;
		}
		else if (index.column() == SaveDiskPeriod)
		{
			return (uint)m_arrAIs[nRow]->m_nSaveDiskPeriod;
		}
		else if (index.column() == SaveDB)
		{
			return m_arrAIs[nRow]->m_bSaveDB;
		}
		else if (index.column() == SaveDBPeriod)
		{
			return (uint)m_arrAIs[nRow]->m_nSaveDBPeriod;
		}
		else if (index.column() == Sensitivity)
		{
			return m_arrAIs[nRow]->m_dblSensitivity;
		}
		else if (index.column() == SensitivityType)
		{
			return (uint)m_arrAIs[nRow]->m_nSensitivityType;
		}
		else if (index.column() == AlarmTagName)
		{
			return m_arrAIs[nRow]->m_strAlarmTagName;
		}
		//else if (index.column() == AlarmType)
		//{
		//	if (m_arrAIs[nRow]->m_nAlarmType == AlarmType::AIALARM)
		//	{
		//		return  m_mapAlarmType[AlarmType::AIALARM];
		//	}
		//	else if (m_arrAIs[nRow]->m_nAlarmType == AlarmType::DIALARM)
		//	{
		//		return  m_mapAlarmType[AlarmType::DIALARM];
		//	}

		//	return QVariant();
		//}
		//else if (index.column() == AlarmDesc)
		//{
		//	return m_arrAIs[nRow]->m_strAlarmDesc;
		//}
		else if (index.column() == ChannelOccNo)
		{
			return (uint)m_arrAIs[nRow]->m_nChannelOccNo;
		}
		else if (index.column() == DeviceOccNo)
		{
			return (uint)m_arrAIs[nRow]->m_nDeviceOccNo;
		}

		else if (index.column() == ReferenceCount)
		{
			return (uint)m_arrAIs[nRow]->m_nReferenceCount;
		}
		else if (index.column() == DataSource)
		{
			return uint(m_arrAIs[nRow]->m_nDataSource);
		}
		else if (index.column() == Express)
		{
			return m_arrAIs[nRow]->m_strExpress;
		}
		else if (index.column() == OccNo)
		{
			return uint(m_arrAIs[nRow]->GetOccNo());
		}


		return QVariant();
	}

	return QVariant();
}

QVariant CAIModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_lstHorizontalHeader.size() > section && section > -1)
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

Qt::ItemFlags CAIModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CAIModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{	
	int nRow = index.row();
	
	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			//Q_ASSERT(m_pChannel);

			/*if (index.column() == OccNo)
			{
				if (m_arrAIs[nRow]->m_nOccNo != value.toInt())
				{
					m_arrAIs[nRow]->m_nOccNo = value.toInt();
				}
			}*/
			if (index.column() == ID)
			{
				if (m_arrAIs[nRow]->m_nID != value.toInt())
				{
					m_arrAIs[nRow]->m_nID = value.toInt();
				}
			}
			else if (index.column() == TagName)
			{
				if (m_arrAIs[nRow]->m_szTagName != value.toString())
				{
					auto strTagName = value.toString();
					//校验tagname长度是否有效
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						return false;
					}
					
					if (m_pFes->CheckTagNameIsExist(value.toString()))
					{
						Q_ASSERT(m_pCore);
						if (!m_pCore)
						{
							return false;
						}
						
						auto strError = tr("ai tagname %1 is exist!!!").arg(value.toString());
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
					std::string strLastTagName = strNodeTagName + "." + m_arrAIs[nRow]->m_szTagName;
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
					pScadaVariableModule->ChangeTagNameNodify(strSourceTagName, QString(strLastTagName.c_str()), Module_ID::DEVICE_AI);
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//前置转发
					//模拟量tagname修改,前置转发source tagname也跟着修改
					if (!m_pFes->ModifyForwardingAISourceTagNameArr(value.toString().toStdString(), std::string(m_arrAIs[nRow]->m_szTagName)))
					{
						auto strError = tr("Forwarding ai sourcetagname hash %1 modify failed!!!").arg(m_arrAIs[nRow]->m_szTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}
					
					///////////////////////////////////////////////////////scada变量模块 外部模块关联关系修改///////////////////////////////////////////////////////


					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					
					
					auto strLastValue = QString("%1").arg(m_arrAIs[nRow]->m_szTagName);
					if (m_pFes->ModifyFesHashMap(AIHASHID, strTagName, strLastValue))
					{
						memset(m_arrAIs[nRow]->m_szTagName, 0, sizeof(m_arrAIs[nRow]->m_szTagName));
						strncpy(m_arrAIs[nRow]->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));
					}
					else
					{
						auto strError = tr("Ai hash tagname %1 modify failed!!!").arg(value.toString());
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}
				}
			}
			else if (index.column() == Desc)
			{
				if (m_arrAIs[nRow]->m_strDescription.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					//校验描述字符串长度是否有效
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}
					
					m_arrAIs[nRow]->m_strDescription = strDesc;
				}
			}
			else if (index.column() == BlockOccNo)
			{
				if (m_arrAIs[nRow]->m_nBlockOccNo != value.toInt())
				{
					m_arrAIs[nRow]->m_nBlockOccNo = value.toInt();
				}
			}
			//else if (index.column() == DataType)
			//{
			//	if (m_arrAIs[nRow]->m_nDataType != value.toInt())
			//	{
			//		m_arrAIs[nRow]->m_nDataType = value.toInt();
			//	}
			//}
			else if (index.column() == InitQua)
			{
				if (m_arrAIs[nRow]->m_nInitialQua != value.toDouble())
				{
					m_arrAIs[nRow]->m_nInitialQua = value.toDouble();
				}
			}
			else if (index.column() == Address)
			{
				if (m_arrAIs[nRow]->m_strAddress.compare(value.toString()) != 0)
				{
					auto strTmp = value.toString();
					//校验地址字符串长度是否有效
					if (!CheckDescIsValid(strTmp, FES_DESC, MAX_ADDRESS_LENGTH_SCADASTUDIO))
					{
						return false;
					}
					
					m_arrAIs[nRow]->m_strAddress = strTmp;
				}
			}
			else if (index.column() == PinLabel)
			{
				if (m_arrAIs[nRow]->m_strPinLabel.compare(value.toString()) != 0)
				{
					auto strTmp = value.toString();
					//校验端子名称字符串长度是否有效
					if (!CheckDescIsValid(strTmp, FES_DESC))
					{
						return false;
					}
					
					m_arrAIs[nRow]->m_strPinLabel = strTmp;
				}
			}
			else if (index.column() == InitValue)
			{
				if (m_arrAIs[nRow]->m_dbInitValue != value.toDouble())
				{
					m_arrAIs[nRow]->m_dbInitValue = value.toDouble();
				}
			}
			//else if (index.column() == Format)
			//{
			//	if (m_arrAIs[nRow]->m_strFormat != value.toString())
			//	{
			//		m_arrAIs[nRow]->m_strFormat = value.toString();
			//	}
			//}
			else if (index.column() == Unit)
			{
				if (m_arrAIs[nRow]->m_strUnit.compare(value.toString()) != 0)
				{
					auto strTmp = value.toString();
					//校验端子名称字符串长度是否有效
					if (!CheckDescIsValid(strTmp, FES_DESC))
					{
						return false;
					}
					
					m_arrAIs[nRow]->m_strUnit = strTmp;
				}
			}
			else if (index.column() == Enable)
			{
				if (m_arrAIs[nRow]->m_bEnable  != value.toBool())
				{
					m_arrAIs[nRow]->m_bEnable = value.toBool();
				}
			}
			else if (index.column() == MaxRaw)
			{
				if (m_arrAIs[nRow]->m_dbMaxRaw != value.toDouble())
				{
					m_arrAIs[nRow]->m_dbMaxRaw = value.toDouble();
				}
			}
			else if (index.column() == MinRaw)
			{
				if (m_arrAIs[nRow]->m_dbMinRaw != value.toDouble())
				{
					m_arrAIs[nRow]->m_dbMinRaw = value.toDouble();
				}
			}
			else if (index.column() == MaxConvert)
			{
				if (m_arrAIs[nRow]->m_dbMaxRaw != value.toDouble())
				{
					m_arrAIs[nRow]->m_dbMaxRaw = value.toDouble();
				}
			}
			else if (index.column() == MinConvert)
			{
				if (m_arrAIs[nRow]->m_dbMinRaw != value.toDouble())
				{
					m_arrAIs[nRow]->m_dbMinRaw = value.toDouble();
				}
			}
			//else if (index.column() == ScaleType)
			//{
			//	if (m_arrAIs[nRow]->m_nScaleType != value.toInt())
			//	{
			//		m_arrAIs[nRow]->m_nScaleType = value.toInt();
			//	}
			//}
			//else if (index.column() == ScaleDesc)
			//{
			//	if (m_arrAIs[nRow]->m_strScaleTagName != value.toString())
			//	{
			//		m_arrAIs[nRow]->m_strScaleTagName = value.toString();
			//	}
			//}
			else if (index.column() == RangeL)
			{
				if (m_arrAIs[nRow]->m_dblRangeL != value.toDouble())
				{
					m_arrAIs[nRow]->m_dblRangeL = value.toDouble();
				}
			}
			else if (index.column() == RangeH)
			{
				if (m_arrAIs[nRow]->m_dblRangeH != value.toDouble())
				{
					m_arrAIs[nRow]->m_dblRangeH = value.toDouble();
				}
			}
			else if (index.column() == HighQua)
			{
				if (m_arrAIs[nRow]->m_dblHighQua != value.toDouble())
				{
					m_arrAIs[nRow]->m_dblHighQua = value.toDouble();
				}
			}
			else if (index.column() == LowQua)
			{
				if (m_arrAIs[nRow]->m_dblLowQua != value.toDouble())
				{
					m_arrAIs[nRow]->m_dblLowQua = value.toDouble();
				}
			}
			else if (index.column() == SaveDisk)
			{
				if (m_arrAIs[nRow]->m_bSaveDisk != value.toBool())
				{
					m_arrAIs[nRow]->m_bSaveDisk = value.toBool();
				}
			}
			else if (index.column() == SaveDiskPeriod)
			{
				if (m_arrAIs[nRow]->m_nSaveDiskPeriod != value.toInt())
				{
					m_arrAIs[nRow]->m_nSaveDiskPeriod = value.toInt();
				}
			}
			else if (index.column() == SaveDB)
			{
				if (m_arrAIs[nRow]->m_bSaveDB != value.toBool())
				{
					m_arrAIs[nRow]->m_bSaveDB = value.toBool();
				}
			}
			else if (index.column() == SaveDBPeriod)
			{
				if (m_arrAIs[nRow]->m_nSaveDBPeriod != value.toInt())
				{
					m_arrAIs[nRow]->m_nSaveDBPeriod = value.toInt();
				}
			}
			else if (index.column() == Sensitivity)
			{
				if (m_arrAIs[nRow]->m_dblSensitivity != value.toDouble())
				{
					m_arrAIs[nRow]->m_dblSensitivity = value.toDouble();
				}
			}
			else if (index.column() == SensitivityType)
			{
				if (m_arrAIs[nRow]->m_nSensitivityType != value.toInt())
				{
					m_arrAIs[nRow]->m_nSensitivityType = value.toInt();
				}
			}
			//else if (index.column() == AlarmType)
			//{
			//	if (m_arrAIs[nRow]->m_nAlarmType != value.toInt())
			//	{
			//		m_arrAIs[nRow]->m_nAlarmType = value.toInt();
			//	}
			//}
			//else if (index.column() == AlarmDesc)
			//{
			//	if (m_arrAIs[nRow]->m_strAlarmTagName.compare(value.toString()) != 0)
			//	{
			//		m_arrAIs[nRow]->m_strAlarmTagName = value.toString();
			//	}
			//}
			else if (index.column() == ChannelOccNo)
			{
				if (m_arrAIs[nRow]->m_nChannelOccNo != value.toInt())
				{
					m_arrAIs[nRow]->m_nChannelOccNo = value.toInt();
				}
			}
			else if (index.column() == DeviceOccNo)
			{
				if (m_arrAIs[nRow]->m_nDeviceOccNo != value.toInt())
				{
					m_arrAIs[nRow]->m_nDeviceOccNo = value.toInt();
				}
			}
			else if (index.column() == ReferenceCount)
			{
				if (m_arrAIs[nRow]->m_nReferenceCount != value.toInt())
				{
					m_arrAIs[nRow]->m_nReferenceCount = value.toInt();
				}
			}
			else if (index.column() == DataSource)
			{
				if (m_arrAIs[nRow]->m_nDataSource != value.toInt())
				{
					m_arrAIs[nRow]->m_nDataSource = value.toInt();
				}
			}
			else if (index.column() == Express)
			{
				if (m_arrAIs[nRow]->m_strExpress.compare(value.toString()) != 0)
				{
					auto strTmp = value.toString();
					//校验表达式字符串长度是否有效
					if (!CheckDescIsValid(strTmp, FES_DESC))
					{
						return false;
					}
					
					m_arrAIs[nRow]->m_strExpress = strTmp;
				}
			}
			else if (index.column() == AlarmTagName)
			{
				if (m_arrAIs[nRow]->m_strAlarmTagName.isEmpty())
				{
					return true;
				}
				
				//删除关联关系
				if (!m_pFes->DeleteAIsRelationAlarmArr(m_arrAIs[nRow]->m_strAlarmTagName.toStdString(), m_arrAIs[nRow]))
				{
					auto strTmp = QObject::tr("[Fes %1]  Delete AI TagName %2 Relation Alarm failed!!!").arg(m_pFes->m_szTagName).arg(m_arrAIs[nRow]->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);

					return false;
				}

				m_arrAIs[nRow]->m_strAlarmTagName.clear();

				return true;
			}
			else if (index.column() == ScaleTagName)
			{
				if (m_arrAIs[nRow]->m_strScaleTagName.isEmpty())
				{
					return true;
				}
				
				//删除关联关系
				if (!m_pFes->DeleteAIsRelationScaleArr(m_arrAIs[nRow]->m_strScaleTagName.toStdString(), m_arrAIs[nRow]))
				{
					auto strTmp = QObject::tr("[Fes %1]  Delete AI TagName %2 Relation Scale failed!!!").arg(m_pFes->m_szTagName).arg(m_arrAIs[nRow]->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);

					return false;
				}

				m_arrAIs[nRow]->m_strScaleTagName.clear();

				return true;
			}

			return true;
		}
		
		return false;
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		if (index.column() == OccNo)
		{
			//if (m_pChannel->m_nOccNo != value.toInt())
			//{
			//	QSet<int32s> set;

			//	set.insert(index.row());

			//	//值改变,属性栏跟着变
			//	emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);

			//	m_pChannel->m_nOccNo = value.toInt();
			//}



			//m_stack->push(new ChangeValueCommand(index, value, this));

			//emit dataChanged(index, index);
		}
		else if (index.column() == TagName)
		{
			//if (m_pChannel->m_szTagName.compare(value.toString()) != 0)
			//{

			//	m_pChannel->m_szTagName = value.toString();

			//	//TagName
			//	m_pItem->setData(value, TAGNAME);

			//	emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			//}


		}
		else if (index.column() == ID)
		{
			//if (m_pChannel->m_nID = value.toInt())
			//{
			//	m_pChannel->m_nID = value.toInt();

			//	emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			//}
		}
		else if (index.column() == Desc)
		{
			//if (m_pChannel->m_strDescription.compare(value.toString()) != 0)
			//{
			//	m_pChannel->m_strDescription = value.toString();

			//	//通道名
			//	m_pItem->setData(value, Qt::EditRole);

			//	emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			//}
		}
		else if (index.column() == ID)
		{
			//if (m_pChannel->m_CommPara.DriverName.compare(value.toString()) != 0)
			//{
			//	m_pChannel->m_CommPara.DriverName = value.toString();

			//	emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			//}
		}
		//else if (index.column() == DriverPara)
		//{
		//	if (m_pChannel->m_CommPara.DriverParam != value.toInt())
		//	{
		//		m_pChannel->m_CommPara.DriverParam = value.toInt();

		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//	}
		//}
		//else if (index.column() == DriverType)
		//{
		//	if (m_pChannel->m_CommPara.DriveType != value.toInt())
		//	{
		//		m_pChannel->m_CommPara.DriveType = value.toInt();

		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//	}
		//}
		//else if (index.column() == Medium)
		//{
		//	if (m_pChannel->m_CommPara.Medium != value.toInt())
		//	{
		//		m_pChannel->m_CommPara.Medium = value.toInt();

		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//	}
		//}
		//else if (index.column() == DeviceAddress)
		//{
		//	QString strTmp = value.toString();

		//	QStringList list = strTmp.split(":");

		//	if (list.count() == 2)
		//	{
		//		if (m_pChannel->m_CommPara.IP.compare(list[0]) != 0 ||
		//			m_pChannel->m_CommPara.Port != list[1].toInt())
		//		{
		//			m_pChannel->m_CommPara.IP = list[0];

		//			m_pChannel->m_CommPara.Port = list[1].toInt();

		//			emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//		}
		//	}
		//	else
		//	{
		//		//输出错误
		//	}

		//	//QString strTmp = QString("%1").arg(m_pChannel->m_CommPara.Port);

		//	//m_pChannel->m_CommPara.IP + ":" + strTmp;
		//}
		//else if (index.column() == COM)
		//{
		//	if (m_pChannel->m_CommPara.Com.compare(value.toString()) != 0)
		//	{
		//		m_pChannel->m_CommPara.Com = value.toString();

		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//	}
		//}
		//else if (index.column() == Baud)
		//{
		//	if (m_pChannel->m_CommPara.Baud != value.toInt())
		//	{
		//		m_pChannel->m_CommPara.Baud = value.toInt();

		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//	}
		//}
		//else if (index.column() == Extention)
		//{
		//	if (m_pChannel->m_strExtention.compare(value.toString()) != 0)
		//	{
		//		m_pChannel->m_strExtention = value.toString();

		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//	}
		//}
		//else if (index.column() == GroupName)
		//{
		//	m_pChannel->m_strGroupName = value.toString();
		//}
	}
	
	return false;
}

//void CAIModel::sort(int column, Qt::SortOrder order/* = Qt::AscendingOrder */)
//{
//
//}

/*! \fn bool CAIModel::InsertRows(int position, int rows, const QModelIndex &parent)
********************************************************************************************************* 
** \brief CAIModel::InsertRows 
** \details 插入数据
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
bool CAIModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_arrAIs.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pFes);
	if(!m_pFes)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{
		
		auto pAnalogPoint = m_pFes->AddNewAnalogInput();

		pAnalogPoint->SetOccNo(i + 1);
		pAnalogPoint->m_nID = i + 1;
		pAnalogPoint->m_strAddress = QString("%1").arg(pAnalogPoint->m_nID);
		//待定
		pAnalogPoint->m_bNewPointScale = true;
		pAnalogPoint->m_bNewPointAlarm = true;

		m_arrAIs.push_back(pAnalogPoint);
	}

	endInsertRows();

	return true;
}

bool CAIModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	//auto nCount = m_arrAIs.size();

	auto pAnalog = m_arrAIs[position];

	auto it = std::find(m_arrAIs.begin(), m_arrAIs.end(), pAnalog);
	if (it != m_arrAIs.end())
	{
		m_arrAIs.erase(it);
	}


	//note 删除关联关系
	QString strTmp = pAnalog->m_szTagName;
	if (!m_pFes->DeleteFesHashItem(AIHASHID, strTmp))
	{
		return false;
	}

	//删除关联关系组合
	if (!m_pFes->DeleteFesDeviceAICombineRelaton(pAnalog->m_szTagName, pAnalog))
	{
		return false;
	}


	////////////////////////////////////////////////////前置AI删除///////////////////////////////////////////////////////////////////////////////////
	////模拟量删除，如果前置用户变量绑定关联的模拟量存在，那么关联模拟量也跟着删除
	////为了方便查找,hash tagname是node tagname + . + 点tagname，在以后修改或者删除的node tagname或者点tagname就可以直接找到，属性tagname不修改
	//auto pNodeModule = m_pCore->GetModule("NODE_CONFIG");
	//Q_ASSERT(pNodeModule);
	//if (!pNodeModule)
	//{
	//	auto strError = tr("Get node module poiter failed!!!");
	//	m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

	//	return false;
	//}
	////fes tagname转node tagname
	//auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(m_pFes->m_szTagName));
	//if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
	//{
	//	return false;
	//}

	////Source hash tagname 
	//std::string strLastTagName = strNodeTagName + "." + pAnalog->m_szTagName;
	//
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
	//pScadaVariableModule->ChangeTagNameNodify(QString(""), QString(strLastTagName.c_str()), Module_ID::DEVICE_AI);

	////模拟量删除，如果转发AI source tagname关联该模拟量，那么转发AI source tagname清空
	//if (!m_pFes->DeleteForwardingAISourceTagNameArr(std::string(pAnalog->m_szTagName)))
	//{
	//	auto strError = QObject::tr("Delete Forwarding AI Source TagName %1 hash fail!!!").arg(pAnalog->m_szTagName);
	//	m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//	return false;
	//}


	////模拟量删除，如果模拟量Scale关联转换，那么关联关系也删除
	//if (!pAnalog->m_strScaleTagName.isEmpty())
	//{
	//	if (!m_pFes->DeleteAIsRelationScaleArr(pAnalog->m_strScaleTagName.toStdString(), pAnalog))
	//	{
	//		auto strError = QObject::tr("Delete AIs Relation Scale %1 hash fail!!!").arg(pAnalog->m_strScaleTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	////模拟量删除，如果模拟量Alarm关联转换，那么关联关系也删除
	//if (!pAnalog->m_strAlarmTagName.isEmpty())
	//{
	//	if (!m_pFes->DeleteAIsRelationAlarmArr(pAnalog->m_strAlarmTagName.toStdString(), pAnalog))
	//	{
	//		auto strError = QObject::tr("Delete AIs Relation Scale %1 hash fail!!!").arg(pAnalog->m_strAlarmTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	delete pAnalog;
	pAnalog = nullptr;

	endRemoveRows();

	if (m_arrAIs.size() == 0)
	{
		RefrushModel();
	}
	
	return true;
}

void CAIModel::emitDataChanged(const QModelIndex &index)
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
void CAIModel::InitPara()
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
	((CAITable *)parent())->setItemDelegateForColumn(TagName, delegate);


	para.desc = tr("Desc");
	para.tagname = "Desc";
	m_mapInfo.insert(Desc, para);
	m_lstHorizontalHeader.append(para.desc);
	CLineEditDelegate *delegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CAITable *)parent())->setItemDelegateForColumn(Desc, delegateDesc);


	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_mapInfo.insert(OccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	CReadOnlyDelegate *pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CAITable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);


	para.desc = tr("BlockOccNo");
	para.tagname = "BlockOccNo";
	m_mapInfo.insert(BlockOccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAITable *)parent())->setItemDelegateForColumn(BlockOccNo, pReadOnlyDelegate);

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
	//((CAITable *)parent())->setItemDelegateForColumn(DataType, pCombo);

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
	((CAITable *)parent())->setItemDelegateForColumn(InitQua, pComboInitQua);


	para.desc = tr("Address");
	para.tagname = "Address";
	m_mapInfo.insert(Address, para);
	m_lstHorizontalHeader.append(para.desc);
	delegate = new CLineEditDelegate(this, LINESTRING, MAX_ADDRESS_LENGTH_SCADASTUDIO);
	((CAITable *)parent())->setItemDelegateForColumn(Address, delegate);

	para.desc = tr("PinLabel");
	para.tagname = "PinLabel";
	m_mapInfo.insert(PinLabel, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAITable *)parent())->setItemDelegateForColumn(PinLabel, delegateDesc);

	para.desc = tr("InitValue");
	para.tagname = "InitValue";
	m_mapInfo.insert(InitValue, para);
	m_lstHorizontalHeader.append(para.desc);
	delegate = new CLineEditDelegate(this, DOUBLETYPE, 6);
	((CAITable *)parent())->setItemDelegateForColumn(InitValue, delegate);

	////暂时保留 待讨论
	////Format x xx xxx xxxx xxxxx x.x
	//para.desc = tr("Format");
	//para.tagname = "Format";
	//m_mapInfo.insert(Format, para);
	//m_lstHorizontalHeader.append(para.desc);
	//delegate = new CLineEditDelegate(this, LINESTRING, 6);
	//((CAITable *)parent())->setItemDelegateForColumn(Format, delegate);

	para.desc = tr("Unit");
	para.tagname = "Unit";
	m_mapInfo.insert(Unit, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAITable *)parent())->setItemDelegateForColumn(Unit, delegateDesc);


	para.desc = tr("Enable");
	para.tagname = "Enable";
	m_mapInfo.insert(Enable, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pDelegateEnable = new CCheckBoxDelegate(this);
	((CAITable *)parent())->setItemDelegateForColumn(Enable, pDelegateEnable);

	para.desc = tr("MaxRaw");
	para.tagname = "MaxRaw";
	m_mapInfo.insert(MaxRaw, para);
	m_lstHorizontalHeader.append(para.desc);
	delegate = new CLineEditDelegate(this, DOUBLETYPE, 6);
	((CAITable *)parent())->setItemDelegateForColumn(InitValue, delegate);

	para.desc = tr("MinRaw");
	para.tagname = "MinRaw";
	m_mapInfo.insert(MinRaw, para);
	m_lstHorizontalHeader.append(para.desc);
	delegate = new CLineEditDelegate(this, DOUBLETYPE, 6);
	((CAITable *)parent())->setItemDelegateForColumn(InitValue, delegate);

	para.desc = tr("MaxConvert");
	para.tagname = "MaxConvert";
	m_mapInfo.insert(MaxConvert, para);
	m_lstHorizontalHeader.append(para.desc);
	delegate = new CLineEditDelegate(this, DOUBLETYPE, 6);
	((CAITable *)parent())->setItemDelegateForColumn(InitValue, delegate);

	para.desc = tr("MinConvert");
	para.tagname = "MinConvert";
	m_mapInfo.insert(MinConvert, para);
	m_lstHorizontalHeader.append(para.desc);
	delegate = new CLineEditDelegate(this, DOUBLETYPE, 6);
	((CAITable *)parent())->setItemDelegateForColumn(InitValue, delegate);

	para.desc = tr("ScaleTagName");
	para.tagname = "ScaleTagName";
	m_mapInfo.insert(ScaleTagName, para);
	m_lstHorizontalHeader.append(para.desc);
	CReadOnlyDelegate *pReadDelegate = new CReadOnlyDelegate(this);
	((CAITable *)parent())->setItemDelegateForColumn(ScaleTagName, pReadDelegate);

	//para.desc = tr("ScaleType");
	//para.tagname = "ScaleType";
	//m_mapInfo.insert(ScaleType, para);
	//m_lstHorizontalHeader.append(para.desc);
	//m_mapScaleType.clear();
	//tmp.desc = tr("linear");
	////tmp.ID = LINEAR;
	//m_mapScaleType.insert(LINEAR, tmp.desc);
	//tmp.desc = tr("nonlinear");
	////tmp.ID = NONLINEAR;
	//m_mapScaleType.insert(NONLINEAR, tmp.desc);
	//((CAITable *)parent())->setItemDelegateForColumn(ScaleType, pReadDelegate);

	//para.desc = tr("ScaleDesc");
	//para.tagname = "ScaleDesc";
	//m_arrInfo.insert(ScaleDesc, para);
	//horizontal_header_list.append(para.desc);
	//pReadDelegate = new CReadOnlyDelegate(this);
	//((CAITable *)parent())->setItemDelegateForColumn(ScaleDesc, pReadDelegate);

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

	para.desc = tr("SaveDisk");
	para.tagname = "SaveDisk";
	m_mapInfo.insert(SaveDisk, para);
	m_lstHorizontalHeader.append(para.desc);
	CCheckBoxDelegate *pDelegateCheck = new CCheckBoxDelegate(this);
	((CAITable *)parent())->setItemDelegateForColumn(SaveDisk, pDelegateCheck);


	para.desc = tr("SaveDiskPeriod");
	para .tagname = "SaveDiskPeriod";
	m_mapInfo.insert(SaveDiskPeriod, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("SaveDB");
	para.tagname = "SaveDB";
	m_mapInfo.insert(SaveDB, para);
	m_lstHorizontalHeader.append(para.desc);
	pDelegateCheck = new CCheckBoxDelegate(this);
	((CAITable *)parent())->setItemDelegateForColumn(SaveDB, pDelegateCheck);

	para.desc = tr("SaveDBPeriod");
	para.tagname = "SaveDBPeriod";
	m_mapInfo.insert(SaveDBPeriod, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("Sensitivity");
	para.tagname = "Sensitivity";
	m_mapInfo.insert(Sensitivity, para);
	m_lstHorizontalHeader.append(para.desc);
	delegate = new CLineEditDelegate(this, DOUBLETYPE, 6);
	((CAITable *)parent())->setItemDelegateForColumn(InitValue, delegate);

	para.desc = tr("SensitivityType");
	para.tagname = "SensitivityType";
	m_mapInfo.insert(SensitivityType, para);
	m_lstHorizontalHeader.append(para.desc);
	QVector<IDDESC> arrVecSensitivityType;
	tmp.desc = tr("abs");
	tmp.ID = "0";
	arrVecSensitivityType.append(tmp);
	tmp.desc = tr("percent");
	tmp.ID = "1";
	arrVecSensitivityType.append(tmp);
	auto pCombo = new CComboBoxDelegate(this, arrVecSensitivityType);
	((CAITable *)parent())->setItemDelegateForColumn(SensitivityType, pCombo);

	para.desc = tr("AlarmTagName");
	para.tagname = "AlarmTagName";
	m_mapInfo.insert(AlarmTagName, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAITable *)parent())->setItemDelegateForColumn(AlarmTagName, pReadOnlyDelegate);

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
	//((CAITable *)parent())->setItemDelegateForColumn(AlarmType, pReadOnlyDelegate);

	//para.desc = tr("AlarmDesc");
	//para.tagname = "AlarmDesc";
	//m_arrInfo.insert(AlarmDesc, para);
	//horizontal_header_list.append(para.desc);
	//((CAITable *)parent())->setItemDelegateForColumn(AlarmDesc, pReadDelegate);

	para.desc = tr("ChannelOccNo");
	para.tagname = "ChannelOccNo";
	m_mapInfo.insert(ChannelOccNo, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("DeviceOccNo");
	para.tagname = "DeviceOccNo";
	m_mapInfo.insert(DeviceOccNo, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("ReferenceCount");
	para.tagname = "ReferenceCount";
	m_mapInfo.insert(ReferenceCount, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("DataSource");
	para.tagname = "DataSource";
	m_mapInfo.insert(DataSource, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("Express");
	para.tagname = "Express";
	m_mapInfo.insert(Express, para);
	m_lstHorizontalHeader.append(para.desc);
	((CAITable *)parent())->setItemDelegateForColumn(Express, delegateDesc);
	

	

	

	


	//para.desc = tr("GroupName");
	//para.tagname = "GroupName";
	//m_arrInfo.insert(GroupName, para);
	//horizontal_header_list.append(para.desc);















	//horizontal_header_list.append(QStringLiteral("大排行号"));
	//horizontal_header_list.append(QStringLiteral("代码"));
	//horizontal_header_list.append(QStringLiteral("ID"));
	//horizontal_header_list.append(QStringLiteral("描述"));
	//horizontal_header_list.append(QStringLiteral("通道大排行号"));
	//horizontal_header_list.append(QStringLiteral("装置大排行号"));
	//horizontal_header_list.append(QStringLiteral("板内点号"));
	//horizontal_header_list.append(QStringLiteral("点地址"));
	//horizontal_header_list.append(QStringLiteral("初始值"));
	//horizontal_header_list.append(QStringLiteral("初始品质"));
	//horizontal_header_list.append(QStringLiteral("大排行号"));
	//horizontal_header_list.append(QStringLiteral("数据来源"));
	//horizontal_header_list.append(QStringLiteral("转换类型"));
	//horizontal_header_list.append(QStringLiteral("转换大排行号"));
	//horizontal_header_list.append(QStringLiteral("告警大排行号"));
	//horizontal_header_list.append(QStringLiteral("有限范围低"));
	//horizontal_header_list.append(QStringLiteral("有限范围高"));
	//horizontal_header_list.append(QStringLiteral("品质高"));
	//horizontal_header_list.append(QStringLiteral("品质低"));
	//horizontal_header_list.append(QStringLiteral("单位"));
	//horizontal_header_list.append(QStringLiteral("表达式"));
	//horizontal_header_list.append(QStringLiteral("端子名称"));
	//horizontal_header_list.append(QStringLiteral("数据类型"));
	//horizontal_header_list.append(QStringLiteral("是否存盘"));
	//horizontal_header_list.append(QStringLiteral("存盘周期"));
	//horizontal_header_list.append(QStringLiteral("是否存库"));
	//horizontal_header_list.append(QStringLiteral("存库周期"));
	//horizontal_header_list.append(QStringLiteral("变化灵敏度"));
	//horizontal_header_list.append(QStringLiteral("引用次数"));


	SetHorizontalHeaderList(m_lstHorizontalHeader);
}

QMap<int32s, CONFIG_PARA> & CAIModel::GetColumnInfo()
{
	return m_mapInfo;
}

void CAIModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}