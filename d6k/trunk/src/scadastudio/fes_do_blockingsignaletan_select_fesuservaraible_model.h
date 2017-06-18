#ifndef _FES_DO_BLoCKINGSIGNALTAGNAME_SELECT_FESUSERVARAIBLE_MODEL_H
#define _FES_DO_BLoCKINGSIGNALTAGNAME_SELECT_FESUSERVARAIBLE_MODEL_H

#include "scadastudiodefine.h"

#include <QAbstractTableModel>

namespace Config
{
	class CUserVariableGroup;
}

class CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, Description, OccNo, DataType, InitValue, InitialQua, Address, PinLabel, SourceTagName, Scale, Alarm
	};
public:
	CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model(QObject *parent, Config::CUserVariableGroup *pVariable);
	~CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model();

public:


	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

private:
	void InitPara();
private:
	QStringList m_horizontal_header_list;
	Config::CUserVariableGroup *m_pVariable;
	QVector<IDDESC> m_arrVecDataType;
	QVector<IDDESC> m_arrInitQua;
};

#endif //_FES_DO_BLoCKINGSIGNALTAGNAME_SELECT_FESUSERVARAIBLE_MODEL_H
