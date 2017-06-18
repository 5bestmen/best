#ifndef SYSTEMVARIABLEMODEL_H
#define SYSTEMVARIABLEMODEL_H

#include <QAbstractTableModel>

namespace Config
{
	class CFesData;
}

class IMainModuleInterface;

class CSystemVariableModel : public QAbstractTableModel
{
	Q_OBJECT

	enum COLUMN
	{
		ID, TagName, Desc, DataType, OccNo
	};
public:
	CSystemVariableModel(QObject *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData);
	~CSystemVariableModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	//bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	Qt::ItemFlags flags(const QModelIndex &index) const;

private:
	void InitPara();
	//系统变量初始化 全局唯一
	void InitData();

private:
	IMainModuleInterface *m_pCore;
	Config::CFesData *m_pFesData;
	QStringList m_horizontal_header_list;
	QMap<int, QString> m_arrDataType;
};

#endif // SYSTEMVARIABLEMODEL_H
