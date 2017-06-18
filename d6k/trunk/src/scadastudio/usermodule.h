#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>

#include "scadastudio/imodule.h"

class IMainWindow;
class IMainModuleInterface;
class CUser;

class CUserManageModule : public QObject, public IModule
{
	Q_OBJECT

public:
	CUserManageModule();
	~CUserManageModule();

	void Init(IMainModuleInterface *pCore);
	void UnInit()
	{

	}

	// �����ļ���д
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	// �رչ����ļ�
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag);
	// �½�һ���յĹ����ļ�
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem);

	// ����ģ��ʵ�֣�����ģ�����
	// �������жϣ�ֻҪ���κ�һ����ģ��ġ��ࡱ��־���ڣ����Ƴ��Ի����ǡ���ȡ����������ǣ�����ģ�����������ģ��ı���ӿ�
	virtual bool GetModifyFlag() const;

	virtual bool Check()
	{
		return true;
	}

	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType)
	{
		Q_UNUSED(tagName);
		Q_UNUSED(lastTagName);

		return true;
	}

	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_UNUSED(pHash);
		Q_UNUSED(pStringPoolVec);
		Q_UNUSED(pDescStringPoolOccNo);

		return true;
	}

private slots:

private:
	IMainWindow *m_pUi;

	IMainModuleInterface *m_pCore;

	CUser *m_pUser;
};

#endif // USERMODEL_H
