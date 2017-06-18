/*! @file ext_service.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  ext_service.h
�ļ�ʵ�ֹ��� :  ��չ�������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��չ�������
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.24
*******************************************************************************/
#ifndef EXT_SVC_H
#define EXT_SVC_H

#include <vector>
#include <memory>

class  CInverter;   

class CExtService
{
public:
	CExtService(void);
	~CExtService(void);
public:
 
protected:
 
private:

	//!  �����
	std::vector<std::shared_ptr<CInverter>>  m_arrInverters;
 
};


#endif // EXT_SVC_H



/** @}*/

