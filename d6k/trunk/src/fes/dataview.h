/*! @file dataview.h
<PRE>
********************************************************************************
模块名       :
文件名       :  dataview.h
文件实现功能 :  内存实时数据的浏览
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  这是大的窗体，其中还需要一个splitter，左边为treeview，
                右边为tabview，tabview里面是gridview，类似scadastudio
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存实时数据的浏览
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/


#ifndef DATA_VIEW_H
#define DATA_VIEW_H

#include <QDialog>
#include <QTreeWidget>
#include <QTableView>
#include <QTextBrowser>
#include <QTabWidget>
#include <QTimer>
#include <QLibrary>
#include <memory>
#include "ui_dataview.h"


#define DataDef_ROLE    (Qt::UserRole+1)       //!<存储数据指针
#define DataType_ROLE   (Qt::UserRole+2)       //!<存储表项类型
#define DataDevOcc_ROLE (Qt::UserRole+3)       //!<存储设备OccNo

class CDbSvc;
class CDataModel;

/*
typedef std::shared_ptr<CDataModel> DataModel_PTR;
typedef std::shared_ptr<QTableView> DataView_PTR;*/

typedef CDataModel* DataModel_PTR;
typedef QTableView* DataView_PTR;

class CDataView : public QDialog
{
	Q_OBJECT

public:
	CDataView(QWidget *parent, std::shared_ptr<CDbSvc> &pMem,const QString& szName);
	~CDataView();

	void SetRdbName(const QString& szName)
	{
		m_szRdbName = szName;
	}
private:
	void InitData();
	bool InitView();
	void ShowData(DataModel_PTR ptrData,int nOccNo);
	void SetHeaderData(DataModel_PTR ptrData);
	void SetGridData(DataModel_PTR ptrData, int nOccNo);
	bool IsFileOpen(const QString& szName);
	bool IsDataViewOpen(const QString& szName);
	void closeEvent(QCloseEvent *);
private:
	Ui::CGridView ui;
	QString m_szRdbName;
	//! 此处引用 内存数据库
	std::shared_ptr<CDbSvc> & m_pMemDB;

private Q_SLOTS:
	void slot_on_tabChanged(int index);
	void slot_on_dbClicked(QTreeWidgetItem* item, int col);
	void slot_on_RemoveTab(int nIndex);
	void slot_on_Timer();
	void slot_on_tabDBClieked(const QModelIndex& index);
	void slot_OnAODbClicked(const QModelIndex& index);
private:

	QLibrary m_LibFes;
	//定时刷新数据
	std::shared_ptr<QTimer> m_pTimer;

	std::shared_ptr<QTreeWidget>m_pTree;

	//概要信息Tab
	std::shared_ptr< QTabWidget > m_pTabWidget;
	//数据信息Tab
	std::shared_ptr< QTabWidget > m_pTabDataInfoWidget;

	//页面数据类型
	int m_nDataType;

	QStringList m_szHeaderList;

	//所有数据模型集合
	std::vector<DataModel_PTR> m_VecDataModel;

	//所有视图集合
	std::vector<DataView_PTR>  m_VecDataView;
	
};

#endif // DATA_VIEW_H


/** @}*/