
#if _MSC_VER >= 1600
#define ENABLE_VLD
#ifdef ENABLE_VLD
//#include <vld.h> 
#endif
#endif



#include "scadastudio/uimgr.h"
#include "fesmodule.h"
#include "equipmentmodel.h"
#include "reportmodel.h"
#include "usermodule.h"
#include "sysmodule.h"
#include "nodeparamodule.h"
#include "seqscriptmodel.h"
#include "xmlreader.h"
//#include "uimodule/outputmessagerclient.h"
#include "graphmodel.h"
#include "ccore.h"
#include "scadastudio/define.h"

#include "standarview.h"

#include <QtGlobal>
#include <QtWidgets/QApplication>
#include <QLibrary> 
#include <QMessageBox>
#include <QFile>
#include <QDebug>

typedef CUIMgr *(*Fun)();
//
 


void LoadConfig(IMainModuleInterface *pCore, QString file = "")
{
	
}

IMainModuleInterface *s_pGlobleCore = nullptr;

bool LogMsg(char *pszMoudleName, char *szLogTxt, int nLevel)
{
	if(s_pGlobleCore)
	{
		return s_pGlobleCore->LogMsg(pszMoudleName, szLogTxt, nLevel);		
	}
	return false;
}

#include <QDomDocument>
int main(int argc, char *argv[])
{
	QFile file("config.xml");

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return 1;
	}

	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();

	writer.writeStartElement("config");

	writer.writeStartElement("c");
	writer.writeEndElement();

	writer.writeEndElement();

	writer.writeEndDocument();
	
	file.close();

	QApplication a(argc, argv);
	
	s_pGlobleCore = new CCore(NULL);

	//s_pGlobleCore->LoadModule(ModuleID::REPORT);
	
	CCore *pCore = dynamic_cast<CCore*>(s_pGlobleCore);
	Q_ASSERT(pCore);
	if (pCore)
	{
		pCore->LoadInternalModule();
		pCore->LoadModules();
	//	pCore->LoadModule(ModuleID::REPORT);
	
	}

	char s1[20];
	char test[21] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 , 1};
	char s2[20];

	s1[0] = 0;
	s2[0] = 0;
	//char str[]= "测试";

	QString str = "测试t";

	int nima = sizeof(str.toStdString().c_str());
	int ttttttt = strlen(str.toStdString().c_str());

	nima = str.size();
	ttttttt = str.length();
	nima = str.count();

	//auto nSize = str.size();

	//strncpy(pAnalog->m_szTagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, strTagName.size()));

	//auto nn = strlen(str);

	//auto nTest = qMin(20, (int)nn);
	//strncpy(test, str, nTest);

	//int nt = strlen(test);


	// 对字符串进行迭代检查，看看是否有非法字符？


	//COutputMessagerClient client;

	//CFesModel *pFesModel = new CFesModel(pUi);
	//pFesModel->Init(pUi, &reader);
	//pFesModel->GetOut(&client);

	//CEquipmentModel *pEquipmentModel = new CEquipmentModel(pUi);
	//pEquipmentModel->Init(pUi);

	//CReportModel *pReportModel = new CReportModel(pUi);
	//pReportModel->Init(pUi);

	//CUserModel *pUserModel = new CUserModel(pUi);
	//pUserModel->Init(pUi);

	//CSysModel *pSysModel = new CSysModel(pUi);
	//pSysModel->Init(pUi);

	//CNodeParaModel *pNodeParaModel = new CNodeParaModel(pUi);
	//pNodeParaModel->Init(pUi);

	//CSeqscriptModel *pSeqScriptModel = new CSeqscriptModel(pUi);
	//pSeqScriptModel->Init(pUi);

	//CGraphModel *pGraph = new CGraphModel(pCore->GetUiModule());
	//pGraph->Init(pCore->GetUiModule(), &reader);

	s_pGlobleCore->GetUIMgr()->show();

	return a.exec();
}
