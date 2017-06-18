#include "testchildlog.h"

#include "Log/Log.h"

#include <Windows.h>

#include <QMessageBox>

testchildlog::testchildlog(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	ui.setupUi(this);
}

testchildlog::~testchildlog()
{
	UnInitialize();
}

bool testchildlog::Initialize()
{
	connect(ui.m_log_btn, SIGNAL(clicked()), this, SLOT(OnLog()));

	if(Log::OpenLog("Hello") == K_FALSE)
	{
		QMessageBox::about(NULL,"Error","Log::OpenLog Hello.log failed!");

		return false;
	}

	return true;
}

void testchildlog::UnInitialize()
{
	Log::CloseLog();
}

void testchildlog::OnLog()
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
		sprintf(buf,"[%d]%s",i,(char*)strText.toStdString().c_str());
		Log::LogMsg((char*)strModule.toStdString().c_str(),buf,nLevel);
		if(i%1000==0)
		{
			::Sleep(1000);
		}
	}
}

