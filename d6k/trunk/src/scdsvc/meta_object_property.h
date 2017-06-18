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

#include <string>
#include "fesapi/fescnst.h"

class CBaseObject
{
public:
	CBaseObject()
	{

	}
	~CBaseObject()
	{

	}
private:
	//! 节点名称
	std::string m_szNodeTagName;
	//! 测点名称
	std::string  m_nTagName;
	//! 属性名称
	std::string  m_nFiledName;


	//! 节点
	int32u m_nNodeOccNo;
	int32u m_nIddType;
	int32u m_nTagOccNo;
	int32u m_nFiledId;

};

class CObjectProperty
{
public:
	CObjectProperty()
	{

	}
	virtual ~CObjectProperty()
	{

	}
public:

private:
	//! 属性名称
	std::string m_szName;
	//! 属性的数据类型
	int32u  m_nDataType;
	//! 应用层是否可以修改值？
	bool m_bIsReadOnly;


};


class CStaticProperty : public CObjectProperty
{
public:
	CStaticProperty();
	virtual ~CStaticProperty();
private:
	//! 属性测值  --- ？
	std::string m_szValue;
};

/*! \class  CDynamicProperty
*   \brief 动态属性 */
class CDynamicProperty : public CObjectProperty
{
public:
    CDynamicProperty();
	virtual ~CDynamicProperty();
public:



private:
	
	//! 关联的测点
	CBaseObject  m_LinkdObject;
 
};

/** @}*/

#endif // META_OBJECT_H
