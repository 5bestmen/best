#ifndef PREDICT_INVERTER_TABLE_WIDGET
#define PREDICT_INVERTER_TABLE_WIDGET

#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/icore.h"
#include "predict_datainfo.h"

#include <QWidget>
#include <QTableWidget>

class CInverterTableWidget : public QTableWidget, public IBase
{
	Q_OBJECT

public:
	CInverterTableWidget(CInverterInfo* inverterItem);
	~CInverterTableWidget();

public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag);
	virtual bool GetModifyFlag();

	CInverterInfo* GetInverterItem() const;
	//获取绑点值
	QString GetBindValue();
public slots:
	//设置绑定值 
	void Slot_SetBindValue();

private:

	CInverterInfo*    m_pInverterItem;
};



class CPredictInverterWidget : public QWidget
{
	Q_OBJECT

public:

	CPredictInverterWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CPredictInverterWidget();

public:

	void AddToTableCtrl(CInverterInfo* pInverterItem,QString szName);

	void ReleaseData();

	void SetTabViewHead();

private:

	CTabWidget *m_pTableWidget;

	IMainModuleInterface *m_pCore;

};
#endif  //PREDICT_INVERTER_TABLE_WIDGET


