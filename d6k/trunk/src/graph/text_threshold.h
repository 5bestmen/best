#pragma once

#include <QDialog>
#include "ui_textthreshold.h"


class QLabel;
//�Զ���ؼ�
class CMapCellWgt : public QWidget
{
	Q_OBJECT
public:
	CMapCellWgt(QWidget *parent = 0);
	//��ʼ������
	void InitWgt();
	~CMapCellWgt()
	{

	}
	//
	void SetMapPath(const QString &strPath);

	const QString &GetMapPath()
	{
		return m_strMapPath;
	}

	//����ͼƬ�ļ�
	QString SaveImageFile(const QString &strFilename);



public slots:
    //
    void Slot_Confomrm();

private:
	//��ɫ��ʶ
	QLabel *m_pLableMap;
	//��ɫ˵��
	QLineEdit *m_pLineEdMap;
	//ѡ�񰴼�
	QPushButton *m_pPushCoose;
	//ͼƬ·��
	QString m_strMapPath;

};



class CDyncText;
class CTextThreshold : public QDialog
{
	Q_OBJECT
	enum TEXTTYPE
	{
		//
		TEXT_ITEM,   //�ı�����
		MAP_ITEM,    //ͼƬ����
	};
public:
	CTextThreshold(QWidget *parent = Q_NULLPTR);
	~CTextThreshold();
public:
	void UpdateAllData(CDyncText* pDyncText);
	//
	void UpdateMapData(CDyncText* pDyncText);

	void InsertDataToWgt(CDyncText* pDyncText);
public slots:
    //ȷ����Ϣ
    void Slot_Conform();
	//add
	void Slot_AddItems();
	//delete
	void Slot_DeleteItems();

private:
	Ui::CTextThreshold ui;

	CDyncText *m_pDyncText;
	//
	int m_nType;
};
