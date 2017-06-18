
#include <iostream>

#include "utils/misc.h"

#include "kernel.h"



/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetElementChildCount 
** \details 
** \param pElement 
** \param strTag 
** \return int 
** \date 2015年9月15日 
** \note 获取XML节点的一级子节点数目
********************************************************************************************************/
int CKernel::GetElementChildCount(TiXmlElement* pElement,std::string strTag)
{
	int nCount = 0;
	TiXmlNode *pTemp = NULL;
	for ( pTemp = pElement->FirstChild(); pTemp != NULL; pTemp = pTemp->NextSibling()) 
	{
		std::string strChildName = pTemp->Value();

		if(!Utils::IsEqualIgnoreCase(strChildName,strTag))
			continue;

		nCount++;
	}
	return nCount;
}

