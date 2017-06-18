

#include "data_def.h"
#include "kernel.h"

/*! \fn CKernel::CreateTransformLinearTable 
********************************************************************************************************* 
** \brief CKernel::CreateTransformLinearTable 
** \details 
** \param pParentElement 配置文件中TransformLinear区根节点
** \return bool true:成功，false:失败
** \date 2015年9月15日 
** \note 创建TRANSFORM_LINEAR内存库
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

		//获取Transform Linear配置信息
		TiXmlElement* pTransLinearElement = pChild->ToElement();           

		//获取属性值
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

		//初始化TRANSFORM_LINEAR对象
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
** \return bool true:成功，false:失败
** \date 2015年9月15日 
** \note 打开TRANSFORM_LINEAR内存库
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
** \date 2015年9月15日 
** \note 关闭TransformLinear内存库
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
** \date 2015年9月15日 
** \note 根据TRANSFORM_LINEAR的OCCNO获取TRANSFORM_LINEAR数据指针
********************************************************************************************************/
TRANSFORM_LINEAR* CKernel::GetTransformLinear(int32u occNO)
{
	int nCount = m_poolTransformLinear.GetArraySize();
	occNO = occNO -1;//occNo从1开始
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
