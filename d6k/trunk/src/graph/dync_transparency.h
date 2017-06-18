/*! @file dync_transparency.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dync_transparency.h
�ļ�ʵ�ֹ��� :  ��̬͸������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��̬͸������
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.19
*******************************************************************************/
#ifndef DYNC_TRANSPARENCY_H
#define DYNC_TRANSPARENCY_H

#include "base_dync.h"
#include <qString>
#include <array>
#include <memory>
  
class CBaseWidget;  

/*! \class CDyncTransparencyData DynamicDef.h  "DynamicDef.h"
*  \brief ��̬͸��
*  \details   ��ЩԪ����̬͸��û��Ч��
*/
class CDyncTransparencyData :public CBaseDyncData
{
public:
	CDyncTransparencyData();
	const CDyncTransparencyData& operator=(const CDyncTransparencyData& src);
	//��������
	virtual void CopyBaseData(CBaseDyncData *src);

	virtual ~CDyncTransparencyData();
public:
	double GetMaxValue() const
	{
		return m_dMaxValue;
	}

	void SetMaxValue(double dMax)
	{
		m_dMaxValue = dMax;
	}

	double GetMinValue() const
	{
		return m_dMinValue;
	}

	void SetMinValue(double dMin)
	{
		m_dMinValue = dMin;
	}

private:
	//! 255��Ӧ��ֵ ��ȫ�ɼ�
	double  m_dMaxValue;
	//! 0 ��Ӧ��ֵ ��ȫ���ɼ�
	double  m_dMinValue;
public:
 	// ��̬���ֵĴ���
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// ���ƿ�¡  ����ռ䲢�����Լ�
	virtual CBaseDyncData *CloneDyncData();

	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);
};

#endif // DYNC_TRANSPARENCY_H


/** @}*/
