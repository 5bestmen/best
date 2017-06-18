#pragma once

#include <QDialog>

//切换定值区号
class QLineEdit;
class CFixOperateWgt : public QDialog
{
	Q_OBJECT

public:
	CFixOperateWgt(QWidget *parent = 0);
	~CFixOperateWgt();
	//初始化界面
	void InitSetWgt();
    QString GetFixData();

public slots:
    void Slot_Conform();
public:
	QLineEdit *m_pLineEdArea;
};
