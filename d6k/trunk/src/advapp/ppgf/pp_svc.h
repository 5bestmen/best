#ifndef PP_SVC_MODULE_H
#define PP_SVC_MODULE_H


#include "base_module.h"
#include "datatypes.h"

#include <QDateTime>


class CPowerPredictSvc : public CBaseModule
{
public:
	CPowerPredictSvc();
	~CPowerPredictSvc();

public:
	virtual bool Initialize();
	virtual void Run();
	virtual void Shutdown();

public:
	void powerPredict4h(const QDateTime& currTime);

	void powerPredict72h(const QDateTime& currTime);

protected:

	virtual void MainLoop();


};














#endif // !PP_SVC_MODULE_H
