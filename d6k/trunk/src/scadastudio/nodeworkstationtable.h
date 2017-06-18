#ifndef NOEDWORKSTATIONTABLE_H
#define NOEDWORKSTATIONTABLE_H

#include <QTableView>

class IMainModuleInterface;

namespace Config
{
	class CNode;
	class CNodeConfigData;
}

class CNodeWorkstationModel;
class CQuoteItem;

class CNodeWorkstationTable : public QTableView
{
	Q_OBJECT

public:
	CNodeWorkstationTable(QWidget *parent, IMainModuleInterface *pCore, Config::CNode *pWorkstationData
		, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig);
	~CNodeWorkstationTable();

	void Refresh();

public slots:
	void DoubleClicked(const QModelIndex & index);

private:
	CNodeWorkstationModel *m_pModel;
	Config::CNode *m_pWorkstationData;
	Config::CNodeConfigData *m_pNodeConfig;
};

#endif // NOEDWORKSTATIONTABLE_H
