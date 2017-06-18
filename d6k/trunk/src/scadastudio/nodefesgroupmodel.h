#ifndef NODEFESGROUPMODEL_H
#define NODEFESGROUPMODEL_H

#include <QAbstractTableModel>

class IMainModuleInterface;
namespace Config
{
	class CNodeGroup;
	class CNodeConfigData;
};

class CNodeFesGroupModel : public QAbstractTableModel
{
	Q_OBJECT

	enum COLUMN
	{
		TagName, HostName, OccNo, BlockNo/*, NodeType*/, Network_IP_A,
		Network_IP_B, IsTimeSource, Program, Config
	};

public:
	CNodeFesGroupModel(QObject *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pFesGroupData
		, Config::CNodeConfigData *pNodeConfig);
	~CNodeFesGroupModel();
	
	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	Qt::ItemFlags flags(const QModelIndex &index) const;

	int GetSize();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	void RefrushModel();
private:
	void InitPara();
private:
	Config::CNodeGroup *m_pFesGroupData;
	QStringList m_horizontal_header_list;
	IMainModuleInterface *m_pCore;
	Config::CNodeConfigData *m_pNodeConfig;
};

#endif // NODEFESGROUPMODEL_H
