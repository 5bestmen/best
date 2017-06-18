#ifndef  BASE_ALGORITHM_H
#define  BASE_ALGORITHM_H

#include <QObject>
#include <QDateTime>
#include <QMap>

#include "predict_database.h"

class CBaseAlgorithm : public QObject
{
	Q_OBJECT

public:
	CBaseAlgorithm(QObject *parent = 0);
	~CBaseAlgorithm();

	virtual void PowerPredict72h(const QDateTime &currTime) = 0;
	virtual void PowerPredict4h(const QDateTime &currTime) = 0;

protected:
	//����ģ�������б�,����map<ģ������,ģ��������>
	QMap<Analogues_Alias, Analogues_Info> GetAnaloguesInfo(QList<Analogues_Alias> & alias);

public:
	//��վ����
	data_type capacity;

};

#endif   //BASE_ALGORITHM_H