
#include "kernel.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateDeviceTable 
** \details 
** \param pParentElement 
** \return bool 
** \date 2015年9月15日 
** \note 创建装置内存库
********************************************************************************************************/
bool CKernel::CreateDeviceTable(TiXmlElement* pParentElement)
{
	int32u nCount = GetElementChildCount(pParentElement,"d");
	if(nCount <=0)
	{
		return false;
	}

	if(!m_poolDevice.CreateArray(SMKEY_MEM_DEVICE,nCount))
	{
		return false;
	}


	TiXmlNode *pChild;
	
	for (pChild = pParentElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string strChildName = pChild->Value();

		if (!Utils::IsEqualIgnoreCase(strChildName, "d"))//不是a(比如是注释等)
			continue;

		//获取device配置信息
		TiXmlElement* pDeviceElement = pChild->ToElement();           

		//获取属性值
		int nOccNo =0;
		int nNameOccNo = 0;
		int nModelOccNo = 0;
		int nType = 0;
		int nNodeOccNo = 0;
		int nChannelOccNo = 0;
		int nAddress = 0;

		pDeviceElement->QueryIntAttribute("OccNo",&nOccNo);
		pDeviceElement->QueryIntAttribute("NameOccNo", &nNameOccNo);
		pDeviceElement->QueryIntAttribute("NodeOccNo", &nNodeOccNo);
		pDeviceElement->QueryIntAttribute("ChannelOccNo", &nChannelOccNo);
		pDeviceElement->QueryIntAttribute("ModelOccNo", &nModelOccNo);
		pDeviceElement->QueryIntAttribute("Type", &nType);
		pDeviceElement->QueryIntAttribute("DeviceAddr", &nAddress);

		//pDeviceElement->QueryIntAttribute("DeviceAddr",&nDeviceAddr);           
		
		DEVICE* pDevice = GetDevice(nOccNo);
		if(pDevice==NULL)
		{
			return false;
		}

		//初始化device对象
		pDevice->OccNo = nOccNo;
		pDevice->NameOccNo = nNameOccNo;
		pDevice->NodeOccNo = nNodeOccNo;
		pDevice->ChannelOccNo = nChannelOccNo;
		pDevice->ModelOccNo = nModelOccNo;
		pDevice->Type = nType;
		pDevice->Address = nAddress;
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::OpenDeviceTable 
** \details 
** \return bool 
** \date 2015年9月15日 
** \note 打开装置内存库
********************************************************************************************************/
bool CKernel::OpenDeviceTable()
{
	if(!m_poolDevice.OpenArray(SMKEY_MEM_DEVICE))
		return false;

	return true;
}


/*! \fn 
********************************************************************************************************* 
** \brief CKernel::DestroyDeviceTable 
** \details 
** \return void 
** \date 2015年9月15日 
** \note 关闭装置内存库
********************************************************************************************************/
void CKernel::DestroyDeviceTable()
{
	m_poolDevice.CloseArray();
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetDevice 
** \details 
** \param occNo 
** \return DEVICE* 
** \date 2015年9月15日 
** \note 根据装置OCCNO获取装置数据指针
********************************************************************************************************/
DEVICE* CKernel::GetDevice(int32u occNo)
{
	int32u nCount = m_poolDevice.GetArraySize();
	occNo = occNo -1;//occNo从1开始
	if(occNo<0 || occNo>=nCount)
		return NULL;

	return m_poolDevice.GetObject(occNo);
}

int32u CKernel::GetDeviceCount(int32u nChannelOccNo)
{
	int nDeviceCount = 0;

	int32u nCount = m_poolDevice.GetArraySize();
	for (int i = 0; i < nCount; i++)
	{
		DEVICE* pDevice = m_poolDevice.GetObject(i);
		if (pDevice==NULL)
			continue;
		if (pDevice->ChannelOccNo == nChannelOccNo)
			nDeviceCount++;
	}
	
	return nDeviceCount;
}

int32u CKernel::GetDeviceAddress(int32u nDeviceOccNO)
{
	DEVICE* pDevice = GetDevice(nDeviceOccNO);
	if (pDevice == NULL)
		return 0;

	return pDevice->Address;
}

void CKernel::GetDeviceOccNos(int32u nChannelOccNo, std::vector<int32u>& vecDeviceOccNos)
{
	int32u nCount = m_poolDevice.GetArraySize();
	for (int i = 0; i < nCount; i++)
	{
		DEVICE* pDevice = m_poolDevice.GetObject(i);
		if (pDevice==NULL)
			continue;

		if (pDevice->ChannelOccNo == nChannelOccNo)
		{
			vecDeviceOccNos.push_back(pDevice->OccNo);
		}
	}
}

