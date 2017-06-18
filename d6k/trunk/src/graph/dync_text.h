#ifndef DYNC_TEXT_H
#define DYNC_TEXT_H

#include <QString>
#include <vector>
#include "base_dync.h"


class CDyncTextValueInfo
{
public:
	void SetOrigData(const QString &strOrig)
	{
		m_strOrigData = strOrig;
	}

	const QString &GetorigData()
	{
		return m_strOrigData;
	}

	void SetDescData(const QString &strDescData)
	{
		m_strDescData = strDescData;
	}

	const QString &GetDescData()
	{
		return m_strDescData;
	}

	const CDyncTextValueInfo& operator=(const CDyncTextValueInfo& src)
	{
		m_strDescData = src.m_strDescData;
		m_strOrigData = src.m_strOrigData;

		return *this;
	}
private:
	//原始值
	QString m_strOrigData;
	//改变后的值
	QString m_strDescData;
};

class CDyncText : public CBaseDyncData
{
public:
	CDyncText();
	virtual ~CDyncText();

	const CDyncText& operator=(const CDyncText& src);
	//复制数据
	virtual void CopyBaseData(CBaseDyncData *src);


public:
	CDyncTextValueInfo * CreateTextData();

	void DeleteTextData(CDyncTextValueInfo *pData);
	//清空Clrdata
	void ClearAllTextData();

	std::vector<CDyncTextValueInfo*> m_arrTexts;
	//

public:
	virtual void PrepareWidgetDync(CBaseWidget *pWidget);
	// 动态部分的处理
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);

	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);
protected:
	QString GetDesc(int nVal);
	
};

#endif // BASE_DYNC_DATA_H
