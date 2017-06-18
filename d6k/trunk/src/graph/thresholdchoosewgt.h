#pragma once

#include <QDialog>
#include <QColor>
#include "ui_thresholdchoosewgt.h"
#include "base_widget.h"


class CEditColorThreshold;
class CDyncClrValueInfo;
class CDyncClrData;

class CThresholdChooseWgt : public QDialog
{
	Q_OBJECT

public:
	CThresholdChooseWgt(QWidget *parent = Q_NULLPTR);
	~CThresholdChooseWgt();
	//����Ĭ������
	void SetDefaultVale();
	//��������
	void InitEditWgt(CDyncClrValueInfo *pBrushData);
	//
	void UpdateEditData(CDyncClrValueInfo *pBrushData);
	//	//��ȡ��ǰ���еı���˵��Ϣ
	const QList<CDyncClrValueInfo *>& GetAllBrushInfo();
	//����ֵ
	void UpdateAllData(CDyncClrData *pDyBrush);

public slots:
    void Slot_AddNewItem();
	//ȷ��
	void Slot_Conform();
	//�༭
	void Slot_EditColor();
	//ɾ��
	void Slot_DeleteColor();
	//defaule
	void Slot_SetDefault();
	//���˫��
	void Slot_TableDoubleClick(QTableWidgetItem *item);
	//����ֵ
	void Slot_UpdateEditData();
	


private:
	Ui::CThresholdChooseWgt ui;
	//�༭����
	CEditColorThreshold *m_pEditColorWgt;
	//����
	QList<CDyncClrValueInfo *> m_lstBrushInfo;
};
