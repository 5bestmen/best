#ifndef NODEFESGROUPTABLE_H
#define NODEFESGROUPTABLE_H

#include <QTableView>

class IMainModuleInterface;
namespace Config
{
	class CNodeGroup;
	class CNodeConfigData;
};

class CNodeFesGroupModel;
class CSortFilterModel;

class CNodeFesGroupTable : public QTableView
{
	Q_OBJECT

public:
	CNodeFesGroupTable(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pFesGroupData
		, Config::CNodeConfigData *pNodeConfig);
	~CNodeFesGroupTable();

	CNodeFesGroupModel *GetModel()
	{
		return m_pModel;
	}

public slots:
	void ShowMouseRightButton(const QPoint& point);

private:
	void AddAnalogPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &index);

private:
	CNodeFesGroupModel *m_pModel;
	CSortFilterModel *m_pSortModel;
};

#endif // NODEFESGROUPTABLE_H
