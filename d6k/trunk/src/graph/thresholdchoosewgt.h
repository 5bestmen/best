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
	//设置默认属性
	void SetDefaultVale();
	//加载数据
	void InitEditWgt(CDyncClrValueInfo *pBrushData);
	//
	void UpdateEditData(CDyncClrValueInfo *pBrushData);
	//	//获取当前所有的比如说信息
	const QList<CDyncClrValueInfo *>& GetAllBrushInfo();
	//给定值
	void UpdateAllData(CDyncClrData *pDyBrush);

public slots:
    void Slot_AddNewItem();
	//确定
	void Slot_Conform();
	//编辑
	void Slot_EditColor();
	//删除
	void Slot_DeleteColor();
	//defaule
	void Slot_SetDefault();
	//表格双击
	void Slot_TableDoubleClick(QTableWidgetItem *item);
	//更新值
	void Slot_UpdateEditData();
	


private:
	Ui::CThresholdChooseWgt ui;
	//编辑界面
	CEditColorThreshold *m_pEditColorWgt;
	//数据
	QList<CDyncClrValueInfo *> m_lstBrushInfo;
};
