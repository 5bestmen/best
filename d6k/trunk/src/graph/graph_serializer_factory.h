/*! @file graph_serializer_factory.h
<PRE>
********************************************************************************
模块名       :
文件名       :  graph_serializer_factory.h
文件实现功能 :  图形文件序列化工厂类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  类似QT的抽象工厂设计模式，为今后的多种序列化方式预留接口
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图形文件序列化基类
*  @author  LiJin
*  @version 1.0
*  @date    2016.08.28
*******************************************************************************/
#ifndef GRAPH_SERIALIZER_FACTORY_H
#define GRAPH_SERIALIZER_FACTORY_H   
 

class CGraphSerializer;

class CGraphSerializerFactory
{
public:
	CGraphSerializerFactory();
	~CGraphSerializerFactory();
public:
	enum 
	{
		XML_TYPE, //!< 保存为xml
		BIN_TYPE, //!< 保存为BIN
	};
	static CGraphSerializer * CreateSerializer( unsigned int nStorageType);

	static void DestroySerializer(CGraphSerializer *pSerializer);

};



#endif // GRAPH_SERIALIZER_FACTORY_H

/** @}*/

