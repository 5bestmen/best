#ifndef FESSELECTPOINTMODEL_H
#define FESSELECTPOINTMODEL_H

#include <QAbstractTableModel>

namespace Config
{
	class CDevice;
	class CUserVariableGroup;
	class CFesData;
}

class CFesSelectPointModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, Description, OccNo, DataType
	};

public:
	CFesSelectPointModel(QObject *parent, Config::CDevice *pDevice, int nType);
	CFesSelectPointModel(QObject *parent, Config::CUserVariableGroup *pUser, int nType);
	CFesSelectPointModel(QObject *parent, Config::CFesData *pFes, int nType);
	~CFesSelectPointModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	Qt::ItemFlags flags(const QModelIndex &index) const;

private:
	QStringList m_horizontal_header_list;
	Config::CDevice *m_pDevice;
	Config::CUserVariableGroup *m_pUserContainer;
	Config::CFesData *m_pFes;
	int m_nType;
};

#endif // FESSELECTPOINTMODEL_H
