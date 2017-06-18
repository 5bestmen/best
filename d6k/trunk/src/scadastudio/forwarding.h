#ifndef FORWARDING_H
#define FORWARDING_H

#include <QObject>

class IMainWindow;
class IMainModuleInterface;
class QStandardItem;
class QAbstractItemModel;

namespace Config
{
	class CFesData;
}

class CForwarding : public QObject
{
	Q_OBJECT

	enum CHANNEL_CHILD
	{
		AI, DI, DO, AO, CG, KWH
	};

public:
	CForwarding(IMainModuleInterface *pCore);
	~CForwarding();

	void ShowMenu(QModelIndex &index);

	void AddForwardingChannelGroup(QModelIndex &index);
	void AddForwardingChannelItem(QModelIndex &index);

	void AddScale(QModelIndex &index);

	void DeleteForwardingChannelGroup(QModelIndex &index);
	void DeleteForwardingChannelItem(QModelIndex &index);

	bool Delete_ForwardingChannelModule_WidgetInTabWidget(const QModelIndex&index, Config::CFesData *pFes);

	void Disconnect();

public slots:
	void showMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex &index);

	void itemChanged(QStandardItem *item);

private:
	QString GetGroupName(QModelIndex &index);
	QString GetLastGroupName(QModelIndex &index);
	void GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec);
	Config::CFesData *GetFes(const QModelIndex &index);

	bool ModifyForwardingChannelChildWidget(QStandardItem *item, const QString &strChannelTagName, const QString &strChannelLastTagName, const QString &strFesTagName);
private:
	IMainWindow *m_pUi;
	IMainModuleInterface *m_pCore;
};

#endif // FORWARDING_H
