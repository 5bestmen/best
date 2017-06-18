#include "configinfo.h"


/*! \fn void CDevice::AddAInInfo(AIN* pAin)
********************************************************************************************************* 
** \brief CDevice::AddAInInfo 
** \details ����ģ������
** \param pAin 
** \return void 
** \author xingzhibing
** \date 2016��9��1�� 
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
** \details ���ӿ�������
** \param pDin 
** \return void 
** \author xingzhibing
** \date 2016��9��1�� 
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
** \details �豸����������װ��
** \param pDevice 
** \return void 
** \author xingzhibing
** \date 2016��9��1�� 
** \note 
********************************************************************************************************/
void CDeviceMgr::AddDevice(CDevice* pDevice)
{
	Q_ASSERT(pDevice);
	m_VecDevice.push_back(pDevice);
}
