#ifndef IUTILSMGR_H
#define IUTILSMGR_H

#include <QObject>

class IUserInfo : public QObject
{
	Q_OBJECT

	virtual ~IUserInfo()
	{

	}

	//��¼�û���Ϣ
	virtual void GetUserLogOnInfo(void *pData) = 0;
	//�û�Ȩ����Ϣ
	virtual void GetUserAuthInfo(void *pData) = 0;
};
#endif // IUTILSMGR_H
