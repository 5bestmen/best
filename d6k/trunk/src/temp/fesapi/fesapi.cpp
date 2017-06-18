/*! @file shape_widget.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  shape_widget.cpp
�ļ�ʵ�ֹ��� :  ������״ͼԪ
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ������״ͼԪ
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
** \date 2015��9��15�� 
** \note �����ڴ��
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
** \date 2015��9��15�� 
** \note �����ڴ��
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
** \date 2015��9��15�� 
** \note ���ڴ��
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
** \date 2015��9��15�� 
** \note �ر��ڴ��
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
** \date 2015��9��15�� 
** \note ����OccNO��ȡAIN
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
** \date 2015��9��15�� 
** \note ����OccNO��ȡDIN
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
** \date 2015��9��15�� 
** \note ����OccNO����AIN��ֵ��Ʒ��
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
** \date 2015��9��15�� 
** \note ����OccNO����DIN��ֵ��Ʒ��
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
** \date 2015��9��15�� 
** \note ����OccNO��ȡAIN��ֵ
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
** \date 2015��9��15�� 
** \note ����OccNO��ȡDIN��ֵ
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
** \date 2015��9��15�� 
** \note ����װ�õ�OccNo��ȡDEVICE
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
** \date 2015��9��15�� 
** \note ����ͨ����OccNo��ȡCHANNEL
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
** \date 2015��9��15�� 
** \note ����OccNO��ȡAIN��ֵ
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
** \date 2015��9��15�� 
** \note ����OccNO��ȡDIN��ֵ
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
** \date 2015��9��15�� 
** \note ��ȡͨ����װ����Ŀ
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
** \date 2015��9��15�� 
** \note ��ȡͨ��������װ�õ�OccNo
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
** \param nChannelOccNo ������ͨ����
** \return KRESULT K_TRUE:�˳� K_FALSE:���˳�
** \date 2015��9��17�� 
** \note �����Ƿ�Ҫ�˳�
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
** \date 2015��9��17�� 
** \note ������������
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
** \date 2015��9��17�� 
** \note ж����������
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
