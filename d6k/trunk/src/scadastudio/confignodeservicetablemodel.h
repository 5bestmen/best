#ifndef CONFIGNODESERVICETABLEMODEL_H
#define CONFIGNODESERVICETABLEMODEL_H

#include <QAbstractTableModel>

namespace Config
{
	class CNodeConfigData;
	struct NODE_SERVICE_ROLE;
	class CNode;
}

class CConfigNodeServiceTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CConfigNodeServiceTableModel(QObject *parent, Config::CNodeConfigData *pConfig, Config::CNode *pNode);
	~CConfigNodeServiceTableModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	std::vector<Config::NODE_SERVICE_ROLE *> m_arrNodeService;

	std::vector<Config::NODE_SERVICE_ROLE *> &GetArr()
	{
		return m_arrVec;
	}

	void appendData(std::vector<Config::NODE_SERVICE_ROLE *> &arrVec);
	void deleteData(Config::NODE_SERVICE_ROLE *pData);

	void RefrushModel();

private:
	Config::CNodeConfigData *m_pConfig;
	std::vector<Config::NODE_SERVICE_ROLE *> m_arrVec;
};

#endif
