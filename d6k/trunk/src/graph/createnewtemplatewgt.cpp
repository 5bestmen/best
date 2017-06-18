#include <QMenu>
#include <QAction>
#include <QFile>
#include <QXmlStreamReader>
#include <memory>
#include <QMessageBox>
#include <QDebug>
#include <QMessageBox>
#include <QUuid>
#include <QMessageBox>
#include "createnewtemplatewgt.h"

CCreateNewTemplateWgt::CCreateNewTemplateWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	m_pRightClickItem = nullptr;

	ui.splitter->setStretchFactor(0, 3);
	ui.splitter->setStretchFactor(1, 7);

	setWindowTitle(tr("CreateTemplate"));
	//确认
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
	//取消
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
	//树形操作
	ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui.treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(Slot_ShowTreeContextMenu(const QPoint&)));
	//点击树
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int )),this ,SLOT(Slot_ItemClicked(QTreeWidgetItem *, int)));
	ui.treeWidget->setEditTriggers(QAbstractItemView::DoubleClicked);

}

CCreateNewTemplateWgt::~CCreateNewTemplateWgt()
{
}

void CCreateNewTemplateWgt::InitData()
{
	//删除 数据
	ui.treeWidget->clear();
	ui.pushButton->setEnabled(true);
	m_strLibaryName.clear();
	m_strShowName.clear();
	ui.lineEdit->setText("");

	//加载配置文件
	QFile tFile(qApp->applicationDirPath() + TEMPLATE_MANMAGER_FILE);
	if (!tFile.open(QFile::ReadOnly | QFile::Text))
	{
		qDebug() << tFile.errorString();
		return;
	}

	//解析文件
	std::shared_ptr<QXmlStreamReader> pXml = std::make_shared<QXmlStreamReader>(&tFile);

	if (!AnalyseManagerFile(pXml))
	{
		QMessageBox::warning(this, tr("warning"), tr("libary manger file error!"));
	}

	tFile.close();
}

bool CCreateNewTemplateWgt::AnalyseManagerFile(std::shared_ptr<QXmlStreamReader> pXml)
{
	while (!pXml->atEnd())
	{
		pXml->readNext();

		if (pXml->isStartElement())
		{
			if (pXml->name().toString() == "Group")
			{
				//根节点
				QTreeWidgetItem *pGroup = new QTreeWidgetItem;
				QString strGroupName = pXml->attributes().value("name").toString();
				pGroup->setFlags(pGroup->flags() | Qt::ItemIsEditable);

				pGroup->setText(0, strGroupName);
				pGroup->setIcon(0, QIcon(qApp->applicationDirPath() + TEMPLATE_FOLDER_ICON));

				
				pGroup->setData(0, Qt::UserRole + 1, TEMPLATE_GROUP);

				ui.treeWidget->addTopLevelItem(pGroup);
				//
				AnalyseGroupNode(pXml, pGroup);
			}
			else if (pXml->name().toString() == "Item")
			{
				//子节点
				QTreeWidgetItem *pChild = new QTreeWidgetItem;
				pChild->setFlags(pChild->flags() | Qt::ItemIsEditable);
				//显示的名称
				QString strItemName = pXml->attributes().value("name").toString();
				pChild->setText(0, strItemName);

				//文件名称
				QString strFileName = pXml->attributes().value("filename").toString();
				//icon
				QString strIcon = pXml->attributes().value("icon").toString();

				pChild->setData(0, Qt::UserRole + 1, TEMPLATE_ITEM);

				pChild->setData(0, Qt::UserRole + 3, strFileName);
				pChild->setData(0, Qt::UserRole + 4, strIcon);

				ui.treeWidget->addTopLevelItem(pChild);

			}
		}
	}
	return true;
}

