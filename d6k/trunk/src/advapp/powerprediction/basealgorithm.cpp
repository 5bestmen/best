#include "basealgorithm.h"
#include <QDebug>

CBaseAlgorithm::CBaseAlgorithm(QObject *parent)
	:QObject(parent)
{
	//��ʼ��capacity��ֵ���������ݿ��ѯcapacity��ֵ���漰���ݿ⣬����

}


CBaseAlgorithm::~CBaseAlgorithm()
{

}

//����ģ�������б�,����map<ģ������,ģ��������>
QMap<Analogues_alias, Analogues_Info> CBaseAlgorithm::GetAnaloguesInfo(QList<Analogues_alias> & alias)
{
	QMap<Analogues_alias, Analogues_Info> map;
	Analogues_Info a_info;

	for each (const Analogues_alias & str in alias)
	{
		if (CPredictDatabase::GetInstance().GetAnalogue_Info(str, a_info))
		{
			map.insert(str, a_info);
		} 
		else
		{
			qDebug() << "unable to get" << str << "information";
		}
	}
	return map;
}