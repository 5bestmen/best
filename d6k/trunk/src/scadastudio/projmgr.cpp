#include <QXmlStreamReader>
#include <QFile>
#include <QDomDocument>
#include <QMenubar>
#include <QFiledialog>
#include <QTextstream>
#include <QToolbar>
#include <QStandardItem>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
#include <QFileInfo>
#include <QApplication>

#include "ccore.h"
#include "modulemgr.h"
#include "projmgr.h"
#include "scadastudio/icore.h"
#include "scadastudio/imainwindow.h"
#include "scadastudiodefine.h"
#include "scadastudio/define.h"
#include "extenservicemodulemgr.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"

typedef equipmentmodel* (*ModelTool)();


CProjMgr::CProjMgr(CCore *pCore)
	:m_pCore(NULL), m_pDomDocument(NULL), m_pFile(nullptr), m_strFile("")
{
	Q_ASSERT(pCore);
	if (pCore == nullptr)
	{
		return;
	}

    InitModelTool();
	
	m_pCore = pCore;

	QAction *pAct = new QAction(tr("open"), pCore->GetUIMgr()->menuBar());
	pAct->setIcon(QIcon(OPEN_GROUP_PNG));
	QMenu *pMenu = pCore->GetUIMgr()->menuBar()->addMenu(QObject::tr("&file"));

	QAction *pActSave = new QAction(tr("save"), pCore->GetUIMgr()->menuBar());
	pActSave->setIcon(QIcon(SAVE_PNG));

	QAction *pCreateNewProjectAct = new QAction(tr("new"), pCore->GetUIMgr()->menuBar());
	pCreateNewProjectAct->setIcon(QIcon(NEW_PNG));

	QAction *pCloseProjectAct = new QAction(tr("close"), pCore->GetUIMgr()->menuBar());
	pCloseProjectAct->setIcon(QIcon(CLOSE_PROJECT_PNG));

	pMenu->addAction(pCreateNewProjectAct);
	pMenu->addAction(pAct);
	pMenu->addAction(pActSave);

	m_pSeparatorAct = pMenu->addSeparator();

	//最近打开项目
	for (int i = 0; i < MaxRecentFiles; ++i) 
	{
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect(recentFileActs[i], SIGNAL(triggered()),
			this, SLOT(OpenRecentFile()));
	}
	m_pSeparatorAct = pMenu->addSeparator();
	for (int i = 0; i < MaxRecentFiles; ++i)
		pMenu->addAction(recentFileActs[i]);

	updateRecentFileActions();

	pMenu->addSeparator();

	pMenu->addAction(pCloseProjectAct);

	auto pObjectMenu = pCore->GetUIMgr()->menuBar()->addMenu("tool");
	QAction *pEquipmentModelAct = new QAction(tr("equipment model"), pObjectMenu);
	pObjectMenu->addAction(pEquipmentModelAct);

	auto pTool = pCore->GetUIMgr()->addToolBar("basic");

	pTool->addAction(pCreateNewProjectAct);
	pTool->addAction(pAct);
	pTool->addAction(pActSave);
	pTool->addAction(pCloseProjectAct);

	connect(pCreateNewProjectAct, SIGNAL(triggered()), this, SLOT(CreateNewProj()));
	connect(pAct, SIGNAL(triggered()), this, SLOT(OpenProj()));
	connect(pActSave, SIGNAL(triggered()), this, SLOT(SaveProj()));
	connect(pCloseProjectAct, SIGNAL(triggered()), this, SLOT(CloseProj()));
	connect(pEquipmentModelAct, SIGNAL(triggered()), this, SLOT(EquipmentModel()));
}

CProjMgr::~CProjMgr()
{
	if (m_pDomDocument)
	{
		delete m_pDomDocument;
		m_pDomDocument = NULL;
	}

	if (m_pFile)
	{
		m_pFile->deleteLater();
	}

    FiniModelTool();
}

