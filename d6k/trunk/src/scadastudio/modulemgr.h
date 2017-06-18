/*! @file modulemgr.h
<PRE>
********************************************************************************
模块名       :
文件名       :  modulemgr.h
文件实现功能 :  模块管理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  各模块的ID需要严肃认真对待，不能随意修改
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   模块管理
*  @author  LiJin
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef MANAGEMODULE_H
#define MANAGEMODULE_H

#include "datatypes.h"

// #include <QObject>
// #include <QVector>
// #include <QMap>
//#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
//#include <map>
#include <QString>
#include <vector>
 
class IModule;
class IMainModuleInterface;
class CQuoteItem;
class QDomDocument;
class QDomElement;
class QString;

#define  PLUGIN_XML "scadastudio.xml"


typedef IModule *(*CreateModule)();
typedef void(*DestroyModule)();

/*! \class CModuleItem  modulemgr.h  "modulemgr.h"
*  \brief 模块信息基类.
*/
class CModuleItem
{
public:
	// 外部模块用此构造
	explicit  CModuleItem(bool bIsExternal) :m_bIsExternalModule(bIsExternal)
	{
		m_nID = 0;
		m_nType = 0;
		m_pModuleInterface = nullptr;
		m_fnDestroyModule = nullptr;
 
	}
	// 内部模块用此构造
	explicit CModuleItem(IModule *pInner,const QString &strModuleName, const QString & szDescript)
	{
		m_fnDestroyModule = nullptr;
		m_pModuleInterface = pInner;
		m_nID = 0;
		m_nType = 0;
		m_szDllName = strModuleName;
		m_szDescript = szDescript;
		//assert(pInner);
		m_bIsExternalModule = false;
	}
	virtual ~CModuleItem()
	{

	}
	unsigned int GetID()
	{
		return m_nID;
	}
	bool operator < (const CModuleItem& other) const
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
	CModuleItem()
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
class CModuleMgr 
{
public:
	CModuleMgr(IMainModuleInterface *pCore);
	~CModuleMgr();
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
	bool AddInternalModule(IModule *pInner,const QString & strModuleName, const QString & szDescript);

	bool LoadPlugInXml();

	virtual bool LoadProjFromFile(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath);

	virtual bool SaveToProjectFile(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath);

	virtual bool LoadInternalModule();

	virtual bool CreateNewProj(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath, CQuoteItem *pTopItem);

	virtual bool  CloseProj(QDomDocument *pDocument, const QString& szPath);


	void AddExternalModule(std::shared_ptr<CModuleItem>pModule);
	void LoadModules();
	void UnLoadModules();


	bool LogString(const char *szLogTxt, int nLevel);

	IModule *GetModule(const char *szModuleName);
	bool GetModulesName(std::vector<QString> & arrNames);
private:
	void SaveDescStringPool();
private:
	//! 模块列表
// 	std::vector<std::shared_ptr<CExternalModule>> m_arrExtModules; //! 外部模块
// 	std::vector< IModule* > m_arrInnerModules; //! 内部模块

	std::set< std::shared_ptr<CModuleItem> > m_setModules;

	//字符串内存池
	std::unordered_map<std::string, int32u> m_arrMap;
	std::vector<std::string> m_arrStringPoolVec;
private:
//	QMap<int, IModule *> m_module;
//	QMap<int, FLAG> m_state;

	IMainModuleInterface *m_pCore;
};

#endif // MANAGEMODULE_H

/** @}*/