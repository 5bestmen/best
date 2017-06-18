
#include <iostream>

//#include "utils/misc.h"

#include <QSharedMemory> 

#include "kernel.h"

CKernel g_kernel;

CKernel::CKernel()
{

}

CKernel::~CKernel()
{

}

/*! \fn bool CKernel::CreateDataPool()
********************************************************************************************************* 
** \brief CKernel::CreateDataPool 
** \details 根据配置文件，创建前置内存库（前置内核使用）
** \return bool 
** \author LiJin
** \date 2016年4月24日 
** \note 
********************************************************************************************************/
bool CKernel::CreateDataPool()
{
	// 读取配置文件 

	// 创建共享内存

	return true;
}


void CKernel::DestroyDataPool()
{

}

bool CKernel::OpenDataPool(int32u nChannelOccNo)
{
	return true;

}

bool CKernel::CloseDataPool(int32u nChannelOccNo)
{
	return true;

}


#if 0
/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateDataPool 
** \details 
** \return bool 
** \date 2015年9月15日 
** \note 根据配置文件，创建前置内存库（前置内核使用）
********************************************************************************************************/
bool CKernel::CreateDataPool()
{
	TiXmlDocument doc("fes.xml");
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay )
	{
		char buf[256];
		::sprintf(buf,"读取xml数据错误. Error='%s'", doc.ErrorDesc());

		return false;
	}

	TiXmlElement* pRootElement = doc.RootElement();
	if(pRootElement==NULL)
		return false;

	TiXmlElement* pChannelElement=NULL;
	TiXmlElement* pDeviceElement=NULL;
	TiXmlElement* pAINElement=NULL;
	TiXmlElement* pDINElement=NULL;
	TiXmlElement* pAOUTElement = NULL;
	TiXmlElement* pDOUTElement = NULL;
	TiXmlElement* pLinearElement = NULL;   //线性转换
	TiXmlElement* pNonLinearElement = NULL;//非线性转换

	TiXmlElement* pAinAlarmElement = NULL;
	TiXmlElement* pDinAlarmElement = NULL;

	//根据配置relation，创建内存库
	TiXmlNode *pChild;
	for ( pChild = pRootElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string strChildName = pChild->Value();

		if (Utils::IsEqualIgnoreCase(strChildName, "channel"))
		{
			pChannelElement = pChild->ToElement();
		}

		if (Utils::IsEqualIgnoreCase(strChildName, "device"))
		{
			pDeviceElement = pChild->ToElement();
		}

		if (Utils::IsEqualIgnoreCase(strChildName, "data"))
		{
			TiXmlNode *pDataChild;
			TiXmlElement* pDataElement = pChild->ToElement();
			for ( pDataChild = pDataElement->FirstChild(); pDataChild != 0; pDataChild = pDataChild->NextSibling()) 
			{
				strChildName = pDataChild->Value();
				if (Utils::IsEqualIgnoreCase(strChildName, "AIN"))
				{
					pAINElement = pDataChild->ToElement();
				}
				if (Utils::IsEqualIgnoreCase(strChildName, "DIN"))
				{
					pDINElement = pDataChild->ToElement();
				}
				if (Utils::IsEqualIgnoreCase(strChildName, "AOUT"))
				{
					pAOUTElement = pDataChild->ToElement();
				}
				if (Utils::IsEqualIgnoreCase(strChildName, "DOUT"))
				{
					pDOUTElement = pDataChild->ToElement();
				}
			}
		}

		if (Utils::IsEqualIgnoreCase(strChildName, "transform"))
		{
			TiXmlNode *pDataChild;
			TiXmlElement* pDataElement = pChild->ToElement();
			for (pDataChild = pDataElement->FirstChild(); pDataChild != 0; pDataChild = pDataChild->NextSibling())
			{
				strChildName = pDataChild->Value();
				if (Utils::IsEqualIgnoreCase(strChildName, "linear"))
				{
					pLinearElement = pDataChild->ToElement();
				}
				if (Utils::IsEqualIgnoreCase(strChildName, "nonlinear"))
				{
					pNonLinearElement = pDataChild->ToElement();
				}
			}
		}

		if (Utils::IsEqualIgnoreCase(strChildName, "alarm"))
		{
			TiXmlNode *pDataChild;
			TiXmlElement* pDataElement = pChild->ToElement();
			for (pDataChild = pDataElement->FirstChild(); pDataChild != 0; pDataChild = pDataChild->NextSibling())
			{
				strChildName = pDataChild->Value();
				if (Utils::IsEqualIgnoreCase(strChildName, "ain_alarm"))
				{
					pAinAlarmElement = pDataChild->ToElement();
				}
				if (Utils::IsEqualIgnoreCase(strChildName, "din_alarm"))
				{
					pDinAlarmElement = pDataChild->ToElement();
				}
			}
		}


	}

	if ((pChannelElement == NULL) || (pDeviceElement == NULL) || 
		(pAINElement == NULL) || (pDINElement == NULL) || (pAOUTElement == NULL) || (pDOUTElement == NULL) ||
		(pLinearElement == NULL) || (pNonLinearElement == NULL) || 
		(pAinAlarmElement == NULL) || (pDinAlarmElement == NULL) )
	{
		return false;
	}

	if(!CreateChannelTable(pChannelElement))
		return false;

	if(!CreateDeviceTable(pDeviceElement))
		return false;

	if(!CreateAINTable(pAINElement))
		return false;

	if(!CreateDINTable(pDINElement))
		return false;

	if (!CreateAOUTTable(pAOUTElement))
		return false;

	if (!CreateDOUTTable(pDOUTElement))
		return false;

	if (!CreateTransformLinearTable(pLinearElement))
		return false; 

	if (!CreateTransformNonLinearTable(pNonLinearElement))
		return false;

	if (!CreateAinAlarmTable(pAinAlarmElement))
		return false;

	if (!CreateDinAlarmTable(pDinAlarmElement))
		return false;
	
	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::OpenDataPool 
