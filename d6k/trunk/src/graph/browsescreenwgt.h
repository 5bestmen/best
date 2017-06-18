#pragma once

#include <QDialog>
#include <memory>
#include <QXmlStreamReader>
#include "ui_browsescreenwgt.h"

class CBrowseScreenWgt : public QDialog
{
	Q_OBJECT

public:
	CBrowseScreenWgt(QWidget *parent = Q_NULLPTR);
	~CBrowseScreenWgt();
	//���ؽֽṹ
	void LoadWindowData(const QString &strProjectFileName);
	//�����ļ�
	void AnalyseObjectFile(std::shared_ptr<QXmlStreamReader> xmlReader);
	//�������нڵ�
	void AnalyseAllNode(std::shared_ptr<QXmlStreamReader> xmlReader);
	//����group�ڵ�
	void AnalyseGroupNode(std::shared_ptr<QXmlStreamReader> xmlReader, QString strFatherName, QTreeWidgetItem *pFather);
private:
	Ui::CBrowseScreenWgt ui;
};
