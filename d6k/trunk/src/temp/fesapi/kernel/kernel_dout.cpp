
#include "utils/misc.h"

#include "kernel.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateDOUTTable 
** \details 
** \param pParentElement �����ļ���DOUT�����ڵ�
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15�� 
** \note ����DOUT�ڴ��
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

		//��ȡDOUT������Ϣ
		TiXmlElement* pDOUTElement = pChild->ToElement();           

		//��ȡ����ֵ
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

		//��ʼ��DOUT����
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
** \date 2015��9��15�� 
** \note ��DOUT�ڴ��
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
** \date 2015��9��15�� 
** \note �ر�DOUT�ڴ��
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
** \date 2015��9��15�� 
** \note ����DOUT��OCCNO��ȡDOUT���ݽṹָ��
********************************************************************************************************/
DOUT* CKernel::GetDOUT(int32u occNO)
{
	int nCount = m_poolDOUT.GetArraySize();
	occNO = occNO -1;//occNo��1��ʼ
	if(occNO<0 || occNO>=nCount)
		return NULL;

	DOUT* pDOUT = m_poolDOUT.GetObject(occNO);

	return pDOUT;
}



