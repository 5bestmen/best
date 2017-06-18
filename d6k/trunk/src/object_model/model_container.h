
#ifndef MODEL_CONTAINER_H
#define MODEL_CONTAINER_H

#include <vector>
#include <memory>
#include <QString>

class CObjectModelInfo;

/*! \class  CModelGroup
*   \brief  ����ģ����
*/
class CModelGroup
{
public:
	CModelGroup();
	explicit CModelGroup(const QString & szName);
	~CModelGroup();
public:
	// �������ļ�
	bool ExportToFile(const QString & szFullFilePath)const;
	// ���ļ�����
	bool ImportFromFile(const QString & szFullFilePath);
	// ����ģ��
	std::shared_ptr<CObjectModelInfo> CreateObjectModel(const QString & szName);
	// ɾ��
	bool RemoveObjectModel(const QString & szName);
	// ������
	bool RenameModel(const QString & szOldName, const QString & szNewName);
    //��ȡģ��
    std::vector<std::shared_ptr<CObjectModelInfo>> GetModels();

	const QString & GetName()const
	{
		return m_szName;
	}

	void SetName(const QString & szName)
	{
		m_szName = szName;
	}
private:
	std::vector<std::shared_ptr<CObjectModelInfo>> m_arrModels;
	//! ������
	QString  m_szName;
};


/*! \class  CModelScene
*   \brief  ҵ�񳡾�����
*/
class CModelScene
{
public:
	CModelScene();
	explicit CModelScene(const QString & szName);
	~CModelScene();
public:
	// �������ļ�
	bool ExportToFile(const QString & szFullFilePath)const;
	// ���ļ�����
	bool ImportFromFile(const QString & szFullFilePath);
	// ������
	std::shared_ptr<CModelGroup> CreateModelGroup(const QString & szName);
	// ����ģ��
	std::shared_ptr<CObjectModelInfo> CreateObjectModel(const QString & szName);
	// ɾ��
	bool RemoveModelGroup(const QString & szName);
	// ɾ��
	bool RemoveObjectModel(const QString & szName);
	// ������
	bool RenameModel(const QString & szOldName, const QString & szNewName);
    //��ȡģ��
    std::vector<std::shared_ptr<CObjectModelInfo>> GetModels();
    //��ȡ��
    std::vector<std::shared_ptr<CModelGroup>> GetModelGrps();


	const QString & GetName()const
	{
		return m_szName;
	}

	void SetName(const QString & szName)
	{
		m_szName = szName;
	}
private:
	// ҵ�񳡾��������ֱ���ж���ģ��
	std::vector<std::shared_ptr<CObjectModelInfo>> m_arrModels;
	// Ҳ������ģ����
	std::vector<std::shared_ptr<CModelGroup>> m_arrModelGrps;
	//! ҵ�񳡾�����
	QString m_szName;
};

class CModelContainer
{
public:
	CModelContainer();
	~CModelContainer();
public:
	// ��·������
	bool LoadFromLibFile();
	// ���浽�ļ���
	void SaveToLibFile()const;
	// ����ҵ�񳡾�
	std::shared_ptr<CModelScene> CreateModelScene(const QString & szName);
	// ɾ��ҵ�񳡾�
	bool RemoveModelScene(const QString & szName);
    //��ȡ����
    std::vector<std::shared_ptr<CModelScene>> GetModelScene();


private:
	//! ����ֻ���г���
	std::vector<std::shared_ptr<CModelScene>>  m_arrScenes;
};
 

#endif // MODEL_CONTAINER_H
