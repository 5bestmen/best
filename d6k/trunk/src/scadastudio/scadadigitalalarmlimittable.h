#ifndef SCADADIGITALALARMLIMITTABLE_H
#define SCADADIGITALALARMLIMITTABLE_H

#include <QTableView>

namespace Config
{
	class CScadaDigitalAlarm;
	class CScadaVariableConfigData;
	class CScadaData;
}

class QStandardItem;
class IMainModuleInterface;
class CScadaDigitalAlarmLimitModel;
class CSortFilterModel;

class CScadaDigitalAlarmLimitTable : public QTableView
{
	Q_OBJECT

public:
	CScadaDigitalAlarmLimitTable(QWidget *parent, IMainModuleInterface *pCore,
		Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada);
	~CScadaDigitalAlarmLimitTable();

	CScadaDigitalAlarmLimitModel *GetModel()
	{
		return m_pModel;
	}

	void Refresh();

public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex &index);

private:
	void AddPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &indexSelect);

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
	CScadaDigitalAlarmLimitModel *m_pModel;
	CSortFilterModel *m_pSort;
};

#endif // SCADADIGITALALARMLIMITTABLE_H
