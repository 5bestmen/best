#include "shell.h"

CShell::CShell()
{

}

CShell::~CShell()
{

}

bool CShell::Open() 
{
	if(1)
	{
		//获取参数
		/*if( m_ParaList.size() < 4 )
			return false;
		
		m_strDriverName = m_ParaList[1].toStdString();
		m_szProjName = m_ParaList[2].toStdString();
		m_nNodeOccNo = m_ParaList[3].toInt();
		m_nOccNo = m_ParaList[4].toInt();*/
	}
	else  
	{
		if(0)
		{
		//	m_nChannelID    = 0;
		//	m_strDriverName = "modbus";
		}
		else
		{
	//		m_nChannelID    = 0;
	//		m_strDriverName = "101";
		}
	}

	return true;
}

bool CShell::Run()
{
	//启动驱动
	QLibrary lib(m_strDriverName.c_str());//dll后缀可以不写

	if(!lib.load()) 
	{ 
		std::string strMsg = m_strDriverName;

		strMsg += ":驱动加载失败";

		return false;
	}

	//获取dll驱动的主入口函数
	typedef int (*mydrivermain)(const char* ,int32u,int32u); 

	mydrivermain pdrivermain = (mydrivermain)lib.resolve("StartIoDriver"); 
	if(!pdrivermain) 
		return false;

	pdrivermain(m_szProjName.c_str(),m_nNodeOccNo,m_nOccNo); 

	return true;
}

void CShell::Close()
{

}
