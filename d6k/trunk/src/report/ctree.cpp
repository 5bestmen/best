#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QTableWidget>
#include <QMessageBox>
#include <QDebug>
#include <QMenu>
#include "ctree.h"
#include "scadastudio/uimgr.h"
#include "scadastudio/icore.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/define.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/imainwindow.h"
#include "reportdefine.h"



CTree::CTree(CTreeWidget* parent, IMainWindow* pUi,IMainModuleInterface* pCore)
{
	Q_ASSERT( parent !=nullptr );
	Q_ASSERT( pUi != nullptr );
	Q_ASSERT( pCore!= nullptr );
	m_pTreeWidget = parent;
	m_pUi = pUi;
	m_pCore = pCore;
	m_pTopItem = NULL;
}


CTree::~CTree()
{
}

/*! \fn void CReportAPI::slot_on_RightMouseClicked(const QPoint&)
*********************************************************************************************************
** \brief CReportAPI::slot_on_RightMouseClicked
** \details 右键菜单栏显示
** \return void
** \author xingzhibing
** \date 2016年4月12日
** \note
********************************************************************************************************/
void CTree::slot_on_RightMouseClicked(const QPoint& point)
{
	bool bRet = false;

	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	if (nType == REPORT_ROOT || nType == REPORT_GROUP_ITEM || nType == REPORT_ITEM)
	{		
		bRet = true;
		ShowMenu(indexSelect);
	}
	emit sig_IfReportClicked(bRet);
}
/*! \fn void CReportAPI::slot_on_DBClicked(const QModelIndex &index)
*********************************************************************************************************
** \brief CReportAPI::slot_on_DBClicked
** \details 双击树控件子菜单响应
** \return void
** \author xingzhibing
** \date 2016年4月12日
** \note
********************************************************************************************************/
void CTree::slot_on_DBClicked(const  QModelIndex &index)
{
	bool bRet = false;

	QModelIndex nIndex = index;

	int nType = nIndex.data(Qt::UserRole).toInt();

	if (nType == REPORT_ROOT || nType == REPORT_GROUP_ITEM)
	{
		bRet = true;
		GetTree()->expanded(nIndex);
	}
	else if (nType == REPORT_ITEM)
	{
		bRet = true;
		emit sig_OpenReportFile(nIndex);
	}	
	emit sig_IfReportClicked(bRet);
}

/*! \fn void CReportAPI::ShowMenu()
*********************************************************************************************************
** \brief CReportAPI::ShowMenu
** \details
** \return void
** \author xingzhibing
** \date 2016年4月12日
** \note
********************************************************************************************************/
void CTree::ShowMenu(QModelIndex &index)
{
	
	int nRet = index.data(Qt::UserRole).toInt();
	if (nRet == REPORT_ROOT)
	{
		ShowRootMenu(index);
	}
	if (nRet == REPORT_GROUP_ITEM)
	{
		ShowGroupMenu(index);
	}
	if (nRet == REPORT_ITEM)
	{
		ShowItemMenu(index);
	}
	
}

/*! \fn void CReportAPI::ShowRootMenu()
*********************************************************************************************************
** \brief CReportAPI::ShowRootMenu
** \details 在日月年 基本报表右击响应
** \return void
** \author xingzhibing
** \date 2016年4月12日
** \note
********************************************************************************************************/
void CTree::ShowRootMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);
	QAction *pAddGroup = new QAction(REPORT_ADD_GROUP, pMenu);
	pAddGroup->setIcon(QIcon(":/images/add_report_group.ico"));
	QAction *pNewReport = new QAction(REPORT_NEW_CREATE, pMenu);
	pNewReport->setIcon(QIcon(":/images/add_report_file.ico"));
	QAction *pImpReport = new QAction(IMPORT_REPORT_ITEM, pMenu);
	pImpReport->setIcon(QIcon(":/images/report_import.ico"));
	pMenu->addAction(pAddGroup);
	pMenu->addAction(pNewReport);
	pMenu->addAction(pImpReport);
	QAction *pSelectAction = pMenu->exec(QCursor::pos());
	if (pSelectAction == pAddGroup)
	{
		AddGroup(index);
	}
	else  if (pSelectAction == pNewReport)
	{
		AddReportFile(index);
	}
	pMenu->deleteLater();
}

