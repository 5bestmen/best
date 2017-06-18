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


#ifndef META_OBJECT_MODEL_H
#define META_OBJECT_MODEL_H

#include <QUuid>
  
/*! \class  CObjectModel
*   \brief ����ģ�� */
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
	// �豸��Ψһ���ͱ�ʶ
	QUuid  m_ModelID;

 
};

/** @}*/

#endif // META_OBJECT_H
