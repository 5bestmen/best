/*! @file dync_color.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dync_color.h
�ļ�ʵ�ֹ��� :  ��̬��ɫ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ��̬��ɫ����
*  @author LiJin
*  @version 1.0
*  @date    2016.02.14
*******************************************************************************/
#ifndef DYNC_COLOUR_DATA_H
#define DYNC_COLOUR_DATA_H

#include "base_dync.h"
#include <QString>
#include <QColor>
#include <array>
#include <memory>
#include <vector>

class QXmlStreamWriter;
class QXmlStreamReader;
  
class CBaseWidget;  

/*! \class CDyncClrValueInfo dync_color.h  "dync_color.h"
*  \brief ��̬��ɫ����
*  \details
*/
class CDyncClrValueInfo
{
public:
	CDyncClrValueInfo();
	CDyncClrValueInfo(const CDyncClrValueInfo  *  pSrc);
	CDyncClrValueInfo(int nIdx);
	~CDyncClrValueInfo();
	const CDyncClrValueInfo& operator=(const CDyncClrValueInfo& src);
	enum BLINKING_TYPE
	{
		BLINKING_NORMAL = 0,    //һ��
		BLINKING_NONE ,  //!< ����˸
		BLINKING_INVISIBLE, //!< ���ɼ�����͸��
		BLINKING_FAST,
		BLINKING_MIDDLE,
		BLINKING_SLOW,
		BLINKING_CUSTOM  //!< �Զ�������
	};
	QString GetBlinkingTypeTxt();
	static BLINKING_TYPE GetBlinkingType(QString &szTxt);

	static const QString SZ_BINKING[BLINKING_CUSTOM + 1];

	static const  QString SZ_BLINKING_NONE;
	static const  QString SZ_BLINKING_INVISIBLE;
	static const  QString SZ_BLINKING_FAST;
	static const  QString SZ_BLINKING_NORMAL;
	static const  QString SZ_BLINKING_SLOW;
	static const  QString SZ_BLINKING_CUSTOM;
	//! Ԥ������ɫ����
	//static const unsigned int ColorTabNum = 8;
	enum { ColorTabNum = 8 };

	using COLOR_ARRAY = std::array < QColor, ColorTabNum>;
	//! Ԥ������ɫ��
	static COLOR_ARRAY m_crTable;
public:
	//! ��ʾ�ڽ��������ֵ
	std::string m_szValue;
	//! ���ڱȽϼ���
	double  m_dValue;
	//! ��ʾ����ɫ
	QColor m_dwColor;
	//! ��˸����ɫ
	QColor m_dwBlinkingColor;
	//! ��˸����
	BLINKING_TYPE m_nBlinkingType;
	//! �Զ�����˸�ļ��
	unsigned int m_nBlinkInterval;
	//! ˳��� ��1��ʼ
	unsigned int m_nIndex;
	//! ������˸ʱ�ı�־λ������ʱ��
	bool m_bBlinkingFlag;
	//! ��һ��ִ��ʱ��ʱ��
	//boost::posix_time::ptime  m_tmLast;
	//! ��˸�ļ��  ��λ����
	enum { BLINKING_NORMAL_INTERVAL_MS = 1000 };
	//! ��˸�ļ��
	enum{ BLINKING_SLOW_INTERVAL_MS = 2000 };
};
/*! \class CDyncClrData dync_color.h  "dync_color.h"
*  \brief ��̬��ɫ�仯
*  \details �����У���������䣬�ı�����ɫ��̬�仯
*/
class CDyncClrData :public CBaseDyncData
{
public:
	CDyncClrData();
	virtual ~CDyncClrData();
public:
	enum DYNC_CLR_TYPE
	{
		DYNC_LINE_CLR = 0,    //!< ������ɫ
		DYNC_FILL_CLR,        //!< �����ɫ
		DYNC_TEXT_CLR,        //!< �ı���ɫ
		DYNC_OTHER_CLR,
	};

	enum { MAX_CLR_CNT = 64 };
public:

	const CDyncClrData& operator=(const CDyncClrData& src);

	//��������
	virtual void CopyBaseData(CBaseDyncData *src);



	//! ��ɫ���ͣ� ����������䣬�ı�
	DYNC_CLR_TYPE m_nDyncClrType;
	//! ��ɫ�б�
	std::vector<CDyncClrValueInfo*> m_arrColors;
public:
	// �Ƚ�������̬�����Ƿ���һ��������
	virtual bool IsDyncTypeEqual(CBaseDyncData *pData) const;

	CDyncClrValueInfo * CreateClrData();

	void DeleteClrData(CDyncClrValueInfo *pData);
	//���Clrdata
	void ClearAllClrData();
	// ����Ƿ�Ϸ�
	virtual bool CheckDyncData();
	// Ԥ����̬
	virtual void PrepareWidgetDync(CBaseWidget *pWidget);
	// ��̬���ֵĴ���
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// ���ƿ�¡  ����ռ䲢�����Լ�
	virtual CBaseDyncData *CloneDyncData();

	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);

protected:
	//��ȡ��ǰ��ɫ
	bool GetColor(const CVariant &value, CDyncClrValueInfo &clrCur, bool &bInvisible) const;
	//! ԭʼ����ɫ [���ʣ���ˢ������]
	QColor		m_dwColor;
	//! ԭʼ��͸���� 
	unsigned int  m_nTransparency;
protected:
 
private:

};

/*! \class CDyncFillingData dync_color.h  "dync_color.h"
*  \brief ��̬��ɫ�仯
*  \details �����У���������䣬�ı�����ɫ��̬�仯
*/
class CDyncFillingData :public CDyncClrData
{
public:
	CDyncFillingData() 
	{
		m_nFillingPolicy = FILLING_B2T;
		m_bAnalogColor = false;
		m_dMinVal = 0;
		m_dMaxVal = 100;
		m_dStartPos = 0;
		m_dEndPos = 0;
	}
	const CDyncFillingData& operator=(const CDyncFillingData& src);

	virtual ~CDyncFillingData() {}

	enum DYNC_FILLING_POLICY
	{
		FILLING_B2T, //!< ���µ���
		FILLING_T2B, //!< ���ϵ���
		FILLING_L2R, //!< ������
		FILLING_R2L  //!< ���ҵ���
	};

	static const QString SZ_FILLING_POLICY[FILLING_R2L + 1];

	//��������
	virtual void CopyBaseData(CBaseDyncData *src);

public:
	//ģ��ɫ
	bool m_bAnalogColor;
	//! ������
	DYNC_FILLING_POLICY m_nFillingPolicy;
	//! ��Сֵ
	double  m_dMinVal;
	//! ���ֵ
	double  m_dMaxVal;
	//! ���ֵ �ٷֱ� 0-100 ֮��
	double  m_dStartPos;
	//! �յ�ֵ
	double  m_dEndPos;
public:
	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);


	// �Ƚ�������̬�����Ƿ���һ��������
	virtual bool IsDyncTypeEqual(CBaseDyncData *pData) const;
	// ����Ƿ�Ϸ�
	virtual bool CheckDyncData();
	// Ԥ����̬
	virtual void PrepareWidgetDync(CBaseWidget *pWidget);
	// ��̬���ֵĴ���
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// ���ƿ�¡  ����ռ䲢�����Լ�
	virtual CBaseDyncData *CloneDyncData();
};

#endif // DYNC_COLOUR_DATA_H


/** @}*/