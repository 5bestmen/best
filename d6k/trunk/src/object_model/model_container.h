
#ifndef MODEL_CONTAINER_H
#define MODEL_CONTAINER_H

#include <vector>
#include <memory>
#include <QString>

class CObjectModelInfo;

/*! \class  CModelGroup
*   \brief  对象模型组
*/
class CModelGroup
{
public:
	CModelGroup();
	explicit CModelGroup(const QString & szName);
	~CModelGroup();
public:
	// 导出成文件
	bool ExportToFile(const QString & szFullFilePath)const;
	// 从文件导入
	bool ImportFromFile(const QString & szFullFilePath);
	// 创建模型
	std::shared_ptr<CObjectModelInfo> CreateObjectModel(const QString & szName);
	// 删除
	bool RemoveObjectModel(const QString & szName);
	// 重命名
	bool RenameModel(const QString & szOldName, const QString & szNewName);
    //获取模型
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
	//! 组名称
	QString  m_szName;
};


/*! \class  CModelScene
*   \brief  业务场景定义
*/
class CModelScene
{
public:
	CModelScene();
	explicit CModelScene(const QString & szName);
	~CModelScene();
public:
	// 导出成文件
	bool ExportToFile(const QString & szFullFilePath)const;
	// 从文件导入
	bool ImportFromFile(const QString & szFullFilePath);
	// 创建组
	std::shared_ptr<CModelGroup> CreateModelGroup(const QString & szName);
	// 创建模型
	std::shared_ptr<CObjectModelInfo> CreateObjectModel(const QString & szName);
	// 删除
	bool RemoveModelGroup(const QString & szName);
	// 删除
	bool RemoveObjectModel(const QString & szName);
	// 重命名
	bool RenameModel(const QString & szOldName, const QString & szNewName);
    //获取模型
    std::vector<std::shared_ptr<CObjectModelInfo>> GetModels();
    //获取组
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
	// 业务场景下面可以直接有对象模型
	std::vector<std::shared_ptr<CObjectModelInfo>> m_arrModels;
	// 也可以有模型组
	std::vector<std::shared_ptr<CModelGroup>> m_arrModelGrps;
	//! 业务场景名称
	QString m_szName;
};

class CModelContainer
{
public:
	CModelContainer();
	~CModelContainer();
public:
	// 从路径加载
	bool LoadFromLibFile();
	// 保存到文件中
	void SaveToLibFile()const;
	// 创建业务场景
	std::shared_ptr<CModelScene> CreateModelScene(const QString & szName);
	// 删除业务场景
	bool RemoveModelScene(const QString & szName);
    //获取数据
    std::vector<std::shared_ptr<CModelScene>> GetModelScene();


private:
	//! 容器只能有场景
	std::vector<std::shared_ptr<CModelScene>>  m_arrScenes;
};
 

#endif // MODEL_CONTAINER_H
