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
	//��ʼ��  ����tab����
	void InitVarTabInfo();
	//��ʼ��  ���� tab����
	void InitWindowTabInfo();

public slots:
    //��
	void Slot_VarBindData();
	//ѡ��window  �ۺ���
	void Slot_ChooseWindow();

private:
	Ui::COrderEditWgt ui;
	//����
	EditCellWgt *m_pVarCellWgt;
	//����
	QComboBox *m_pVarActionCombox;
	//���͵�����
	EditCellWgt *m_pVarTransforVarCellWgt;
	//ѡͨʱ��
	QSpinBox *m_pVarxtTime;
	//����ַ���
	QSpinBox *m_pVarMaxStringLegth;

	//����
	EditCellWgt *m_pWindowCellWgt;
	//���ڶ���
	QComboBox *m_pWindowActionCombobox;

};
