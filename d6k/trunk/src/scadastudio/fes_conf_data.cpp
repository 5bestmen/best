/*! @file
<PRE>
********************************************************************************
ģ����       :  ���ýṹ
�ļ���       :  base_data.cpp
�ļ�ʵ�ֹ��� :  �������ýṹ
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date	2016.10.31
*/

#include "fes_conf_data.h"
#include "feschannel_forwardingchannel_data.h"

#include "scadastudiodefine.h"
#include "scadastudio/icore.h"
#include "scadastudio/iprojmgr.h"

#include "stl_util-inl.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "variant.h"
#include "fesapi/fescnst.h"
#include "../fespub/tag_att.h"

#include "global.h"
#include "scadastudio/imodule.h"
#include "log/log2file.h"

#include <algorithm>
#include <QFile>
#include <QXmlStreamReader>
#include <QModelIndex>
#include <QXmlStreamWriter>
#include <QDomElement>
#include <QObject>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>

namespace Config
{	 


	CFesData::CFesData(IMainModuleInterface *pCore) :m_pCore(nullptr), m_pChannelGroup(nullptr), m_pLinearTransformGroup(nullptr), m_pNonLinearTransformGroup(nullptr)
		, m_pAnalogGroup(nullptr), m_pDigitalGroup(nullptr), m_pUserVariableGroup(nullptr), m_pForwardingChannelGroup(nullptr)
	{
		m_strDescription = "test";
		m_strExtention = "test";
		//tagname�����ʼ��
		memset(m_szTagName, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		//sizeof(pData->m_szTagName)
		m_nID = 0;

		m_arrSystemVariable.clear();

		Q_ASSERT(pCore);
		if (!pCore)
		{
			return;
		}

		m_pCore = pCore;

		m_pChannelGroup = new CChannelGroup();
		m_pLinearTransformGroup = new CTransformGroup;
		m_pNonLinearTransformGroup = new CTransformGroup;
		m_pAnalogGroup = new CAnalogAlarmGroup;
		m_pDigitalGroup = new CDigitalAlarmGroup;
		m_pUserVariableGroup = new CUserVariableGroup;

		m_pForwardingChannelGroup = new CForwardingChannelGroup;

		for (int32u i = 0; i < FES_HASH_TYPE_NUM::MAX_IDD_NUM; i++)
		{
			m_arrMaps[i].clear();
		}

		m_arrScaleToAIMap.clear();
		m_arrAlarmToAIMap.clear();
		m_arrAlarmToDIMap.clear();

		m_arrScaleToUserVariableMap.clear();
		m_arrAlarmToUserVariableMap.clear();
		m_arrSourceTagNameToUserVariableMap.clear();

		m_arrScaleToForwardingAIMap.clear();
		m_arrScaleToForwardingAOMap.clear();

		//DI������ң��
		m_arrDIToDoubleDIMap.clear();

		//����ң�ŵ���
		m_arrDOToAssociatedDIMap.clear();
		m_arrDOToBlockingSignalTagMap.clear();

		m_arrAIToForwardingAIMap.clear();
		m_arrDIToForwardingDIMap.clear();
		m_arrAOToForwardingAOMap.clear();
		m_arrDOToForwardingDOMap.clear();

		InitSystemVariable();
	}

	CFesData::~CFesData()
	{
		Clear();
	}

	/*! \fn void CFesData::InitSystemVariable()
	********************************************************************************************************* 
	** \brief Config::CFesData::InitSystemVariable 
	** \details ϵͳ���������ǳ��򶨵�
	** \return void 
	** \author gw
	** \date 2017��2��14�� 
	** \note 
	********************************************************************************************************/
	void CFesData::InitSystemVariable()
	{
		m_arrSystemVariable.clear();
		auto *pTmp = new Config::CSystemVariable;
		pTmp->m_nID = 1;
		memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
		QString strTmp = "$YEAR";
		int nSize = strTmp.size();
		strncpy(pTmp->m_szTagName, strTmp.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		pTmp->m_strDescription = QObject::tr("system-year");
		pTmp->m_nType = DATA_TYPE::DT_WORD;
		if (this->CheckTagNameIsExist(pTmp->m_szTagName))
		{
			auto strError = QObject::tr("system variable tagname %1 is exist!!!").arg(pTmp->m_szTagName);
			s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		else
		{
			if (!this->PushFesTagNameHashMap(SYSTEMVARIABLEID, pTmp->m_szTagName, pTmp))
			{
				auto strError = QObject::tr("system variable init error!!!");
				s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			this->m_arrSystemVariable.push_back(pTmp);
		}



		pTmp = new Config::CSystemVariable;
		pTmp->m_nID = 2;
		memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
		strTmp = "$MONTH";
		nSize = strTmp.size();
		strncpy(pTmp->m_szTagName, strTmp.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		pTmp->m_strDescription = QObject::tr("system-month");
		pTmp->m_nType = DATA_TYPE::DT_WORD;
		if (this->CheckTagNameIsExist(pTmp->m_szTagName))
		{
			auto strError = QObject::tr("system variable tagname %1 is exist!!!").arg(pTmp->m_szTagName);
			s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		else
		{
			if (!this->PushFesTagNameHashMap(SYSTEMVARIABLEID, pTmp->m_szTagName, pTmp))
			{
				auto strError = QObject::tr("system variable init error!!!");
				s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			this->m_arrSystemVariable.push_back(pTmp);
		}


		pTmp = new Config::CSystemVariable;
		pTmp->m_nID = 3;
		memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
		strTmp = "$DAY";
		nSize = strTmp.size();
		strncpy(pTmp->m_szTagName, strTmp.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		pTmp->m_strDescription = QObject::tr("system-day");
		pTmp->m_nType = DATA_TYPE::DT_WORD;
		if (this->CheckTagNameIsExist(pTmp->m_szTagName))
		{
			auto strError = QObject::tr("system variable tagname %1 is exist!!!").arg(pTmp->m_szTagName);
			s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		else
		{
			if (!this->PushFesTagNameHashMap(SYSTEMVARIABLEID, pTmp->m_szTagName, pTmp))
			{
				auto strError = QObject::tr("system variable init error!!!");
				s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			this->m_arrSystemVariable.push_back(pTmp);
		}
	}

	void CFesData::Save()
	{
		//�ӿ� �����ǰ������
		//



		//�ӿ� ����ͨ������
	}

	void CFesData::Clear()
	{
		if (m_pChannelGroup)
		{
			delete m_pChannelGroup;
			m_pChannelGroup = nullptr;
		}


		if (m_pLinearTransformGroup)
		{
			delete m_pLinearTransformGroup;
			m_pLinearTransformGroup = nullptr;
		}


		if (m_pNonLinearTransformGroup)
		{
			delete m_pNonLinearTransformGroup;
			m_pNonLinearTransformGroup = nullptr;
		}


		if (m_pAnalogGroup)
		{
			delete m_pAnalogGroup;
			m_pAnalogGroup = nullptr;
		}


		if (m_pDigitalGroup)
		{
			delete m_pDigitalGroup;
			m_pDigitalGroup = nullptr;
		}

		//for (size_t i = 0; i < m_arrDIAlarms.size(); i++)
		//{
		//	delete m_arrDIAlarms[i];
		//}
		//m_arrDIAlarms.clear();

		STLDeleteElements(&m_arrSystemVariable);
		m_arrSystemVariable.clear();

		if (m_pUserVariableGroup)
		{
			delete m_pUserVariableGroup;
			m_pUserVariableGroup = nullptr;
		}

		if (m_pForwardingChannelGroup)
		{
			delete m_pForwardingChannelGroup;
			m_pForwardingChannelGroup = nullptr;
		}


		for (int32u i = 0; i < FES_HASH_TYPE_NUM::MAX_IDD_NUM; i++)
		{
			m_arrMaps[i].clear();
		}

		m_arrScaleToAIMap.clear();
		m_arrAlarmToAIMap.clear();
		m_arrAlarmToDIMap.clear();
	}

	/*! \fn void CFesData::LoadFesData(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::LoadFesData 
	** \details ����ǰ������
	** \param reader 
	** \return void 
	** \author gw
	** \date 2016��9��1�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::LoadFesData(QXmlStreamReader &reader)
	{
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "channel")
				{
					ReadChannel(reader, m_pChannelGroup);
				}
				else if (strTmp == "scale")
				{
					ReadTransform(reader);
				}
				else if (strTmp == "alarm")
				{
					ReadAlarm(reader);
				}
				else if (strTmp == "variable")
				{
					ReadVariable(reader);
				}
				else if (strTmp == "forwardingchannel")
				{
					ReadForwardingChannel(reader, m_pForwardingChannelGroup);
				}
			}
			else if (reader.isEndElement() && strTmp == "fes")
			{
				break;
			}
		}

		return true;
	}

	bool CFesData::ReadChannel(QXmlStreamReader &reader, Config::CChannelGroup *pChannelGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "c")
				{
					ReadC(reader, pChannelGroup);
				}
				else if (strTmp == "group")
				{
					Config::CChannelGroup *pChannelGroupChild = new Config::CChannelGroup;
					pChannelGroupChild->m_strGroup = reader.attributes().value("name").toString();

					pChannelGroup->m_arrChannelGroup.insert(std::make_pair(pChannelGroupChild->m_strGroup.toStdString(), pChannelGroupChild));
					
					//��group������group
					reader.readNext();
					ReadChannelGroupNode(reader, pChannelGroupChild);
				}
				else if (strTmp == "channel")
				{
					//read channel next node group
					//reader.readNext();
				}
			}
			else if (reader.isEndElement())
			{
				if (strTmp == "channel")
				{
					return true;
				}
			}

			reader.readNext();
		}
		//while (!reader.atEnd())
		//{
		//	QString strTmp = reader.name().toString();
		//	if (reader.isStartElement())
		//	{
		//		QString strTmp = reader.name().toString();

		//		if (strTmp == "c")
		//		{
		//			ReadC(reader, pChannelGroup);
		//		}
		//		else if (strTmp == "group")
		//		{
		//			Config::CChannelGroup *pChannelGroupChild = new Config::CChannelGroup;
		//			pChannelGroupChild->m_strGroup = reader.attributes().value("name").toString();
		//			
		//			//group read next
		//			reader.readNext();

		//			ReadChannel(reader, pChannelGroup);
		//		}
		//	}
		//	else if (reader.isEndElement())
		//	{
		//		break;
		//	}

		//	reader.readNext();
		//}

		return true;
	}

	bool CFesData::ReadChannelGroupNode(QXmlStreamReader &reader, Config::CChannelGroup *pChannelGroup)
	{	
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();
				if (strTmp == "c")
				{
					ReadC(reader, pChannelGroup);
				}
				else if (strTmp == "group")
				{
					Config::CChannelGroup *pChannelChildGroup = new Config::CChannelGroup;
					pChannelChildGroup->m_strGroup = reader.attributes().value("name").toString();

					pChannelGroup->m_arrChannelGroup.insert(std::make_pair(pChannelChildGroup->m_strGroup.toStdString(), pChannelChildGroup));

					//��group������group
					reader.readNext();

					ReadChannelGroupNode(reader, pChannelChildGroup);
					
					continue;
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}


	/*! \fn void ReadC(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::ReadC 
	** \details 
	** \param reader 
	** \return void 
	** \author gw
	** \date 2016��9��1�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadC(QXmlStreamReader &reader, Config::CChannelGroup *pChannelGroup)
	{
		using namespace Config;
		while (!reader.atEnd())
		{	
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				if (strTmp == "c")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						/*auto strError = QObject::tr("channel OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);*/

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						//auto strError = QObject::tr("Tag name is empty!!!");

						//s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CChannelData *pChannel = new CChannelData();

					pChannel->SetOccNo(nOccNo);

					memset(pChannel->m_szTagName, 0, sizeof(pChannel->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pChannel->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pChannel->m_nID = reader.attributes().value("ID").toUInt();

					QString strDescription = reader.attributes().value("Description").toString();
					pChannel->m_strDescription = strDescription;

					QString strDriverName = reader.attributes().value("DriverName").toString();
					pChannel->m_CommPara.DriverName = reader.attributes().value("DriverName").toString();

					pChannel->m_CommPara.m_strDriverParam = reader.attributes().value("DriverPara").toString();

					//int32u nDriverType = reader.attributes().value("DriverType").toInt();
					pChannel->m_CommPara.DriveType = reader.attributes().value("DriverType").toInt();

					//int8u nMedium = reader.attributes().value("Medium").toInt();
					pChannel->m_CommPara.Medium = reader.attributes().value("Medium").toInt();

					//QString strIP = reader.attributes().value("IP").toString();
					//pChannel->m_CommPara.IP = reader.attributes().value("IP").toString();

					//int32u nPort = reader.attributes().value("Port").toInt();
					//pChannel->m_CommPara.Port = reader.attributes().value("Port").toInt();

					//QString strCom = reader.attributes().value("COM").toString();
					//pChannel->m_CommPara.Com = reader.attributes().value("COM").toString();

					//int32u nBaud = reader.attributes().value("Baud").toInt();
					//pChannel->m_CommPara.Baud = reader.attributes().value("Baud").toInt();

					QString strChannelPara = reader.attributes().value("ChannelPara").toString();
					pChannel->m_strChannelPara = strChannelPara;

					QString strExtend = reader.attributes().value("Extention").toString();
					pChannel->m_strExtention = reader.attributes().value("Extention").toString();

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pChannel->m_strGroupName = reader.attributes().value("GroupName").toString();

					//m_vecChannel.push_back(pChannel);

					auto bTest = CHECK_LESS_ZERO_INT(nID);
					if (!bTest)
					{
						//auto strFesError = QString(QObject::tr("ID %1 of channel %2 is invalid < 0")).arg(nID).arg(strTagName);
						//m_pCore->LogMsg(CHANNEL_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						//auto strFesError = QString(QObject::tr("channel %1 is empty")).arg(strTagName);
						//m_pCore->LogMsg(CHANNEL_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					else if (strTagName.count() > MAX_TAGNAME_LEN_SCADASTUDIO)
					{
						//auto strFesError = QString(QObject::tr("channel %1 size is more than %2 ")).arg(strTagName).arg(MAX_NAME_LENGTH_SCADASTUDIO);
						//m_pCore->LogMsg(CHANNEL_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					if (!CheckTagNameIsExist(pChannel->m_szTagName))
					{
			
						pChannel->LoadDevicessData(reader, this);

						//���ݽṹ
						pChannelGroup->m_arrChannelItem.insert(std::make_pair(pChannel->m_szTagName, pChannel));

						//ǰ��ͳһ����ͨ��hash
						PushFesTagNameHashMap(CHANNELHASHID, pChannel->m_szTagName, pChannel);
					}
					else
					{
						reader.readNext();

						return false;
					}

				}
			}
			else if (reader.isEndElement())
			{
				QString strTmp = reader.name().toString();
				if (strTmp == "c")
				{
					break;
				}
			}

			reader.readNext();

		}

		return true;
	}

	/*! \fn bool CFesData::ReadTransform(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadTransform 
	** \details ��ȡת������
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��9��1�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadTransform(QXmlStreamReader &reader)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "linear")
				{
					Q_ASSERT(m_pLinearTransformGroup);
					if (!m_pLinearTransformGroup)
					{
						return false;
					}

					ReadLinear(reader, m_pLinearTransformGroup);
				}
				else if (strTmp == "nonlinear")
				{
					Q_ASSERT(m_pNonLinearTransformGroup);
					if (!m_pNonLinearTransformGroup)
					{
						return false;
					}
					
					ReadNonlinear(reader, m_pNonLinearTransformGroup);
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CFesData::ReadLinear(QXmlStreamReader &reader, CTransformGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadLinear 
	** \details ��ȡ����ת������
	** \param reader 
	** \param pGroup ǰ��ת������
	** \return bool 
	** \author gw
	** \date 2016��9��1�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadLinear(QXmlStreamReader &reader, CTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "l")
				{
					ReadL(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CTransformGroup *pGroupChild = pGroup->Find(strTmp);
					if (!pGroupChild)
					{
						pGroupChild = new Config::CTransformGroup;
						pGroupChild->SetGroupName(reader.attributes().value("name").toString());

						pGroup->AddTransformGroup(pGroupChild);
					}


					//��group������group
					reader.readNext();

					ReadLinearGroupNode(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement() && strTmp == "linear")
			{
				break;
			}
		}

		return true;
	}

	bool CFesData::ReadL(QXmlStreamReader &reader, CTransformGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}
		
		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "l")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("linear OccNo is invalid!!!");
						m_pCore->LogMsg(LINEAR_SCALE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("linear Tag name is empty!!!");
						m_pCore->LogMsg(LINEAR_SCALE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CLinearTransform *pLinearTransform = new CLinearTransform();

					//pLinearTransform->SetOccNo(nOccNo);
					memset(pLinearTransform->m_szTagName, 0, sizeof(pLinearTransform->m_szTagName));
					strncpy(pLinearTransform->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));

					int32u nID = reader.attributes().value("ID").toUInt();
					pLinearTransform->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pLinearTransform->m_strDescription = strDescription;

					fp64 dbMaxRaw = reader.attributes().value("MaxRaw").toDouble();
					pLinearTransform->m_dbMaxRaw = dbMaxRaw;

					fp64 dbMinRaw = reader.attributes().value("MinRaw").toDouble();
					pLinearTransform->m_dbMinRaw = dbMinRaw;

					fp64 dbMaxScale = reader.attributes().value("MaxScale").toDouble();
					pLinearTransform->m_dbMaxScale = dbMaxScale;

					fp64 dbMinScale = reader.attributes().value("MinScale").toDouble();
					pLinearTransform->m_dbMinScale = dbMinScale;

					int8u nInverse = reader.attributes().value("Inverse").toInt();
					pLinearTransform->m_bInverseFactor = nInverse;

					int8u nEnaleFactor = reader.attributes().value("EnaleFactor").toInt();
					pLinearTransform->m_bEnableFactor = nEnaleFactor;

					fp64 dbGain = reader.attributes().value("Gain").toDouble();
					pLinearTransform->m_dbGain = dbGain;

					fp64 dbOffset = reader.attributes().value("Offset").toDouble();
					pLinearTransform->m_dbOffset = dbOffset;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pLinearTransform->m_strGroupName = strGroupName;

					pLinearTransform->m_nType = reader.attributes().value("ScaleType").toInt();


					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of linear scale %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(LINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					CTransform *pTmp = new CTransform;
					pTmp->SetOccNo(nOccNo);

					pTmp->m_nType = pLinearTransform->m_nType;
					memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
					auto nSize = strlen(pLinearTransform->m_szTagName);
					strncpy(pTmp->m_szTagName, pLinearTransform->m_szTagName, qMin(MAX_TAGNAME_LEN_SCADASTUDIO, (int)nSize));

					pTmp->SetLinear(pLinearTransform);

					//��ӵ�ǰ��ת��hash��
					PushFesTagNameHashMap(HASHID::TRANSFORMHASHID, strTagName, pTmp);
					pGroup->AddTransformItem(pTmp);
				}

				reader.readNext();
			}
			else if (reader.isEndElement())
			{
				break;
			}
		}

		return true;
	}


	/*! \fn bool CFesData::ReadLinearGroupNode(QXmlStreamReader &reader, CTransformGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadLinearGroupNode 
	** \details ������ת����
	** \param reader 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016��10��31�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadLinearGroupNode(QXmlStreamReader &reader, CTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();
				if (strTmp == "l")
				{
					ReadL(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					auto strTmp = reader.attributes().value("name").toString();
					auto *pTransformChildGroup = pGroup->Find(strTmp);
					if (!pTransformChildGroup)
					{
						pTransformChildGroup = new CTransformGroup;
						pTransformChildGroup->SetGroupName(reader.attributes().value("name").toString());

						pGroup->AddTransformGroup(pTransformChildGroup);
					}


					//��group������group
					reader.readNext();

					ReadLinearGroupNode(reader, pTransformChildGroup);

					continue;
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CFesData::ReadNonlinear(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadNonlinear 
	** \details ��ȡ������ת������
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadNonlinear(QXmlStreamReader &reader, CTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "nl")
				{
					ReadNl(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					auto strTmp = reader.attributes().value("name").toString();
					CTransformGroup *pChildGroup = pGroup->Find(strTmp);
					if (!pChildGroup)
					{
						pChildGroup = new CTransformGroup;
						pChildGroup->SetGroupName(reader.attributes().value("name").toString());
						pGroup->AddTransformGroup(pChildGroup);
					}
					
					
					reader.readNext();

					ReadNonLinearGroupNode(reader, pChildGroup);
				}
			}
			else if (reader.isEndElement() && strTmp == "nonlinear")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}


	/*! \fn bool CFesData::ReadNonLinearGroupNode(QXmlStreamReader &reader, CTransformGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadNonLinearGroupNode 
	** \details ��������ת��
	** \param reader 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016��10��31�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadNonLinearGroupNode(QXmlStreamReader &reader, CTransformGroup *pGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();
				if (strTmp == "nl")
				{
					ReadNl(reader, pGroup);
				}
				else if (strTmp == "group")
				{
					auto strTmp = reader.attributes().value("name").toString();
					auto *pTransformChildGroup = pGroup->Find(strTmp);
					if (!pTransformChildGroup)
					{
						pTransformChildGroup = new CTransformGroup;
						pTransformChildGroup->SetGroupName(reader.attributes().value("name").toString());

						pGroup->AddTransformGroup(pTransformChildGroup);
					}

					//��group������group
					reader.readNext();

					ReadNonLinearGroupNode(reader, pTransformChildGroup);

					continue;
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CFesData::ReadNl(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadNl 
	** \details 
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadNl(QXmlStreamReader &reader, CTransformGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}
		
		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "nl")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("nl OccNo is invalid!!!");
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						auto strError = QObject::tr("Tag name is empty!!!");
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CNonlinearTransform *pNonliear = new CNonlinearTransform();

					pNonliear->SetOccNo(nOccNo);
					memset(pNonliear->m_szTagName, 0, sizeof(pNonliear->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pNonliear->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pNonliear->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pNonliear->m_strDescription = strDescription;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pNonliear->m_strGroupName = strGroupName;

					pNonliear->m_nType = reader.attributes().value("ScaleType").toInt();

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of nonlinear scale %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(NONLINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					Readnl(reader, pNonliear);

					CTransform *pTmp = new CTransform;

					pTmp->m_nType = pNonliear->m_nType;
					memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
					nSize = sizeof(pNonliear->m_szTagName);
					strncpy(pTmp->m_szTagName, pNonliear->m_szTagName, qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					pTmp->SetNonLinear(pNonliear);
					pTmp->SetOccNo(nOccNo);

					PushFesTagNameHashMap(HASHID::TRANSFORMHASHID, strTagName, pTmp);
					pGroup->AddTransformItem(pTmp);
				}

				strTmp = reader.name().toString();
			}
			else if (reader.isEndElement())
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	
	/*! \fn bool CFesData::Readnl(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::Readnl 
	** \details 
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::Readnl(QXmlStreamReader &reader, Config::CNonlinearTransform *pData)
	{
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "p")
				{
					ReadP(reader, pData);
				}
			}
			else if (reader.isEndElement())
			{
				//reader.skipCurrentElement();

				break;
			}
		}

		return true;
	}

	
	/*! \fn bool CFesData::ReadP(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadP 
	** \details 
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadP(QXmlStreamReader &reader, Config::CNonlinearTransform *pData)
	{
		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "p")
				{
					NONLINEAR_SCALE *pNonScale = new NONLINEAR_SCALE;

					fp64 dbMaxRaw = reader.attributes().value("RawValue").toDouble();
					pNonScale->RawValue = dbMaxRaw;

					fp64 dbMaxScale = reader.attributes().value("ScaleValue").toDouble();
					pNonScale->ScaleValue = dbMaxScale;

					pData->m_arrPNonliear.push_back(pNonScale);
				}

				reader.readNext();
			}
			else if (reader.isEndElement())
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	/*! \fn bool CFesData::ReadAlarm(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadAlarm 
	** \details ��ȡ�澯����
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadAlarm(QXmlStreamReader &reader)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "ain_alarm")
				{
					Q_ASSERT(m_pAnalogGroup);
					if (!m_pAnalogGroup)
					{
						continue;
					}
					ReadAINAlarm(reader, m_pAnalogGroup);
				}
				else if (strTmp == "din_alarm")
				{
					Q_ASSERT(m_pDigitalGroup);
					if (!m_pDigitalGroup)
					{
						continue;
					}
					ReadDINAlarm(reader, m_pDigitalGroup);

					//if (reader.isEndElement() && "din_alarm" == reader.name().toString())
					//{
					//	return true;
					//}
				}
			}
			else if (reader.isEndElement() && strTmp == "alarm")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}


	/*! \fn bool CFesData::ReadAnalogGroupNode(QXmlStreamReader &reader, CAnalogGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadAnalogGroupNode 
	** \details ��ģ�����澯��
	** \param reader 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016��11��9�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadAnalogGroupNode(QXmlStreamReader &reader, CAnalogAlarmGroup *pGroup)
	{
		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "ain_alarm_node")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ain alarm OccNo is invalid!!!");
						m_pCore->LogMsg(AI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("ain alarm Tag name is empty!!!");
						m_pCore->LogMsg(AI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CAnalogAlarm *pAlarm = new CAnalogAlarm;
					//ģ��������
					pAlarm->SetOccNo(nOccNo);
					memset(pAlarm->m_szTagName, 0, sizeof(pAlarm->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pAlarm->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pAlarm->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pAlarm->m_strDescription = strDescription;

					int8u nEnable = reader.attributes().value("Enable").toInt();
					pAlarm->m_bEnable = nEnable;

					int8u nOnQualityGood = reader.attributes().value("OnQualityGood").toUInt();
					pAlarm->m_bOnQualityGood = nOnQualityGood;

					int8u nDeadType = reader.attributes().value("DeadType").toUInt();
					pAlarm->m_nDeadType = nDeadType;

					fp64 dbDeadArea = reader.attributes().value("DeadArea").toDouble();
					pAlarm->m_dbDeadArea = dbDeadArea;

					auto nAlarmType = reader.attributes().value("AlarmType").toInt();
					pAlarm->m_nAlarmType = nAlarmType;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pAlarm->m_strGroupName = strGroupName;

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of ai alarm %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(AI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString(QObject::tr("ai alarm %1 is empty")).arg(strTagName);
						m_pCore->LogMsg(AI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					else if (strTagName.count() > MAX_NAME_LENGTH_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("ai alarm %1 size is more than %2 ")).arg(strTagName).arg(MAX_NAME_LENGTH_SCADASTUDIO);
						m_pCore->LogMsg(AI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					ReadAa(reader, pAlarm);
					pGroup->AddAnalogItem(pAlarm);
					PushFesTagNameHashMap(HASHID::ANALOGALARMHASHID, strTagName, pAlarm);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CAnalogAlarmGroup *pGroupChild = new Config::CAnalogAlarmGroup;

					pGroupChild->SetGroupName(reader.attributes().value("name").toString());

					pGroup->AddAnalogGroup(pGroupChild);


					//��group������group
					reader.readNext();

					ReadAnalogGroupNode(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CFesData::ReadAINAlarm(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadAINAlarm 
	** \details 
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadAINAlarm(QXmlStreamReader &reader, CAnalogAlarmGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}
		
		using namespace Config;

		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "ain_alarm_node")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ain alarm OccNo is invalid!!!");
							m_pCore->LogMsg(AI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						auto strError = QObject::tr("ain alarm Tag name is empty!!!");
						m_pCore->LogMsg(AI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CAnalogAlarm *pAlarm = new CAnalogAlarm;
					//ģ��������
					pAlarm->SetOccNo(nOccNo);
					memset(pAlarm->m_szTagName, 0, sizeof(pAlarm->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pAlarm->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pAlarm->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pAlarm->m_strDescription = strDescription;

					int8u nEnable = reader.attributes().value("Enable").toInt();
					pAlarm->m_bEnable = nEnable;

					int8u nOnQualityGood = reader.attributes().value("OnQualityGood").toUInt();
					pAlarm->m_bOnQualityGood = nOnQualityGood;

					int8u nDeadType = reader.attributes().value("DeadType").toUInt();
					pAlarm->m_nDeadType = nDeadType;

					fp64 dbDeadArea = reader.attributes().value("DeadArea").toDouble();
					pAlarm->m_dbDeadArea = dbDeadArea;

					auto nAlarmType = reader.attributes().value("AlarmType").toInt();
					pAlarm->m_nAlarmType = nAlarmType;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pAlarm->m_strGroupName = strGroupName;

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of ai alarm %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(AI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					ReadAa(reader, pAlarm);

					pGroup->AddAnalogItem(pAlarm);
					PushFesTagNameHashMap(HASHID::ANALOGALARMHASHID, strTagName, pAlarm);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CAnalogAlarmGroup *pGroupChild = new Config::CAnalogAlarmGroup;

					pGroupChild->SetGroupName(reader.attributes().value("name").toString());

					pGroup->AddAnalogGroup(pGroupChild);


					//��group������group
					reader.readNext();

					ReadAnalogGroupNode(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement() && strTmp == "ain_alarm")
			{
				break;
			}
		}

		return true;
	}

	/*! \fn bool CFesData::ReadAa(QXmlStreamReader &reader, Config::CAnalogAlarm *pAlarm)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadAa 
	** \details 
	** \param reader 
	** \param pAlarm 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadAa(QXmlStreamReader &reader, Config::CAnalogAlarm *pAlarm)
	{
		Q_ASSERT(pAlarm);
		if (!pAlarm)
		{
			return false;
		}

		while (!reader.atEnd())
		{
			reader.readNext();

			if (reader.isStartElement() && reader.name() == "aa")
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "aa")
				{
					Readaa(reader, pAlarm);
				}
			}
			else if (reader.isEndElement() && reader.name() == "ain_alarm_node")
			{
				break;
			}
		}

		return true;
	}

	/*! \fn bool CFesData::Readaa(QXmlStreamReader &reader, Config::CAnalogAlarm *pAlarm)
	********************************************************************************************************* 
	** \brief Config::CFesData::Readaa 
	** \details 
	** \param reader 
	** \param pAlarm 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::Readaa(QXmlStreamReader &reader, Config::CAnalogAlarm *pAlarm)
	{
		Q_ASSERT(pAlarm);
		if (!pAlarm)
		{
			return false;
		}
		
		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "aa")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ain alarm offlimit OccNo is invalid!!!");
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					int32u nBlockNo = reader.attributes().value("BlockNo").toInt();
					Q_ASSERT(nBlockNo >= MIN_BLOCKNO_SCADASTUDIO && nBlockNo <= MAX_BLOCKNO_SCADASTUDIO);
					if (nBlockNo <= MIN_BLOCKNO_SCADASTUDIO || nBlockNo > MAX_BLOCKNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ain alarm offlimit BlockNo is invalid!!!");
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						auto strError = QObject::tr("ain alarm offlimit Tag name %1 is invalid!!!").arg(strTagName);
						m_pCore->LogMsg(AI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CAinAlarmLimit *pAlarmOfflimit = new CAinAlarmLimit;
					pAlarmOfflimit->SetOccNo(nOccNo);
					memset(pAlarmOfflimit->m_szTagName, 0, sizeof(pAlarmOfflimit->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pAlarmOfflimit->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					QString strDescription = reader.attributes().value("Description").toString();
					pAlarmOfflimit->m_strDescription = strDescription;

					int32u nID = reader.attributes().value("ID").toUInt();
					pAlarmOfflimit->m_nID = nID;

					

					//QString strTimeFormat = reader.attributes().value("TimeFormat").toString();
					//pAlarmOfflimit->TimeFormat = strTimeFormat;

					int8u nCondition = reader.attributes().value("Condition").toUInt();
					pAlarmOfflimit->m_nCondition = nCondition;

					pAlarmOfflimit->m_nCategory = reader.attributes().value("Category").toUInt();
					
					fp64 dbValue = reader.attributes().value("Value").toDouble();
					pAlarmOfflimit->m_dbValue = dbValue;

					fp64 dbLowValue = reader.attributes().value("LowValue").toDouble();
					pAlarmOfflimit->m_dbLowValue = dbLowValue;

					int32u nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();
					pAlarmOfflimit->m_nDelayAlarm = nDelayAlarm;

					pAlarmOfflimit->m_nPriority = reader.attributes().value("Priority").toUInt();
					
					pAlarmOfflimit->m_bSupportAck = reader.attributes().value("SupportAck").toUInt();
					pAlarmOfflimit->m_bSupportDelete = reader.attributes().value("SupportDelete").toUInt();

					pAlarmOfflimit->m_nAckType = reader.attributes().value("AckType").toInt();

					QString strSoundFile = reader.attributes().value("SoundFile").toString();
					pAlarmOfflimit->m_strSoundFile = strSoundFile;
					int32u nPlaySoundTimes = reader.attributes().value("PlaySoundTimes").toUInt();
					pAlarmOfflimit->m_nPlaySoundTimes = nPlaySoundTimes;

					pAlarmOfflimit->m_strSpeechAlarmText = reader.attributes().value("SpeechAlarmText").toString();
					
					pAlarmOfflimit->m_bBeep = reader.attributes().value("Beep").toUInt();

					pAlarmOfflimit->m_strPushGraph = reader.attributes().value("PushGraph").toString();

					pAlarmOfflimit->m_bBlinkGraph = reader.attributes().value("BlinkGraph").toInt();

					pAlarmOfflimit->m_bLog = reader.attributes().value("Log").toInt();

					pAlarmOfflimit->m_nBackColor = reader.attributes().value("BackColor").toInt();
					pAlarmOfflimit->m_nTextColor = reader.attributes().value("TextColor").toInt();
					pAlarmOfflimit->m_nBlinkBackColor = reader.attributes().value("BlinkBackColor").toInt();
					pAlarmOfflimit->m_nBlinkTextColor = reader.attributes().value("BlinkTextColor").toInt();

					pAlarmOfflimit->m_bLightBoard = reader.attributes().value("LightBoard").toInt();
					pAlarmOfflimit->m_strDispGuide = reader.attributes().value("DispGuide").toString();
					pAlarmOfflimit->m_strCommands = reader.attributes().value("Commands").toString();

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of ai alarm limit %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(AI_ALARM_LIMIT_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					pAlarm->m_arrAlarmOfflimit.push_back(pAlarmOfflimit);
					PushFesTagNameHashMap(HASHID::ANALOGALARMLIMITHASHID, strTagName, pAlarmOfflimit);
				}

				reader.readNext();
			}
			else if (reader.isEndElement() && strTmp == "aa")
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}


	/*! \fn bool CFesData::ReadDigitalGroupNode(QXmlStreamReader &reader, CAnalogGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadDigitalGroupNode 
	** \details ����������
	** \param reader 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016��11��9�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadDigitalGroupNode(QXmlStreamReader &reader, CDigitalAlarmGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}
		
		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement() && reader.name() == "din_alarm_node")
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "din_alarm_node")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("di alarm OccNo is invalid!!!");
						m_pCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("di alarm Tag name is empty!!!");
						m_pCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CDigitalAlarm *pDigitalAlarm = new CDigitalAlarm;

					//����������
					pDigitalAlarm->SetOccNo(nOccNo);
					memset(pDigitalAlarm->m_szTagName, 0, sizeof(pDigitalAlarm->m_szTagName));
					strncpy(pDigitalAlarm->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigitalAlarm->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pDigitalAlarm->m_strDescription = strDescription;

					int8u nEnable = reader.attributes().value("Enable").toInt();
					pDigitalAlarm->m_bEnable = nEnable;

					int8u nOnQualityGood = reader.attributes().value("OnQualityGood").toUInt();
					pDigitalAlarm->m_bOnQualityGood = nOnQualityGood;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pDigitalAlarm->m_strGroupName = strGroupName;

					pDigitalAlarm->m_nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();

					auto nAlarmType = reader.attributes().value("AlarmType").toInt();
					pDigitalAlarm->m_nAlarmType = nAlarmType;

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of di alarm %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString(QObject::tr("di alarm %1 is empty")).arg(strTagName);
						m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.count() > MAX_NAME_LENGTH_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("di alarm %1 is more than %2")).arg(strTagName);
						m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					ReadDa(reader, pDigitalAlarm);
					pGroup->AddDigitalItem(pDigitalAlarm);
					PushFesTagNameHashMap(HASHID::DIGITALALARMHASHID, strTagName, pDigitalAlarm);
					//m_arrDIAlarms.push_back(pDigitalAlarm);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CDigitalAlarmGroup *pGroupChild = new Config::CDigitalAlarmGroup;

					pGroupChild->SetGroupName(reader.attributes().value("name").toString());

					pGroup->AddDigitalGroup(pGroupChild);


					//��group������group
					reader.readNext();

					ReadDigitalGroupNode(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}


	/*! \fn bool CFesData::ReadDINAlarm(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadDINAlarm 
	** \details ��ȡ�������澯����
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadDINAlarm(QXmlStreamReader &reader, CDigitalAlarmGroup *pGroup)
	{
		using namespace Config;
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "din_alarm_node")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("di alarm OccNo is invalid!!!");
						m_pCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						auto strError = QObject::tr("di alarm Tag name %1 is invalid!!!").arg(strTagName);
						m_pCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CDigitalAlarm *pDigitalAlarm = new CDigitalAlarm;

					//����������
					pDigitalAlarm->SetOccNo(nOccNo);
					memset(pDigitalAlarm->m_szTagName, 0, sizeof(pDigitalAlarm->m_szTagName));
					strncpy(pDigitalAlarm->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigitalAlarm->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pDigitalAlarm->m_strDescription = strDescription;

					int8u nEnable = reader.attributes().value("Enable").toInt();
					pDigitalAlarm->m_bEnable = nEnable;

					int8u nOnQualityGood = reader.attributes().value("OnQualityGood").toUInt();
					pDigitalAlarm->m_bOnQualityGood = nOnQualityGood;

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pDigitalAlarm->m_strGroupName = strGroupName;

					pDigitalAlarm->m_nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();

					auto nAlarmType = reader.attributes().value("AlarmType").toInt();
					pDigitalAlarm->m_nAlarmType = nAlarmType;

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of di alarm %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					ReadDa(reader, pDigitalAlarm);

					pGroup->AddDigitalItem(pDigitalAlarm);
					PushFesTagNameHashMap(HASHID::DIGITALALARMHASHID, strTagName, pDigitalAlarm);
					//m_arrDIAlarms.push_back(pDigitalAlarm);
				}
				else if (strTmp == "group")
				{
					Q_ASSERT(pGroup);
					if (!pGroup)
					{
						return false;
					}
					auto strTmp = reader.attributes().value("name").toString();
					Config::CDigitalAlarmGroup *pGroupChild = new Config::CDigitalAlarmGroup;

					pGroupChild->SetGroupName(reader.attributes().value("name").toString());

					pGroup->AddDigitalGroup(pGroupChild);


					//��group������group
					reader.readNext();

					ReadDigitalGroupNode(reader, pGroupChild);
				}
			}
			else if (reader.isEndElement() && strTmp == "din_alarm")
			{
				break;
			}
		}

		return true;
	}

	/*! \fn bool CFesData::ReadDa(QXmlStreamReader &reader, Config::CDigtalAlarm *pDigital)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadDa 
	** \details 
	** \param reader 
	** \param pDigital 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadDa(QXmlStreamReader &reader, Config::CDigitalAlarm *pDigital)
	{
		Q_ASSERT(pDigital);
		if (!pDigital)
		{
			return false;
		}
		
		while (!reader.atEnd())
		{
			reader.readNext();

			if (reader.isStartElement() && reader.name() == "da")
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "da")
				{
					Readda(reader, pDigital);
				}
			}
			else if (reader.isEndElement() && reader.name() == "din_alarm_node")
			{
				break;
			}
		}

		return true;
	}

	/*! \fn bool CFesData::Readda(QXmlStreamReader &reader, Config::CDigtalAlarm *pDigital)
	********************************************************************************************************* 
	** \brief Config::CFesData::Readda 
	** \details 
	** \param reader 
	** \param pDigital 
	** \return bool 
	** \author gw
	** \date 2016��9��2�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::Readda(QXmlStreamReader &reader, Config::CDigitalAlarm *pDigital)
	{
		Q_ASSERT(pDigital);
		if (!pDigital)
		{
			return false;
		}
		
		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "da")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("da OccNo is invalid!!!");
						m_pCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					int32u nBlockNo = reader.attributes().value("BlockNo").toInt();
					Q_ASSERT(nBlockNo >= MIN_BLOCKNO_SCADASTUDIO && nBlockNo <= MAX_BLOCKNO_SCADASTUDIO);
					if (nBlockNo <= MIN_BLOCKNO_SCADASTUDIO || nBlockNo > MAX_BLOCKNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("da BlockNo is invalid!!!");
						m_pCore->LogMsg(DI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (strTagName.isEmpty() == true)
					{
						auto strError = QObject::tr("da Tag name is empty!!!");
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CDinAlarmLimit *pDigitalOfflimit = new CDinAlarmLimit;
					pDigitalOfflimit->OccNo = nOccNo;
					memset(pDigitalOfflimit->TagName, 0, sizeof(pDigitalOfflimit->TagName));
					int nSize = strTagName.size();
					strncpy(pDigitalOfflimit->TagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigitalOfflimit->ID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pDigitalOfflimit->Description = strDescription;

					int8u nCondition = reader.attributes().value("Condition").toUInt();
					pDigitalOfflimit->Condition = nCondition;

					pDigitalOfflimit->Category = reader.attributes().value("Category").toUInt();

					int32u nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();
					pDigitalOfflimit->DelayAlarm = nDelayAlarm;

					pDigitalOfflimit->Priority = reader.attributes().value("Priority").toUInt();

					int8u nSupportAck = reader.attributes().value("SupportAck").toUInt();
					pDigitalOfflimit->SupportAck = nSupportAck;

					int8u nSupportDelete = reader.attributes().value("SupportDelete").toUInt();
					pDigitalOfflimit->SupportDelete = nSupportDelete;

					pDigitalOfflimit->AckType = reader.attributes().value("AckType").toUInt();

					QString strSoundFile = reader.attributes().value("SoundFile").toString();
					pDigitalOfflimit->SoundFile = strSoundFile;

					int32u nPlaySoundTimes = reader.attributes().value("PlaySoundTimes").toUInt();
					pDigitalOfflimit->PlaySoundTimes = nPlaySoundTimes;

					pDigitalOfflimit->SpeechAlarmText = reader.attributes().value("SpeechAlarmText").toString();
					
					pDigitalOfflimit->Beep = reader.attributes().value("Beep").toInt();
					pDigitalOfflimit->PushGraph = reader.attributes().value("PushGraph").toString();
					pDigitalOfflimit->BlinkGraph = reader.attributes().value("BlinkGraph").toInt();
					pDigitalOfflimit->Log = reader.attributes().value("Log").toInt();

					pDigitalOfflimit->BackColor = reader.attributes().value("BackColor").toInt();
					pDigitalOfflimit->TextColor = reader.attributes().value("TextColor").toInt();
					pDigitalOfflimit->BlinkBackColor = reader.attributes().value("BlinkBackColor").toInt();
					pDigitalOfflimit->BlinkTextColor = reader.attributes().value("BlinkTextColor").toInt();

					pDigitalOfflimit->DispGuide = reader.attributes().value("DispGuide").toString();

					QString strCommands = reader.attributes().value("Commands").toString();
					pDigitalOfflimit->Commands = strCommands;

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of di alarm limit %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					pDigital->m_arrDigtalOfflimit.push_back(pDigitalOfflimit);
					PushFesTagNameHashMap(HASHID::DIGITALALARMLIMITHASHID, strTagName, pDigitalOfflimit);
				}

				reader.readNext();
			}
			else if (reader.isEndElement() && strTmp == "da")
			{
				break;
			}
			else
			{
				reader.readNext();
			}
		}
		
		return true;
	}

	/*! \fn bool CFesData::ReadForwardingChannel(QXmlStreamReader & reader, Config::CChannelGroup * pChannelGroup)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadForwardingChannel 
	** \details ��ת��ͨ������
	** \param reader 
	** \param pChannelGroup 
	** \return bool 
	** \author gw
	** \date 2017��4��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadForwardingChannel(QXmlStreamReader & reader, Config::CForwardingChannelGroup *pChannelGroup)
	{
		Q_ASSERT(pChannelGroup);
		if (!pChannelGroup)
		{
			return false;
		}
		
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "forward_c")
				{
					ReadForwardingC(reader, pChannelGroup);
				}
				else if (strTmp == "group")
				{
					Config::CForwardingChannelGroup *pChannelGroupChild = new Config::CForwardingChannelGroup;
					pChannelGroupChild->m_strGroupName = reader.attributes().value("name").toString();

					pChannelGroup->m_arrGroup.insert(std::make_pair(pChannelGroupChild->m_strGroupName.toStdString(), pChannelGroupChild));

					//��group������group
					reader.readNext();
					ReadForwardingChannelGroupNode(reader, pChannelGroupChild);
				}
				else if (strTmp == "forwardingchannel")
				{
					//read channel next node group
					//reader.readNext();
				}
			}
			else if (reader.isEndElement() && strTmp == "forwardingchannel")
			{
				if (strTmp == "forwardingchannel")
				{
					return true;
				}
			}

			reader.readNext();
		}

		return true;
	}

	bool CFesData::ReadForwardingC(QXmlStreamReader & reader, Config::CForwardingChannelGroup * pChannelGroup)
	{
		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				if (strTmp == "forward_c")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
					 /*auto strError = QObject::tr("channel OccNo is invalid!!!");
					 s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);*/

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, FES_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						//auto strError = QObject::tr("Tag name is empty!!!");

						//s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CForwardingChannelData *pChannel = new CForwardingChannelData();

					pChannel->SetOccNo(nOccNo);

					memset(pChannel->m_szTagName, 0, sizeof(pChannel->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pChannel->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pChannel->m_nID = reader.attributes().value("ID").toUInt();

					QString strDescription = reader.attributes().value("Description").toString();
					pChannel->m_strDescription = strDescription;

					QString strDriverName = reader.attributes().value("DriverName").toString();
					pChannel->m_CommPara.DriverName = reader.attributes().value("DriverName").toString();

					pChannel->m_CommPara.m_strDriverParam = reader.attributes().value("DriverPara").toString();

					//int32u nDriverType = reader.attributes().value("DriverType").toInt();
					pChannel->m_CommPara.DriveType = reader.attributes().value("DriverType").toInt();

					//int8u nMedium = reader.attributes().value("Medium").toInt();
					pChannel->m_CommPara.Medium = reader.attributes().value("Medium").toInt();

					//QString strIP = reader.attributes().value("IP").toString();
					//pChannel->m_CommPara.IP = reader.attributes().value("IP").toString();

					//int32u nPort = reader.attributes().value("Port").toInt();
					//pChannel->m_CommPara.Port = reader.attributes().value("Port").toInt();

					//QString strCom = reader.attributes().value("COM").toString();
					//pChannel->m_CommPara.Com = reader.attributes().value("COM").toString();

					//int32u nBaud = reader.attributes().value("Baud").toInt();
					//pChannel->m_CommPara.Baud = reader.attributes().value("Baud").toInt();

					QString strChannelPara = reader.attributes().value("ChannelPara").toString();
					pChannel->m_strChannelPara = strChannelPara;

					QString strExtend = reader.attributes().value("Extention").toString();
					pChannel->m_strExtention = reader.attributes().value("Extention").toString();

					QString strGroupName = reader.attributes().value("GroupName").toString();
					pChannel->m_strGroupName = reader.attributes().value("GroupName").toString();

					//m_vecChannel.push_back(pChannel);

					auto bTest = CHECK_LESS_ZERO_INT(nID);
					if (!bTest)
					{
						//auto strFesError = QString(QObject::tr("ID %1 of channel %2 is invalid < 0")).arg(nID).arg(strTagName);
						//m_pCore->LogMsg(CHANNEL_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					if (!CheckTagNameIsExist(pChannel->m_szTagName))
					{

						pChannel->LoadData(reader, this);

						//���ݽṹ
						pChannelGroup->m_arrItem.insert(std::make_pair(pChannel->m_szTagName, pChannel));

						//ǰ��ͳһ����ͨ��hash
						PushFesTagNameHashMap(FORWARDINGCHANNEL, pChannel->m_szTagName, pChannel);
					}
					else
					{
						reader.readNext();

						return false;
					}

				}
			}
			else if (reader.isEndElement())
			{
				QString strTmp = reader.name().toString();
				if (strTmp == "forward_c")
				{
					break;
				}
			}
			else
			{
				reader.readNext();
			}
		}

		return true;
	}

	bool CFesData::ReadForwardingChannelGroupNode(QXmlStreamReader &reader, Config::CForwardingChannelGroup *pChannelGroup)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();
				if (strTmp == "forward_c")
				{
					ReadForwardingC(reader, pChannelGroup);
				}
				else if (strTmp == "group")
				{
					Config::CForwardingChannelGroup *pChannelChildGroup = new Config::CForwardingChannelGroup;
					pChannelChildGroup->m_strGroupName = reader.attributes().value("name").toString();

					pChannelGroup->m_arrGroup.insert(std::make_pair(pChannelChildGroup->m_strGroupName.toStdString(), pChannelChildGroup));

					//��group������group
					reader.readNext();

					ReadForwardingChannelGroupNode(reader, pChannelChildGroup);

					continue;
				}
			}
			else if (reader.isEndElement() && strTmp == "group")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	bool CFesData::SaveFesData(const QString &szRoot, int nOccNo, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)

	{
		QString fileName = QString("%1%2").arg(m_szTagName).arg(".xml");

		//ǰ���ļ��޸�

		QString strPath = szRoot + "/scada/";

		strPath = strPath + fileName;

		QFile file(strPath);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QString strTmp = QString(QObject::tr("open %1 fail!!!")).arg(fileName);

			s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			MYLIB::Log2File(LOG_FES_LOG, fileName.toStdString().c_str(), true);

			return false;
		}
		QString strTmp = QString("[Open %1 success!!!]").arg(fileName);
		MYLIB::Log2File(LOG_FES_LOG ,strTmp.toStdString().c_str(), true);

		QXmlStreamWriter writer(&file);
		writer.setAutoFormatting(true);
		writer.writeStartDocument();


		writer.writeStartElement("fes");
		writer.writeAttribute("Description", m_strDescription);
		const auto it = pHash->find(m_strDescription.toStdString());
		if (it == pHash->end())
		{
			//�ַ�����
			*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
			pHash->insert(std::make_pair(m_strDescription.toStdString(), *pDescStringPoolOccNo));
			pStringPoolVec->push_back(m_strDescription.toStdString());
			//DescriptionOccNo
			
			writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
		}
		else
		{
			writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
		}

		//У��tagname
		if (!CheckTagNameIsValid(m_szTagName, FES_DESC))
		{
			strTmp = QString(QObject::tr("[Error: Fes %1 TagName is invalid!!!]")).arg(m_szTagName);
			s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			MYLIB::Log2File(LOG_FES_LOG, fileName.toStdString().c_str(), true);
		}

		strTmp = QString(QObject::tr("[Fes %1 log start!!!]")).arg(m_szTagName);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
		writer.writeAttribute("TagName", m_szTagName);
		writer.writeAttribute("version", VERSION);
		writer.writeAttribute("ID", QString("%1").arg(m_nID));
		writer.writeAttribute("Extention", m_strExtention);
		writer.writeAttribute("GroupName", m_strGroupName);
		//�����к�
		m_nOccNo = nOccNo;
		if (m_nOccNo <= 0)
		{
			strTmp = QString(QObject::tr("[Fes %1 TagName OccNo %2 is invalid!!!]")).arg(m_szTagName).arg(m_nOccNo);
			s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			MYLIB::Log2File(LOG_FES_LOG, fileName.toStdString().c_str(), true);
		}
		writer.writeAttribute("OccNo", QString("%1").arg(m_nOccNo));
		
		SaveTransform(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);
		SaveAlarm(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);
		SaveChannel(writer, m_pChannelGroup, pHash, pStringPoolVec, pDescStringPoolOccNo);
		//SaveChannelChildNode(writer, m_pChannelGroup);
		SaveForwardingChannel(writer, m_pForwardingChannelGroup, pHash, pStringPoolVec, pDescStringPoolOccNo);


		SaveVariable(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		writer.writeEndDocument();

		return true;
	}

	bool CFesData::SaveChannel(QXmlStreamWriter &writer, Config::CChannelGroup *pChannelGroup, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pChannelGroup);
		if (!pChannelGroup)
		{
			return false;
		}
		
		auto strTmp = QString("-->Fes TagName %1  Channel log start").arg(m_szTagName);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);
		writer.writeStartElement("channel");

		int nCounts = (int)m_arrMaps[CHANNELHASHID].size();
		strTmp = QString("-->Fes TagName %1  Channel count is %2").arg(m_szTagName).arg(nCounts);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);
		writer.writeAttribute("Count", QString("%1").arg(nCounts));

		int nOccNoChannel = 0, nOccNoDevice = 0, nOccNoAI = 0, nOccNoDI = 0, nOccNoAO = 0, nOccNoDO = 0;
		SaveChannelChildNode(writer, /*m_pChannelGroup*/pChannelGroup, nOccNoChannel, nOccNoDevice, nOccNoAI, nOccNoDI, nOccNoAO, nOccNoDO, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}


	/*! \fn bool CFesData::SaveChannelChildNode(QXmlStreamWriter &writer, Config::CChannelGroup *pChannelGroup)
	********************************************************************************************************* 
	** \brief Config::CFesData::SaveChannelChildNode 
	** \details ͨ����ڵ�
	** \param writer 
	** \param pChannelGroup 
	** \return bool 
	** \author gw
	** \date 2016��10��21�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::SaveChannelChildNode(QXmlStreamWriter &writer, Config::CChannelGroup *pChannelGroup, int &nOccNoChannel, int &nOccNoDevice, int &nAIOccNo
		, int &nDIOccNo, int &nAOOccNo, int &nDOOccNo, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		auto ite = pChannelGroup->m_arrChannelGroup.begin();
		for (; ite != pChannelGroup->m_arrChannelGroup.end(); ++ite)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", ite->second->m_strGroup);

			SaveChannelChildNode(writer, ite->second, nOccNoChannel, nOccNoDevice, nAIOccNo, nDIOccNo, nAOOccNo, nDOOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}
		
		auto it = pChannelGroup->m_arrChannelItem.begin();
		for (; it != pChannelGroup->m_arrChannelItem.end(); ++it)
		{
			it->second->SaveChannelData(writer, nOccNoChannel, nOccNoDevice, nAIOccNo, nDIOccNo, nAOOccNo, nDOOccNo, this, pHash, pStringPoolVec, pDescStringPoolOccNo);
		}

		return true;
	}

	/*! \fn bool CFesData::SaveForwardingChannel(QXmlStreamWriter & writer, Config::CForwardingChannelGroup * pChannelGroup, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	********************************************************************************************************* 
	** \brief Config::CFesData::SaveForwardingChannel 
	** \details ����ת��ͨ��
	** \param writer 
	** \param pChannelGroup 
	** \param  
	** \param pHash 
	** \param pStringPoolVec 
	** \param pDescStringPoolOccNo 
	** \return bool 
	** \author gw
	** \date 2017��4��13�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::SaveForwardingChannel(QXmlStreamWriter & writer, Config::CForwardingChannelGroup * pChannelGroup, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	{
		Q_ASSERT(pChannelGroup);
		if (!pChannelGroup)
		{
			return false;
		}

		auto strTmp = QString("-->Fes %1  Forwarding Channel log start!!!").arg(m_szTagName);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);

		writer.writeStartElement("forwardingchannel");

		int nCounts = (int)m_arrMaps[FORWARDINGCHANNEL].size();

		writer.writeAttribute("Count", QString("%1").arg(nCounts));
		strTmp = QString("-->Fes %1  Forwarding Channel count is %2!!!").arg(m_szTagName).arg(nCounts);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);

		int nOccNoChannel = 0, nOccNoAI = 0, nOccNoDI = 0, nOccNoAO = 0, nOccNoCG = 0, nOccNoDO = 0, nKwhOccNo = 0;
		SaveForwardingChannelChildNode(writer, /*m_pChannelGroup*/pChannelGroup, nOccNoChannel, nOccNoAI, nOccNoDI, nOccNoAO, nOccNoCG, nOccNoDO, nKwhOccNo,pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}

	bool CFesData::SaveForwardingChannelChildNode(QXmlStreamWriter & writer, Config::CForwardingChannelGroup * pChannelGroup, int & nOccNoChannel, int &nAIOccNo, int &nDIOccNo
		, int &nAOOccNo, int &nCGOccNo, int &nDOOccNo, int& nKwhOccNo, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	{
		auto ite = pChannelGroup->m_arrGroup.begin();
		for (; ite != pChannelGroup->m_arrGroup.end(); ++ite)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", ite->second->m_strGroupName);

			SaveForwardingChannelChildNode(writer, ite->second, nOccNoChannel, nAIOccNo, nDIOccNo, nAOOccNo, nCGOccNo, nDOOccNo, nKwhOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}

		auto it = pChannelGroup->m_arrItem.begin();
		for (; it != pChannelGroup->m_arrItem.end(); ++it)
		{
			it->second->SaveForwardingChannelData(writer, nOccNoChannel, nAIOccNo, nDIOccNo, nAOOccNo, nCGOccNo, nDOOccNo, nKwhOccNo, this, pHash, pStringPoolVec, pDescStringPoolOccNo);
		}

		return true;
	}

	bool CFesData::SaveTransform(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(m_pLinearTransformGroup);
		if (!m_pLinearTransformGroup)
		{
			return false;
		}
		
		auto strTmp = QString(QObject::tr("[-->Scale log start!!!]"));
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);

		writer.writeStartElement("scale");

		SaveLinear(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		SaveNonlinear(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();
		
		return true;
	}


	/*! \fn bool CFesData::SaveLinearTransformChildNode(QXmlStreamWriter &writer, Config::CTransformGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CFesData::SaveLinearTransformChildNode 
	** \details ��������ת����
	** \param writer 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016��11��1�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::SaveLinearTransformChildNode(QXmlStreamWriter &writer, Config::CTransformGroup *pGroup, int &nOccNo
		, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		auto &arr = pGroup->GetGroup();
		for each (auto var in arr)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", var->GetGroupName());

			SaveLinearTransformChildNode(writer, var, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}
		

		auto &arrLinear = pGroup->GetItem();
		for (auto const &linear : arrLinear)
		{
			Q_ASSERT(linear);
			auto pTmp = linear->GetLinear();
			Q_ASSERT(pTmp);

			writer.writeStartElement("l");

			if (!CheckTagNameIsValid(pTmp->m_szTagName, FES_DESC))
			{
				auto strTmp = QString(QObject::tr("[---->Fes %1  Scale Linear Tagname %2 is invalid!!!]")).arg(m_szTagName).arg(pTmp->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			writer.writeAttribute("TagName", QString("%1").arg(pTmp->m_szTagName));

			nOccNo++;
			if (nOccNo <= 0)
			{
				auto strTmp = QString(QObject::tr("[---->Scale Linear Tagname %1 OccNo %2 is invalid!!!]")).arg(pTmp->m_szTagName).arg(nOccNo);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			linear->SetOccNo(nOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nOccNo));
	
			writer.writeAttribute("ID", QString("%1").arg(pTmp->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(pTmp->m_strDescription));
			const auto it = pHash->find(pTmp->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				//�ַ�����
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(pTmp->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(pTmp->m_strDescription.toStdString());
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}
			
			writer.writeAttribute("ScaleType", QString("%1").arg(pTmp->m_nType));
			writer.writeAttribute("MaxRaw", QString::number(pTmp->m_dbMaxRaw, 10, 6));
			writer.writeAttribute("MinRaw", QString::number(pTmp->m_dbMinRaw, 10, 6));
			writer.writeAttribute("MaxScale", QString::number(pTmp->m_dbMaxScale, 10, 6));
			writer.writeAttribute("MinScale", QString::number(pTmp->m_dbMinScale, 10, 6));
			writer.writeAttribute("Inverse", QString("%1").arg(pTmp->m_bInverseFactor));;
			writer.writeAttribute("EnaleFactor", QString("%1").arg(pTmp->m_bEnableFactor));
			writer.writeAttribute("Gain", QString("%1").arg(pTmp->m_dbGain));
			writer.writeAttribute("Offset", QString("%1").arg(pTmp->m_dbOffset));
			writer.writeAttribute("GroupName", QString("%1").arg(pTmp->m_strGroupName));

			writer.writeEndElement();
		}

		return true;
	}

	bool CFesData::SaveLinear(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(m_pLinearTransformGroup);
		if (!m_pLinearTransformGroup)
		{
			return false;
		}

		auto strTmp = QString(QObject::tr("[-->Scale Linear Log Start!!!]"));
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
		
		writer.writeStartElement("linear");
		auto nCount = m_pLinearTransformGroup->GetItemCount();
		
		strTmp = QString(QObject::tr("[-->Scale Linear Count:%1!!!]")).arg(nCount);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
		
		writer.writeAttribute("Count", QString("%1").arg(nCount));

		int nOccNo = 0;
		SaveLinearTransformChildNode(writer, m_pLinearTransformGroup, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}
	/*! \fn bool CFesData::SaveNonLinearTransformChildNode(QXmlStreamWriter &writer, Config::CTransformGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CFesData::SaveNonLinearTransformChildNode 
	** \details ���������ת����
	** \param writer 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016��11��1�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::SaveNonLinearTransformChildNode(QXmlStreamWriter &writer, Config::CTransformGroup *pGroup, int &nOccNo
		, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		auto &arr = pGroup->GetGroup();
		for each (auto var in arr)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", var->GetGroupName());

			SaveNonLinearTransformChildNode(writer, var, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}

		auto &arrNonLinear = pGroup->GetItem();
		for (auto const &nonlinear : arrNonLinear)
		{
			Q_ASSERT(nonlinear);
			auto pTmp = nonlinear->GetNonLinear();
			Q_ASSERT(pTmp);
			auto bFlag = CheckTagNameIsValid(pTmp->m_szTagName, FES_DESC);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				auto strTmp = QString(QObject::tr("[Error----> Fes TagName %1 Scale NonLinear TagName %2 is invalid!!!]")).arg(m_szTagName).arg(pTmp->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeStartElement("nl");

			nOccNo++;
			//pTmp->SetOccNo(nOccNo);
			//ת�������к������
			nonlinear->SetOccNo(nOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nonlinear->GetOccNo()));
			writer.writeAttribute("TagName", QString("%1").arg(pTmp->m_szTagName));
			writer.writeAttribute("ID", QString("%1").arg(pTmp->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(pTmp->m_strDescription));
			const auto it = pHash->find(pTmp->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				//�ַ�����
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(pTmp->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(pTmp->m_strDescription.toStdString());

				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("ScaleType", QString("%1").arg(nonlinear->m_nType));
			writer.writeAttribute("Count", QString("%1").arg(pTmp->m_arrPNonliear.size()));
			writer.writeAttribute("GroupName", QString("%1").arg(pTmp->m_strGroupName));

			if (pTmp->m_arrPNonliear.size() == 0)
			{
				auto strTmp = QString(QObject::tr("[Error---->Error:Fes %1  Scale NonLinear TagName %2  Point Count is 0!!!]")).arg(m_szTagName).arg(pTmp->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			for (auto const &p : pTmp->m_arrPNonliear)
			{
				writer.writeStartElement("p");

				writer.writeAttribute("RawValue", QString("%1").arg(p->RawValue));
				writer.writeAttribute("ScaleValue", QString("%1").arg(p->ScaleValue));

				writer.writeEndElement();
			}


			writer.writeEndElement();
		}


		return true;
	}

	bool CFesData::SaveNonlinear(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(m_pLinearTransformGroup);
		if (!m_pLinearTransformGroup)
		{
			return false;
		}
		
		writer.writeStartElement("nonlinear");

		auto nCount = m_pLinearTransformGroup->GetItemCount();
		
		auto strTmp = QString(QObject::tr("[-->Scale NonLinear count is %1!!!]")).arg(nCount);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
		writer.writeAttribute("Count", QString("%1").arg(nCount));
		
		int nOccNo = 0;
		SaveNonLinearTransformChildNode(writer, m_pNonLinearTransformGroup, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}


	bool CFesData::SaveAlarm(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		auto strTmp = QString(QObject::tr("[-->Alarm Log Start!!!]"));
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
		writer.writeStartElement("alarm");

		Q_ASSERT(m_pAnalogGroup);
		if (!m_pAnalogGroup)
		{
			return false;
		}

		int nCounts = (int)m_arrMaps[HASHID::ANALOGALARMHASHID].size();
		strTmp = QString(QObject::tr("[-->Fes TagName %1  Alarm analog alarm count is %1!!!]")).arg(nCounts);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
		
		writer.writeStartElement("ain_alarm");
		writer.writeAttribute("Count", QString("%1").arg(nCounts));
		int nAnalogAlarmOccNo = 0, nAanalogAlarmBlockNo = 0;
		SaveAnalogAlarm(writer, m_pAnalogGroup, nAnalogAlarmOccNo, nAanalogAlarmBlockNo, pHash, pStringPoolVec, pDescStringPoolOccNo);
		writer.writeEndElement();

		writer.writeStartElement("din_alarm");
		nCounts = (int)m_arrMaps[HASHID::DIGITALALARMHASHID].size();
		
		strTmp = QString(QObject::tr("[-->Fes TagName %1  Alarm digital alarm count is %2!!!]")).arg(m_szTagName).arg(nCounts);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
		
		writer.writeAttribute("Count", QString("%1").arg(nCounts));
		int nDigitalAlarmOccNo = 0, nDigitalAlarmLimitOccNo = 0;
		SaveDigtalAlarm(writer, m_pDigitalGroup, nDigitalAlarmOccNo, nDigitalAlarmLimitOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);
		writer.writeEndElement();

		writer.writeEndElement();
		return true;
	}

	bool CFesData::SaveAnalogAlarm(QXmlStreamWriter &writer, CAnalogAlarmGroup *pGroup, int &nAanalogAlarmOccNo, int &nAanalogAlarmLimitOccNo
		, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto &arrGroup = pGroup->GetGroup();
		for each (auto var in arrGroup)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", QString("%1").arg(var->GetGroupName()));

			SaveAnalogAlarm(writer, var, nAanalogAlarmOccNo, nAanalogAlarmLimitOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}


		auto &arrItem = pGroup->GetItem();
		Q_ASSERT(nAanalogAlarmOccNo + 1 > 0);
		Q_ASSERT(nAanalogAlarmLimitOccNo + 1 > 0);
		for each (auto var in arrItem)
		{
			writer.writeStartElement("ain_alarm_node");
			
			nAanalogAlarmOccNo++;
			var->SetOccNo(nAanalogAlarmOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nAanalogAlarmOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			if (!CheckTagNameIsValid(var->m_szTagName, FES_DESC))
			{
				auto strTmp = QString(QObject::tr("[Error-->Fes %1  Alarm analog alarm TagName %2 is invalid!!!]")).arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			//�ַ����ڴ��
			const auto it = pHash->find(var->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				//�ַ�����
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(var->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(var->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}
			
			writer.writeAttribute("AlarmType", QString("%1").arg(var->m_nAlarmType));
			writer.writeAttribute("Enable", QString("%1").arg(var->m_bEnable));
			writer.writeAttribute("OnQualityGood", QString("%1").arg(var->m_bOnQualityGood));
			writer.writeAttribute("DeadType", QString("%1").arg(var->m_nDeadType));
			auto tmp = QString::number(var->m_dbDeadArea, 2, 6);
			writer.writeAttribute("DeadArea", tmp);
			writer.writeAttribute("DelayAlarm", QString("%1").arg(var->m_nDelayAlarm));
			writer.writeAttribute("GroupName", QString("%1").arg(var->m_strGroupName));

			auto const &analogOfflimit = var->m_arrAlarmOfflimit;
			writer.writeAttribute("Count", QString("%1").arg(analogOfflimit.size()));
			if (analogOfflimit.size() == 0)
			{
				auto strTmp = QString(QObject::tr("[Error-->Fes %1  Alarm analog alarm %2 limit count is 0!!!]")).arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			int nBlockNo = 0;
			for (auto const &tmp : analogOfflimit)
			{
				writer.writeStartElement("aa");

				nAanalogAlarmLimitOccNo++;
				tmp->SetOccNo(nAanalogAlarmLimitOccNo);
				writer.writeAttribute("OccNo", QString("%1").arg(nAanalogAlarmLimitOccNo));
				nBlockNo++;
				tmp->SetBlockNo(nBlockNo);
				writer.writeAttribute("BlockNo", QString("%1").arg(nBlockNo));
				writer.writeAttribute("TagName", QString("%1").arg(tmp->m_szTagName));
				if (!CheckTagNameIsValid(tmp->m_szTagName, FES_DESC))
				{
					auto strTmp = QString(QObject::tr("[Error-->Fes %1  Alarm analog alarm limit TagNames %2 invalid!!!]")).arg(m_szTagName).arg(tmp->m_szTagName);
					MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
					m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}

				writer.writeAttribute("ID", QString("%1").arg(tmp->m_nID));
				writer.writeAttribute("Description", QString("%1").arg(tmp->m_strDescription));
				//�ַ����ڴ��
				const auto it = pHash->find(tmp->m_strDescription.toStdString()) ;
				if (it == pHash->end())
				{
					//�ַ�����
					*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
					pHash->insert(std::make_pair(tmp->m_strDescription.toStdString(), *pDescStringPoolOccNo));
					pStringPoolVec->push_back(tmp->m_strDescription.toStdString());

					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
				}
				else
				{
					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
				}
				
				//writer.writeAttribute("TimeFormat", QString("%1").arg(tmp->TimeFormat));
				writer.writeAttribute("Condition", QString("%1").arg(tmp->m_nCondition));
				writer.writeAttribute("Category", QString("%1").arg(tmp->m_nCategory));
				writer.writeAttribute("Value", QString("%1").arg(tmp->m_dbValue));
				writer.writeAttribute("LowValue", QString("%1").arg(tmp->m_dbLowValue));
				writer.writeAttribute("DelayAlarm", QString("%1").arg(tmp->m_nDelayAlarm));
				writer.writeAttribute("Priority", QString("%1").arg(tmp->m_nPriority));
				writer.writeAttribute("SupportAck", QString("%1").arg(tmp->m_bSupportAck));
				writer.writeAttribute("SupportDelete", QString("%1").arg(tmp->m_bSupportDelete));
				writer.writeAttribute("AckType", QString("%1").arg(tmp->m_nAckType));
				writer.writeAttribute("SoundFile", QString("%1").arg(tmp->m_strSoundFile));
				writer.writeAttribute("PlaySoundTimes", QString("%1").arg(tmp->m_nPlaySoundTimes));
				writer.writeAttribute("SpeechAlarmText", QString("%1").arg(tmp->m_strSpeechAlarmText));
				writer.writeAttribute("Beep", QString("%1").arg(tmp->m_bBeep));
				writer.writeAttribute("PushGraph", QString("%1").arg(tmp->m_strPushGraph));
				writer.writeAttribute("BlinkGraph", QString("%1").arg(tmp->m_bBlinkGraph));
				writer.writeAttribute("Log", QString("%1").arg(tmp->m_bLog));
				writer.writeAttribute("BackColor", QString("%1").arg(tmp->m_nBackColor));
				writer.writeAttribute("TextColor", QString("%1").arg(tmp->m_nTextColor));
				writer.writeAttribute("BlinkBackColor", QString("%1").arg(tmp->m_nBlinkBackColor));
				writer.writeAttribute("BlinkTextColor", QString("%1").arg(tmp->m_nBlinkTextColor));
				writer.writeAttribute("LightBoard", QString("%1").arg(tmp->m_bLightBoard));
				writer.writeAttribute("DispGuide", QString("%1").arg(tmp->m_strDispGuide));

				writer.writeAttribute("Commands", QString("%1").arg(tmp->m_strCommands));

				writer.writeEndElement();
			}

			writer.writeEndElement();
		}

		return true;
	}

	bool CFesData::SaveDigtalAlarm(QXmlStreamWriter &writer, CDigitalAlarmGroup *pGroup, int &nDigitalAlarmOccNo, int &nDigitalAlarmLimitOccNo
		, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto &arrGroup = pGroup->GetGroup();
		for each (auto var in arrGroup)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", QString("%1").arg(var->GetGroupName()));

			SaveDigtalAlarm(writer, var, nDigitalAlarmOccNo, nDigitalAlarmLimitOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}



		auto &arrItem = pGroup->GetItem();
		Q_ASSERT(nDigitalAlarmOccNo + 1 > 0);
		Q_ASSERT(nDigitalAlarmLimitOccNo + 1 > 0);
		for each (auto var in arrItem)
		{
			writer.writeStartElement("din_alarm_node");
			
			nDigitalAlarmOccNo++;

			var->SetOccNo(nDigitalAlarmOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(var->GetOccNo()));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			if (!CheckTagNameIsValid(var->m_szTagName, FES_DESC))
			{
				auto strTmp = QString(QObject::tr("[Error-->Fes %1  Alarm digital alarm TagName %2 is invalid!!!]")).arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			//�ַ����ڴ��
			const auto it = pHash->find(var->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(var->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(var->m_strDescription.toStdString());

				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//�ַ�������
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}
			
			writer.writeAttribute("AlarmType", QString("%1").arg(var->m_nAlarmType));
			writer.writeAttribute("DelayAlarm", QString("%1").arg(var->m_nDelayAlarm));
			writer.writeAttribute("Enable", QString("%1").arg(var->m_bEnable));
			writer.writeAttribute("OnQualityGood", QString("%1").arg(var->m_bOnQualityGood));
			writer.writeAttribute("GroupName", QString("%1").arg(var->m_strGroupName));

			auto const &digtalOfflimit = var->m_arrDigtalOfflimit;
			if (digtalOfflimit.size() == 0)
			{
				auto strTmp = QString(QObject::tr("[Error-->Fes %1  Alarm digital alarm TagName %2 count is 0!!!]")).arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			writer.writeAttribute("Count", QString("%1").arg(digtalOfflimit.size()));
			int nBolckNo = 0;
			for (auto const &tmp : digtalOfflimit)
			{
				writer.writeStartElement("da");

				nDigitalAlarmLimitOccNo++;
				tmp->OccNo = nDigitalAlarmLimitOccNo;
				writer.writeAttribute("OccNo", QString("%1").arg(tmp->OccNo));

				nBolckNo++;
				tmp->BlockNo = nBolckNo;
				writer.writeAttribute("BlockNo", QString("%1").arg(nBolckNo));

				writer.writeAttribute("TagName", QString("%1").arg(tmp->TagName));
				if (!CheckTagNameIsValid(tmp->TagName, FES_DESC))
				{
					auto strTmp = QString(QObject::tr("[Error-->Fes %1  Alarm digital alarm limit TagName %2 is invalid!!!]")).arg(m_szTagName).arg(tmp->TagName);
					MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
					m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}
				writer.writeAttribute("ID", QString("%1").arg(tmp->ID));
				writer.writeAttribute("Description", QString("%1").arg(tmp->Description));
				//�ַ����ڴ��
				const auto it = pHash->find(tmp->Description.toStdString())  ;
				if (it == pHash->end())
				{
					*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;

					pHash->insert(std::make_pair(tmp->Description.toStdString(), *pDescStringPoolOccNo));
					pStringPoolVec->push_back(tmp->Description.toStdString());

					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
				}
				else
				{
					//�ַ�������
					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
				}

				writer.writeAttribute("Condition", QString("%1").arg(tmp->Condition));
				writer.writeAttribute("Category", QString("%1").arg(tmp->Category));
				writer.writeAttribute("DelayAlarm", QString("%1").arg(tmp->DelayAlarm));
				writer.writeAttribute("Priority", QString("%1").arg(tmp->Priority));
				writer.writeAttribute("SupportAck", QString("%1").arg(tmp->SupportAck));
				writer.writeAttribute("SupportDelete", QString("%1").arg(tmp->SupportDelete));
				writer.writeAttribute("AckType", QString("%1").arg(tmp->AckType));
				writer.writeAttribute("SoundFile", QString("%1").arg(tmp->SoundFile));
				writer.writeAttribute("PlaySoundTimes", QString("%1").arg(tmp->PlaySoundTimes));
				writer.writeAttribute("SpeechAlarmText", QString("%1").arg(tmp->SpeechAlarmText));
				writer.writeAttribute("Beep", QString("%1").arg(tmp->Beep));
				writer.writeAttribute("PushGraph", QString("%1").arg(tmp->PushGraph));
				writer.writeAttribute("BlinkGraph", QString("%1").arg(tmp->BlinkGraph));
				writer.writeAttribute("Log", QString("%1").arg(tmp->Log));
				
				writer.writeAttribute("BackColor", QString("%1").arg(tmp->BackColor));
				writer.writeAttribute("TextColor", QString("%1").arg(tmp->TextColor));
				writer.writeAttribute("BlinkBackColor", QString("%1").arg(tmp->BlinkBackColor));
				writer.writeAttribute("BlinkTextColor", QString("%1").arg(tmp->BlinkTextColor));

				writer.writeAttribute("DispGuide", QString("%1").arg(tmp->DispGuide));
				writer.writeAttribute("Commands", QString("%1").arg(tmp->Commands));

				writer.writeEndElement();
			}

			writer.writeEndElement();
		}

		return true;
	}


	/*! \fn bool CFesData::SaveVariable(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CFesData::SaveVariable 
	** \details �������
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2016��10��9�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::SaveVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("variable");
		auto strTmp = QString("-->Fes %1  Variable log start!!!").arg(m_szTagName);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);

		int nCount = m_arrMaps[HASHID::SYSTEMVARIABLEID].size();
		strTmp = QString("-->Fes %1 System Variable count is %2!!!").arg(m_szTagName).arg(nCount);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
		SaveSystemVariable(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);

		nCount = m_arrMaps[HASHID::USERVIRIABLEID].size();
		strTmp = QString("-->Fes %1 User Variable count is %2!!!").arg(m_szTagName).arg(nCount);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
		SaveUserVariable(writer, pHash, pStringPoolVec, pDescStringPoolOccNo);



		writer.writeEndElement();

		return true;
	}


	/*! \fn bool CFesData::SaveSystemVariable(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CFesData::SaveSystemVariable 
	** \details ����ϵͳ����
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2016��10��9�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::SaveSystemVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("system");
		writer.writeAttribute("Count", QString("%1").arg(m_arrSystemVariable.size()));

		int nOccNo = 0;
		for each (auto var in m_arrSystemVariable)
		{
			writer.writeStartElement("s");

			nOccNo++;
			var->SetOccNo(nOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			if (!CheckTagNameIsValid(var->m_szTagName, FES_DESC))
			{
				auto strTmp = QString(QObject::tr("[Error-->Fes %1  System Varaible TagName %2 is invalid!!!]")).arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			const auto it = pHash->find(var->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(var->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(var->m_strDescription.toStdString());

				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
 			}
			else
			{
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}
			
			writer.writeAttribute("DataType", QString("%1").arg(var->m_nType));

			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}


	/*! \fn bool CFesData::SaveUserVariable(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CFesData::SaveUserVariable 
	** \details �����û�����
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2016��10��9�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::SaveUserVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo
	)
	{
		writer.writeStartElement("user");

		SaveUserVariableChildNode(writer, m_pUserVariableGroup, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}


	bool CFesData::SaveUserVariableChildNode(QXmlStreamWriter &writer, Config::CUserVariableGroup *pGroup, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}
		
		auto ite = pGroup->m_arrGroup.begin();
		for (; ite != pGroup->m_arrGroup.end(); ++ite)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", (*ite)->GetGroupName());
			writer.writeAttribute("count", QString("%1").arg(pGroup->m_arrItem.size()));

			SaveUserVariableChildNode(writer, (*ite), pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}

		auto it = pGroup->m_arrItem.begin();
		for each (auto var in pGroup->m_arrItem)
		{
			writer.writeStartElement("u");

			writer.writeAttribute("OccNo", QString("%1").arg(var->GetOccNo()));
			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			if (!CheckTagNameIsValid(var->m_szTagName, FES_DESC))
			{
				auto strTmp = QString(QObject::tr("[Error-->Fes %1  User Varaible TagName %2 is invalid!!!]")).arg(m_szTagName).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(var->m_nID));
			writer.writeAttribute("Description", QString("%1").arg(var->m_strDescription));
			const auto it = pHash->find(var->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(var->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(var->m_strDescription.toStdString());

				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}
			
			
			writer.writeAttribute("DataType", QString("%1").arg(var->m_nType));
			writer.writeAttribute("InitValue", QString("%1").arg(var->m_dbInitValue));
			writer.writeAttribute("InitialQua", QString("%1").arg(var->m_nInitialQua));
			writer.writeAttribute("Address", QString("%1").arg(var->m_strAddress));
			writer.writeAttribute("PinLabel", QString("%1").arg(var->m_strPinLabel));
			writer.writeAttribute("SourceTagName", QString("%1").arg(var->m_strSourceTagName));
			
			if (!var->m_strSourceTagName.isEmpty())
			{
				//����tagname��OccNo
				QString strTmp = QString("%1").arg(var->m_szTagName);
				var->m_strSourceOccNo = "";
				if (!GetUserVariableRelatedSourceOccNo(strTmp, var->m_strSourceTagName, var->m_strSourceOccNo))
				{
					auto strError = QObject::tr("User variable tagname %1 relate sourcetagname failed!!!").arg(var->m_szTagName);
					LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					writer.writeEndElement();

					break;
				}
				writer.writeAttribute("SourceOccNo", QString("%1").arg(var->m_strSourceOccNo));
			}


			//������ת�������к�
			//�ж�ת��tagname�Ƿ�Ϊ��
			if (var->m_strScaleTagName.isEmpty())
			{
				writer.writeAttribute("ScaleOccNo", QString("%1").arg(0));
				writer.writeAttribute("ScaleType", QString("%1").arg(LINEAR));
				//writer.writeAttribute("ScaleDesc", QString("%1").arg(""));
				writer.writeAttribute("ScaleTagName", QString("%1").arg(""));

			}
			else
			{
				auto pTransform = (Config::CTransform *)GetData(var->m_strScaleTagName, HASHID::TRANSFORMHASHID);
				Q_ASSERT(pTransform);
				if (!pTransform)
				{
					auto strError = QObject::tr("scale tagname %1 is isvalid!!!").arg(var->m_strScaleTagName);
					LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					writer.writeEndElement();

					break;
				}

				writer.writeAttribute("ScaleOccNo", QString("%1").arg(pTransform->GetOccNo()));
				writer.writeAttribute("ScaleType", QString("%1").arg(var->m_nScaleType));
				//writer.writeAttribute("ScaleDesc", QString("%1").arg(var->m_strScaleDesc));
				writer.writeAttribute("ScaleTagName", QString("%1").arg(var->m_strScaleTagName));
			}

			//�����ĸ澯�����к�
			if (var->m_strAlarmTagName.isEmpty())
			{
				writer.writeAttribute("AlarmOccNo", QString("%1").arg(0));
				writer.writeAttribute("AlarmType", QString("%1").arg(LINEAR));
				//writer.writeAttribute("AlarmDesc", QString("%1").arg(""));
				writer.writeAttribute("AlarmTagName", QString("%1").arg(""));
			}
			else
			{
				if (var->m_nAlarmType == AlarmType::AIALARM)
				{
					auto pAlarm = (Config::CAnalogAlarm *)GetData(var->m_strAlarmTagName, HASHID::ANALOGALARMHASHID);
					Q_ASSERT(pAlarm);
					if (!pAlarm)
					{
						auto strError = QObject::tr("Alarm tagname %1 is isvalid!!!").arg(var->m_strAlarmTagName);
						LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						writer.writeEndElement();

						break;
					}

					writer.writeAttribute("AlarmOccNo", QString("%1").arg(pAlarm->GetOccNo()));
					writer.writeAttribute("AlarmType", QString("%1").arg(pAlarm->m_nAlarmType));
					//writer.writeAttribute("AlarmDesc", QString("%1").arg(pAlarm->m_strDescription));
					writer.writeAttribute("AlarmTagName", QString("%1").arg(pAlarm->m_szTagName));
				}
				else if (var->m_nAlarmType == AlarmType::DIALARM)
				{
					auto pAlarm = (Config::CDigitalAlarm *)GetData(var->m_strAlarmTagName, HASHID::DIGITALALARMHASHID);
					Q_ASSERT(pAlarm);
					if (!pAlarm)
					{
						auto strError = QObject::tr("Alarm tagname %1 is isvalid!!!").arg(var->m_strAlarmTagName);
						LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						writer.writeEndElement();

						break;
						
					}

					writer.writeAttribute("AlarmOccNo", QString("%1").arg(pAlarm->GetOccNo()));
					writer.writeAttribute("AlarmType", QString("%1").arg(pAlarm->m_nAlarmType));
					//writer.writeAttribute("AlarmDesc", QString("%1").arg(pAlarm->m_strDescription));
					writer.writeAttribute("AlarmTagName", QString("%1").arg(pAlarm->m_szTagName));
				}
				else
				{
					auto strError = QObject::tr("User variable tagname %1  related alarm type %2 is invalid!!!").arg(var->m_strScaleTagName).arg(var->m_nAlarmType);
					LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
				}
				
			}

			writer.writeEndElement();
		}

		return true;
	}

	/*! \fn bool CFesData::ReadVariable(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadVariable 
	** \details ��ȡǰ�ñ���
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��10��9�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadVariable(QXmlStreamReader &reader)
	{
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "system")
				{
					ReadSystemVariable(reader);
				}
				else if (strTmp == "user")
				{
					Q_ASSERT(m_pUserVariableGroup);
					if (!m_pUserVariableGroup)
					{
						reader.readNext();

						continue;
					}

					ReadUserVariableGroup(reader, m_pUserVariableGroup);
				}
			}
			else if (reader.isEndElement())
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}


	/*! \fn bool CFesData::ReadSystemVariable(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadSystemVariable 
	** \details 
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��11��29�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadSystemVariable(QXmlStreamReader &reader)
	{
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "system")
				{

				}
				else if (strTmp == "s")
				{
					auto pSystem = new CSystemVariable();

					pSystem->SetOccNo(reader.attributes().value("OccNo").toInt());
					pSystem->m_nID = reader.attributes().value("ID").toInt();

					memset(pSystem->m_szTagName, 0, sizeof(pSystem->m_szTagName));
					QString strTagName = reader.attributes().value("TagName").toString();
					int nSize = strTagName.size();
					strncpy(pSystem->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

					pSystem->m_strDescription = reader.attributes().value("Description").toString();
					pSystem->m_nType = reader.attributes().value("DataType").toInt();


					if (!CheckTagNameIsExist(strTagName))
					{
						if (!PushFesTagNameHashMap(HASHID::SYSTEMVARIABLEID, strTagName, pSystem))
						{
							auto strError = QObject::tr("system variable read error!!!");
							s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

							continue;
						}

						//m_arrSystemVariable.push_back(pSystem);
					}
				}

			}
			else if (reader.isEndElement() && strTmp == "system")
			{
				break;
			}
		}

		return true;
	}


	/*! \fn	bool CFesData::ReadUserVariableGroup(QXmlStreamReader &reader)
	********************************************************************************************************* 
	** \brief Config::CFesData::ReadUserVariableGroup 
	** \details ���û������� rootĬ����
	** \param reader 
	** \return bool 
	** \author gw
	** \date 2016��10��9�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ReadUserVariableGroup(QXmlStreamReader &reader, Config::CUserVariableGroup *pGroup)
	{	
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			auto strError = QObject::tr("read user variable error!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
		QString strTmp = reader.name().toString();
		while (!reader.atEnd())
		{
			reader.readNext();

			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				 if (strTmp == "u")
				{
					//if (strTmp == "s")
					{

						
						auto pUser = new CUserVariable();

						pUser->SetOccNo(reader.attributes().value("OccNo").toInt());
						pUser->m_nID = reader.attributes().value("ID").toInt();

						QString strTagName = reader.attributes().value("TagName").toString();
						if (!CheckTagNameIsValid(strTagName, FES_DESC))
						{
							reader.readNext();

							continue;
						}	
						memset(pUser->m_szTagName, 0, sizeof(pUser->m_szTagName));
						int nSize = strTagName.size();
						strncpy(pUser->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

						pUser->m_strDescription = reader.attributes().value("Description").toString();
						pUser->m_nType = reader.attributes().value("DataType").toInt();
						pUser->m_dbInitValue = reader.attributes().value("InitValue").toDouble();
						pUser->m_nInitialQua = reader.attributes().value("InitialQua").toInt();
						pUser->m_strSourceTagName = reader.attributes().value("SourceTagName").toString();
						if (!pUser->m_strSourceTagName.isEmpty())
						{
							auto list = pUser->m_strSourceTagName.split(".");
							//SourceTagName�ݶ��ж���׼
							if (list.count() == 3)
							{
								//note
								//�û���������source tagname
								//Ϊ�˷������,hash tagname��node tagname + . + ��tagname�����Ժ��޸Ļ���ɾ����node tagname���ߵ�tagname�Ϳ���ֱ���ҵ�������tagname���޸�
								auto strTagName = list[0] + "." + list[1];
								AddUserVariableRelationSourceTagNameArr(strTagName.toStdString(), pUser);
							}
							else
							{
								auto strError = QObject::tr("user variable read error,SourceTagName %1 is invalid!!!").arg(pUser->m_strSourceTagName);
								s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

								continue;
							}						
						}
						
						pUser->m_strSourceOccNo = reader.attributes().value("SourceOccNo").toString();
						pUser->m_strScaleTagName = reader.attributes().value("ScaleTagName").toString();
						if (!pUser->m_strScaleTagName.isEmpty())
						{
							//note
							//�û���������ת����ϵ
							AddUserVariableRelationScaleArr(pUser->m_strScaleTagName.toStdString(), pUser);
						}

						pUser->m_nScaleType = reader.attributes().value("ScaleType").toInt();
						pUser->m_nScaleOccNo = reader.attributes().value("ScaleOccNo").toInt();
						//pUser->m_strScaleDesc = reader.attributes().value("ScaleDesc").toInt();
						//
						pUser->m_strAlarmTagName = reader.attributes().value("AlarmTagName").toString();
						if (!pUser->m_strAlarmTagName.isEmpty())
						{
							//note
							//�û����������澯
							AddUserVariableRelationAlarmArr(pUser->m_strAlarmTagName.toStdString(), pUser);
						}

						pUser->m_nAlarmType = reader.attributes().value("AlarmType").toInt();
						pUser->m_nAlarmOccNo = reader.attributes().value("AlarmOccNo").toInt();
						//pUser->m_strAlarmDesc = reader.attributes().value("AlarmDesc").toString();
						if (!CheckTagNameIsExist(strTagName))
						{
							if (!PushFesTagNameHashMap(HASHID::USERVIRIABLEID, strTagName, pUser))
							{
								auto strError = QObject::tr("user variable read error!!!");
								s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

								continue;
							}
							//note
							//����û��������û���
							pGroup->AddUserVariableItem(pUser);
						}
						else
						{
							auto strError = QObject::tr("user variable read error!!!");
							s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

							continue;
						}
					}
				}
				 else if (strTmp == "group")
				 {


					 QString strName = reader.attributes().value("name").toString();
					 //if (!CheckTagNameIsExist(strName))
					 {
						 auto *pTmp = new CUserVariableGroup;
						 Q_ASSERT(pTmp);
						 if (!pTmp)
						 {
							 auto strError = QObject::tr("read uservariablegroup error!!!");
							 s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

							 continue;
						 }
						 
						 if (!PushFesTagNameHashMap(HASHID::USERVIRIABLEGROUPID, strName, pTmp))
						 {
							 auto strError = QObject::tr("user variable group name %1 is exist!!!").arg(strName);
							 s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

							 continue;
						 }

						 pTmp->SetGroupName(strName);
						 pGroup->AddUserVariableGroup(pTmp);
						 ReadUserVariableGroup(reader, pTmp);
					 }
					 //else
					 //{
						// auto strError = QObject::tr("user variable read error!!!");
						// s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						// continue;
					 //}



				 }
			}
			else if (reader.isEndElement() && strTmp == "group")
			{
				break;
			}
		}

		return true;
	}


	/*! \fn	bool AddNewChannelItem(CChannelGroup *pChannelGroup)
	********************************************************************************************************* 
	** \brief Config::AddNewChannelItem 
	** \details ����µ�ͨ���� m_arrChannelTagNameMap��¼ǰ�õ�����ͨ����
				�Ƚϴ���ͨ�� ����ǰ��tagname
	** \param pChannelGroup 
	** \return bool 
	** \author gw
	** \date 2016��10��20�� 
	** \note 
	********************************************************************************************************/
	CChannelData *CFesData::AddNewChannelItem(CChannelGroup *pChannelGroup)
	{
		Q_ASSERT(pChannelGroup);
		if (!pChannelGroup)
		{
			return nullptr;
		}
		
		Q_ASSERT(s_pGlobleCore);
		if (!s_pGlobleCore)
		{
			return nullptr;
		}

		CChannelData *pData = new CChannelData();
		Q_ASSERT(pData);
		if (pData == nullptr)
			return nullptr;

		//ǰ���ڵ�ͨ������
		auto count = GetArrCount(CHANNELHASHID);
		if (count < 0)
		{
			return nullptr;
		}
		while (true)
		{
			QString strChannel = QString("channel%1").arg(count);
			QString strDesc = QString(QObject::tr("channel%1")).arg(count);

			std::string str = strChannel.toStdString();

			//tagname�Ƿ���� ���� fes channel device ai di ao do���е�hash��Ҫ���
			if (CheckTagNameIsExist(strChannel))
			{
				count++;

				continue;
			}

			pData->m_nID = count;
			pData->m_strDescription = strDesc;

			bool bFlag = CheckTagNameIsValid(strChannel, FES_DESC);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return nullptr;
			}

			if (!CheckTagNameIsValid(strChannel, FES_DESC))
			{
				return nullptr;
			}
			memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
			int nSize = strChannel.size();
			strncpy(pData->m_szTagName, strChannel.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

			if (!CheckDescIsValid(strDesc, FES_DESC))
			{
				pData->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
			}
			else
			{
				pData->m_strDescription = strDesc;
			}

			pChannelGroup->m_arrChannelItem.insert(std::make_pair(str, pData));
			//push channel hash
			this->PushFesTagNameHashMap(CHANNELHASHID, strChannel, pData);
			break;
		}

		return pData;
	}

	///*! \fn bool CFesData::CheckModifyChannelTagNameExist(const QString &strFesTagName)
	//********************************************************************************************************* 
	//** \brief Config::CFesData::CheckModifyChannelTagNameExist 
	//** \details ����޸ĵ�ͨ��tagname��ǰ�����Ƿ����
	//** \param strFesTagName 
	//** \return bool 
	//** \author gw
	//** \date 2016��10��20�� 
	//** \note 
	//********************************************************************************************************/
	//bool CFesData::CheckModifyChannelTagNameExist(const QString &strChannelTagName)
	//{
	//	auto it = m_arrChannelTagNameMap.find(strChannelTagName.toStdString());
	//	if (it != m_arrChannelTagNameMap.end())
	//	{
	//		return true;
	//	}

	//	return false;
	//}


	/*! \fn CDevice *CFesData::AddNewDeviceItem()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNewDeviceItem 
	** \details 
	** \return CDevice * 
	** \author gw
	** \date 2016��10��25�� 
	** \note 
	********************************************************************************************************/
	CDevice *CFesData::AddNewDeviceItem()
	{
		Q_ASSERT(s_pGlobleCore);
		if (!s_pGlobleCore)
		{
			return nullptr;
		}

		CDevice *pData = new CDevice(s_pGlobleCore);
		Q_ASSERT(pData);
		if (pData == nullptr)
			return nullptr;

		//ǰ���ڵ�װ�ø���
		auto count = GetArrCount(DEVICEHASHID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strDeviceTagName;
		QString strDeviceDesc;
		while (true)
		{
			strDeviceTagName = QString("device%1").arg(count);
			strDeviceDesc = QString(QObject::tr("device%1")).arg(count);


			if (CheckTagNameIsExist(strDeviceTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pData->m_nID = count;

		if (!CheckTagNameIsValid(strDeviceTagName, FES_DESC))
		{
			return nullptr;
		}
		memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
		int nSize = strDeviceTagName.size();
		strncpy(pData->m_szTagName, strDeviceTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		if (!CheckDescIsValid(strDeviceDesc, FES_DESC))
		{
			pData->m_strDescription = strDeviceDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pData->m_strDescription = strDeviceDesc;
		}

		//push device hash
		this->PushFesTagNameHashMap(DEVICEHASHID, strDeviceTagName, pData);

		return pData;
	}


	/*! \fn CAnalogInput *CFesData::AddNewAnalogInput()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNewAnalogInput 
	** \details ���ģ������
	** \return CAnalogInput * 
	** \author gw
	** \date 2016��10��27�� 
	** \note 
	********************************************************************************************************/
	CAnalogInput *CFesData::AddNewAnalogInput()
	{
		auto pAnalogInputData = new CAnalogInput;

		auto count = GetArrCount(AIHASHID);
		QString strAITagName;
		QString strAIDesc;
		while (true)
		{
			strAITagName = QString("AnalogInput%1").arg(count);
			strAIDesc = QString(QObject::tr("AnalogInput%1")).arg(count);


			if (CheckTagNameIsExist(strAITagName))
			{
				count++;

				continue;
			}

			break;
		}

		pAnalogInputData->m_nID = count;

		//У��tagname�����Ƿ���Ч
		if (!CheckTagNameIsValid(strAITagName, FES_DESC))
		{
			return nullptr;
		}
		memset(pAnalogInputData->m_szTagName, 0, sizeof(pAnalogInputData->m_szTagName));
		int nSize = strAITagName.size();
		strncpy(pAnalogInputData->m_szTagName, strAITagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		if (!CheckDescIsValid(strAIDesc, FES_DESC))
		{
			pAnalogInputData->m_strDescription = strAIDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO);
		}
		else
		{
			pAnalogInputData->m_strDescription = strAIDesc;
		}


		//push device hash
		this->PushFesTagNameHashMap(AIHASHID, strAITagName, pAnalogInputData);

		return pAnalogInputData;
	}

	/*! \fn CDigitalInput *CFesData::AddNewDigitalInput()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNewDigitalInput 
	** \details ��ӿ�����
	** \return CDigitalInput * 
	** \author gw
	** \date 2016��10��27�� 
	** \note 
	********************************************************************************************************/
	CDigitalInput *CFesData::AddNewDigitalInput()
	{
		auto pDIData = new CDigitalInput;

		auto count = GetArrCount(DIHASHID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strDITagName;
		QString strDIDesc;
		strDITagName.clear();
		strDIDesc.clear();

		while (true)
		{
			strDITagName = QString("DigitalInput%1").arg(count);
			strDIDesc = QString(QObject::tr("DigitalInput%1")).arg(count);


			if (CheckTagNameIsExist(strDITagName))
			{
				count++;

				continue;
			}

			break;
		}
	
		pDIData->m_nID = count;
		pDIData->m_strDescription = strDIDesc;

		if (!CheckTagNameIsValid(strDITagName, FES_DESC))
		{
			return nullptr;
		}
		memset(pDIData->m_szTagName, 0, sizeof(pDIData->m_szTagName));
		int nSize = strDITagName.size();
		strncpy(pDIData->m_szTagName, strDITagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		if (!CheckDescIsValid(strDIDesc, FES_DESC))
		{
			pDIData->m_strDescription = strDIDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pDIData->m_strDescription = strDIDesc;
		}


		//push device hash
		this->PushFesTagNameHashMap(DIHASHID, pDIData->m_szTagName, pDIData);

		return pDIData;
	}

	/*! \fn CAnalogOutput *CFesData::AddNewAnalogOutput()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNewAnalogOutput 
	** \details ���ģ�������
	** \return CAnalogOutput * 
	** \author gw
	** \date 2016��10��27�� 
	** \note 
	********************************************************************************************************/
	CAnalogOutput *CFesData::AddNewAnalogOutput()
	{
		auto pAOData = new CAnalogOutput;

		auto count = GetArrCount(AOHASHID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strAOTagName;
		QString strAODesc;
		strAOTagName.clear();
		strAODesc.clear();

		while (true)
		{
			strAOTagName = QString("AnalogOutput%1").arg(count);
			strAODesc = QString(QObject::tr("AnalogOutput%1")).arg(count);


			if (CheckTagNameIsExist(strAOTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pAOData->m_nID = count;
		pAOData->m_strDescription = strAOTagName;

		if (!CheckTagNameIsValid(strAOTagName, FES_DESC))
		{
			return nullptr;
		}
		memset(pAOData->m_szTagName, 0, sizeof(pAOData->m_szTagName));
		int nSize = strAOTagName.size();
		strncpy(pAOData->m_szTagName, strAOTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		if (!CheckDescIsValid(strAODesc, FES_DESC))
		{
			pAOData->m_strDescription = strAOTagName.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pAOData->m_strDescription = strAOTagName;
		}

		//push device hash
		this->PushFesTagNameHashMap(AOHASHID, pAOData->m_szTagName, pAOData);

		return pAOData;
	}


	/*! \fn CDigitalOutput *CFesData::AddNewDigitalOutput()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNewDigitalOutput 
	** \details 
	** \return CDigitalOutput * 
	** \author gw
	** \date 2016��10��27�� 
	** \note 
	********************************************************************************************************/
	CDigitalOutput *CFesData::AddNewDigitalOutput()
	{
		auto pDOData = new CDigitalOutput;

		auto count = GetArrCount(DOHASHID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strDOTagName;
		QString strDODesc;
		strDOTagName.clear();
		strDODesc.clear();

		while (true)
		{
			strDOTagName = QString("DigitalOutput%1").arg(count);
			strDODesc = QString(QObject::tr("DigitalOutput%1")).arg(count);


			if (CheckTagNameIsExist(strDOTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pDOData->m_nID = count;

		if (!CheckTagNameIsValid(strDOTagName, FES_DESC))
		{
			return nullptr;
		}
		memset(pDOData->m_szTagName, 0, sizeof(pDOData->m_szTagName));
		int nSize = strDOTagName.size();
		strncpy(pDOData->m_szTagName, strDOTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		if (!CheckDescIsValid(strDODesc, FES_DESC))
		{
			pDOData->m_strDescription = strDODesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pDOData->m_strDescription = strDODesc;
		}

		//push device hash
		this->PushFesTagNameHashMap(DOHASHID, strDOTagName, pDOData);

		return pDOData;
	}


	/*! \fn CTransform *CFesData::AddLinearTransform()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddLinearTransform 
	** \details ǰ���������ת��
	** \return CTransform * 
	** \author gw
	** \date 2016��11��4�� 
	** \note 
	********************************************************************************************************/
	CTransform *CFesData::AddNewLinearTransform()
	{
		auto pData = new CTransform;
		auto pLinear = new CLinearTransform;

		auto count = GetArrCount(TRANSFORMHASHID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strTransformTagName;
		QString strTransformDesc;
		strTransformTagName.clear();
		strTransformDesc.clear();

		while (true)
		{
			strTransformTagName = QString("transform%1").arg(count);
			strTransformDesc = QString(QObject::tr("transform%1")).arg(count);


			if (CheckTagNameIsExist(strTransformTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pLinear->m_nID = count;

		if (!CheckTagNameIsValid(strTransformTagName,FES_DESC))
		{
			return nullptr;
		}
		memset(pLinear->m_szTagName, 0, sizeof(pLinear->m_szTagName));
		int nSize = strTransformTagName.size();
		strncpy(pLinear->m_szTagName, strTransformTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		if (!CheckDescIsValid(strTransformDesc, FES_DESC))
		{
			pLinear->m_strDescription = strTransformDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pLinear->m_strDescription = strTransformDesc;
		}

		pData->SetLinear(pLinear);
		pData->m_nType = ScaleType::LINEAR;
		//push transform hash
		this->PushFesTagNameHashMap(TRANSFORMHASHID, strTransformTagName, pData);

		return pData;
	}


	/*! \fn CTransform *CFesData::AddNonLinearTransform()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNonLinearTransform 
	** \details ǰ����ӷ�����ת��
	** \return CTransform * 
	** \author gw
	** \date 2016��11��4�� 
	** \note 
	********************************************************************************************************/
	CTransform *CFesData::AddNewNonLinearTransform()
	{
		auto pData = new CTransform;
		auto pNonLinear = new CNonlinearTransform;

		auto count = GetArrCount(TRANSFORMHASHID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strTransformTagName;
		QString strTransformDesc;
		strTransformTagName.clear();
		strTransformDesc.clear();

		while (true)
		{
			strTransformTagName = QString("nontransform%1").arg(count);
			strTransformDesc = QString(QObject::tr("nontransform%1")).arg(count);


			if (CheckTagNameIsExist(strTransformTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pNonLinear->m_nID = count;

		if (!CheckTagNameIsValid(strTransformTagName, FES_DESC))
		{
			return nullptr;
		}
		memset(pNonLinear->m_szTagName, 0, sizeof(pNonLinear->m_szTagName));
		int nSize = strTransformTagName.size();
		strncpy(pNonLinear->m_szTagName, strTransformTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		if (!CheckDescIsValid(strTransformDesc, FES_DESC))
		{
			pNonLinear->m_strDescription = strTransformDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pNonLinear->m_strDescription = strTransformDesc;
		}

		pData->SetNonLinear(pNonLinear);
		pData->m_nType = ScaleType::NONLINEAR;
		//push transform hash
		this->PushFesTagNameHashMap(TRANSFORMHASHID, strTransformTagName, pData);

		return pData;
	}


	/*! \fn CAnalogAlarm *CFesData::AddNewAnalogAlarm()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNewAnalogAlarm 
	** \details �����ģ�����澯
	** \return CAnalogAlarm * 
	** \author gw
	** \date 2016��11��11�� 
	** \note 
	********************************************************************************************************/
	CAnalogAlarm *CFesData::AddNewAnalogAlarm()
	{
		QString strTagName;
		QString strDesc;
		strTagName.clear();
		strDesc.clear();

		auto count = GetArrCount(HASHID::ANALOGALARMHASHID);
		while (true)
		{
			strTagName = QString(QObject::tr("analogalarm%1")).arg(count);
			strDesc = QString("analogalarm%1").arg(count);
			
			if (CheckTagNameIsExist(strTagName))
			{
				count++;

				continue;
			}

			break;
		}

		auto pData = new CAnalogAlarm;
		pData->m_nID = count;
		pData->m_nAlarmType = AIALARM;

		if (!CheckTagNameIsValid(strTagName, FES_DESC))
		{
			return nullptr;
		}
		memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
		auto nSize = strTagName.size();
		strncpy(pData->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
	
		if (!CheckDescIsValid(strDesc, FES_DESC))
		{
			pData->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pData->m_strDescription = strDesc;
		}

		//��ӵ�hash����
		PushFesTagNameHashMap(HASHID::ANALOGALARMHASHID, strTagName, pData);

		return pData;
	}


	/*! \fn CAinAlarmLimit *CFesData::AddNewAinAlarmLimit()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNewAinAlarmLimit 
	** \details ����ģ�����澯��ֵ
	** \return CAinAlarmLimit * 
	** \author gw
	** \date 2016��11��11�� 
	** \note 
	********************************************************************************************************/
	CAinAlarmLimit *CFesData::AddNewAinAlarmLimit()
	{
		auto pAnalogAlarmOfflimit = new Config::CAinAlarmLimit;

		int nAlarmCount = GetArrCount(HASHID::ANALOGALARMLIMITHASHID);
		QString strTagname;
		QString strDesc;
		while (true)
		{
			strTagname = QString("%1%2").arg(QObject::tr("analogalarmofflimit")).arg(nAlarmCount);
			strDesc = QString("%1%2").arg(QObject::tr("analogalarmofflimitdesc")).arg(nAlarmCount);

			if (CheckTagNameIsExist(strTagname))
			{
				nAlarmCount++;

				continue;
			}

			break;
		}




		pAnalogAlarmOfflimit->m_nID = nAlarmCount + 1;

		if (!CheckTagNameIsValid(strTagname, FES_DESC))
		{
			return nullptr;
		}
		memset(pAnalogAlarmOfflimit->m_szTagName, 0, sizeof(pAnalogAlarmOfflimit->m_szTagName));
		int nSize = strTagname.size();
		strncpy(pAnalogAlarmOfflimit->m_szTagName, strTagname.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		if (!CheckDescIsValid(strDesc, FES_DESC))
		{
			pAnalogAlarmOfflimit->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pAnalogAlarmOfflimit->m_strDescription = strDesc;
		}
		
		pAnalogAlarmOfflimit->m_nCondition = GREATEREQUAL;
		pAnalogAlarmOfflimit->m_nCategory = AlarmCategory::COMMON;
		pAnalogAlarmOfflimit->m_nAckType = AlarmAckType::DELETE;

		pAnalogAlarmOfflimit->m_nPriority = 0;
		pAnalogAlarmOfflimit->m_strCommands = "";
		pAnalogAlarmOfflimit->m_strSoundFile = "";
		pAnalogAlarmOfflimit->m_bSupportAck = false;

		PushFesTagNameHashMap(HASHID::ANALOGALARMLIMITHASHID, strTagname, pAnalogAlarmOfflimit);

		return pAnalogAlarmOfflimit;
	}


	/*! \fn CDigitalAlarm *CFesData::AddNewDigitalAlarm()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNewDigitalAlarm 
	** \details 
	** \return CDigitalAlarm * 
	** \author gw
	** \date 2016��11��11�� 
	** \note 
	********************************************************************************************************/
	CDigitalAlarm *CFesData::AddNewDigitalAlarm()
	{
		QString strTagName;
		QString strDesc;
		strTagName.clear();
		strDesc.clear();

		auto count = GetArrCount(HASHID::DIGITALALARMHASHID);
		while (true)
		{
			strTagName = QString("digitalalarm%1").arg(count);
			strDesc = QString("digitalalarm%1").arg(count);

			if (CheckTagNameIsExist(strTagName))
			{
				count++;

				continue;
			}

			break;
		}

		auto pData = new CDigitalAlarm;
		pData->m_nID = count;
		pData->m_nAlarmType = DIALARM;

		if (!CheckTagNameIsValid(strTagName, FES_DESC))
		{
			return nullptr;
		}
		memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
		auto nSize = strTagName.size();
		strncpy(pData->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		if (!CheckDescIsValid(strDesc, FES_DESC))
		{
			pData->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pData->m_strDescription = strDesc;
		}


		PushFesTagNameHashMap(HASHID::DIGITALALARMHASHID, strTagName, pData);

		return pData;
	}


	/*! \fn ALARM_OFFLIMIT_DI *CFesData::AddNewDigitalAlarmLimit()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNewDigitalAlarmLimit 
	** \details ����µĿ������澯��ֵ
	** \return ALARM_OFFLIMIT_DI * 
	** \author gw
	** \date 2016��11��11�� 
	** \note 
	********************************************************************************************************/
	CDinAlarmLimit *CFesData::AddNewDigitalAlarmLimit()
	{
		auto pDigtalAlarmOfflimit = new Config::CDinAlarmLimit;

		int nAlarmCount = GetArrCount(HASHID::DIGITALALARMLIMITHASHID);

		QString strTagname;
		QString strDesc;
		strTagname.clear();
		strDesc.clear();

		while (true)
		{
			strTagname = QString("%1%2").arg("digtalalarmlimit").arg(nAlarmCount);
			strDesc = QString("%1%2").arg(QObject::tr("digtalalarmlimit")).arg(nAlarmCount);

			if (CheckTagNameIsExist(strTagname))
			{
				nAlarmCount++;

				continue;
			}

			break;
		}


		pDigtalAlarmOfflimit->ID = nAlarmCount + 1;

		if (!CheckTagNameIsValid(strTagname, FES_DESC))
		{
			return nullptr;
		}
		memset(pDigtalAlarmOfflimit->TagName, 0, sizeof(pDigtalAlarmOfflimit->TagName));
		int nSize = strTagname.size();
		strncpy(pDigtalAlarmOfflimit->TagName, strTagname.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		if (!CheckDescIsValid(strDesc, FES_DESC))
		{
			pDigtalAlarmOfflimit->Description = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pDigtalAlarmOfflimit->Description = strDesc;
		}


		pDigtalAlarmOfflimit->Condition = GREATEREQUAL;

		pDigtalAlarmOfflimit->Priority = 0;
		pDigtalAlarmOfflimit->Commands = "";
		pDigtalAlarmOfflimit->SoundFile = "";
		pDigtalAlarmOfflimit->SupportAck = false;

		PushFesTagNameHashMap(HASHID::DIGITALALARMLIMITHASHID, strTagname, pDigtalAlarmOfflimit);

		return pDigtalAlarmOfflimit;
	}


	/*! \fn CUserVariable *CFesData::AddUserVariable()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddUserVariable 
	** \details 
	** \return CUserVariable * 
	** \author gw
	** \date 2016��12��5�� 
	** \note 
	********************************************************************************************************/
	CUserVariable *CFesData::AddUserVariable()
	{
		auto pUser = new Config::CUserVariable;

		int nCount = GetArrCount(HASHID::USERVIRIABLEID);

		QString strTagname;
		QString strDesc;
		strTagname.clear();
		strDesc.clear();

		while (true)
		{
			strTagname = QString("%1%2").arg("uservariable").arg(nCount + 1);
			strDesc = QString("%1%2").arg(QObject::tr("uservariable")).arg(nCount + 1);

			if (CheckTagNameIsExist(strTagname))
			{
				nCount++;

				continue;
			}

			break;
		}


		pUser->m_nID = nCount + 1;
		if (!CheckTagNameIsValid(strTagname, FES_DESC))
		{
			return nullptr;
		}
		memset(pUser->m_szTagName, 0, sizeof(pUser->m_szTagName));
		int nSize = strTagname.size();
		strncpy(pUser->m_szTagName, strTagname.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		if (!CheckDescIsValid(strDesc, FES_DESC))
		{
			pUser->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pUser->m_strDescription = strDesc;
		}
		
		pUser->m_nType = DATA_TYPE::DT_DOUBLE;
		pUser->m_dbInitValue = 0;
		pUser->m_nInitialQua = InitialQua::NormalQua;
		pUser->SetOccNo(0);

		PushFesTagNameHashMap(HASHID::USERVIRIABLEID, strTagname, pUser);

		return pUser;
	}

	CForwardingChannelData * CFesData::AddNewForwardingChannelItem(CForwardingChannelGroup *pGroup)
	{
		Q_ASSERT(s_pGlobleCore);
		if (!s_pGlobleCore)
		{
			return nullptr;
		}

		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return nullptr;
		}

		CForwardingChannelData *pData = new CForwardingChannelData();
		Q_ASSERT(pData);
		if (pData == nullptr)
			return nullptr;

		//ǰ���ڵ�ͨ������
		auto count = GetArrCount(FORWARDINGCHANNEL);
		if (count < 0)
		{
			return nullptr;
		}

		QString strTagName = "";
		QString strDesc = "";
		while (true)
		{
			strTagName = QString("ForwardingChannel%1").arg(count);
			strDesc = QString(QObject::tr("ForwardingChannel%1")).arg(count);

			std::string str = strTagName.toStdString();

			//tagname�Ƿ���� ���� fes channel device ai di ao do���е�hash��Ҫ���
			if (CheckTagNameIsExist(strTagName))
			{
				count++;

				continue;
			}

			pData->m_nID = count;

			break;
		}

		if (CheckTagNameIsValid(strTagName, FES_DESC))
		{
			memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
			int nSize = strTagName.size();
			strncpy(pData->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

			//���item��group
			pGroup->AddItem(std::string(pData->m_szTagName), pData);
			//push channel hash
			this->PushFesTagNameHashMap(FORWARDINGCHANNEL, pData->m_szTagName, pData);
		}
		else
		{
			return nullptr;
		}

		if (!CheckDescIsValid(strDesc, FES_DESC))
		{
			pData->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pData->m_strDescription = strDesc;
		}

		return pData;
	}

	/*! \fn CForwardingAnalogInputData * CFesData::AddNewForwardingAIItem()
	********************************************************************************************************* 
	** \brief Config::CFesData::AddNewForwardingAIItem 
	** \details ���ת��ң��
	** \return CForwardingAnalogInputData * 
	** \author gw
	** \date 2017��3��27�� 
	** \note 
	********************************************************************************************************/
	CForwardingAnalogInputData * CFesData::AddNewForwardingAIItem()
	{
		auto pAnalogInputData = new CForwardingAnalogInputData;

		auto count = GetArrCount(FORWARDINGAIHASHID);
		QString strAITagName;
		QString strAIDesc;
		while (true)
		{
			strAITagName = QString("ForwardingAnalogInput%1").arg(count);
			strAIDesc = QString(QObject::tr("ForwardingAnalogInput%1")).arg(count);


			if (CheckTagNameIsExist(strAITagName))
			{
				count++;

				continue;
			}

			break;
		}

		pAnalogInputData->m_nID = count;

		if (!CheckTagNameIsValid(strAITagName, FES_DESC))
		{
			return nullptr;
		}
		memset(pAnalogInputData->m_szTagName, 0, sizeof(pAnalogInputData->m_szTagName));
		int nSize = strAITagName.size();
		strncpy(pAnalogInputData->m_szTagName, strAITagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		
		//�����ַ���������������ƣ���ȡ�ַ���
		if (!CheckDescIsValid(strAIDesc, FES_DESC))
		{
			pAnalogInputData->m_strDescription = strAIDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pAnalogInputData->m_strDescription = strAIDesc;
		}

		//push hash
		this->PushFesTagNameHashMap(FORWARDINGAIHASHID, strAITagName, pAnalogInputData);

		return pAnalogInputData;
	}

	CForwardingDigitalInputData * CFesData::AddNewForwardingDIItem()
	{
		auto pDigitalInputData = new CForwardingDigitalInputData;
		auto count = GetArrCount(FORWARDINGDIHASHID);
		QString strDITagName;
		QString strDIDesc;
		while (true)
		{
			strDITagName = QString("ForwardingDigitalInput%1").arg(count);
			strDIDesc = QString(QObject::tr("ForwardingDigitalInput%1")).arg(count);


			if (CheckTagNameIsExist(strDITagName))
			{
				count++;

				continue;
			}

			break;
		}

		pDigitalInputData->m_nID = count;
		
		if (!CheckTagNameIsValid(strDITagName, FES_DESC))
		{
			return nullptr;
		}
		memset(pDigitalInputData->m_szTagName, 0, sizeof(pDigitalInputData->m_szTagName));
		int nSize = strDITagName.size();
		strncpy(pDigitalInputData->m_szTagName, strDITagName.toStdString().c_str(), qMin(nSize, MAX_TAGNAME_LEN_SCADASTUDIO));
	
		//�����ַ���������������ƣ���ȡ�ַ���
		if (!CheckDescIsValid(strDIDesc, FES_DESC))
		{
			pDigitalInputData->m_strDescription = strDITagName.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pDigitalInputData->m_strDescription = strDIDesc;
		}

		//push hash
		this->PushFesTagNameHashMap(FORWARDINGDIHASHID, strDITagName, pDigitalInputData);

		return pDigitalInputData;
	}

	CForwardingDigitalOutputData * CFesData::AddNewForwardingDOItem()
	{
		auto pDOData = new CForwardingDigitalOutputData;

		auto count = GetArrCount(FORWARDINGDOHASHID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strDOTagName;
		QString strDODesc;
		strDOTagName.clear();
		strDODesc.clear();

		while (true)
		{
			strDOTagName = QString("ForwardingDigitalOutput%1").arg(count);
			strDODesc = QString(QObject::tr("ForwardingDigitalOutput%1")).arg(count);


			if (CheckTagNameIsExist(strDOTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pDOData->m_nID = count;

		if (!CheckTagNameIsValid(strDOTagName, FES_DESC))
		{
			return nullptr;
		}

		if (!CheckDescIsValid(strDODesc, FES_DESC))
		{
			pDOData->m_strDescription = strDODesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pDOData->m_strDescription = strDODesc;
		}

		memset(pDOData->m_szTagName, 0, sizeof(pDOData->m_szTagName));
		int nSize = strDOTagName.size();
		strncpy(pDOData->m_szTagName, strDOTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			
		//push device hash
		this->PushFesTagNameHashMap(FORWARDINGDOHASHID, strDOTagName, pDOData);

		return pDOData;
	}

	CForwardingAnalogOutputData * CFesData::AddNewForwardingAOItem()
	{
		auto pAOData = new CForwardingAnalogOutputData;

		auto count = GetArrCount(FORWARDINGDOHASHID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strAOTagName;
		QString strAODesc;
		strAOTagName.clear();
		strAODesc.clear();

		while (true)
		{
			strAOTagName = QString("ForwardingAnalogOutput%1").arg(count);
			strAODesc = QString(QObject::tr("ForwardingAnalogOutput%1")).arg(count);


			if (CheckTagNameIsExist(strAOTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pAOData->m_nID = count;

		if (!CheckTagNameIsValid(strAOTagName, FES_DESC))
		{
			return nullptr;
		}
		else
		{
			memset(pAOData->m_szTagName, 0, sizeof(pAOData->m_szTagName));
			int nSize = strAOTagName.size();
			strncpy(pAOData->m_szTagName, strAOTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		}

		if (!CheckDescIsValid(strAODesc, FES_DESC))
		{
			pAOData->m_strDescription = strAODesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pAOData->m_strDescription = strAODesc;
		}

		//push hash
		this->PushFesTagNameHashMap(FORWARDINGAOHASHID, strAOTagName, pAOData);

		return pAOData;
	}

	CForwardingControlGearData * CFesData::AddNewForwardingCGItem()
	{
		auto pCGData = new CForwardingControlGearData;

		auto count = GetArrCount(FORWARDINGCGHASHID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strCGTagName;
		QString strCGDesc;
		strCGTagName.clear();
		strCGDesc.clear();

		while (true)
		{
			strCGTagName = QString("ForwardingControlGear%1").arg(count);
			strCGDesc = QString(QObject::tr("ForwardingControlGear%1")).arg(count);


			if (CheckTagNameIsExist(strCGTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pCGData->m_nID = count;

		if (!CheckTagNameIsValid(strCGTagName, FES_DESC))
		{
			return nullptr;
		}
		else
		{
			memset(pCGData->m_szTagName, 0, sizeof(pCGData->m_szTagName));
			int nSize = strCGTagName.size();
			strncpy(pCGData->m_szTagName, strCGTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		}

		if (!CheckDescIsValid(strCGDesc, FES_DESC))
		{
			strCGDesc = strCGDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pCGData->m_strDescription = strCGDesc;
		}

		//push hash
		this->PushFesTagNameHashMap(FORWARDINGCGHASHID, strCGTagName, pCGData);

		return pCGData;
	}

	CForwardingKWHData * CFesData::AddNewForwardingKWHItem()
	{
		auto pKWHData = new CForwardingKWHData;

		auto count = GetArrCount(FORWARDINGKWHHASHID);
		if (count < 0)
		{
			return nullptr;
		}

		QString strKWHTagName;
		QString strKWHDesc;
		strKWHTagName.clear();
		strKWHDesc.clear();

		while (true)
		{
			strKWHTagName = QString("ForwardingKWH%1").arg(count);
			strKWHDesc = QString(QObject::tr("ForwardingKWH%1")).arg(count);


			if (CheckTagNameIsExist(strKWHTagName))
			{
				count++;

				continue;
			}

			break;
		}

		pKWHData->m_nID = count;

		if (!CheckTagNameIsValid(strKWHTagName, FES_DESC))
		{
			return nullptr;
		}
		else
		{
			memset(pKWHData->m_szTagName, 0, sizeof(pKWHData->m_szTagName));
			int nSize = strKWHTagName.size();
			strncpy(pKWHData->m_szTagName, strKWHTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		}

		if (!CheckDescIsValid(strKWHDesc, FES_DESC))
		{
			pKWHData->m_strDescription = strKWHDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
		}
		else
		{
			pKWHData->m_strDescription = strKWHDesc;
		}

		//push hash
		this->PushFesTagNameHashMap(FORWARDINGKWHHASHID, strKWHTagName, pKWHData);

		return pKWHData;
	}

	/*! \fn bool CFesData::PushFesTagNameHashMap(const QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::PushFesTagNameHashMap 
	** \details fes hash
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016��10��27�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::PushFesTagNameHashMap(int nIDD, const QString &strTagName, CBaseData *pData)
	{
		auto bFlag = nIDD < 0 || nIDD >= FES_HASH_TYPE_NUM::MAX_IDD_NUM;
		Q_ASSERT(!bFlag);
		if (nIDD < 0 || nIDD >= FES_HASH_TYPE_NUM::MAX_IDD_NUM)
		{
			return false;
		}

		bFlag = CheckTagNameIsValid(strTagName, FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}

		Q_ASSERT(pData);
		if (!pData)
		{
			return false;
		}

		m_arrMaps[nIDD].insert(make_pair(strTagName.toStdString(), pData));

		return true;
	}


	/*! \fn bool CFesData::CheckTagNameIsExist(const QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::CheckTagNameIsExist 
	** \details ���tagname�Ƿ���� fes channel device ai di ao do
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016��10��27�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::CheckTagNameIsExist(const QString &strTagName)
	{
		auto szTmp = strTagName.toStdString();
		for (int i = 0; i < FES_HASH_TYPE_NUM::MAX_IDD_NUM; i++)
		{
			auto it = m_arrMaps[i].find(szTmp);
			if (it != m_arrMaps[i].end())
			{
				return true;
			}
		}

		return false;
	}


	/*! \fn CBaseData *CFesData::GetData()
	********************************************************************************************************* 
	** \brief Config::CFesData::GetData 
	** \details ͨ��tagname key�ҵ�value
	** \return CBaseData * 
	** \author gw
	** \date 2016��11��18�� 
	** \note 
	********************************************************************************************************/
	CBaseData *CFesData::GetData(QString &strTagName, int nType)
	{
		auto it = m_arrMaps[nType].find(strTagName.toStdString());
		if (it != m_arrMaps[nType].end())
		{
			return it->second;
		}

		return nullptr;
	}

	/*! \fn CBaseData *CFesData::GetDataFromAllData(QString &strTagName, int &nType)
	********************************************************************************************************* 
	** \brief Config::CFesData::GetDataFromAllData 
	** \details 
	** \param strTagName NodeTagName
	** \param nType output��������, ��AIN��DIN��AOut��DOut
	** \return CBaseData * 
	** \author gw
	** \date 2017��2��28�� 
	** \note 

	//fesdatadef.h studio��Ӧǰ��IDD_TYPE
	enum IDD_TYPE : int32u
	{
	IDD_NULL = 0,
	IDD_AIN = 1,  //!< ģ����
	IDD_DIN = 2,  //!< ������
	IDD_AOUT = 3, //!< ģ����
	IDD_DOUT = 4, //!< ����
	IDD_USERVAR = 5, //!< �û��Զ������
	IDD_SYSVAR = 6,  //!< ϵͳ����


	IDD_NODE = 7,
	IDD_CHANNEL = 10,
	IDD_DEVICE = 11,
	IDD_OBJECT = 12,  //!< ����
	IDD_OBJECT_VAR = 13, //!< �����ڲ�����

	IDD_AINALARM = 15,
	IDD_DINALARM = 16,

	IDD_AINALARMLIMIT = 17,
	IDD_DINALARMLIMIT = 18,

	IDD_TRANSLINEAR = 19,
	IDD_TRANSNONLINEAR = 20,

	MAX_IDD = IDD_TRANSNONLINEAR,
	};
	********************************************************************************************************/
	CBaseData *CFesData::GetDataFromAllData(QString &strTagName, int &nType)
	{
		for (int i = 0; i < FES_HASH_TYPE_NUM::MAX_IDD_NUM; i++)
		{
			auto it = m_arrMaps[i].find(strTagName.toStdString());
			if (it != m_arrMaps[i].end())
			{	
				nType = i;

				switch (nType)
				{
				case HASHID::AIHASHID:
				{
					nType = IDD_TYPE::IDD_AIN;
					return it->second;
				}
				case HASHID::DIHASHID:
				{
					nType = IDD_TYPE::IDD_DIN;
					return it->second;
				}
				case HASHID::AOHASHID:
				{
					nType = IDD_TYPE::IDD_AOUT;
					return it->second;
				}
				case HASHID::DOHASHID:
				{
					nType = IDD_TYPE::IDD_DOUT;
					return it->second;
				}
				case HASHID::FESHASHID:
				{
					return it->second;
				}
				case HASHID::SYSTEMVARIABLEID:
				{
					nType = IDD_TYPE::IDD_SYSVAR;
					return it->second;
				}
				case HASHID::USERVIRIABLEID:
				{
					nType = IDD_TYPE::IDD_USERVAR;
					return it->second;
				}
				default:
					break;
				}


			}
		}

		return nullptr;
	}

	/*! \fn QString &CFesData::GetSourceOccNo(QString &strTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::GetSourceOccNo 
	** \details 
	** \param strTagName 
	** \return QString & 
	** \author gw
	** \date 2017��2��28�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::GetUserVariableRelatedSourceOccNo(const QString &strTagName, const QString &strRelatedTagName, QString &strOutput)
	{
		auto bFlag = strRelatedTagName.isEmpty() || strRelatedTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			auto strError = QObject::tr("User variable tagname %1 related sourcetagname is invalid!!!").arg(strTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		if (strTagName.isEmpty())
		{
			return false;
		}
		
		//[].[].[]��ʱ�������
		auto &list = strRelatedTagName.split(".");
		if (list.size() == 3)
		{
			//
			auto &strNodeTagName = list[0];
			auto &strTagName = list[1];
			auto &strAttrTagName = list[2];
			int nNodeType = -1;

			auto pNode = m_pCore->GetModule("node_config");
			Q_ASSERT(pNode);
			if (!pNode)
			{
				return nullptr;
			}
			//node -> fes
			std::string szFesTagName = pNode->GetFesConfigNameByNodeName(strNodeTagName.toStdString());
			//Q_ASSERT(!szFesTagName.empty());;
			if (szFesTagName.empty())
			{
				QMessageBox box;
				box.setText(QObject::tr("Fes node %1 do not relate to fes!!!").arg(strNodeTagName));
				box.exec();
				
				return false;
			}

			auto pNodeData = GetDataFromAllData(QString("%1").arg(szFesTagName.c_str()), nNodeType);
			auto strError = QObject::tr("User variable tagname %1 related sourcetagname can not find!!!").arg(strTagName);
			//ǰ���û�����������ֻ��ǰ��ģ��������������������ж�
			Q_ASSERT(pNodeData /*&& nNodeType != -1*/);
			if (!pNodeData)
			{
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
				return false;
			}
			auto pNodeOccNo = pNodeData->GetOccNo();
			auto bFlag = pNodeOccNo > 0 && pNodeOccNo <= MAX_OCCNO_SCADASTUDIO;
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
				return false;
			}
			int nType = -1;
			auto pData = GetDataFromAllData(strTagName, nType);
			Q_ASSERT(pData && nType != -1);
			if (!pData)
			{
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
				return false;
			}
			auto pOccNo = pData->GetOccNo();
			bFlag = pOccNo > 0 && pOccNo <= MAX_OCCNO_SCADASTUDIO;
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
				return "";
			}

			CTagAttMgr attr;
			int32u nAttrOccNo = -1;;
			bFlag = attr.GetAttIndex(nType, strAttrTagName.toStdString().c_str(), nAttrOccNo);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
				return false;
			}

			strOutput = QString("%1.%2:%3.%4").arg(pNodeOccNo).arg(nType).arg(pOccNo).arg(nAttrOccNo);
			return true;
		}
		

		return false;
	}

	/*! \fn bool CFesData::ModifyChannelHashMap(const QString &strChannelTagName, const QString &strLastChannelTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyChannelHashMap 
	** \details ǰ��ͳһ����ͨ��hash �޸�key
	** \param strChannelTagName 
	** \param strLastChannelTagName 
	** \return bool 
	** \author gw
	** \date 2016��10��20�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyFesHashMap(int32u nIDD, const QString &strTagName, const QString &strLastTagName)
	{
		//ǰ�ù���hash tagname key�޸�
		auto &arr = m_arrMaps[nIDD];
		
		auto it = arr.find(strLastTagName.toStdString());
		if (it != arr.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(arr[strTagName.toStdString()], it->second);
			// Erase old key-value from map
			arr.erase(it);

			return true;
		}
		else
		{
			return false;
		}

		return false;
	}


	/*! \fn bool CFesData::DeleteChannelHashItem(QString &strChannelTagnname)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteChannelHashItem 
	** \details ɾ��ǰ��hash ��ɾ���ڴ�
	** \param strChannelTagnname 
	** \return bool 
	** \author gw
	** \date 2016��10��26�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteFesHashItem(int nIDD, const QString &strTagnname)
	{
		bool bFlag = CheckTagNameIsValid(strTagnname, FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}

		auto &arr = m_arrMaps[nIDD];
		
		auto it = arr.find(strTagnname.toStdString());
		if (it != arr.end())
		{
			arr.erase(it);

			return true;
		}

		return false;
	}

	/*! \fn bool CFesData::AddAIRelationScaleArr(std::string &tagName, CAnalogInput *pAnalogInput)
	********************************************************************************************************* 
	** \brief Config::CFesData::AddAIRelationScaleArr 
	** \details 
	** \param tagName 
	** \param pAnalogInput 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::AddAIRelationScaleArr(std::string &tagName, CAnalogInput *pAnalogInput)
 	{
		if (tagName.empty())
		{
			return true;
		}
		
		auto it = m_arrScaleToAIMap.find(tagName);
		if (it != m_arrScaleToAIMap.end())
		{
			it->second.push_back(pAnalogInput);

			return true;
		}

		std::vector<CAnalogInput *> vec;
		vec.clear();
		vec.push_back(pAnalogInput);
		m_arrScaleToAIMap.insert(make_pair(tagName, vec));

		return true;
	}

	/*! \fn bool CFesData::ModifyAIsRelationScaleArr(std::string &tagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyAIsRelationScaleArr 
	** \details 
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyAIsRelationScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	{
		auto it = m_arrScaleToAIMap.find(lastTagName);
		if (it != m_arrScaleToAIMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strScaleTagName = tagName.c_str();
				var->m_strScaleDesc = desc.c_str();
				var->m_nScaleType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrScaleToAIMap[tagName], it->second);
			// Erase old key-value from map
			m_arrScaleToAIMap.erase(it);

			return true;
		}

		return false;
	}

	bool CFesData::ModifyAIsScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	{
		auto it = m_arrScaleToAIMap.find(lastTagName);
		if (it != m_arrScaleToAIMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strScaleTagName = tagName.c_str();
				var->m_strScaleDesc = desc.c_str();
				var->m_nScaleType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrScaleToAIMap[tagName], it->second);
			// Erase old key-value from map
			m_arrScaleToAIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyAIsRelationScaleDescArr(std::string &tagName, std::string &desc)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyAIsRelationScaleDescArr 
	** \details 
	** \param tagName 
	** \param desc 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyAIsRelationScaleDescArr(std::string &tagName, std::string &desc)
	{
		auto bFlag = tagName.empty() || tagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return false;
		}
		
		auto it = m_arrScaleToAIMap.find(tagName);
		if (it != m_arrScaleToAIMap.end())
		{
			auto &arr = it->second;
			
			for each (auto var in arr)
			{
				var->m_strScaleDesc = desc.c_str();
			}
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteAIsRelationScaleArr(std::string &tagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteAIsRelationScaleArr 
	** \details 
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteAIsScaleArr(const std::string &szTagName)
	{
		auto bFlag = CheckTagNameIsValid(szTagName.c_str(), FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}
		
		auto it = m_arrScaleToAIMap.find(szTagName);
		if (it != m_arrScaleToAIMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strScaleTagName = "";
				var->m_strScaleDesc = "";
			}

			m_arrScaleToAIMap.erase(it);
		}



		return true;
	}

	/*! \fn bool CFesData::DeleteAIsRelationScaleArr(const std::string &szTagName, CAnalogInput *pAnalogInput)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteAIsRelationScaleArr 
	** \details ǰ��AIɾ�������AI Scale tagname�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param szTagName 
	** \param pAnalogInput 
	** \return bool 
	** \author gw
	** \date 2017��5��20�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteAIsRelationScaleArr(const std::string &szTagName, CAnalogInput *pAnalogInput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pAnalogInput);
		if (!pAnalogInput)
		{
			return false;
		}

		//����ת��key�Ƿ����
		auto it = m_arrScaleToAIMap.find(szTagName);
		//�а󶨹�ϵ
		if (it != m_arrScaleToAIMap.end())
		{
			auto &arr = it->second;
			//�û�����vec
			auto ite = std::find(arr.begin(), arr.end(), pAnalogInput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrScaleToAIMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::AddAIRelationAlarmArr(std::string &tagName, CAnalogAlarm *pAnalogAlarm)
	********************************************************************************************************* 
	** \brief Config::CFesData::AddAIRelationAlarmArr 
	** \details 
	** \param tagName 
	** \param pAnalogAlarm 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::AddAIRelationAlarmArr(std::string &tagName, CAnalogInput *pAnalogInput)
	{
		if (tagName.empty())
		{
			return true;
		}

		auto it = m_arrAlarmToAIMap.find(tagName);
		if (it != m_arrAlarmToAIMap.end())
		{
			it->second.push_back(pAnalogInput);

			return true;
		}

		std::vector<CAnalogInput *> vec;
		vec.clear();
		vec.push_back(pAnalogInput);
		m_arrAlarmToAIMap.insert(make_pair(tagName, vec));

		return true;
	}

	/*! \fn bool CFesData::ModifyAIsRelationAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyAIsRelationAlarmArr 
	** \details 
	** \param tagName 
	** \param lastTagName 
	** \param desc 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyAIsRelationAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, Config::CAnalogInput *pAnalogInput)
	{
		Q_UNUSED(desc);

		Q_ASSERT(pAnalogInput);
		if (!pAnalogInput)
		{
			return false;
		}

		auto it = m_arrAlarmToAIMap.find(lastTagName);
		if (it != m_arrAlarmToAIMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pAnalogInput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrAlarmToAIMap.find(tagName);

				pAnalogInput->m_strAlarmTagName = tagName.c_str();
				if (itee != m_arrAlarmToAIMap.end())
				{
					itee->second.push_back(pAnalogInput);
				}
				else
				{
					std::vector<CAnalogInput *> vec;
					vec.clear();
					vec.push_back(pAnalogInput);
					m_arrAlarmToAIMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyAIsRelationAlarmDescArr(std::string &tagName, std::string &desc)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyAIsRelationAlarmDescArr 
	** \details 
	** \param tagName 
	** \param desc 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyAIsRelationAlarmDescArr(std::string &tagName, std::string &desc)
	{
		auto it = m_arrAlarmToAIMap.find(tagName);
		if (it != m_arrAlarmToAIMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strAlarmDesc = desc.c_str();
			}
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteAIsRelationAlarmArr(std::string &tagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteAIsRelationAlarmArr 
	** \details 
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyAIsAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	{
		auto it = m_arrAlarmToAIMap.find(lastTagName);
		if (it != m_arrAlarmToAIMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strAlarmTagName = tagName.c_str();
				var->m_strAlarmDesc = desc.c_str();
				var->m_nAlarmType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrAlarmToAIMap[tagName], it->second);
			// Erase old key-value from map
			m_arrAlarmToAIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteAIsRelationAlarmArr(std::string &tagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteAIsRelationAlarmArr 
	** \details ɾ��ģ�����澯,AIs��������ĸ澯����ɾ��
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2017��2��13�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteAIsAlarmArr(const std::string &szTagName)
	{
		auto it = m_arrAlarmToAIMap.find(szTagName);
		if (it != m_arrAlarmToAIMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strAlarmTagName = "";
				var->m_strAlarmDesc = "";
			}

			m_arrAlarmToAIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteAIsRelationAlarmArr(const std::string &szTagName, CAnalogInput *pAnalogInput)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteAIsRelationAlarmArr 
	** \details 
	** \param szTagName 
	** \param pAnalogInput 
	** \return bool 
	** \author gw
	** \date 2017��5��20�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteAIsRelationAlarmArr(const std::string &szTagName, CAnalogInput *pAnalogInput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pAnalogInput);
		if (!pAnalogInput)
		{
			return false;
		}

		//����ת��key�Ƿ����
		auto it = m_arrAlarmToAIMap.find(szTagName);
		//�а󶨹�ϵ
		if (it != m_arrAlarmToAIMap.end())
		{
			auto &arr = it->second;
			//�û�����vec
			auto ite = std::find(arr.begin(), arr.end(), pAnalogInput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrAlarmToAIMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::AddDIRelationDoubleDIArr(std::string &tagName, CDigitalInput *pDigitalInput)
	********************************************************************************************************* 
	** \brief Config::CFesData::AddDIRelationDoubleDIArr 
	** \details DI������ң�� ��ӹ�����ϵ
	** \param tagName 
	** \param pDigitalInput 
	** \return bool 
	** \author gw
	** \date 2017��5��22�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::AddDIRelationDoubleDIArr(const std::string &szTagName, CDigitalInput *pDigitalInput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pDigitalInput);
		if (!pDigitalInput)
		{
			return false;
		}

		if (szTagName.empty())
		{
			return true;
		}

		auto it = m_arrDIToDoubleDIMap.find(szTagName);
		if (it != m_arrDIToDoubleDIMap.end())
		{
			it->second.push_back(pDigitalInput);

			return true;
		}

		std::vector<CDigitalInput *> vec;
		vec.clear();
		vec.push_back(pDigitalInput);
		m_arrDIToDoubleDIMap.insert(make_pair(szTagName, vec));

		return true;
	}

	/*! \fn bool CFesData::ModifyDIRelationDoubleDIArr(const std::string &szTagName, const std::string &szLastTagName, CDigitalInput *pDigitalInput)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyDIRelationDoubleDIArr 
	** \details DI������ң�� �޸Ĺ�����ϵ
	** \param szTagName 
	** \param szLastTagName 
	** \param pDigitalInput 
	** \return bool 
	** \author gw
	** \date 2017��5��22�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyDIRelationDoubleDIArr(const std::string &szTagName, const std::string &szLastTagName, CDigitalInput *pDigitalInput)
	{
		Q_ASSERT(pDigitalInput);
		if (!pDigitalInput)
		{
			return false;
		}

		auto it = m_arrDIToDoubleDIMap.find(szLastTagName);
		if (it != m_arrDIToDoubleDIMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pDigitalInput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrDIToDoubleDIMap.find(szTagName);

				memset(pDigitalInput->m_szDoubleDI, 0, sizeof(pDigitalInput->m_szDoubleDI));
				int nSize = szTagName.size();
				strncpy(pDigitalInput->m_szDoubleDI, szTagName.c_str(), qMin(nSize, MAX_TAGNAME_LEN_SCADASTUDIO));
				
				if (itee != m_arrDIToDoubleDIMap.end())
				{
					itee->second.push_back(pDigitalInput);
				}
				else
				{
					std::vector<CDigitalInput *> vec;
					vec.clear();
					vec.push_back(pDigitalInput);
					m_arrDIToDoubleDIMap.insert(make_pair(szTagName, vec));
				}
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyDIDoubleDIArr(const std::string &szTagName, const std::string &szLastTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyDIDoubleDIArr 
	** \details ң��tagname�޸ģ����doubledi�Ĵ�ң�Ź�����ң�ţ���ô�������Ĵ�ң��Ҳ�����޸�
	** \param szTagName 
	** \param szLastTagName 
	** \return bool 
	** \author gw
	** \date 2017��5��22�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyDIDoubleDIArr(const std::string &szTagName, const std::string &szLastTagName)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC) || !CheckTagNameIsValid(szLastTagName.c_str(), FES_DESC))
		{
			return false;
		}
		
		auto it = m_arrDIToDoubleDIMap.find(szLastTagName);
		if (it != m_arrDIToDoubleDIMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				memset(var->m_szDoubleDI, 0, sizeof(var->m_szDoubleDI));
				int nSize = szTagName.size();
				strncpy(var->m_szDoubleDI, szTagName.c_str(), qMin(nSize, MAX_TAGNAME_LEN_SCADASTUDIO));
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrDIToDoubleDIMap[szTagName], it->second);
			// Erase old key-value from map
			m_arrDIToDoubleDIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteDIDoubleDIArr(const std::string &szTagName, const std::string &szLastTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteDIDoubleDIArr 
	** \details ���doubledi�Ĵ�ң���й�������ô�������Ĵ�ң��hashҲ����ɾ��
	** \param szTagName 
	** \param pDigitalInput 
	** \return bool 
	** \author gw
	** \date 2017��5��22�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteDIRelationDoubleDIArr(const std::string &szTagName, CDigitalInput *pDigitalInput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pDigitalInput);
		if (!pDigitalInput)
		{
			return false;
		}

		//����ת��key�Ƿ����
		auto it = m_arrDIToDoubleDIMap.find(szTagName);
		//�а󶨹�ϵ
		if (it != m_arrDIToDoubleDIMap.end())
		{
			auto &arr = it->second;
			//�û�����vec
			auto ite = std::find(arr.begin(), arr.end(), pDigitalInput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrDIToDoubleDIMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteDIDoubleDIArr(const std::string &szTagName, CDigitalInput *pDigitalInput)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteDIDoubleDIArr 
	** \details di tagnameɾ��������д�ң�Ź�������ô�������Ĵ�ң��hash���ɾ��
	** \param szTagName 
	** \param pDigitalInput 
	** \return bool 
	** \author gw
	** \date 2017��5��22�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteDIDoubleDIArr(const std::string &szTagName, CDigitalInput *pDigitalInput)
	{
		auto it = m_arrDIToDoubleDIMap.find(szTagName);
		if (it != m_arrDIToDoubleDIMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				memset(var->m_szDoubleDI, 0, sizeof(var->m_szDoubleDI));
			}

			m_arrDIToDoubleDIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::AddDIRelationAlarmArr(std::string &tagName, CDigitalInput *pDigitalInput)
	********************************************************************************************************* 
	** \brief Config::CFesData::AddDIRelationAlarmArr 
	** \details 
	** \param tagName 
	** \param pDigitalInput 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::AddDIRelationAlarmArr(std::string &tagName, CDigitalInput *pDigitalInput)
	{
		bool bFlag = CheckTagNameIsValid(tagName.c_str(), FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}

		auto it = m_arrAlarmToDIMap.find(tagName);
		if (it != m_arrAlarmToDIMap.end())
		{
			it->second.push_back(pDigitalInput);

			return true;
		}

		std::vector<CDigitalInput *> vec;
		vec.clear();
		vec.push_back(pDigitalInput);
		m_arrAlarmToDIMap.insert(make_pair(tagName, vec));

		return true;
	}

	/*! \fn bool CFesData::ModifyDIsRelationAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyDIsRelationAlarmArr 
	** \details �޸Ŀ������澯 di��������й����ĸ澯һ�������޸�
	** \param tagName 
	** \param lastTagName 
	** \param desc 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyDIsAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	{
		auto it = m_arrAlarmToDIMap.find(lastTagName);
		if (it != m_arrAlarmToDIMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strAlarmTag = tagName.c_str();
				var->m_strAlarmDesc = desc.c_str();
				var->m_nAlarmType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrAlarmToDIMap[tagName], it->second);
			// Erase old key-value from map
			m_arrAlarmToDIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyDIsAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyDIsAlarmArr 
	** \details DIs��������澯����DIs�����޸�DIs������ĸ澯
	** \param tagName 
	** \param lastTagName 
	** \param desc 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2017��2��13�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyDIsRelationAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, Config::CDigitalInput *pDigitalInput)
	{
		Q_UNUSED(desc);
		
		Q_ASSERT(pDigitalInput);
		if (!pDigitalInput)
		{
			return false;
		}
		
		auto it = m_arrAlarmToDIMap.find(lastTagName);
		if (it != m_arrAlarmToDIMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pDigitalInput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrAlarmToDIMap.find(tagName);

				pDigitalInput->m_strAlarmTag = tagName.c_str();
				if (itee != m_arrAlarmToDIMap.end())
				{
					itee->second.push_back(pDigitalInput);
				}
				else
				{
					std::vector<CDigitalInput *> vec;
					vec.clear();
					vec.push_back(pDigitalInput);
					m_arrAlarmToDIMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	bool CFesData::ModifyDIsRelationAlarmDescArr(std::string &tagName, std::string &desc)
	{
		auto it = m_arrAlarmToDIMap.find(tagName);
		if (it != m_arrAlarmToDIMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strAlarmDesc = desc.c_str();
			}
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteDIsRelationAlarmArr(std::string &tagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteDIsRelationAlarmArr 
	** \details 
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteDIsAlarmArr(const std::string &szTagName)
	{
		auto it = m_arrAlarmToDIMap.find(szTagName);
		if (it != m_arrAlarmToDIMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strAlarmTag = "";
				var->m_strAlarmDesc = "";
			}

			m_arrAlarmToDIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteDIsRelationAlarmArr(const std::string &szTagName, CDigitalInput *pDigitalInput)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteDIsRelationAlarmArr 
	** \details ǰ��DIɾ�������Alarm tagname�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param szTagName 
	** \param pDigitalInput 
	** \return bool 
	** \author gw
	** \date 2017��5��20�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteDIsRelationAlarmArr(const std::string &szTagName, CDigitalInput *pDigitalInput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pDigitalInput);
		if (!pDigitalInput)
		{
			return false;
		}

		//����ת��key�Ƿ����
		auto it = m_arrAlarmToDIMap.find(szTagName);
		//�а󶨹�ϵ
		if (it != m_arrAlarmToDIMap.end())
		{
			auto &arr = it->second;
			//�û�����vec
			auto ite = std::find(arr.begin(), arr.end(), pDigitalInput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrAlarmToDIMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::AddAORelationAlarmArr(std::string &tagName, CAnalogOutput *pAnalogOutput)
	********************************************************************************************************* 
	** \brief Config::CFesData::AddAORelationAlarmArr 
	** \details 
	** \param tagName 
	** \param pAnalogOutput 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::AddAORelationScaleArr(std::string &tagName, CAnalogOutput *pAnalogOutput)
	{
		if (tagName.empty())
		{
			return true;
		}

		auto it = m_arrScaleToAOMap.find(tagName);
		if (it != m_arrScaleToAOMap.end())
		{
			it->second.push_back(pAnalogOutput);

			return true;
		}

		std::vector<CAnalogOutput *> vec;
		vec.clear();
		vec.push_back(pAnalogOutput);
		m_arrScaleToAOMap.insert(make_pair(tagName, vec));

		return true;
	}

	/*! \fn bool CFesData::ModifyAOsRelationAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyAOsRelationAlarmArr 
	** \details 
	** \param tagName 
	** \param lastTagName 
	** \param desc 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyAOsRelationScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, Config::CAnalogOutput *pAnalogOutput)
	{
		Q_UNUSED(desc);

		Q_ASSERT(pAnalogOutput);
		if (!pAnalogOutput)
		{
			return false;
		}

		auto it = m_arrScaleToAOMap.find(lastTagName);
		if (it != m_arrScaleToAOMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pAnalogOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrScaleToAOMap.find(tagName);

				pAnalogOutput->m_strScaleTagName = tagName.c_str();
				if (itee != m_arrScaleToAOMap.end())
				{
					itee->second.push_back(pAnalogOutput);
				}
				else
				{
					std::vector<CAnalogOutput *> vec;
					vec.clear();
					vec.push_back(pAnalogOutput);
					m_arrScaleToAOMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyAOsScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyAOsScaleArr 
	** \details �޸�ת�� AOs��������й�����ת�� ת�������޸�
	** \param tagName 
	** \param lastTagName 
	** \param desc 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2017��2��16�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyAOsScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	{
		auto it = m_arrScaleToAOMap.find(lastTagName);
		if (it != m_arrScaleToAOMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strScaleTagName = tagName.c_str();
				var->m_strScaleDesc = desc.c_str();
				var->m_nScaleType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrScaleToAOMap[tagName], it->second);
			// Erase old key-value from map
			m_arrScaleToAOMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyAOsRelationAlarmDescArr(std::string &tagName, std::string &desc)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyAOsRelationAlarmDescArr 
	** \details 
	** \param tagName 
	** \param desc 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyAOsRelationScaleDescArr(std::string &tagName, std::string &desc)
	{
		auto it = m_arrScaleToAOMap.find(tagName);
		if (it != m_arrScaleToAOMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strScaleDesc = desc.c_str();
			}
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteAOsRelationScaleArr(std::string &tagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteAOsRelationScaleArr 
	** \details 
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2016��12��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteAOsScaleArr(const std::string &szTagName)
	{
		auto it = m_arrScaleToAOMap.find(szTagName);
		if (it != m_arrScaleToAOMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strScaleTagName = "";
				var->m_strScaleDesc = "";
			}

			m_arrScaleToAOMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteAOsRelationScaleArr(std::string &tagName, CAnalogOutput *pAnalogOutput)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteAOsRelationScaleArr 
	** \details ǰ��AOɾ�������Scale�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param tagName 
	** \param pAnalogOutput 
	** \return bool 
	** \author gw
	** \date 2017��5��20�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteAOsRelationScaleArr(const std::string &szTagName, CAnalogOutput *pAnalogOutput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pAnalogOutput);
		if (!pAnalogOutput)
		{
			return false;
		}

		//����ת��key�Ƿ����
		auto it = m_arrScaleToAOMap.find(szTagName);
		//�а󶨹�ϵ
		if (it != m_arrScaleToAOMap.end())
		{
			auto &arr = it->second;
			//�û�����vec
			auto ite = std::find(arr.begin(), arr.end(), pAnalogOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrScaleToAOMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::AddDORelationAssociatedDIArr(std::string &tagName, CDigitalOutput *pDigitalOutput)
	********************************************************************************************************* 
	** \brief Config::CFesData::AddDORelationAssociatedDIArr 
	** \details fes do AssociatedDI��ӹ�����ϵ ����ң�ŵ���
	** \param tagName 
	** \param pDigitalOutput 
	** \return bool 
	** \author gw
	** \date 2017��5��22�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::AddDORelationAssociatedDIArr(const std::string &szTagName, CDigitalOutput *pDigitalOutput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}
		

		auto it = m_arrDOToAssociatedDIMap.find(szTagName);
		if (it != m_arrDOToAssociatedDIMap.end())
		{
			it->second.push_back(pDigitalOutput);

			return true;
		}

		std::vector<CDigitalOutput *> vec;
		vec.clear();
		vec.push_back(pDigitalOutput);
		m_arrDOToAssociatedDIMap.insert(make_pair(szTagName, vec));

		return true;
	}

	/*! \fn bool CFesData::ModifyDORelationAssociatedDIArr(const std::string &szTagName, std::string &lastTagName, CDigitalOutput *pDigitalOutput)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyDORelationAssociatedDIArr 
	** \details fes do AssociatedDI�޸Ĺ�����ϵ ����ң�ŵ���
	** \param szTagName 
	** \param lastTagName 
	** \param pDigitalOutput 
	** \return bool 
	** \author gw
	** \date 2017��5��22�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyDORelationAssociatedDIArr(const std::string &szTagName, const std::string &szLastTagName, CDigitalOutput *pDigitalOutput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC) || !CheckTagNameIsValid(szLastTagName.c_str(), FES_DESC))
		{
			return false;
		}
		
		Q_ASSERT(pDigitalOutput);
		if (!pDigitalOutput)
		{
			return false;
		}

		auto it = m_arrDOToAssociatedDIMap.find(szLastTagName);
		if (it != m_arrDOToAssociatedDIMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pDigitalOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrDOToAssociatedDIMap.find(szTagName);

				memset(pDigitalOutput->m_szAssociatedDI, 0, sizeof(pDigitalOutput->m_szAssociatedDI));
				int nSize = szTagName.size();
				strncpy(pDigitalOutput->m_szAssociatedDI, szTagName.c_str(), qMin(nSize, MAX_TAGNAME_LEN_SCADASTUDIO));

				if (itee != m_arrDOToAssociatedDIMap.end())
				{
					itee->second.push_back(pDigitalOutput);
				}
				else
				{
					std::vector<CDigitalOutput *> vec;
					vec.clear();
					vec.push_back(pDigitalOutput);
					m_arrDOToAssociatedDIMap.insert(make_pair(szTagName, vec));
				}
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrDOToAssociatedDIMap.erase(it);
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyDOAssociatedDIArr(const std::string &szTagName, const std::string &lastTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyDOAssociatedDIArr 
	** \details	ǰ��DI�޸ģ�ǰ��DO AssociatedDI������ң�ţ���ô������ϵ�޸� 1�Զ��ϵ
	** \param szTagName 
	** \param lastTagName 
	** \return bool 
	** \author gw
	** \date 2017��5��27�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyDOAssociatedDIArr(const std::string &szTagName, const std::string &szLastTagName)
	{
		auto it = m_arrDOToAssociatedDIMap.find(szLastTagName);
		if (it != m_arrDOToAssociatedDIMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				memset(var->m_szAssociatedDI, 0, sizeof(var->m_szAssociatedDI));
				int nSize = szTagName.size();
				strncpy(var->m_szAssociatedDI, szTagName.c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrDOToAssociatedDIMap[szTagName], it->second);
			// Erase old key-value from map
			m_arrDOToAssociatedDIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteAOsAssociatedDIArr(const std::string &szTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteAOsAssociatedDIArr 
	** \details ɾ��AO�����AO��AssociatedDI�й�������ô������ϵҲɾ��
	** \param szTagName 
	** \return bool 
	** \author gw
	** \date 2017��5��23�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteDOsRelationAssociatedDIArr(const std::string &szTagName, CDigitalOutput *pDigitalOutput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pDigitalOutput);
		if (!pDigitalOutput)
		{
			return false;
		}

		//����ת��key�Ƿ����
		auto it = m_arrDOToAssociatedDIMap.find(szTagName);
		//�а󶨹�ϵ
		if (it != m_arrDOToAssociatedDIMap.end())
		{
			auto &arr = it->second;
			//�û�����vec
			auto ite = std::find(arr.begin(), arr.end(), pDigitalOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrDOToAssociatedDIMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteAOsAssociatedDIArr(const std::string &szTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteAOsAssociatedDIArr 
	** \details ǰ��DIɾ�������ǰ��AO��AssociatedDI������ң��DI����ô������ϵɾ��
	** \param szTagName 
	** \return bool 
	** \author gw
	** \date 2017��5��23�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteDOsAssociatedDIArr(const std::string &szTagName)
	{
		auto it = m_arrDOToAssociatedDIMap.find(szTagName);
		if (it != m_arrDOToAssociatedDIMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				//�������
				memset(var->m_szAssociatedDI, 0, sizeof(var->m_szAssociatedDI));
			}

			m_arrDOToAssociatedDIMap.erase(it);
		}

		return true;
	}

	/*! \fn 	bool CFesData::AddDORelationBlockingSignalTagArr(const std::string &szTagName, CDigitalOutput *pDigitalOutput)
	********************************************************************************************************* 
	** \brief Config::CFesData::AddDORelationBlockingSignalTagArr 
	** \details ���fes do BlockingSignalTag����(di tagname���û�����bool���ͱ���)
	** \param szTagName 
	** \param pDigitalOutput 
	** \return bool 
	** \author gw
	** \date 2017��5��26�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::AddDORelationBlockingSignalTagArr(const std::string &szTagName, CDigitalOutput *pDigitalOutput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}


		auto it = m_arrDOToBlockingSignalTagMap.find(szTagName);
		if (it != m_arrDOToBlockingSignalTagMap.end())
		{
			it->second.push_back(pDigitalOutput);

			return true;
		}

		std::vector<CDigitalOutput *> vec;
		vec.clear();
		vec.push_back(pDigitalOutput);
		m_arrDOToBlockingSignalTagMap.insert(make_pair(szTagName, vec));

		return true;
	}

	/*! \fn bool CFesData::ModifyDORelationBlockingSignalTagArr(const std::string &szTagName, const std::string &lastTagName, CDigitalOutput *pDigitalOutput)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyDORelationBlockingSignalTagArr 
	** \details �޸�fes do BlockingSignalTag����(di tagname���û�����bool���ͱ���)
	** \param szTagName 
	** \param lastTagName 
	** \param pDigitalOutput 
	** \return bool 
	** \author gw
	** \date 2017��5��26�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyDORelationBlockingSignalTagArr(const std::string &szTagName, const std::string &szLastTagName, CDigitalOutput *pDigitalOutput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC) || !CheckTagNameIsValid(szLastTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pDigitalOutput);
		if (!pDigitalOutput)
		{
			return false;
		}

		auto it = m_arrDOToBlockingSignalTagMap.find(szLastTagName);
		if (it != m_arrDOToBlockingSignalTagMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pDigitalOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrDOToBlockingSignalTagMap.find(szTagName);

				memset(pDigitalOutput->m_szBlockingSignalTag, 0, sizeof(pDigitalOutput->m_szBlockingSignalTag));
				int nSize = szTagName.size();
				strncpy(pDigitalOutput->m_szBlockingSignalTag, szTagName.c_str(), qMin(nSize, MAX_TAGNAME_LEN_SCADASTUDIO));

				if (itee != m_arrDOToBlockingSignalTagMap.end())
				{
					itee->second.push_back(pDigitalOutput);
				}
				else
				{
					std::vector<CDigitalOutput *> vec;
					vec.clear();
					vec.push_back(pDigitalOutput);
					m_arrDOToBlockingSignalTagMap.insert(make_pair(szTagName, vec));
				}
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrDOToBlockingSignalTagMap.erase(it);
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyDOBlockingSignalTagArr(const std::string &szTagName, const std::string &szLastTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyDOBlockingSignalTagArr 
	** \details ǰ��DI�޸ģ����ǰ��DO BlockingSignalTag������ң�ţ���ô������ϵ�޸� һ�Զ��ϵ
	** \param szTagName 
	** \param szLastTagName 
	** \return bool 
	** \author gw
	** \date 2017��5��27�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyDOBlockingSignalTagArr(const std::string &szTagName, const std::string &szLastTagName)
	{
		auto it = m_arrDOToBlockingSignalTagMap.find(szLastTagName);
		if (it != m_arrDOToBlockingSignalTagMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				memset(var->m_szBlockingSignalTag, 0, sizeof(var->m_szBlockingSignalTag));
				int nSize = szTagName.size();
				strncpy(var->m_szBlockingSignalTag, szTagName.c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrDOToBlockingSignalTagMap[szTagName], it->second);
			// Erase old key-value from map
			m_arrDOToBlockingSignalTagMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteDOsRelationBlockingSignalTagArr(const std::string &szTagName, CDigitalOutput *pDigitalOutput)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteDOsRelationBlockingSignalTagArr 
	** \details ɾ��fes do BlockingSignalTag����(di tagname���û�����bool���ͱ���)
	** \param szTagName 
	** \param pDigitalOutput 
	** \return bool 
	** \author gw
	** \date 2017��5��26�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteDOsRelationBlockingSignalTagArr(const std::string &szTagName, CDigitalOutput *pDigitalOutput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pDigitalOutput);
		if (!pDigitalOutput)
		{
			return false;
		}

		//����ת��key�Ƿ����
		auto it = m_arrDOToBlockingSignalTagMap.find(szTagName);
		//�а󶨹�ϵ
		if (it != m_arrDOToBlockingSignalTagMap.end())
		{
			auto &arr = it->second;
			//�û�����vec
			auto ite = std::find(arr.begin(), arr.end(), pDigitalOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrDOToBlockingSignalTagMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteDOsBlockingSignalTagArr(const std::string &szTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteDOsBlockingSignalTagArr 
	** \details ǰ��di tagname��ǰ���û�����bool���ͱ���ɾ����fes do BlockingSignalTag����(di tagname���û�����bool���ͱ���)Ҳɾ�� һ�Զ��ϵ
	** \param szTagName 
	** \return bool 
	** \author gw
	** \date 2017��5��27�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteDOsBlockingSignalTagArr(const std::string &szTagName)
	{ 
		auto it = m_arrDOToBlockingSignalTagMap.find(szTagName);
		if (it != m_arrDOToBlockingSignalTagMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				//�������
				memset(var->m_szBlockingSignalTag, 0, sizeof(var->m_szBlockingSignalTag));
			}

			m_arrDOToBlockingSignalTagMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::AddUserVariableRelationScaleArr(std::string &tagName, CUserVariable *pVairalbe)
	********************************************************************************************************* 
	** \brief Config::CFesData::AddUserVariableRelationScaleArr 
	** \details �û����������ת��
	** \param tagName 
	** \param pVairalbe 
	** \return bool 
	** \author gw
	** \date 2017��2��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::AddUserVariableRelationScaleArr(std::string &tagName, CUserVariable *pVairalbe)
	{
		Q_ASSERT(!tagName.empty());
		if (tagName.empty())
		{
			return false;
		}

		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto it = m_arrScaleToUserVariableMap.find(tagName);
		if (it != m_arrScaleToUserVariableMap.end())
		{
			it->second.push_back(pVairalbe);

			return true;
		}

		std::vector<CUserVariable *> vec;
		vec.clear();
		vec.push_back(pVairalbe);
		m_arrScaleToUserVariableMap.insert(make_pair(tagName, vec));

		return true;
	}

	/*! \fn bool CFesData::ModifyUserVariableRelationScaleArr(CUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName, int type)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyUserVariableRelationScaleArr 
	** \details �û����������ת���޸�
	** \param pVairalbe 
	** \param tagName 
	** \param lastTagName 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2017��2��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyUserVariableRelationScaleArr(CUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName, int type)
	{
		Q_UNUSED(type);

		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto bFlag = tagName.empty() || lastTagName.empty();
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return false;
		}

		auto it = m_arrScaleToUserVariableMap.find(lastTagName);
		if (it != m_arrScaleToUserVariableMap.end())
		{
			//hash������һ��tagname�Ƿ����
			auto &arr = it->second;

			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				//hash remove
				arr.erase(ite);

				auto itee = m_arrScaleToUserVariableMap.find(tagName);
				//hash����tagname�Ƿ����
				if (itee != m_arrScaleToUserVariableMap.end())
				{
					itee->second.push_back(pVairalbe);
				}
				else
				{
					std::vector<CUserVariable *> vec;
					vec.clear();
					vec.push_back(pVairalbe);
					m_arrScaleToUserVariableMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyUserVariableScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyUserVariableScaleArr 
	** \details 
	** \param tagName 
	** \param lastTagName 
	** \param desc 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2017��2��17�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyUserVariableScaleArr(const std::string &szTagName,const  std::string &szLastTagName,const  std::string &szDesc, int type)
	{
		Q_UNUSED(szDesc);

		auto it = m_arrScaleToUserVariableMap.find(szLastTagName);
		if (it != m_arrScaleToUserVariableMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strScaleTagName = szTagName.c_str();
				//var->m_strScaleDesc = desc.c_str();
				var->m_nScaleType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrScaleToUserVariableMap[szTagName], it->second);
			// Erase old key-value from map
			m_arrScaleToUserVariableMap.erase(it);
		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteUserVariableScaleArr(std::string &tagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteUserVariableScaleArr 
	** \details 
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2017��2��18�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteUserVariableScaleArr(const std::string &szTagName)
	{
		auto it = m_arrScaleToUserVariableMap.find(szTagName);
		if (it != m_arrScaleToUserVariableMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strScaleTagName = "";
				//var->m_strScaleDesc = "";
			}

			m_arrScaleToUserVariableMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteUserVariableRelationScaleArr(const std::string &szTagName, CUserVariable *pVairalbe)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteUserVariableRelationScaleArr 
	** \details �û�����ɾ�������Scale�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param szTagName 
	** \return bool 
	** \author gw
	** \date 2017��5��20�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteUserVariableRelationScaleArr(const std::string &szTagName, CUserVariable *pVairalbe)
	{
		//û�й���ת��
		if (szTagName.empty())
		{
			return true;
		}
		
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		//����ת��key�Ƿ����
		auto it = m_arrScaleToUserVariableMap.find(szTagName);
		//�а󶨹�ϵ
		if (it != m_arrScaleToUserVariableMap.end())
		{
			auto &arr = it->second;
			//�û�����vec
			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrScaleToUserVariableMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::AddUserVariableRelationAlarmArr(std::string &tagName, CUserVariable *pVairalbe)
	********************************************************************************************************* 
	** \brief Config::CFesData::AddUserVariableRelationAlarmArr 
	** \details �û����������澯hash
	** \param tagName 
	** \param pVairalbe 
	** \return bool 
	** \author gw
	** \date 2017��3��1�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::AddUserVariableRelationAlarmArr(std::string &tagName, CUserVariable *pVairalbe)
	{
		Q_ASSERT(!tagName.empty());
		if (tagName.empty())
		{
			return false;
		}

		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto it = m_arrAlarmToUserVariableMap.find(tagName);
		if (it != m_arrAlarmToUserVariableMap.end())
		{
			it->second.push_back(pVairalbe);

			return true;
		}

		std::vector<CUserVariable *> vec;
		vec.clear();
		vec.push_back(pVairalbe);
		m_arrAlarmToUserVariableMap.insert(make_pair(tagName, vec));

		return true;
	}

	/*! \fn bool ModifyUserVariableRelationAlarmArr(CUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName, int type)
	********************************************************************************************************* 
	** \brief Config::ModifyUserVariableRelationAlarmArr 
	** \details �û����������ĸ澯�޸�hash
	** \param pVairalbe 
	** \param tagName 
	** \param lastTagName 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2017��3��1�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyUserVariableRelationAlarmArr(CUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName)
	{
		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto bFlag = tagName.empty() || lastTagName.empty();
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return false;
		}

		auto it = m_arrAlarmToUserVariableMap.find(lastTagName);
		if (it != m_arrAlarmToUserVariableMap.end())
		{
			//hash������һ��tagname�Ƿ����
			auto &arr = it->second;

			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				//hash remove
				arr.erase(ite);

				auto itee = m_arrAlarmToUserVariableMap.find(tagName);
				//hash����tagname�Ƿ����
				if (itee != m_arrAlarmToUserVariableMap.end())
				{
					itee->second.push_back(pVairalbe);
				}
				else
				{
					std::vector<CUserVariable *> vec;
					vec.clear();
					vec.push_back(pVairalbe);
					m_arrAlarmToUserVariableMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyUserVariableAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	********************************************************************************************************* 
	** \brief Config::CFesData::ModifyUserVariableAlarmArr 
	** \details �澯�޸�,���������û�����hash�޸�
	** \param tagName 
	** \param lastTagName 
	** \param desc 
	** \param type 
	** \return bool 
	** \author gw
	** \date 2017��3��1�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::ModifyUserVariableAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	{
		Q_UNUSED(desc);

		auto it = m_arrAlarmToUserVariableMap.find(lastTagName);
		if (it != m_arrAlarmToUserVariableMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strAlarmTagName = tagName.c_str();
				//var->m_strScaleDesc = desc.c_str();
				var->m_nAlarmType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrAlarmToUserVariableMap[tagName], it->second);
			// Erase old key-value from map
			m_arrAlarmToUserVariableMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteUserVariableAlarmArr(std::string &tagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteUserVariableAlarmArr 
	** \details 
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2017��3��1�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteUserVariableAlarmArr(const std::string &szTagName)
	{
		auto it = m_arrAlarmToUserVariableMap.find(szTagName);
		if (it != m_arrAlarmToUserVariableMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strAlarmTagName = "";
				var->m_nAlarmType = AlarmType::ALARMTYPENULL;
			}

			m_arrAlarmToUserVariableMap.erase(it);
		}

		return true;
	}
	/*! \fn bool CFesData::DeleteUserVariableRelationAlarmArr(std::string &tagName, CUserVariable *pVairalbe)
	*********************************************************************************************************
	** \brief Config::CFesData::DeleteUserVariableRelationAlarmArr
	** \details �û�����ɾ�������Alarm�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param tagName
	** \param pVairalbe
	** \return bool
	** \author gw
	** \date 2017��5��19��
	** \note
	********************************************************************************************************/
	bool CFesData::DeleteUserVariableRelationAlarmArr(std::string &tagName, CUserVariable *pVairalbe)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto it = m_arrAlarmToUserVariableMap.find(tagName);
		//�а󶨹�ϵ
		if (it != m_arrAlarmToUserVariableMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrAlarmToUserVariableMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	bool CFesData::AddUserVariableRelationSourceTagNameArr(std::string &tagName, CUserVariable *pVairalbe)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto it = m_arrSourceTagNameToUserVariableMap.find(tagName);
		if (it != m_arrSourceTagNameToUserVariableMap.end())
		{
			it->second.push_back(pVairalbe);

			return true;
		}

		std::vector<CUserVariable *> vec;
		vec.clear();
		vec.push_back(pVairalbe);
		m_arrSourceTagNameToUserVariableMap.insert(make_pair(tagName, vec));

		return true;
	}

	bool CFesData::ModifyUserVariableRelationSourceTagNameArr(CUserVariable * pVairalbe, std::string & tagName, std::string & lastTagName)
	{
		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto bFlag = tagName.empty() || lastTagName.empty();
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return false;
		}

		auto it = m_arrSourceTagNameToUserVariableMap.find(lastTagName);
		if (it != m_arrSourceTagNameToUserVariableMap.end())
		{
			//hash������һ��tagname�Ƿ����
			auto &arr = it->second;

			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				//hash remove
				arr.erase(ite);

				auto itee = m_arrSourceTagNameToUserVariableMap.find(tagName);
				//hash����tagname�Ƿ����
				if (itee != m_arrSourceTagNameToUserVariableMap.end())
				{
					itee->second.push_back(pVairalbe);
				}
				else
				{
					std::vector<CUserVariable *> vec;
					vec.clear();
					vec.push_back(pVairalbe);
					m_arrSourceTagNameToUserVariableMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyUserVariableSourceTagNameArr(std::string &tagName, std::string &lastTagName)
	*********************************************************************************************************
	** \brief Config::CFesData::ModifyUserVariableSourceTagNameArr
	** \details
	** \param tagName
	** \param lastTagName
	** \return bool
	** \author gw
	** \date 2017��5��16��
	** \note
	********************************************************************************************************/
	bool CFesData::ModifyUserVariableSourceTagNameArr(std::string &tagName, std::string &lastTagName)
	{
		auto it = m_arrSourceTagNameToUserVariableMap.find(lastTagName);
		if (it != m_arrSourceTagNameToUserVariableMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				auto list = var->m_strSourceTagName.split(".");
				//��ʱ�ж�����
				if (list.count() == 3)
				{
					//��tagname
					if (list[0] + "." + list[1] == lastTagName.c_str())
					{
						var->m_strSourceTagName = QString("%1.%2").arg(tagName.c_str()).arg(list[2]);
					}
					else
					{
						return false;
					}
				}

				//var->m_strScaleDesc = desc.c_str();
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrSourceTagNameToUserVariableMap[tagName], it->second);
			// Erase old key-value from map
			m_arrSourceTagNameToUserVariableMap.erase(it);
		}

		return true;
	}


	bool CFesData::DeleteUserVariableSourceTagNameArr(std::string &tagName)
	{
		auto it = m_arrSourceTagNameToUserVariableMap.find(tagName);

		if (it != m_arrSourceTagNameToUserVariableMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strSourceTagName = "";
			}

			m_arrSourceTagNameToUserVariableMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteUserVariableRelationSourceTagNameArr(std::string &tagName)
	*********************************************************************************************************
	** \brief Config::CFesData::DeleteUserVariableRelationSourceTagNameArr
	** \details �û�����ɾ�������SourceTagName�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param tagName
	** \return bool
	** \author gw
	** \date 2017��5��19��
	** \note
	********************************************************************************************************/
	bool CFesData::DeleteUserVariableRelationSourceTagNameArr(std::string &tagName, CUserVariable *pVairalbe)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pVairalbe);
		if (!pVairalbe)
		{
			return false;
		}

		auto it = m_arrSourceTagNameToUserVariableMap.find(tagName);
		//�а󶨹�ϵ
		if (it != m_arrSourceTagNameToUserVariableMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pVairalbe);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrSourceTagNameToUserVariableMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::AddForwardingAIRelationScaleArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput)
	*********************************************************************************************************
	** \brief Config::CFesData::AddForwardingAIRelationScaleArr
	** \details ת��AI����ǰ��ת��
	** \param tagName
	** \param pAnalogInput
	** \return bool
	** \author gw
	** \date 2017��5��5��
	** \note
	********************************************************************************************************/
	bool CFesData::AddForwardingAIRelationScaleArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput)
	{
		if (tagName.empty())
		{
			return true;
		}

		auto it = m_arrScaleToForwardingAIMap.find(tagName);
		if (it != m_arrScaleToForwardingAIMap.end())
		{
			it->second.push_back(pAnalogInput);

			return true;
		}

		std::vector<CForwardingAnalogInputData *> vec;
		vec.clear();
		vec.push_back(pAnalogInput);
		m_arrScaleToForwardingAIMap.insert(make_pair(tagName, vec));

		return true;
	}

	/*! \fn bool CFesData::ModifyForwardingAIsRelationScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	*********************************************************************************************************
	** \brief Config::CFesData::ModifyForwardingAIsRelationScaleArr
	** \details �޸�ת��AI����ǰ��ת��
	** \param tagName
	** \param lastTagName
	** \param desc
	** \param type
	** \return bool
	** \author gw
	** \date 2017��5��5��
	** \note
	********************************************************************************************************/
	bool CFesData::ModifyForwardingAIsRelationScaleArr(CForwardingAnalogInputData *pData, std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	{
		//�޸ı����ܲ鵽
		auto it = m_arrScaleToForwardingAIMap.find(lastTagName);
		if (it != m_arrScaleToForwardingAIMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pData);
			if (ite != arr.end())
			{
				//hash remove
				arr.erase(ite);

				auto itee = m_arrScaleToForwardingAIMap.find(tagName);
				//hash����tagname�Ƿ����
				if (itee != m_arrScaleToForwardingAIMap.end())
				{
					itee->second.push_back(pData);
				}
				else
				{
					std::vector<CForwardingAnalogInputData *> vec;
					vec.clear();
					vec.push_back(pData);
					m_arrScaleToForwardingAIMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	bool CFesData::ModifyForwardingAIsScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	{
		Q_UNUSED(desc);

		auto it = m_arrScaleToForwardingAIMap.find(lastTagName);
		if (it != m_arrScaleToForwardingAIMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strScaleTagName = tagName.c_str();
				//var->m_strScaleDesc = desc.c_str();
				var->m_nScaleType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrScaleToForwardingAIMap[tagName], it->second);
			// Erase old key-value from map
			m_arrScaleToForwardingAIMap.erase(it);
		}

		return true;
	}

	bool CFesData::DeleteForwardingAIsScaleArr(const std::string &szTagName)
	{
		auto it = m_arrScaleToForwardingAIMap.find(szTagName);
		if (it != m_arrScaleToForwardingAIMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strScaleTagName = "";
				var->m_nScaleType = ScaleType::LINEAR;
			}

			m_arrScaleToForwardingAIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteForwardingAIRelationScaleArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput)
	*********************************************************************************************************
	** \brief Config::CFesData::DeleteForwardingAIRelationScaleArr
	** \details ת��ͨ��AIɾ�������Scale�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param tagName
	** \param pAnalogInput
	** \return bool
	** \author gw
	** \date 2017��5��19��
	** \note
	********************************************************************************************************/
	bool CFesData::DeleteForwardingAIRelationScaleArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pAnalogInput);
		if (!pAnalogInput)
		{
			return false;
		}

		auto it = m_arrScaleToForwardingAIMap.find(tagName);
		//�а󶨹�ϵ
		if (it != m_arrScaleToForwardingAIMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pAnalogInput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrScaleToForwardingAIMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	bool CFesData::AddForwardingAORelationScaleArr(std::string &tagName, CForwardingAnalogOutputData *pAnalogOutput)
	{
		if (tagName.empty())
		{
			return true;
		}

		auto it = m_arrScaleToForwardingAOMap.find(tagName);
		if (it != m_arrScaleToForwardingAOMap.end())
		{
			it->second.push_back(pAnalogOutput);

			return true;
		}

		std::vector<CForwardingAnalogOutputData *> vec;
		vec.clear();
		vec.push_back(pAnalogOutput);
		m_arrScaleToForwardingAOMap.insert(make_pair(tagName, vec));

		return true;
	}

	bool CFesData::ModifyForwardingAOsRelationScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, Config::CForwardingAnalogOutputData *pAnalogOutput)
	{
		Q_UNUSED(desc);

		Q_ASSERT(pAnalogOutput);
		if (!pAnalogOutput)
		{
			return false;
		}

		auto it = m_arrScaleToForwardingAOMap.find(lastTagName);
		if (it != m_arrScaleToForwardingAOMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pAnalogOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrScaleToForwardingAOMap.find(tagName);

				pAnalogOutput->m_strScaleTagName = tagName.c_str();
				if (itee != m_arrScaleToForwardingAOMap.end())
				{
					itee->second.push_back(pAnalogOutput);
				}
				else
				{
					std::vector<CForwardingAnalogOutputData *> vec;
					vec.clear();
					vec.push_back(pAnalogOutput);
					m_arrScaleToForwardingAOMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	bool CFesData::ModifyForwardingAOsScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type)
	{
		Q_UNUSED(desc);

		auto it = m_arrScaleToForwardingAOMap.find(lastTagName);
		if (it != m_arrScaleToForwardingAOMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strScaleTagName = tagName.c_str();
				//var->m_strScaleDesc = desc.c_str();
				var->m_nScaleType = type;
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrScaleToForwardingAOMap[tagName], it->second);
			// Erase old key-value from map
			m_arrScaleToForwardingAOMap.erase(it);
		}

		return true;
	}

	bool CFesData::DeleteForwardingAOsScaleArr(const std::string &szTagName)
	{
		auto it = m_arrScaleToForwardingAOMap.find(szTagName);
		if (it != m_arrScaleToForwardingAOMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strScaleTagName = "";
				var->m_nScaleType = ScaleType::LINEAR;
			}

			m_arrScaleToForwardingAOMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteForwardingAORelationScaleArr(std::string &tagName, CForwardingAnalogOutputData *pAnalogOutput)
	*********************************************************************************************************
	** \brief Config::CFesData::DeleteForwardingAORelationScaleArr
	** \details ת��ͨ��AOɾ�������Scale�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param tagName
	** \param pAnalogOutput
	** \return bool
	** \author gw
	** \date 2017��5��19��
	** \note
	********************************************************************************************************/
	bool CFesData::DeleteForwardingAORelationScaleArr(std::string &tagName, CForwardingAnalogOutputData *pAnalogOutput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pAnalogOutput);
		if (!pAnalogOutput)
		{
			return false;
		}

		auto it = m_arrScaleToForwardingAOMap.find(tagName);
		//�а󶨹�ϵ
		if (it != m_arrScaleToForwardingAOMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pAnalogOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrScaleToForwardingAOMap.erase(it);
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::AddForwardingAIRelationAISourceTagNameArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput)
	*********************************************************************************************************
	** \brief Config::CFesData::AddForwardingAIRelationAISourceTagNameArr
	** \details ǰ��AI tagname��key��ת��AI vec��value
	** \param tagName
	** \param pAnalogInput
	** \return bool
	** \author gw
	** \date 2017��5��17��
	** \note
	********************************************************************************************************/
	bool CFesData::AddForwardingAIRelationSourceTagNameArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		auto it = m_arrAIToForwardingAIMap.find(tagName);
		if (it != m_arrAIToForwardingAIMap.end())
		{
			it->second.push_back(pAnalogInput);

			return true;
		}

		std::vector<CForwardingAnalogInputData *> vec;
		vec.clear();
		vec.push_back(pAnalogInput);
		m_arrAIToForwardingAIMap.insert(make_pair(tagName, vec));

		return true;
	}

	bool CFesData::ModifyForwardingAIRelationSourceTagNameArr(std::string &tagName, std::string &lastTagName, CForwardingAnalogInputData *pAnalogInput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC) || !CheckTagNameIsValid(lastTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pAnalogInput);
		if (!pAnalogInput)
		{
			return false;
		}

		auto it = m_arrAIToForwardingAIMap.find(lastTagName);
		if (it != m_arrAIToForwardingAIMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pAnalogInput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrAIToForwardingAIMap.find(tagName);

				pAnalogInput->m_strScaleTagName = tagName.c_str();
				if (itee != m_arrAIToForwardingAIMap.end())
				{
					itee->second.push_back(pAnalogInput);
				}
				else
				{
					std::vector<CForwardingAnalogInputData *> vec;
					vec.clear();
					vec.push_back(pAnalogInput);
					m_arrAIToForwardingAIMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::ModifyForwardingAISourceTagNameArr(std::string &tagName, std::string &lastTagName)
	*********************************************************************************************************
	** \brief Config::CFesData::ModifyForwardingAISourceTagNameArr
	** \details ǰ��AI tagname�޸ģ�ǰ��ת��AI�󶨹�����ǰ��AI tagnameҲ�����޸�
	** \param tagName
	** \param lastTagName
	** \return bool
	** \author gw
	** \date 2017��5��17��
	** \note
	********************************************************************************************************/
	bool CFesData::ModifyForwardingAISourceTagNameArr(std::string &tagName, std::string &lastTagName)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC) || !CheckTagNameIsValid(lastTagName.c_str(), FES_DESC))
		{
			return false;
		}

		auto it = m_arrAIToForwardingAIMap.find(lastTagName);
		if (it != m_arrAIToForwardingAIMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strSourceTagName = tagName.c_str();
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrAIToForwardingAIMap[tagName], it->second);
			// Erase old key-value from map
			m_arrAIToForwardingAIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteForwardingAISourceTagNameArr(std::string &tagName)
	*********************************************************************************************************
	** \brief Config::CFesData::DeleteForwardingAISourceTagNameArr
	** \details ǰ��AI tagnameɾ����ת��AI�󶨹���source tagname�����
	** \param tagName
	** \return bool
	** \author gw
	** \date 2017��5��17��
	** \note
	********************************************************************************************************/
	bool CFesData::DeleteForwardingAISourceTagNameArr(std::string &tagName)
	{
		auto it = m_arrAIToForwardingAIMap.find(tagName);
		if (it != m_arrAIToForwardingAIMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strSourceTagName = "";
			}

			m_arrAIToForwardingAIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteForwardingAIRelationSourceTagNameArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput)
	*********************************************************************************************************
	** \brief Config::CFesData::DeleteForwardingAIRelationSourceTagNameArr
	** \details ת��ͨ��AIɾ�������sourcetagname�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param tagName
	** \param pAnalogInput
	** \return bool
	** \author gw
	** \date 2017��5��19��
	** \note
	********************************************************************************************************/
	bool CFesData::DeleteForwardingAIRelationSourceTagNameArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pAnalogInput);
		if (!pAnalogInput)
		{
			return false;
		}

		auto it = m_arrAIToForwardingAIMap.find(tagName);
		//�а󶨹�ϵ
		if (it != m_arrAIToForwardingAIMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pAnalogInput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrAIToForwardingAIMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	bool CFesData::AddForwardingDIRelationSourceTagNameArr(std::string & tagName, CForwardingDigitalInputData * pDigitalInput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		auto it = m_arrDIToForwardingDIMap.find(tagName);
		if (it != m_arrDIToForwardingDIMap.end())
		{
			it->second.push_back(pDigitalInput);

			return true;
		}

		std::vector<CForwardingDigitalInputData *> vec;
		vec.clear();
		vec.push_back(pDigitalInput);
		m_arrDIToForwardingDIMap.insert(make_pair(tagName, vec));

		return true;
	}

	bool CFesData::ModifyForwardingDIRelationSourceTagNameArr(std::string &tagName, std::string &lastTagName, CForwardingDigitalInputData *pDigitalInput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC) || !CheckTagNameIsValid(lastTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pDigitalInput);
		if (!pDigitalInput)
		{
			return false;
		}

		auto it = m_arrDIToForwardingDIMap.find(lastTagName);
		if (it != m_arrDIToForwardingDIMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pDigitalInput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrDIToForwardingDIMap.find(tagName);

				pDigitalInput->m_strSourceTagName = tagName.c_str();
				if (itee != m_arrDIToForwardingDIMap.end())
				{
					itee->second.push_back(pDigitalInput);
				}
				else
				{
					std::vector<CForwardingDigitalInputData *> vec;
					vec.clear();
					vec.push_back(pDigitalInput);
					m_arrDIToForwardingDIMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	bool CFesData::ModifyForwardingDISourceTagNameArr(std::string &tagName, std::string &lastTagName)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC) || !CheckTagNameIsValid(lastTagName.c_str(), FES_DESC))
		{
			return false;
		}

		auto it = m_arrDIToForwardingDIMap.find(lastTagName);
		if (it != m_arrDIToForwardingDIMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strSourceTagName = tagName.c_str();
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrDIToForwardingDIMap[tagName], it->second);
			// Erase old key-value from map
			m_arrDIToForwardingDIMap.erase(it);
		}

		return true;
	}

	bool CFesData::DeleteForwardingDISourceTagNameArr(std::string &tagName)
	{
		auto it = m_arrDIToForwardingDIMap.find(tagName);
		if (it != m_arrDIToForwardingDIMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strSourceTagName = "";
			}

			m_arrDIToForwardingDIMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteForwardingDIRelationSourceTagNameArr(std::string &tagName, CForwardingDigitalInputData *pDigitalInput)
	*********************************************************************************************************
	** \brief Config::CFesData::DeleteForwardingDIRelationSourceTagNameArr
	** \details ת��ͨ��DIɾ�������sourcetagname�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param tagName
	** \param pDigitalInput
	** \return bool
	** \author gw
	** \date 2017��5��19��
	** \note
	********************************************************************************************************/
	bool CFesData::DeleteForwardingDIRelationSourceTagNameArr(std::string &tagName, CForwardingDigitalInputData *pDigitalInput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pDigitalInput);
		if (!pDigitalInput)
		{
			return false;
		}

		auto it = m_arrDIToForwardingDIMap.find(tagName);
		//�а󶨹�ϵ
		if (it != m_arrDIToForwardingDIMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pDigitalInput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrDIToForwardingDIMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	bool CFesData::AddForwardingAORelationSourceTagNameArr(std::string &tagName, CForwardingAnalogOutputData *pAnalogOutput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		auto it = m_arrAOToForwardingAOMap.find(tagName);
		if (it != m_arrAOToForwardingAOMap.end())
		{
			it->second.push_back(pAnalogOutput);

			return true;
		}

		std::vector<CForwardingAnalogOutputData *> vec;
		vec.clear();
		vec.push_back(pAnalogOutput);
		m_arrAOToForwardingAOMap.insert(make_pair(tagName, vec));

		return true;
	}

	bool CFesData::ModifyForwardingAORelationSourceTagNameArr(std::string &tagName, std::string &lastTagName, CForwardingAnalogOutputData *pAnalogOutput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC) || !CheckTagNameIsValid(lastTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pAnalogOutput);
		if (!pAnalogOutput)
		{
			return false;
		}

		auto it = m_arrAOToForwardingAOMap.find(lastTagName);
		if (it != m_arrAOToForwardingAOMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pAnalogOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrAOToForwardingAOMap.find(tagName);

				pAnalogOutput->m_strSourceTagName = tagName.c_str();
				if (itee != m_arrAOToForwardingAOMap.end())
				{
					itee->second.push_back(pAnalogOutput);
				}
				else
				{
					std::vector<CForwardingAnalogOutputData *> vec;
					vec.clear();
					vec.push_back(pAnalogOutput);
					m_arrAOToForwardingAOMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	bool CFesData::ModifyForwardingAOSourceTagNameArr(std::string &tagName, std::string &lastTagName)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC) || !CheckTagNameIsValid(lastTagName.c_str(), FES_DESC))
		{
			return false;
		}

		auto it = m_arrAOToForwardingAOMap.find(lastTagName);
		if (it != m_arrAOToForwardingAOMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strSourceTagName = tagName.c_str();
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrAOToForwardingAOMap[tagName], it->second);
			// Erase old key-value from map
			m_arrAOToForwardingAOMap.erase(it);
		}

		return true;
	}

	bool CFesData::DeleteForwardingAOSourceTagNameArr(const std::string &szTagName)
	{
		auto it = m_arrAOToForwardingAOMap.find(szTagName);
		if (it != m_arrAOToForwardingAOMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strSourceTagName = "";
			}

			m_arrAOToForwardingAOMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteForwardingAORelationSourceTagNameArr(std::string &tagName, CForwardingAnalogOutputData *pAnalogOutput)
	*********************************************************************************************************
	** \brief Config::CFesData::DeleteForwardingAORelationSourceTagNameArr
	** \details ת��ͨ��AOɾ�������sourcetagname�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param tagName
	** \param pAnalogOutput
	** \return bool
	** \author gw
	** \date 2017��5��19��
	** \note
	********************************************************************************************************/
	bool CFesData::DeleteForwardingAORelationSourceTagNameArr(std::string &tagName, CForwardingAnalogOutputData *pAnalogOutput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pAnalogOutput);
		if (!pAnalogOutput)
		{
			return false;
		}

		auto it = m_arrAOToForwardingAOMap.find(tagName);
		//�а󶨹�ϵ
		if (it != m_arrAOToForwardingAOMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pAnalogOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrAOToForwardingAOMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	bool CFesData::AddForwardingDORelationSourceTagNameArr(std::string &tagName, CForwardingDigitalOutputData *pDigitalOutput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		auto it = m_arrDOToForwardingDOMap.find(tagName);
		if (it != m_arrDOToForwardingDOMap.end())
		{
			it->second.push_back(pDigitalOutput);

			return true;
		}

		std::vector<CForwardingDigitalOutputData *> vec;
		vec.clear();
		vec.push_back(pDigitalOutput);
		m_arrDOToForwardingDOMap.insert(make_pair(tagName, vec));

		return true;
	}

	bool CFesData::ModifyForwardingDORelationSourceTagNameArr(std::string &tagName, std::string &lastTagName, CForwardingDigitalOutputData *pDigitalOutput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC) || !CheckTagNameIsValid(lastTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pDigitalOutput);
		if (!pDigitalOutput)
		{
			return false;
		}

		auto it = m_arrDOToForwardingDOMap.find(lastTagName);
		if (it != m_arrDOToForwardingDOMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pDigitalOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);

				//map�����µ�tagname�Ƿ����
				auto itee = m_arrDOToForwardingDOMap.find(tagName);

				pDigitalOutput->m_strSourceTagName = tagName.c_str();
				if (itee != m_arrDOToForwardingDOMap.end())
				{
					itee->second.push_back(pDigitalOutput);
				}
				else
				{
					std::vector<CForwardingDigitalOutputData *> vec;
					vec.clear();
					vec.push_back(pDigitalOutput);
					m_arrDOToForwardingDOMap.insert(make_pair(tagName, vec));
				}
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	bool CFesData::ModifyForwardingDOSourceTagNameArr(std::string &tagName, std::string &lastTagName)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC) || !CheckTagNameIsValid(lastTagName.c_str(), FES_DESC))
		{
			return false;
		}

		auto it = m_arrDOToForwardingDOMap.find(lastTagName);
		if (it != m_arrDOToForwardingDOMap.end())
		{
			auto &arr = it->second;
			for each (auto var in arr)
			{
				var->m_strSourceTagName = tagName.c_str();
			}

			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(m_arrDOToForwardingDOMap[tagName], it->second);
			// Erase old key-value from map
			m_arrDOToForwardingDOMap.erase(it);
		}

		return true;
	}

	/*! \fn 
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteForwardingDOSourceTagNameArr 
	** \details 
	** \param tagName 
	** \return bool 
	** \author gw
	** \date 2017��5��27�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteForwardingDOSourceTagNameArr(const std::string &szTagName)
	{
		auto it = m_arrDOToForwardingDOMap.find(szTagName);
		if (it != m_arrDOToForwardingDOMap.end())
		{
			auto &arr = it->second;

			for each (auto var in arr)
			{
				var->m_strSourceTagName = "";
			}

			m_arrDOToForwardingDOMap.erase(it);
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteForwardingDORelationSourceTagNameArr(std::string &tagName, CForwardingDigitalOutputData *pDigitalOutput)
	*********************************************************************************************************
	** \brief Config::CFesData::DeleteForwardingDORelationSourceTagNameArr
	** \details ת��ͨ��DOɾ�������sourcetagname�а󶨹�������ô�󶨹�ϵҪɾ��
	** \param tagName
	** \param pDigitalOutput
	** \return bool
	** \author gw
	** \date 2017��5��18��
	** \note
	********************************************************************************************************/
	bool CFesData::DeleteForwardingDORelationSourceTagNameArr(std::string &tagName, CForwardingDigitalOutputData *pDigitalOutput)
	{
		if (!CheckTagNameIsValid(tagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pDigitalOutput);
		if (!pDigitalOutput)
		{
			return false;
		}

		auto it = m_arrDOToForwardingDOMap.find(tagName);
		//�а󶨹�ϵ
		if (it != m_arrDOToForwardingDOMap.end())
		{
			auto &arr = it->second;
			auto ite = std::find(arr.begin(), arr.end(), pDigitalOutput);
			if (ite != arr.end())
			{
				arr.erase(ite);
			}
			else
			{
				return false;
			}

			if (arr.size() == 0)
			{
				m_arrDOToForwardingDOMap.erase(it);
			}

		}
		else
		{
			return false;
		}

		return true;
	}

	/*! \fn bool CFesData::DeleteFeviceDeviceAICombineRelaton(const std::string &szTagName)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteFeviceDeviceAICombineRelaton 
	** \details ɾ��ǰ��װ��AI ɾ��AI������Ĺ�ϵ������AI�Ĺ�ϵ
	** \param szTagName 
	** \return bool 
	** \author gw
	** \date 2017��5��27�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteFesDeviceAICombineRelaton(const std::string &szTagName, CAnalogInput *pAnalogInput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}

		Q_ASSERT(pAnalogInput);
		if (!pAnalogInput)
		{
			return false;
		}

		//////////////////////////////////////////////////װ��AI��������Scale��Alarm//////////////////////////////////////////////////////////////////////////
		//ģ����ɾ�������ģ����Scale����ת������ô������ϵҲɾ��
		//[0]
		if (!pAnalogInput->m_strScaleTagName.isEmpty())
		{
			if (!this->DeleteAIsRelationScaleArr(pAnalogInput->m_strScaleTagName.toStdString(), pAnalogInput))
			{
				auto strError = QObject::tr("Delete AIs Relation Scale %1 hash fail!!!").arg(pAnalogInput->m_strScaleTagName);
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}
		}

		//ģ����ɾ�������ģ����Alarm����ת������ô������ϵҲɾ��
		//[1]
		if (!pAnalogInput->m_strAlarmTagName.isEmpty())
		{
			if (!this->DeleteAIsRelationAlarmArr(pAnalogInput->m_strAlarmTagName.toStdString(), pAnalogInput))
			{
				auto strError = QObject::tr("Delete AIs Relation Scale %1 hash fail!!!").arg(pAnalogInput->m_strAlarmTagName);
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}
		}

		/////////////////////////////////////////////ǰ��ת������װ��AI��Scada�������û���������װ��AI//////////////////////////////////////////////////////////////

		//ģ����ɾ�������ǰ���û������󶨹�����ģ�������ڣ���ô����ģ����Ҳ����ɾ��
		//Ϊ�˷������,hash tagname��node tagname + . + ��tagname�����Ժ��޸Ļ���ɾ����node tagname���ߵ�tagname�Ϳ���ֱ���ҵ�������tagname���޸�
		auto pNodeModule = m_pCore->GetModule("NODE_CONFIG");
		Q_ASSERT(pNodeModule);
		if (!pNodeModule)
		{
			return false;
		}
		//fes tagnameתnode tagname
		auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(this->m_szTagName));
		if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
		{
			return false;
		}

		//Source hash tagname 
		std::string strLastTagName = strNodeTagName + "." + pAnalogInput->m_szTagName;

		//ǰ���û�����sourcetagname����ǰ��AI
		//[2]
		if (!this->DeleteUserVariableSourceTagNameArr(strLastTagName))
		{
			return false;
		}

		//ģ����ɾ�������ת��AI source tagname������ģ��������ô������ϵɾ��
		//[3]
		if (!this->DeleteForwardingAISourceTagNameArr(std::string(pAnalogInput->m_szTagName)))
		{
			auto strError = QObject::tr("Delete Forwarding AI Source TagName %1 hash fail!!!").arg(pAnalogInput->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		////////////////////////////////////////////////////֪ͨ�ⲿģ��scada variable module/////////////////////////////////////////////////////
		//[4]
		std::vector<QString>  vec;
		m_pCore->GetModulesName(vec);
		auto pScadaVariableModule = m_pCore->GetModule("scada_variable");
		Q_ASSERT(pScadaVariableModule);
		if (!pScadaVariableModule)
		{
			auto strError = QObject::tr("Get scada variable module poiter failed!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
		pScadaVariableModule->ChangeTagNameNodify(QString(""), QString(strLastTagName.c_str()), Module_ID::DEVICE_AI);
	
		return true;
	}

	/*! \fn bool CFesData::DeleteFesDeviceDICombineRelaton(const std::string &szTagName, CDigitalInput *pDigitalInput)
	********************************************************************************************************* 
	** \brief Config::CFesData::DeleteFesDeviceDICombineRelaton 
	** \details ɾ��ǰ��װ��DI ɾ��DI������Ĺ�ϵ������DI�Ĺ�ϵ
	** \param szTagName 
	** \param pDigitalInput 
	** \return bool 
	** \author gw
	** \date 2017��5��31�� 
	** \note 
	********************************************************************************************************/
	bool CFesData::DeleteFesDeviceDICombineRelaton(const std::string &szTagName, CDigitalInput *pDigitalInput)
	{
		if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
		{
			return false;
		}
		
		Q_ASSERT(pDigitalInput);
		if (!pDigitalInput)
		{
			return false;
		}

		//////////////////////////////////////////////////////////////////////������Ĺ�ϵ AlarmTag DoubleDI˫λң�ŵĴ�ң�� //////////////////////////////////////////////////////////////////////////////////////
		//������ɾ�������DI alram�����澯����ô������ϵɾ��
		//[0]
		if (!pDigitalInput->m_strAlarmTag.isEmpty())
		{
			if (!this->DeleteDIsRelationAlarmArr(pDigitalInput->m_strAlarmTag.toStdString(), pDigitalInput))
			{
				auto strError = QObject::tr("Delete DIs %1 Relation Alarm %2 hash fail!!!").arg(pDigitalInput->m_szTagName).arg(pDigitalInput->m_strAlarmTag);
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}
		}

		//������ɾ���������ң���й�����ɾ��������ϵ
		//[1]
		if (strlen(pDigitalInput->m_szDoubleDI) > 0)
		{
			if (!this->DeleteDIRelationDoubleDIArr(pDigitalInput->m_szDoubleDI, pDigitalInput))
			{
				auto strError = QObject::tr("Delete DIs %1 Relation DoubleDI %2 hash fail!!!").arg(pDigitalInput->m_szTagName).arg(pDigitalInput->m_szDoubleDI);
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////////�����˹��� DIDoubleDI  fes do AssociatedDI  fes do BlockingSignalTag  ǰ���û�����  scada variable��sourcetagname ///////////////////////////////////////////////
		//������ɾ���������ң�ŵĴ�ң�Ź�����ң�ţ���ô������ϵɾ��
		//[2]
		if (!this->DeleteDIDoubleDIArr(pDigitalInput->m_szTagName, pDigitalInput))
		{
			auto strError = QObject::tr("Delete DIs Relation DoubleDI %1 hash fail!!!").arg(pDigitalInput->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		//ɾ��ң�ţ����fes do AssociatedDI������ң�ţ���ô������ϵɾ��
		//[3]
		if (!this->DeleteDOsAssociatedDIArr(pDigitalInput->m_szTagName))
		{
			auto strError = QObject::tr("Delete DOs AssociatedDI %1 hash fail!!!").arg(pDigitalInput->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		//ɾ��ң�ţ����fes do BlockingSignalTag������ң�ţ���ô������ϵɾ��
		//BlockingSignalTag�����Դ�ǰ���û���������(bool����)
		//[4]
		if (!this->DeleteDOsBlockingSignalTagArr(pDigitalInput->m_szTagName))
		{
			auto strError = QObject::tr("Delete DOs BlockingSignalTag %1 hash fail!!!").arg(pDigitalInput->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}


		//note
		//Ϊ�˷������,hash tagname��node tagname + . + ��tagname�����Ժ��޸Ļ���ɾ����node tagname���ߵ�tagname�Ϳ���ֱ���ҵ�������tagname���޸�
		auto pNodeModule = m_pCore->GetModule("NODE_CONFIG");
		Q_ASSERT(pNodeModule);
		if (!pNodeModule)
		{
			auto strError = QObject::tr("Get node module poiter failed!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
		auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(this->m_szTagName));
		if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
		{
			return false;
		}

		//Source hash tagname 
		std::string strLastTagName = strNodeTagName + "." + pDigitalInput->m_szTagName;
		//������ɾ������ǰ���û������󶨹�������������ô����������Ҳ�����޸�
		//[5]
		if (!this->DeleteUserVariableSourceTagNameArr(strLastTagName))
		{
			return false;
		}

		//������ɾ�������ǰ��ת��DI�󶨹�����ģ�������ڣ���ô����ģ����Ҳ�����޸�
		//[6]
		if (!this->DeleteForwardingDISourceTagNameArr(std::string(pDigitalInput->m_szTagName)))
		{
			return false;
		}


		////////////////////////////////////////////////////֪ͨ�ⲿģ��scada variable module/////////////////////////////////////////////////////
		//������ɾ����scada variable��sourcetagname�����Ŀ�������ϵҲɾ��
		//[7]
		std::vector<QString>  vec;
		m_pCore->GetModulesName(vec);
		auto pScadaVariableModule = m_pCore->GetModule("scada_variable");
		Q_ASSERT(pScadaVariableModule);
		if (!pScadaVariableModule)
		{
			auto strError = QObject::tr("Get scada variable module poiter failed!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
		pScadaVariableModule->ChangeTagNameNodify(QString(""), QString(strLastTagName.c_str()), Module_ID::DEVICE_DI);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		return true;
}

/*! \fn bool CFesData::DeleteFesDeviceAOCombineRelaton(const std::string &szTagName, CAnalogOutput *pAnalogOutput)
********************************************************************************************************* 
** \brief Config::CFesData::DeleteFesDeviceAOCombineRelaton 
** \details ɾ��ǰ��װ��AO ɾ��AO������Ĺ�ϵ������AO�Ĺ�ϵ
** \param szTagName 
** \param pAnalogOutput 
** \return bool 
** \author gw
** \date 2017��6��1�� 
** \note 
********************************************************************************************************/
bool CFesData::DeleteFesDeviceAOCombineRelaton(const std::string &szTagName, CAnalogOutput *pAnalogOutput)
{
	if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
	{
		return false;
	}

	Q_ASSERT(pAnalogOutput);
	if (!pAnalogOutput)
	{
		return false;
	}
	
	//////////////////////////////////////////////////////////������Ĺ�ϵ ScaleTagName////////////////////////////////////////////////////////////////////////////
	//AOɾ�������AO scale������ת������ô�����󶨹�ϵҲɾ��
	//[0]
	if (!pAnalogOutput->m_strScaleTagName.isEmpty())
	{
		if (!this->DeleteAOsRelationScaleArr(pAnalogOutput->m_strScaleTagName.toStdString(), pAnalogOutput))
		{
			auto strError = QObject::tr("Fes %1  Delete AOs %2 Relation Scale %3 hash fail!!!").arg(m_szTagName).arg(pAnalogOutput->m_szTagName).arg(pAnalogOutput->m_strScaleTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}


	//////////////////////////////////////////////////////////////////////////////�����˹��� �û�������sourcetagname///////////////////////////////////////////////
	//Ϊ�˷������,hash tagname��node tagname + . + ��tagname�����Ժ��޸Ļ���ɾ����node tagname���ߵ�tagname�Ϳ���ֱ���ҵ�������tagname���޸�
	auto pNodeModule = m_pCore->GetModule("NODE_CONFIG");
	Q_ASSERT(pNodeModule);
	if (!pNodeModule)
	{
		auto strError = QObject::tr("Get node module poiter failed!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}
	auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(this->m_szTagName));
	if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
	{
		return false;
	}


	std::string strLastTagName = strNodeTagName + "." + pAnalogOutput->m_szTagName;
	//������ɾ������ǰ���û�����sourcetagname�󶨹�������������ô����������Ҳ�����޸�
	//[1]
	if (!this->DeleteUserVariableSourceTagNameArr(strLastTagName))
	{
		return false;
	}

	//AOɾ�������ǰ��ת��AO����ǰ��AO tagname����ô������ϵɾ��
	//[2]
	if (!this->DeleteForwardingAOSourceTagNameArr(szTagName))
	{
		auto strError = QObject::tr("Delete ForwardingAO SourceTagName %1 hash fail!!!").arg(szTagName.c_str());
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	////////////////////////////////////////////////////֪ͨ�ⲿģ��scada variable module/////////////////////////////////////////////////////
	//ģ����ɾ����scada variable���û�������sourcetagname�Ĺ�����ϵҲɾ��
	std::vector<QString>  vec;
	m_pCore->GetModulesName(vec);
	auto pScadaVariableModule = m_pCore->GetModule("scada_variable");
	Q_ASSERT(pScadaVariableModule);
	if (!pScadaVariableModule)
	{
		auto strError = QObject::tr("Get scada variable module poiter failed!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}
	pScadaVariableModule->ChangeTagNameNodify(QString(""), QString(strLastTagName.c_str()), Module_ID::DEVICE_AO);

	return true;
}

/*! \fn bool CFesData::DeleteFesDeviceDOCombineRelaton(const std::string &szTagName, CDigitalOutput *pDigitalOutput)
********************************************************************************************************* 
** \brief Config::CFesData::DeleteFesDeviceDOCombineRelaton 
** \details ɾ��ǰ��װ��DO ɾ��DO������Ĺ�ϵ������DO�Ĺ�ϵ
** \param szTagName 
** \param pDigitalOutput 
** \return bool 
** \author gw
** \date 2017��6��2�� 
** \note 
********************************************************************************************************/
bool CFesData::DeleteFesDeviceDOCombineRelaton(const std::string &szTagName, CDigitalOutput *pDigitalOutput)
{
	if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
	{
		return false;
	}

	Q_ASSERT(pDigitalOutput);
	if (!pDigitalOutput)
	{
		return false;
	}

	//////////////////////////////////////////////////////////������Ĺ�ϵ ǰ��do AssociatedDI  ǰ��do BlockingSignal////////////////////////////////////////////////////////////////////////////
	//ɾ��ǰ��DO�����ǰ��do AssociatedDI�й�������ô������ϵɾ��
	if (strlen(pDigitalOutput->m_szAssociatedDI) > 0)
	{
		if (!this->DeleteDOsRelationAssociatedDIArr(pDigitalOutput->m_szAssociatedDI, pDigitalOutput))
		{
			auto strError = QObject::tr("Delete DOs %1 Relation AssociatedDI %2 hash fail!!!").arg(szTagName.c_str()).arg(pDigitalOutput->m_szAssociatedDI);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	//ɾ��ǰ��DO�����ǰ��do BlockingSignal�й�������ô������ϵɾ��
	if (strlen(pDigitalOutput->m_szBlockingSignalTag) > 0)
	{
		if (!this->DeleteDOsRelationBlockingSignalTagArr(pDigitalOutput->m_szBlockingSignalTag, pDigitalOutput))
		{
			auto strError = QObject::tr("Delete DOs %1 Relation AssociatedDI %2 hash fail!!!").arg(szTagName.c_str()).arg(pDigitalOutput->m_szBlockingSignalTag);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}



	//////////////////////////////////////////////�����˹��� ǰ���û�������sourcetagname ǰ��ת��DO  scada�û�������sourcetagname///////////////////////////////////////////////
	//������ɾ��������û������󶨹����Ŀ��������ڣ���ô������������ϵҲ����ɾ��
	//Ϊ�˷������,hash tagname��node tagname + . + ��tagname�����Ժ��޸Ļ���ɾ����node tagname���ߵ�tagname�Ϳ���ֱ���ҵ�������tagname���޸�
	auto pNodeModule = m_pCore->GetModule("NODE_CONFIG");
	Q_ASSERT(pNodeModule);
	if (!pNodeModule)
	{
		auto strError = QObject::tr("Get node module poiter failed!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}
	auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(this->m_szTagName));
	if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
	{
		return false;
	}

	//ǰ���û�����Source hash tagname 
	std::string strLastTagName = strNodeTagName + "." + pDigitalOutput->m_szTagName;

	if (!this->DeleteUserVariableSourceTagNameArr(strLastTagName))
	{
		return false;
	}

	//ǰ��ת��ͨ��sourcetagname����Do tagname��ϵ
	if (!this->DeleteForwardingDOSourceTagNameArr(szTagName))
	{
		auto strError = QObject::tr("Delete ForwardingDO SourceTagName %1 hash fail!!!").arg(szTagName.c_str());
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
		auto strError = QObject::tr("Get scada variable module poiter failed!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}
	pScadaVariableModule->ChangeTagNameNodify(QString(""), QString(strLastTagName.c_str()), Module_ID::DEVICE_DO);

	return true;
}

/*! \fn bool CFesData::DeleteFesAnalogAlarmCombineRelaton(const std::string &szTagName, CAnalogAlarm *pAnalogAlarm)
********************************************************************************************************* 
** \brief Config::CFesData::DeleteFesAnalogAlarmCombineRelaton 
** \details 
** \param szTagName 
** \param pAnalogAlarm 
** \return bool 
** \author gw
** \date 2017��6��6�� 
** \note 
********************************************************************************************************/
bool CFesData::DeleteFesAnalogAlarmCombineRelaton(const std::string &szTagName, CAnalogAlarm *pAnalogAlarm)
{
	if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
	{
		return false;
	}

	Q_ASSERT(pAnalogAlarm);
	if (!pAnalogAlarm)
	{
		return false;
	}
	
	//////////////////////////////AI�ĸ澯��USERVARAIBLE�ĸ澯����ģ�����澯ɾ��/////////////////////////////////////
	//ɾ�������澯 ģ�����澯
	this->DeleteAIsAlarmArr(szTagName);
	//ģ�����澯ɾ��,�û����������ĸ澯hashɾ��
	this->DeleteUserVariableAlarmArr(szTagName);

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief Config::CFesData::DeleteFesDigitalAlarmCombineRelaton 
** \details 
** \param szTagName 
** \param pDigitalAlarm 
** \return bool 
** \author gw
** \date 2017��6��6�� 
** \note 
********************************************************************************************************/
bool CFesData::DeleteFesDigitalAlarmCombineRelaton(const std::string &szTagName, CDigitalAlarm *pDigitalAlarm)
{
	if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
	{
		return false;
	}

	Q_ASSERT(pDigitalAlarm);
	if (!pDigitalAlarm)
	{
		return false;
	}

	////////////////////////DI��USERVARAIBLE�����������澯ɾ��/////////////////////////////////
	//ɾ�������ĸ澯
	this->DeleteDIsAlarmArr(szTagName);
	//�������澯ɾ��,�û����������ĸ澯hashɾ��
	this->DeleteUserVariableAlarmArr(szTagName);

	return true;
}

/*! \fn bool CFesData::DeleteFesScaleCombineRelaton(const std::string &szTagName)
********************************************************************************************************* 
** \brief Config::CFesData::DeleteFesScaleCombineRelaton 
** \details ǰ��ת��ɾ��������ϵ
** \param szTagName 
** \return bool 
** \author gw
** \date 2017��6��7�� 
** \note 
********************************************************************************************************/
bool CFesData::DeleteFesScaleCombineRelaton(const std::string &szTagName)
{
	if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
	{
		return false;
	}

	//////////////////////////////ɾ��ת������AI AO USERVARAIBLE////////////////////////////////////////////
	//ɾ��Fes AIs ScaleTagName������ת��tagname
	this->DeleteAIsScaleArr(szTagName);

	//ɾ��Fes AOs ScaleTagName������ת��tagname
	this->DeleteAOsScaleArr(szTagName);

	//ǰ���û���������ת��ɾ��
	this->DeleteUserVariableScaleArr(szTagName);

	//ת��ɾ����ת��ͨ��AIs�󶨹�����ת��Ҳɾ��
	this->DeleteForwardingAIsScaleArr(szTagName);
	//ת��ɾ����ת��ͨ��AOs�󶨹�����ת��Ҳɾ��
	this->DeleteForwardingAOsScaleArr(szTagName);

	return true;
}

/*! \fn bool CFesData::DeleteFesUserVaraibleCombineRelaton(const std::string &szTagName)
********************************************************************************************************* 
** \brief Config::CFesData::DeleteFesUserVaraibleCombineRelaton 
** \details ǰ���û�����ɾ����ǰ���û����������ĺ͹���ǰ���û������Ĺ�ϵɾ��
** \param szTagName 
** \return bool 
** \author gw
** \date 2017��6��8�� 
** \note 
********************************************************************************************************/
bool CFesData::DeleteFesUserVaraibleCombineRelaton(const std::string &szTagName, CUserVariable *pUser)
{
	if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
	{
		return false;
	}
	
	//////////////////////////////////////////ǰ���û������������=====================================
	//ǰ���û������ĸ澯AlarmTagName�����澯��ϵɾ��
	if (!pUser->m_strAlarmTagName.isEmpty())
	{
		if (!this->DeleteUserVariableRelationAlarmArr(pUser->m_strAlarmTagName.toStdString(), pUser))
		{
			auto strError = QObject::tr("User Variable Relation Alarm %1 of User Variable  %2 delete failed!!!").arg(pUser->m_strAlarmTagName).arg(pUser->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	//ǰ���û�������ת��ScaleTagName����ת����ϵɾ��
	if (!pUser->m_strScaleTagName.isEmpty())
	{
		if (!this->DeleteUserVariableRelationScaleArr(pUser->m_strScaleTagName.toStdString(), pUser))
		{
			auto strError = QObject::tr("User Variable Relation Scale %1 of User Variable  %2 delete failed!!!").arg(pUser->m_strScaleTagName).arg(pUser->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	//ǰ���û�����ɾ�������ǰ���û�����sourcetagname�й�������ô����hashҲɾ���󶨹�ϵ
	if (!pUser->m_strSourceTagName.isEmpty())
	{
		auto list = pUser->m_strSourceTagName.split(".");
		//�ݶ��ж�����
		if (list.count() == 3)
		{
			auto strTmp = list[0] + "." + list[1];
			if (!this->DeleteUserVariableRelationSourceTagNameArr(strTmp.toStdString(), pUser))
			{
				auto strError = QObject::tr("Delete User Variable Relation SourceTagName Arr %1 delete failed!!!").arg(pUser->m_strSourceTagName);
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}
		}
		else
		{	
			auto strError = QObject::tr("User Variable SourceTagName %1 of User Variable Arr %2 is invalid!!!").arg(pUser->m_strSourceTagName).arg(pUser->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	//ǰ���û�����ɾ����ǰ��do BlockingSignalTag����������û���������ô������ϵɾ�� һ�Զ��ϵ
	if (!this->DeleteDOsBlockingSignalTagArr(pUser->m_szTagName))
	{
		auto strError = QObject::tr("Delete DOs BlockingSignalTag %1 hash failed!!!").arg(pUser->m_szTagName);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	//////////////////////////////////////////////����ǰ���û�����//////////////////////////////////////////////////////////////
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
	auto strNodeTagName = pNodeModule->GetNodeNameByFesConfigName(std::string(this->m_szTagName));
	if (!CheckTagNameIsValid(strNodeTagName.c_str(), FES_DESC))
	{
		return false;
	}

	//Source hash tagname 
	auto strSourceTagName = QString("%1.%2").arg(strNodeTagName.c_str()).arg(pUser->m_szTagName);
	if (!this->DeleteUserVariableSourceTagNameArr(strSourceTagName.toStdString()))
	{
		auto strError = QObject::tr("Delete User Variable SourceTagName %1 failed!!!").arg(strSourceTagName);
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
		auto strError = QObject::tr("Get scada variable module poiter failed!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}
	bool bFlag = pScadaVariableModule->ChangeTagNameNodify(QString(""), strSourceTagName, Module_ID::USERVARAIBLE);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = QObject::tr("Delete Scada varaible SourceTagName %1 relation failed!!!").arg(strSourceTagName);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

bool CFesData::DeleteFesForwardingAICombineRelaton(const std::string &szTagName, CForwardingAnalogInputData *pAnalogInput)
{
	if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
	{
		return false;
	}

	Q_ASSERT(pAnalogInput);
	if (!pAnalogInput)
	{
		return false;
	}
	
	//ת��ͨ��AIɾ�������ת��AI�й���sourcetagname hash����ô����hashҲɾ���󶨹�ϵ
	if (!pAnalogInput->m_strSourceTagName.isEmpty())
	{
		if (!this->DeleteForwardingAIRelationSourceTagNameArr(pAnalogInput->m_strSourceTagName.toStdString(), pAnalogInput))
		{
			auto strError = QObject::tr("Delete Forwarding AI Relation SourceTagName Arr %1 delete failed!!!").arg(pAnalogInput->m_strSourceTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	//ת��ͨ��AIɾ�������ת��AI�й���Scale hash����ô����hashҲɾ���󶨹�ϵ
	if (!pAnalogInput->m_strScaleTagName.isEmpty())
	{
		if (!this->DeleteForwardingAIRelationScaleArr(pAnalogInput->m_strScaleTagName.toStdString(), pAnalogInput))
		{
			auto strError = QObject::tr("Delete Forwarding AI Relation Scale Arr %1 delete failed!!!").arg(pAnalogInput->m_strScaleTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	return true;
}

bool CFesData::DeleteFesForwardingDICombineRelaton(const std::string &szTagName, CForwardingDigitalInputData *pDigitalInput)
{
	if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
	{
		return false;
	}

	Q_ASSERT(pDigitalInput);
	if (!pDigitalInput)
	{
		return false;
	}
	
	//ת��ͨ��DIɾ�������ת��DI�й���sourcetagname hash����ô����hashҲɾ���󶨹�ϵ
	if (!pDigitalInput->m_strSourceTagName.isEmpty())
	{
		if (!this->DeleteForwardingDIRelationSourceTagNameArr(pDigitalInput->m_strSourceTagName.toStdString(), pDigitalInput))
		{
			auto strError = QObject::tr("Delete Forwarding DI Relation SourceTagName Arr %1 delete failed!!!").arg(pDigitalInput->m_strSourceTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	return true;
}

bool CFesData::DeleteFesForwardingAOCombineRelaton(const std::string &szTagName, CForwardingAnalogOutputData *pAnalogOutput)
{
	if (!CheckTagNameIsValid(szTagName.c_str(), FES_DESC))
	{
		return false;
	}

	Q_ASSERT(pAnalogOutput);
	if (!pAnalogOutput)
	{
		return false;
	}

	//ת��ͨ��AOɾ�������ת��AO�й���sourcetagname hash����ô����hashҲɾ���󶨹�ϵ
	if (!pAnalogOutput->m_strSourceTagName.isEmpty())
	{
		if (!this->DeleteForwardingAORelationSourceTagNameArr(pAnalogOutput->m_strSourceTagName.toStdString(), pAnalogOutput))
		{
			auto strError = QObject::tr("Delete Forwarding AO %1 Relation SourceTagName Arr %2 delete failed!!!").arg(szTagName.c_str()).arg(pAnalogOutput->m_strSourceTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	//ת��ͨ��AOɾ�������ת��AO�й���Scale hash����ô����hashҲɾ���󶨹�ϵ
	if (!pAnalogOutput->m_strScaleTagName.isEmpty())
	{
		if (!this->DeleteForwardingAORelationScaleArr(pAnalogOutput->m_strScaleTagName.toStdString(), pAnalogOutput))
		{
			auto strError = QObject::tr("Delete Forwarding AO Relation Scale Arr %1 delete failed!!!").arg(szTagName.c_str()).arg(pAnalogOutput->m_strScaleTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	return true;
}

bool CFesData::DeleteFesForwardingDOCombineRelaton(const std::string &szTagName, CForwardingDigitalOutputData *pDigitalOutput)
{
	//ת��ͨ��DOɾ�������ת��DO�й���sourcetagname hash����ô����hashҲɾ���󶨹�ϵ
	if (!pDigitalOutput->m_strSourceTagName.isEmpty())
	{
		if (!this->DeleteForwardingDORelationSourceTagNameArr(pDigitalOutput->m_strSourceTagName.toStdString(), pDigitalOutput))
		{
			auto strError = QObject::tr("Delete Forwarding DO Relation SourceTagName Arr %1 delete failed!!!").arg(pDigitalOutput->m_strSourceTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief Config::CFesData::ChangeNodeTagNameNodifyFesUserVaraible 
** \details 
** \param szNodetagName 
** \param szNodelastTagName 
** \return bool 
** \author gw
** \date 2017��6��13�� 
** \note 
********************************************************************************************************/
bool CFesData::ChangeNodeTagNameNodifyFesUserVaraible(const std::string &szNodetagName, const std::string &szNodelastTagName)
{
	auto it = m_arrSourceTagNameToUserVariableMap.begin();
	std::map <std::string, std::string> vecDelete;
	vecDelete.clear();
	for (; it != m_arrSourceTagNameToUserVariableMap.end(); ++it)
	{
		//key node tagname + "." + data tagname
		QString strTmp = it->first.c_str();
		auto list = strTmp.split(".");
		if (list.count() != 2)
		{
			Q_ASSERT(nullptr);
			return false;
		}

		std::string szNewTagName = szNodetagName + "." + list[1].toStdString();
		//node tagname��ͬ
		if (list[0] == szNodelastTagName.c_str())
		{
			//ɾ��
			if (szNodetagName.empty())
			{
				auto &sourceTagNameArr = it->second;
				for each (auto varSourceTagName in sourceTagNameArr)
				{
					//key node tagname + "." + data tagname + "." + attr tagname
					auto list = varSourceTagName->m_strSourceTagName.split(".");
					//��ʱ�ж�����
					if (list.count() == 3)
					{
						varSourceTagName->m_strSourceTagName = QString("%1").arg(szNodetagName.c_str());
					}
					else
					{
						return false;
					}
				}

				vecDelete.insert(std::pair<std::string, std::string>(it->first, it->first));
			}
			else
			{
				//�޸�
				auto &sourceTagNameArr = it->second;
				for each (auto varSourceTagName in sourceTagNameArr)
				{
					//key node tagname + "." + data tagname + "." + attr tagname
					auto list = varSourceTagName->m_strSourceTagName.split(".");
					//��ʱ�ж�����
					if (list.count() == 3)
					{
						varSourceTagName->m_strSourceTagName = QString("%1.%2.%3").arg(szNodetagName.c_str()).arg(list[1]).arg(list[2]);
					}
					else
					{
						return false;
					}
				}

				vecDelete.insert(std::pair<std::string, std::string>(it->first, szNewTagName));
			}
		}
	}

	for each (auto var in vecDelete)
	{
		if (szNodetagName.empty())
		{
			//ɾ��
			auto it = m_arrSourceTagNameToUserVariableMap.find(var.first);
			if (it != m_arrSourceTagNameToUserVariableMap.end())
			{
				m_arrSourceTagNameToUserVariableMap.erase(it);
			}
			else
			{
				return false;
			}
		}
		else 
		{
			//�޸�
			auto it = m_arrSourceTagNameToUserVariableMap.find(var.first);
			if (it != m_arrSourceTagNameToUserVariableMap.end())
			{
				std::swap(m_arrSourceTagNameToUserVariableMap[var.second], it->second);

				m_arrSourceTagNameToUserVariableMap.erase(it);
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

/*! \fn bool CFesData::LogToFile(const char* filename, const char* log)
********************************************************************************************************* 
** \brief Config::CFesData::LogToFile 
** \details ǰ�ñ���������־�ļ�
** \param filename 
** \param log 
** \return bool 
** \author xingzhibing
** \date 2017��6��23�� 
** \note 
********************************************************************************************************/
bool CFesData::LogToFile(const char* pFilename, const char* pLog)
{
	MYLIB::Log2File(pFilename, pLog, true);
	
	auto &arrChannel = m_arrMaps[HASHID::CHANNELHASHID];
	
	std::vector<std::pair<std::string, CBaseData *>> elems(arrChannel.begin(), arrChannel.end());
	std::sort(elems.begin(), elems.end());
	
	//ͨ��
	MYLIB::Log2File(pFilename, "[Start Channel Log Succes!!!]", true);
	for each (auto var in elems)
	{
		auto pTmp = static_cast<CChannelData *>(var.second);
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			MYLIB::Log2File(pFilename, "[Channel Log �� Channel Pointer is nullptr!!!]", true);

			return false;
		}

		auto strTmp = QString("[Start Channel %1 Log Succes!!!]").arg(var.first.c_str());
		var.second;
	}

	return true;
}

/*! \fn bool CFesConfigData::LoadChildItem(CFesGroup *pFesGroup, QDomElement &e, const QString &szRoot)
	*********************************************************************************************************
	** \brief Config::CFesConfigData::LoadChildItem
	** \details �ݹ�ǰ����
	** \param pFesGroup
	** \param e
	** \param szRoot
	** \return bool
	** \author gw
	** \date 2016��10��18��
	** \note
	********************************************************************************************************/
	bool CFesConfigData::LoadChildItem(CFesData *pFes, const QString &filePath)
	{
		LoadFesFile(pFes, filePath);

		return true;
	}

	CFesData *CFesConfigData::AddNewFesData(CFesGroup *pFesGroup)
	{
		Q_ASSERT(s_pGlobleCore);
		if (!s_pGlobleCore)
		{
			return nullptr;
		}

		CFesData *pData = /*pFesGroup->CreateFesItem();*/ new CFesData(s_pGlobleCore);
		Q_ASSERT(pData);
		if (pData == nullptr)
			return nullptr;

		auto index = (int)m_arrHashTagNameMap.size();
		while (true)
		{
			QString strFes = QString("fes%1").arg(index);
			QString strDesc = QString(QObject::tr("fes%1")).arg(index);

			std::string str = strFes.toStdString();
			//ǰ��hash����
			std::unordered_map<std::string, CFesData *>::const_iterator it = m_arrHashTagNameMap.find(str);
			if (it == m_arrHashTagNameMap.end())
			{
				auto &tmp = std::make_pair(str, pData);
				m_arrHashTagNameMap.insert(tmp);
			}
			else
			{
				index++;

				continue;
			}

			pData->m_nID = index;

			bool bFlag = CheckTagNameIsValid(strFes, FES_DESC);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return nullptr;
			}



			memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
			int nSize = strFes.size();
			strncpy(pData->m_szTagName, strFes.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

			if (!CheckDescIsValid(strDesc, FES_DESC))
			{
				pData->m_strDescription = strDesc.mid(0, MAX_NAME_LENGTH_SCADASTUDIO / 2);
			}
			else
			{
				pData->m_strDescription = strDesc;
			}

			//������
			pFesGroup->m_arrFesItem.insert(std::make_pair(str, pData));
			break;
		}
		QString str = QString("%1").arg(pData->m_szTagName);

		m_arrFesDatas.push_back(pData);

		//����hash
		pData->PushFesTagNameHashMap(FESHASHID, str, pData);

		return pData;
	}


	/*! \fn	bool CFesConfigData::DeleteFesItem(std::string szFesTag)
	*********************************************************************************************************
	** \brief Config::CFesConfigData::DeleteFesItem
	** \details m_arrTagNameMap hash ���ٲ��� ͳ��ǰ�ø��� �����ڴ�ṹ��group
	** \param szFesTag
	** \return bool
	** \author gw
	** \date 2016��10��20��
	** \note
	********************************************************************************************************/
	bool CFesConfigData::DeleteHashFesItem(std::string &szFesTag)
	{
		auto it = m_arrHashTagNameMap.find(szFesTag);
		if (it != m_arrHashTagNameMap.end())
		{
			m_arrHashTagNameMap.erase(it);

			return true;
		}

		return false;
	}

	bool CFesConfigData::CheckModifyFesTagNameExist(const QString &strFesTagName)
	{
		auto it = m_arrHashTagNameMap.find(strFesTagName.toStdString());
		if (it != m_arrHashTagNameMap.end())
		{
			return true;
		}

		return false;
	}
	/*! \fn bool CFesConfigData::SaveProjectFesNode(QDomDocument *pXml, QDomElement &e)
	*********************************************************************************************************
	** \brief Config::CFesConfigData::SaveProjectFesNode
	** \details
	** \param pXml
	** \param e
	** \return bool
	** \author gw
	** \date 2016��10��18��
	** \note
	********************************************************************************************************/
	bool CFesConfigData::SaveProjectFesNode(QDomDocument *pXml, QDomElement &e, const QString &szRoot, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		m_pFesGroup;
		Q_ASSERT(m_pFesGroup);
		if (!m_pFesGroup)
		{
			return false;
		}

		MYLIB::Log2File(LOG_FES_LOG, "[start Fes Log]", false);
		//���湤���ļ�
		SaveChildNode(pXml, e, m_pFesGroup);

		//�����к�
		int nOccNo = 0;
		SaveChildItem(m_pFesGroup, szRoot, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

		

		return true;
	}

	/*! \fn bool CFesConfigData::SaveChildNode(QDomDocument *pXml, QDomElement &e, CFesGroup *pFesGroup)
	*********************************************************************************************************
	** \brief Config::CFesConfigData::SaveChildNode
	** \details
	** \param pXml
	** \param e
	** \param pFesGroup
	** \return bool
	** \author gw
	** \date 2016��10��18��
	** \note
	********************************************************************************************************/
	bool CFesConfigData::SaveChildNode(QDomDocument *pXml, QDomElement &e, CFesGroup *pFesGroup)
	{
		Q_ASSERT(pXml);
		Q_ASSERT(pFesGroup);
		if (pXml == nullptr || pFesGroup == nullptr)
		{
			return false;
		}

		auto it = pFesGroup->m_arrFesGroup.begin();
		for (; it != pFesGroup->m_arrFesGroup.end(); ++it)
		{
			QDomElement fElet = pXml->createElement("group");
			fElet.setAttribute("name", it->second->m_strGroup);

			e.appendChild(fElet);

			SaveChildNode(pXml, fElet, it->second);
		}

		auto ite = pFesGroup->m_arrFesItem.begin();
		for (; ite != pFesGroup->m_arrFesItem.end(); ++ite)
		{
			QDomElement fElet = pXml->createElement("file");
			auto strFile = QString("%1.xml").arg(ite->second->m_szTagName);
			fElet.setAttribute("name", ite->second->m_szTagName);

			e.appendChild(fElet);
		}

		return true;
	}

	/*! \fn bool CFesConfigData::SaveChildItem(CFesGroup *pFesGroup, const QString &szRoot)
	*********************************************************************************************************
	** \brief Config::CFesConfigData::SaveChildItem
	** \details ����ǰ����
	** \param pFesGroup
	** \param szRoot
	** \return bool
	** \author gw
	** \date 2016��10��18��
	** \note
	********************************************************************************************************/
	bool CFesConfigData::SaveChildItem(CFesGroup *pFesGroup, const QString &szRoot, int nOccNo
		, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)

	{
		auto it = pFesGroup->m_arrFesItem.begin();
		for (; it != pFesGroup->m_arrFesItem.end(); ++it)
		{
			nOccNo++;
			it->second->SaveFesData(szRoot, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);
		}

		auto ite = pFesGroup->m_arrFesGroup.begin();
		for (; ite != pFesGroup->m_arrFesGroup.end(); ++ite)
		{
			SaveChildItem(ite->second, szRoot, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);
		}

		return true;
	}

	/*! \fn bool CFesConfigData::LoadProjectFesNode(QDomElement &e)
	*********************************************************************************************************
	** \brief Config::CFesConfigData::LoadProjectFesNode
	** \details ���ع����ļ�
	** \param e
	** \return bool
	** \author gw
	** \date 2016��10��18��
	** \note
	********************************************************************************************************/
	bool CFesConfigData::LoadProjectFesNode(QDomElement &e, const QString &szRoot)
	{
		QDomNode n = e.firstChild();

		m_arrHashTagNameMap.clear();

		while (!n.isNull())
		{
			QDomElement e = n.toElement(); // try to convert the node to an element.

			auto strTmp = e.tagName();
			if (strTmp.compare("group") == 0)
			{
				// �ļ�·��
				QString strGroupName = e.attribute("name");
				if (strGroupName.isEmpty() == false)
				{
					CFesGroup *pFesGroup = new CFesGroup;
					pFesGroup->m_strGroup = strGroupName;
					LoadChildGroup(pFesGroup, e, szRoot);

					m_pFesGroup->m_arrFesGroup.insert(std::make_pair(strGroupName.toStdString(), pFesGroup));
					//vec.append(strFile);
				}
				else
				{
					//TODO LOG
				}
			}
			else if (strTmp.compare("file") == 0)
			{
				QString strTagName = e.attribute("name");
				CFesData *pFes = new CFesData(s_pGlobleCore);
				memset(pFes->m_szTagName, 0, sizeof(pFes->m_szTagName));
				int nSize = strTagName.size();
				strncpy(pFes->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
				m_pFesGroup->m_arrFesItem.insert(std::make_pair(strTagName.toStdString(), pFes));

				if (LoadChildItem(pFes, pFes->m_szTagName))
				{

				}
			}

			n = n.nextSibling();
		}

		return true;
	}


	/*! \fn bool CFesConfigData::LoadChildGroup(CFesGroup *pFesGroup, QDomElement &e, const QString &szRoot)
	*********************************************************************************************************
	** \brief Config::CFesConfigData::LoadChildGroup
	** \details �ݹ������
	** \param pFesGroup
	** \param e
	** \param szRoot
	** \return bool
	** \author gw
	** \date 2016��10��18��
	** \note
	********************************************************************************************************/
	bool CFesConfigData::LoadChildGroup(CFesGroup *pFesGroup, QDomElement &e, const QString &szRoot)
	{
		QDomNode n = e.firstChild();

		while (!n.isNull())
		{
			QDomElement e = n.toElement(); // try to convert the node to an element.

			auto strTmp = e.tagName();
			if (strTmp.compare("group") == 0)
			{
				QString strGroupName = e.attribute("name");
				CFesGroup *pFesGroupChild = new CFesGroup;
				pFesGroupChild->m_strGroup = strGroupName;

				pFesGroup->m_arrFesGroup.insert(std::make_pair(strGroupName.toStdString(), pFesGroupChild));

				LoadChildGroup(pFesGroupChild, e, szRoot);
			}
			else if (strTmp.compare("file") == 0)
			{
				QString strTagName = e.attribute("name");
				CFesData *pFes = new CFesData(s_pGlobleCore);
				memset(pFes->m_szTagName, 0, sizeof(pFes->m_szTagName));
				int nSize = strTagName.size();
				strncpy(pFes->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
				pFesGroup->m_arrFesItem.insert(std::make_pair(strTagName.toStdString(), pFes));

				LoadChildItem(pFes, pFes->m_szTagName);
			}

			n = n.nextSibling();
		}

		return true;
	}


	void CFesConfigData::Clear()
	{
		//STLDeleteElements(&m_arrFesDatas);
		m_arrFesDatas.clear();

		m_arrHashTagNameMap.clear();


		if (m_pFesGroup)
		{
			delete m_pFesGroup;
			m_pFesGroup = nullptr;

			//m_pFesGroup = new CFesGroup;
		}
	}

	/*! \fn void CFesConfigData::Create()
	*********************************************************************************************************
	** \brief Config::CFesConfigData::Create
	** \details ������
	** \return void
	** \author gw
	** \date 2017��1��5��
	** \note
	********************************************************************************************************/
	void CFesConfigData::Create()
	{
		if (!m_pFesGroup)
		{
			m_pFesGroup = new CFesGroup;
		}
	}

	//************************************
	// Method:    LogToFile
	// FullName:  Config::CFesConfigData::LogToFile
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: char * filename
	// Parameter: char * log
	//************************************
	bool CFesConfigData::LogToFile(const char * filename, const char * log)
	{
		MYLIB::Log2File(filename, log, true);
		
		m_arrHashTagNameMap;

		std::vector<std::pair<std::string, CFesData *>> elems(m_arrHashTagNameMap.begin(), m_arrHashTagNameMap.end());
		std::sort(elems.begin(), elems.end());

		for each (auto var in elems)
		{
			auto strTmp = QString("[Start Fes %1 Log Succes!!!]").arg(var.first.c_str());
			var.second->LogToFile(filename, strTmp.toStdString().c_str());
		}

		return false;
	}
	//
	/*! \fn CConfigData::CConfigData()
	*********************************************************************************************************
	** \brief Config::CConfigData::CConfigData
	** \details ��������
	** \return
	** \author gw
	** \date 2016��9��1��
	** \note
	********************************************************************************************************/
	CFesConfigData::CFesConfigData()
		:m_pFesGroup(nullptr)
	{
		m_arrFesDatas.clear();

		m_arrHashTagNameMap.clear();

		//m_arrFesGroupMap.clear();

		m_pFesGroup = new CFesGroup;
	}

	/*! \fn CConfigData::~CConfigData()
	*********************************************************************************************************
	** \brief Config::CConfigData::~CConfigData
	** \details
	** \return
	** \author gw
	** \date 2016��9��1��
	** \note
	********************************************************************************************************/

	CFesConfigData::~CFesConfigData()
	{
		// 		for each (auto var in m_arrFesDatas)
		// 		{
		// 			delete var;
		// 			var = nullptr;
		// 		}
		//		STLDeleteElements(&m_arrFesDatas);

		//for (auto it = m_arrFesGroupMap.begin(); it != m_arrFesGroupMap.end(); ++it)
		//{
		//	delete it->second;
		//}
		//m_arrFesGroupMap.clear();

		//for (auto it = m_arrTagNameMap.begin(); it != m_arrTagNameMap.end(); ++it)
		//{
		//	delete it->second;
		//}
		m_arrHashTagNameMap.clear();

		delete m_pFesGroup;
		m_pFesGroup = nullptr;
	}

	bool CFesConfigData::LoadFesFile(CFesData *pFesData, const QString &filePath)
	{
		// 
		QXmlStreamReader xml;
		auto strPath = s_pGlobleCore->GetProjMgr()->GetFile()->fileName();
		auto nIndex = strPath.lastIndexOf("/");
		strPath = strPath.mid(0, nIndex);
		strPath += "/scada/";
		//for (auto const &var : vecFilePaths)
		{
			QString strTmp = strPath + "/" + filePath + ".xml";
			QFile file(strTmp);

			if (!file.open(QFile::ReadOnly | QFile::Text))
			{
				auto strError = QString(QObject::tr("load fes file %1 failed!!!")).arg(strTmp);
				s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), 1);

				return false;
			}

			xml.setDevice(&file);

			if (xml.readNextStartElement())
			{
				QString strTmp = xml.name().toString();

				QString strTmp1 = xml.attributes().value("version").toString();

				if (xml.name() == "fes" && xml.attributes().value("version") == "1.0")
				{
					//CFesData *pFesData = new CFesData(m_pCore);

					QString strTagName = xml.attributes().value("TagName").toString();

					//sprintf_s(pFesData->m_szTagName, TAGNAME_SIZE + 1, "%s", strTagName.toStdString().c_str());
					int nSize = strTagName.size();
					strncpy(pFesData->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					QString strDesc = xml.attributes().value("Description").toString();
					pFesData->m_strDescription = strDesc;

					QString strGroup = xml.attributes().value("GroupName").toString();
					pFesData->m_strGroupName = strGroup;

					int nID = xml.attributes().value("ID").toInt();
					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString("ID %1 of fes %2 is invalid < 0").arg(nID).arg(strTagName);
						s_pGlobleCore->LogMsg(FES_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);

						delete pFesData;
						pFesData = nullptr;

						return false;
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString("fes %1 is empty").arg(strTagName);
						s_pGlobleCore->LogMsg(FES_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);

						delete pFesData;
						pFesData = nullptr;

						return false;
					}

					//fes tagname�Ƿ����
					auto it = m_arrHashTagNameMap.find(pFesData->m_szTagName);
					if (it != m_arrHashTagNameMap.end())
					{
						file.close();

						return false;
					}

					pFesData->LoadFesData(xml);

					m_arrFesDatas.push_back(pFesData);

					//ǰ�ô�hash mapҲҪ����ǰ�õ�tagname
					pFesData->PushFesTagNameHashMap(FESHASHID, pFesData->m_szTagName, pFesData);

					//push��fes hash map
					m_arrHashTagNameMap.insert(std::make_pair(pFesData->m_szTagName, pFesData));
				}
			}
			else
			{
				auto strError = QString("Fes file %1 format is error!!!").arg(strTmp);
				s_pGlobleCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
			}

			file.close();
		}

		return true;
	}

	/*! \fn CFesGroup::CFesGroup()
	*********************************************************************************************************
	** \brief Config::CFesGroup::CFesGroup
	** \details ǰ����
	** \return
	** \author gw
	** \date 2016��10��14��
	** \note
	********************************************************************************************************/
	CFesGroup::CFesGroup()
	{

	}

	CFesGroup::~CFesGroup()
	{
		for (auto it = m_arrFesGroup.begin(); it != m_arrFesGroup.end(); ++it)
		{
			delete it->second;
		}
		m_arrFesGroup.clear();

		for (auto it = m_arrFesItem.begin(); it != m_arrFesItem.end(); ++it)
		{
			delete it->second;
		}
		m_arrFesItem.clear();
	}

	/*! \fn bool CFesGroup::CheckModifyGroupNameExist(CFesGroup *pFesGroup, QString strGroupName)
	*********************************************************************************************************
	** \brief Config::CFesGroup::CheckModifyGroupNameExist
	** \details ����޸ĵ������Ƿ��ظ�
	** \param pFesGroup
	** \param strGroupName
	** \return bool
	** \author gw
	** \date 2016��10��19��
	** \note
	********************************************************************************************************/
	bool CFesGroup::CheckModifyGroupNameExist(CFesGroup *pFesGroup, QString strGroupName)
	{
		for (auto it = m_arrFesGroup.begin(); it != m_arrFesGroup.end(); ++it)
		{
			if (it->second != pFesGroup)
			{
				if (it->second->m_strGroup == strGroupName)
				{
					return true;
				}
			}
		}

		return false;
	}

	/*! \fn CFesGroup *CFesGroup::CreateGroup(const QString& strGroupName)
	*********************************************************************************************************
	** \brief Config::CFesGroup::CreateGroup
	** \details ����ǰ����
	** \param strGroupName
	** \return CFesGroup *
	** \author gw
	** \date 2016��10��17��
	** \note
	********************************************************************************************************/
	CFesGroup *CFesGroup::CreateGroup()
	{
		int nIndex = (int)m_arrFesGroup.size();
		QString strName = "";
		while (true)
		{
			strName = QString("%1%2").arg(GROUP_NAME).arg(nIndex);

			auto it = m_arrFesGroup.find(strName.toStdString());
			if (it == m_arrFesGroup.end())
			{
				auto pGroup = new CFesGroup;
				pGroup->m_strGroup = strName;

				m_arrFesGroup.insert(std::make_pair(strName.toStdString(), pGroup));

				return pGroup;
			}
			else
			{
				nIndex++;
			}
		}

		return nullptr;
	}


	/*! \fn void CFesGroup::ModifyGroup(CFesGroup *pFesGroup)
	*********************************************************************************************************
	** \brief Config::CFesGroup::ModifyGroup
	** \details �޸�map������
	** \param pFesGroup
	** \return void
	** \author gw
	** \date 2016��10��19��
	** \note
	********************************************************************************************************/
	bool CFesGroup::ModifyGroup(const QString &strTagName, const QString &strLastTagName)
	{
		auto ite = m_arrFesGroup.find(strLastTagName.toStdString());
		if (ite != m_arrFesGroup.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.

			ite->second->m_strGroup = strTagName;
			std::swap(m_arrFesGroup[strTagName.toStdString()], ite->second);
			// Erase old key-value from map
			m_arrFesGroup.erase(ite);

			return true;
		}

		//auto it = m_arrFesGroup.begin();
		//for (; it != m_arrFesGroup.end(); ++it)
		//{
		//	if (it->second->ModifyGroup(strTagName, strLastTagName))
		//	{
		//		return true;
		//	}
		//}

		return false;
	}


	/*! \fn bool CFesGroup::DeletGroup(CFesGroup *pFesGroup)
	*********************************************************************************************************
	** \brief Config::CFesGroup::DeletGroup
	** \details ɾ����
	** \param pFesGroup
	** \return bool
	** \author gw
	** \date 2016��10��19��
	** \note
	********************************************************************************************************/
	bool CFesGroup::DeletGroup(CFesGroup *pFesGroup, const QString &strTagGroup)
	{
		Q_UNUSED(pFesGroup);

		auto it = m_arrFesGroup.find(strTagGroup.toStdString());
		if (it == m_arrFesGroup.end())
		{
			return false;
		}

		delete it->second;

		m_arrFesGroup.erase(it);

		return true;
	}

	/*! \fn bool CFesGroup::DeletItem(CFesGroup *pFesGroup, const QString &strTagGroup)
	*********************************************************************************************************
	** \brief Config::CFesGroup::DeletItem
	** \details ɾ�����ڵ���
	** \param pFesGroup
	** \param strTagGroup
	** \return bool
	** \author gw
	** \date 2016��10��19��
	** \note
	********************************************************************************************************/
	bool CFesGroup::DeletItem(CFesData *pFes, const QString &strTagName)
	{
		Q_UNUSED(pFes);

		auto it = m_arrFesItem.find(strTagName.toStdString());
		if (it == m_arrFesItem.end())
		{
			return false;
		}

		delete it->second;

		m_arrFesItem.erase(it);

		return true;
	}


	/*! \fn
	*********************************************************************************************************
	** \brief Config::CFesGroup::CreateFesItem
	** \details �������ǰ����
	** \return CFesData *
	** \author gw
	** \date 2016��10��18��
	** \note
	********************************************************************************************************/
	CFesData *CFesGroup::CreateFesItem()
	{
		//int nIndex = m_arrFesItem.size();
		//QString strName = "";
		////while (true)
		//{
		//	strTagName = QString("%1%2").arg(tr("fes")).arg(nIndex);

		//	auto it = m_arrFesItem.find(strName.toStdString());
		//	if (it == m_arrFesItem.end())
		//	{
		//		auto pFesItem = new CFesData(s_pGlobleCore);
		//		memset(pFesItem->m_szTagName, 0, sizeof(pFesItem->m_szTagName));
		//		int nSize = strName.size();
		//		strncpy(pFesItem->m_szTagName, strName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, nSize));

		//		m_arrFesItem.insert(std::make_pair(, pFesItem));

		//		return pFesItem;
		//	}
		//	else
		//	{
		//		nIndex++;
		//	}
		//}

		return nullptr;
	}


	/*! \fn bool CFesGroup::ModifyItem(const QString &strTagName, const QString &strLastTagName)
	*********************************************************************************************************
	** \brief Config::CFesGroup::ModifyItem
	** \details
	** \param strTagName
	** \param strLastTagName
	** \return bool
	** \author gw
	** \date 2016��10��19��
	** \note
	********************************************************************************************************/
	bool CFesGroup::ModifyItem(const QString &strTagName, const QString &strLastTagName)
	{
		auto ite = m_arrFesItem.find(strLastTagName.toStdString());
		if (ite != m_arrFesItem.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.

			memset(ite->second->m_szTagName, 0, sizeof(ite->second->m_szTagName));
			int nSize = strTagName.size();
			strncpy(ite->second->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			std::swap(m_arrFesItem[strTagName.toStdString()], ite->second);
			// Erase old key-value from map
			m_arrFesItem.erase(ite);

			return true;
		}

		auto it = m_arrFesGroup.begin();
		for (; it != m_arrFesGroup.end(); ++it)
		{
			if (it->second->ModifyItem(strTagName, strLastTagName))
			{
				return true;
			}
		}

		return false;
	}


}