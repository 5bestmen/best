#ifndef PREDICT_AI_MODEL_H
#define PREDICT_AI_MODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QVariant>

#include "powerpredictdefine.h"
#include "datatypes.h"
//#include "powerpredictconfig.h"
#include "scadastudio/icore.h"
#include "predictdatainfo.h"




class CPredictAiModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum AiColumn
	{
		ID, TagName, Desc, OccNo,
	};

public:
	CPredictAiModel(QObject* parent, std::vector <CAiInput *> &vecAi, IMainModuleInterface* pCore);

	~CPredictAiModel();

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

	std::vector <CAiInput *> &GetAis()
	{
		return m_vecAI;
	}

private:

	void InitPara();

private:
	QStringList horizontal_header_list;

	IMainModuleInterface* m_pCore;

	QMap<int32s, Predict_Attributes> m_mapInfo;
	
	std::vector <CAiInput*> &m_vecAI;

};


#endif  //PREDICT_AI_MODEL_H

