#include <QFile>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QDateTime>
#include "xmlwriter.h"

CXmlWriter::CXmlWriter( QObject *parent)
	: QObject(parent)
{

}

CXmlWriter::~CXmlWriter()
{
	
}

/*! \fn void CXmlWriter::SetFileInfo(std::shared_ptr <CDeviceMgr> pMgr)
********************************************************************************************************* 
** \brief CXmlWriter::SetFileInfo 
** \details 获取已有管理器信息
** \param pMgr 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlWriter::SetFileInfo(std::shared_ptr <CDeviceMgr> pMgr)
{
	Q_ASSERT(pMgr);
	m_pDevMgr = pMgr;
}

/*! \fn void CXmlWriter::WriteFile(const QString& szName)
********************************************************************************************************* 
** \brief CXmlWriter::WriteFile 
** \details 开始书写配置文件信息
** \param szName 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlWriter::WriteFile(const QString& szName)
{
	QFile  file(szName);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::information(Q_NULLPTR,"error", "createfile error!!");
		return;
	}

	//文件开始写
	emit sig_ProcessInfo(tr("Begin Writing File~~"));
	m_StartTime = GetProessInfo();

	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement("TestConfig");

	
	CDevice *pDev = Q_NULLPTR ;
	for (int i = 0; i < m_pDevMgr->GetDevNum();++i)
	{
		pDev=m_pDevMgr->GetDev(i);
		Q_ASSERT(pDev);
		if (pDev)
		{
			WriteDevInfo(&writer,pDev);
		}
		QString szTime = QString("[%1th] Device ,Time Consuming %2").arg(i).arg((GetProessInfo()-m_StartTime));
		emit sig_ProcessInfo(szTime);
	}
	writer.writeEndDocument();

	file.close();

	if (file.error())
	{
		QMessageBox::information(Q_NULLPTR, "error", file.errorString());
	}

	emit sig_ProcessInfo(tr("End Writing File~~Time:%1").arg(GetProessInfo()-m_StartTime));
}

/*! \fn void CXmlWriter::WriteDevInfo(QXmlStreamWriter* pWriter, CDevice* pDev)
********************************************************************************************************* 
** \brief CXmlWriter::WriteDevInfo 
** \details 写设备信息
** \param pWriter 
** \param pDev 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlWriter::WriteDevInfo(QXmlStreamWriter* pWriter, CDevice* pDev)
{
	Q_ASSERT(pDev);
	Q_ASSERT(pWriter);

	pWriter->writeStartElement("Device");
	pWriter->writeAttribute("DevOccNo",QString::number(pDev->m_OccNo));
	pWriter->writeAttribute("DevName",pDev->m_szDevName);

	AIN* pAin = Q_NULLPTR;
	DIN* pDIn = Q_NULLPTR;
	pWriter->writeStartElement("AIN");
	for (int i = 0; i < pDev->GetAINCount();++i)
	{
		pAin = pDev->GetAin(i);
		if (pAin)
		{
			WriteAinInfo(pWriter, pAin);
		}
	}
	pWriter->writeEndElement();

	pWriter->writeStartElement("DIN");
	for (int j = 0; j < pDev->GetDINCount();++j)
	{
		pDIn = pDev->GetDin(j);
		if (pDIn)
		{
			WriteDinInfo(pWriter, pDIn);
		}
	}
	pWriter->writeEndElement();

	pWriter->writeEndElement();
}

/*! \fn void CXmlWriter::WriteAinInfo(QXmlStreamWriter* pWriter, AIN* pAin)
********************************************************************************************************* 
** \brief CXmlWriter::WriteAinInfo 
** \details 写单点模拟量点信息
** \param pWriter 
** \param pAin 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlWriter::WriteAinInfo(QXmlStreamWriter* pWriter, AIN* pAin)
{
	Q_ASSERT(pWriter && pAin);
	pWriter->writeStartElement("a");

	pWriter->writeAttribute("OccNo", QString::number(pAin->m_OccNo));
	pWriter->writeAttribute("Id", QString::number(pAin->m_Id));
	pWriter->writeAttribute("ChannelOccNo", QString::number(pAin->m_ChannelOccNo));
	pWriter->writeAttribute("DeviceOccNo", QString::number(pAin->m_DeviceOccNo));
	pWriter->writeAttribute("BlockOccNo", QString::number(pAin->m_BlockOccNo));
	pWriter->writeAttribute("Address", QString::number(pAin->m_Address));
	pWriter->writeAttribute("DataSource", QString::number(pAin->m_DataSource));
	pWriter->writeAttribute("InitQua", QString::number(pAin->m_InitVal));

	pWriter->writeAttribute("TransFromType", QString::number(pAin->m_TransFromType));
	pWriter->writeAttribute("AlarmType", QString::number(pAin->m_AlarmType));
	pWriter->writeAttribute("RangeL", QString::number(pAin->m_RangeL));
	pWriter->writeAttribute("RangeH", QString::number(pAin->m_RangeH));
	pWriter->writeAttribute("HighQua", QString::number(pAin->m_HighQua));
	pWriter->writeAttribute("LowQua", QString::number(pAin->m_LowQua));
	pWriter->writeAttribute("PinLanel", QString::number(pAin->m_PinLanel));
	pWriter->writeAttribute("DataType", QString::number(pAin->m_DataType));

	pWriter->writeAttribute("SaveDisk", QString::number(pAin->m_SaveDisk));
	pWriter->writeAttribute("SaveDiskPeriod", QString::number(pAin->m_SaveDiskPeriod));
	pWriter->writeAttribute("SaveDb", QString::number(pAin->m_SaveDb));
	pWriter->writeAttribute("SaveDbPeriod", QString::number(pAin->m_SaveDbPeriod));
	pWriter->writeAttribute("Sensitivity", QString::number(pAin->m_Sensitivity));
	pWriter->writeAttribute("ReferenceCount", QString::number(pAin->m_ReferenceCount));

	pWriter->writeAttribute("szTagName", QString(pAin->m_szTagName));
	pWriter->writeAttribute("szDescription", QString(pAin->m_szDescription));
	pWriter->writeAttribute("szTransFormName", QString(pAin->m_szTransFormName));
	pWriter->writeAttribute("szTransFormDesc", QString(pAin->m_szTransFormDesc));
	pWriter->writeAttribute("szAlarmTagName", QString(pAin->m_szAlarmTagName));
	pWriter->writeAttribute("szAlarmDesc", QString(pAin->m_szAlarmDesc));
	pWriter->writeAttribute("szUnitName", QString(pAin->m_szUnitName));
	pWriter->writeAttribute("szExpress", QString(pAin->m_szExpress));

	pWriter->writeEndElement();
}

/*! \fn void CXmlWriter::WriteDinInfo(QXmlStreamWriter* pWriter, DIN* pDin)
********************************************************************************************************* 
** \brief CXmlWriter::WriteDinInfo 
** \details 写单点开关量信息
** \param pWriter 
** \param pDin 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlWriter::WriteDinInfo(QXmlStreamWriter* pWriter, DIN* pDin)
{
	Q_ASSERT(pWriter && pDin);
	pWriter->writeStartElement("d");

	pWriter->writeAttribute("OccNo", QString::number(pDin->m_OccNo));
	pWriter->writeAttribute("Id", QString::number(pDin->m_Id));
	pWriter->writeAttribute("ChannelOccNo", QString::number(pDin->m_ChannelOccNo));
	pWriter->writeAttribute("DeviceOccNo", QString::number(pDin->m_DeviceOccNo));
	pWriter->writeAttribute("BlockOccNo", QString::number(pDin->m_BlockOccNo));
	pWriter->writeAttribute("Address", QString::number(pDin->m_Address));
	pWriter->writeAttribute("DataSource", QString::number(pDin->m_DataSource));
	pWriter->writeAttribute("InitQua", QString::number(pDin->m_InitVal));
	pWriter->writeAttribute("TransFromType", QString::number(pDin->m_TransFromType));
	pWriter->writeAttribute("AlarmType", QString::number(pDin->m_AlarmType));
	pWriter->writeAttribute("PinLabel", QString::number(pDin->m_PinLabel));
	pWriter->writeAttribute("IsSBO", QString::number(pDin->m_bIsSBO));
	pWriter->writeAttribute("TelCtrlWaitTime", QString::number(pDin->m_TelCtrlWaitTime));
	pWriter->writeAttribute("TelCtrlSendTime", QString::number(pDin->m_TelCtrlSendTime));
	pWriter->writeAttribute("ReferenceCount", QString::number(pDin->m_ReferenceCount));
	pWriter->writeAttribute("szTagName", QString(pDin->m_szTagName));
	pWriter->writeAttribute("szDescription", QString(pDin->m_szDescription));
	pWriter->writeAttribute("szTransFormName", QString(pDin->m_szTransFormName));
	pWriter->writeAttribute("szTransFormDesc", QString(pDin->m_szTransFormDesc));
	pWriter->writeAttribute("szAlarmTagName", QString(pDin->m_szAlarmTagName));
	pWriter->writeAttribute("szAlarmDesc", QString(pDin->m_szAlarmDesc));
	pWriter->writeAttribute("szExpress", QString(pDin->m_szExpress));

	pWriter->writeEndElement();
}

/*! \fn unsigned int  CXmlWriter::GetProessInfo()
********************************************************************************************************* 
** \brief CXmlWriter::GetProessInfo 
** \details 获取本地文件时间信息
** \return unsigned int 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
unsigned int  CXmlWriter::GetProessInfo()
{
	return QDateTime::currentDateTime().toTime_t();
}
