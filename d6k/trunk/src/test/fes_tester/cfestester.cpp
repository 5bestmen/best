#include "cfestester.h"
#include "fesapi.h"
#include "mail/mail.h"
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QHostInfo>
#include <memory>


typedef int(*scada_main)(const char*, unsigned int);
typedef int(*scada_stop)();
typedef int(*scada_alarm)(QString);

CFesTester::CFesTester(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_szFilePath = QString::null;
	m_pLineEdit = new QLineEdit;
	m_nMyNode = INVALID_OCCNO;
	ui.comboBox->setLineEdit(m_pLineEdit);
	InitProjView();
	QObject::connect(ui.m_RadioAo, &QRadioButton::clicked, this, &CFesTester::slot_OnRadioChanged);
	QObject::connect(ui.m_RadioDo, &QRadioButton::clicked, this, &CFesTester::slot_OnRadioChanged);
	QObject::connect(ui.m_RadioMsg, &QRadioButton::clicked, this, &CFesTester::slot_OnRadioChanged);

	QObject::connect(ui.btn_open, &QPushButton::clicked, this, &CFesTester::slot_OnPathSelection);
	QObject::connect(ui.m_BtnLoad,&QPushButton::clicked,this,&CFesTester::slot_OnLoad);
	QObject::connect(ui.m_BtnUnLoad, &QPushButton::clicked, this, &CFesTester::slot_OnUnLoad);

	QObject::connect(ui.m_ComChannel,SIGNAL(currentIndexChanged(int )),this,SLOT(slot_OnDOComBoxChannelChanged(int )));
	QObject::connect(ui.m_ComDOut, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_OnDOInfoComBoxChanged(int)));
	QObject::connect(ui.m_AoChannelComBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_OnAOComBoxChannelChanged(int)));
	QObject::connect(ui.m_AoComBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_OnAOInfoComBoxChanged(int)));
	QObject::connect(ui.m_MsgComBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_OnMsgComBoxChannelChanded(int)));

	QObject::connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ComChannged(int)));

	QObject::connect(ui.m_OK, &QPushButton::clicked, this, &CFesTester::slot_OnOk);
	QObject::connect(ui.m_Cancel, &QPushButton::clicked, this, &CFesTester::slot_OnCancel);

	m_nRadioIndex = -1;

	OpenPostOffice("FES");
	int nMailID;
	OpenMailBox("FES", "fes_tester", &nMailID);
}

void CFesTester::InitProjView()
{
	QFile file("fsh.xml");

	if (!file.open(QIODevice::ReadOnly | QFile::Text))
	{
		return;
	}
	QDomDocument document;
	if (!document.setContent(&file))
	{
		file.close();
		return;
	}
	QDomElement nRoot = document.documentElement();
	if (nRoot.tagName() != "config")
	{
		return;
	}

	m_nMaxRecentFileNum = nRoot.attribute("maxnum").toInt();

	QDomNode nNode = nRoot.firstChild();
	std::shared_ptr<FileInfo> pInfo;
	for (; !nNode.isNull(); nNode = nNode.nextSibling())
	{
		QDomElement pData = nNode.toElement();

		QString szName = pData.tagName();

		if (szName == "fsh")
		{
			pInfo = std::make_shared<FileInfo>();
			pInfo->m_szName = pData.attribute("name");
			pInfo->m_szPath = pData.attribute("path");
			m_FileInfoQue.push_back(pInfo);
		}
	}
	file.close();

	//加载所有数据之后 界面进行初始化
	for (auto i : m_FileInfoQue)
	{
		if (i->m_szName.isEmpty() == false)
		{
			ui.comboBox->addItem(i->m_szPath);
		}
	}

	ui.comboBox->setCurrentIndex(-1);
}

bool CFesTester::IsFileHasAdded(const QString& szName)
{
	for (auto i : m_FileInfoQue)
	{
		if (i->m_szPath == szName)
		{
			return true;
		}
	}
	return false;
}

