#ifndef DOMODEL_H
#define DOMODEL_H

#include <QAbstractTableModel>
#include "scadastudiodefine.h"
#include "datatypes.h"

class QUndoStack;

namespace Config
{
	class CDigitalOutput;
	class CFesData;
}

class IMainModuleInterface;

class CDOModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, OccNo, BlockOccNo, /*DataType,*/ Desc, InitValue, InitQua, Address, PinLabel,
		IsSBO, TelCtlWaitTime, TelCtlSendTime, TelCtlExecTime, AssociatedDI, AssociatedDIValType, BlockingSignalTag,
		BlockingProgram, StartProgram, ReferenceCount, ChannelOccNo, DeviceOccNo
	};

public:
	CDOModel(QObject *parent, std::vector <Config::CDigitalOutput *> &arrDOs, Config::CFesData *pFes, IMainModuleInterface *pCore);
	~CDOModel();

	QMap<int32s, CONFIG_PARA> &GetColumnInfo();

	std::vector <Config::CDigitalOutput *> &GetDOs()
	{
		return m_arrDOs;
	}

	void RefrushModel()
	{
		beginResetModel();
		endResetModel();
	}

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

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

	std::vector <Config::CDigitalOutput *> &GetAIs()
	{
		return m_arrDOs;
	}

private:
	void InitPara();

private:
	QStringList m_lstHorizontalHeader;

	std::vector <Config::CDigitalOutput *> &m_arrDOs;

	QMap<int32s, CONFIG_PARA> m_mapInfo;

	Config::CFesData *m_pFes;

	IMainModuleInterface *m_pCore;
};

#endif // DOMODEL_H
