/*! @file graph_serializer_factory.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       : graph_serializer_factory.cpp
�ļ�ʵ�ֹ��� : ͼ���ļ����л�������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼ���ļ����л�������
*  @author  LiJin
*  @version 1.0
*  @date    2016.08.28
*******************************************************************************/

#include "graph_file.h"
#include "graph_serializer_factory.h"

#include "graph_serializer.h"
#include "xml_serializer.h"
#include <QDebug>


CGraphSerializerFactory::CGraphSerializerFactory()
{

}

CGraphSerializerFactory::~CGraphSerializerFactory()
{

}

CGraphSerializer * CGraphSerializerFactory::CreateSerializer( unsigned int nStorageType)
{	
	if (nStorageType == XML_TYPE)
	{
		return new CGraphXmlSerializer;
	}
	else if (nStorageType == BIN_TYPE)
	{
		return nullptr;
	}

	return nullptr;
}


void CGraphSerializerFactory::DestroySerializer(CGraphSerializer *pSerializer)
{
	Q_ASSERT(pSerializer);
	if (pSerializer)
	{
		delete pSerializer;
		pSerializer = nullptr;
	}
}

/** @}*/

