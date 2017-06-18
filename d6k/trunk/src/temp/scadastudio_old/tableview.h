#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include "define.h"

class CTableModel;
class CSortFilterModel;
class CReadTableConfig;
class CDbapplitonapi;

class CTableView : public QTableView
{
	Q_OBJECT

public:
	enum{LINEEDIT,CHECKBOX,COMBOBOX,COMBOBOXTXTINDEX,COMBOBOXTXTSTRING};

public:
	CTableView(QWidget *parent, QString descName, int type = 0, QString tableName = "", QString group = "");
	~CTableView();

	void AddRow(QVector<QString> &rowList);

	int RowCount();

	void InitHeader(QStringList &header);

	void GetFieldInfo(QString strTableName);

	void SetChannelViewHeader();

	void Init();

private:
	void SetDelegateForChannelView(QVector<CHANNEL> &channelStr);

public:
	CTableModel *m_pChannelModelData;
	CSortFilterModel *m_pSortFilterModel;

	CReadTableConfig *m_pReadTableConfig;

	QVector<CHANNEL> m_channelFieldVec;

	CDbapplitonapi *m_pDbi;

	QString m_strChannelName;

	int m_nType;

	QString m_strTableName;

	QString m_strGroup;
};

#endif // TABLEVIEW_H
