#include "browsescreenwgt.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <memory>

#define GRAPH_PNG ":images/graph.png"

#define CLOSE_GROUP_PNG ":images/close_group.png"

CBrowseScreenWgt::CBrowseScreenWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("WindowBrowse"));
}

CBrowseScreenWgt::~CBrowseScreenWgt()
{
}

void CBrowseScreenWgt::LoadWindowData(const QString &strProjectFileName)
{
	//情况数据
	ui.treeWidget->clear();

	QFile tObjFile(strProjectFileName);

	if (tObjFile.open(QIODevice::ReadOnly| QIODevice::Text))
	{
		std::shared_ptr<QXmlStreamReader> pXml = std::make_shared<QXmlStreamReader>(&tObjFile);

		AnalyseObjectFile(pXml);

		tObjFile.close();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"),tr("Load Object File Error!"));
	}
}

void CBrowseScreenWgt::AnalyseObjectFile(std::shared_ptr<QXmlStreamReader> xmlReader)
{
	while (!xmlReader->atEnd())
	{
		xmlReader->readNext();
		if (xmlReader->isStartElement())
		{
			//起始节点
			if (xmlReader->name().toString() == "graph")
			{
				AnalyseAllNode(xmlReader);
				break;
			}

		}
	}
}

void CBrowseScreenWgt::AnalyseAllNode(std::shared_ptr<QXmlStreamReader> xmlReader)
{
	while (!xmlReader->atEnd())
	{
		xmlReader->readNext();
		if (xmlReader->isStartElement())
		{
			//起始节点
			if (xmlReader->name().toString() == "file")
			{
				QTreeWidgetItem *fileItem = new QTreeWidgetItem;
				fileItem->setIcon(0, QIcon(GRAPH_PNG));

				QString strFilename = xmlReader->attributes().value("name").toString();
				fileItem->setText(0, strFilename);
				fileItem->setData(0, Qt::UserRole + 1, strFilename);

				ui.treeWidget->addTopLevelItem(fileItem);

			}
			else if (xmlReader->name().toString() == "group")
			{
				QTreeWidgetItem *groupItem = new QTreeWidgetItem;
				groupItem->setIcon(0, QIcon(CLOSE_GROUP_PNG));

				QString strGroupname =  xmlReader->attributes().value("name").toString();

				groupItem->setText(0, strGroupname);
				groupItem->setData(0, Qt::UserRole + 1, strGroupname);

				ui.treeWidget->addTopLevelItem(groupItem);

				AnalyseGroupNode(xmlReader,strGroupname,groupItem);
			}

		}

		if (xmlReader->isEndElement())
		{
			if (xmlReader->name().toString() == "graph")
			{
				return;
			}
		}
	}
}

void CBrowseScreenWgt::AnalyseGroupNode(std::shared_ptr<QXmlStreamReader> xmlReader, QString strFatherName, QTreeWidgetItem *pFather)
{
	Q_ASSERT(pFather);
	if (pFather == nullptr)
	{
		return;
	}

	//遍历单个节点
	while (!xmlReader->atEnd())
	{
		xmlReader->readNext();

		if (xmlReader->isStartElement())
		{
			if (xmlReader->name().toString() == "file")
			{
				QTreeWidgetItem *fileItem = new QTreeWidgetItem;
				fileItem->setIcon(0,QIcon(GRAPH_PNG));

				fileItem->setText(0, xmlReader->attributes().value("file").toString());

				QString strFilename = strFatherName + "/" + xmlReader->attributes().value("name").toString();

				fileItem->setText(0, xmlReader->attributes().value("name").toString());
				fileItem->setData(0, Qt::UserRole + 1, strFilename);
				
				pFather->addChild(fileItem);

			}
			else if (xmlReader->name().toString() == "group")
			{
				QTreeWidgetItem *groupItem = new QTreeWidgetItem;
				groupItem->setIcon(0, QIcon(CLOSE_GROUP_PNG));

				QString strGroupname = strFatherName + "/" + xmlReader->attributes().value("name").toString();

				groupItem->setText(0, xmlReader->attributes().value("name").toString());

				groupItem->setData(0, Qt::UserRole + 1, strGroupname);

				pFather->addChild(groupItem);

				AnalyseGroupNode(xmlReader, strGroupname,groupItem);
			}
		}
		else if (xmlReader->isEndElement())
		{
			//结束节点

			if (xmlReader->name().toString() == "group")
			{
				return;
			}
		}
	}

}
