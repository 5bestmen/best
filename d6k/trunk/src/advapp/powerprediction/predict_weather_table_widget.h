#ifndef PREDICT_WEATHER_TABLE_WIDGET
#define PREDICT_WEATHER_TABLE_WIDGET

#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/icore.h"
#include "predict_datainfo.h"

#include <QWidget>
#include <QTableWidget>

class CWeatherTableWidget : public QTableWidget, public IBase
{
	Q_OBJECT

public:
	CWeatherTableWidget(CWeatherData* weatherItem);
	~CWeatherTableWidget();

public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag);
	virtual bool GetModifyFlag();

	CWeatherData* GetWeatherItem() const;
	//获取绑点值
	QString GetBindValue();
	public slots:
	//设置绑定值 
	void Slot_SetBindValue();
private:

	CWeatherData*    m_pWeatherItem;
};



class CPredictWeatherWidget : public QWidget
{
	Q_OBJECT

public:

	CPredictWeatherWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CPredictWeatherWidget();

public:

	void AddToTableCtrl(CWeatherData* pWeatherItem, QString strName);

	void ReleaseData();

	void SetTabViewHead();

private:

	CTabWidget *m_pTableWidget;

	IMainModuleInterface *m_pCore;

};
#endif  //PREDICT_WEATHER_TABLE_WIDGET
