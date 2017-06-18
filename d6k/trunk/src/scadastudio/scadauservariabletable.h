#ifndef SCADAUSERVARIABLETABLE_H
#define SCADAUSERVARIABLETABLE_H

#include <QTableView>

namespace Config
{
	class CScadaVariableConfigData;
	class CScadaUserVariableGroup;
	class CScadaData;
}

class IMainModuleInterface;
class CScadaUserVariableModel;
class CSortFilterModel;

class CScadaUserVariableTable : public QTableView
{
	Q_OBJECT

public:
	CScadaUserVariableTable(QWidget *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pScadaVirableConfig
		, Config::CScadaUserVariableGroup *pGroup, Config::CScadaData *pScada);
	~CScadaUserVariableTable();

	CScadaUserVariableModel *GetModel()
	{
		Q_ASSERT(m_pModel);
		if (!m_pModel)
		{
			return nullptr;
		}
		
		return m_pModel;
	}

	CSortFilterModel *GetSortModel()
	{
		Q_ASSERT(m_pSort);
		if (!m_pSort)
		{
			return nullptr;
		}

		return m_pSort;
	}

	void Refresh();

private slots:
	void DoubleClicked(const QModelIndex & index);
	void ShowMouseRightButton(const QPoint& point);

private:
	void AddUserVariablePoint(QModelIndex &index);
	void DeleteUserVariablePoint(QModelIndex &index);

	Config::CScadaVariableConfigData *m_pScadaVirableConfig;

private:
	CScadaUserVariableModel *m_pModel;
	CSortFilterModel *m_pSort;
	IMainModuleInterface *m_pCore;
	Config::CScadaData *m_pScada;
};

#endif // SCADAUSERVARIABLETABLE_H
