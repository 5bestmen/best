#pragma once

#include <QDialog>
#include <QColor>
#include <QSpinBox>
#include "ui_editcolorthreshold.h"


class QPushButton;
class QLabel;
class QLineEidt;
class QComboBox;
class QSpinBox;
//�Զ���ؼ�
class EditCellWgt : public QWidget
{
	Q_OBJECT
public:
	EditCellWgt(QWidget *parent = 0);
	//��ʼ������
	void InitWgt();
	~EditCellWgt()
	{

	}

	void SetLableHidden();

	//
	void SetColor(const QColor &col);
	const QColor &GetColor()
	{
		return m_cloChoose;
	}
	//

	void SetVarData(const QString &strVar);

	const QString& GetVarData()
	{
		return m_strBindValue;
	}

signals:
	//
	void Signals_Comform();

private:
	//��ɫ��ʶ
	QLabel *m_pLableColor;
	//��ɫ˵��
	QLineEdit *m_pLineEdColor;
	//ѡ�񰴼�
	QPushButton *m_pPushCoose;
	//��ɫ
	QColor m_cloChoose;
	//��ֵ
	QString m_strBindValue;
	
};


class CEditColorThreshold : public QDialog
{
	Q_OBJECT

public:
	enum 
	{
		ENUM_EDIT,     //�༭
		ENUM_CREATE,  //����
	};
	CEditColorThreshold(QWidget *parent = Q_NULLPTR);
	~CEditColorThreshold();
	//��ʼ�����ݽṹ
	void InitEditorWgt();
   //���ó�ʼ��ֵ
	void ResetWgtData();
public:
	double GetValue()
	{
		return m_nValue;
	}

	void SetValue(double nValue)
	{
		ui.tableWidget->item(0, 1)->setText(QString::number(nValue));
	}

	const QString& GetVarable()
	{
		return m_strBindData;
	}

	void SetVarable(const QString &strVar)
	{
		m_pVarCellWgt->SetVarData(strVar);
	}

	void SetMode(int nIndex);


	int GetMode()
	{
		return m_nMode;
	}
	int GetFlashingTime()
	{
		return m_nFlashingTime;
	}

	void SetFlashingTime(int nTime)
	{
		m_pFlashTime->setValue(nTime);
	}

	const QColor & GetColor()
	{
		return m_Color;
	}

	void SetColor(const QColor &col)
	{
		m_pColorCellWgt->SetColor(col);
	}

	const QColor& GetFlashingColor()
	{
		return m_FlashingColr;
	}
	void SetFlahingColor(const QColor &col)
	{
		m_pFlashingWgt->SetColor(col);
	}

	int GetTypeFlay()
	{
		return m_nTypeFlag;
	}

	void SetTypeFlag(int nFlag)
	{
		m_nTypeFlag = nFlag;
	}


public slots:
    void Slot_CellClicked(int row, int column);
	//��ɫ
	void Slot_ColorChange();
	//flashing color
	void Slot_FlashColorChange();
	//var
	void Slot_VarChoose();
	//˫��table
	void Slot_DoubleClickTable(QTableWidgetItem *item);
	//����
	void Slot_VarBindData();
	//ȷ��
	void Slot_Conform();
signals:
	void Signal_Comform();
private:
	Ui::CEditColorThreshold ui;
	//value
	double m_dVlaue;
	//fashingTime
	int m_nFlashingTime;
	//��ɫ
	QColor m_Color;
	//��˸ɫ
	QColor m_FlashingColr;

	//����
	EditCellWgt *m_pVarCellWgt;
	//��ɫ
	EditCellWgt *m_pColorCellWgt;
	//��˸ɫ
	EditCellWgt *m_pFlashingWgt;
	//
	QComboBox *m_pComMode;
	//
	QSpinBox *m_pFlashTime;
	//��ֵ
	double m_nValue;
	//��ֵ
	QString m_strBindData;
	//ģʽ
	int m_nMode;
	//��ɫ
	QColor m_nColor;
	// flashing ʱ��
	int m_nFlashTime;
	//flashing ��ɫ
	QColor m_nFlashingColor;
	//�½�  �༭flag
	int m_nTypeFlag;
};
