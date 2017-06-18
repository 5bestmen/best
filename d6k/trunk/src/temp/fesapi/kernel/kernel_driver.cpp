
#include <QLibrary>
#include "ace/OS_NS_unistd.h"
#include "ace/Process_Manager.h"

#include "mail/mail.h"

#include "kernel.h"


/*! \fn 
********************************************************************************************************* 
** \brief CKernel::IsDriverQuit 
** \details 
** \param nChannelOccNo 
** \return bool true:退出 false:不退出
** \date 2015年9月17日 
** \note 驱动是否要退出
********************************************************************************************************/
bool CKernel::IsDriverQuit(int32u nChannelOccNo)
{
	if(GetChannelCoreState(nChannelOccNo) == DRIVER_STATE_UNLOAD)
		return true;

	return false;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::LoadDrivers 
** \details 
** \return void 
** \date 2015年9月17日 
** \note 加载所有驱动，并设置驱动内核状态
********************************************************************************************************/
bool CKernel::LoadDrivers()
{
	int32u nChannelCount = m_poolChannel.GetArraySize();
	int32u nLoaded = 0;
	for(int32u i=0;i<nChannelCount;i++)
	{
		CHANNEL* pChannel = m_poolChannel.GetObject(i);
		if(pChannel == NULL)
			continue;

		int32u nChannelOccNo = pChannel->OccNo;
		std::string strDriverName = pChannel->DriverName;
		int32u nDriverType = pChannel->DriverType;

		//1、创建通道邮箱
		Mail::CreateMailBox(SMKEY_MAIL_CORE + nChannelOccNo);

		//2、加载驱动
#ifdef DEBUG
		strDriverName = strDriverName + "d";
#endif
		bool IsLoaded = false;
		if(nDriverType == DRIVER_TYPE_EXE) IsLoaded = LoadExeDriver(nChannelOccNo,strDriverName.c_str());
		if(nDriverType == DRIVER_TYPE_DLL) IsLoaded = LoadDllDriver(nChannelOccNo,strDriverName.c_str());
		
		if(IsLoaded)
			nLoaded += 1;

		pChannel->StateCore = IsLoaded ? DRIVER_STATE_LOAD : DRIVER_STATE_UNLOAD;
	}

	if(nLoaded == nChannelCount)
		return true;

	return false;
}


/*! \fn 
********************************************************************************************************* 
** \brief CKernel::LoadExeDriver 
** \details 
** \param nChannelOccNo 
** \param strDriverName 
** \return bool 
** \date 2015年9月17日 
** \note 加载shell.exe,由shell.exe加载dll驱动
********************************************************************************************************/
bool CKernel::LoadExeDriver(int32u nChannelOccNo, const char* strDriverName)
{
	char buf[128];

#ifdef DEBUG
	sprintf(buf, "shelld %d %s", nChannelOccNo, strDriverName);
#else
	sprintf(buf, "shell %d %s", nChannelOccNo, strDriverName);
#endif

	ACE_Process_Options options;
	options.command_line(ACE_TEXT(buf));

	//create sub process
	pid_t pid = ACE_Process_Manager::instance()->spawn(options);

	// 启动进程失败
	if (pid == ACE_INVALID_PID)
		return false;

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::LoadDllDriver 
** \details 
** \param nChannelOccNo 
** \param strDriverName 
** \return bool 
** \date 2015年9月17日 
** \note 加载dll驱动
********************************************************************************************************/
bool CKernel::LoadDllDriver(int32u nChannelOccNo,const char* strDriverName)
{
	QLibrary lib(strDriverName);

	if(lib.load()) 
	{ 
		//获取dll驱动的主入口函数
		typedef int (*mydllentry)(int); 
		mydllentry pdllentry = (mydllentry)lib.resolve("drivermain"); 
		if(pdllentry) 
		{ 
			pdllentry(nChannelOccNo); 
			return true;
		}
	} 

	return false;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::UnLoadDrivers 
** \details 
** \return void 
** \date 2015年9月17日 
** \note 设置所有通道的内核状态为退出状态
********************************************************************************************************/
void CKernel::UnLoadDrivers()
{
	//设置所有驱动内核状态为退出
	int32u nChannelCount = m_poolChannel.GetArraySize();
	for(int32u i=0;i<nChannelCount;i++)
	{
		CHANNEL* pChannel = m_poolChannel.GetObject(i);
		if(pChannel == NULL)
			continue;

		pChannel->StateCore = DRIVER_STATE_UNLOAD;
	}

	//判断所有驱动运行状态是否为退出
	while(IsDriversQuit()!=K_TRUE)
	{
		Sleep(1);
	}

	//所有驱动退出后，删除通道邮箱
	for (int32u i = 0; i < nChannelCount; i++)
	{
		CHANNEL* pChannel = m_poolChannel.GetObject(i);
		if (pChannel == NULL)
			continue;

		Mail::DestroyMailBox(SMKEY_MAIL_CORE + pChannel->OccNo);
	}
}


/*! \fn 
********************************************************************************************************* 
** \brief CKernel::IsDriversQuit 
** \details 
** \return bool 
** \date 2015年9月17日 
** \note 判断驱动是否已经全部退出
********************************************************************************************************/
bool CKernel::IsDriversQuit()
{
	int32u nChannelCount = m_poolChannel.GetArraySize();
	for(int32u i=0;i<nChannelCount;i++)
	{
		CHANNEL* pChannel = m_poolChannel.GetObject(i);
		if(pChannel == NULL)
			continue;

		int32u nChannelOccNo = pChannel->OccNo;

		if (pChannel->StateDriver != DRIVER_STATE_UNLOAD)
			return false;
	}
	return true;
}

