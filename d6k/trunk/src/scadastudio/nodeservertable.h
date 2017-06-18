#ifndef NODESERVERTABLE_H
#define NODESERVERTABLE_H

#include <QTableView>

class IMainModuleInterface;
namespace Config
{
	class CNodeServer;
	class CNodeConfigData;
};

class CNodeServerModel;
class CSortFilterModel;

class CNodeServerTable : public QTableView
{
	Q_OBJECT

public:
	CNodeServerTable(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeServer *pServerData
		, Config::CNodeConfigData *pNodeConfig);
	~CNodeServerTable();

public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex & index);

private:
	void AddAnalogPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &index);

private:
	CNodeServerModel *m_pModel;
	CSortFilterModel *m_pSortModel;
	Config::CNodeConfigData *m_pNodeConfig;
	Config::CNodeServer *m_pServerData;
};

#endif // NODESERVERTABLE_H
