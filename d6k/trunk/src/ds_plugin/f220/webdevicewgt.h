#ifndef WEBDEVICEWGT_H
#define WEBDEVICEWGT_H

#include <QWidget>

class QLineEdit;
class QWebEngineView;
class CWebDeviceWgt : public QWidget
{
	Q_OBJECT

public:
	CWebDeviceWgt(QWidget *parent = Q_NULLPTR);
	void InitWgt();
	~CWebDeviceWgt();
public slots:
    void Slot_Confrom();
	//ǰ��
	void Slot_backPage();
	//ǰ��
	void Slot_Forword();
private:
	//ip
	QLineEdit *m_pLineIpAddr;

	//�˿ں�
	QLineEdit *m_pLinePort;
	//
	QWebEngineView *m_pWebView;
};


#endif