/*! \fn QDomDocument *CProjMgr::GetDocument()
********************************************************************************************************* 
** \brief CProjMgr::GetDocument 
** \details 
** \return QDomDocument * 
** \author gw
** \date 2016年4月7日 
** \note 
********************************************************************************************************/
QDomDocument *CProjMgr::GetDocument()
{
	if (m_pDomDocument)
	{
		return m_pDomDocument;
	}

	return m_pDomDocument;
}

/*! \fn void CProjMgr::OpenProj()
********************************************************************************************************* 
** \brief CProjMgr::OpenProj 
** \details 打开工程
** \return void 
** \author gw
** \date 2016年6月3日 
** \note 
********************************************************************************************************/
void CProjMgr::OpenProj()
{
	QFileDialog dialog;
	dialog.setOption(QFileDialog::DontUseNativeDialog);
	QStringList list;

	QString strPath;

	if (dialog.exec())
	{
		strPath = dialog.selectedFiles()[0];
	}
	else
	{
		return;
	}

	if (m_pFile)
	{
		delete m_pFile;
		m_pFile = nullptr;
	}

	if (m_pDomDocument)
	{
		delete m_pDomDocument;
		m_pDomDocument = nullptr;
	}

	//QString strPath = "../project/scadastudio/scadastudio.proj";
	
	

	m_pFile = new QFile(strPath);

	if (!m_pFile->open(QFile::ReadWrite))
	{
		m_pCore->LogMsg("Project file path", "debug path wrong", 1);
		
		return;
	}

	if (!m_pDomDocument)
	{
		m_pDomDocument = new QDomDocument("mydocument");
		//QFile file(strPath);
		//if (!file.open(QIODevice::ReadOnly))
		//	return;
		if (!m_pDomDocument->setContent(m_pFile)) 
		{
			m_pFile->close();
			return;
		}
	}


	//pFile->close();
	//pFile->deleteLater();

	if(m_pCore)
	{
		//delete all children of parent;
		//QStandardItem * loopItem = parent; //main loop item
		QList<QStandardItem *> carryItems; //Last In First Out stack of items
		QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
		auto pTop = (QStandardItem *)m_pCore->GetUIMgr()->GetTopItem();
		while (pTop->rowCount())
		{
			itemsToBeDeleted << pTop->takeRow(0);
			//if the row removed has children:
			if (itemsToBeDeleted.at(0)->hasChildren())
			{
				carryItems << pTop;                 //put on the stack the current loopItem
				pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
			}
			//if current loopItem has no more rows but carryItems list is not empty:
			if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

		}

		qDeleteAll(itemsToBeDeleted);

		QString szRootPath = QFileInfo(strPath).absolutePath();
		m_strProjectPath = szRootPath;

		m_root = m_pDomDocument->documentElement();
		//一般模块
		m_pCore->GetModuleMgr()->LoadProjFromFile(m_pDomDocument, &m_root, szRootPath);
		//扩展服务模块
		m_pCore->GetExtendServiceModuleMgr()->LoadProjFromFile(m_pDomDocument, &m_root, szRootPath);
	
		m_pCore->GetUIMgr()->GetLeftTree()->setSortingEnabled(false);
		m_pCore->GetUIMgr()->GetLeftTree()->setSortingEnabled(true);
		//排序
		m_pCore->GetUIMgr()->GetLeftTree()->sortByColumn(0, Qt::AscendingOrder);
	}
}

/*! \fn void CProjMgr::SaveProj()
********************************************************************************************************* 
** \brief CProjMgr::SaveProj 
** \details 保存工程
** \return void 
** \author gw
** \date 2016年6月3日 
** \note 
********************************************************************************************************/
void CProjMgr::SaveProj()
{	
	if (!m_pDomDocument)
	{
		return;
	}
	m_pDomDocument->documentElement();
	m_pCore->GetModuleMgr()->SaveToProjectFile(m_pDomDocument, &m_pDomDocument->documentElement(), m_strProjectPath);

	m_pCore->GetExtendServiceModuleMgr()->SaveToProjectFile(m_pDomDocument, &m_pDomDocument->documentElement(), m_strProjectPath);

	m_pFile->resize(0);
	QTextStream stream(m_pCore->GetProjMgr()->GetFile());
	stream.setDevice(m_pCore->GetProjMgr()->GetFile());
	m_pCore->GetProjMgr()->GetDocument()->save(stream, 4);

	SetCurrentFile(m_strFile);

	//tabwdiget没有窗口
	if (m_pCore->GetUIMgr()->GetTabWidget()->count() == 0)
	{
		return;
	}

	IBase *pBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
	Q_ASSERT(pBase);
	if (!pBase)
	{
		return;
	}
	//暂时保存先用刷新
	pBase->Refresh();

	//m_pCore->GetProjMgr()->GetFile()->close();
}

