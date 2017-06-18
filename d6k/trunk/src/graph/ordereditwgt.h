#pragma once

#include <QDialog>
#include "ui_ordereditwgt.h"

class EditCellWgt;
class QComboBox;
class QSpinBox;

class COrderEditWgt : public QDialog
{
	Q_OBJECT

public:
	COrderEditWgt(QWidget *parent = Q_NULLPTR);
	~COrderEditWgt();
	//初始化  变量tab内容
	void InitVarTabInfo();
	//初始化  窗口 tab内容
	void InitWindowTabInfo();

public slots:
    //绑定
	void Slot_VarBindData();
	//选择window  槽函数
	void Slot_ChooseWindow();

private:
	Ui::COrderEditWgt ui;
	//变量
	EditCellWgt *m_pVarCellWgt;
	//动作
	QComboBox *m_pVarActionCombox;
	//传送到变量
	EditCellWgt *m_pVarTransforVarCellWgt;
	//选通时间
	QSpinBox *m_pVarxtTime;
	//最大字符数
	QSpinBox *m_pVarMaxStringLegth;

	//窗口
	EditCellWgt *m_pWindowCellWgt;
	//窗口动作
	QComboBox *m_pWindowActionCombobox;

};
