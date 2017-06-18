#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include "scadastudiodefine.h"

class CTableModel;
class CSortFilterModel;
class CReadTableConfig;
//class CDbapplitonapi;
class IMainWindow;

class CTableView : public QTableView
{
	Q_OBJECT

public:
	enum{LINEEDIT,CHECKBOX,COMBOBOX,COMBOBOXTXTINDEX,COMBOBOXTXTSTRING};

public:
	CTableView(IMainWindow *parent,const QString & tagName, const QString & descName, int type = 0, QString tableName = "", QString group = "");
	~CTableView();

	void AddRow(QVector<QString> &rowList);

	int RowCount();

	void InitHeader(QStringList &header);

	void GetFieldInfo(QString strTableName);

	void SetChannelViewHeader();

	void Init();

	void Save();

	void DeleteData();

	void Update();

	void Add();

	//…Ë÷√…æ≥˝±Í÷æ
	void SetDeleFlag();

private:
	void SetDelegateForChannelView(QVector<CHANNELSCADASTUDIO> &channelStr);

signals:
	void SendToAttr(CTableView *pView);

public slots:
	//void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int>())
	//{
	//	
	//}

	void SelectRow(int nRow);

public:
	CTableModel *m_pChannelModelData;
	CSortFilterModel *m_pSortFilterModel;

	CReadTableConfig *m_pReadTableConfig;

	QVector<CHANNELSCADASTUDIO> m_channelFieldVec;

	//CDbapplitonapi *m_pDbi;

	QString m_strChannelTagName;

	int m_nType;

	QString m_strTableName;

	QString m_strGroup;

	QString m_strChannelName;
};

#endif // TABLEVIEW_H
