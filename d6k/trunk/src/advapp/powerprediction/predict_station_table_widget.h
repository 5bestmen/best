#ifndef PREDICT_STATION_TABLE_WIDGET
#define PREDICT_STATION_TABLE_WIDGET

#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/icore.h"
#include "predict_datainfo.h"

#include <QWidget>
#include <QTableWidget>

class CStationTableWidget : public QTableWidget, public IBase
{
	Q_OBJECT

public:
	CStationTableWidget(CPlantInfo* plantItem);
	~CStationTableWidget();

public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag);
	virtual bool GetModifyFlag();

	CPlantInfo* GetPlantItem() const;
	//获取绑点值
	QString GetBindValue();
public slots:
    //设置绑定值 
    void Slot_SetBindValue();

private:
	CPlantInfo*    m_pPlantItem;
};



class CPredictStationWidget : public QWidget
{
	Q_OBJECT

public:

	CPredictStationWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CPredictStationWidget();

public:

	void AddToTableCtrl(CPlantInfo* pPlantItem, QString strName);

	void ReleaseData();

	void SetTabViewHead();

private:

	CTabWidget *m_pTableWidget;

	IMainModuleInterface *m_pCore;

};


#endif  //PREDICT_STATION_TABLE_WIDGET
