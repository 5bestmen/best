

#include "data_def.h"
#include "kernel.h"

/*! \fn CKernel::CreateTransformLinearTable 
********************************************************************************************************* 
** \brief CKernel::CreateTransformLinearTable 
** \details 
** \param pParentElement �����ļ���TransformLinear�����ڵ�
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15�� 
** \note ����TRANSFORM_LINEAR�ڴ��
********************************************************************************************************/
bool CKernel::CreateTransformLinearTable(TiXmlElement* pParentElement)
{
	int nCount = GetElementChildCount(pParentElement,"l");
	if(nCount <=0)
	{
		return false;
	}

	if (!m_poolTransformLinear.CreateArray(SMKEY_MEM_TRANSFORM_LINEAR, nCount))
	{
		return false;
	}

	TiXmlNode *pChild;
	
	for (pChild = pParentElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string strChildName = pChild->Value();

		if(!Utils::IsEqualIgnoreCase(strChildName,"l"))
			continue;

		//��ȡTransform Linear������Ϣ
		TiXmlElement* pTransLinearElement = pChild->ToElement();           

		//��ȡ����ֵ
		int nOccNO = 0;
		int nNameOccNo = 0;
		fp64 fMaxRaw = 0;
		fp64 fMinRaw = 0;
		fp64 fMaxScale = 0;
		fp64 fMinScale = 0;

		int nInverse = 0;
		int nEnableFactor=0;
		fp64  fGain=0;
		fp64  fOffset=0;
		
		pTransLinearElement->QueryIntAttribute("OccNo", &nOccNO);
		pTransLinearElement->QueryIntAttribute("NameOccNo", &nNameOccNo);
		pTransLinearElement->QueryDoubleAttribute("MaxRaw", &fMaxRaw);
		pTransLinearElement->QueryDoubleAttribute("MinRaw", &fMinRaw);
		pTransLinearElement->QueryDoubleAttribute("MaxScale", &fMaxScale);
		pTransLinearElement->QueryDoubleAttribute("MinScale", &fMinScale);
		pTransLinearElement->QueryIntAttribute("Inverse", &nInverse);
		pTransLinearElement->QueryIntAttribute("EnableFactor", &nEnableFactor);
		pTransLinearElement->QueryDoubleAttribute("Gain", &fGain);
		pTransLinearElement->QueryDoubleAttribute("Offset", &fOffset);

		TRANSFORM_LINEAR* pTransformLinear = GetTransformLinear(nOccNO);
		if(pTransformLinear==NULL)
		{
			return false;
		}

		//��ʼ��TRANSFORM_LINEAR����
		pTransformLinear->OccNo = nOccNO;
		pTransformLinear->NameOccNo = nNameOccNo;
		pTransformLinear->MaxRaw = fMaxRaw;
		pTransformLinear->MinRaw = fMinRaw;
		pTransformLinear->MaxScale = fMaxScale;
		pTransformLinear->MinScale = fMinScale;
		pTransformLinear->Inverse = nInverse;
		pTransformLinear->EnableFactor = nEnableFactor;
		pTransformLinear->Gain = fGain;
		pTransformLinear->Offset = fOffset;
	}

	return true;
}

/*! \fn CKernel::OpenTransformLinearTable
********************************************************************************************************* 
** \brief CKernel::OpenTransformLinearTable 
** \details 
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15�� 
** \note ��TRANSFORM_LINEAR�ڴ��
********************************************************************************************************/
bool CKernel::OpenTransformLinearTable()
{
	if (!m_poolTransformLinear.OpenArray(SMKEY_MEM_TRANSFORM_LINEAR))
		return false;

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::DestroyTransformLinearTable 
** \details 
** \return void 
** \date 2015��9��15�� 
** \note �ر�TransformLinear�ڴ��
********************************************************************************************************/
void CKernel::DestroyTransformLinearTable()
{
	m_poolTransformLinear.CloseArray();
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetTransformLinear
** \details 
** \param occNO 
** \return TRANSFORM_LINEAR* 
** \date 2015��9��15�� 
** \note ����TRANSFORM_LINEAR��OCCNO��ȡTRANSFORM_LINEAR����ָ��
********************************************************************************************************/
TRANSFORM_LINEAR* CKernel::GetTransformLinear(int32u occNO)
{
	int nCount = m_poolTransformLinear.GetArraySize();
	occNO = occNO -1;//occNo��1��ʼ
	if(occNO<0 || occNO>=nCount)
		return NULL;

	TRANSFORM_LINEAR* pTransformLinear = m_poolTransformLinear.GetObject(occNO);

	return pTransformLinear;
}


fp64 CKernel::LinearTransform(int32u nTransformOccNo, fp64 fValue)
{
	TRANSFORM_LINEAR* pTransformLinear = GetTransformLinear(nTransformOccNo);
	if (pTransformLinear == NULL)
		return fValue;


	fp64 fTransValue = fValue;

	return fTransValue;
}
