/*! @file db_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  db_svc.cpp
文件实现功能 :  内存数据库
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存数据库
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "db_svc.h"
#include "log/log.h"
#include "fes_db.h"
#include "fesapi/fescnst.h"
#include <QObject> 
#include <QString> 
//#include <QHostInfo>
#include <QDebug>
#include <memory>

CDbSvc::CDbSvc(CScadaSvc* pServer, const std::string & szMailBoxName, int &nMailBoxID) : CBaseModule(pServer, szMailBoxName, nMailBoxID)
{
	m_bStopFlag = false;
	m_nMyNodeOccNo = INVALID_OCCNO;
//	m_pNodeGrpMgr = std::make_shared<CNodeGroupMgr>();
	m_pMem = std::make_shared<CShareMem>();
}

CDbSvc::~CDbSvc(void)
{ 
	if (m_pMem)
	{
		m_pMem->Destroy();
		m_pMem.reset();
		m_pMem = nullptr;
	}
	if (m_pDBAliveFlag)
	{
		m_pDBAliveFlag->reset();
		m_pDBAliveFlag.reset();
	}
}

/*! \fn bool CDbSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CDbSvc::Initialize 
** \details 初始化  根据参数映射共享内存，建立与SCDSVC一样的数据结构
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CDbSvc::Initialize(const char *pszDataPath, unsigned int nMode)
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

	
	QString szDataBaseName = QString(pszDataPath) + "/scada";
	// EVENT 设置成 手动模式，初始化为 true，名字为内存库名字 第一参数为模式设定 ，第二参数为是否有信号 ，第三参数为进程内外 ，第四为全局信号名称
	m_pDBAliveFlag = std::make_shared<ACE_Event>(1, 0, USYNC_PROCESS, szDataBaseName.toStdString().c_str());

	bool bRet = IsDBAlive(0);

	return true;
}

void CDbSvc::Run()
{

}

void CDbSvc::Shutdown()
{
	if (m_pDBAliveFlag)
	{
		m_pDBAliveFlag.reset();
	}
}


bool CDbSvc::IsDBAlive(unsigned  int nTimeout)
{
	if (m_pDBAliveFlag == nullptr)
		return false;

	ACE_Time_Value tm_val(0, nTimeout * 1000);

	int nRet = m_pDBAliveFlag->wait(&tm_val, 0);

	return (nRet == 0) ? true : false;
}

bool CDbSvc::GetAinValue(int32u nNodeOccNo, int32u nOccNo, fp64 & fpVal)const
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO  && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}

	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	std::shared_ptr<CFesDB> pFes = nullptr;

	auto it = m_mapFes.find(nNodeOccNo);
	if (it == m_mapFes.end())
	{	 
		return false;
	}
	else
	{
		pFes = it->second;
		Q_ASSERT(pFes);
		if (pFes == nullptr)
		{
			return false;
		}
	}
	Q_ASSERT(pFes->GetNodeOccNo() == nNodeOccNo);

	CVariant dblVal(fpVal, DT_DOUBLE);
	int8u nQua = 0;

	bool bRet = pFes->GetAinValue(nNodeOccNo, dblVal, nQua);
	if (bRet)
	{
		fpVal = dblVal.operator double();
	}

	return bRet;
}

bool CDbSvc::GetDinValue(int32u nNodeOccNo, int32u nOccNo, int8u & bVal)const
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO  && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}

	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	auto it = m_mapFes.find(nNodeOccNo);

	std::shared_ptr<CFesDB> pFes = nullptr;
	if (it == m_mapFes.end())
	{
		return false;
	}
	else
	{
		pFes = it->second;
		if (!pFes)
		{
			return false;
		}
	}
	Q_ASSERT(pFes->GetNodeOccNo() == nNodeOccNo);

	CVariant nVariant(bVal, DT_BOOLEAN);

	int8u nQua = 0;

	bool bRet = pFes->GetDinValue(nNodeOccNo, nVariant, nQua);
	if (bRet)
	{
		bVal = nVariant.operator unsigned char();
	}

	return bRet;
}
#if 0
/*! \fn bool CDbSvc::UpdateAinValue(int32u nNodeOccNo, int nAinOccNo, fp64  nVal)
********************************************************************************************************* 
** \brief CDbSvc::UpdateAinValue 
** \details  用于内部更新AIN值
** \param nNodeOccNo 
** \param nAinOccNo 
** \param nVal 
** \return bool 
** \author xingzhibing
** \date 2017年1月18日 
** \note 
********************************************************************************************************/
bool CDbSvc::UpdateAinValue(int32u nNodeOccNo, int32u nAinOccNo, fp64  dblVal)
{
	Q_ASSERT(nNodeOccNo !=INVALID_OCCNO  && nNodeOccNo <=MAX_NODE_OCCNO );
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo >MAX_NODE_OCCNO)
	{
		return false;
	}

	Q_ASSERT(nAinOccNo !=INVALID_OCCNO && nAinOccNo <=MAX_OCCNO);
	if (nAinOccNo ==INVALID_OCCNO || nAinOccNo > MAX_OCCNO)
	{
		return false;
	}

	std::shared_ptr<CFesDB> pFes = nullptr;

	auto it = m_mapFes.find(nNodeOccNo);
	if (it == m_mapFes.end())
	{
		Q_ASSERT(false);
		return false;
	}
	else
	{
		pFes = it->second;
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}
	}
	CVariant nVariant(dblVal,DT_DOUBLE);
// 	nVariant.Type = DT_FLOAT;
// 	memcpy(&nVariant.Value, &nVal,sizeof  fp64);
	return pFes->UpdateAinValue(nAinOccNo, nVariant, 1);
}
/*! \fn bool CDbSvc::UpdateDinValue(int32u nNodeOccNo, int nOccNo, int8u& nVal)
********************************************************************************************************* 
** \brief CDbSvc::UpdateDinValue 
** \details 用于内部更新DIN值
** \param nNodeOccNo 
** \param nOccNo 
** \param nVal 
** \return bool 
** \author xingzhibing
** \date 2017年1月18日 
** \note 
********************************************************************************************************/
bool CDbSvc::UpdateDinValue(int32u nNodeOccNo, int32u nOccNo, int8u nVal)
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO  && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}

	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	auto it = m_mapFes.find(nNodeOccNo);

	std::shared_ptr<CFesDB> pFes = nullptr;
	if (it == m_mapFes.end())
	{
		return false;
	}
	else
	{
		pFes = it->second;
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}
	}
	CVariant nVariant(nVal, DT_BOOLEAN);
// 	CVariant nVariant;
// 	nVariant.Type = DT_CHAR;
// 	memcpy(&nVariant.Value, &nVal, sizeof  int8u);
	return pFes->UpdateDinValue(nOccNo, nVariant, 1);
}

#endif



/** @}*/
