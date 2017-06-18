#pragma once

#include <QWidget>
#include "ui_filetranswgt.h"
#include "asdu_info.h"

extern QString g_FileName;

class CCommThread;
class CFileTransWgt : public QWidget
{
	Q_OBJECT

public:
	CFileTransWgt(CCommThread *pCommunicate,QWidget *parent = Q_NULLPTR);
	~CFileTransWgt();
public slots:
    //refresh
    void Slot_RefreshItems();
	//downloa
	void Slot_DownLoadItems();
	//upload
	void Slot_upLoadItems();

	//update items
	void Slot_upDataITems(QList<Catalog_Info>& lstItems);


private:
	Ui::CFileTransWgt ui;
	//
	CCommThread *m_pCommuncate;
};
