#ifndef IMODEL_H
#define IMODEL_H

#include <QObject>
#include <unordered_map>

#include "datatypes.h"

class IMainModuleInterface;
class QDomDocument;
class QDomElement;
class CQuoteItem;
struct DESC_STRING_POOL;

namespace Config
{
	class CFesConfigData;
}

class IModule
{
public:
	virtual ~IModule()
	{

	}
	// 以下为每个模块必须实现的接口
	virtual void Init(IMainModuleInterface *pCore) = 0;

	virtual void UnInit() = 0;

	// 工程文件读写（此接口，不仅仅保存proj工程文件，另外，子模块的所有内容的保存也在该接口内实现）
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot) = 0;
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot) = 0;
	// 关闭工程文件
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag) = 0;
	// 新建一个空的工程文件
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem) = 0;

	// 由子模块实现，由主模块调用
	// 主程序判断，只要有任何一个子模块的“脏”标志存在，就推出对话框《是、否、取消》，如果是，则主模块调用所有子模块的保存接口
	virtual bool GetModifyFlag()const = 0;	 
#if 0
	class CCheckRtn
	{
	public:
		CCheckRtn();
		~CCheckRtn();
	public:
		unsigned int m_nLevel;      //! 等级：告警、错误
		unsigned int m_nNumber;     //! 错误号
		QString      m_szDescript;  //! 检查结果，比如，使用了未定义的变量。
	};
#endif

	virtual bool Check() = 0;

	// 特定模块实现的特定的接口
	// 该接口由 前置配置 (CFesModule) 模块实现，可供其他模块调用，获取已配置的前置数据名称
	virtual bool GetFesConfigName(std::vector< std::string > & arrFesNames)
	{
		Q_UNUSED(arrFesNames);

		return true;
	}

	// 特定模块实现的特定的接口
	// 该接口由 前置配置 (CScadaVariableModule) 模块实现，可供其他模块调用，获取已配置的前置数据名称
	virtual bool GetScadaConfigName(std::vector< std::string > & arrScadaNames)
	{
		Q_UNUSED(arrScadaNames);

		return true;
	}

	// 特定模块实现的特定的接口
	// 该接口由 前置配置 (CNodeModule) 模块实现，可供其他模块调用，获取已配置的节点数据名称
	virtual std::string GetNodeNameByFesConfigName(std::string &szFesCongfigName)
	{
		Q_UNUSED(szFesCongfigName);

		return std::string();
	}
	// 特定模块实现的特定的接口
	// 该接口由 前置配置 (CNodeModule) 模块实现，可供其他模块调用，获取已配置的节点数据名称
	virtual std::string GetFesConfigNameByNodeName(std::string &szFesNodeName)
	{
		Q_UNUSED(szFesNodeName);

		return std::string();
	}


	virtual QWidget* SelectTagDialog(const std::vector<int>& arrAllowDataTypes, bool bFilterEnWrite, std::string & szNodeTagName, std::string & szTagName, std::string &szFiledName)
	{
		Q_UNUSED(arrAllowDataTypes);
		Q_UNUSED(bFilterEnWrite);
		Q_UNUSED(szNodeTagName);
		Q_UNUSED(szTagName);
		Q_UNUSED(szFiledName)

		return nullptr;
	}

	//点tagname修改,通知绑定点的模块做出相应的修改 删除tagname为空
	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType) = 0;

	//字符串描述内存池
	/*! \fn virtual bool GetDesPoolArr(std::set<std::string> *pVec, std::vector<std::string> *pStringPoolVec) = 0
	********************************************************************************************************* 
	** \brief IModule::GetDesPoolArr 
	** \details 
	** \param pVec 查找vec
	** \param pStringPoolVec 字符串内存池
	** \return bool 
	** \author gw
	** \date 2017年3月2日 
	** \note 
	********************************************************************************************************/
	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo) = 0;
	//{
	//	Q_UNUSED(vec);
	//	Q_UNUSED(nOccNo);

	//	return true;
	//}
protected:
	QString  m_szModuleName; //! 模块名称 ：FES\GRAPH\REPORT\... 

	// 
};

#endif // IMODEL_H

