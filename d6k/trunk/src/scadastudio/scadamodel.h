#ifndef SCADAMODEL_H
#define SCADAMODEL_H

#include <QAbstractTableModel>
#include "config_data.h"
#include "scadastudiodefine.h"

class CQuoteItem;
class IMainModuleInterface;
namespace Config
{
	class CScadaVariableConfigData;
	class CScadaData;
}


class CScadaModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMNTYPE{
		LINESTRING = 2
	};
	
	enum COLUMN{
		ID, TagName, Name, OccNo, Extention, GroupName
	};

	CScadaModel(QObject *parent, Config::CScadaData *pData, CQuoteItem *pItem, IMainModuleInterface *pCore
		, Config::CScadaVariableConfigData *pConfig);
	~CScadaModel();

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
private:
	QStringList horizontal_header_list;
	QStringList vertical_header_list;

	QMap<int32s, CONFIG_PARA> m_arrInfo;

	Config::CScadaData *m_pScadaData;

	IMainModuleInterface *m_pCore;

	CQuoteItem *m_pItem;

	Config::CScadaVariableConfigData *m_pScadaConfig;
};

#endif // FESDATAMODEL_H
