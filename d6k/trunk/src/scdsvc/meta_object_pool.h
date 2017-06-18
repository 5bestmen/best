/*! @file meta_object.h
<PRE>
********************************************************************************
模块名       :  对本节点的所有tagname进行管理
文件名       :  tagname.h
文件实现功能 :
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  tagname按类型存于共享内存中
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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
*   \brief 对象池 */
class CMetaObjectPool
{
public:
	CMetaObjectPool();
	~CMetaObjectPool();
public:
	// 根据occno访问一个对象
	CMetaObject * GetMetaObject(int32u nOccNo);
	// 根据tagname获取对象
	CMetaObject * GetMetaObject( const std::string & szTagName);


private:
	//! 所有的对象（包含子对象）扁平化布局
	std::vector<CMetaObject*> m_arrRawObjects;
	 
};

/** @}*/

#endif // META_OBJECT_POOL_H
