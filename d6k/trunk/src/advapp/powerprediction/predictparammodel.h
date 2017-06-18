#ifndef PREDICT_PARAM_MODEL_H
#define PREDICT_PARAM_MODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QVariant>

#include "datatypes.h"
#include "scadastudio/icore.h"
#include "powerpredictdefine.h"
#include "predictdatainfo.h"

class CPredictParamModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum ParamColumn 
	{
		ID, TagName, Desc, OccNo, Value,
	};

public:
	CPredictParamModel(QObject* parent, std::vector <CParamInput *> &vecAi, IMainModuleInterface* pCore);

	~CPredictParamModel();

	void SetHorizontalHeaderList(QStringList horizontalHeaderList);

	void SetVerticalHeaderList(QStringList verticalHeaderList);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void RefrushModel();

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	
	std::vector <CParamInput *> &GetAis()
	{
		return m_vecParam;
	}

private:

	void InitPara();

	QStringList horizontal_header_list;

	IMainModuleInterface* m_pCore;

	QMap<int32s, Predict_Attributes> m_mapInfo;

	std::vector <CParamInput*> &m_vecParam;

};
#endif //PREDICT_PARAM_MODEL_H
