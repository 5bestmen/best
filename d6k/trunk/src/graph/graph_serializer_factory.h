/*! @file graph_serializer_factory.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graph_serializer_factory.h
�ļ�ʵ�ֹ��� :  ͼ���ļ����л�������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ����QT�ĳ��󹤳����ģʽ��Ϊ���Ķ������л���ʽԤ���ӿ�
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼ���ļ����л�����
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
		XML_TYPE, //!< ����Ϊxml
		BIN_TYPE, //!< ����ΪBIN
	};
	static CGraphSerializer * CreateSerializer( unsigned int nStorageType);

	static void DestroySerializer(CGraphSerializer *pSerializer);

};



#endif // GRAPH_SERIALIZER_FACTORY_H

/** @}*/

