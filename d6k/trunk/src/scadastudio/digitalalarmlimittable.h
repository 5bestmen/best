#ifndef DIGITALALARMLIMITTABLE_H
#define DIGITALALARMLIMITTABLE_H

#include <QTableView>

class IMainModuleInterface;
class CDigitalAlarmLimitModel;
class CAlarm;

namespace Config
{
	class CDigitalAlarm;
	class CFesData;
}

class QStandardItem;
class CSortFilterModel;

class CDigitalAlarmLimitTable : public QTableView
{
	Q_OBJECT

public:
	CDigitalAlarmLimitTable(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CDigitalAlarmLimitTable();

	CDigitalAlarmLimitModel *GetModel()
	{
		return m_pModel;
	}

public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex & index);

private:
	void AddAnalogPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &index);

	int QColorToInt(const QColor &color) const
	{
		//将Color 从QColor 转换成 int  
		return   (int)(((unsigned int)color.blue() << 16) | (unsigned short)(((unsigned short)color.green() << 8) | color.red()));
	}
	QColor IntToQColor(const int &intColor) const
	{
		//将Color 从int 转换成 QColor  
		int red = intColor & 255;
		int green = intColor >> 8 & 255;
		int blue = intColor >> 16 & 255;
		return QColor(red, green, blue);
	}

private:
	CDigitalAlarmLimitModel *m_pModel;
	CSortFilterModel *m_pSortModel;
};

#endif // DIGITALALARMLIMITTABLE_H
