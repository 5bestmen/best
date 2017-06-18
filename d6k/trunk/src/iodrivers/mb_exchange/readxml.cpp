#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QDebug>
#include <QDomDocument>
#include "readxml.h"


CReadXML::CReadXML()
 {

}

CReadXML::~CReadXML()
{
			
}

 /*! \fn 
 ********************************************************************************************************* 
 ** \brief  CReadXML::ReadXML
 ** \details 读取xml文件
 ** \param  strFilename 文件名
 ** \return bool
 ** \author HuangYuqing
 ** \date   2016年8月25日
 ** \note 
 ********************************************************************************************************/
 bool CReadXML::ReadXML(const QString &strFilename)
{
	if (strFilename.isEmpty())
	{
		//qDebug() << "Warning, file is null, please check!";
		QMessageBox::information(NULL,QString("Warning"), QString("File is null, please check!"));
		return false;
	}
	QFile fConfigFile(strFilename);
	if (fConfigFile.open(QFile::ReadOnly | QFile::Text))
	{
		QDomDocument  document;

		// 设置文件，这时会将流设置为初始状态
		if (!document.setContent(&fConfigFile))
		{
			fConfigFile.close();
			return false;
		}

		QDomElement root = document.documentElement();
		if (root.tagName() != "config")
		{
			fConfigFile.close();
			return false;
		}

		AnalyseXMLData(&root);

		fConfigFile.close();
	}
	else
	{
		//qDebug() << "Warning, open file error!";
		QMessageBox::information(NULL, QString("Error"), QString("Open file error!"));
		return false;
	}
	return true;
}

 /*! \fn 
 ********************************************************************************************************* 
 ** \brief  CReadXML::AnalyseXMLData
 ** \details 解析xml文件
 ** \param  root 根节点
 ** \return void
 ** \author HuangYuqing
 ** \date   2016年8月25日
 ** \note 
 ********************************************************************************************************/
 void CReadXML::AnalyseXMLData(QDomElement* root)
{
	QDomNode node = root->firstChild();
	for (; !node.isNull(); node = node.nextSibling())
	{
		if (node.nodeName() == "station")
		{
			//QSysMesInfo *pSysInfo=new QSysMesInfo;
			QDomElement pAIElement = node.toElement();
			if (pAIElement.isNull())
			{
				continue;
			}

			if (!ReadStation(pAIElement))
			{
				// log
				continue;
			}
		}

		if (node.nodeName() == "device")
		{
			//QSysMesInfo *pSysInfo=new QSysMesInfo;
			QDomElement pAIElement = node.toElement();
			if (pAIElement.isNull())
			{
				continue;
			}

			if (!ReadDevice(pAIElement))
			{
				// log
				continue;
			}
		}

		if (node.nodeName() == "paraconfig")
		{
			//QSysMesInfo *pSysInfo=new QSysMesInfo;
			QDomElement pAIElement = node.toElement();
			if (pAIElement.isNull())
			{
				continue;
			}

			if (!ReadParaconfig(pAIElement))
			{
				// log
				continue;
			}
		}
	}
}
	
 /*! \fn 
 ********************************************************************************************************* 
 ** \brief  CReadXML::ReadStation
 ** \details 解析厂站数据
 ** \param  dElement
 ** \return bool
 ** \author HuangYuqing
 ** \date   2016年8月25日
 ** \note 
 ********************************************************************************************************/
 bool CReadXML::ReadStation(QDomElement& dElement)
{
	QDomNode dnChild;
	for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
	{
		QDomElement dData = dnChild.toElement();

		SStationInfo s_StationInfo;
		s_StationInfo.Station_id = dData.attribute("StationID").toUInt();
		s_StationInfo.Station_name = dData.attribute("StationName");
	}

	return true;
}

 /*! \fn 
 ********************************************************************************************************* 
 ** \brief  CReadXML::ReadDevice
 ** \details 解析设备数据
 ** \param  dElement
 ** \return bool
 ** \author HuangYuqing
 ** \date   2016年8月25日
 ** \note 
 ********************************************************************************************************/
 bool CReadXML::ReadDevice(QDomElement& dElement)
 {
	 QDomNode dnChild;
	 for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
	 {
		 QDomElement dData = dnChild.toElement();

		 SDeviceInfo s_DeviceInfo;
		 s_DeviceInfo.Device_id = dData.attribute("DeviceID").toUInt();
		 s_DeviceInfo.Device_name = dData.attribute("DeviceName");
		 s_DeviceInfo.Device_address = dData.attribute("DeviceAddr").toInt();
		 s_DeviceInfo.Protocol = dData.attribute("Protocol");
		 s_DeviceInfo.strIP = dData.attribute("IP");
		 s_DeviceInfo.Port = dData.attribute("Port").toInt();
		 s_DeviceInfo.AnalogStartAddr = dData.attribute("AnalogStartAddr").toInt();
		 s_DeviceInfo.BinaryStartAddr = dData.attribute("BinaryStartAddr").toInt();
		 s_DeviceInfo.KwhStartAddr = dData.attribute("KwhStartAddr").toInt();
		 s_DeviceInfo.AnalogTelectrlStartAddr = dData.attribute("AnalogTelectrlStartAddr").toInt();
		 s_DeviceInfo.BinaryTelectrlStartAddr = dData.attribute("BinaryTelectrlStartAddr").toInt();
		 s_DeviceInfo.FuncodeAnalog = dData.attribute("FuncodeAnalog").toInt();
		 s_DeviceInfo.FuncodeBinary = dData.attribute("FuncodeBinary").toInt();
		 s_DeviceInfo.FuncodeKwh = dData.attribute("FuncodeKwh").toInt();
		 s_DeviceInfo.FuncodeTelectrlAnalog = dData.attribute("FuncodeTelectrlAnalog").toInt();
		 s_DeviceInfo.FuncodeTelectrlBinary = dData.attribute("FuncodeTelectrlBinary").toInt();

		 m_mapDevice.insert(s_DeviceInfo.Device_id, s_DeviceInfo);
	 }

	 return true;
 }


 /*! \fn 
 ********************************************************************************************************* 
 ** \brief  CReadXML::ReadParaconfig
 ** \details 解析参数数据
 ** \param  dElement
 ** \return bool
 ** \author HuangYuqing
 ** \date   2016年8月25日
 ** \note 
 ********************************************************************************************************/
 bool CReadXML::ReadParaconfig(QDomElement& dElement)
 {
	 QDomNode dnChild;
	 for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
	 {
		 QDomElement dData = dnChild.toElement();

		 if (dData.nodeName() == "d")
		 {
			 //QSysMesInfo *pSysInfo=new QSysMesInfo;
			 QDomElement pAIElement = dData.toElement();
			 int DeviceID = dData.attribute("DeviceID").toInt();
			 //int DeviceAddr = dData.attribute("DeviceAddr").toInt();

			 if (pAIElement.isNull())
			 {
				 continue;
			 }

			 if (!ReadSingleDeviceInfo(pAIElement, DeviceID))
			 {
				 // log
				 continue;
			 }
		 }
	 }
	 return true;
 }

 /*! \fn 
 ********************************************************************************************************* 
 ** \brief  CReadXML::ReadSingleDeviceInfo
 ** \details 解析设备参数信息
 ** \param  dElement
 ** \return bool
 ** \author HuangYuqing
 ** \date   2016年8月25日
 ** \note 
 ********************************************************************************************************/
 bool CReadXML::ReadSingleDeviceInfo(QDomElement& dElement, int DeviceID)
 {
	 QDomNode dnChild;
	 for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
	 {
		 QDomElement dData = dnChild.toElement();

		 if (dData.nodeName() == "analog")
		 {
			 //QSysMesInfo *pSysInfo=new QSysMesInfo;
			 QDomElement pAIElement = dData.toElement();
			 if (pAIElement.isNull())
			 {
				 continue;
			 }

			 if (!ReadAnalog(pAIElement, DeviceID))
			 {
				 // log
				 continue;
			 }
		 }
		 else if (dData.nodeName() == "binary")
		 {
			 //QSysMesInfo *pSysInfo=new QSysMesInfo;
			 QDomElement pAIElement = dData.toElement();
			 if (pAIElement.isNull())
			 {
				 continue;
			 }

			 if (!ReadBinary(pAIElement, DeviceID))
			 {
				 // log
				 continue;
			 }
		 }
		 else if (dData.nodeName() == "kwh")
		 {
			 //QSysMesInfo *pSysInfo=new QSysMesInfo;
			 QDomElement pAIElement = dData.toElement();
			 if (pAIElement.isNull())
			 {
				 continue;
			 }

			 if (!ReadKWH(pAIElement, DeviceID))
			 {
				 // log
				 continue;
			 }
		 }
		 else
		 {
			 //log
		 }

	 }

	 return true;
 }

 /*! \fn 
 ********************************************************************************************************* 
 ** \brief  CReadXML::ReadAnalog
 ** \details 解析Analog
 ** \param  dElement
 ** \return bool
 ** \author HuangYuqing
 ** \date   2016年8月25日
 ** \note 
 ********************************************************************************************************/
 bool CReadXML::ReadAnalog(QDomElement& dElement, int DeviceID)
 {
	 QDomNode dnChild;
	 for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
	 {
		 QDomElement dData = dnChild.toElement();

		 SAiInfo s_AiInfo;

		 s_AiInfo.nAiID = dData.attribute("ID").toInt();
		 s_AiInfo.strAiName = dData.attribute("Name");
		 s_AiInfo.nAddr = dData.attribute("Address").toInt();
		 s_AiInfo.nTransType = dData.attribute("TransType").toInt();

		 if (s_AiInfo.nTransType != 0)
		 {
			 QString strTransInfo = dData.attribute("TransInfo");
			 QByteArray byTransInfo = strTransInfo.toLatin1();

			 // 解析目标和条件表达式
			 if (!AnalyzeExpression(byTransInfo.data(), s_AiInfo.transInfo))
			 {
				 return false;
			 }
		 }


		 m_mapAnalog[DeviceID].push_back(s_AiInfo);

	 }

	 return true;
 }

 /*! \fn 
 ********************************************************************************************************* 
 ** \brief  CReadXML::ReadBinary
 ** \details 解析Binary
 ** \param  dElement
 ** \return bool
 ** \author HuangYuqing
 ** \date   2016年8月25日
 ** \note 
 ********************************************************************************************************/
 bool CReadXML::ReadBinary(QDomElement& dElement, int DeviceID)
 {
	 QDomNode dnChild;
	 for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
	 {
		 QDomElement dData = dnChild.toElement();

		 SDiInfo s_DiInfo;
		 s_DiInfo.nDiID = dData.attribute("ID").toInt();
		 s_DiInfo.strDiName = dData.attribute("Name");
		 s_DiInfo.nAddr = dData.attribute("Address").toInt();
		 s_DiInfo.nTransType = dData.attribute("TransType").toInt();

		 if (s_DiInfo.nTransType != 0)
		 {
			 QString strTransInfo = dData.attribute("TransInfo");
			 QByteArray byTransInfo = strTransInfo.toLatin1();

			 // 解析目标和条件表达式
			 if (!AnalyzeExpression(byTransInfo.data(), s_DiInfo.transInfo))
			 {
				 return false;
			 }
		 }

		 m_mapBinary[DeviceID].push_back(s_DiInfo);


	 }

	 return true;
 }

 /*! \fn 
 ********************************************************************************************************* 
 ** \brief  CReadXML::ReadKWH
 ** \details 解析KWH
 ** \param  dElement
 ** \return bool
 ** \author HuangYuqing
 ** \date   2016年8月25日
 ** \note 
 ********************************************************************************************************/
 bool CReadXML::ReadKWH(QDomElement& dElement, int DeviceID)
 {
	 QDomNode dnChild;
	 for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
	 {
		 QDomElement dData = dnChild.toElement();

		 SKwhInfo s_KwhInfo;
		 s_KwhInfo.nKwhID = dData.attribute("ID").toInt();
		 s_KwhInfo.strKwhName = dData.attribute("Name");
		 s_KwhInfo.nAddr = dData.attribute("Address").toInt();

		 m_mapKwh[DeviceID].push_back(s_KwhInfo);


	 }

	 return true;
 }

 QMap<int, SDeviceInfo> CReadXML::GetDeviceInfo()
 {
	 return m_mapDevice;
 }



 bool CReadXML::AnalyzeExpression(const char* source, STransInfo& transInfo)
 {
	 trim(const_cast <char*>(source));
	 int nLen = strlen(source);
	 const char* p = source;
	 if (p[0] != BEGIN_FLAG_LEFT || p[nLen - 1] != END_FLAG_RIGHT)
	 {
		 return false;
	 }

	 char bufTarget[1024] = { 0 };
	 char bufCond[1024] = { 0 };

	 p += 1;

	 int index = FindPosition(p, END_FLAG_RIGHT);
	 if (index == 0)
	 {
		 return false;
	 }

	 strncpy(bufTarget, p, index);

	 p += index + 1;
	 if (strlen(p) == 0)
	 {
		 // 目标解析
		 if (!AnalyzeTarget(bufTarget, transInfo))
		 {
			 return false;
		 }

		 return true;
	 }

	 // 截取条件
	 int nCondLen = strlen(p);
	 if (p[0] != BEGIN_FLAG_LEFT || p[nCondLen - 1] != END_FLAG_RIGHT)
	 {
		 return false;
	 }

	 p += 1;

	 index = FindPosition(p, END_FLAG_RIGHT);
	 if (index == 0)
	 {
		 return false;
	 }

	 strncpy(bufCond, p, index);

	 // 目标解析
	 if (!AnalyzeTarget(bufTarget, transInfo))
	 {
		 return false;
	 }

	 // 条件解析
	 if (!AnalyzeCond(bufCond, transInfo))
	 {
		 return false;
	 }

	 return true;
 }


 bool CReadXML::AnalyzeTarget(const char* src, STransInfo& transInfo)
 {
	 const char* p = src;
	 if (p == NULL)
	 {
		 return false;
	 }

	 int nLen = strlen(src);

	 // 判断是否多目标（&） [1,3,9&1,4,9&1,5,9&1,6,9] 4台PLC照明配置 
	 int cnt = FlagCount(p, MORE_FLAG);
	 if (cnt == 0)
	 {
		 if (!AnalyzeTargetSingle(p, transInfo, nLen))
		 {
			 return false;
		 }
	 }
	 else
	 {
		 char tmp[TRANSINFO_LEN + 1] = { 0 };

		 for (int i = 0; i <= cnt; i++)
		 {
			 int index = FindPosition(p, MORE_FLAG);
			 if (index == 0)
			 {
				 memcpy(tmp, p, src + nLen - p);
			 }
			 else
			 {
				 memcpy(tmp, p, index);
			 }

			 p += index + 1;

			 if (!AnalyzeTargetSingle(tmp, transInfo, strlen(tmp)))
			 {
				 return false;
			 }

			 memset(tmp, 0, TRANSINFO_LEN + 1);
		 }

	 }

	 return true;
 }

 bool CReadXML::AnalyzeTargetSingle(const char* src, STransInfo& transInfo, int nLen)
 {
	 const char* p = src;
	 if (p == NULL)
	 {
		 return false;
	 }

	 if (FlagCount(p, MORE_MSG) % 2 != 0)
	 {
		 return false;
	 }

	 int nStationID = 0;
	 int nDeviceID = 0;
	 int nDI_ID = 0;
	 int nYK_NUM = 0;

	 char tmp[TRANSINFO_LEN + 1] = { 0 };
	 int index = FindPosition(p, MORE_MSG);
	 memcpy(tmp, p, index);

	 nStationID = atoi(tmp);
	 p += index + 1;

	 memset(tmp, 0, TRANSINFO_LEN + 1);
	 index = FindPosition(p, MORE_MSG);
	 memcpy(tmp, p, index);
	 nDeviceID = atoi(tmp);
	 p += index + 1;

	 memset(tmp, 0, TRANSINFO_LEN + 1);
	 memcpy(tmp, p, src + nLen - p);
	 nDI_ID = atoi(tmp);

	 STargetInfo targetInfo;
	 targetInfo.Target_station_id = nStationID;
	 targetInfo.Targert_device_id = nDeviceID;
	 targetInfo.Target_id = nDI_ID;

	 transInfo.s_vecTarget.push_back(targetInfo);

	 return true;

 }

 bool CReadXML::AnalyzeCond(const char* src, STransInfo& transInfo)
 {
	 const char* p = src;
	 if (p == NULL)
	 {
		 return false;
	 }

	 int nLen = strlen(src);

	 int cnt = FlagCount(p, MORE_FLAG);
	 if (cnt == 0)
	 {
		 if (!AnalyzeCondSingle(p, transInfo, nLen))
		 {
			 return false;
		 }
	 }
	 else
	 {
		 char tmp[TRANSINFO_LEN + 1] = { 0 };

		 for (int i = 0; i <= cnt; i++)
		 {
			 int index = FindPosition(p, MORE_FLAG);
			 if (index == 0)
			 {
				 memcpy(tmp, p, src + nLen - p);
			 }
			 else
			 {
				 memcpy(tmp, p, index);
			 }

			 p += index + 1;

			 if (!AnalyzeCondSingle(tmp, transInfo, strlen(tmp)))
			 {
				 return false;
			 }

			 memset(tmp, 0, TRANSINFO_LEN + 1);
		 }

	 }

	 return true;
 }

 bool CReadXML::AnalyzeCondSingle(const char* src, STransInfo& transInfo, int nLen)
 {
	 const char* p = src;
	 if (p == NULL)
	 {
		 return false;
	 }

	 if (FlagCount(p, MORE_MSG) % 2 != 0 || FlagCount(p, STATE_FLAG) <= 0)
	 {
		 return false;
	 }

	 int nStationID = 0;
	 int nChannelID = 0;
	 int nDeviceID = 0;
	 int nDI_ID = 0;
	 int nCond = 0;

	 char tmp[TRANSINFO_LEN + 1] = { 0 };
	 int index = FindPosition(p, MORE_MSG);
	 memcpy(tmp, p, index);
	 nStationID = atoi(tmp);
	 p += index + 1;

	 memset(tmp, 0, TRANSINFO_LEN + 1);
	 index = FindPosition(p, MORE_MSG);
	 memcpy(tmp, p, index);
	 nDeviceID = atoi(tmp);
	 p += index + 1;

	 memset(tmp, 0, TRANSINFO_LEN + 1);
	 index = FindPosition(p, STATE_FLAG);
	 memcpy(tmp, p, index);
	 nDI_ID = atoi(tmp);
	 p += index + 1;

	 memset(tmp, 0, TRANSINFO_LEN + 1);
	 memcpy(tmp, p, src + nLen - p);
	 nCond = atoi(tmp);

	 SConditionInfo condInfo;
	 condInfo.Condition_station_id = nStationID;
	 condInfo.Condition_device_id = nDeviceID;
	 condInfo.Condition_id = nDI_ID;
	 condInfo.Condition_flag = nCond;

	 transInfo.s_vecCondition.push_back(condInfo);

	 return true;
 }

 int CReadXML::FindPosition(const char * src, char flag)
 {
	 if (src == NULL)
	 {
		 return 0;
	 }

	 int len = strlen(src);
	 for (int i = 0; i < len; i++)
	 {
		 if (src[i] == flag)
		 {
			 return i;
		 }
	 }

	 return 0;
 }

 int CReadXML::FlagCount(const char * src, char flag)
 {
	 if (src == NULL)
	 {
		 return 0;
	 }

	 int cnt = 0;

	 int len = strlen(src);
	 for (int i = 0; i < len; i++)
	 {
		 if (src[i] == flag)
		 {
			 cnt++;
		 }
	 }

	 return cnt;
 }

 QMap<int, QVector<SDiInfo> > CReadXML::GetBinaryInfo()
 {
     return m_mapBinary;
 }

 QMap<int, QVector<SAiInfo> > CReadXML::GetAnalogInfo()
 {
	 return m_mapAnalog;
 }