/*! \fn void CProjMgr::setCurrentFile(const QString &fileName)
********************************************************************************************************* 
** \brief CProjMgr::setCurrentFile 
** \details 保存当前工程文件
** \param fileName 
** \return void 
** \author gw
** \date 2017年2月20日 
** \note 
********************************************************************************************************/
void CProjMgr::SetCurrentFile(const QString &fileName)
{
	QSettings settings("My Organization", "My Application");
	QStringList files = settings.value("recentProjectFileList").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > MaxRecentFiles)
		files.removeLast();

	settings.setValue("recentProjectFileList", files);

	updateRecentFileActions();

}

/*! \fn void CProjMgr::CreateNewProj()
********************************************************************************************************* 
** \brief CProjMgr::CreateNewProj 
** \details 创建新工程
** \return void 
** \author gw
** \date 2016年8月15日 
** \note 
********************************************************************************************************/
void CProjMgr::CreateNewProj()
{
	QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save File"),/*"../project/"*/QString(),
		tr("Project (*.proj)"), 0, QFileDialog::DontUseNativeDialog);

	if (!fileName.isEmpty())
	{	
		auto nIndex = fileName.lastIndexOf("/");
		auto strPath = fileName.mid(0, nIndex);
		auto strProjectName = fileName.mid(nIndex + 1);
		
		nIndex = strProjectName.lastIndexOf(".proj");
		strProjectName = strProjectName.mid(0, nIndex);

		//fileName += "scadastudio/scada/";

		QDir dir;
		auto strProjectPath = strPath + "/" + strProjectName;
		bool exist = dir.exists(strProjectPath);
		
		if (!exist)
		{
			bool ok = dir.mkdir(strPath + "/" + strProjectName);
			if (!ok)
			{
				auto strError = QString(tr("%1 file folder create fail!!")).arg(strPath + "/" + strProjectName);

				m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}
		else
		{
			QFile file(strProjectPath);
			file.remove();
		}

		if (m_pFile)
		{
			delete m_pFile;
			m_pFile = nullptr;
		}

		if (m_pDomDocument)
		{
			delete m_pDomDocument;
			m_pDomDocument = nullptr;
		}

		//QString strPath = "../project/scadastudio/scadastudio.proj";

		auto strFile = strProjectPath + "/" + strProjectName + ".proj";
		m_strFile = strFile;
		m_strProjectPath = strProjectPath;
		m_pDomDocument = new QDomDocument("mydocument");
		
		m_pFile = new QFile(strFile);
		//QFile file(strFile);	 //相对路径、绝对路径、资源路径都可以  
		if (!m_pFile->open(QFile::WriteOnly | QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容  
		{
			return;
		}

		//QDomDocument document("mydocument");

		//if (!m_pDomDocument)
		//{
		//	m_pDomDocument = new QDomDocument("mydocument");
		//	//QFile file(strPath);
		//	//if (!file.open(QIODevice::ReadOnly))
		//	//	return;
		//	m_pDomDocument->setContent(m_pFile);
		//}

		//auto bSuccess = m_pDomDocument->setContent(m_pFile);

		if (m_pCore)
		{
			//delete all children of parent;
			//QStandardItem * loopItem = parent; //main loop item
			QList<QStandardItem *> carryItems; //Last In First Out stack of items
			QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
			auto pTop = (QStandardItem *)m_pCore->GetUIMgr()->GetTopItem();
			while (pTop->rowCount())
			{
				itemsToBeDeleted << pTop->takeRow(0);
				//if the row removed has children:
				if (itemsToBeDeleted.at(0)->hasChildren())
				{
					carryItems << pTop;                 //put on the stack the current loopItem
					pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
				}
				//if current loopItem has no more rows but carryItems list is not empty:
				if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

			}

			qDeleteAll(itemsToBeDeleted);

			//写入xml头部  
			QDomProcessingInstruction instruction; //添加处理命令  
			instruction = m_pDomDocument->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
			m_pDomDocument->appendChild(instruction);

			m_root = m_pDomDocument->createElement("Project");
			m_root.setAttribute("version", "1.0");
			m_pDomDocument->appendChild(m_root);

			m_pCore->GetUIMgr()->GetTopItem();
			m_pCore->GetModuleMgr()->CreateNewProj(m_pDomDocument, &m_root, m_strProjectPath, m_pCore->GetUIMgr()->GetTopItem());

			m_pCore->GetExtendServiceModuleMgr()->CreateNewProj(m_pDomDocument, &m_root, m_strProjectPath, m_pCore->GetUIMgr()->GetTopItem());
			////输出到文件  
			//QTextStream out_stream(m_pFile);
			//m_pDomDocument->save(out_stream, 4); //缩进4格
		}
		
		m_pCore->GetUIMgr()->GetLeftTree()->setSortingEnabled(false);
		m_pCore->GetUIMgr()->GetLeftTree()->setSortingEnabled(true);
		//排序
		m_pCore->GetUIMgr()->GetLeftTree()->sortByColumn(0, Qt::AscendingOrder);
	}
	else
	{
		QMessageBox box;
	}
}

/*! \fn void CProjMgr::CloseProj()
********************************************************************************************************* 
** \brief CProjMgr::CloseProj 
** \details 关闭工程
** \return void 
** \author gw
** \date 2016年8月16日 
** \note 
********************************************************************************************************/
void CProjMgr::CloseProj()
{
	if (!m_pDomDocument)
	{
		m_pCore->LogMsg(SCADASTUDIO_DESC, LOG_STRING("scadastudio project is empty!!!"), LEVEL_1);
		return;
	}
	
	m_pCore->GetModuleMgr()->CloseProj(m_pDomDocument, m_strProjectPath);
}

/*! \fn void CProjMgr::updateRecentFileActions()
********************************************************************************************************* 
** \brief CProjMgr::updateRecentFileActions 
** \details 更新最近打开项目
** \return void 
** \author gw
** \date 2017年2月20日 
** \note 
********************************************************************************************************/
void CProjMgr::updateRecentFileActions()
{
	QSettings settings("My Organization", "My Application");
	QStringList files = settings.value("recentProjectFileList").toStringList();

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) 
	{
		QString text = tr("&%1 %2").arg(i + 1).arg(files[i]);
		recentFileActs[i]->setText(text);
		recentFileActs[i]->setData(files[i]);
		recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentFileActs[j]->setVisible(false);

	m_pSeparatorAct->setVisible(numRecentFiles > 0);
}

/*! \fn void CProjMgr::loadFile(const QString &fileName)
********************************************************************************************************* 
** \brief CProjMgr::loadFile 
** \details 
** \param fileName 
** \return void 
** \author gw
** \date 2017年2月20日 
** \note 
********************************************************************************************************/
void CProjMgr::loadFile(const QString &fileName)
{
	m_strFile = fileName;
	
	//打开工程
	if (m_pFile)
	{
		delete m_pFile;
		m_pFile = nullptr;
	}

	if (m_pDomDocument)
	{
		delete m_pDomDocument;
		m_pDomDocument = nullptr;
	}

	//QString strPath = "../project/scadastudio/scadastudio.proj";
	QApplication::setOverrideCursor(Qt::WaitCursor);


	m_pFile = new QFile(m_strFile);

	if (!m_pFile->open(QFile::ReadWrite))
	{
		m_pCore->LogMsg("Project file path", "debug path wrong", 1);

		QApplication::restoreOverrideCursor();

		return;
	}

	if (!m_pDomDocument)
	{
		m_pDomDocument = new QDomDocument("mydocument");
		//QFile file(strPath);
		//if (!file.open(QIODevice::ReadOnly))
		//	return;
		if (!m_pDomDocument->setContent(m_pFile))
		{
			QApplication::restoreOverrideCursor();
			
			auto strError = QObject::tr("%1 file is empty!!!").arg(m_strFile);
			m_pCore->LogMsg("", strError.toStdString().c_str(), LEVEL_1);

			m_pFile->close();
			return;
		}
	}


	//pFile->close();
	//pFile->deleteLater();

	if (m_pCore)
	{
		//delete all children of parent;
		//QStandardItem * loopItem = parent; //main loop item
		QList<QStandardItem *> carryItems; //Last In First Out stack of items
		QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
		auto pTop = (QStandardItem *)m_pCore->GetUIMgr()->GetTopItem();
		while (pTop->rowCount())
		{
			itemsToBeDeleted << pTop->takeRow(0);
			//if the row removed has children:
			if (itemsToBeDeleted.at(0)->hasChildren())
			{
				carryItems << pTop;                 //put on the stack the current loopItem
				pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
			}
			//if current loopItem has no more rows but carryItems list is not empty:
			if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

		}

		qDeleteAll(itemsToBeDeleted);

		QString szRootPath = QFileInfo(m_strFile).absolutePath();
		m_strProjectPath = szRootPath;

		//一般模块
		m_root = m_pDomDocument->documentElement();
		m_pCore->GetModuleMgr()->LoadProjFromFile(m_pDomDocument, &m_root, szRootPath);
		//扩展服务模块

		m_pCore->GetExtendServiceModuleMgr()->LoadProjFromFile(m_pDomDocument, &m_root, szRootPath);

		m_pCore->GetUIMgr()->GetLeftTree()->setSortingEnabled(false);
		m_pCore->GetUIMgr()->GetLeftTree()->setSortingEnabled(true);
		//排序
		m_pCore->GetUIMgr()->GetLeftTree()->sortByColumn(0, Qt::AscendingOrder);
	}

	QApplication::restoreOverrideCursor();

	SetCurrentFile(fileName);
	//statusBar()->showMessage(tr("File loaded"), 2000);
}

// 初始化模型工具
void CProjMgr::InitModelTool()
{
    m_pModelLib = nullptr;
    m_pModelDll = nullptr;
}

// 释放
void CProjMgr::FiniModelTool()
{
    if (nullptr != m_pModelLib)
    {
        delete m_pModelLib;
        m_pModelLib = nullptr;
    }

    if (nullptr != m_pModelDll)
    {
        delete m_pModelDll;
        m_pModelDll = nullptr;
    }
}

/*! \fn void CProjMgr::EquipmentModel()
********************************************************************************************************* 
** \brief CProjMgr::EquipmentModel 
** \details 装置模板
** \return void 
** \author gw
** \date 2016年9月9日 
** \note 
********************************************************************************************************/
void CProjMgr::EquipmentModel()
{
    if (nullptr != m_pModelDll)
    {
        delete m_pModelDll;
        m_pModelDll = nullptr;
    }

    if (nullptr != m_pModelLib)
    {
        delete m_pModelLib;
        m_pModelLib = nullptr;
    }

#ifdef _DEBUG
    m_pModelLib = new QLibrary("equipmentmodeld");
#else
    m_pModelLib = new QLibrary("equipmentmodel");
#endif

    //判断是否正确加载
    if (!m_pModelLib->load())
    {
        QMessageBox warning(QMessageBox::Warning, "Warning", QStringLiteral("Model加载失败!!!"));
        warning.exec();
        return;
    }

    // 获取模型工具
    ModelTool getModelToolDll = (ModelTool)m_pModelLib->resolve("GetModelToolDll");
    if (getModelToolDll)
    {
        m_pModelDll = getModelToolDll();
        m_pModelDll->show();
    }
}

/*! \fn void CProjMgr::OpenRecentFile()
********************************************************************************************************* 
** \brief CProjMgr::OpenRecentFile 
** \details 
** \return void 
** \author gw
** \date 2017年2月20日 
** \note 
********************************************************************************************************/
void CProjMgr::OpenRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		loadFile(action->data().toString());	
	}
}