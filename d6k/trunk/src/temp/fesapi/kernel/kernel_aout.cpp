
#include "utils/misc.h"

#include "kernel.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateAOUTTable 
** \details 
** \param pParentElement �����ļ���AOUT�����ڵ�
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15�� 
** \note ����AOUT�ڴ��
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

		//��ȡAOUT������Ϣ
		TiXmlElement* pAOUTElement = pChild->ToElement();           

		//��ȡ����ֵ
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
		
		AOUT* pAOUT = m_poolAOUT.GetObject(nOccNO-1);//occNo��1��ʼ
		if(pAOUT==NULL)
		{
			return false;
		}

		//��ʼ��AOUT����
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
** \date 2015��9��15�� 
** \note ��AOUT�ڴ��
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
** \date 2015��9��15�� 
** \note �ر�AOUT�ڴ��
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
** \date 2015��9��15�� 
** \note ����AOUT��OCCNO��ȡAOUT���ݽṹָ��
********************************************************************************************************/
AOUT* CKernel::GetAOUT(int32u occNO)
{
	int nCount = m_poolAOUT.GetArraySize();
	occNO = occNO -1;//occNo��1��ʼ
	if(occNO<0 || occNO>=nCount)
		return NULL;

	AOUT* pAOUT = m_poolAOUT.GetObject(occNO);

	return pAOUT;
}



