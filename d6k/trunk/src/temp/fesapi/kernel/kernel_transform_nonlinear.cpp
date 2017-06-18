

#include "data_def.h"
#include "kernel.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateTransformNonLinearTable 
** \details 
** \param pParentElement �����ļ���TransformNonLinear�����ڵ�
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15�� 
** \note ����TransformNonLinear�ڴ��
********************************************************************************************************/
bool CKernel::CreateTransformNonLinearTable(TiXmlElement* pParentElement)
{
	int nCount = GetElementChildCount(pParentElement,"nl");
	if(nCount <=0)
	{
		return false;
	}

	if (!m_poolTransformNonLinear.CreateArray(SMKEY_MEM_TRANSFORM_NONLINEAR, nCount))
	{
		return false;
	}

	TiXmlNode *pChild;
	
	for (pChild = pParentElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string strChildName = pChild->Value();

		if(!Utils::IsEqualIgnoreCase(strChildName,"nl"))
			continue;

		//��ȡAIN������Ϣ
		TiXmlElement* pTransformNonLinearElement = pChild->ToElement();           

		//��ȡ����ֵ
		int nOccNO = 0;
		int nNameOccNo = 0;
		int nAtomCount = 0;

		pTransformNonLinearElement->QueryIntAttribute("OccNo", &nOccNO);
		pTransformNonLinearElement->QueryIntAttribute("NameOccNo", &nNameOccNo);
		pTransformNonLinearElement->QueryIntAttribute("Count", &nAtomCount);

		TRANSFORM_NONLINEAR* pTransformNonLinear = GetTransformNonLinear(nOccNO);
		if (pTransformNonLinear == NULL)
		{
			return false;
		}

		//��ʼ��AIN����
		pTransformNonLinear->OccNo = nOccNO;
		pTransformNonLinear->NameOccNo = nNameOccNo;
		pTransformNonLinear->DataCount = nAtomCount;

		TiXmlNode *pSubChild;
		int nIndex = 0;
		for (pSubChild = pTransformNonLinearElement->FirstChild(); pSubChild != 0; pSubChild = pSubChild->NextSibling())
		{
			std::string strChildName = pSubChild->Value();

			if (!Utils::IsEqualIgnoreCase(strChildName, "p"))
				continue;

			//��ȡ�����Ե�������Ϣ
			TiXmlElement* pTransformNonLinearPointElement = pSubChild->ToElement();

			fp64 fRawValue   = 0;
			fp64 fScaleValue = 0;

			pTransformNonLinearPointElement->QueryDoubleAttribute("RawValue", &fRawValue);
			pTransformNonLinearPointElement->QueryDoubleAttribute("ScaleValue", &fScaleValue);

			if (nIndex >= TRANSFORM_NONLINEAR::MAX_NON_LINEAR_NUM)
				return false;

			pTransformNonLinear->NonLinearData[nIndex].RawValue = fRawValue;
			pTransformNonLinear->NonLinearData[nIndex].ScaleValue = fScaleValue;
			nIndex++;
		}
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::OpenTransformNonLinearTable 
** \details 
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15�� 
** \note ��TransformNonLinear�ڴ��
********************************************************************************************************/
bool CKernel::OpenTransformNonLinearTable()
{
	if (!m_poolTransformNonLinear.OpenArray(SMKEY_MEM_TRANSFORM_NONLINEAR))
		return false;

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::DestroyTransformNonLinearTable 
** \details 
** \return void 
** \date 2015��9��15�� 
** \note �ر�TransformNonLinear�ڴ��
********************************************************************************************************/
void CKernel::DestroyTransformNonLinearTable()
{
	m_poolTransformNonLinear.CloseArray();
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetTransformNonLinear 
** \details 
** \param occNO 
** \return TRANSFORM_NONLINEAR* 
** \date 2015��9��15�� 
** \note ����TRANSFORM_NONLINEAR��OCCNO��ȡTRANSFORM_NONLINEAR����ָ��
********************************************************************************************************/
TRANSFORM_NONLINEAR* CKernel::GetTransformNonLinear(int32u occNO)
{
	int nCount = m_poolTransformNonLinear.GetArraySize();
	occNO = occNO -1;//occNo��1��ʼ
	if(occNO<0 || occNO>=nCount)
		return NULL;

	TRANSFORM_NONLINEAR* pTransformNonLinear = m_poolTransformNonLinear.GetObject(occNO);

	return pTransformNonLinear;
}

fp64 CKernel::NonLinearTransform(int32u nTransformOccNo, fp64 fValue)
{
	TRANSFORM_NONLINEAR* pTransformNonLinear = GetTransformNonLinear(nTransformOccNo);
	if (pTransformNonLinear == NULL)
		return fValue;

	TRANSFORM_NONLINEAR::LINEAR_ATOM dataLeft, dataRight;

	for (int i = 0; i < pTransformNonLinear->DataCount; i++)
	{
		dataLeft = pTransformNonLinear->NonLinearData[i];

		if (i + 1 >= pTransformNonLinear->DataCount)
			return fValue;
		else
		{
			dataRight = pTransformNonLinear->NonLinearData[i + 1];
		}
		
		//�˵�ֵ
		if (dataLeft.RawValue == fValue)
			return dataLeft.ScaleValue;

		if (dataRight.RawValue == fValue)
			return dataRight.ScaleValue;

		//�ҵ�����
		if (dataLeft.RawValue< fValue  && dataRight.RawValue > fValue)
		{
			fp64 k = (dataRight.ScaleValue - dataLeft.ScaleValue) / (dataRight.RawValue - dataLeft.RawValue);
			fp64 b = dataLeft.ScaleValue - k * dataLeft.RawValue;
			return k * fValue + b;
		}
	}

	return fValue;
}


