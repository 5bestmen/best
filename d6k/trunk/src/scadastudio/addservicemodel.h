#ifndef ADDSERVICEMODEL_H
#define ADDSERVICEMODEL_H

#include <QAbstractTableModel>

namespace Config
{
	class CNodeConfigData;
	struct NODE_SERVICE_ROLE;
}

class CAddServiceModel : public QAbstractTableModel
{
	Q_OBJECT

	enum
	{
		Name, LoadType, Argument, Description
	};

public:
	CAddServiceModel(QObject *parent, Config::CNodeConfigData *pConfig);
	~CAddServiceModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	std::vector<Config::NODE_SERVICE_ROLE *> m_arrNodeService;

	std::vector<Config::NODE_SERVICE_ROLE *> &GetArr()
	{
		return m_arrNodeService;
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
	void InitPara();


private:
	Config::CNodeConfigData *m_pConfig;
	QStringList m_horizontal_header_list;
};

#endif
