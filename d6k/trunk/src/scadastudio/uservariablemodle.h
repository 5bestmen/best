#ifndef USERVARIABLEMODLE_H
#define USERVARIABLEMODLE_H

#include <QAbstractTableModel>

class IMainModuleInterface;

namespace Config
{
	class CUserVariableGroup;
	class CFesData;
	class CUserVariable;
}

class CUserVariableModel : public QAbstractTableModel
{
	Q_OBJECT


public:
	enum COLUMN
	{
		ID, TagName, Description, OccNo, DataType, InitValue, InitialQua, Address, PinLabel, SourceTagName, Scale, Alarm
	};

	CUserVariableModel(QObject *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData, Config::CUserVariableGroup *pVariable);
	~CUserVariableModel();

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

	Config::CUserVariableGroup *GetUserVariableGroup()
	{
		return m_pVariable;
	}

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	std::vector<Config::CUserVariable *> &GetArr();

	void RefrushModel();
private:
	void InitPara();

private:
	QStringList m_horizontal_header_list;
	IMainModuleInterface *m_pCore;
	Config::CUserVariableGroup *m_pVariable;
	Config::CFesData *m_pFesData;
};

#endif // USERVARIABLEMODLE_H
