/*! @file mem_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  mem_db.cpp
文件实现功能 :  内存数据库基类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存数据库基类
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.2
*******************************************************************************/
#include "mem_db.h"

#include "fesapi/fesdatadef.h"

#include "log/log.h"
#include <QObject> 
#include <QString> 
#include <QDebug>


CMemDB::CMemDB() :m_bStopFlag(false)
{ 
	 
}

CMemDB::~CMemDB(void)
{ 

}

void CMemDB::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("SCADA", szLogTxt, nLevel, nullptr);
}

/*! \fn bool CMemDB::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CMemDB::Initialize 
** \details 初始化  加载proj文件，分别接线各个配置文件
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CMemDB::Initialize(const char *pszDataPath, unsigned int nMode)
{ 
	QString szLog;
	if (pszDataPath && strlen((pszDataPath)))
	{
		szLog = QString(QObject::tr("Start project [%1]...")).arg(pszDataPath);
	}
	else
	{
		szLog = QObject::tr("Start project...");
	}
	
	LogMsg(szLog.toStdString().c_str(), 0);

	return true;
}

void CMemDB::Run()
{

}

void CMemDB::Shutdown()
{


}

void CMemDB::SetTaskStopFlag()
{
	std::unique_lock<std::mutex> lk(m_mutThread);
	m_bStopFlag = true;
	m_cvThread.notify_one();
}

void CMemDB::StartModule()
{
	if (m_pMainTask != nullptr)
	{
		m_pMainTask.reset(new (std::thread)(&CMemDB::MainTask, this, nullptr));
		//m_pMainTask = std::make_shared<std::thread>(*m_p103Impl);
	}
	else
	{
		m_pMainTask = std::make_shared<std::thread>(&CMemDB::MainTask, this, nullptr);
	}
}

void CMemDB::StopModule()
{
	SetTaskStopFlag();
	// 退出线程
	Q_ASSERT(m_pMainTask != nullptr);
	if (m_pMainTask != nullptr && m_pMainTask->joinable())
	{
		m_pMainTask->join();
	}
}

void CMemDB::MainTask(void *pImpl)
{
//	LogString("start main task....", 2);
	qDebug() << "start main task....";
	while (true)
	{
		std::unique_lock<std::mutex> lk(m_mutThread);

		auto bRet = m_cvThread.wait_for(lk, std::chrono::seconds(1), std::bind(&CMemDB::IsStopped, this));
		if (m_bStopFlag)
		{
			qDebug() << "exit main task....";
			//			LogString("exit main task1....", 2);
			return;
		}
 
	//	MainLoop();
	}
} 
/*! \fn size_t  CMemDB::EstimateMemSize()
********************************************************************************************************* 
** \brief CMemDB::EstimateMemSize 
** \details 加载配置文件后，预估所需的共享内存的尺寸是多少，继承的类中必须要实现
** \return size_t 
** \author LiJin 
** \date 2016年11月2日 
** \note 
********************************************************************************************************/
size_t  CMemDB::EstimateMemSize()
{
	Q_ASSERT(false);
	return 0;
}
/*! \fn bool CMemDB::GetAinValue(int32u nOccNo, CVariantEx & val, int8u &nQuality)
********************************************************************************************************* 
** \brief CMemDB::GetAinValue 
** \details 应用层获取模拟量测值等信息，供内部模块（网络总线等等）
** \param nOccNo 
** \param val 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年11月2日 
** \note 
********************************************************************************************************/
bool CMemDB::GetAinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const
{
	Q_ASSERT(false);

	return true;
}

bool CMemDB::GetDinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const
{
	Q_ASSERT(false);

	return true;
}
/*! \fn bool  CMemDB::UpdateAinValue(int32u nOccNo, CVariantEx & val, int8u nQuality)
********************************************************************************************************* 
** \brief CMemDB::UpdateAinValue 
** \details 更新内存库中的测值，由网络总线调用
** \param nOccNo 
** \param val 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年11月3日 
** \note 
********************************************************************************************************/
bool  CMemDB::UpdateAinValue(int32u nOccNo, const CVariant & val, int8u nQuality)
{

	return true;
}

bool  CMemDB::UpdateDinValue(int32u nOccNo, const CVariant & val, int8u nQuality)
{

	return true;
}

/** @}*/