/*! \fn void CReportAPI::ShowGroupMenu()
*********************************************************************************************************
** \brief CReportAPI::ShowGroupMenu
** \details 在组菜单下右击响应
** \return void
** \author xingzhibing
** \date 2016年4月12日
** \note
********************************************************************************************************/
void CTree::ShowGroupMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);
	QAction *pAddReport = new QAction(REPORT_NEW_CREATE, pMenu);
	pAddReport->setIcon(QIcon(":/images/add_report_file.ico"));

	QAction* pImport = new QAction(IMPORT_REPORT_ITEM, pMenu);
	pImport->setIcon(QIcon(":/images/report_import.ico"));

	QAction *pAddGroup = new QAction(REPORT_ADD_GROUP, pMenu);
	pAddGroup->setIcon(QIcon(":/images/add_report_group.ico"));

	QAction *pDelGroup = new QAction(REPORT_DELETE_GROUP, pMenu);
	pDelGroup->setIcon(QIcon(":/images/delete_report_group.ico"));

	pMenu->addAction(pAddReport);
	pMenu->addAction(pImport);
	pMenu->addAction(pAddGroup);
	pMenu->addAction(pDelGroup);

	QAction *pSelAction = pMenu->exec(QCursor::pos());
	if (pSelAction)
	{
		if (pSelAction == pAddReport)        //添加报表文件
		{
			AddReportFile(index);
		}
		else if (pSelAction == pDelGroup)   //删除组
		{
			DelGroup(index);
		}
		else if (pSelAction == pAddGroup)
		{
			AddGroup(index);
		}
		else if (pSelAction==pImport)
		{
			ImportReportFile(index);
		}
	}
	pMenu->deleteLater();
}

/*! \fn void CReportAPI::ShowItemMenu()
*********************************************************************************************************
** \brief CReportAPI::ShowItemMenu
** \details
** \return void
** \author xingzhibing
** \date 2016年4月12日
** \note
********************************************************************************************************/
void CTree::ShowItemMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);

	QAction *pDelReport = new QAction(DELETE_REPORT_ITEM, pMenu);
	pDelReport->setIcon(QIcon(":/images/delete_report_file.ico"));

	QAction *pModName = new QAction(MODITY_NAME_ITEM, pMenu);
	pModName->setIcon(QIcon(":/images/report_modify_name.ico"));

	QAction *pModType = new QAction(MODITY_TYPE_ITEM, pMenu);
	pModType->setIcon(QIcon(":/images/report_modify_type.ico"));

	pMenu->addAction(pDelReport);
	pMenu->addAction(pModName);
	pMenu->addAction(pModType);

	QAction *pSelAction = pMenu->exec(QCursor::pos());
	if (pSelAction)
	{
		if (pSelAction == pDelReport)        //删除报表文件
		{
			DelReportFile(index);
		}
		else if (pSelAction == pModName)     //修改报表文件名称
		{
			ModReportName(index);
		}
		else if (pSelAction==pModType)       //修改报表类型
		{
			ModReportType(index);
		}
	}
	pMenu->deleteLater();
}

/*! \fn void CReportAPI::AddGroup(QModelIndex &index)
*********************************************************************************************************
** \brief CReportAPI::AddGroup
** \details 添加报表组
** \param index
** \return void
** \author xingzhibing
** \date 2016年4月12日
** \note
********************************************************************************************************/
void CTree::AddGroup(QModelIndex &index)
{
	emit sig_AddReportGroup(index);
}

/*! \fn void CReportAPI::AddGroupChildItem(CQuoteItem* item, int Role)
*********************************************************************************************************
** \brief CReportAPI::AddGroupChildItem
** \details 固定格式添加组成员
** \param item
** \param Role
** \return void
** \author xingzhibing
** \date 2016年4月13日
** \note
********************************************************************************************************/
void CTree::AddGroupChildItem(QString str, CQuoteItem* item, int Role, QIcon Icon)
{
	Q_ASSERT(item);

	QString strName = str;

	if (item->data(Qt::WhatsThisRole).toInt() < MAX_REPORT_GROUP_COUNT)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(Role, Qt::UserRole);

		newItem->setIcon(Icon);

		item->appendRow(newItem);

	}
}

