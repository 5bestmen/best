#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>
#include <QSet>
#include <QUndoStack>
#include "datatypes.h"
#include "scadastudiodefine.h"
#include "config_data.h"

class CQuoteItem;
class IMainModuleInterface;

class CForwardingChannelModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum ITEM{NORMALITEM,MODIFIEDITEM};

	enum COLUMN{
		ID, OccNo, TagName, ChannelName, DriverName, DriverPara, ChannelParameter, /*DriverType, Medium, DeviceAddress, COM, Baud*/ Extention, GroupName
	};

	enum MEDIUM{
		INTERNET, SERIALPORT
	};

	enum DRIVERTYPE{
		PROCEEDOUT, PROCEEDIN
	};

	enum DRIVERNAME{
		DRIVER_101, DRIVER_104, DRIVER_MODBUS, DRIVER_103
	};

public:
	CForwardingChannelModel(QObject *parent, Config::CForwardingChannelData *pChannel, CQuoteItem *pItem, Config::CFesData *pFes, IMainModuleInterface *pCore);
	~CForwardingChannelModel();

	void SetHorizontalHeaderList(QStringList horizontalHeaderList);

	void SetVerticalHeaderList(QStringList verticalHeaderList);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;  

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void SetModalDatas(QList< QVector<QString> > &rowlist);

	void RefrushModel();

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	void sort (int column, Qt::SortOrder order = Qt::AscendingOrder);

	QList<QVector<QString>> &GetDataList();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	bool InsertRowsInModel(int position, int rows, QSet<int> &set);

	void emitDataChanged(const QModelIndex &index);

	QModelIndex createDataIndex(int row, int col, void *data = 0)
	{
		return QAbstractTableModel::createIndex(row, col, data);
	}

	QUndoStack *undoStackTest();

	void SaveDataModel(QString tableName);

	void AddRowModel(QVector<QString> &rowList, int rowType);

	void GetColumInfo(QMap<QString, QString> &desc2RealField);

	void RemoveRowModel(QVector<QString> &rowList, int rowType);

	void InitPara();

	QMap<int32s, CONFIG_PARA> &GetColumnInfo();

	Config::CForwardingChannelData *GetChannelData();

	QVector<IDDESC> &GetDriverName();

	QVector<IDDESC> &GetDriverPara101();

	QVector<IDDESC> &GetDriverPara104();

	QVector<IDDESC> &GetDriverParaModbus();

	QVector<IDDESC> &GetMedium();

	QVector<IDDESC> &GetCOM();

	QVector<IDDESC> &GetBaud();

	CQuoteItem *GetCurrentTreeItem();

private:
	//bool IsChannelTagNameExist(QString &strTagName);

public slots:
	void OnClickedView(const QModelIndex &index);
signals :
	//void SendChangeChannelModelRows(QSet<int32s> &set, int32s nType, CChannelModel *pModel);
public:
	QStringList horizontal_header_list;
	QStringList vertical_header_list;
	QList< QVector<QString> > arr_row_list;
	QList< QVector<int> > m_itemStatus;

	QUndoStack *m_stack;

	QList<int> m_rowStatus;

	QMap<QString, QString> m_desc2RealField;

private:
	QMap<int32s, CONFIG_PARA> m_arrInfo;

	Config::CForwardingChannelData *m_pChannel;
	Config::CFesData *m_pFesData;
	CQuoteItem *m_pItem;

	QMap<int32s, IDDESC> m_arrVec;
	QVector<IDDESC> m_arrDriverName;
	QVector<IDDESC> m_arrDriverPara101;
	QVector<IDDESC> m_arrDriverPara104;
	QVector<IDDESC> m_arrDriverParaModbus;
	QVector<IDDESC> m_arrMedium;
	QVector<IDDESC> m_arrCOM;
	QVector<IDDESC> m_arrBaud;

	IMainModuleInterface *m_pCore;
};

#endif // TABLEMODEL_H
