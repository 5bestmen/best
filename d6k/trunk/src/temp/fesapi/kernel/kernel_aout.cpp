
#include "utils/misc.h"

#include "kernel.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateAOUTTable 
** \details 
** \param pParentElement 配置文件中AOUT区根节点
** \return bool true:成功，false:失败
** \date 2015年9月15日 
** \note 创建AOUT内存库
********************************************************************************************************/
bool CKernel::CreateAOUTTable(TiXmlElement* pParentElement)
{
	int nCount = GetElementChildCount(pParentElement,"ao");
	if(nCount <=0)
	{
		return false;
	}

	if(!m_poolAOUT.CreateArray(SMKEY_MEM_AOUT,nCount))
	{
		return false;
	}

	TiXmlNode *pChild;

	for (pChild = pParentElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string strChildName = pChild->Value();

		if(!Utils::IsEqualIgnoreCase(strChildName,"ao"))
			continue;

		//获取AOUT配置信息
		TiXmlElement* pAOUTElement = pChild->ToElement();           

		//获取属性值
		int nOccNO = 0;
		int nBlockOccNO = 0;
		int nNameOccNO = 0;
		int nNodeOccNO = 0;
		int nChannelOccNo = 0;
		int nDeviceOccNO = 0;

		pAOUTElement->QueryIntAttribute("OccNo", &nOccNO);
		pAOUTElement->QueryIntAttribute("BlockOccNo", &nBlockOccNO);
		pAOUTElement->QueryIntAttribute("NameOccNo", &nNameOccNO);
		pAOUTElement->QueryIntAttribute("NodeOccNo", &nNodeOccNO);
		pAOUTElement->QueryIntAttribute("ChannelOccNo", &nChannelOccNo);
		pAOUTElement->QueryIntAttribute("DeviceOccNo", &nDeviceOccNO);
		
		AOUT* pAOUT = m_poolAOUT.GetObject(nOccNO-1);//occNo从1开始
		if(pAOUT==NULL)
		{
			return false;
		}

		//初始化AOUT对象
		pAOUT->OccNo = nOccNO;
		pAOUT->BlockNo = nBlockOccNO;
		pAOUT->NameOccNo = nNameOccNO;
		pAOUT->NodeOccNo = nNodeOccNO;
		pAOUT->ChannelOccNo = nChannelOccNo;
		pAOUT->DeviceOccNo = nDeviceOccNO;
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::OpenAOUTTable 
** \details 
** \return bool 
** \date 2015年9月15日 
** \note 打开AOUT内存库
********************************************************************************************************/
bool CKernel::OpenAOUTTable()
{
	if(!m_poolAOUT.OpenArray(SMKEY_MEM_AOUT))
		return false;

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::DestroyAOUTTable 
** \details 
** \return void 
** \date 2015年9月15日 
** \note 关闭AOUT内存库
********************************************************************************************************/
void CKernel::DestroyAOUTTable()
{
	m_poolAOUT.CloseArray();
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetAOUT 
** \details 
** \param occNO 
** \return AOUT* 
** \date 2015年9月15日 
** \note 根据AOUT的OCCNO获取AOUT数据结构指针
********************************************************************************************************/
AOUT* CKernel::GetAOUT(int32u occNO)
{
	int nCount = m_poolAOUT.GetArraySize();
	occNO = occNO -1;//occNo从1开始
	if(occNO<0 || occNO>=nCount)
		return NULL;

	AOUT* pAOUT = m_poolAOUT.GetObject(occNO);

	return pAOUT;
}



