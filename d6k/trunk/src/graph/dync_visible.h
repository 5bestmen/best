/*! @file dync_visible.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dync_visible.h
�ļ�ʵ�ֹ��� :  ��̬�ɼ�����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��̬�ɼ�����
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.19
*******************************************************************************/
#ifndef DYNC_VISIBLE_H
#define DYNC_VISIBLE_H

#include "base_dync.h"
#include <qString>
#include <array>
#include <memory>
  
class CBaseWidget;  

 
/*! \class CDyncVisibleData  dync_visible.h  "dync_visible.h"
*  \brief ��̬�ɼ�
*  \details  ֧�ֶ�̬��˸������˸�����٣����٣�������˸��
*/
class CDyncVisibleData :public CBaseDyncData
{
public:
	CDyncVisibleData();
	const CDyncVisibleData& operator=(const CDyncVisibleData& src);
	//��������
	virtual void CopyBaseData(CBaseDyncData *src);

	virtual ~CDyncVisibleData();
public:
	enum BLINKING_TYPE
	{
		BLINKING_NONE = 0,
		BLINKING_FAST,
		BLINKING_NORMAL,
		BLINKING_SLOW,
		BLINKING_CUSTOM  //!< �Զ�������
	};
	enum CMP_CONDITION
	{
		CONDITION_EQ = 0, //!< ����
		CONDITION_NE,    //!< ������
		CONDITION_GE,    //!< ���ڵ���
		CONDITION_GT,    //!< ����
		CONDITION_LE,    //!< С�ڵ���
		CONDITION_LT,    //!< С��
	};
	void SetVisableValue(double nValue)
	{
		m_dValue = nValue;
	}

	double GetVisableValue() const
	{
		return m_dValue;
	}

	void SetDeadBand(double nBand)
	{
		m_dDeadBand = nBand;
	}

	double GetDeadBand() const
	{
		return m_dDeadBand;
	}

	void SetCondition(unsigned int nCond)
	{
		m_nCondition = nCond;
	}

	unsigned int GetCondition() const
	{
		return m_nCondition;
	}

	void SetFlashType(unsigned int nFlashType)
	{
		m_nBlinking = nFlashType;
	}

	unsigned int GetFlashType() const
	{
		return m_nBlinking;
	}

	void SetFlashTime(unsigned int nFlashTime)
	{
		m_nBlinkInterval = nFlashTime;
	}

	unsigned int GetFlashTime() const
	{
		return m_nBlinkInterval;
	}
private:
	//! ���ڱȽϼ���
	double  m_dValue;
	//! ������Χ   ֻ�е���/������ ��������Ч
	double  m_dDeadBand;
	//! ����: > >= == <= <
	unsigned int m_nCondition;
	//! ��˸���� ��������ʱ��˸
	unsigned int m_nBlinking;
	//! �Զ�����˸���͵���˸����
	unsigned int m_nBlinkInterval;
public:
	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);
	// ��̬���ֵĴ���
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// ���ƿ�¡  ����ռ䲢�����Լ�
	virtual CBaseDyncData *CloneDyncData();
private:
	bool SetVisible(bool bFlag, CBaseWidget *pWidget);
	bool SetBlinking(bool bFlag, CBaseWidget *pWidget);
	void PluseWidget(CBaseWidget *pWidget);
	bool HasBlinking();
	//! ��˸�ļ��
	static const unsigned int BLINKING_NORMAL_CNT = 2;
	//! ��˸�ļ��
	static const unsigned int BLINKING_SLOW_CNT = 4;
};

#endif // DYNC_VISIBLE_H


/** @}*/
