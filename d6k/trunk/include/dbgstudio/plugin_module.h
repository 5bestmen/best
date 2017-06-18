#ifndef IMODEL_H
#define IMODEL_H
#include <QObject>

class IMainModule;

class IPluginModule : public QObject
{
	Q_OBJECT
public:
	IPluginModule()
	{
		 
	}
	virtual IPluginModule::~IPluginModule()
	{

	}

	virtual void Init(IMainModule *pCore) = 0;

	virtual void UnInit() = 0;

	virtual void SetLoadDeviceName(const QString &strDeviceName) = 0;
 
//	virtual IMainModule *GetMainModule() const = 0;
};

#endif // IMODEL_H

