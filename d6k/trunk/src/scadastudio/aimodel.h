#ifndef AIMODEL_H
#define AIMODEL_H

#include "scadastudiodefine.h"
#include "datatypes.h"

#include <QAbstractTableModel>

class QUndoStack;

namespace Config
{
	class CAnalogInput;
	class CFesData;
}

class IMainModuleInterface;

class CAIModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, Desc, OccNo, BlockOccNo/*, DataType*/, InitQua, Address, PinLabel, InitValue, Format, Unit, Enable,
		MaxRaw, MinRaw, MaxConvert, MinConvert, ScaleTagName, ScaleType/*, ScaleDesc*/, RangeL, RangeH, HighQua, LowQua,
		SaveDisk, SaveDiskPeriod, SaveDB, SaveDBPeriod, Sensitivity, SensitivityType, AlarmTagName, AlarmType/*, AlarmDesc*/,
		ChannelOccNo, DeviceOccNo, ReferenceCount, DataSource, Express, 
	};

public:
	CAIModel(QObject *parent, std::vector <Config::CAnalogInput *> &arrAIs, Config::CFesData *pFes, IMainModuleInterface *pCore);
	~CAIModel();

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

	QMap<int32s, CONFIG_PARA> &GetColumnInfo();

	std::vector <Config::CAnalogInput *> &GetAIs()
	{
		return m_arrAIs;
	}

private:
	void InitPara();

private:
	QStringList m_lstHorizontalHeader;

	std::vector <Config::CAnalogInput *> & m_arrAIs;

	QMap<int32s, CONFIG_PARA> m_mapInfo;

	Config::CFesData *m_pFes;

	IMainModuleInterface *m_pCore;

	QMap<int, QString> m_mapAlarmType;
	QMap<int, QString> m_mapScaleType;
};

#endif // AIMODEL_H
