/*! @file meta_object.h
<PRE>
********************************************************************************
ģ����       :  �Ա��ڵ������tagname���й���
�ļ���       :  tagname.h
�ļ�ʵ�ֹ��� :
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  tagname�����ʹ��ڹ����ڴ���
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author LiJin
*  @version 1.0
*  @date    2016.12.08
*/


#ifndef META_OBJECT_POOL_H
#define META_OBJECT_POOL_H

#include "datatypes.h"
#include <vector>
#include <memory>
#include <string>
#include <QUuid>
#include <unordered_map>
 

class CMetaObject;


/*! \class  CMetaObjectPool
*   \brief ����� */
class CMetaObjectPool
{
public:
	CMetaObjectPool();
	~CMetaObjectPool();
public:
	// ����occno����һ������
	CMetaObject * GetMetaObject(int32u nOccNo);
	// ����tagname��ȡ����
	CMetaObject * GetMetaObject( const std::string & szTagName);


private:
	//! ���еĶ��󣨰����Ӷ��󣩱�ƽ������
	std::vector<CMetaObject*> m_arrRawObjects;
	 
};

/** @}*/

#endif // META_OBJECT_POOL_H
