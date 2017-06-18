#ifndef PREDICT_MODEL_H
#define PREDICT_MODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QVariant>
#include <QModelIndex>

#include "predictdatainfo.h"

//��ͷ����
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

	//���캯��
	explicit CPredictModel(CPredictItem& modelInfo, QObject* parent = 0);

	//��������
	~CPredictModel();

	//��ȡ������
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

	//��ȡ������
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

	//��ȡ��ͷ
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	//��ȡ������
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

	//��ȡitem���
	virtual Qt::ItemFlags flags(const QModelIndex& index) const;

	//���ñ�����
	virtual bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole);

	//�޸ı�����
	bool ModifyContentData(const QModelIndex& index, const QVariant& value, int& role);

	//���ñ�ͷ
	void SetHeadTitles(QStringList headList);

	//ˢ��ģ��
	void RefushModel()
	{
		beginResetModel();
		endResetModel();
	}

private:

	//��ʾYC����
	QVariant ShowContent(int nCol, int nRow) const;

public:

	//YC��Ϣ
	CPredictItem& m_predictItemInfo;

private:

	//��ͷ
	QStringList m_strHeadList;

};

#endif  //PREDICT_MODEL_H