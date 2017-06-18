#ifndef ALARMINFOMODEL_H
#define ALARMINFOMODEL_H


#include "alarmdataconfig.h"

#include <QAbstractTableModel>
#include <memory>
#include <QIcon>
enum HEADER
{
	ALARM_TYPE,          //�澯����
	ALARM_TIME,          //�澯ʱ��
	ALARM_VALUE,         //����ԭʼ����
	ALARM_STATE,         //�澯״̬
	ALARM_DESC,          //���ս�������
};

class CAlarmInfoModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CAlarmInfoModel(QObject *parent, std::shared_ptr<CAlarmInfoQueue>& AlarmQueue);
	~CAlarmInfoModel();

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)const;

public:
	void SetRowCount(int  nRowCount)
	{
		m_nRowCount = nRowCount;
	}
	void SetColCount(int  nColCount)
	{
		m_nColCount = nColCount;
	}
	void SetHeaderData(const QStringList& nlist)
	{
		m_szHeaderList = nlist;
	}
	void RefeshModel()
	{
		beginResetModel();
		endResetModel();
	}
private:
	QVariant ShowData(int row,int col,int role) const;

protected:
	QString GetEventType(int nType) const;
private:
	QIcon m_RedIcon;
	QIcon m_GreenIcon;
private:
	mutable int m_nCount;
	int m_nRowCount;
	int m_nColCount;
	QStringList m_szHeaderList;
private:
	std::shared_ptr<CAlarmInfoQueue>& m_AlarmQueue;
};

#endif // ALARMINFOMODEL_H
