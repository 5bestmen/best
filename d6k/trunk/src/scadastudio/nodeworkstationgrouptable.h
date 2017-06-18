#ifndef NOEDWORKSTATIONGROUPTABLE_H
#define NOEDWORKSTATIONGROUPTABLE_H

#include <QTableView>

class IMainModuleInterface;

namespace Config
{
	class CNodeGroup;
	class CNodeConfigData;
}

class CNodeWorkstationGroupModel;
class CSortFilterModel;

class CNodeWorkstationGroupTable : public QTableView
{
	Q_OBJECT

public:
	CNodeWorkstationGroupTable(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pWorkstationData
		, Config::CNodeConfigData *pNodeConfig);
	~CNodeWorkstationGroupTable();

public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex & index);

private:
	void AddAnalogPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &index);
private:
	CNodeWorkstationGroupModel *m_pModel;
	CSortFilterModel *m_pSortModel;
	Config::CNodeGroup *m_pWorkstationData;
	Config::CNodeConfigData *m_pNodeConfig;
};

#endif // NOEDWORKSTATIONGROUPTABLE_H
