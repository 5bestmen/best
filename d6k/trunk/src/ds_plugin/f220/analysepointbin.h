#ifndef ANALYSEPOINTBIN_H
#define ANALYSEPOINTBIN_H

#include <QObject>
#include <QMap>

class CAnalysePointBin : public QObject
{
    Q_OBJECT

public:
    CAnalysePointBin(QObject *parent=0);
    void InitData(const QString &strFileName);
    //解析数据
    //void AnalyseBinData(const QByteArray & byData);
    //const QByteArray & GetBtData();
    ~CAnalysePointBin();

	const QList<int> GetAnaloglst()
	{
		return m_lstAnalog;
	}

	const QList<int> GetBinarylst()
	{
		return m_lstBinary;
	}

	const QList<int> GetKwhlst()
	{
		return m_lstKwh;
	}

	const QList<int> GetControllst()
	{
		return m_lstControl;
	}
    
private:
    //需要测的点   类型标识  该类型下需要测的点
   // QMap<unsigned int, QList<int>> m_mapNeedpoints;
	//遥测
	QList<int> m_lstAnalog;
	//遥信
	QList<int> m_lstBinary;
	//遥控
	QList<int> m_lstControl;
	//遥脉
	QList<int> m_lstKwh;
};

#endif // ANALYSEPOINTBIN_H
