#pragma once

#include <QDialog>

//�л���ֵ����
class QLineEdit;
class CFixOperateWgt : public QDialog
{
	Q_OBJECT

public:
	CFixOperateWgt(QWidget *parent = 0);
	~CFixOperateWgt();
	//��ʼ������
	void InitSetWgt();
    QString GetFixData();

public slots:
    void Slot_Conform();
public:
	QLineEdit *m_pLineEdArea;
};
