#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QAbstractTableModel>

class IMainModuleInterface;
namespace Config
{
	class CDevice;
	class CFesData;
}

class CChannel;
class CQuoteItem;

class CDeviceModel : public QAbstractTableModel
{
	Q_OBJECT

	enum COLUMN{
		ID, TagName, Description, OccNo, Priority, DeviceAddr, ExtentionParam, DeviceModelID, DeviceType
	};

public:
	CDeviceModel(QObject *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, CChannel *pModule, CQuoteItem *pItem
		, Config::CFesData *pFesData);
	~CDeviceModel();

	void RefrushModel();

protected:
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	Qt::ItemFlags flags(const QModelIndex &index) const;

private:
	void InitPara();
private:
	Config::CDevice *m_pDeviceData;

	QStringList m_lstHeader;

	CQuoteItem *m_pItem;

	IMainModuleInterface *m_pCore;

	Config::CFesData *m_pFesData;
};

#endif // DEVICEMODEL_H
