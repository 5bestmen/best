#pragma once

#include <QDialog>
#include <memory>
#include <QXmlStreamReader>
#include "ui_createnewtemplatewgt.h"

#define TEMPLATE_MANMAGER_FILE  "./gtemplate/tlibrarymanger"
#define  TEMPLATE_LIB_PATH      "./gtemplate/tlibrary/"
#define  TEMPLATE_IMAGE_PATH    "./gtemplate/image/"

#define TEMPLATE_FOLDER_ICON "./gtemplate/image/folder.ico"



class CCreateNewTemplateWgt : public QDialog
{
	Q_OBJECT

public:
	enum 
	{
		TEMPLATE_ITEM,    //对象
		TEMPLATE_GROUP,   //组
	};
	CCreateNewTemplateWgt(QWidget *parent = Q_NULLPTR);
	~CCreateNewTemplateWgt();
	//初始化界面
	void InitData();
	//解析文件
	bool AnalyseManagerFile(std::shared_ptr<QXmlStreamReader> pXml);
	//解析组节点
	void AnalyseGroupNode(std::shared_ptr<QXmlStreamReader> pXml, QTreeWidgetItem *pGroupItem);
	//
	const QString &GetLibraryName()
	{
		return m_strLibaryName;
	}

	//保存数据
	void SaveManagerData();
	//解析组节点
	void SaveGroupNodeData(std::shared_ptr<QXmlStreamWriter> pXml,QTreeWidgetItem *pParent);

public slots:
    void Slot_Conform();
	//右击菜单
	void Slot_ShowTreeContextMenu(const QPoint& pfClick);
	//add group
	void Slot_AddGroup();
	//add item
	void Slot_AddItem();
	//delete
	void Slot_Delete();
	//单击树节点
	void Slot_ItemClicked(QTreeWidgetItem *item, int column);

private:
	Ui::CCreateNewTemplateWgt ui;
	//当前右击item
	QTreeWidgetItem *m_pRightClickItem;
	//库名称
	QString m_strLibaryName;
	//显示名称
	QString m_strShowName;
};