void CCreateNewTemplateWgt::AnalyseGroupNode(std::shared_ptr<QXmlStreamReader> pXml, QTreeWidgetItem *pGroupItem)
{
	Q_ASSERT(pGroupItem);
	if (pGroupItem == nullptr)
	{
		return;
	}

	while (!pXml->atEnd())
	{
		pXml->readNext();

		if (pXml->isStartElement())
		{
			if (pXml->name().toString() == "Group")
			{
				//根节点
				QTreeWidgetItem *pGroup = new QTreeWidgetItem;
				pGroup->setFlags(pGroup->flags() | Qt::ItemIsEditable);

				QString strGroupName = pXml->attributes().value("name").toString();

				pGroup->setText(0, strGroupName);
				pGroup->setIcon(0, QIcon(qApp->applicationDirPath() + TEMPLATE_FOLDER_ICON));
				pGroupItem->addChild(pGroup);

				pGroup->setData(0, Qt::UserRole + 1, TEMPLATE_GROUP);

				//
				AnalyseGroupNode(pXml, pGroup);
			}
			else if (pXml->name().toString() == "Item")
			{
				//子节点
				QTreeWidgetItem *pChild = new QTreeWidgetItem;
				pChild->setFlags(pChild->flags() | Qt::ItemIsEditable);
				//显示的名称
				QString strItemName = pXml->attributes().value("name").toString();
				pChild->setText(0, strItemName);

				//文件名称
				QString strFileName = pXml->attributes().value("filename").toString();
				pChild->setData(0, Qt::UserRole + 1, strFileName);

				//文件icon
				QString strFileIcon = pXml->attributes().value("icon").toString();

				pChild->setData(0, Qt::UserRole + 1, TEMPLATE_ITEM);

				pChild->setData(0, Qt::UserRole + 3, strFileName);
				pChild->setData(0, Qt::UserRole + 4, strFileIcon);

				pGroupItem->addChild(pChild);

			}
		}
		else
		{
			if (pXml->isEndElement())
			{
				if (pXml->name().toString() == "Group")
				{
					return;
				}
			}
		}


	}
}

void CCreateNewTemplateWgt::SaveManagerData()
{
	QFile tFile(qApp->applicationDirPath() + TEMPLATE_MANMAGER_FILE);
	if (!tFile.open(QFile::WriteOnly | QFile::Text))
	{
		qDebug() << tFile.errorString();
		return;
	}

	std::shared_ptr<QXmlStreamWriter> pXmlWriter = std::make_shared<QXmlStreamWriter>(&tFile);
	pXmlWriter->setAutoFormatting(true);
	pXmlWriter->writeStartDocument();

	pXmlWriter->writeStartElement("Template");
	//
	for (int i=0; i<ui.treeWidget->topLevelItemCount(); i++)
	{
		SaveGroupNodeData(pXmlWriter,ui.treeWidget->topLevelItem(i));
	}
	//
	pXmlWriter->writeEndElement();

	pXmlWriter->writeEndDocument();

	tFile.close();
	if (tFile.error())
	{
		qDebug() << "Error: Cannot write file: " << tFile.errorString();
	}


}

void CCreateNewTemplateWgt::SaveGroupNodeData(std::shared_ptr<QXmlStreamWriter> pXml, QTreeWidgetItem *pParent)
{
	Q_ASSERT(pParent);
	if (pParent == nullptr)
	{
		return;
	}

	if (pParent->data(0,Qt::UserRole+1) == TEMPLATE_GROUP)
	{
		//group
		pXml->writeStartElement("Group");
		pXml->writeAttribute("name", pParent->text(0));
		for (int i=0; i<pParent->childCount(); i++)
		{
			SaveGroupNodeData(pXml, pParent->child(i));
		}

		pXml->writeEndElement();
	}
	else if (pParent->data(0,Qt::UserRole+1) == TEMPLATE_ITEM)
	{
		//item
		pXml->writeStartElement("Item");

		pXml->writeAttribute("name", pParent->text(0));
		pXml->writeAttribute("filename", pParent->data(0, Qt::UserRole + 3).toString());
		pXml->writeAttribute("icon", pParent->data(0, Qt::UserRole + 4).toString());

		pXml->writeEndElement();
	}
}

