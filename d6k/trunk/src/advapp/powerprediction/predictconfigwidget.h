#pragma once

#include <QWidget>
#include "ui_cconfigwidget.h"
#include <QObject>

class CConfigWidget : public QWidget
{
	Q_OBJECT

public:
	CConfigWidget(QWidget *parent = Q_NULLPTR);
	~CConfigWidget();

public:	
	enum TabKind
	{
		Digitals,
	    Analogues,
		Relays,
		SetPoints,
	};

protected:
	void InitSlots();
	void InitTable();
	void Load();

protected slots:
	void OnAdd();
	void OnRemove();
	void OnSave();
	void OnSetting();

private:
	Ui::cconfigwidget ui;
};
