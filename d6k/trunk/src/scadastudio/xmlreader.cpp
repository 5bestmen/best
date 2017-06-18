/*! @file
<PRE>
********************************************************************************
模块名       :  Proj
文件名       :	xmlreader.cpp
文件实现功能 :  Proj
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
*  @date	2015.12.24
*/

#include <QtWidgets>

#include "xmlreader.h"

//! [0]
CXmlReader::CXmlReader()
{

}
//! [0]

//! [1]
bool CXmlReader::read(QIODevice *device)
{
    xml.setDevice(device);

    if (xml.readNextStartElement()) 
	{
		QString strTmp = xml.name().toString();

		QString strTmp1 = xml.attributes().value("version").toString();

		if (xml.name() == "Project" && xml.attributes().value("version") == "1.0")
		{
			readXBEL();
		}
		else
		{
			xml.raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
		}
	}
	else
	{

	}

    return !xml.error();
}
//! [1]

//! [2]
QString CXmlReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}
//! [2]

//! [3]
void CXmlReader::readXBEL()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "Project");

	while(!xml.atEnd())
	{
		xml.readNext();
	
		if(xml.isStartElement())
		{
			QString strTmp = xml.name().toString();

			if (xml.name() == "fes")
			{
				ReadFes(0);
			}
			else if (xml.name() == "equipment")
			{
				ReadEquipment(0);
			}
			else if (xml.name() == "report")
			{
				ReadReport(0);
			}
			else if (xml.name() == "logic")
			{
				xml.skipCurrentElement();
			}
			else if (xml.name() == "user")
			{
				xml.skipCurrentElement();
			}
			else if (xml.name() == "system")
			{
				xml.skipCurrentElement();
			}
			else if (xml.name() == "node")
			{
				xml.skipCurrentElement();
			}
			else if (xml.name() == "recipe")
			{
				xml.skipCurrentElement();
			}
			else if (xml.name() == "web")
			{
				xml.skipCurrentElement();
			}
			else if (xml.name() == "extend")
			{
				xml.skipCurrentElement();
			}
			else if (xml.name() == "graph")
			{
				//readSeparator(0);
				xml.skipCurrentElement();
			}
			else
				xml.skipCurrentElement();
		}
	}
}
//! [3]

//! [4]
void CXmlReader::readTitle(QTreeWidgetItem *item)
{
	Q_UNUSED(item);
	
	Q_ASSERT(xml.isStartElement() && xml.name() == "f");

    //QString title = xml.readElementText();
    //item->setText(0, title);
}
//! [4]

//! [5]
void CXmlReader::ReadReport(QTreeWidgetItem *item)
{
	Q_UNUSED(item);

	Q_ASSERT(xml.isStartElement() && xml.name() == "report");

	while (!xml.atEnd())
	{
		xml.readNext();

		if (xml.isStartElement())
		{
			QString strTmp = xml.attributes().value("file").toString();

			if (xml.name() == "r")
			{
				ReadR();
			}
		}
		else if (xml.isEndElement())
		{
			break;;
		}
	}
}
//! [5]

/*! \fn void CXmlReader::ReadFes(QTreeWidgetItem *item)
********************************************************************************************************* 
** \brief CXmlReader::ReadFes 
** \details 
** \param item 
** \return void 
** \author gw
** \date 2015年12月24日 
** \note 
********************************************************************************************************/
void CXmlReader::ReadFes(QTreeWidgetItem *item)
{
	Q_UNUSED(item);
	
	while (!xml.atEnd())
	{
		xml.readNext();

		if (xml.isStartElement())
		{
			QString strTmp = xml.attributes().value("file").toString();
			
			if (xml.name() == "f")
			{
				ReadF();
			}
		}
		else if (xml.isEndElement())
		{
			break;;
		}
	}
}

/*! \fn void CXmlReader::ReadF()
********************************************************************************************************* 
** \brief CXmlReader::ReadF 
** \details 前置文件
** \return void 
** \author gw
** \date 2015年12月24日 
** \note 
********************************************************************************************************/
void CXmlReader::ReadF()
{
	while (!xml.atEnd())
	{
		if (xml.isStartElement())
		{
			QString strTmp = xml.attributes().value("file").toString();

			m_fesFileVec.append(strTmp);

			xml.readNext();
		}
		else if (xml.isEndElement())
		{
			break;
		}
	}
}

void CXmlReader::ReadEquipment(QTreeWidgetItem *item)
{
	Q_UNUSED(item);
	
	Q_ASSERT(xml.isStartElement() && xml.name() == "equipment");

	while (!xml.atEnd())
	{
		xml.readNext();

		if (xml.isStartElement())
		{
			QString strTmp = xml.attributes().value("file").toString();

			if (xml.name() == "e")
			{
				ReadE();
			}
		}
		else if (xml.isEndElement())
		{
			break;;
		}
	}
}

QTreeWidgetItem *CXmlReader::createChildItem(QTreeWidgetItem *item)
{
	QTreeWidgetItem *childItem;
	if (item) {
		childItem = new QTreeWidgetItem(item);
	}
	else {
		childItem = new QTreeWidgetItem(treeWidget);
	}
	childItem->setData(0, Qt::UserRole, xml.name().toString());
	return childItem;
}

/*! \fn void CXmlReader::ReadE()
********************************************************************************************************* 
** \brief CXmlReader::ReadE 
** \details 对象
** \return void 
** \author gw
** \date 2015年12月24日 
** \note 
********************************************************************************************************/
void CXmlReader::ReadE()
{
	while (!xml.atEnd())
	{
		if (xml.isStartElement())
		{
			QString strTmp = xml.attributes().value("file").toString();

			m_equipmentVec.append(strTmp);

			xml.readNext();
		}
		else if (xml.isEndElement())
		{
			break;
		}
	}
}

/*! \fn void  CXmlReader::ReadR()
********************************************************************************************************* 
** \brief CXmlReader::ReadR 
** \details 报表
** \return void 
** \author gw
** \date 2015年12月24日 
** \note 
********************************************************************************************************/
void  CXmlReader::ReadR()
{
	while (!xml.atEnd())
	{
		if (xml.isStartElement())
		{
			QString strTmp = xml.attributes().value("file").toString();

			m_reportVec.append(strTmp);

			xml.readNext();
		}
		else if (xml.isEndElement())
		{
			break;
		}
	}
}

QVector<QString> &CXmlReader::GetFesVec()
{
	return m_fesFileVec;
}