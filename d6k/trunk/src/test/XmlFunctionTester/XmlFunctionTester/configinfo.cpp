#include "configinfo.h"


/*! \fn void CDevice::AddAInInfo(AIN* pAin)
********************************************************************************************************* 
** \brief CDevice::AddAInInfo 
** \details 增加模拟量点
** \param pAin 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CDevice::AddAInInfo(AIN* pAin)
{
	Q_ASSERT(pAin);
	m_VecAinData.push_back(pAin);
}

/*! \fn void CDevice::AddDINInfo(DIN* pDin)
********************************************************************************************************* 
** \brief CDevice::AddDINInfo 
** \details 增加开关量点
** \param pDin 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CDevice::AddDINInfo(DIN* pDin)
{
	Q_ASSERT(pDin);
	m_VecDinData.push_back(pDin);
}

/*! \fn void CDeviceMgr::AddDevice(CDevice* pDevice)
********************************************************************************************************* 
** \brief CDeviceMgr::AddDevice 
** \details 设备管理器增加装置
** \param pDevice 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CDeviceMgr::AddDevice(CDevice* pDevice)
{
	Q_ASSERT(pDevice);
	m_VecDevice.push_back(pDevice);
}
