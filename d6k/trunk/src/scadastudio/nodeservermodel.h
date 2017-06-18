#ifndef NODESERVERMODEL_H
#define NODESERVERMODEL_H

#include <QAbstractTableModel>

class IMainModuleInterface;
namespace Config
{
	class CNodeServer;
	class CNode;
	class CNodeConfigData;
};

class QPushButton;
class CNodeServerModel : public QAbstractTableModel
{
	Q_OBJECT


public:
	enum COLUMN
	{
		TagName, HostName, OccNo, BlockNo/*, NodeType*/, Network_IP_A,
		Network_IP_B, IsTimeSource, Program, Config, Service
	};

	CNodeServerModel(QObject *parent, IMainModuleInterface *pCore, Config::CNodeServer *pServerData
		, Config::CNodeConfigData *pNodeConfig);
	~CNodeServerModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	Qt::ItemFlags flags(const QModelIndex &index) const;

	int GetSize();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());
private:
	void InitPara();

private:
	QStringList m_horizontal_header_list;
	//std::vector<Config::CNode *> m_arrNodes;
	Config::CNodeServer *m_pServerData;
	IMainModuleInterface *m_pCore;
	Config::CNodeConfigData *m_pNodeConfig;
	QMap<QModelIndex, QPushButton*> m_btns;
};

#endif // NODESERVERMODEL_H
