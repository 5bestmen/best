#include "base_algorithm.h"
#include <QDebug>

CBaseAlgorithm::CBaseAlgorithm(QObject *parent)
	:QObject(parent)
{
	//初始化capacity的值，从主数据库查询capacity的值，涉及数据库，待定

}


CBaseAlgorithm::~CBaseAlgorithm()
{

}

//根据模拟量名列表,生成map<模拟量名,模拟量属性>
QMap<Analogues_Alias, Analogues_Info> CBaseAlgorithm::GetAnaloguesInfo(QList<Analogues_Alias> & alias)
{
	QMap<Analogues_Alias, Analogues_Info> map;
	Analogues_Info a_info;

	for each (const Analogues_Alias & str in alias)
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