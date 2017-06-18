#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QMap>
#include <QSet>
#include "config_data.h"
#include "scadastudiodefine.h"


class IMainWindow;
class QStandardItem;
class CAttriwindow;
class CChannelModel;
class QtProperty;
class CQuoteItem;
//class COutputMessagerClient;
class IMainModuleInterface;
class CAIModel;
class CDIModel;
class CAOModel;
class CDOModel;

namespace Config
{
	class CDevice;
}

class CChannel : public QObject
{
	Q_OBJECT

	enum Baud{ B300 = 300, B600 = 600, B1200 = 1200, B2400 = 2400, B4800 = 4800, B9600 = 9600,
		B19200 = 19200, B38400 = 38400
	};
	enum DEVICE_CHILD
	{
		AI, DI, AO, DO
	};
public:
	CChannel(IMainModuleInterface *pCore);
	~CChannel();

	void AddChannelGroup(QModelIndex &index);

	void ShowMenu(QModelIndex &index);

	void AddChannel(QModelIndex &index);

	void AddDevice(QModelIndex &index);

	void MofifyDevice(QModelIndex &index);

	void AddChannelTagName(QString &strTagName, QString &strDesc);

	bool CheckTagNameAndChannelName(QString &strTagName, QString &strDesc);

	void DisConnect();

	void DeviceExportToBeModelAction(QModelIndex &index);
	void ExportToBeModel(Config::CDevice *pDevice, QString &strModelPath, QString &strModelDesc, QString &strFactory);
	Config::CFesData * GetFes(QModelIndex & index);
private:
	void ModifyChannel(QModelIndex &index);
	void DeleteChannelItem(QModelIndex &index);
	void DeleteChannelGroup(QModelIndex &index);
	void DeleteDeviceItem(QModelIndex &index);

	void DeleteDeviceModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes);
	void DeleteChannelModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes);

	void InitPara();

	QString GetGroupName(QModelIndex &index);

	void CreateChannelAttr(CChannelModel *pModel, QSet<int32s> &set);

	void CreateAttriConnect();

	int CallBackBeforeCloseTab(void *pData)
	{
		Q_UNUSED(pData);
		
		return 0;
	}
	
	void CreateAIAttr(CAIModel *pModel);
	void SetAIAttr(CAIModel *pModel);
	void ValueChangedAI(QtProperty *property, const QString &val);
	void ValueChangedAI(QtProperty *property, int val);
	void ValueChangedAI(QtProperty *property, double val);
	void ValueChangedAI(QtProperty *property, bool val);

	void CreateDIAttr(CDIModel *pModel);
	void ValueChangedDI(QtProperty *property, const QString &val);
	void ValueChangedDI(QtProperty *property, int val);
	void ValueChangedDI(QtProperty *property, double val);
	void ValueChangedDI(QtProperty *property, bool val);

	void CreateAOAttr(CAOModel *pModel);
	void ValueChangedAO(QtProperty *property, const QString &val);
	void ValueChangedAO(QtProperty *property, int val);
	void ValueChangedAO(QtProperty *property, double val);
	void ValueChangedAO(QtProperty *property, bool val);

	void CreateDOAttr(CDOModel *pModel);
	void ValueChangedDO(QtProperty *property, const QString &val);
	void ValueChangedDO(QtProperty *property, int val);
	void ValueChangedDO(QtProperty *property, double val);
	void ValueChangedDO(QtProperty *property, bool val);

public slots:
	void showMouseRightButton(const QPoint& point);

	void doubleClicked(const QModelIndex &index);

	void itemChanged(QStandardItem *item);

	void SendSelectedRows(QSet<int32s> &set, int32s nType, CChannelModel *pModel);

	void SendChangeChannelModelRows(QSet<int32s> &set, int32s nType, CChannelModel *pModel);

	void valueChanged(QtProperty *property, const QString &val);
	void valueChanged(QtProperty *property, int val);
	void valueChanged(QtProperty *property, double val);
	void valueChanged(QtProperty *property, bool val);

	void collapsed(const QModelIndex &index);

	void expanded(const QModelIndex &index);

	QStandardItem * FindChannelItem(QStandardItem *item, QStandardItem *channelItem);

	void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModule);
private:
	IMainWindow *m_pUi;

	CAttriwindow *m_pAttr;

	QVector<IDDESC> *m_pDriverNameInfo;

	QVector<IDDESC> *m_pDriverParaInfo;

	QVector<IDDESC> *m_pMediumInfo;

	QVector<IDDESC> *m_pCOMInfo;

	QVector<IDDESC> *m_pBaudInfo;

	QMap<QtProperty *, QString> m_propertyToId;
	QMap<QString, QtProperty *> m_idToProperty;

	QMap<QtProperty *, QString> m_propertyToIdAI;
	QMap<QString, QtProperty *> m_idToPropertyAI;

	bool m_bConnectAttr;

	CChannelModel *m_pModel;

	CQuoteItem *m_pCurrentTabItem;

	IMainModuleInterface *m_pCore;

	bool m_bCreateAIAttr;

	CAIModel *m_pAIModel;

	QSet<int32s> m_setSelect;

	CDIModel *m_pDIModel;

	QMap<QtProperty *, QString> m_propertyToIdDI;
	QMap<QString, QtProperty *> m_idToPropertyDI;
	bool m_bCreateDIAttr;

	CAOModel *m_pAOModel;

	QMap<QtProperty *, QString> m_propertyToIdAO;
	QMap<QString, QtProperty *> m_idToPropertyAO;

	QMap<QtProperty *, QString> m_propertyToIdDO;
	QMap<QString, QtProperty *> m_idToPropertyDO;

	CDOModel *m_pDOModel;
};

#endif // CHANNEL_H
