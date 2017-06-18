#ifndef SETSCADAALARMDIALOG_H
#define SETSCADAALARMDIALOG_H

#include <QDialog>
#include "ui_setscadaalarmdialog.h"

namespace Config
{
	class CScadaVariableConfigData;
	class CScadaAinAlarmLimit;
	class CScadaAnalogAlarmGroup;
	class CScadaDinAlarmLimit;
	class CScadaDigitalAlarmGroup;
	class CScadaData;
}

class QStandardItemModel;
class CQuoteItem;

class CSetScadaAlarmDialog : public QDialog
{
	Q_OBJECT

	enum AIALARMCOLUMN
	{
		TagName, Description, Enable, OnQualityGood, DeadType, DeadArea, AlarmType
	};

	enum Limit
	{
		LimitTagName, LimitDescription, LimitID, LimitCondition, LimitCategory, LimitValue, LimitLowValue/*, LimitWriteLevel, LimitReadLevel*/, LimitDelayAlarm/*, LimitLevel, LimitCommands
		, LimitSoundFile, LimitPlaySoundTimes*/, LimitPriority, LimitSupportAck, LimitSupportDelete, LimitAckType, LimitPushPic, LimitPdr, LimitPlaySound, LimitBlink, LimitLogAlarmWindow, LimitAccidentHandle,
		LimitAccidentPrint, LimitLightPlate
	};

	enum Condition
	{
		GREATEROREQUAL,
		LESSOREQUAL,
		EQUAL,
		RATEDOWN,
		RATEUP,
		NOTEQUAL,
		BETWEEN
	};

public:
	CSetScadaAlarmDialog(QWidget *parent, Config::CScadaVariableConfigData *pConfig, QString strTagName, Config::CScadaData *pScada, int nType);
	~CSetScadaAlarmDialog();

	int GetType()
	{
		return m_nAlarmType;
	}

	QString GetDesc()
	{
		return m_strDesc;
	}

	QString GetTagName()
	{
		return m_strTagName;
	}

private:
	void InitAlarm(Config::CScadaData *pScada, int nType);
	bool InitAIAlarmGroup(CQuoteItem *pItem, Config::CScadaAnalogAlarmGroup *pGroup);
	void InitAIOfflimit(CQuoteItem *pItem, std::vector<Config::CScadaAinAlarmLimit *> &m_arrAlarmOfflimit);
	bool InitDIAlarmGroup(CQuoteItem *pItem, Config::CScadaDigitalAlarmGroup *pGroup);
	void InitDIOfflimit(CQuoteItem *pItem, std::vector<Config::CScadaDinAlarmLimit *> &m_arrOfflimit);

	void DumpModel(const QAbstractItemModel * M, const QModelIndex & Top, const QString &S);

	void InitPara();
private slots:
	void Clicked(const QModelIndex &index);
	void OnOK();
	void OnCancel();
	void showMouseRightButton(const QPoint& point);
private:
	Ui::CSetScadaAlarmDialog ui;
	Config::CScadaVariableConfigData *m_pConfig;
	QStandardItemModel *m_pModel;
	int m_nAlarmType;
	QString m_strTagName;
	QString m_strDesc;
	QStringList m_horizontal_header_list;

	QMap<int, QString> m_arrAnalogAlarmConditon;
	QMap<int, QString> m_arrDigitalAlarmConditon;
	QMap<int, QString> m_arrAlarmCategory;
	QMap<int, QString> m_arrAlarmAckType;
};

#endif // SETSCADAALARMDIALOG_H
