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

#include <QStringList>
#include "readtableconfig.h"

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
	m_file = new QFile("config.xml");

	if(!m_file->open(QIODevice::ReadOnly |  QIODevice::Text))
	{
		
	}

	m_xmlRead = new QXmlStreamReader(m_file);
}

/*! \fn ** \brief CReadTableConfig::~CReadTableConfig 
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
	m_file->close();
	
	delete m_file;

	m_file = NULL;

	delete m_xmlRead;

	m_xmlRead = NULL;
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
bool CReadTableConfig::ReadFieldData(QString &strTableName, QVector<CHANNEL> &channelStrVec)
{	
	while (!m_xmlRead->atEnd())
	{
		m_xmlRead->readNext();

		if (m_xmlRead->isStartElement())
		{
			if (m_xmlRead->name() == "lib")
			{
				QXmlStreamAttributes attributes = m_xmlRead->attributes();
				
				QString strTableTmp = attributes.value("libname").toString();

				while (!m_xmlRead->atEnd())
				{
					m_xmlRead->readNext();
					
					QXmlStreamAttributes attributes = m_xmlRead->attributes();

					QString ttt = attributes.value("fieldname").toString();
					
					//lib
					if (m_xmlRead->isStartElement())
					{
						/*if(m_xmlRead->name() == "field")
						{*/
							QXmlStreamAttributes attributes = m_xmlRead->attributes();

							QString ttt = attributes.value("fieldname").toString();

							while(!m_xmlRead->atEnd())
							{	
								//field
								if(m_xmlRead->isStartElement())
								{
									if(m_xmlRead->name() == "field")
									{
										QXmlStreamAttributes attributes = m_xmlRead->attributes();

										CHANNEL channelStr;

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

											m_channelDataVec.append(channelStr);
										}
										
									}
								}
								else if(m_xmlRead->isEndElement())
								{	
									//End field
									break;
								}

								m_xmlRead->readNext();
							}
						/*}*/
					}
					else if(m_xmlRead->isEndElement())
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
	
	while(!m_xmlRead->atEnd() && !m_xmlRead->hasError())
	{
		QXmlStreamReader::TokenType token = m_xmlRead->readNext();

		QStringRef strTmp = m_xmlRead->name();

		if(token == QXmlStreamReader::StartDocument)
		{
			continue;
		}

		QXmlStreamAttributes attributes = m_xmlRead->attributes();

		QString ttt = attributes.value("v").toString();

		if(token == QXmlStreamReader::StartElement)
		{
			strTmp = m_xmlRead->name();

			QXmlStreamAttributes attributes1 = m_xmlRead->attributes();

			if(m_xmlRead->name() == "lib")
			{
				QXmlStreamAttributes attributes = m_xmlRead->attributes();

				QString strTmp = attributes.value("libname").toString();

				if(strTmp.compare(strTableName, Qt::CaseInsensitive) != COMPARE_TRUE)
				{
					//continue;
				}

				while(!m_xmlRead->atEnd() && !m_xmlRead->hasError())
				{
					QXmlStreamReader::TokenType token = m_xmlRead->readNext();

					if(token == QXmlStreamReader::StartElement)
					{
						if(m_xmlRead->name() == "field")
						{
							QXmlStreamAttributes attributes = m_xmlRead->attributes();

							CHANNEL channelStr;

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
			m_xmlRead->readNext();
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
QStringList CReadTableConfig::getHeaderList()
{
	QStringList list;

	for(int i = 0; i < m_channelDataVec.count(); i++)
	{
		list.append(m_channelDataVec[i].fieldAlasName);
	}

	return list;
}