void CFesTester::slot_OnPathSelection()
{
	QString szName = QFileDialog::getOpenFileName(this, tr("Open Config File"), ".", "Project File(*.proj)");
	if (szName.isEmpty())
	{
		//TODO Log File Not Exist
		return;
	}
	m_szFilePath = szName;
	m_pLineEdit->setText(m_szFilePath);

	//文件中添加
	QString szName1 = QFileInfo(m_szFilePath).fileName();

	bool bRet = IsFileHasAdded(szName);

	//文件未添加
	if (!bRet)
	{
		//1、文件数目达到最大数目值
		if (m_FileInfoQue.size() >= m_nMaxRecentFileNum)
		{
			while (m_FileInfoQue.size() >= m_nMaxRecentFileNum && m_FileInfoQue.empty() == false)
			{
				m_FileInfoQue.pop_back();
			}
		}
		std::shared_ptr<FileInfo> pFileInfo = std::make_shared<FileInfo>();
		pFileInfo->m_szPath = szName;
		pFileInfo->m_szName = QFileInfo(szName).fileName();
		m_FileInfoQue.push_front(pFileInfo);
	}

	//写进文件中
	QFile  file("fsh.xml");

	if (!file.open(QIODevice::WriteOnly | QFile::Text))
	{
		return;
	}
	QDomDocument doc;
	QDomProcessingInstruction instruction;
	instruction = doc.createProcessingInstruction("xml", QString("version=\"1.0\" encoding=\"UTF-8\""));

	QDomElement root = doc.createElement("config");

	QDomAttr nMaxNum = doc.createAttribute("maxnum");
	root.setAttribute("maxnum", 10);

	doc.appendChild(root);

	for (auto i : m_FileInfoQue)
	{
		QDomElement nFshEle = doc.createElement("fsh");
		QDomAttr szName = doc.createAttribute("name");
		QDomAttr szPath = doc.createAttribute("path");

		nFshEle.setAttribute("name", i->m_szName);
		nFshEle.setAttribute("path", i->m_szPath);

		root.appendChild(nFshEle);
	}
	QTextStream out(&file);
	doc.save(out, 4);

	file.close();
}

void CFesTester::slot_OnLoad()
{
	if (m_szFilePath.isNull())
	{
		QMessageBox::information(this, tr("Error"), tr("File Path is Empty"));
		return;
	}

	bool bRet=OpenIO(m_szFilePath.toStdString().c_str(),0,0);

	if (!bRet)
	{
		QMessageBox::information(this, "fes_db_error", "open fes db error");
		return;
	}
	//可以执行下一步初始化

	QString nName = QHostInfo::localHostName();

	GetMyNodeOccNoByHostName(nName.toStdString().c_str(), &m_nMyNode);

	Q_ASSERT(m_nMyNode!=INVALID_OCCNO && m_nMyNode <= MAX_NODE_OCCNO);

	ui.m_BtnLoad->setChecked(false);
}

void CFesTester::slot_OnUnLoad()
{
	CloseIO(0);
}

void CFesTester::slot_OnRadioChanged()
{
	if (ui.m_RadioAo->isChecked())
	{
		m_nRadioIndex = AO_TYPE;
		InitAOChannelFunc();
		SetAoFuncEnable(true);
		SetDoFuncEnable(false);
		SetMsgFuncEnable(false);
	}
	else if (ui.m_RadioDo->isChecked())
	{
		m_nRadioIndex = DO_TYPE;
		InitDOChannelFunc();
		SetAoFuncEnable(false);
		SetDoFuncEnable(true);
		SetMsgFuncEnable(false);
	}
	else if (ui.m_RadioMsg->isChecked())
	{
		m_nRadioIndex = MSGC_TYPE;
		InitMsgChannelFunc();
		SetAoFuncEnable(false);
		SetDoFuncEnable(false);
		SetMsgFuncEnable(true);
	}
	else
	{
		m_nRadioIndex = -1;
	}
}

void CFesTester::slot_OnPathComBoxChanged(int nIndex)
{
	Q_UNUSED(nIndex);
	m_szFilePath = m_pLineEdit->text();
}

void CFesTester::slot_OnAOComBoxChannelChanged(int  nIndex)
{
	if (nIndex<0)
	{
		return;
	}
	int32u nChannelID = ui.m_AoChannelComBox->currentText().toInt();
	Q_ASSERT(nChannelID != INVALID_OCCNO && nChannelID <= MAX_CHANNEL_OCCNO);
	ui.m_AoComBox->clear();
	InitAoInfo(nChannelID);
}

void CFesTester::slot_OnAOInfoComBoxChanged(int nIndex)
{
	if (nIndex < 0)
	{
		return;
	}

	

}

void CFesTester::slot_OnDOComBoxChannelChanged(int nIndex)
{
	if (nIndex<0)
	{
		return;
	}
	int32u nChannelID=ui.m_ComChannel->currentText().toInt();
	Q_ASSERT(nChannelID !=INVALID_OCCNO && nChannelID <=MAX_CHANNEL_OCCNO);
	ui.m_ComDOut->clear();
	InitDoInfo(nChannelID);
}

void CFesTester::slot_OnDOInfoComBoxChanged(int nIndex)
{

}

void CFesTester::slot_OnMsgComBoxChannelChanded(int nIndex)
{

}

