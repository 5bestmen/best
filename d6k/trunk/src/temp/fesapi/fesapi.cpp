/*! @file shape_widget.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  shape_widget.cpp
文件实现功能 :  基本形状图元
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本形状图元
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/


#include <vector>


#define IOSDK_LIB
#include "fesapi/fesapi.h"
#include "kernel/kernel.h"


/*! \fn  FESAPI KRESULT CreateIO()
********************************************************************************************************* 
** \brief CreateIO 
** \details 
** \return KRESULT 
** \date 2015年9月15日 
** \note 创建内存库
********************************************************************************************************/
FESAPI KRESULT CreateIO()
{
	if(g_kernel.CreateDataPool())
		return K_TRUE;

	return K_FALSE;
}

/*! \fn  FESAPI KRESULT DestroyIO()
********************************************************************************************************* 
** \brief DestroyIO 
** \details 
** \return KRESULT 
** \date 2015年9月15日 
** \note 销毁内存库
********************************************************************************************************/
FESAPI KRESULT DestroyIO()
{
	g_kernel.DestroyDataPool();

	return K_TRUE;
}

/*! \fn FESAPI KRESULT OpenIO(int32u nChannelOccNO)
********************************************************************************************************* 
** \brief OpenIO 
** \details 
** \param nChannelOccNO 
** \return KRESULT 
** \date 2015年9月15日 
** \note 打开内存库
********************************************************************************************************/
FESAPI KRESULT OpenIO(int32u nChannelOccNO)
{
	if(g_kernel.OpenDataPool(nChannelOccNO))
		return K_TRUE;

	return K_FALSE;
}

/*! \fn FESAPI KRESULT CloseIO(int32u nChannelOccNO)
********************************************************************************************************* 
** \brief CloseIO 
** \details 
** \param nChannelOccNO 
** \return KRESULT 
** \date 2015年9月15日 
** \note 关闭内存库
********************************************************************************************************/
FESAPI KRESULT CloseIO(int32u nChannelOccNO)
{
	if(g_kernel.CloseDataPool(nChannelOccNO))
		return K_TRUE;

	return K_FALSE;
}
#if 0
/*! \fn KRESULT GetAIN(int32u occNO,AIN** ppAIN)
********************************************************************************************************* 
** \brief GetAIN 
** \details 
** \param occNO 
** \param ppAIN 
** \return KRESULT 
** \date 2015年9月15日 
** \note 根据OccNO获取AIN
********************************************************************************************************/
KRESULT GetAIN(int32u occNO,AIN** ppAIN)
{
	AIN* pAin = g_kernel.GetAIN(occNO);

	*ppAIN = pAin;

	if(pAin == NULL) 
		return K_FALSE;

	return K_TRUE;
}

/*! \fn KRESULT GetDIN(int32u occNO,DIN** ppDIN)
********************************************************************************************************* 
** \brief GetDIN 
** \details 
** \param occNO 
** \param ppDIN 
** \return KRESULT 
** \date 2015年9月15日 
** \note 根据OccNO获取DIN
********************************************************************************************************/
KRESULT GetDIN(int32u occNO,DIN** ppDIN)
{
	DIN* pDin = g_kernel.GetDIN(occNO);

	*ppDIN = pDin;

	if(pDin == NULL)
		return K_FALSE;

	return K_TRUE;
}

/*! \fn KRESULT SetAINValue(int32u occNO,fp64 fValue,int32u nQuality)
********************************************************************************************************* 
** \brief SetAINValue 
** \details 
** \param occNO 
** \param fValue 
** \param nQuality 
** \return KRESULT 
** \date 2015年9月15日 
** \note 根据OccNO设置AIN的值和品质
********************************************************************************************************/
KRESULT IoSetAINValue(int32u nChannelOccNo,int32u occNO,fp64 fValue, int8u nQuality)
{
	if (g_kernel.IoSetAINValue(nChannelOccNo,occNO, fValue, nQuality))
		return K_TRUE;
	return K_FALSE;
}

KRESULT AppSetAINValue(int32u nAppOccNo, int32u nOccNo, fp64 fValue, int8u nQuality)
{
	if (g_kernel.AppSetAINValue(nAppOccNo, nOccNo, fValue, nQuality))
		return K_TRUE;
	return K_FALSE;
}
/*! \fn KRESULT SetDINValue(int32u occNO,int8u nValue,int32u nQuality)
********************************************************************************************************* 
** \brief SetDINValue 
** \details 
** \param occNO 
** \param nValue 
** \param nQuality 
** \return KRESULT 
** \date 2015年9月15日 
** \note 根据OccNO设置DIN的值和品质
********************************************************************************************************/
KRESULT IoSetDINValue(int32u nChannelOccNo, int32u occNO, int8u nValue, int8u nQuality)
{
	if (g_kernel.SetDINValue(nChannelOccNo,occNO, nValue, nQuality))
		return K_TRUE;
	return K_FALSE;
}

