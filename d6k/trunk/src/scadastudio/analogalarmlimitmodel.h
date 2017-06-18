#ifndef ANALOGALARMLIMITMODEL_H
#define ANALOGALARMLIMITMODEL_H

#include <QAbstractTableModel>

class IMainModuleInterface;
class CAlarm;

namespace Config
{
	class CAinAlarmLimit;
	class CFesData;
	class CAnalogAlarm;
}

class QStandardItem;
class IMainModuleInterface;

class CAnalogAlarmLimitModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, Description, OccNo, BlockNo, Condition, Category, Value, LowValue, DelayAlarm,
		Priority, SupportAck, SupportDelete, AckType, SoundFile, PlaySoundTimes, SpeechAlarmText,
		Beep, PushGraph, BlinkGraph, Log, BackColor, TextColor, BlinkBackColor, BlinkTextColor, 
		LightBoard, DispGuide, Commands
	};
public:
	CAnalogAlarmLimitModel(QObject *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CAnalogAlarmLimitModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void RefrushModel();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

private:
	void InitPara();

	//int  QColorToInt(const QColor &color)
	//{
	//	//将Color 从QColor 转换成 int  
	//	return   (int)(((unsigned int)color.blue() << 16) | (unsigned short)(((unsigned short)color.green() << 8) | color.red()));
	//}

	//QColor IntToQColor( int intColor)
	//{
	//	//将Color 从int 转换成 QColor  
	//	int red = intColor & 255;
	//	int green = intColor >> 8 & 255;
	//	int blue = intColor >> 16 & 255;
	//	return QColor(red, green, blue);
	//}

private:
	QStringList m_horizontal_header_list;
	Config::CAnalogAlarm *m_pAlarmData;
	//QMap<int, CONFIG_PARA> m_arrInfo;
	QStandardItem *m_pItem;
	Config::CFesData *m_pFes;
	IMainModuleInterface *m_pCore;
};

#endif // ANALOGALARMLIMITMODEL_H