void CFesTester::slot_OnOk()
{
	switch (m_nRadioIndex)
	{
	case AO_TYPE:
	{
		int32u nAoOccNo = ui.m_AoComBox->currentText().toInt();
		if (!ui.lineEdit_3->text().isEmpty())
		{
			fp64 nVal = ui.lineEdit_3->text().toFloat();
			AppSetAoutValue(nAoOccNo, nVal, 0);
		}
		break;
	}		
	case DO_TYPE:
	{
		int32u nDoOccNo = ui.m_ComDOut->currentText().toInt();
		int32u nOperIndex = ui.m_ComDoOper->currentIndex();
		if (nOperIndex!=0)
		{
			AppSetDoutValue(nDoOccNo, nOperIndex, nOperIndex);
		}
		break;
	}	
	case MSGC_TYPE:
	{
		int32u nChannelOccNo = ui.m_MsgComBox->currentText().toInt();
		if (!ui.lineEdit_2->text().isEmpty())
		{
			QString str = ui.lineEdit_2->text();
			IoDiagAlarm(nChannelOccNo, 1, str.toStdString().c_str(),nullptr);
		}
		break;
	}		
	default:
		break;
	}
}

void CFesTester::slot_OnCancel()
{

}

void CFesTester::SetAoFuncEnable(bool bEnable)
{
	if (bEnable)
	{
		ui.groupBox_3->setStyleSheet(" color: green ");
	}
	else
	{
		ui.groupBox_3->setStyleSheet(" color: gray ");
	}
	ui.lineEdit_3->setEnabled(bEnable);
	ui.m_AoChannelComBox->setEnabled(bEnable);
	ui.m_AoComBox->setEnabled(bEnable);
}

void CFesTester::SetDoFuncEnable(bool bEnable)
{
	if (bEnable)
	{
		ui.groupBox_2->setStyleSheet(" color: green ");
	}
	else
	{
		ui.groupBox_2->setStyleSheet(" color: gray ");
	}
	ui.m_ComChannel->setEnabled(bEnable);
	ui.m_ComDOut->setEnabled(bEnable);
	ui.m_ComDoOper->setEnabled(bEnable);
}

void CFesTester::SetMsgFuncEnable(bool bEnable)
{
	if (bEnable)
	{
		ui.groupBox_4->setStyleSheet(" color: green ");
	}
	else
	{
		ui.groupBox_4->setStyleSheet(" color: gray ");
	}
	
	ui.m_MsgComBox->setEnabled(bEnable);
	ui.lineEdit_2->setEnabled(bEnable);
}

void CFesTester::InitAOChannelFunc()
{
	int32u nChannelCount;
	GetChannelCount(m_nMyNode, &nChannelCount);
	for (int i = 1; i <= nChannelCount; ++i)
	{
		ui.m_AoChannelComBox->addItem(QString::number(i));
	}
}

void CFesTester::InitDOChannelFunc()
{
	int32u nChannelCount;
	GetChannelCount(m_nMyNode, &nChannelCount);

	for (int i = 1; i <= nChannelCount;++i)
	{
		ui.m_ComChannel->addItem(QString::number(i));
	}

}

void CFesTester::InitMsgChannelFunc()
{
	int32u nChannelCount;
	GetChannelCount(m_nMyNode, &nChannelCount);
	for (int i = 1; i <= nChannelCount; ++i)
	{
		ui.m_MsgComBox->addItem(QString::number(i));
	}
}

void CFesTester::InitDoInfo(int32u nChannelID)
{
	int32u  nDoCount;
	GetDoutCountInChannel(nChannelID, &nDoCount);
	if (nDoCount<=0)
	{
		return;
	}
	std::unique_ptr <int32u[]> pOccNos(new int32u[nDoCount]());
	bool bRet = GetDoutOccNosInChannel(nChannelID, pOccNos.get(), nDoCount);
	Q_ASSERT(bRet);
	for (int i = 0; i < nDoCount;++i)
	{
		ui.m_ComDOut->addItem(QString::number(pOccNos[i]));
	}
}

void CFesTester::InitAoInfo(int32u nChannelID)
{
	int32u  nAoCount;
	GetAoutCountInChannel(nChannelID, &nAoCount);
	if (nAoCount <= 0)
	{
		return;
	}
	std::unique_ptr <int32u[]> pOccNos(new int32u[nAoCount]());
	bool bRet = GetAoutOccNosInChannel(nChannelID, pOccNos.get(), nAoCount);
	Q_ASSERT(bRet);
	for (int i = 0; i < nAoCount; ++i)
	{
		ui.m_AoComBox->addItem(QString::number(pOccNos[i]));
	}
}

void CFesTester::InitMsgInfo(int32u nChannelID)
{
	int32u nChannelCount;
	GetChannelCount(m_nMyNode, &nChannelCount);

	for (int i = 1; i <= nChannelCount; ++i)
	{
		ui.m_MsgComBox->addItem(QString::number(i));
	}

}