** \details 
** \param nChannelOccNo 通道OccNO
** \return bool 
** \date 2015年9月15日 
** \note 打开前置所创建的内存库(驱动使用)
********************************************************************************************************/
bool CKernel::OpenDataPool(int32u nChannelOccNo)
{
	if(!OpenChannelTable())
		return false;

	if(!OpenDeviceTable())
		return false;

	if(!OpenAINTable())
		return false;

	if(!OpenDINTable())
		return false;

	if (!OpenAOUTTable())
		return false;

	if (!OpenDOUTTable())
		return false;

	if (!OpenTransformLinearTable())
		return false;

	if (!OpenTransformNonLinearTable())
		return false;

	if (!OpenAinAlarmTable())
		return false;

	if (!OpenDinAlarmTable())
		return false;
		
	//设置驱动状态
	SetChannelDriverState(nChannelOccNo,DRIVER_STATE_LOAD);

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CloseDataPool 
** \details 
** \param nChannelOccNo 通道OccNo
** \return bool 
** \date 2015年9月15日 
** \note 关闭前置创建的内存库(驱动使用)
********************************************************************************************************/
bool CKernel::CloseDataPool(int32u nChannelOccNo)
{
	//设置驱动状态
	SetChannelDriverState(nChannelOccNo,DRIVER_STATE_UNLOAD);

	DestroyDataPool();

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::DestroyDataPool 
** \details 
** \return void 
** \date 2015年9月15日 
** \note 销毁内存库(前置内核使用)
********************************************************************************************************/
void CKernel::DestroyDataPool()
{
	DestroyAINTable();
	DestroyDINTable();
	
	DestroyAOUTTable();
	DestroyDOUTTable();

	DestroyDeviceTable();
	DestroyChannelTable();

	DestroyTransformLinearTable();
	DestroyTransformNonLinearTable();

	DestroyAinAlarmTable();
	DestroyDinAlarmTable();
}

#endif
