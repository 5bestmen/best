
#include "kernel.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateChannelTable 
** \details 
** \param pParentElement 
** \return bool 
** \date 2015年9月15日 
** \note 创建通道内存库
********************************************************************************************************/
bool CKernel::CreateChannelTable(TiXmlElement* pParentElement)
{
	int nCount = GetElementChildCount(pParentElement,"c");
	if(nCount <=0)
	{
		return false;
	}

	if(!m_poolChannel.CreateArray(SMKEY_MEM_CHANNEL,nCount))
	{
		return false;
	}


	TiXmlNode *pChild;
	int nChannelIndex = 0;
	for (pChild = pParentElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string strChildName = pChild->Value();

		if (!Utils::IsEqualIgnoreCase(strChildName, "c"))//不是a(比如是注释等)
			continue;

		//获取channel配置信息
		TiXmlElement* pChannelElement = pChild->ToElement();           

		//获取属性值
		int nOccNo =0;
		std::string strChannelName("");
		std::string strDriverName("");

		pChannelElement->QueryIntAttribute("OccNo",&nOccNo);           
		pChannelElement->QueryStringAttribute("DriverName",&strDriverName);
		pChannelElement->QueryStringAttribute("ChannelName",&strChannelName);

		CHANNEL* pChannel = GetChannel(nOccNo);

		if(pChannel==NULL)
		{
			return false;
		}

		//初始化channel对象
		pChannel->OccNo = nOccNo;

		if (strChannelName.length()>CHANNEL::MAX_NAME_LENGTH)
			strChannelName = strChannelName.substr(0, CHANNEL::MAX_NAME_LENGTH);
		sprintf(pChannel->ChannelName,"%s",strChannelName.c_str());

		if (strDriverName.length()>CHANNEL::MAX_NAME_LENGTH)
			strDriverName = strDriverName.substr(0, CHANNEL::MAX_NAME_LENGTH);
		sprintf(pChannel->DriverName,"%s",strDriverName.c_str());

	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::OpenChannelTable 
** \details 
** \return bool 
** \date 2015年9月15日 
** \note 打开通道内存库
********************************************************************************************************/
bool CKernel::OpenChannelTable()
{
	if(!m_poolChannel.OpenArray(SMKEY_MEM_CHANNEL))
		return false;

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::DestroyChannelTable 
** \details 
** \return void 
** \date 2015年9月15日 
** \note 关闭通道内存库
********************************************************************************************************/
void CKernel::DestroyChannelTable()
{
	m_poolChannel.CloseArray();
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetChannel 
** \details 
** \param occNo 
** \return CHANNEL* 
** \date 2015年9月15日 
** \note 根据通道的OCCNO获取通道数据指针
********************************************************************************************************/
CHANNEL* CKernel::GetChannel(int32u occNo)
{
	int nCount = m_poolChannel.GetArraySize();
	occNo = occNo -1;//occNo从1开始
	if(occNo<0 || occNo>=nCount)
		return NULL;

	return m_poolChannel.GetObject(occNo);
}


bool CKernel::SetChannelCoreState(int32u nChannelOccNo,int8u nState)
{
	CHANNEL* pChannel = GetChannel(nChannelOccNo);
	if(pChannel==NULL)
		return false;

	pChannel->StateCore = nState;
	
	return true;
}

bool CKernel::SetChannelDriverState(int32u nChannelOccNo,int8u nState)
{
	CHANNEL* pChannel = GetChannel(nChannelOccNo);
	if(pChannel==NULL)
		return false;

	pChannel->StateDriver = nState;

	return true;
}

int8u CKernel::GetChannelCoreState(int32u nChannelOccNo)
{
	CHANNEL* pChannel = GetChannel(nChannelOccNo);
	if(pChannel==NULL)
		return DRIVER_STATE_UNKNOWN;

	return pChannel->StateCore;
}

int8u CKernel::GetChannelDriverState(int32u nChannelOccNo)
{
	CHANNEL* pChannel = GetChannel(nChannelOccNo);
	if(pChannel==NULL)
		return DRIVER_STATE_UNKNOWN;

	return pChannel->StateDriver;
}

