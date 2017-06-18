/*
#ifndef POWER_PREDICT_SP_H
#define POWER_PREDICT_SP_H

#include <QDialog>

#include <QStandardItemModel>           //12.27

#include "ui_powerpredictsp.h"

class QSqlQueryModel;
class QTreeWidgetItem;



namespace Config            //12.27
{
	class CFesConfigData;
	class CFesData;
	class CDevice;
	class CFesGroup;
	class CChannelGroup;
}


class CQuoteItem;

class PowerPredictSP : public QDialog
{
	Q_OBJECT

public:

	enum Type
	{
		Digital,
		Analogue,
		SetPoint,
		Relay
	};

	class Item
	{
	public:
		Item(const QString & n, int s, int d, int p, int t)
		{
			name = n;
			station = s;
			rtu = d;
			index = p;
			type = t;
		}

		int     station;
		int     rtu;
		int     index;
		int     type;
		QString name;
	};

	PowerPredictSP(Type typ, QWidget *parent = 0, Config::CFesConfigData *pConfig);

	~PowerPredictSP();

	QList<Item *> getItems();

	bool isChoosed(Item * item);

protected:
	void init();

	void appendStation(int id, const QString & name);

	void appendDevice(int station, int device, const QString & name, QTreeWidgetItem * parent);

	void addItem(const QModelIndex & index, int station, int rtu, int type);

protected slots:

    //void currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem);

	void add();

	void addAll();

	void remove();

	void removeAll();

private:

	void CreateUi(Config::CFesConfigData *pConfig);     //12.27

	void CreateGroupUi(Config::CFesGroup *pFesGroup, CQuoteItem *pItem);  //12.27

private:

	QSqlQueryModel * model;

	Ui::PowerPredictSP * ui;

	Type  type;

	QList<Item *>  items;

	QStandardItemModel * m_pModel;     //12.27

	Config::CFesConfigData *m_pConfig;
};
#endif  //POWER_PREDICT_SP_H*/