#include "pp_svc.h"

CPowerPredictSvc::CPowerPredictSvc() : CBaseModule()
{

}

CPowerPredictSvc::~CPowerPredictSvc()
{


}


bool CPowerPredictSvc::Initialize()
{

	
	return true;
}

void CPowerPredictSvc::Run()
{


}

void CPowerPredictSvc::Shutdown()
{
	
}

void CPowerPredictSvc::MainLoop()
{

}

void CPowerPredictSvc::powerPredict4h(const QDateTime& currTime)
{
	/*QDateTime predictStartTime, predictEndTime;
	bool isFind4h = false;
	QVector<float> maintenanceValues;
	QString str;
	bool tmpTest = false;

	predictStartTime = currTime.addSecs(15 * 60);
	int hour = predictStartTime.time().hour();
	int minu = (predictStartTime.time().minute() / 15) * 15;
	str = QString("%1 %2:%3:00").arg(predictStartTime.toString("yyyy-MM-dd")).arg(hour, 2, 10, QChar('0')).arg(minu, 2, 10, QChar('0'));
	predictStartTime = QDateTime::fromString(str, "yyyy-MM-dd hh:mm::ss");
	predictEndTime = predictStartTime.addSecs(60 * 60 * 4 - 15 * 60);

	if ((currTime.time().hour() <= 4 || (currTime.time().hour() >= 16)
	{
		for (int i = 0; i < 16; i++)
		{

		}
	}*/
}

void CPowerPredictSvc::powerPredict72h(const QDateTime& currTime)
{

}