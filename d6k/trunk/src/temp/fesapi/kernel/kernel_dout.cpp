
#include "utils/misc.h"

#include "kernel.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateDOUTTable 
** \details 
** \param pParentElement 配置文件中DOUT区根节点
** \return bool true:成功，false:失败
** \date 2015年9月15日 
** \note 创建DOUT内存库
********************************************************************************************************/
bool CKernel::CreateDOUTTable(TiXmlElement* pParentElement)
{
	int nCount = GetElementChildCount(pParentElement,"do");
	if(nCount <=0)
	{
		return false;
	}

	if(!m_poolDOUT.CreateArray(SMKEY_MEM_DOUT,nCount))
	{
		return false;
	}

	TiXmlNode *pChild;

	for (pChild = pParentElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string strChildName = pChild->Value();

		if(!Utils::IsEqualIgnoreCase(strChildName,"do"))
			continue;

		//获取DOUT配置信息
		TiXmlElement* pDOUTElement = pChild->ToElement();           

		//获取属性值
		int nOccNO = 0;
		int nBlockOccNO = 0;
		int nNameOccNO = 0;
		int nNodeOccNO = 0;
		int nChannelOccNo = 0;
		int nDeviceOccNO = 0;

		pDOUTElement->QueryIntAttribute("OccNo", &nOccNO);
		pDOUTElement->QueryIntAttribute("BlockOccNo", &nBlockOccNO);
		pDOUTElement->QueryIntAttribute("NameOccNo", &nNameOccNO);
		pDOUTElement->QueryIntAttribute("NodeOccNo", &nNodeOccNO);
		pDOUTElement->QueryIntAttribute("ChannelOccNo", &nChannelOccNo);
		pDOUTElement->QueryIntAttribute("DeviceOccNo", &nDeviceOccNO);
		
		DOUT* pDOUT = GetDOUT(nOccNO);

		if(pDOUT==NULL)
		{
			return false;
		}

		//初始化DOUT对象
		pDOUT->OccNo = nOccNO;
		pDOUT->BlockNo = nBlockOccNO;
		pDOUT->NameOccNo = nNameOccNO;
		pDOUT->NodeOccNo = nNodeOccNO;
		pDOUT->ChannelOccNo = nChannelOccNo;
		pDOUT->DeviceOccNo = nDeviceOccNO;
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::OpenDOUTTable 
** \details 
** \return bool 
** \date 2015年9月15日 
** \note 打开DOUT内存库
********************************************************************************************************/
bool CKernel::OpenDOUTTable()
{
	if(!m_poolDOUT.OpenArray(SMKEY_MEM_DOUT))
		return false;

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::DestroyDOUTTable 
** \details 
** \return void 
** \date 2015年9月15日 
** \note 关闭DOUT内存库
********************************************************************************************************/
void CKernel::DestroyDOUTTable()
{
	m_poolDOUT.CloseArray();
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetDOUT 
** \details 
** \param occNO 
** \return DOUT* 
** \date 2015年9月15日 
** \note 根据DOUT的OCCNO获取DOUT数据结构指针
********************************************************************************************************/
DOUT* CKernel::GetDOUT(int32u occNO)
{
	int nCount = m_poolDOUT.GetArraySize();
	occNO = occNO -1;//occNo从1开始
	if(occNO<0 || occNO>=nCount)
		return NULL;

	DOUT* pDOUT = m_poolDOUT.GetObject(occNO);

	return pDOUT;
}



