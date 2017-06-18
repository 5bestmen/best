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
*  @date    2016.09.23
*/


#ifndef META_OBJECT_MODEL_H
#define META_OBJECT_MODEL_H

#include <QUuid>
  
/*! \class  CObjectModel
*   \brief 对象模型 */
class CObjectModel
{
public:
    CObjectModel();
	~CObjectModel();
public:
	const QUuid & GetUuid() const
	{
		return m_ModelID;
	}

private:
	// 设备的唯一类型标识
	QUuid  m_ModelID;

 
};

/** @}*/

#endif // META_OBJECT_H
