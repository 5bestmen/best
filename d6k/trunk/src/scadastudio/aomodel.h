#ifndef AOMODEL_H
#define AOMODEL_H

#include "datatypes.h"
#include <QAbstractTableModel>

struct CONFIG_PARA;

namespace Config
{
	class CAnalogOutput;
	class CFesData;
}

class IMainModuleInterface;

class CAOModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, OccNo, BlockOccNo, /*DataType,*/ Desc, InitQua, Address, PinLabel, InitValue, Format, Unit, Enable, MaxRaw, MinRaw, MaxConvert, MinConvert,
		ScaleTagName/*, ScaleDesc*/, ScaleType, RangeL, RangeH, HighQua, LowQua, ReferenceCount, ChannelOccNo, DeviceOccNo
	};

	CAOModel(QObject *parent, std::vector <Config::CAnalogOutput *> &arrAOs, Config::CFesData *pFes, IMainModuleInterface *pCore);
	~CAOModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	QMap<int32s, CONFIG_PARA> &GetColumnInfo()
	{
		return m_mapInfo;
	}

	std::vector <Config::CAnalogOutput *> &GetAOs()
	{
		return m_arrAOs;
	}

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	void RefrushModel();

private:
	void InitPara();
	Qt::ItemFlags flags(const QModelIndex &index) const;
private:
	std::vector <Config::CAnalogOutput*> & m_arrAOs;

	QStringList m_lstHorizontalHeader;

	QMap<int32s, CONFIG_PARA> m_mapInfo;

	Config::CFesData *m_pFes;

	IMainModuleInterface *m_pCore;

	QMap<int, QString> m_mapScaleType;
};

#endif // AOMODEL_H
