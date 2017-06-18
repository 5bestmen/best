#ifndef IUTILSMGR_H
#define IUTILSMGR_H

#include <QObject>

class IUserInfo : public QObject
{
	Q_OBJECT

	virtual ~IUserInfo()
	{

	}

	//登录用户信息
	virtual void GetUserLogOnInfo(void *pData) = 0;
	//用户权限信息
	virtual void GetUserAuthInfo(void *pData) = 0;
};
#endif // IUTILSMGR_H
