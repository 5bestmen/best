#ifndef SCADAANALOGALARMLIMITTABLE_H
#define SCADAANALOGALARMLIMITTABLE_H

#include <QTableView>

namespace Config
{
	class CScadaAnalogAlarm;
	class CScadaVariableConfigData;
	class CScadaData;
}

class QStandardItem;
class IMainModuleInterface;
class CScadaAnalogAlarmLimitModel;
class CSortFilterModel;

class CScadaAnalogAlarmLimitTable : public QTableView
{
	Q_OBJECT

public:
	CScadaAnalogAlarmLimitTable(QWidget *parent, IMainModuleInterface *pCore,
		Config::CScadaAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada);
	~CScadaAnalogAlarmLimitTable();

	CScadaAnalogAlarmLimitModel *GetModel()
	{
		return m_pModel;
	}

	void Refresh();

public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex &index);

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
	CScadaAnalogAlarmLimitModel *m_pModel;
	CSortFilterModel *m_pSortModel;
};

#endif // SCADAANALOGALARMLIMITTABLE_H
