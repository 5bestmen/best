/*! @file subscription_list.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  subscription_list.h
�ļ�ʵ�ֹ��� :  ���ķ����б�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ���ķ���ά�������嵥���������嵥��ʱɨ�����ݿ⣬���ɶ��ı��ģ�Ȼ��
                ���������߷���ʱ���͸ñ��ģ����ǵ�ʵʱ���ⱨ�Ĳ�������ʽ���档
				���ң����Բ��ö��ģ�ң�ž�����Ҫ���ö���ģʽ��
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���ķ���
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.03
*******************************************************************************/
#ifndef SUB_LIST_MODULE_H
#define SUB_LIST_MODULE_H

#include "datatypes.h"
#include "base_module.h"
#include <memory>
#include <set>

class CSubScriptionList
{
public:
	CSubScriptionList();
	~CSubScriptionList();
public:

private:
	//! �����ĵĽڵ� 
	int32u m_nProviderOccNo;

	//! �����ߣ�˭�������������
	int32u m_nSubscriberOccNo;

	//! ���ĵ�ģ�����嵥
	std::set<int32u> m_setAins;
	std::set<int32u> m_setDins; 

};
   

#endif // SUB_LIST_MODULE_H


/** @}*/