/*! \fn KRESULT GetAINValue(int32u occNO,fp32* pValue)
********************************************************************************************************* 
** \brief GetAINValue 
** \details 
** \param occNO 
** \param pValue 
** \return KRESULT 
** \date 2015年9月15日 
** \note 根据OccNO获取AIN的值
********************************************************************************************************/
KRESULT GetAINValue(int32u occNO,fp64* pValue)
{
	//Assert(pValue != NULL);

	*pValue = g_kernel.GetAINValue(occNO);

	return K_TRUE;
}

/*! \fn KRESULT GetDINValue(int32u occNO,int8u* pValue)
********************************************************************************************************* 
** \brief GetDINValue 
** \details 
** \param occNO 
** \param pValue 
** \return KRESULT 
** \date 2015年9月15日 
** \note 根据OccNO获取DIN的值
********************************************************************************************************/
KRESULT GetDINValue(int32u occNO,int8u* pValue)
{
	//Assert(pValue != NULL);

	*pValue = g_kernel.GetDINValue(occNO);

	return K_TRUE;
}


/*! \fn KRESULT GetDevice(int32u nDeviceOccNO,DEVICE** ppDevice)
********************************************************************************************************* 
** \brief GetDevice 
** \details 
** \param nDeviceOccNO 
** \param ppDevice 
** \return KRESULT 
** \date 2015年9月15日 
** \note 根据装置的OccNo获取DEVICE
********************************************************************************************************/
KRESULT GetDevice(int32u nDeviceOccNO,DEVICE** ppDevice)
{
	DEVICE* pDevice = g_kernel.GetDevice(nDeviceOccNO);

	*ppDevice = pDevice;

	if(pDevice == NULL)
		return K_TRUE;

	return K_FALSE;
}

FESAPI KRESULT GetDeviceAddress(int32u nDeviceOccNO, int32u* pDeviceAddress)
{
	*pDeviceAddress = g_kernel.GetDeviceAddress(nDeviceOccNO);
	return K_TRUE;
}
/*! \fn KRESULT GetChannel(int32u nChannelOccNO,CHANNEL** ppChannel)
********************************************************************************************************* 
** \brief GetChannel 
** \details 
** \param nChannelOccNO 
** \param ppChannel 
** \return KRESULT 
** \date 2015年9月15日 
** \note 根据通道的OccNo获取CHANNEL
********************************************************************************************************/
KRESULT GetChannel(int32u nChannelOccNO,CHANNEL** ppChannel)
{
	CHANNEL* pChannel = g_kernel.GetChannel(nChannelOccNO);

	*ppChannel = pChannel;

	if(ppChannel == NULL)
		return K_TRUE;

	return K_FALSE;
}


/*! \fn KRESULT AppGetAinValue(int32u nOccNo,fp64* pValue)
********************************************************************************************************* 
** \brief AppGetAinValue 
** \details 
** \param nOccNo 
** \param pValue 
** \return KRESULT 
** \date 2015年9月15日 
** \note 根据OccNO获取AIN的值
********************************************************************************************************/
KRESULT AppGetAinValue(int32u nOccNo,fp64* pValue)
{
	AIN* pAin = g_kernel.GetAIN(nOccNo);
	if(pAin == NULL)
		return K_FALSE;

	*pValue = pAin->Value;

	return K_TRUE;
}

/*! \fn KRESULT AppGetDinValue(int32u nOccNo,int8u* pValue)
********************************************************************************************************* 
** \brief AppGetDinValue 
** \details 
** \param nOccNo 
** \param pValue 
** \return KRESULT 
** \date 2015年9月15日 
** \note 根据OccNO获取DIN的值
********************************************************************************************************/
KRESULT AppGetDinValue(int32u nOccNo,int8u* pValue)
{
	DIN* pDin = g_kernel.GetDIN(nOccNo);
	if(pDin == NULL)
		return K_FALSE;

	*pValue = pDin->Value;

	return K_TRUE;
}

