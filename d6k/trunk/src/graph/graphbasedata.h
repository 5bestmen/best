#ifndef D_GRAPHBASEDATA_H
#define D_GRAPHBASEDATA_H

#include <string>
#include <QString>

class CGraphBaseData
{
public:
	enum 
	{
		BASE_TYPE_FOLDER = 1,    //文件目录
		BASE_TYPE_FILE,      //文件
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
	//item类型
	int m_nItemType;
	//item显示内容
	std::string m_strItemName;
	//文件绝对路径
	QString m_strFilePath;
};

#endif // D_GRAPH_FOLDER_H
