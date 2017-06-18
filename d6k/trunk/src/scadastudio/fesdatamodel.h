#ifndef FESDATAMODEL_H
#define FESDATAMODEL_H

#include <QAbstractTableModel>
#include "config_data.h"
#include "scadastudiodefine.h"

class CQuoteItem;
class IMainModuleInterface;
namespace Config
{
	class CFesConfigData;
}


class CFesDataModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMNTYPE{
		LINESTRING = 2
	};
	
	enum COLUMN{
		ID, TagName, FesName, OccNo, Extention, GroupName
	};

	CFesDataModel(QObject *parent, Config::CFesData *pFesData, CQuoteItem *pItem, IMainModuleInterface *pCore
		, Config::CFesConfigData *pFesConfig);
	~CFesDataModel();

	void SetHorizontalHeaderList(QStringList horizontalHeaderList);

	void SetVerticalHeaderList(QStringList verticalHeaderList);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void SetModalDatas(QList< QVector<QString> > &rowlist);

	void RefrushModel();

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	//void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

	QList<QVector<QString>> &GetDataList();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	bool InsertRowsInModel(int position, int rows, QSet<int> &set);

	void emitDataChanged(const QModelIndex &index);

	QModelIndex createDataIndex(int row, int col, void *data = 0)
	{
		return QAbstractTableModel::createIndex(row, col, data);
	}

private:
	void InitPara();

	bool IsModifyFesTagNameExist(QString &strTagName);
private:
	QStringList m_lstHorizontalHeader;
	QStringList m_lstVerticalHeader;

	QMap<int32s, CONFIG_PARA> m_mapInfo;

	Config::CFesData *m_pFesData;

	IMainModuleInterface *m_pCore;

	CQuoteItem *m_pItem;

	Config::CFesConfigData *m_pFesConfig;
};

#endif // FESDATAMODEL_H
