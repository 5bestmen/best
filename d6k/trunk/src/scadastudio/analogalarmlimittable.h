#ifndef ANALOGALARMLIMITTABLE_H
#define ANALOGALARMLIMITTABLE_H

#include <QTableView>

class IMainModuleInterface;
class CAlarm;
class CAnalogAlarmLimitModel;
class CSortFilterModel;

namespace Config
{
	class CAinAlarmLimit;
	class CFesData;
	class CAnalogAlarm;
}

class QStandardItem;

class CAnalogAlarmLimitTable : public QTableView
{
	Q_OBJECT

public:
	CAnalogAlarmLimitTable(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CAnalogAlarmLimitTable();

	CAnalogAlarmLimitModel *GetModel()
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
	CAnalogAlarmLimitModel *m_pModel;
	CSortFilterModel *m_pSortModel;
};

#endif // ANALOGALARMLIMITTABLE_H
