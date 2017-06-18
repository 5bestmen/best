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

	
	//! �Ƿ������豸������
	bool m_bEnableObjectAlarm;


	std::vector< CStaticProperty*> m_arrStaticProps;
	std::vector< CDynamicProperty*> m_arrDynamicProps;

	std::vector< CMetaObject*> m_arrSubObjects;

	//! ���ڵ㣬���Ϊ�ռ�Ϊ���ڵ�
	CMetaObject *m_pParentObject;

	//! �����ԭʼģ��
	QUuid  m_ModelID;
};

/** @}*/

#endif // META_OBJECT_H
