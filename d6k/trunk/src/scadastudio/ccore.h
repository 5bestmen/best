#ifndef CCORE_H
#define CCORE_H

#include "scadastudio/icore.h"
#include "scadastudiodefine.h"

#include <QObject>
#include <QVector>

#include <QObject>
#include "../src/fespub/tag_att.h"
#include "variant.h"


class IMainModuleInterface;
class IProjMgr;
class CModuleMgr;
//class COutputMessagerClient;
class CExtendServiceModuleMgr;

class CCore : public QObject, public IMainModuleInterface
{
	Q_OBJECT
public:
	CCore(QObject *parent);
	virtual ~CCore();

	//���main uiָ��
	virtual IMainWindow *GetUIMgr();

	//���ģ�����ָ��
	virtual CModuleMgr *GetModuleMgr();

	//�����չ����ģ�����ָ��
	virtual CExtendServiceModuleMgr *GetExtendServiceModuleMgr();

	//��ù��̹���ָ��
	virtual IProjMgr *GetProjMgr();

	//����ģ��
	void LoadModules();
	//virtual bool SelectPoint(std::string & szInputPointName);

	virtual bool SelectTag(const std::vector<int>& arrAllowDataTypes, bool bFilterEnWrite, std::string & szNodeTagName, std::string & szTagName, std::string &szFiledName);

	//Log
	virtual bool LogMsg(const  char *pszMoudleName,const char *szLogTxt, int nLevel);
	virtual bool LogCheckMsg(const char *pszMoudleName, const char *szDescript, int nLevel, int nErrNo) ;

	virtual IModule *GetModule(const char *szTxt);

	virtual bool GetModulesName(std::vector<QString> & arrNames);

 	//����ģ���޸�
// 	virtual void SetModifyFlag(int nModuleID, bool bIsModified);

// 	//���ģ��״̬
// 	virtual bool GetModifyFlag(int nModuleID);

	//�����ڲ�ģ��
	bool LoadInternalModule();
private:
	IMainWindow *m_pUi;

	IProjMgr *m_pProjMgr;

	CModuleMgr *m_pModuleMgr;

	CExtendServiceModuleMgr *m_pExtendServiceModuleMgr;

//	COutputMessagerClient *m_pOutClient;

public slots:
	void DeleteCoreSlot();
};

#endif // CCORE_H
