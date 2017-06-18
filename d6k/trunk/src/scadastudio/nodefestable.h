#ifndef NODEFESGROUPTABLE_H
#define NODEFESGROUPTABLE_H

#include <QTableView>

class IMainModuleInterface;
namespace Config
{
	class CNode;
	class CNodeConfigData;
};

class CNodeFesModel;
class CSortFilterModel;
class CQuoteItem;

class CNodeFesTable : public QTableView
{
	Q_OBJECT

public:
	CNodeFesTable(QWidget *parent, IMainModuleInterface *pCore, Config::CNode *pData
		, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig);
	~CNodeFesTable();

	CNodeFesModel *GetModel()
	{
		return m_pModel;
	}

public slots:
	void ShowMouseRightButton(const QPoint& point);

private:
	void AddAnalogPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &index);
private:
	CNodeFesModel *m_pModel;
	CSortFilterModel *m_pSortModel;
};

#endif // NODEFESGROUPTABLE_H
