#ifndef PROJMGR_H
#define PROJMGR_H

#include <QObject>
#include <QLibrary>
#include "scadastudio/iprojmgr.h"
#include "../src/equipmentmodel/equipmentmodel.h"

//class IMainModuleInterface;
class  CCore;
class CProjMgr : public QObject, public IProjMgr
{
	Q_OBJECT

public:
	CProjMgr(CCore *pCore);
	~CProjMgr();

	virtual QDomDocument *GetDocument();

	virtual QFile *GetFile()
	{
		if (!m_pFile)
		{
			return nullptr;
		}
		else
		{
			return m_pFile;
		}
	}

	QString GetProjectPath()
	{
		return m_strProjectPath;
	}

    // ��ʼ��ģ�͹���
    void InitModelTool();

    // �ͷ�
    void FiniModelTool();

	void SetCurrentFile(const QString &fileName);

public slots:
	//�½�����
	void CreateNewProj();
	//�򿪹���
	void OpenProj();
	//���湤��
	void SaveProj();
	//�رչ���
	void CloseProj();
	void EquipmentModel();

	void OpenRecentFile();
private:
	void updateRecentFileActions();
	void loadFile(const QString &fileName);
private:
	QDomDocument *m_pDomDocument;

	QDomElement m_root;

	CCore *m_pCore;

	QFile *m_pFile;

	QString m_strProjectPath;

    QLibrary* m_pModelLib;
    equipmentmodel* m_pModelDll;

	enum { MaxRecentFiles = 5 };
	QAction *recentFileActs[MaxRecentFiles];
	QAction *m_pSeparatorAct;

	QString m_strFile;
};

#endif // PROJMGR_H
