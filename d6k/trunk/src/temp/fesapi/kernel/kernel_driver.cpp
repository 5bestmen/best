
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
** \return bool true:�˳� false:���˳�
** \date 2015��9��17�� 
** \note �����Ƿ�Ҫ�˳�
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
** \date 2015��9��17�� 
** \note �������������������������ں�״̬
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

		//1������ͨ������
		Mail::CreateMailBox(SMKEY_MAIL_CORE + nChannelOccNo);

		//2����������
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
** \date 2015��9��17�� 
** \note ����shell.exe,��shell.exe����dll����
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

	// ��������ʧ��
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
** \date 2015��9��17�� 
** \note ����dll����
********************************************************************************************************/
bool CKernel::LoadDllDriver(int32u nChannelOccNo,const char* strDriverName)
{
	QLibrary lib(strDriverName);

	if(lib.load()) 
	{ 
		//��ȡdll����������ں���
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
** \date 2015��9��17�� 
** \note ��������ͨ�����ں�״̬Ϊ�˳�״̬
********************************************************************************************************/
void CKernel::UnLoadDrivers()
{
	//�������������ں�״̬Ϊ�˳�
	int32u nChannelCount = m_poolChannel.GetArraySize();
	for(int32u i=0;i<nChannelCount;i++)
	{
		CHANNEL* pChannel = m_poolChannel.GetObject(i);
		if(pChannel == NULL)
			continue;

		pChannel->StateCore = DRIVER_STATE_UNLOAD;
	}

	//�ж�������������״̬�Ƿ�Ϊ�˳�
	while(IsDriversQuit()!=K_TRUE)
	{
		Sleep(1);
	}

	//���������˳���ɾ��ͨ������
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
** \date 2015��9��17�� 
** \note �ж������Ƿ��Ѿ�ȫ���˳�
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

