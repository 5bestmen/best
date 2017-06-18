#ifndef  POWER_PREDICT_DATA_H
#define  POWER_PREDICT_DATA_H


#include <QVector>
#include <QAbstractTableModel>
#include <QVariant>


class CPointItem
{
public:
	CPointItem()
	{

	}

	~CPointItem()
	{

	}

public:

	QVector<CPointInfo> m_vecTableInfo;


};



class CPointInfo
{
public:
	CPointInfo()
	{

	}

	~CPointInfo()
	{

	}

public:

	QString m_strName;           //����
	QString m_strDesc;           //����
	int     m_nType;             //�������
	QString m_strRelationPoint;  //�������

};



class CTabContent : public QAbstractTableModel
{
public:

	CTabContent();
	~CTabContent();

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

public:

	bool ModifyContentData(const QModelIndex& index, const QVariant& value, int &role);


};






#endif  //POWER_PREDICT_DATA_H