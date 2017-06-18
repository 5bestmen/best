#ifndef SCADAUSERVARIABLESELECTSCALEDIALOG_H
#define SCADAUSERVARIABLESELECTSCALEDIALOG_H

#include <QDialog>
#include "ui_scadauservariableselectscaledialog.h"

namespace Config
{
	class CScadaVariableConfigData;
	class CScadaTransformGroup;
	class CScadaData;
}

class QStandardItemModel;
class CQuoteItem;

class CScadaUserVariableSelectScaleDialog : public QDialog
{
	Q_OBJECT

	enum
	{
		ScaleType, MinRaw, MaxRaw, Min, Max, Inverse, Gain, Offset
	};

public:
	CScadaUserVariableSelectScaleDialog(QWidget *parent, Config::CScadaVariableConfigData *pConfig, 
		QString strScaleName, Config::CScadaData *pScada);
	~CScadaUserVariableSelectScaleDialog();

	int GetType()
	{
		return m_nScaleType;
	}

	QString GetTagName()
	{
		return m_strTagName;
	}

public slots:
	void Clicked(const QModelIndex &index);
	void OnOK();
	void OnCancel();

private:
	void InitTransform(QStandardItemModel *pModel, Config::CScadaData *pScada);
	bool InitTransformGroup(CQuoteItem *pItem, Config::CScadaTransformGroup *pGroup);
	bool InitNonTransformGroup(CQuoteItem *pItem, Config::CScadaTransformGroup *pGroup);

	void DumpModel(const QAbstractItemModel * M, const QModelIndex & Top, const QString &S);
private:
	Ui::CScadaUserVariableSelectScaleDialog ui;
	QStandardItemModel *m_pModel;
	Config::CScadaVariableConfigData *m_pConfig;
	int m_nScaleType;
	QString m_strTagName;
	QString m_strDesc;
};

#endif // SCADAUSERVARIABLESELECTSCALEDIALOG_H
