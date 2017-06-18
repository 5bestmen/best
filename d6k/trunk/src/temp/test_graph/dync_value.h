/*! @file dync_value.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dync_value.h
�ļ�ʵ�ֹ��� :  ��̬��ֵ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ��̬��ֵ����
*  @author LiJin
*  @version 1.0
*  @date    2016.02.19
*******************************************************************************/
#ifndef DYNC_VALUE_DATA_H
#define DYNC_VALUE_DATA_H

#include "base_dync.h"
#include <qString>
#include <array>
#include <memory>
  
class CBaseWidget;  

/*! \class CDyncValueData dync_value.h  "dync_value.h"
*  \brief ��̬��ֵ�仯
*  \details
*/
class CDyncValueData :public CBaseDyncData
{
public:
	CDyncValueData();
	const CDyncValueData& operator=(const CDyncValueData& src);
	virtual ~CDyncValueData();
public:
	enum VAL_FORMAT
	{
		X = 0,
		XX,
		XXX,
		XXXX,
		XXXXX,
		X_X, //!< һλС��
		X_XX,
		X_XXX,
		X_XXXX,
		X_XXXXX
	};
public:
	//! ��ʾ�ĸ�ʽ
	VAL_FORMAT m_nFormat;
public:
//	virtual void SaveXml(JWXml::CXmlNodePtr  pDyncPtr) const;
//	virtual bool LoadXml(JWXml::CXmlNodePtr  pDyncPtr);

	// ��̬���ֵĴ���
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// ���ƿ�¡  ����ռ䲢�����Լ�
	virtual CBaseDyncData *CloneDyncData();
};

 

#endif // DYNC_VALUE_DATA_H


/** @}*/
