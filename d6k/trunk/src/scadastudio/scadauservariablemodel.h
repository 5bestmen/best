#ifndef SCADAUSERVARIABLEMODEL_H
#define SCADAUSERVARIABLEMODEL_H

#include <QAbstractTableModel>

namespace Config
{
	class CScadaVariableConfigData;
	class CScadaUserVariableGroup;
	class CScadaUserVariable;
	class CScadaData;
}

class IMainModuleInterface;

class CScadaUserVariableModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum COLUMN
	{
		ID, TagName, Description, OccNo, DataType, InitValue, InitialQua, Address, PinLabel, SourceTagName, Scale, Alarm
	};

public:
	CScadaUserVariableModel(QObject *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pScadaVirableConfig
		, Config::CScadaUserVariableGroup *pGroup, Config::CScadaData *pScada);
	~CScadaUserVariableModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	Qt::ItemFlags flags(const QModelIndex &index) const;

	QStringList &GetColumnList()
	{
		return m_horizontal_header_list;
	}

	Config::CScadaUserVariableGroup *GetUserVariableGroup()
	{
		return m_pGroup;
	}

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	std::vector<Config::CScadaUserVariable *> &GetArr();
						
	void RefrushModel();
private:
	void InitPara();

private:
	IMainModuleInterface *m_pCore;
	Config::CScadaVariableConfigData *m_pScadaVirableConfig;
	Config::CScadaUserVariableGroup *m_pGroup;
	QStringList m_horizontal_header_list;
	Config::CScadaData *m_pScada;
	IMainModuleInterface *m_pGlobleCore;
};

#endif // SCADAUSERVARIABLEMODEL_H
