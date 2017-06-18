#include "filetranswgt.h"
#include "commthread.h"
#include <QFileDialog>
#include <QMessageBox>

QString g_FileName = "";


CFileTransWgt::CFileTransWgt(CCommThread *pCommunicate, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_pCommuncate = pCommunicate;

	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_RefreshItems()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_DownLoadItems()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(Slot_upLoadItems()));

	connect(m_pCommuncate,SIGNAL(Signal_FIleCatalogINfo(QList<Catalog_Info>&)),this,SLOT(Slot_upDataITems(QList<Catalog_Info>&)));
}

CFileTransWgt::~CFileTransWgt()
{
}

void CFileTransWgt::Slot_RefreshItems()
{
	ui.treeWidget->clear();

	FILE_CATALOG_REQUEST_1 tCatalogRequest;

	tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
	tCatalogRequest.cot.SetCot(COT_REQ);   //激活

	tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

	tCatalogRequest.m_nOperatorType = 1;    //读目录
	tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

	strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //目录
	tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

	tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
									   //默认时间为所有时间

									   //发送数据


	FILE_ATTR_INFO tReadAction;

	tReadAction.asduAddr.SetAddr(1);    //装置地址
	tReadAction.cot.SetCot(COT_ACT);   //激活

	tReadAction.type = D_FILE_TRANSPORT;   //文件传输

	tReadAction.m_strFileName = "";    //文件名称

	tReadAction.m_nOperatorType = 8;    //召唤数据

	tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

	tReadAction.m_cFileAttr = 0; //文件

	tCatalogRequest.m_fileInfo = tReadAction;

	m_pCommuncate->SendCatalogRequest(tCatalogRequest);
}

void CFileTransWgt::Slot_DownLoadItems()
{
	//
	if (ui.treeWidget->currentItem() == nullptr)
	{
		return;
	}

	if (ui.treeWidget->currentItem()->data(0,Qt::UserRole+1).toInt() == 1)
	{
		//目录
		QMessageBox::warning(0, tr("Warning"), tr("Please Choose File"));
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save File"));

	if (!fileName.isEmpty())
	{
		//
		FILE_CATALOG_REQUEST_1 tCatalogRequest;

		tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
		tCatalogRequest.cot.SetCot(COT_REQ);   //激活

		tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

		tCatalogRequest.m_nOperatorType = 1;    //读目录
		tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

		strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //目录
		tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

		tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
										   //默认时间为所有时间

										   //发送数据


		FILE_ATTR_INFO tReadAction;

		tReadAction.asduAddr.SetAddr(1);    //装置地址
		tReadAction.cot.SetCot(COT_ACT);   //激活

		tReadAction.type = D_FILE_TRANSPORT;   //文件传输

											   //tWriteAction.m_nOperatorType = 7;    //写目录

											   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

		tReadAction.m_strFileName = ui.treeWidget->currentItem()->text(0);    //文件名称

		tReadAction.m_nOperatorType = 1;

		tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

		tReadAction.m_cFileAttr = 0; //文件

		tCatalogRequest.m_fileInfo = tReadAction;

		m_pCommuncate->SendCatalogRequest(tCatalogRequest);

		g_FileName = fileName;
	}
}

void CFileTransWgt::Slot_upLoadItems()
{

	if (ui.treeWidget->currentItem() == nullptr)
	{
		return;
	}

	if (ui.treeWidget->currentItem()->data(0, Qt::UserRole + 1).toInt() != 1)
	{
		//目录
		QMessageBox::warning(0, tr("Warning"), tr("Please Choose Folder"));
		return;
	}

	QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open File"));

	QString tFileOwnName = fileName.split("/").last();

	if (!fileName.isEmpty())
	{
		//
		FILE_CATALOG_REQUEST_1 tCatalogRequest;

		tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
		tCatalogRequest.cot.SetCot(COT_REQ);   //激活

		tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

		tCatalogRequest.m_nOperatorType = 1;    //读目录
		tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

		strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //目录
		tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

		tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
										   //默认时间为所有时间

										   //发送数据


		FILE_ATTR_INFO tReadAction;

		tReadAction.asduAddr.SetAddr(1);    //装置地址
		tReadAction.cot.SetCot(COT_ACT);   //激活

		tReadAction.type = D_FILE_TRANSPORT;   //文件传输

											   //tWriteAction.m_nOperatorType = 7;    //写目录

											   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

		tReadAction.m_strFileName = ui.treeWidget->currentItem()->text(0) + "/" + tFileOwnName;    //文件名称

		tReadAction.m_nOperatorType = 9;    //只召唤目录

		tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

		tReadAction.m_cFileAttr = 0; //文件

		tCatalogRequest.m_fileInfo = tReadAction;

		m_pCommuncate->SendCatalogRequest(tCatalogRequest);
		//Slot_CheckDeviceTable();

		FILE_ATTR_INFO tWriteAction;

		tWriteAction.asduAddr.SetAddr(1);    //装置地址
		tWriteAction.cot.SetCot(COT_ACT);   //激活

		tWriteAction.type = D_FILE_TRANSPORT;   //文件传输

		tWriteAction.m_nOperatorType = 7;    //写目录

		tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

		tWriteAction.m_strFileName = ui.treeWidget->currentItem()->text(0) + "/" + tFileOwnName; ;    //文件名称

		tWriteAction.m_cFileNameLength = tWriteAction.m_strFileName.length();   //目录长度

		QFile tFile(fileName);

		if (!tFile.open(QFile::ReadOnly))
		{
			QMessageBox::warning(this, tr("Warning"), tr("file open file error!"));
			return;
		}

		tWriteAction.m_FileSize = tFile.size();   //文件大小

		tWriteAction.m_btArrayData = tFile.readAll();   //发送数据


		m_pCommuncate->SendWriteActionRequest(tWriteAction);
	}


}

void CFileTransWgt::Slot_upDataITems(QList<Catalog_Info>& lstItems)
{
	//清空树

	for (auto fileInfo : lstItems)
	{
		//遍历所有树 和 子节点

		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setText(0,fileInfo.m_strFileName);
		pItem->setData(0,Qt::UserRole+1,fileInfo.m_cFileAttr);
		ui.treeWidget->addTopLevelItem(pItem);
	}
}