void CCreateNewTemplateWgt::Slot_ShowTreeContextMenu(const QPoint & pfClick)
{
	//右击菜单
	m_pRightClickItem = ui.treeWidget->itemAt(pfClick);

	QMenu *pMenu = new QMenu(NULL);

	if (m_pRightClickItem != nullptr)
	{
		QAction *pGroupAct = new QAction(tr("AddGroup"),pMenu);
		pMenu->addAction(pGroupAct);
		connect(pGroupAct,SIGNAL(triggered()),this,SLOT(Slot_AddGroup()));

// 		QAction *pItemAct = new QAction(tr("AddItem"), pMenu);
// 		pMenu->addAction(pItemAct);
// 		connect(pItemAct, SIGNAL(triggered()), this, SLOT(Slot_AddItem()));

		QAction *pDeleteAct = new QAction(tr("Delete"), pMenu);
		pMenu->addAction(pDeleteAct);
		connect(pDeleteAct,SIGNAL(triggered()),this,SLOT(Slot_Delete()));
	}
	else
	{
		QAction *pGroupAct = new QAction(tr("AddGroup"), pMenu);
		pMenu->addAction(pGroupAct);
		connect(pGroupAct, SIGNAL(triggered()), this, SLOT(Slot_AddGroup()));

// 		QAction *pItemAct = new QAction(tr("AddItem"), pMenu);
// 		pMenu->addAction(pItemAct);
// 		connect(pItemAct, SIGNAL(triggered()), this, SLOT(Slot_AddItem()));
	}

	pMenu->exec(QCursor::pos());
	pMenu->deleteLater();
}

void CCreateNewTemplateWgt::Slot_AddGroup()
{
	if (m_pRightClickItem == nullptr)
	{
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);

		pItem->setText(0, tr("Group%1").arg(ui.treeWidget->topLevelItemCount()));
		pItem->setIcon(0, QIcon(qApp->applicationDirPath() + TEMPLATE_FOLDER_ICON));

		pItem->setData(0, Qt::UserRole + 1, TEMPLATE_GROUP);


		ui.treeWidget->addTopLevelItem(pItem);
	}
	else
	{
		QTreeWidgetItem *pParent = m_pRightClickItem->parent();

		if (pParent == nullptr)
		{
			if (m_pRightClickItem->data(0,Qt::UserRole+1).toInt() == TEMPLATE_GROUP)
			{
				QTreeWidgetItem *pItem = new QTreeWidgetItem;
				pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);

				pItem->setText(0, tr("Group%1").arg(m_pRightClickItem->childCount()));
				pItem->setIcon(0, QIcon(qApp->applicationDirPath() + TEMPLATE_FOLDER_ICON));
				m_pRightClickItem->addChild(pItem);

				pItem->setData(0, Qt::UserRole + 1, TEMPLATE_GROUP);

			}
			else if (m_pRightClickItem->data(0, Qt::UserRole + 1).toInt() == TEMPLATE_ITEM)
			{
				QTreeWidgetItem *pItem = new QTreeWidgetItem;
				pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);

				pItem->setText(0, tr("Group%1").arg(ui.treeWidget->topLevelItemCount()));
				pItem->setIcon(0, QIcon(qApp->applicationDirPath() + TEMPLATE_FOLDER_ICON));
				ui.treeWidget->addTopLevelItem(pItem);

				pItem->setData(0, Qt::UserRole + 1, TEMPLATE_GROUP);

			}

		}
		else
		{
			if (m_pRightClickItem->data(0, Qt::UserRole + 1).toInt() == TEMPLATE_GROUP)
			{
				QTreeWidgetItem *pItem = new QTreeWidgetItem;
				pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);

				pItem->setText(0, tr("Group%1").arg(m_pRightClickItem->childCount()));
				pItem->setIcon(0, QIcon(qApp->applicationDirPath() + TEMPLATE_FOLDER_ICON));
				m_pRightClickItem->addChild(pItem);

				pItem->setData(0, Qt::UserRole + 1, TEMPLATE_GROUP);

			}
			else if (m_pRightClickItem->data(0, Qt::UserRole + 1).toInt() == TEMPLATE_ITEM)
			{
				QTreeWidgetItem *pItem = new QTreeWidgetItem;
				pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);

				pItem->setText(0, tr("Group%1").arg(pParent->childCount()));
				pItem->setIcon(0, QIcon(qApp->applicationDirPath() + TEMPLATE_FOLDER_ICON));
				pParent->addChild(pItem);

				pItem->setData(0, Qt::UserRole + 1, TEMPLATE_GROUP);

			}
		}
	}

}

