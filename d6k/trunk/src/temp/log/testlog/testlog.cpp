#include "testlog.h"

#include "Log/Log.h"

#include <Windows.h>

#include <QProcess>
#include <QMessageBox>


testlog::testlog(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
}

testlog::~testlog()
{
	UnInitialize();
}

bool testlog::Initialize()
{
	connect(ui.m_log_btn, SIGNAL(clicked()), this, SLOT(OnLog()));
	connect(ui.m_exe_btn, SIGNAL(clicked()), this, SLOT(OnExe()));

	if(Log::CreateLog("Hello") == K_FALSE)
	{
		QMessageBox::about(NULL,"Error","Log::CreateLog Hello.log failed!");
		return false;
	}

	return true;
}

void testlog::UnInitialize()
{
	Log::DestroyLog();
}

void testlog::OnLog()
{
	QString strModule = ui.m_log_module->text();
	QString strText = ui.m_log_text->text();
	QString strLevel = ui.m_log_level->text();
	QString strCount = ui.m_log_count->text();

	int nLevel = atoi(strLevel.toStdString().c_str()); 
	int nCount = atoi(strCount.toStdString().c_str()); 

	char buf[512];
	for(int i=0;i<nCount;i++)
	{
		std::string str = strText.toStdString();
		const char* ch = str.c_str();

		sprintf(buf,"%s",ch);

		sprintf(buf,"[%d]%s",i,(char*)strText.toStdString().c_str());
		Log::LogMsg((char*)strModule.toStdString().c_str(),buf,nLevel);
		if(i%1000==0)
		{
			::Sleep(1000);
		}
	}
}

void testlog::OnExe()
{
	QProcess *pProcess = new QProcess(this);
	pProcess->start("testchildlogd");
}
