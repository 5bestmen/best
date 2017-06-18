#ifndef PREDICT_OUTPUT_TABLE_WIDGET
#define PREDICT_OUTPUT_TABLE_WIDGET

#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/icore.h"
#include "predict_datainfo.h"

#include <QWidget>
#include <QTableWidget>

class COutputTableWidget : public QTableWidget, public IBase
{
	Q_OBJECT

public:
	COutputTableWidget(CPredictData* outputItem);
	~COutputTableWidget();

public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag);
	virtual bool GetModifyFlag();

	CPredictData* GetOutputItem() const;
	//获取绑点值
	QString GetBindValue();
public slots:
	//设置绑定值 
	void Slot_SetBindValue();

private:

	CPredictData*    m_pOutputItem;
};



class CPredictOutputWidget : public QWidget
{
	Q_OBJECT

public:

	CPredictOutputWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CPredictOutputWidget();

public:

	void AddToTableCtrl(CPredictData* pOutputItem, QString szName);

	void ReleaseData();

	void SetTabViewHead();

private:

	CTabWidget *m_pTableWidget;

	IMainModuleInterface *m_pCore;

};
#endif  //PREDICT_OUTPUT_TABLE_WIDGET

