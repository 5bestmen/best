#ifndef SCADALINEARTABLE_H
#define SCADALINEARTABLE_H

#include <QTableView>

namespace Config
{
	class CScadaTransform;
	class CScadaVariableConfigData;
	class CScadaData;
}
class CQuoteItem;
class CScadaVariableModule;
class CScadaLinearScaleModel;
class CSortFilterModel;

class CScadaLinearScaleTable : public QTableView
{
	Q_OBJECT

public:
	CScadaLinearScaleTable(QWidget *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
		, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada);
	~CScadaLinearScaleTable();

	void Refresh();

public slots:
	void DoubleClicked(const QModelIndex & index);

private:
	CScadaLinearScaleModel *m_pModel;
	CSortFilterModel *m_pSort;
	Config::CScadaTransform *m_pTransform;
};

#endif // SCADALINEARTABLE_H
