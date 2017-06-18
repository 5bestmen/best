/*! @file dll_lib.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dll_lib.h
�ļ�ʵ�ֹ��� :  �����������һ����������ж������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���������
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.24
*******************************************************************************/
#ifndef DLL_LIB_H
#define DLL_LIB_H


#include "datatypes.h"

#include <vector>
#include <memory>
   

class CExtService;

class CAppMain
{
public:
	CAppMain(void);
	~CAppMain(void);
public:
	//! ע�����
	bool RegisterService(int32u nOccNo);
	//! ���� 
	bool Start(int32u nOccNo);
	//! ֹͣ 
	bool Stop(int32u nOccNo); 
protected:
 
private:
	 
	//!  s
 	std::vector<std::shared_ptr<CExtService>>  m_arrServices;
 
};


#endif // DLL_LIB_H

/** @}*/


