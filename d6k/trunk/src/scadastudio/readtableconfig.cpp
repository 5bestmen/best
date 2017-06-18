/*! @file
<PRE>
********************************************************************************
模块名       :  读xml配置文件
文件名       :	readtableconfig.cpp
文件实现功能 :  读xml配置文件
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.17
*/

#include "readtableconfig.h"
#include "scadastudio/icore.h"

#include <QStringList>
#include <QDebug>
#include <QApplication>

#include <QXmlStreamReader> 
#include <QFile>


/*! \fn CReadTableConfig::CReadTableConfig()
********************************************************************************************************* 
** \brief CReadTableConfig::CReadTableConfig 
** \details 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CReadTableConfig::CReadTableConfig()
{
	QString szXmlPath = qApp->applicationDirPath();
	szXmlPath += "/";
  
  	szXmlPath += "config.xml";

	m_pFile = new QFile(szXmlPath);

	if(m_pFile->open(QIODevice::ReadOnly |  QIODevice::Text) == false)
	{
		QString szLog = QObject::tr("Open [ ") + szXmlPath + QObject::tr(" ] failed");
		LogString(szLog.toStdString().c_str(), 1);
	}
	else
	{
		QString szLog = QObject::tr("Open [ ") + szXmlPath + QObject::tr(" ] successed");
		LogString(szLog.toStdString().c_str(), 1);
	}

	m_pXmlRead = new QXmlStreamReader(m_pFile);
}
/*! \fn   CReadTableConfig::~CReadTableConfig 
********************************************************************************************************* 
** \brief CReadTableConfig::~CReadTableConfig 
** \details 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CReadTableConfig::~CReadTableConfig()
{
	if (m_pFile)
	{
		m_pFile->close();
		delete m_pFile;
	}

	m_pFile = nullptr;

	if (m_pXmlRead)
	{
		delete m_pXmlRead;
	}

	m_pXmlRead = nullptr;
}
/*! \fn bool CReadTableConfig::ReadFieldData(QString &strTableName, QVector<CHANNEL> &channelStrVec)
********************************************************************************************************* 
** \brief CReadTableConfig::ReadFieldData 
** \details 读xml配置文件
** \param strTableName 
** \param channelStrVec 
** \return bool 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
bool CReadTableConfig::ReadFieldData(QString &strTableName, QVector<CHANNELSCADASTUDIO> &channelStrVec)
{	
	while (!m_pXmlRead->atEnd())
	{
		m_pXmlRead->readNext();

		if (m_pXmlRead->isStartElement())
		{
			if (m_pXmlRead->name() == "lib")
			{
				QXmlStreamAttributes attributes = m_pXmlRead->attributes();
				
				QString strTableTmp = attributes.value("libname").toString();

				while (!m_pXmlRead->atEnd())
				{
					m_pXmlRead->readNext();
					
					QXmlStreamAttributes attributes = m_pXmlRead->attributes();

					QString ttt = attributes.value("fieldname").toString();
					
					//lib
					if (m_pXmlRead->isStartElement())
					{
						/*if(m_xmlRead->name() == "field")
						{*/
							QXmlStreamAttributes attributes = m_pXmlRead->attributes();

							QString ttt = attributes.value("fieldname").toString();

							while(!m_pXmlRead->atEnd())
							{	
								//field
								if(m_pXmlRead->isStartElement())
								{
									if(m_pXmlRead->name() == "field")
									{
										QXmlStreamAttributes attributes = m_pXmlRead->attributes();

										CHANNELSCADASTUDIO channelStr;

										channelStr.fieldName = attributes.value("fieldname").toString();

										channelStr.fieldAlasName = attributes.value("fieldalias").toString();

										channelStr.fieldType = attributes.value("fieldvoltype").toString();

										channelStr.fieldWidth = attributes.value("fieldwidth").toString().toInt();

										channelStr.fieldFlag= attributes.value("fieldflag").toString().toInt();

										channelStr.keyFlag = attributes.value("fieldiskey").toString().toInt();

										channelStr.searchTable = attributes.value("searchtable").toString();

										channelStr.valueField = attributes.value("valuefield").toString();

										channelStr.showDesc = attributes.value("showdesc").toString();

										channelStr.defaultValue = attributes.value("defaultvalue").toString();

										if(strTableName == strTableTmp)
										{
											channelStrVec.append(channelStr);

											m_arrChannelData.append(channelStr);
										}
										
									}
								}
								else if(m_pXmlRead->isEndElement())
								{	
									//End field
									break;
								}

								m_pXmlRead->readNext();
							}
						/*}*/
					}
					else if(m_pXmlRead->isEndElement())
					{
						//End lib
						break;
					}
				}
			}
		}
	}
	
	return true;

	bool bReturn = false;
	
	while(!m_pXmlRead->atEnd() && !m_pXmlRead->hasError())
	{
		QXmlStreamReader::TokenType token = m_pXmlRead->readNext();

		QStringRef strTmp = m_pXmlRead->name();

		if(token == QXmlStreamReader::StartDocument)
		{
			continue;
		}

		QXmlStreamAttributes attributes = m_pXmlRead->attributes();

		QString ttt = attributes.value("v").toString();

		if(token == QXmlStreamReader::StartElement)
		{
			strTmp = m_pXmlRead->name();

			QXmlStreamAttributes attributes1 = m_pXmlRead->attributes();

			if(m_pXmlRead->name() == "lib")
			{
				QXmlStreamAttributes attributes = m_pXmlRead->attributes();

				QString strTmp = attributes.value("libname").toString();

				if(strTmp.compare(strTableName, Qt::CaseInsensitive) != COMPARE_TRUE)
				{
					//continue;
				}

				while(!m_pXmlRead->atEnd() && !m_pXmlRead->hasError())
				{
					QXmlStreamReader::TokenType token = m_pXmlRead->readNext();

					if(token == QXmlStreamReader::StartElement)
					{
						if(m_pXmlRead->name() == "field")
						{
							QXmlStreamAttributes attributes = m_pXmlRead->attributes();

							CHANNELSCADASTUDIO channelStr;

							channelStr.fieldName = attributes.value("fieldname").toString();

							channelStr.fieldAlasName = attributes.value("fieldalias").toString();

							channelStr.fieldType = attributes.value("fieldvoltype").toString();

							channelStr.fieldWidth = attributes.value("fieldwidth").toString().toInt();

							channelStr.fieldFlag= attributes.value("fieldflag").toString().toInt();

							channelStr.keyFlag = attributes.value("fieldiskey").toString().toInt();

							channelStr.searchTable = attributes.value("searchtable").toString();

							channelStr.valueField = attributes.value("valuefield").toString();

							channelStr.showDesc = attributes.value("showdesc").toString();

							channelStr.defaultValue = attributes.value("defaultvalue").toString();

							channelStrVec.append(channelStr);
						}
					}
				}

				bReturn = true;
			}
		}
		else if (token == QXmlStreamReader::EndElement)
		{
			m_pXmlRead->readNext();
		}
	}

	return bReturn;
}

/*! \fn QStringList CReadTableConfig::getHeaderList()
********************************************************************************************************* 
** \brief CReadTableConfig::getHeaderList 
** \details 
** \return QT_NAMESPACE::QStringList 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
QStringList CReadTableConfig::GetHeaderList()
{
	QStringList list;

	for(int i = 0; i < m_arrChannelData.count(); i++)
	{
		list.append(m_arrChannelData[i].fieldAlasName);
	}

	return list;
}

extern IMainModuleInterface *s_pGlobleCore;

bool  CReadTableConfig::LogString(const char *szLogTxt, int nLevel)
{
// 	Q_ASSERT(m_pCore);
// 	if (m_pCore == nullptr)
// 		return false;
// 
// 	m_pCore->LogMsg("scadastudio", szLogTxt, nLevel);

	Q_ASSERT(s_pGlobleCore);
	if (s_pGlobleCore == nullptr)
		return false;

	s_pGlobleCore->LogMsg("scadastudio", szLogTxt, nLevel);

	return true;
}