/*! \fn void CReportAPI::DelGroup(QModelIndex &index)
*********************************************************************************************************
** \brief CReportAPI::DelGroup
** \details 删除分组信息
** \param index
** \return void
** \author xingzhibing
** \date 2016年4月13日
** \note
********************************************************************************************************/
void CTree::DelGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	Q_ASSERT(item);

	int nType = item->data(Qt::UserRole).toInt();
	//组类型判断
	if (nType == REPORT_GROUP_ITEM)
	{
		if (item->hasChildren())//若存成子成员，则需要等子成员删除后再删组级
		{
			QMessageBox::information(0, QString(QStringLiteral("删除错误")), QString(QStringLiteral("文件下有报表文件，请先删除报表文件再继续")));
			return;
		}
		else
		{
			CQuoteItem *pItem = dynamic_cast<CQuoteItem*>(item->parent());
			if (pItem)
			{
				pItem->removeRow(item->row());
			}
		}
	}
}

/*! \fn void CReportAPI::DelReportFile(QModelIndex &index)
*********************************************************************************************************
** \brief CReportAPI::DelReportFile
** \details 删除报表文件信息
** \param index
** \return void
** \author xingzhibing
** \date 2016年4月13日
** \note
********************************************************************************************************/
void CTree::DelReportFile(QModelIndex &index)
{
	emit sig_DelReportFile(index);
}

void CTree::AddReportFile(QModelIndex &index)
{
	emit sig_AddReportFile(index);
}

/*! \fn  void CTree::InitTreeModel()
********************************************************************************************************* 
** \brief CTree::InitTreeModel 
** \details 初始化树控件
** \return void 
** \author xingzhibing
** \date 2016年6月12日 
** \note 
********************************************************************************************************/
void CTree::InitTreeModel()
{
	QObject::connect(GetTree(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slot_on_RightMouseClicked(const QPoint&)));
	QObject::connect(GetTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slot_on_DBClicked(const QModelIndex &)));
}

/*! \fn void CTree::ModReportName(QModelIndex& index)
********************************************************************************************************* 
** \brief CTree::ModReportName 
** \details 修改报表名称
** \param index 
** \return void 
** \author xingzhibing
** \date 2016年7月20日 
** \note 
********************************************************************************************************/
void CTree::ModReportName(QModelIndex& index)
{
	emit sig_ModReportName(index);
}

/*! \fn void CTree::ModReportType(QModelIndex& index)
********************************************************************************************************* 
** \brief CTree::ModReportType 
** \details 修改报表类型
** \param index 
** \return void 
** \author xingzhibing
** \date 2016年7月20日 
** \note 
********************************************************************************************************/
void CTree::ModReportType(QModelIndex& index)
{
	emit sig_ModReportType(index);
}

/*! \fn void CTree::AddChildItem(CTableInfo* info, CQuoteItem* item, int Role, QIcon Icon)
********************************************************************************************************* 
** \brief CTree::AddChildItem 
** \details 
** \param info 
** \param item 
** \param Role 
** \param Icon 
** \return void 
** \author xingzhibing
** \date 2016年7月27日 
** \note 
********************************************************************************************************/
void CTree::AddChildItem(CTableInfo info, CQuoteItem* item, int Role, QIcon Icon)
{
	Q_ASSERT(item);

	QString strName = info.m_szTableName;

	if (item->data(Qt::WhatsThisRole).toInt() <= MAX_REPORT_GROUP_COUNT)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		//Name
		newItem->setData(QVariant(info.m_szTableName), Qt::UserRole + 1);
		//Path
		newItem->setData(QVariant(info.m_szTablePath), Qt::UserRole+2);

		newItem->setData(Role, Qt::UserRole);

		newItem->setIcon(Icon);

		item->appendRow(newItem);
	}
//	m_pCore->SetModifyFlag(REPORT, true);
}

/*! \fn void CTree::ImportReportFile(QModelIndex& index)
********************************************************************************************************* 
** \brief CTree::ImportReportFile 
** \details 导入报表文件
** \param index 
** \return void 
** \author xingzhibing
** \date 2016年8月4日 
** \note 
********************************************************************************************************/
void CTree::ImportReportFile(QModelIndex& index)
{
	Q_UNUSED(index);
	emit sig_ImpReportFile(index);
//	m_pCore->SetModifyFlag(REPORT, true);
}

void CTree::slot_on_TreeItemChanged(QStandardItem* item)
{

}


