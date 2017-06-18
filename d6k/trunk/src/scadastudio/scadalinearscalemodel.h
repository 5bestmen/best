#ifndef SCADALINEARSCALEMODEL_H
#define SCADALINEARSCALEMODEL_H

#include <QAbstractTableModel>

namespace Config
{
	class CScadaTransform;
	class CScadaVariableConfigData;
	class CScadaLinearTransform;
	class CScadaData;
}
class CQuoteItem;
class CScadaVariableModule;

class CScadaLinearScaleModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN{
		ID, TagName, ScaleName, OccNo, Extention, ScaleType/*, GroupName*/
	};

public:
	CScadaLinearScaleModel(QObject *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
		, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada);
	~CScadaLinearScaleModel();

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
	QStringList m_horizontal_header_list;
	Config::CScadaTransform *m_pTransform;
	CQuoteItem *m_pItem;
	Config::CScadaVariableConfigData *m_pConfig;
	Config::CScadaLinearTransform *m_pLinear;
	QString m_strTypeDesc;
	Config::CScadaData *m_pScada;
};

#endif // SCADALINEARSCALEMODEL_H
