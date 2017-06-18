#pragma once

#include <QDialog>
#include "ui_textthreshold.h"


class QLabel;
//自定义控件
class CMapCellWgt : public QWidget
{
	Q_OBJECT
public:
	CMapCellWgt(QWidget *parent = 0);
	//初始化界面
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

	//备份图片文件
	QString SaveImageFile(const QString &strFilename);



public slots:
    //
    void Slot_Confomrm();

private:
	//颜色标识
	QLabel *m_pLableMap;
	//颜色说明
	QLineEdit *m_pLineEdMap;
	//选择按键
	QPushButton *m_pPushCoose;
	//图片路径
	QString m_strMapPath;

};



class CDyncText;
class CTextThreshold : public QDialog
{
	Q_OBJECT
	enum TEXTTYPE
	{
		//
		TEXT_ITEM,   //文本类型
		MAP_ITEM,    //图片类型
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
    //确认信息
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
