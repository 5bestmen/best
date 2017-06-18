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
	// ����Ϊÿ��ģ�����ʵ�ֵĽӿ�
	virtual void Init(IMainModuleInterface *pCore) = 0;

	virtual void UnInit() = 0;

	// �����ļ���д���˽ӿڣ�����������proj�����ļ������⣬��ģ����������ݵı���Ҳ�ڸýӿ���ʵ�֣�
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot) = 0;
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot) = 0;
	// �رչ����ļ�
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag) = 0;
	// �½�һ���յĹ����ļ�
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem) = 0;

	// ����ģ��ʵ�֣�����ģ�����
	// �������жϣ�ֻҪ���κ�һ����ģ��ġ��ࡱ��־���ڣ����Ƴ��Ի����ǡ���ȡ����������ǣ�����ģ�����������ģ��ı���ӿ�
	virtual bool GetModifyFlag()const = 0;	 
#if 0
	class CCheckRtn
	{
	public:
		CCheckRtn();
		~CCheckRtn();
	public:
		unsigned int m_nLevel;      //! �ȼ����澯������
		unsigned int m_nNumber;     //! �����
		QString      m_szDescript;  //! ����������磬ʹ����δ����ı�����
	};
#endif

	virtual bool Check() = 0;

	// �ض�ģ��ʵ�ֵ��ض��Ľӿ�
	// �ýӿ��� ǰ������ (CFesModule) ģ��ʵ�֣��ɹ�����ģ����ã���ȡ�����õ�ǰ����������
	virtual bool GetFesConfigName(std::vector< std::string > & arrFesNames)
	{
		Q_UNUSED(arrFesNames);

		return true;
	}

	// �ض�ģ��ʵ�ֵ��ض��Ľӿ�
	// �ýӿ��� ǰ������ (CScadaVariableModule) ģ��ʵ�֣��ɹ�����ģ����ã���ȡ�����õ�ǰ����������
	virtual bool GetScadaConfigName(std::vector< std::string > & arrScadaNames)
	{
		Q_UNUSED(arrScadaNames);

		return true;
	}

	// �ض�ģ��ʵ�ֵ��ض��Ľӿ�
	// �ýӿ��� ǰ������ (CNodeModule) ģ��ʵ�֣��ɹ�����ģ����ã���ȡ�����õĽڵ���������
	virtual std::string GetNodeNameByFesConfigName(std::string &szFesCongfigName)
	{
		Q_UNUSED(szFesCongfigName);

		return std::string();
	}
	// �ض�ģ��ʵ�ֵ��ض��Ľӿ�
	// �ýӿ��� ǰ������ (CNodeModule) ģ��ʵ�֣��ɹ�����ģ����ã���ȡ�����õĽڵ���������
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

	//��tagname�޸�,֪ͨ�󶨵��ģ��������Ӧ���޸� ɾ��tagnameΪ��
	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType) = 0;

	//�ַ��������ڴ��
	/*! \fn virtual bool GetDesPoolArr(std::set<std::string> *pVec, std::vector<std::string> *pStringPoolVec) = 0
	********************************************************************************************************* 
	** \brief IModule::GetDesPoolArr 
	** \details 
	** \param pVec ����vec
	** \param pStringPoolVec �ַ����ڴ��
	** \return bool 
	** \author gw
	** \date 2017��3��2�� 
	** \note 
	********************************************************************************************************/
	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo) = 0;
	//{
	//	Q_UNUSED(vec);
	//	Q_UNUSED(nOccNo);

	//	return true;
	//}
protected:
	QString  m_szModuleName; //! ģ������ ��FES\GRAPH\REPORT\... 

	// 
};

#endif // IMODEL_H