void CCreateNewTemplateWgt::Slot_AddItem()
{
	m_pRightClickItem = ui.treeWidget->currentItem();

	if (m_pRightClickItem == nullptr)
	{
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);

		ui.treeWidget->addTopLevelItem(pItem);
	}
	else
	{
		QTreeWidgetItem *pParent = m_pRightClickItem->parent();

		if (pParent == nullptr)
		{
			QTreeWidgetItem *pItem = new QTreeWidgetItem;
			pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);

			pItem->setText(0, tr("Item"));
			ui.treeWidget->addTopLevelItem(pItem);
		}
		else
		{
			QTreeWidgetItem *pItem = new QTreeWidgetItem;
			pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);

			pItem->setText(0, tr("Item"));
			pParent->addChild(pItem);
		}
	}
}

void CCreateNewTemplateWgt::Slot_Delete()
{
	QTreeWidgetItem *pClickItem = ui.treeWidget->currentItem();
	if (pClickItem != nullptr)
	{
		delete pClickItem;
	}

}

void CCreateNewTemplateWgt::Slot_ItemClicked(QTreeWidgetItem *item, int column)
{
	Q_ASSERT(item);
	Q_UNUSED(column);

	if (item == nullptr)
	{
		return;
	}

	if (item->data(0,Qt::UserRole+1).toInt() == TEMPLATE_ITEM)
	{
		//对象
		ui.lineEdit->setText(item->text(0));
	}
	else
	{
		ui.lineEdit->setText("");
	}
}

