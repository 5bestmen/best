#ifndef SYSMODEL_H
#define SYSMODEL_H

#include <QObject>
#include "scadastudio/imodule.h"

class IMainModuleInterface;
class IMainWindow;

class CSysConfigurationModule : public QObject, public IModule
{
	Q_OBJECT

public:
	CSysConfigurationModule();
	~CSysConfigurationModule();

	void Init(IMainModuleInterface *pUi);
	void UnInit()
	{

	}

	// 工程文件读写
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	// 关闭工程文件
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag);
	// 新建一个空的工程文件
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem);

	// 由子模块实现，由主模块调用
	// 主程序判断，只要有任何一个子模块的“脏”标志存在，就推出对话框《是、否、取消》，如果是，则主模块调用所有子模块的保存接口
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

	//字符串描述内存池
	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_UNUSED(pHash);
		Q_UNUSED(pStringPoolVec);
		Q_UNUSED(pDescStringPoolOccNo);

		return true;
	}

public slots:

private:
	IMainWindow *m_pUi;
	IMainModuleInterface *m_pCore;
};

#endif // SYSMODEL_H
