#ifndef NODEFESGROUPMODEL_H
#define NODEFESGROUPMODEL_H

#include <QAbstractTableModel>

class IMainModuleInterface;
namespace Config
{
	class CNode;
	class CNodeConfigData;
};

class CQuoteItem;

class CNodeFesModel : public QAbstractTableModel
{
	Q_OBJECT

	enum COLUMN
	{
		TagName, HostName, OccNo, BlockNo/*, NodeType*/, Network_IP_A,
		Network_IP_B, IsTimeSource, Program, Config
	};

public:
	CNodeFesModel(QObject *parent, IMainModuleInterface *pCore, Config::CNode *pData
		, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig);
	~CNodeFesModel();
	
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
	Config::CNode *m_pData;
	QStringList m_horizontal_header_list;
	IMainModuleInterface *m_pCore;
	CQuoteItem *m_pItem;
	Config::CNodeConfigData *m_pNodeConfig;
};

#endif // NODEFESGROUPMODEL_H
