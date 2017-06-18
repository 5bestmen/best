
#include "utils/misc.h"

#include "kernel.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateDINTable 
** \details 
** \param pParentElement �����ļ���DIN�����ڵ�
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15�� 
** \note ����DIN�ڴ��
********************************************************************************************************/
bool CKernel::CreateDINTable(TiXmlElement* pParentElement)
{
	int nCount = GetElementChildCount(pParentElement,"di");
	if(nCount <=0)
	{
		return false;
	}

	if(!m_poolDIN.CreateArray(SMKEY_MEM_DIN,nCount))
	{
		return false;
	}

	TiXmlNode *pChild;

	for (pChild = pParentElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string strChildName = pChild->Value();

		if(!Utils::IsEqualIgnoreCase(strChildName,"di"))
			continue;

		//��ȡDIN������Ϣ
		TiXmlElement* pDINElement = pChild->ToElement();           

		//��ȡ����ֵ
		int nOccNO = 0;
		int nChannelOccNo = 0;
		int nDeviceOccNO = 0;
		int nBlockOccNO = 0;
		std::string strAddress;
		pDINElement->QueryIntAttribute("OccNo",&nOccNO);           
		pDINElement->QueryIntAttribute("ChannelOccNo",&nChannelOccNo);           
		pDINElement->QueryIntAttribute("DeviceOccNo",&nDeviceOccNO);
		pDINElement->QueryIntAttribute("BlockOccNo",&nBlockOccNO);
		pDINElement->QueryStringAttribute("Address", &strAddress);

		DIN* pDIN = GetDIN(nOccNO);

		if(pDIN==NULL)
		{
			return false;
		}

		//��ʼ��DIN����
		pDIN->OccNo = nOccNO;
		pDIN->ChannelOccNo = nChannelOccNo;
		pDIN->DeviceOccNo = nDeviceOccNO;
		pDIN->BlockNo = nBlockOccNO;

		if (strAddress.length() > MAX_ADDRESS_LENGTH)
			strAddress = strAddress.substr(0, MAX_ADDRESS_LENGTH);
		sprintf(pDIN->Address, "%s", strAddress.c_str());
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::OpenDINTable 
** \details 
** \return bool 
** \date 2015��9��15�� 
** \note ��AIN�ڴ��
********************************************************************************************************/
bool CKernel::OpenDINTable()
{
	if(!m_poolDIN.OpenArray(SMKEY_MEM_DIN))
		return false;

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::DestroyDINTable 
** \details 
** \return void 
** \date 2015��9��15�� 
** \note �ر�DIN�ڴ��
********************************************************************************************************/
void CKernel::DestroyDINTable()
{
	m_poolDIN.CloseArray();
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetDIN 
** \details 
** \param occNO 
** \return DIN* 
** \date 2015��9��15�� 
** \note ����DIN��OCCNO��ȡDIN���ݽṹָ��
********************************************************************************************************/
DIN* CKernel::GetDIN(int32u occNO)
{
	int nCount = m_poolDIN.GetArraySize();
	occNO = occNO -1;//occNo��1��ʼ
	if(occNO<0 || occNO>=nCount)
		return NULL;

	DIN* pDIN = m_poolDIN.GetObject(occNO);

	return pDIN;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::SetDINValue 
** \details 
** \param occNO 
** \param nValue 
** \param nQuality 
** \return bool 
** \date 2015��9��15�� 
** \note ����DIN��OCCNO������ֵ��Ʒ��
********************************************************************************************************/
bool CKernel::SetDINValue(int32u nChannelOccNo, int32u occNO, int8u nValue, int8u nQuality)
{
	DIN* pDIN = GetDIN(occNO);
	if(pDIN == NULL)
		return false;

	pDIN->RawValue = nValue;
	pDIN->Quality = nQuality;

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetDINValue 
** \details 
** \param occNO 
** \return int8u 
** \date 2015��9��15�� 
** \note ����AIN��OCCNO��ȡ��ֵ
********************************************************************************************************/
int8u CKernel::GetDINValue(int32u occNO)
{
	DIN* pDIN = GetDIN(occNO);
	if(pDIN == NULL)
		return 0;

	return pDIN->RawValue;
}


int32u CKernel::GetDinCount(int32u nChannelOccNo, int32u nDeviceOccNo)
{
	int nDinCount = 0;
	int nCount = m_poolDIN.GetArraySize();

	for (int i = 0; i < nCount; i++)
	{
		DIN* pDIN = m_poolDIN.GetObject(i);
		if (pDIN == NULL)
			continue;

		if ((pDIN->ChannelOccNo == nChannelOccNo) && (pDIN->DeviceOccNo == nDeviceOccNo))
			nDinCount++;
	}

	return nDinCount;
}

void CKernel::GetDinOccNos(int32u nChannelOccNo, int32u nDeviceOccNo, std::vector<int32u>& vecDinOccNos)
{
	int32u nCount = m_poolDIN.GetArraySize();
	for (int i = 0; i < nCount; i++)
	{
		DIN* pDIN = m_poolDIN.GetObject(i);
		if (pDIN == NULL)
			continue;

		if ((pDIN->ChannelOccNo == nChannelOccNo) && (pDIN->DeviceOccNo == nDeviceOccNo))
		{
			vecDinOccNos.push_back(pDIN->OccNo);
		}
	}
}

int32u CKernel::GetDinAddress(int32u nDinOccNo)
{
	DIN* pDIN = GetDIN(nDinOccNo);
	if (pDIN == NULL)
		return 0;

	std::string strAddress = pDIN->Address;

	//return atoi((char*)strAddress.c_str());
	return Utils::HexToInt((char*)strAddress.c_str());
}
