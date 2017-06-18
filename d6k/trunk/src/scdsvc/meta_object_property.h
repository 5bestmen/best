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
	//! �ڵ�����
	std::string m_szNodeTagName;
	//! �������
	std::string  m_nTagName;
	//! ��������
	std::string  m_nFiledName;


	//! �ڵ�
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
	//! ��������
	std::string m_szName;
	//! ���Ե���������
	int32u  m_nDataType;
	//! Ӧ�ò��Ƿ�����޸�ֵ��
	bool m_bIsReadOnly;


};


class CStaticProperty : public CObjectProperty
{
public:
	CStaticProperty();
	virtual ~CStaticProperty();
private:
	//! ���Բ�ֵ  --- ��
	std::string m_szValue;
};

/*! \class  CDynamicProperty
*   \brief ��̬���� */
class CDynamicProperty : public CObjectProperty
{
public:
    CDynamicProperty();
	virtual ~CDynamicProperty();
public:



private:
	
	//! �����Ĳ��
	CBaseObject  m_LinkdObject;
 
};

/** @}*/

#endif // META_OBJECT_H
