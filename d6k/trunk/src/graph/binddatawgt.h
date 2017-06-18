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
// 	//��ֵ
// 	QString m_strBindValue;
// };

class QLabel;
class CBindDataWgt : public QWidget
{
	Q_OBJECT
public:
	enum 
	{
		ITEM_COLOR_RANGE,    //��ɫ��ֵ
		ITEM_VAR,            //��
		ITEM_TEXT_RANGE,   //������ֵ
		ITEM_MAP_RANGE,    //ͼ��Ԥ��
		ITEM_BACKGROUND_CHOOSE,  //����ͼƬѡ��
		ITEM_PUSHBUTTON_ORDER,    //��ť�����
	};
	CBindDataWgt(QWidget *parent = 0);
	//����bindֵ
	void SetBindData(const QString &strData);
	//��ȡ��ֵ
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
	//����ͼƬ�ļ�
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
	//���ͱ�ʶ
	int m_nTypeFlag;
	//�������
	std::vector<int> m_arrDataType;
	QLabel *m_pLable;
	//
	bool m_bWriteFlag;
	//����
	long long m_lDynamicBrush;

};