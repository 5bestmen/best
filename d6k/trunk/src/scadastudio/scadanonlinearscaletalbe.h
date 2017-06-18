#ifndef SCADANONLINEARSCALETALBE_H
#define SCADANONLINEARSCALETALBE_H

#include <QTableView>

namespace Config
{
	class CScadaTransform;
	class CScadaVariableConfigData;
}
class CQuoteItem;
class CScadaVariableModule;
class CScadaNonlinearScaleModel;

class CScadaNonlinearScaleTable : public QTableView
{
	Q_OBJECT

public:
	CScadaNonlinearScaleTable(QWidget *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
		, Config::CScadaVariableConfigData *pConfig);
	~CScadaNonlinearScaleTable();

	void Refresh();

public slots:
	void DoubleClicked(const QModelIndex & index);



private:
	CScadaNonlinearScaleModel *m_pModel;
	Config::CScadaTransform *m_pTransform;
};

#endif // SCADANONLINEARSCALETALBE_H
