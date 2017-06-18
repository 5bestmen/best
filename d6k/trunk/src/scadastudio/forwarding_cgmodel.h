#ifndef FORWARDING_CGMODEL_H
#define FORWARDING_CGMODEL_H

#include "datatypes.h"
#include <QAbstractTableModel>

struct CONFIG_PARA;

namespace Config
{
	class CForwardingChannelData;
	class CForwardingControlGearData;
	class CFesData;
}

class IMainModuleInterface;

class CForwardingCGModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, Desc, SourceTagName, OccNo, BlockNo, GroupNo, EntryNo, Address, FUN, Info
	};

	CForwardingCGModel(QObject *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingCGModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	QMap<int32s, CONFIG_PARA> &GetColumnInfo()
	{
		return m_arrInfo;
	}

	std::vector <Config::CForwardingControlGearData *> &GetCGs();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	void RefrushModel();

private:
	void InitPara();
	Qt::ItemFlags flags(const QModelIndex &index) const;
private:
	Config::CForwardingChannelData *m_pChannelData;

	QStringList m_horizontal_header_list;

	QMap<int32s, CONFIG_PARA> m_arrInfo;

	Config::CFesData *m_pFes;

	IMainModuleInterface *m_pCore;
};

#endif // FORWARDING_CGMODEL_H
