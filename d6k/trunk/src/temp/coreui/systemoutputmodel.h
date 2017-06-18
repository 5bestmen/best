#ifndef ULTRA_LIST_H
#define ULTRA_LIST_H

#include <QAbstractTableModel>
#include <QColor>
#include <QTime>

//#include <ace/Date_Time.h> 
//#include <ace/OS_NS_sys_time.h>


#include <vector>
#include <string>


class CDataItem
{
public:
	CDataItem(int nType, std::string strTag, QColor color)
	{
		m_nMsg = nType;
		m_strTag = strTag;
		m_color = color;

		//unsigned long lCurTime = (unsigned long)(ACE_OS::gettimeofday().sec()); 
		//ACE_Time_Value tv2(lCurTime);
		//ACE_Date_Time tvTime(tv2);//stc add 2013-03-25

		QString strTmp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

		char buf[32];
		//sprintf(buf,"%d:%d:%d:%d:%d:%d:%d\n",tvTime.year()%100,tvTime.month(),tvTime.day(),tvTime.hour(),tvTime.minute(),tvTime.second(),tvTime.microsec());
		//sprintf(buf,"%d:%d:%d %d:%d:%d\n",tvTime.year(),tvTime.month(),tvTime.day(),tvTime.hour(),tvTime.minute(),tvTime.second());
		m_strTime = strTmp.toStdString();
	}
	int m_nMsg;
	std::string m_strTag;
	std::string m_strTime;
	QColor m_color;
};
class CSystemOutputModel : public QAbstractTableModel
{
public:
	explicit CSystemOutputModel(QObject *parent = 0, int rowCount = 1000000, int colCount = 6);
	~CSystemOutputModel();

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
	int m_rowCount;
	int m_colCount;

private:
	std::vector<CDataItem*> m_items;

public:
	int AddItem(int nType, std::string strTag, QColor color);
	void ClearAll();
};

#endif // ULTRA_LIST_H