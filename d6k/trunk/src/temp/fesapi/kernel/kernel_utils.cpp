
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
** \date 2015��9��15�� 
** \note ��ȡXML�ڵ��һ���ӽڵ���Ŀ
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

