#ifndef DIMODEL_H
#define DIMODEL_H

#include "datatypes.h"
#include "scadastudiodefine.h"
#include <QAbstractTableModel>

class QUndoStack;

namespace Config
{
	class CDigitalInput;
	class CFesData;
}

class IMainModuleInterface;

class CDIModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, DoubleDI, OccNo, BlockOccNo/*, DataType*/, Desc, InitValue, InitQua, Address, PinLabel,
		SignalType, SOE, BitCloseString, BitOpenString, AlarmTagName/*, AlarmType*/, SaveDisk, SaveDiskPeriod, SaveDB, SaveDBPeriod, ReferenceCount, DataSource,
		ChannelOccNo, DeviceOccNo, Express
	}; 

public:
	CDIModel(QObject *parent, std::vector <Config::CDigitalInput *> &arrDIs, Config::CFesData *pFes, IMainModuleInterface *pCore);
	~CDIModel();

	QMap<int32s, CONFIG_PARA> &GetColumnInfo();

	std::vector <Config::CDigitalInput *> &GetDIs()
	{
		return m_arrDIs;
	}

	void RefrushModel()
	{
		beginResetModel();
		endResetModel();
	}

public:
	void SetHorizontalHeaderList(QStringList horizontalHeaderList);

	void SetVerticalHeaderList(QStringList verticalHeaderList);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void SetModalDatas(QList< QVector<QString> > &rowlist);

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	//void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

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

	//std::vector <Config::CAnalogInput *> &GetAIs()
	//{
	//	return m_arrAIs;
	//}

private:
	void InitPara();

private:
	QStringList m_lstHorizontalHeader;

	std::vector <Config::CDigitalInput *> &m_arrDIs;

	QMap<int32s, CONFIG_PARA> m_mapInfo;

	Config::CFesData *m_pFes;

	IMainModuleInterface *m_pCore;

	QMap<int, QString> m_mapAlarmType;
};

#endif // DIMODEL_H
