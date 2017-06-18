#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QEvent>
#include <QLabel>
#include "base_widget.h"
// class CBindDataWgt : public QWidget
// {
// 	Q_OBJECT
// 
// public:
// 	CBindDataWgt(QWidget *parent = Q_NULLPTR);
// 	~CBindDataWgt();
// 	void SetBindWgt();
// 
// private:
// 	Ui::CBindDataWgt ui;
// 	//绑定值
// 	QString m_strBindValue;
// };

class QLabel;
class CBindDataWgt : public QWidget
{
	Q_OBJECT
public:
	enum 
	{
		ITEM_COLOR_RANGE,    //颜色阈值
		ITEM_VAR,            //绑定
		ITEM_TEXT_RANGE,   //文字阈值
		ITEM_MAP_RANGE,    //图形预置
		ITEM_BACKGROUND_CHOOSE,  //背景图片选择
		ITEM_PUSHBUTTON_ORDER,    //按钮命令框
	};
	CBindDataWgt(QWidget *parent = 0);
	//设置bind值
	void SetBindData(const QString &strData);
	//获取绑定值
	const QString & GetBindData() const 
	{ 
		return m_pLineEdit->text(); 
	}
	//
	void SetTypeFalg(int nFlag)
	{
		m_nTypeFlag = nFlag;
	}
	//
	void SetArrDataTypes(const std::vector<int> &arrTypes)
	{
		m_arrDataType = arrTypes;
	}

	void SetWriteFlag(bool bFlag)
	{
		m_bWriteFlag = bFlag;
	}

	void SetLineHidden()
	{
		m_pLable->setText("....");
		m_pLable->setHidden(false);
		m_pLineEdit->setHidden(true);
	}

	void SetDynamicBrush(long long lBrush)
	{
		m_lDynamicBrush = lBrush;
	}
	//备份图片文件
	QString SaveImageFile(const QString &strFilename);

signals:
	void Signal_ValueChanged(const QString &filePath);
protected:
	void focusInEvent(QFocusEvent *e);
	void focusOutEvent(QFocusEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

	bool eventFilter(QObject *watched, QEvent *event);

	private slots:
	void Slot_ButtonClicked();
private:
	QLineEdit *m_pLineEdit;
	//类型标识
	int m_nTypeFlag;
	//绑点数据
	std::vector<int> m_arrDataType;
	QLabel *m_pLable;
	//
	bool m_bWriteFlag;
	//数据
	long long m_lDynamicBrush;

};