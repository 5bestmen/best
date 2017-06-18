#ifndef SELECTSCALEDIALOG_H
#define SELECTSCALEDIALOG_H

#include <QDialog>
#include "ui_selectscaledialog.h"

namespace Config
{
	class CFesData;

	class CAnalogInput;
	class CTransformGroup;
}

class QStandardItemModel;
class CQuoteItem;
class QTreeView;
class CSelectScaleTable;

class CSelectScaleDialog : public QDialog
{
	Q_OBJECT

	enum{
		ScaleType, MinRaw, MaxRaw, Min, Max, Inverse, Gain, Offset
	};

public:
	CSelectScaleDialog(QWidget *parent, Config::CFesData *pFes, QString strScaleName
		, int ScaleType, bool bNewPoint, int nType);
	~CSelectScaleDialog();

	int GetType()
	{
		return m_nScaleType;
	}

	QString GetTagName()
	{
		return m_strTagName;
	}

	QString GetDesc()
	{
		return m_strDesc;
	}

private:
	void InitTransform(QStandardItemModel *pModel, Config::CFesData *pFesData);
	void InitLinearTransform(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData, Config::CAnalogInput *pAI);
	void InitNonlinearTransform(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData);
	bool InitTransformGroup(CQuoteItem *pItem, Config::CTransformGroup *pGroup);
	bool InitNonTransformGroup(CQuoteItem *pItem, Config::CTransformGroup *pGroup);

	void InitTable(Config::CFesData *pFes);

	void DumpModel(const QAbstractItemModel * M, const QModelIndex & Top, const QString &S);

public slots:
	void Clicked(const QModelIndex &index);
	void OnOK();
	void OnCancel();


private:
	Ui::CSelectScaleDialog ui;

	Config::CFesData *m_pFesData;

	int m_nScaleType;
	QString m_strTagName;
	QString m_strDesc;
	bool m_bNewPointScale;

	QStandardItemModel *m_pModel;

	Config::CAnalogInput *m_pAI;
};

#endif // SELECTSCALEDIALOG_H
