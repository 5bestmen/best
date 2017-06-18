

#ifndef EXTENDSERVICEMANAGEMODULE_H
#define EXTENDSERVICEMANAGEMODULE_H

// #include <QObject>
// #include <QVector>
// #include <QMap>
//#include <vector>
#include <memory>
#include <set>
//#include <map>
#include <QString>
 
class IModule;
class IMainModuleInterface;
class CQuoteItem;
class QDomDocument;
class QDomElement;
class QString;
class CExtendServiceModule;

#define  EXTENDSERVICEPLUGIN_XML "scadastudio.xml"


typedef IModule *(*CreateModule)();
typedef void(*DestroyModule)();

/*! \class CModuleItem  modulemgr.h  "modulemgr.h"
*  \brief 模块信息基类.
*/
class CExtendServiceModuleItem
{
public:
	// 外部模块用此构造
	explicit  CExtendServiceModuleItem(bool bIsExternal) :m_bIsExternalModule(bIsExternal)
	{
		m_nID = 0;
		m_nType = 0;
		m_pModuleInterface = nullptr;
		m_fnDestroyModule = nullptr;
 
	}
	// 内部模块用此构造
	explicit CExtendServiceModuleItem(IModule *pInner, QString &strModuleName)
	{
		m_fnDestroyModule = nullptr;
		m_pModuleInterface = pInner;
		//m_nID = nID;
		m_szDllName = strModuleName;
		//assert(pInner);
		m_bIsExternalModule = false;

	}
	virtual ~CExtendServiceModuleItem()
	{

	}
	unsigned int GetID()
	{
		return m_nID;
	}
	bool operator < (const CExtendServiceModuleItem& other) const
	{
		return m_nID < other.m_nID;
	}
	bool IsExternalModule()
	{
		return m_bIsExternalModule;
	}
public:
	unsigned int m_nID;
	QString      m_szDescript;         //! 显示在树叶上
	IModule     *m_pModuleInterface;    //! 模块的接口
	DestroyModule m_fnDestroyModule;
	QString      m_szName;
	QString      m_szDllName;  //! 动态库名称，不同的系统平台上需要自己追加扩展名

	unsigned int m_nType;      //! 一级插件？还是二级插件（放在扩展服务下面）
	QString      m_szVersion;
protected:
	bool m_bIsExternalModule;
	CExtendServiceModuleItem()
	{

	}
};

#if 0
/*! \class CInternalModule  modulemgr.h  "modulemgr.h"
*  \brief 内部模块信息基类.
*/
class CInternalModule : public CModuleItem
{
public:
	CInternalModule(IModule *pInner,unsigned int nID)
	{
		m_bIsExternalModule = false;

	}
	virtual ~CInternalModule()
	{

	}
private:
	CInternalModule()
	{
		m_bIsExternalModule = false;

	}
};

/*! \class CExternalModule  modulemgr.h  "modulemgr.h"
*  \brief 外部模块信息基类.
*/
class CExternalModule : public CModuleItem
{
public:
	CExternalModule()
	{
		m_nID = 0;
		m_nType = 0;
		m_pModuleInterface = nullptr;
		m_fnDestroyModule = nullptr;
		m_bIsExternalModule = true;
	}
	virtual ~CExternalModule()
	{

	}
public:

};
#endif

/*! \class CModuleMgr  modulemgr.h  "modulemgr.h"
*  \brief 模块管理.
*/
class CExtendServiceModuleMgr 
{
public:
	CExtendServiceModuleMgr(IMainModuleInterface *pCore);
	~CExtendServiceModuleMgr();
public:
	enum INTERNAL_MODULE_ID
	{
		SYS_CONFIG_ID = 1,      //!< 系统组态
		NODE_CONFIG_ID = 2,     //!< 节点配置
		USER_MANAGER_ID = 3,    //!< 用户管理
		FES_CONFIG_ID = 4,      //!< 前置配置
#if 0
		// 对象组态 5  在外部的xml里面写死
		// 图形    6
		// 历史与报表 7
#endif

	};
public:
	bool LoadPlugInXml();

	virtual bool LoadProjFromFile(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath);

	virtual bool SaveToProjectFile(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath);

	virtual bool LoadInternalModule();

	virtual bool CreateNewProj(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath, CQuoteItem *pTopItem);

	virtual bool  CloseProj(QDomDocument *pDocument, const QString& szPath);


	void AddExternalModule(std::shared_ptr<CExtendServiceModuleItem>pModule);
	void LoadModules();
	void UnLoadModules();


	bool LogString(const char *szLogTxt, int nLevel);

	IModule *GetModule(const char *szModuleName);

	std::set< std::shared_ptr<CExtendServiceModuleItem> > &GetModuleArr()
	{
		return m_setModules;
	}

	QDomDocument *GetDocument()
	{
		return m_pDocument;
	}

	QDomElement *GetElement()
	{
		return m_pRoot;
	}

	QString GetPath()
	{
		return m_szPath;
	}

	CQuoteItem *GetExtendServiceItem();

	

private:
	//! 模块列表
// 	std::vector<std::shared_ptr<CExternalModule>> m_arrExtModules; //! 外部模块
// 	std::vector< IModule* > m_arrInnerModules; //! 内部模块

	std::set< std::shared_ptr<CExtendServiceModuleItem> > m_setModules;

private:
//	QMap<int, IModule *> m_module;
//	QMap<int, FLAG> m_state;

	IMainModuleInterface *m_pCore;
	QDomDocument *m_pDocument;
	QDomElement *m_pRoot;
	QString m_szPath;
	CQuoteItem *m_pTopItem;
	CExtendServiceModule *m_pExtendServiceModule;
};

#endif // EXTENDSERVICEMANAGEMODULE_H

/** @}*/