#ifndef SYSTEMVARIABLETABLE_H
#define SYSTEMVARIABLETABLE_H

#include <QTableView>

namespace Config
{
	class CFesData;
}

class IMainModuleInterface;
class CSortFilterModel;
class CSystemVariableModel;

class CSystemVariableTable : public QTableView
{
	Q_OBJECT

public:
	CSystemVariableTable(QWidget *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData);
	~CSystemVariableTable();

private:
	CSortFilterModel *m_pSortModel;

	CSystemVariableModel *m_pModel;
};

#endif // SYSTEMVARIABLETABLE_H
