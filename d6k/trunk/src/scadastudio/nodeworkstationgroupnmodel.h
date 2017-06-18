#ifndef NODEWORKSTATIONGROUPMODEL_H
#define NODEWORKSTATIONGROUPMODEL_H

#include <QAbstractTableModel>

class IMainModuleInterface;

namespace Config
{
	class CNodeGroup;
	class CNodeConfigData;
}

class CNodeWorkstationGroupModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum COLUMN
	{
		TagName, HostName, OccNo, BlockNo/*, NodeType*/, Network_IP_A,
		Network_IP_B, IsTimeSource, Program, Config, Service
	};

public:

	CNodeWorkstationGroupModel(QObject *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pWorkstationData
		, Config::CNodeConfigData *pNodeConfig);
	~CNodeWorkstationGroupModel();

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
	Config::CNodeGroup *m_pWorkstationData;
	QStringList m_horizontal_header_list;
	IMainModuleInterface *m_pCore;
	Config::CNodeConfigData *m_pNodeConfig;
};

#endif // NODEWORKSTATIONGROUPMODEL_H
