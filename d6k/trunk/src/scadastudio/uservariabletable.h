#ifndef USERVARIABLETABLE_H
#define USERVARIABLETABLE_H

#include <QTableView>

class IMainModuleInterface;

namespace Config
{
	class CUserVariableGroup;
	class CFesData;
}

class CSortFilterModel;
class CUserVariableModel;
class IMainModuleInterface;

class CUserVariableTable : public QTableView
{
	Q_OBJECT

public:
	CUserVariableTable(QWidget *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData, Config::CUserVariableGroup *pVariable);
	~CUserVariableTable();

	CUserVariableModel *GetModel()
	{
		return m_pModel;
	}

	CSortFilterModel *GetSortModel()
	{
		return m_pSort;
	}

	void RefrushModel();

private:
	void AddUserVariablePoint(QModelIndex &index);
	void DeleteUserVariablePoint(QModelIndex &index);

private slots:
	void DoubleClicked(const QModelIndex & index);
	void ShowMouseRightButton(const QPoint& point);
private:
	CSortFilterModel *m_pSort;
	CUserVariableModel *m_pModel;
	IMainModuleInterface *m_pCore;
	Config::CFesData *m_pFesData;
};

#endif // USERVARIABLETABLE_H
