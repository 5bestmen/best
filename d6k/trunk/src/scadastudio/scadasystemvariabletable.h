#ifndef SCADASYSTEMVARIABLETABLE_H
#define SCADASYSTEMVARIABLETABLE_H

#include <QTableView>

namespace Config
{
	class CScadaVariableConfigData;
	class CScadaData;
}

class IMainModuleInterface;
class CSortFilterModel;
class CScadaSystemVariableModel;

class CScadaSystemVariableTable : public QTableView
{
	Q_OBJECT

public:
	CScadaSystemVariableTable(QWidget *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pConfigData
		, Config::CScadaData *pData);
	~CScadaSystemVariableTable();

	CSortFilterModel *GetSortModel()
	{
		return m_pSortModel;
	}

private:
	CSortFilterModel *m_pSortModel;

	CScadaSystemVariableModel *m_pModel;
};

#endif // SCADASYSTEMVARIABLETABLE_H
