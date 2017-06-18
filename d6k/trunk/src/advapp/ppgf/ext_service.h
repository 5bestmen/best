/*! @file ext_service.h
<PRE>
********************************************************************************
模块名       :
文件名       :  ext_service.h
文件实现功能 :  扩展服务入口
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   扩展服务入口
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

	//!  逆变器
	std::vector<std::shared_ptr<CInverter>>  m_arrInverters;
 
};


#endif // EXT_SVC_H



/** @}*/

