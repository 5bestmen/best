#ifndef COILVIEW_H
#define COILVIEW_H

#include <QAbstractTableModel>
#include <QWidget>

//#include "memdata.h"

class QTableView;
class QStandardItemModel;
class QStandardItem;
class CMemData;

#if 0
enum
{
	COL_LEVEL = 0,
	COL_INDEX,  //!< ÐòºÅ
	COL_DATE,
	COL_TIME,
	COL_MODULE,
	COL_COMMENT,
	COL_PROCESS,
	COL_THREAD,
	COL_MAX,
};
#endif

class CGridModel : public QAbstractTableModel
{
public:
	explicit CGridModel(CMemData & MemData, QObject *parent = 0);

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant headerData(int section, Qt::Orientation /* orientation */, int role) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void SetColCount(int nCount)
	{
		m_nColCount = nCount;
	}
	void SetRowCount(int nCount)
	{
		m_nRowCount = nCount;
	}

	void SetHeadTitles(QStringList & headlist);
private:
	QStringList m_lstHead;//±íÍ·
 
	int m_nRowCount;
	int m_nColCount;

private:
	CMemData  &m_MemData;
//	CMemData * m_pMemData;
};


class CCoilView : public QWidget
{
    Q_OBJECT
public:
	explicit CCoilView(CMemData & MemData, QWidget *parent = 0);
public:


signals:

public slots :
	void OnCoilDoubleClicked(const QModelIndex & index);
protected:
	void InitView(CMemData  &mem);

private:
	//tableview
	QTableView *m_pTableView;
	//model
	//QStandardItemModel *m_pTableModel;

	CGridModel *m_pTableModel;
};

#endif // COILVIEW_H