#ifndef PREDICT_INVERTER_GROUP_TABLE_WIDGET
#define PREDICT_INVERTER_GROUP_TABLE_WIDGET

#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/icore.h"
#include "predict_datainfo.h"

#include <QWidget>
#include <QTableWidget>

class CInverterGrpTableWidget : public QTableWidget, public IBase
{
	Q_OBJECT

public:
	CInverterGrpTableWidget(CInverterGroup* inverterGrpItem);
	~CInverterGrpTableWidget();

public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag);
	virtual bool GetModifyFlag();

	CInverterGroup* GetInverterGrpItem() const;
	//获取绑点值
	QString GetBindValue();
public slots:
	//设置绑定值 
	void Slot_SetBindValue();

private:

	CInverterGroup*    m_pInverterGrpItem;
};



class CPredictInverterGrpWidget : public QWidget
{
	Q_OBJECT

public:

	CPredictInverterGrpWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CPredictInverterGrpWidget();

public:

	void AddToTableCtrl(CInverterGroup* pInverterGrpItem);

	void ReleaseData();

	void SetTabViewHead();

private:

	CTabWidget *m_pTableWidget;

	IMainModuleInterface *m_pCore;

};
#endif  //PREDICT_INVERTER_GROUP_TABLE_WIDGET

