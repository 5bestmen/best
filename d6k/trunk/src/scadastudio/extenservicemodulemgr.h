

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
*  \brief ģ����Ϣ����.
*/
class CExtendServiceModuleItem
{
public:
	// �ⲿģ���ô˹���
	explicit  CExtendServiceModuleItem(bool bIsExternal) :m_bIsExternalModule(bIsExternal)
	{
		m_nID = 0;
		m_nType = 0;
		m_pModuleInterface = nullptr;
		m_fnDestroyModule = nullptr;
 
	}
	// �ڲ�ģ���ô˹���
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
	QString      m_szDescript;         //! ��ʾ����Ҷ��
	IModule     *m_pModuleInterface;    //! ģ��Ľӿ�
	DestroyModule m_fnDestroyModule;
	QString      m_szName;
	QString      m_szDllName;  //! ��̬�����ƣ���ͬ��ϵͳƽ̨����Ҫ�Լ�׷����չ��

	unsigned int m_nType;      //! һ����������Ƕ��������������չ�������棩
	QString      m_szVersion;
protected:
	bool m_bIsExternalModule;
	CExtendServiceModuleItem()
	{

	}
};

#if 0
/*! \class CInternalModule  modulemgr.h  "modulemgr.h"
*  \brief �ڲ�ģ����Ϣ����.
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
*  \brief �ⲿģ����Ϣ����.
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
*  \brief ģ�����.
*/
class CExtendServiceModuleMgr 
{
public:
	CExtendServiceModuleMgr(IMainModuleInterface *pCore);
	~CExtendServiceModuleMgr();
public:
	enum INTERNAL_MODULE_ID
	{
		SYS_CONFIG_ID = 1,      //!< ϵͳ��̬
		NODE_CONFIG_ID = 2,     //!< �ڵ�����
		USER_MANAGER_ID = 3,    //!< �û�����
		FES_CONFIG_ID = 4,      //!< ǰ������
#if 0
		// ������̬ 5  ���ⲿ��xml����д��
		// ͼ��    6
		// ��ʷ�뱨�� 7
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
	//! ģ���б�
// 	std::vector<std::shared_ptr<CExternalModule>> m_arrExtModules; //! �ⲿģ��
// 	std::vector< IModule* > m_arrInnerModules; //! �ڲ�ģ��

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