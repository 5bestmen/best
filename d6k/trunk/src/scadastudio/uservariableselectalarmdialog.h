#ifndef USERVARIABLESELECTALARMDIALOG_H
#define USERVARIABLESELECTALARMDIALOG_H

#include <QDialog>
#include "ui_uservariableselectalarmdialog.h"

class QStandardItemModel;

namespace Config
{
	class CFesData;

	class CAnalogInput;

	class CAinAlarmLimit;

	class CDinAlarmLimit;

	class CAnalogAlarm;
	
	class CDigitalAlarm;

	class CAnalogAlarmGroup;

	class CDigitalAlarmGroup;

	class CUserVariable;
}

class CQuoteItem;
class IMainModuleInterface;

class CUserVariableSelectAlarmDialog : public QDialog
{
	Q_OBJECT
	
	enum AIALARMCOLUMN
	{
		TagName, Description, Enable, OnQualityGood, DeadType, DeadArea, AlarmType
	};

	enum AIOfflimit
	{
		LimitTagName, LimitDescription, LimitID, LimitCondition, LimitCategory, LimitValue, LimitLowValue/*, LimitWriteLevel, LimitReadLevel*/
		, LimitDelayAlarm/*, LimitLevel, LimitCommands , LimitSoundFile, LimitPlaySoundTimes*/, LimitPriority, LimitSupportAck, LimitSupportDelete, LimitAckType, LimitPushPic, LimitPdr, LimitPlaySound, LimitBlink, LimitLogAlarmWindow, LimitAccidentHandle,
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
	CUserVariableSelectAlarmDialog(QWidget *parent, Config::CFesData *pFes, Config::CUserVariable *pData, int nType);
	~CUserVariableSelectAlarmDialog();

	int GetType()
	{
		return m_nAlarmType;
	}

	//QString GetDesc()
	//{
	//	return m_strDesc;
	//}

	QString GetTagName()
	{
		return m_strTagName;
	}

private slots:
	void Clicked(const QModelIndex &index);

	void OnOK();

	void OnCancel();
	void showMouseRightButton(const QPoint& point);
private:
	void InitAlarm(int nType);

	bool InitAIAlarmGroup(CQuoteItem *pItem, Config::CAnalogAlarmGroup *pGroup);
	bool InitDIAlarmGroup(CQuoteItem *pItem, Config::CDigitalAlarmGroup *pGroup);

	void InitAIAlarm(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData);
	void InitAIOfflimit(CQuoteItem *pItem, std::vector<Config::CAinAlarmLimit *> &m_arrAlarmOfflimit);

	void InitDIAlarm(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData);
	void InitDIOfflimit(CQuoteItem *pItem, std::vector<Config::CDinAlarmLimit *> &m_arrOfflimit);

	void InitTable(Config::CFesData *pFes, Config::CAnalogInput *pAI);

	void InitAIAlarm(Config::CAnalogAlarm *pItem);

	void InitDIAlarm(Config::CDigitalAlarm *pDigtalAlarm);

	void DumpModel(const QAbstractItemModel * M, const QModelIndex & Top, const QString &S);

	void InitPara();
private:
	Ui::CSelectAlarmDialog ui;

	Config::CFesData *m_pFesData;

	int m_nAlarmType;
	QString m_strTagName;
	//QString m_strDesc;

	QStandardItemModel *m_pModel;

	Config::CUserVariable *m_pData;

	QMap<int, QString> m_arrAnalogAlarmConditon;
	QMap<int, QString> m_arrDigitalAlarmConditon;
	QMap<int, QString> m_arrAlarmCategory;
	QMap<int, QString> m_arrAlarmAckType;
};

#endif // SELECTALARMDIALOG_H
