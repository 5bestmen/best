#ifndef FORWARDING_AIMODEL_H
#define FORWARDING_AIMODEL_H

#include <QAbstractTableModel>
#include "scadastudiodefine.h"
#include "datatypes.h"

class QUndoStack;

namespace Config
{
	class CForwardingChannelData;
	class CForwardingAnalogInputData;
	class CFesData;
}

class IMainModuleInterface;

class CForwardingAIModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, Desc, SourceTagName, OccNo, BlockOccNo, /*组号*/GroupNo, /*条目号*/EntryNo, /*地址*/Address, FUN, INF, MaxRaw, MinRaw, MaxConvert, MinConvert, ScaleTagName, ScaleType, HLimit,
		LLimit, Threshold
	};

public:
	CForwardingAIModel(QObject *parent, Config::CForwardingChannelData *pChannel, Config::CFesData *pFes, IMainModuleInterface *pCore);
	~CForwardingAIModel();

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

	std::vector <Config::CForwardingAnalogInputData *> &GetAIs();

private:
	void InitPara();

private:
	QStringList horizontal_header_list;

	Config::CForwardingChannelData *m_pChannel;

	QMap<int32s, CONFIG_PARA> m_arrInfo;

	Config::CFesData *m_pFes;

	IMainModuleInterface *m_pCore;

	QMap<int, QString> m_arrVecAlarmType;
	QMap<int, QString> m_arrVecScaleType;
};

#endif // FORWARDING_AIMODEL_H
