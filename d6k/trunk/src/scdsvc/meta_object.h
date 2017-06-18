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


#ifndef META_OBJECT_H
#define META_OBJECT_H

#include "datatypes.h"
#include <vector>
#include <memory>
#include <string>
#include <QUuid>

class CStaticProperty;
class CDynamicProperty;

class CMetaObject
{
public:
    CMetaObject();
	~CMetaObject();
public:


private:

	int32u m_nOccNo;
	std::string m_szTagName;

	
	//! 是否启用设备级报警
	bool m_bEnableObjectAlarm;


	std::vector< CStaticProperty*> m_arrStaticProps;
	std::vector< CDynamicProperty*> m_arrDynamicProps;

	std::vector< CMetaObject*> m_arrSubObjects;

	//! 父节点，如果为空即为根节点
	CMetaObject *m_pParentObject;

	//! 对象的原始模型
	QUuid  m_ModelID;
};

/** @}*/

#endif // META_OBJECT_H