/*! \fn 
********************************************************************************************************* 
** \brief GetDeviceCount 
** \details 
** \param pCount 
** \return KRESULT 
** \date 2015年9月15日 
** \note 获取通道下装置数目
********************************************************************************************************/
KRESULT GetDeviceCount(int32u nChannelOccNo, int32u* pCount)
{
	*pCount = g_kernel.GetDeviceCount(nChannelOccNo);

	return K_TRUE;
}

/*! \fn KRESULT GetDeviceOccNos(int32u arrayDeviceOccNo[],int32u arraySize)
********************************************************************************************************* 
** \brief GetDeviceOccNosInChannel 
** \details 
** \param arrayDeviceOccNo 
** \param arraySize 
** \return KRESULT 
** \date 2015年9月15日 
** \note 获取通道下所有装置的OccNo
********************************************************************************************************/
KRESULT GetDeviceOccNos(int32u nChannelOccNo, int32u arrayDeviceOccNo[], int32u arraySize)
{
	std::vector<int32u> vecOccNos;
	g_kernel.GetDeviceOccNos(nChannelOccNo,vecOccNos);

	for(int32u i=0;i<vecOccNos.size();i++)
	{
		if(i>=arraySize)
			return K_FALSE;

		arrayDeviceOccNo[i] = vecOccNos[i];
	}
	return K_TRUE;
}

/*! \fn KRESULT IsDriverQuit(int32u nChannelOccNo)
********************************************************************************************************* 
** \brief IsDriverQuit 
** \details 
** \param nChannelOccNo 驱动的通道号
** \return KRESULT K_TRUE:退出 K_FALSE:不退出
** \date 2015年9月17日 
** \note 驱动是否要退出
********************************************************************************************************/
KRESULT IsDriverQuit(int32u nChannelOccNo)
{
	if(g_kernel.IsDriverQuit(nChannelOccNo))
		return K_TRUE;

	return K_FALSE;
}
/*! \fn KRESULT LoadDrivers()
********************************************************************************************************* 
** \brief LoadDrivers 
** \details 
** \return KRESULT 
** \date 2015年9月17日 
** \note 加载所有驱动
********************************************************************************************************/
KRESULT LoadDrivers()
{
	if(g_kernel.LoadDrivers())
		return K_TRUE;

	return K_FALSE;
}
/*! \fn KRESULT UnLoadDrivers()
********************************************************************************************************* 
** \brief UnLoadDrivers 
** \details 
** \return KRESULT 
** \date 2015年9月17日 
** \note 卸载所有驱动
********************************************************************************************************/
KRESULT UnLoadDrivers()
{
	g_kernel.UnLoadDrivers();

	return K_TRUE;
}


KRESULT GetAinCount(int32u nChannelOccNo, int32u nDeviceOccNo, int32u* pCount)
{
	*pCount = g_kernel.GetAinCount(nChannelOccNo, nDeviceOccNo);

	return K_TRUE;
}

KRESULT GetAinOccNos(int32u nChannelOccNo, int32u nDeviceOccNo,int32u arrayAinOccNo[], int32u arraySize)
{
	std::vector<int32u> vecOccNos;
	g_kernel.GetAinOccNos(nChannelOccNo, nDeviceOccNo,vecOccNos);

	for (int32u i = 0; i < vecOccNos.size(); i++)
	{
		if (i >= arraySize)
			return K_FALSE;

		arrayAinOccNo[i] = vecOccNos[i];
	}
	return K_TRUE;
}


KRESULT GetDinCount(int32u nChannelOccNo, int32u nDeviceOccNo, int32u* pCount)
{
	*pCount = g_kernel.GetDinCount(nChannelOccNo, nDeviceOccNo);

	return K_TRUE;
}

KRESULT GetDinOccNos(int32u nChannelOccNo, int32u nDeviceOccNo, int32u arrayDinOccNo[], int32u arraySize)
{
	std::vector<int32u> vecOccNos;
	g_kernel.GetDinOccNos(nChannelOccNo, nDeviceOccNo, vecOccNos);

	for (int32u i = 0; i < vecOccNos.size(); i++)
	{
		if (i >= arraySize)
			return K_FALSE;

		arrayDinOccNo[i] = vecOccNos[i];
	}
	return K_TRUE;
}

FESAPI KRESULT GetAinAddress(int32u nAinOccNo, int32u* pAinAddress)
{
	*pAinAddress = g_kernel.GetAinAddress(nAinOccNo);

	return K_TRUE;
}

FESAPI KRESULT GetDinAddress(int32u nDinOccNo, int32u* pDinAddress)
{
	*pDinAddress = g_kernel.GetDinAddress(nDinOccNo);

	return K_TRUE;
}

#endif
