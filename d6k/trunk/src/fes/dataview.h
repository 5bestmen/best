/*! @file dataview.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dataview.h
�ļ�ʵ�ֹ��� :  �ڴ�ʵʱ���ݵ����
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ���Ǵ�Ĵ��壬���л���Ҫһ��splitter�����Ϊtreeview��
                �ұ�Ϊtabview��tabview������gridview������scadastudio
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �ڴ�ʵʱ���ݵ����
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


#define DataDef_ROLE    (Qt::UserRole+1)       //!<�洢����ָ��
#define DataType_ROLE   (Qt::UserRole+2)       //!<�洢��������
#define DataDevOcc_ROLE (Qt::UserRole+3)       //!<�洢�豸OccNo

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
	//! �˴����� �ڴ����ݿ�
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
	//��ʱˢ������
	std::shared_ptr<QTimer> m_pTimer;

	std::shared_ptr<QTreeWidget>m_pTree;

	//��Ҫ��ϢTab
	std::shared_ptr< QTabWidget > m_pTabWidget;
	//������ϢTab
	std::shared_ptr< QTabWidget > m_pTabDataInfoWidget;

	//ҳ����������
	int m_nDataType;

	QStringList m_szHeaderList;

	//��������ģ�ͼ���
	std::vector<DataModel_PTR> m_VecDataModel;

	//������ͼ����
	std::vector<DataView_PTR>  m_VecDataView;
	
};

#endif // DATA_VIEW_H


/** @}*/