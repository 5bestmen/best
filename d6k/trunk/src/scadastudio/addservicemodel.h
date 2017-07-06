#ifndef ADDSERVICEMODEL_H
#define ADDSERVICEMODEL_H

#include <QAbstractTableModel>

namespace Config
{
	class CNodeConfigData;
	struct NODE_SERVICE_ROLE;
}

class CAddServiceModel : public QAbstractTableModel
{
	Q_OBJECT

	enum
	{
		Name, AppType, LoadType, Argument, Description, Prority
	};

	enum AppType
	{
		Inn_APP = 0,    //����Ӧ��
		Ext_App = 1,    //��չ����
	};

	enum LoadType
	{
		Load_Dll = 0,   //dll����
		Load_Exe = 1,   //exe����
	};

public:
	CAddServiceModel(QObject *parent, Config::CNodeConfigData *pConfig);
	~CAddServiceModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	std::vector<Config::NODE_SERVICE_ROLE *> m_arrNodeService;

	std::vector<Config::NODE_SERVICE_ROLE *> &GetArr()
	{
		return m_arrNodeService;
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
	void InitPara();


private:
	Config::CNodeConfigData *m_pConfig;
	QStringList m_horizontal_header_list;
	QMap<int, QString> m_appTypeMap;
	QMap<int, QString> m_loadTypeMap;
};

#endif
