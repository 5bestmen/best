#ifndef _FES_DO_BLoCKINGSIGNALTAGNAME_SELECT_DEVICEDI_MODEL_H
#define _FES_DO_BLoCKINGSIGNALTAGNAME_SELECT_DEVICEDI_MODEL_H

#include "scadastudiodefine.h"

#include <QAbstractTableModel>

namespace Config
{
	class CAnalogInput;
	class CDevice;
}

class CFes_Do_BlockingSignalTag_Select_DeviceDI_Model : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, DoubleDI, Desc, InitValue, InitQua, Address, PinLabel, SignalType, Express
	};
public:
	CFes_Do_BlockingSignalTag_Select_DeviceDI_Model(QObject *parent, Config::CDevice *pDevice);
	~CFes_Do_BlockingSignalTag_Select_DeviceDI_Model();

	void SetHorizontalHeaderList(QStringList horizontalHeaderList);

	void SetVerticalHeaderList(QStringList verticalHeaderList);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void SetModalDatas(QList< QVector<QString> > &rowlist);

	void RefrushModel();

	void emitDataChanged(const QModelIndex &index);

	QModelIndex createDataIndex(int row, int col, void *data = 0)
	{
		return QAbstractTableModel::createIndex(row, col, data);
	}

private:
	void InitPara();
private:
	QStringList horizontal_header_list;
	Config::CDevice *m_pDevice;
	QVector<IDDESC> m_arrInitQua;
	QVector<IDDESC> m_arrSignalType;
};

#endif
