#ifndef SCALELIEARMODEL_H
#define SCALELIEARMODEL_H

#include <QAbstractTableModel>
//#include "config_data.h"
#include "scadastudiodefine.h"

class CQuoteItem;

namespace Config
{
	class CTransform;
	class CTransform;
	class CLinearTransform;
	class CNonlinearTransform;
	class CFesData;
}

class CScaleModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN{
		ID, TagName, ScaleName, OccNo, Extention, ScaleConfig
	};
	
	enum TYPE
	{
		LINEAR, NONLINEAR
	};

	CScaleModel(QObject *parent, Config::CTransform *pTransform, CQuoteItem *pItem
		, Config::CFesData *pFesConfig);
	~CScaleModel();

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

	void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

	QList<QVector<QString>> &GetDataList();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	bool InsertRowsInModel(int position, int rows, QSet<int> &set);

	void emitDataChanged(const QModelIndex &index);

	QModelIndex createDataIndex(int row, int col, void *data = 0)
	{
		return QAbstractTableModel::createIndex(row, col, data);
	}

	int GetType()
	{
		return m_nType;
	}

	Config::CLinearTransform * GetLinear();

	Config::CNonlinearTransform *GetNonLinear();

	void SetLinear(Config::CLinearTransform *pLinear);

	void SetType(int nType)
	{
		m_nType = nType;
	}
private:
	void InitPara();

private:
	QStringList horizontal_header_list;

	QMap<int, CONFIG_PARA> m_arrInfo;

	Config::CLinearTransform *m_pScaleLiear;

	Config::CNonlinearTransform *m_pNonScaleLiear;

	//线性 非线性
	int m_nType;

	QMap<int, QString> m_arrVecType;

	//转换结构信息
	Config::CTransform *m_pTransform;

	CQuoteItem *m_pItem;

	Config::CFesData *m_pFesConfig;
};

#endif // SCALELIEARMODEL_H
