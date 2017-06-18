#ifndef CSACLE_H
#define CSACLE_H

#include <QObject>
#include <QMap>
#include "config_data.h"
#include "scadastudiodefine.h"

class IMainWindow;
class QStandardItem;
class QtProperty;
class CAttriwindow;
class IMainModuleInterface;
class IMainWindow;

class CScale : public QObject
{
	Q_OBJECT

public:
	CScale(IMainModuleInterface *pUi);
	~CScale();

	void AddScaleGroup(QModelIndex &index);

	void ShowMenu(QModelIndex &index);

	void AddScale(QModelIndex &index);

	void Disconnect();

private:
	int CallBackBeforeCloseTabScale(void *pData)
	{
		Q_UNUSED(pData);
		
		return 0;
	}

	void CreateAttriConnect();

	bool DeleteGroup(const QModelIndex &index);
	bool DeleteModuleWidgetInTabWidget(const QModelIndex &index, QString &strFesTagName, Config::CFesData *pFes);

	bool DeleteItem(const QModelIndex &index);
public slots:
	void showMouseRightButton(const QPoint &point);

	void itemChanged(QStandardItem *item);

	void doubleClicked(const QModelIndex &index);

	void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

	void SendChangeChannelModelRows(QSet<int32s> &set, int32s nType, void *pModel);

	void Expanded(const QModelIndex &index);
	void Collapsed(const QModelIndex &index);
private:
	IMainWindow *m_pUi;

	//std::vector<Config::CFesData *> &m_arrFes;

	QMap<QtProperty *, QString> m_propertyToId;
	QMap<QString, QtProperty *> m_idToProperty;

	bool m_bConnectAttr;

	CAttriwindow *m_pAttr;

	IMainModuleInterface *m_pCore;
};

#endif // CSACLE_H
