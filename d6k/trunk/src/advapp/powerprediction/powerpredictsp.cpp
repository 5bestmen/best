/*
#include "powerpredictsp.h"
#include "ui_powerpredictsp.h"
#include "powerpredictdefine.h"
#include "../../scadastudio/scadastudiodefine.h"
#include "../../../include/scadastudio/define.h"
#include "../../../include/scadastudio/quoteItem.h"

#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QStringList>
#include <QTreeWidget>
#include <QStringList>

namespace Qt
{
	enum ItemDataExpandRole
	{
		TypeRole    = UserRole + 1,
		StationRole = UserRole + 2,
		DeviceRole  = UserRole + 3,
		PointRole   = UserRole + 4
	};
}

#define TYPE_STATION   0x01
#define TYPE_DEVICE    0x02
#define TYPE_DIGITAL   0x04
#define TYPE_ANALOGUE  0x08
#define TYPE_SETPOINT  0x10
#define TYPE_RELAY     0x20


PowerPredictSP::PowerPredictSP(PowerPredictSP::Type typ, QWidget *parent, Config::CFesConfigData *pConfig)
	:QDialog(parent)
	, ui(new Ui::PowerPredictSP)
	, type(typ)
	, m_pModel(nullptr)
	, m_pConfig(nullptr)
{

	Q_ASSERT(pConfig);

	if (!pConfig)
	{
		return;
	}

	ui->setupUi(this);

	init();


	Qt::WindowFlags flags = windowFlags();
	flags &= ~Qt::WindowContextHelpButtonHint;
	setWindowFlags(flags);

	m_pConfig = pConfig;

	CreateUi(m_pConfig);
}

PowerPredictSP::~PowerPredictSP()
{

}

void PowerPredictSP::init()
{
	connect(ui->add_btn, SIGNAL(clicked()), this, SLOT(add()));
	connect(ui->addall_btn, SIGNAL(clicked()), this, SLOT(addAll()));
	connect(ui->remove_btn, SIGNAL(clicked()), this, SLOT(remove()));
	connect(ui->removeall_btn, SIGNAL(clicked()), this, SLOT(removeAll()));
}


void PowerPredictSP::add()
{
	QTreeWidgetItem * current = ui->tree->currentItem();
	if (!current)  return;

	int type = current->data(0, Qt::TypeRole).toInt();
	int station = current->data(0, Qt::StationRole).toInt();
	int rtu = current->data(0, Qt::DeviceRole).toInt();

	int kind;
	switch (type)
	{
	case TYPE_DIGITAL:  kind = Digital; break;
	case TYPE_ANALOGUE: kind = Analogue; break;
	case TYPE_SETPOINT: kind = SetPoint; break;
	case TYPE_RELAY:    kind = Relay;   break;
	default:
		return;
	}

	QModelIndex index = ui->table->currentIndex();
	addItem(index, station, rtu, kind);
}

void PowerPredictSP::addAll()
{
	QTreeWidgetItem * current = ui->tree->currentItem();
	if (!current) return;

	int type    = current->data(0, Qt::TypeRole).toInt();
	int station = current->data(0, Qt::StationRole).toInt();
	int rtu     = current->data(0, Qt::DeviceRole).toInt();

	int kind;
	switch (type)
	{
	case TYPE_DIGITAL  : kind = Digital; break;
	case TYPE_ANALOGUE : kind = Analogue; break;
	case TYPE_SETPOINT : kind = SetPoint; break;
	case TYPE_RELAY    : kind = Relay; break;
	default:
		break;
	}

	for (int i = 0; i < model->rowCount(); i++)
	{
		QModelIndex index = model->index(i, 0);
		addItem(index, station, rtu, kind);
	}

}


void PowerPredictSP::remove()
{
	QTableWidgetItem * tableItem = ui->point->currentItem();
	if (!tableItem) return;

	Item * item = (Item*)tableItem->data(Qt::UserRole).value<void*>();
	if (!item) return;

	ui->point->removeRow(tableItem->row());

	items.removeOne(item);
	delete item;
}


void PowerPredictSP::removeAll()
{
	ui->point->setRowCount(0);
	qDeleteAll(items);
	items.clear();
}


void PowerPredictSP::addItem(const QModelIndex & index, int station, int rtu, int type)
{
	if (!index.isValid())  return;

	
}


void PowerPredictSP::CreateUi(Config::CFesConfigData *pConfig)
{
	m_pModel = new QStandardItemModel(this);

	ui->tree->setModel(m_pModel);

	QString strTitleName = QObject::tr("Choose Point");

	m_pModel->setHorizontalHeaderItem(QStringList() << strTitleName);

	auto &arrVec = pConfig->GetFecArr();

	auto pTopPowerPredictItem = new CQuoteItem(tr("fes config"));

	pTopPowerPredictItem->setData(DEVICE_PNG, Qt::UserRole);
	pTopPowerPredictItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pTopPowerPredictItem->setData(reinterpret_cast<long long>(m_pConfig->m_pFesGroup), POINTERDATA);
	pTopPowerPredictItem->setIcon(QIcon(DEVICE_PNG));
	pTopPowerPredictItem->setEditable(false);

	m_pModel->appendRow(pTopPowerPredictItem);

	//CreateGroupUi(m_pConfig->m_pFesGroup, pTopPowerPredictItem);



}*/