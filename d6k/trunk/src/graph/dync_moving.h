#ifndef DYNC_MOVING_H
#define DYNC_MOVING_H

#include <QPoint>
#include <QVector>
#include "base_dync.h"


class CDyncMovingData : public CBaseDyncData
{
public:
	CDyncMovingData();
	~CDyncMovingData();

	const CDyncMovingData& operator=(const CDyncMovingData& src);

	//��������
	virtual void CopyBaseData(CBaseDyncData *src);

public:

	int GetStartOffset() const
	{
		return m_nStartOffset;
	}

	void SetStartoffset(int nMin)
	{
		m_nStartOffset = nMin;
	}

	int GetEndOffset() const
	{
		return m_nEndOffsedData;
	}

	void SetEndoffset(int nMax)
	{
		m_nEndOffsedData = nMax;
	}

	void SetStartData(int nStart)
	{
		m_nStartData = nStart;
	}

	int GetStartData() const
	{
		return m_nStartData;
	}

	void SetEndData(int nEnd)
	{
		m_nEndData = nEnd;
	}

	int GetEndData() const
	{
		return m_nEndData;
	}

	bool GetBackFlag() const
	{
		return m_bBackFlag;
	}

	void SetBackFlag(bool bBack)
	{
		m_bBackFlag = bBack;
	}

	void SetDistance(double dDis)
	{
		m_dDistance = dDis;
	}

	double GetDistance() const
	{
		return m_dDistance;
	}

	void SetStartAngel(int nAngle)
	{
		m_nStartAngle = nAngle;
	}

	int GetStartAngle() const
	{
		return m_nStartAngle;
	}

	int GetEndAngel() const
	{
		return m_nEndANgle;
	}

	void SetEndAngle(int nAngle)
	{
		m_nEndANgle = nAngle;
	}
public:
	virtual void PrepareWidgetDync(CBaseWidget *pWidget);
	// ��̬���ֵĴ���
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// ���ƿ�¡  ����ռ䲢�����Լ�
	virtual CBaseDyncData *CloneDyncData();

	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);

private:
	//�����ƶ�
	bool m_bBackFlag;
	//������Сֵ
	int m_nStartOffset;
	//�������ֵ
	int m_nEndOffsedData;
	//��ʼֵ
	int m_nStartData;
	//����ֵ
	int m_nEndData;
	
//����ؼ�����
private:
	qreal m_fXAis;
	qreal m_fYAis;
	//λ������
	double m_dDistance;

	//��ʼ�Ƕ�
	int m_nStartAngle;
	//�����Ƕ�
	int m_nEndANgle;
	//
};


class CDyncGroupMovingData : public CBaseDyncData
{
public:
	CDyncGroupMovingData();
	~CDyncGroupMovingData();

	const CDyncGroupMovingData& operator=(const CDyncGroupMovingData& src);
	//��������
	virtual void CopyBaseData(CBaseDyncData *src);

public:

	void SetStartData(int nStart)
	{
		m_nStartData = nStart;
	}

	int GetStartData() const
	{
		return m_nStartData;
	}

	void SetEndData(int nEnd)
	{
		m_nEndData = nEnd;
	}

	int GetEndData() const
	{
		return m_nEndData;
	}

	const QVector<QPointF>& GetPoints()
	{
		return m_arrPoints;
	}

	void InsertPonit(const QPointF &pfPoint)
	{
		m_arrPoints.append(pfPoint);
	}

	void ClearAllPoints()
	{
		m_arrPoints.clear();
	}
public:
	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);

private:

	//��ʼֵ
	int m_nStartData;
	//����ֵ
	int m_nEndData;
	//�㼯�� ��Ե�
	QVector<QPointF> m_arrPoints;

};



#endif 
