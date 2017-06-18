#ifndef PREDICTION_MODULE_H
#define PREDICTION_MODULE_H

#include "scadastudio/imodule.h"
#include "define.h"
#include "predict_datainfo.h"

#include <QObject>

class IMainWindow;
class IMainModuleInterface;
class CTreeWidget;
class CPredictTree;
class QAbstractItemModel;
class QStandardItemModel;
class CQuoteItem;
class CPredictMgr;
class QStandardItem;
class CPredictGroup;
class CPowerPredictApi;

CPowerPredictApi *GetPredictModuleApi();

IMainModuleInterface *GetPreIMainInterface();


class CPowerPredictApi : public QObject, public IModule
{
	Q_OBJECT

public:
	CPowerPredictApi();
	~CPowerPredictApi();
public:
	//��ʼ������
	virtual void Init(IMainModuleInterface *pCore);

	//ж�ع���
	virtual void UnInit();

	//�½�����
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot, CQuoteItem *pTopItem);

	//���湤��
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot);

	//���ع���
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot);

	//�رչ���
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot, bool bSaveFlag)
	{
		Q_UNUSED(pXml);
		Q_UNUSED(pRoot);
		Q_UNUSED(szRoot);
		Q_UNUSED(bSaveFlag);
	}

	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType)
	{
		Q_UNUSED(tagName);
		Q_UNUSED(lastTagName);
		Q_UNUSED(nType);
		return true;
	}

	//��ȡ���
	virtual bool GetModifyFlag() const
	{
		return true;
	}

	virtual bool Check()
	{
		return true;
	}

	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_UNUSED(pStringPoolVec);
		Q_UNUSED(pHash);
		Q_UNUSED(pDescStringPoolOccNo);
		return true;
	}

	IMainModuleInterface * GetIMainInterface()
	{
		return m_pCore;
	}

public:

	//��־���
	bool LogMsg(const char* szLogTxt, int nLevel);

	void GetPredictItemMap(const QAbstractItemModel* pModel, const QModelIndex& top, int nType, QVector<QModelIndex>& indexVec);

	void SetPredictPath(const QString& path);

	bool LoadPredictNode(QDomElement& domElm, CQuoteItem* pTtem, CPredictGroup* pPrdtGrp,const QString& szRoot);

	bool LoadGroupNode(QDomElement& domElm, CQuoteItem* pItem, CPredictGroup* pPrdtGroup);

	bool LoadItemNode(QDomElement& domElm, CQuoteItem* pItem, CPredictGroup* pPrdtGrp);

	bool LoadPredictFile(CStationData* pStnData, const QString& fileName, const QString& szRoot);

	void CreatePredictGroupUi(CPredictGroup* pPtdtGroup, CQuoteItem* pItem);

private slots:

	//�����
	void Slot_AddGroup(QModelIndex& index);
	
	//ɾ����
	void Slot_DelGroup(QModelIndex& index);

	//���item
	void Slot_AddItem(QModelIndex& index);

	//ɾ��item
	void Slot_DelItem(QModelIndex& index);

	//��������
	void Slot_AddInverter(QModelIndex& index);

	//ɾ�������
	void Slot_DelInverter(QModelIndex& index);

	//
	void Slot_ItemChanged(QStandardItem* pItem);


	//����������վ
	//void Slot_OnLoadItem(const CPredictItem* pItem, const QModelIndex& index);


private:

	//��ģ��ָ��,Init()�ڳ�ʼ��
	IMainModuleInterface *m_pCore;

	//������ָ��,Init()�ڳ�ʼ�� 
	IMainWindow *m_pUi;

	//Ԥ�����ָ��,Init()�ڳ�ʼ��
	CPredictMgr *m_pPrdtMgr;

	//�����modelָ��,Init()�ڳ�ʼ��
	QStandardItemModel *m_pModel;

	//���οؼ�ָ��,Init()�ڳ�ʼ��
	CPredictTree* m_pPredictTree;

	//·��
	QString m_strPredictPath;

	//�����
	static int m_nTreeDepth;


};

#endif  //PREDICTION_MODULE_H