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
		TEMPLATE_ITEM,    //����
		TEMPLATE_GROUP,   //��
	};
	CCreateNewTemplateWgt(QWidget *parent = Q_NULLPTR);
	~CCreateNewTemplateWgt();
	//��ʼ������
	void InitData();
	//�����ļ�
	bool AnalyseManagerFile(std::shared_ptr<QXmlStreamReader> pXml);
	//������ڵ�
	void AnalyseGroupNode(std::shared_ptr<QXmlStreamReader> pXml, QTreeWidgetItem *pGroupItem);
	//
	const QString &GetLibraryName()
	{
		return m_strLibaryName;
	}

	//��������
	void SaveManagerData();
	//������ڵ�
	void SaveGroupNodeData(std::shared_ptr<QXmlStreamWriter> pXml,QTreeWidgetItem *pParent);

public slots:
    void Slot_Conform();
	//�һ��˵�
	void Slot_ShowTreeContextMenu(const QPoint& pfClick);
	//add group
	void Slot_AddGroup();
	//add item
	void Slot_AddItem();
	//delete
	void Slot_Delete();
	//�������ڵ�
	void Slot_ItemClicked(QTreeWidgetItem *item, int column);

private:
	Ui::CCreateNewTemplateWgt ui;
	//��ǰ�һ�item
	QTreeWidgetItem *m_pRightClickItem;
	//������
	QString m_strLibaryName;
	//��ʾ����
	QString m_strShowName;
};
