#ifndef PREDICT_DI_MODEL_H
#define PREDICT_DI_MODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QMap>
#include <QVariant>

//#include "powerpredictconfig.h"
#include "datatypes.h"
#include "powerpredictdefine.h"
#include "scadastudio/icore.h"
#include "predictdatainfo.h"


class CPredictDiModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum DiColumn
	{
		ID, TagName, Desc, OccNo,
	};


public:
	CPredictDiModel(QObject* parent, std::vector <CDiInput *> &vecDi, IMainModuleInterface* pCore);

	~CPredictDiModel();

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

	std::vector <CDiInput *> &GetDis()
	{
		return m_vecDI;
	}

private:
	void InitPara();

private:

	QStringList horizontal_header_list;

	IMainModuleInterface* m_pCore;

	QMap<int32s, Predict_Attributes> m_mapInfo;

	std::vector <CDiInput*> &m_vecDI;

};



#endif  //PREDICT_DI_MODEL_H
