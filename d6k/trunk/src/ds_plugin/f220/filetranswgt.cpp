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

	tCatalogRequest.asduAddr.SetAddr(1);    //װ�õ�ַ
	tCatalogRequest.cot.SetCot(COT_REQ);   //����

	tCatalogRequest.type = D_FILE_TRANSPORT;   //�ļ�����

	tCatalogRequest.m_nOperatorType = 1;    //��Ŀ¼
	tCatalogRequest.m_nCatalogID.SetAddr(0);       //Ŀ¼ID  ��ʱ��Ϊ0

	strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //Ŀ¼
	tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //Ŀ¼����

	tCatalogRequest.m_nCallFlag = 0;   //Ŀ¼�������ļ�
									   //Ĭ��ʱ��Ϊ����ʱ��

									   //��������


	FILE_ATTR_INFO tReadAction;

	tReadAction.asduAddr.SetAddr(1);    //װ�õ�ַ
	tReadAction.cot.SetCot(COT_ACT);   //����

	tReadAction.type = D_FILE_TRANSPORT;   //�ļ�����

	tReadAction.m_strFileName = "";    //�ļ�����

	tReadAction.m_nOperatorType = 8;    //�ٻ�����

	tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //�ļ�����

	tReadAction.m_cFileAttr = 0; //�ļ�

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
		//Ŀ¼
		QMessageBox::warning(0, tr("Warning"), tr("Please Choose File"));
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save File"));

	if (!fileName.isEmpty())
	{
		//
		FILE_CATALOG_REQUEST_1 tCatalogRequest;

		tCatalogRequest.asduAddr.SetAddr(1);    //װ�õ�ַ
		tCatalogRequest.cot.SetCot(COT_REQ);   //����

		tCatalogRequest.type = D_FILE_TRANSPORT;   //�ļ�����

		tCatalogRequest.m_nOperatorType = 1;    //��Ŀ¼
		tCatalogRequest.m_nCatalogID.SetAddr(0);       //Ŀ¼ID  ��ʱ��Ϊ0

		strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //Ŀ¼
		tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //Ŀ¼����

		tCatalogRequest.m_nCallFlag = 0;   //Ŀ¼�������ļ�
										   //Ĭ��ʱ��Ϊ����ʱ��

										   //��������


		FILE_ATTR_INFO tReadAction;

		tReadAction.asduAddr.SetAddr(1);    //װ�õ�ַ
		tReadAction.cot.SetCot(COT_ACT);   //����

		tReadAction.type = D_FILE_TRANSPORT;   //�ļ�����

											   //tWriteAction.m_nOperatorType = 7;    //дĿ¼

											   //tWriteAction.m_nFileID = 0;       //�ļ�ID  ��ʱ��Ϊ0

		tReadAction.m_strFileName = ui.treeWidget->currentItem()->text(0);    //�ļ�����

		tReadAction.m_nOperatorType = 1;

		tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //�ļ�����

		tReadAction.m_cFileAttr = 0; //�ļ�

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
		//Ŀ¼
		QMessageBox::warning(0, tr("Warning"), tr("Please Choose Folder"));
		return;
	}

	QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open File"));

	QString tFileOwnName = fileName.split("/").last();

	if (!fileName.isEmpty())
	{
		//
		FILE_CATALOG_REQUEST_1 tCatalogRequest;

		tCatalogRequest.asduAddr.SetAddr(1);    //װ�õ�ַ
		tCatalogRequest.cot.SetCot(COT_REQ);   //����

		tCatalogRequest.type = D_FILE_TRANSPORT;   //�ļ�����

		tCatalogRequest.m_nOperatorType = 1;    //��Ŀ¼
		tCatalogRequest.m_nCatalogID.SetAddr(0);       //Ŀ¼ID  ��ʱ��Ϊ0

		strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //Ŀ¼
		tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //Ŀ¼����

		tCatalogRequest.m_nCallFlag = 0;   //Ŀ¼�������ļ�
										   //Ĭ��ʱ��Ϊ����ʱ��

										   //��������


		FILE_ATTR_INFO tReadAction;

		tReadAction.asduAddr.SetAddr(1);    //װ�õ�ַ
		tReadAction.cot.SetCot(COT_ACT);   //����

		tReadAction.type = D_FILE_TRANSPORT;   //�ļ�����

											   //tWriteAction.m_nOperatorType = 7;    //дĿ¼

											   //tWriteAction.m_nFileID = 0;       //�ļ�ID  ��ʱ��Ϊ0

		tReadAction.m_strFileName = ui.treeWidget->currentItem()->text(0) + "/" + tFileOwnName;    //�ļ�����

		tReadAction.m_nOperatorType = 9;    //ֻ�ٻ�Ŀ¼

		tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //�ļ�����

		tReadAction.m_cFileAttr = 0; //�ļ�

		tCatalogRequest.m_fileInfo = tReadAction;

		m_pCommuncate->SendCatalogRequest(tCatalogRequest);
		//Slot_CheckDeviceTable();

		FILE_ATTR_INFO tWriteAction;

		tWriteAction.asduAddr.SetAddr(1);    //װ�õ�ַ
		tWriteAction.cot.SetCot(COT_ACT);   //����

		tWriteAction.type = D_FILE_TRANSPORT;   //�ļ�����

		tWriteAction.m_nOperatorType = 7;    //дĿ¼

		tWriteAction.m_nFileID = 0;       //�ļ�ID  ��ʱ��Ϊ0

		tWriteAction.m_strFileName = ui.treeWidget->currentItem()->text(0) + "/" + tFileOwnName; ;    //�ļ�����

		tWriteAction.m_cFileNameLength = tWriteAction.m_strFileName.length();   //Ŀ¼����

		QFile tFile(fileName);

		if (!tFile.open(QFile::ReadOnly))
		{
			QMessageBox::warning(this, tr("Warning"), tr("file open file error!"));
			return;
		}

		tWriteAction.m_FileSize = tFile.size();   //�ļ���С

		tWriteAction.m_btArrayData = tFile.readAll();   //��������


		m_pCommuncate->SendWriteActionRequest(tWriteAction);
	}


}

void CFileTransWgt::Slot_upDataITems(QList<Catalog_Info>& lstItems)
{
	//�����

	for (auto fileInfo : lstItems)
	{
		//���������� �� �ӽڵ�

		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setText(0,fileInfo.m_strFileName);
		pItem->setData(0,Qt::UserRole+1,fileInfo.m_cFileAttr);
		ui.treeWidget->addTopLevelItem(pItem);
	}
}
