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
//自定义控件
class EditCellWgt : public QWidget
{
	Q_OBJECT
public:
	EditCellWgt(QWidget *parent = 0);
	//初始化界面
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
	//颜色标识
	QLabel *m_pLableColor;
	//颜色说明
	QLineEdit *m_pLineEdColor;
	//选择按键
	QPushButton *m_pPushCoose;
	//颜色
	QColor m_cloChoose;
	//绑定值
	QString m_strBindValue;
	
};


class CEditColorThreshold : public QDialog
{
	Q_OBJECT

public:
	enum 
	{
		ENUM_EDIT,     //编辑
		ENUM_CREATE,  //创建
	};
	CEditColorThreshold(QWidget *parent = Q_NULLPTR);
	~CEditColorThreshold();
	//初始化数据结构
	void InitEditorWgt();
   //设置初始化值
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
	//颜色
	void Slot_ColorChange();
	//flashing color
	void Slot_FlashColorChange();
	//var
	void Slot_VarChoose();
	//双击table
	void Slot_DoubleClickTable(QTableWidgetItem *item);
	//变量
	void Slot_VarBindData();
	//确定
	void Slot_Conform();
signals:
	void Signal_Comform();
private:
	Ui::CEditColorThreshold ui;
	//value
	double m_dVlaue;
	//fashingTime
	int m_nFlashingTime;
	//颜色
	QColor m_Color;
	//闪烁色
	QColor m_FlashingColr;

	//变量
	EditCellWgt *m_pVarCellWgt;
	//颜色
	EditCellWgt *m_pColorCellWgt;
	//闪烁色
	EditCellWgt *m_pFlashingWgt;
	//
	QComboBox *m_pComMode;
	//
	QSpinBox *m_pFlashTime;
	//阈值
	double m_nValue;
	//绑定值
	QString m_strBindData;
	//模式
	int m_nMode;
	//颜色
	QColor m_nColor;
	// flashing 时间
	int m_nFlashTime;
	//flashing 颜色
	QColor m_nFlashingColor;
	//新建  编辑flag
	int m_nTypeFlag;
};
