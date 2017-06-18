/*! @file graph_serializer_factory.cpp
<PRE>
********************************************************************************
模块名       :
文件名       : graph_serializer_factory.cpp
文件实现功能 : 图形文件序列化工厂类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图形文件序列化工厂类
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