void CCreateNewTemplateWgt::Slot_Conform()
{
	//
	QString strNewName = ui.lineEdit->text();

	if (strNewName.remove(" ").isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Library Name Error!"));
		return;
	}

	//确认  更新配置文件
	QTreeWidgetItem *pCurrentItem = ui.treeWidget->currentItem();

	if (pCurrentItem == nullptr)
	{
		//根目录加载
	    for (int i=0; i<ui.treeWidget->topLevelItemCount(); i++)
	    {
			if ((ui.treeWidget->topLevelItem(i)->data(0,Qt::UserRole+1) == TEMPLATE_ITEM)
				&& (ui.treeWidget->topLevelItem(i)->text(0) == ui.lineEdit->text()))
			{
				//对象
				int nResulit = QMessageBox::warning(this,tr("Warning"),tr("%1 file already exists,Replace It?").arg(ui.lineEdit->text()), QMessageBox::Yes | QMessageBox::Cancel);

				if (nResulit == QMessageBox::Cancel)
				{
					return;
				}

				//生成新的文件名
				QUuid tuuid = QUuid::createUuid();
				ui.treeWidget->topLevelItem(i)->setData(0, Qt::UserRole + 3, tuuid.toString() + ".lib");
				m_strLibaryName = tuuid.toString() + ".lib";

				SaveManagerData();
				return;
			}
			
	    }

		QUuid tuuid = QUuid::createUuid();
		QTreeWidgetItem *pTreeItem = new QTreeWidgetItem;
		pTreeItem->setFlags(pTreeItem->flags() | Qt::ItemIsEditable);

		pTreeItem->setText(0,ui.lineEdit->text());

		pTreeItem->setData(0, Qt::UserRole + 1, TEMPLATE_ITEM);
		pTreeItem->setData(0,Qt::UserRole+3,tuuid.toString() + ".lib");

		m_strLibaryName = tuuid.toString() + ".lib";

		ui.treeWidget->addTopLevelItem(pTreeItem);

	}
	else if (pCurrentItem->data(0,Qt::UserRole+1) == TEMPLATE_ITEM)
	{
		//item选项
		if (pCurrentItem->parent() == nullptr)
		{
			for (int i = 0; i < ui.treeWidget->topLevelItemCount(); i++)
			{
				if ((ui.treeWidget->topLevelItem(i)->data(0, Qt::UserRole + 1) == TEMPLATE_ITEM)
					&& (ui.treeWidget->topLevelItem(i)->text(0) == ui.lineEdit->text()))
				{
					//对象
					int nResulit = QMessageBox::warning(this, tr("Warning"), tr("%1 file already exists,Replace It?"), QMessageBox::Yes | QMessageBox::Cancel);

					if (nResulit == QMessageBox::Cancel)
					{
						return;
					}

					//生成新的文件名
					QUuid tuuid = QUuid::createUuid();
					ui.treeWidget->topLevelItem(i)->setData(0, Qt::UserRole + 3, tuuid.toString() + ".lib");

					m_strLibaryName = tuuid.toString() + ".lib";
					SaveManagerData();
					return;
				}

			}

			QUuid tuuid = QUuid::createUuid();
			QTreeWidgetItem *pTreeItem = new QTreeWidgetItem;
			pTreeItem->setFlags(pTreeItem->flags() | Qt::ItemIsEditable);

			pTreeItem->setText(0, ui.lineEdit->text());

			pTreeItem->setData(0, Qt::UserRole + 1, TEMPLATE_ITEM);
			pTreeItem->setData(0, Qt::UserRole + 3, tuuid.toString() + ".lib");
			m_strLibaryName = tuuid.toString() + ".lib";

			ui.treeWidget->addTopLevelItem(pTreeItem);
		}
		else
		{
			for (int i = 0; i < pCurrentItem->parent()->childCount(); i++)
			{
				if ((pCurrentItem->parent()->child(i)->data(0, Qt::UserRole + 1) == TEMPLATE_ITEM)
					&& (pCurrentItem->parent()->child(i)->text(0) == ui.lineEdit->text()))
				{
					//对象
					int nResulit = QMessageBox::warning(this, tr("Warning"), tr("%1 file already exists,Replace It?"), QMessageBox::Yes | QMessageBox::Cancel);

					if (nResulit == QMessageBox::Cancel)
					{
						return;
					}

					//生成新的文件名
					QUuid tuuid = QUuid::createUuid();
					pCurrentItem->parent()->child(i)->setData(0, Qt::UserRole + 3, tuuid.toString() + ".lib");

					m_strLibaryName = tuuid.toString() + ".lib";
					SaveManagerData();
					return;
				}

			}

			QUuid tuuid = QUuid::createUuid();
			QTreeWidgetItem *pTreeItem = new QTreeWidgetItem;
			pTreeItem->setFlags(pTreeItem->flags() | Qt::ItemIsEditable);
			pTreeItem->setText(0, ui.lineEdit->text());

			pTreeItem->setData(0, Qt::UserRole + 1, TEMPLATE_ITEM);
			pTreeItem->setData(0, Qt::UserRole + 3, tuuid.toString() + ".lib");

			m_strLibaryName = tuuid.toString() + ".lib";

			pCurrentItem->parent()->addChild(pTreeItem);

		}

	}
	else if (pCurrentItem->data(0, Qt::UserRole + 1) == TEMPLATE_GROUP)
	{
		//group选项
		for (int i = 0; i < pCurrentItem->childCount(); i++)
		{
			if ((pCurrentItem->child(i)->data(0, Qt::UserRole + 1) == TEMPLATE_ITEM)
				&& (pCurrentItem->child(i)->text(0) == ui.lineEdit->text()))
			{
				//对象
				int nResulit = QMessageBox::warning(this, tr("Warning"), tr("%1 file already exists,Replace It?"), QMessageBox::Yes | QMessageBox::Cancel);

				if (nResulit == QMessageBox::Cancel)
				{
					return;
				}

				//生成新的文件名
				QUuid tuuid = QUuid::createUuid();
				pCurrentItem->child(i)->setData(0, Qt::UserRole + 3, tuuid.toString() + ".lib");

				m_strLibaryName = tuuid.toString() + ".lib";
				SaveManagerData();
				return;
			}

		}

		QUuid tuuid = QUuid::createUuid();
		QTreeWidgetItem *pTreeItem = new QTreeWidgetItem;
		pTreeItem->setFlags(pTreeItem->flags() | Qt::ItemIsEditable);

		pTreeItem->setText(0, ui.lineEdit->text());

		pTreeItem->setData(0, Qt::UserRole + 1, TEMPLATE_ITEM);
		pTreeItem->setData(0, Qt::UserRole + 3, tuuid.toString() + ".lib");

		m_strLibaryName = tuuid.toString() + ".lib";

		pCurrentItem->addChild(pTreeItem);

	}

	SaveManagerData();
 
	ui.pushButton->setEnabled(false);
}
