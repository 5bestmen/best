#ifndef PREDICT_MODEL_H
#define PREDICT_MODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QVariant>
#include <QModelIndex>

#include "predictdatainfo.h"

//表头名称
enum
{
	ID,
	TagName,
	Describe,
	OccNo,
};

class CPredictModel : public QAbstractTableModel
{
public:

	//构造函数
	explicit CPredictModel(CPredictItem& modelInfo, QObject* parent = 0);

	//析构函数
	~CPredictModel();

	//获取表列数
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

	//获取表行数
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

	//获取表头
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	//获取表数据
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

	//获取item标记
	virtual Qt::ItemFlags flags(const QModelIndex& index) const;

	//设置表数据
	virtual bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole);

	//修改表数据
	bool ModifyContentData(const QModelIndex& index, const QVariant& value, int& role);

	//设置表头
	void SetHeadTitles(QStringList headList);

	//刷新模型
	void RefushModel()
	{
		beginResetModel();
		endResetModel();
	}

private:

	//显示YC数据
	QVariant ShowContent(int nCol, int nRow) const;

public:

	//YC信息
	CPredictItem& m_predictItemInfo;

private:

	//表头
	QStringList m_strHeadList;

};

#endif  //PREDICT_MODEL_H