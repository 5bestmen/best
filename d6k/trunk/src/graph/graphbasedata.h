#ifndef D_GRAPHBASEDATA_H
#define D_GRAPHBASEDATA_H

#include <string>
#include <QString>

class CGraphBaseData
{
public:
	enum 
	{
		BASE_TYPE_FOLDER = 1,    //�ļ�Ŀ¼
		BASE_TYPE_FILE,      //�ļ�
	};

	CGraphBaseData();

	virtual ~CGraphBaseData();

	int GetItemType()
	{
		return m_nItemType;
	}

	void SetItemType(int nType)
	{
		m_nItemType = nType;
	}

	void SetItemName(const std::string &strItemName)
	{
		m_strItemName = strItemName;
	}

	const std::string &GetItemName()
	{
		return m_strItemName;
	}

	virtual void GetItemInfo();

	void SetFilePath(const QString &strItemPath)
	{
		m_strFilePath = strItemPath;
	}

	const QString& GetFilePath()
	{
		return m_strFilePath;
	}
private:
	//item����
	int m_nItemType;
	//item��ʾ����
	std::string m_strItemName;
	//�ļ�����·��
	QString m_strFilePath;
};

#endif // D_GRAPH_FOLDER_H
