#ifndef SCADASYSTEMVARIABLEMODEL_H
#define SCADASYSTEMVARIABLEMODEL_H

#include <QAbstractTableModel>

namespace Config
{
	class CScadaVariableConfigData;
	class CScadaData;
}

class IMainModuleInterface;

class CScadaSystemVariableModel : public QAbstractTableModel
{
	Q_OBJECT

	enum COLUMN
	{
		ID, TagName, Desc, DataType, OccNo
	};
public:
	CScadaSystemVariableModel(QObject *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pConfigData
		, Config::CScadaData *pData);
	~CScadaSystemVariableModel();

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
	Config::CScadaVariableConfigData *m_pConfigData;
	QStringList m_horizontal_header_list;
	QMap<int, QString> m_arrDataType;
	Config::CScadaData *m_pData;
};

#endif // SCADASYSTEMVARIABLEMODEL_H
