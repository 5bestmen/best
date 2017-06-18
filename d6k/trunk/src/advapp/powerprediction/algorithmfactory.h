#ifndef  ALGORITHM_FACTORY_H
#define  ALGORITHM_FACTORY_H

#include <QObject>
#include <QPointer>
#include <QTimer>
#include "basealgorithm.h"
#include "proportionalgorithm.h"

class CAlgorithmFactory : public QObject
{
	Q_OBJECT

private:

	CAlgorithmFactory(QObject *parent = 0);
	
	~CAlgorithmFactory();

public:

	enum Predict_Algorithm_Type
	{
		Proportion_Algorithm = 1,
		Fann_Algorithm,
		Libsvm_Algorithm
	};

	enum Predict_Interval
	{
		Predict_Interval_15m = 900,
		Predict_Interval_20m = 1200
	};

public:

	static CAlgorithmFactory& GetInstance();

	void PowerPredict72h();

	void Start();

	void Stop();

	void SetType(Predict_Algorithm_Type type);

	void StartSyncTimer15m();

	void StartSyncTimer20m();

	Predict_Algorithm_Type GetType()
	{
		return m_Algorithm_Type;
	}

private:

	Predict_Algorithm_Type m_Algorithm_Type;

	QPointer<CBaseAlgorithm>  pBaseAlgorithm;

	QPointer<CProportionAlgorithm> pProportionAlgorithm;

	QTimer * syncTime15m_4h;

	QTimer * syncTime20m_72h;



};




#endif  //ALGORITHM_FACTORY_H