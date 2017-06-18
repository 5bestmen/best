#include "vmc.h"
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>

typedef int(*vmc_main)(const char*, unsigned int);
typedef int(*vmc_stop)();

vmc::vmc(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_szFilePath = QString::null;
	m_pLineEdit = new QLineEdit;
	ui.comboBox->setLineEdit(m_pLineEdit);
	InitComView();
	QObject::connect(ui.btn_open, SIGNAL(clicked()), this, SLOT(on_BtnGetFilePath()));
	QObject::connect(ui.btn_dataView, SIGNAL(clicked()), this, SLOT(on_BtnDataShow()));
	QObject::connect(ui.btn_stop, SIGNAL(clicked()), this, SLOT(on_BtnStop()));
	QObject::connect(ui.btn_AlarmInfo, SIGNAL(clicked()), this, SLOT(on_BtnAlarmView()));
	QObject::connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ComChannged(int)));

	Qt::WindowFlags flags = Qt::Dialog;

	flags |= Qt::WindowMinimizeButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
}

void vmc::InitComView()
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

bool vmc::IsFileHasAdded(const QString& szName)
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

void vmc::on_btn_start_clicked()
{
	if (m_szFilePath.isNull())
	{
		QMessageBox::information(this, tr("Error"), tr("File Path is Empty"));
		return;
	}

	QString szModuleName = qApp->applicationDirPath();

	szModuleName = QFileInfo(szModuleName).absoluteFilePath();
	
	Q_ASSERT(szModuleName.isEmpty() == false);
	if (szModuleName.isEmpty() == true)
	{
		QMessageBox::information(this, "Error", tr("Bin Path is Empty"));
		return;
	}

	//const QString szModuleName = "vmcs.dll";

	szModuleName += "/";
	szModuleName += "vmcs.dll";

	QLibrary m_LibFes;
	m_LibFes.setFileName(szModuleName);

	if (m_LibFes.load())
	{
		vmc_main  fnMain = (vmc_main)m_LibFes.resolve("StartHMI");
		vmc_stop  fnStop = (vmc_stop)m_LibFes.resolve("StopHMI");
		m_fnScdsvcMain = fnMain;
		m_fnScdsvcStop = fnStop;
		if (m_fnScdsvcMain != nullptr)
		{
			m_fnScdsvcMain(m_szFilePath.toStdString().c_str(), 1);
		}
	}
	else
	{
		QMessageBox::information(this, "Error", m_LibFes.errorString());
	}
}

void vmc::on_BtnGetFilePath()
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

void vmc::on_BtnDataShow()
{

}

void vmc::on_BtnStop()
{
	if (m_fnScdsvcStop != nullptr)
	{
		m_fnScdsvcStop();
	}
}

void vmc::on_BtnAlarmView()
{

}

void vmc::on_ComChannged(int nIndex)
{
	Q_UNUSED(nIndex);
	m_szFilePath = m_pLineEdit->text();
}
