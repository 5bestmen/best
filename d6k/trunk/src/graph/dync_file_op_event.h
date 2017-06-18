/*! @file dync_file_op_event.h
<PRE>
********************************************************************************
模块名       :
文件名       :  dync_file_op_event.h
文件实现功能 :  文件操作命令
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本的命令事件定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/
#ifndef DYNC_FILE_OP_EVENT_H
#define DYNC_FILE_OP_EVENT_H


#include "dync_event.h"
#include "base_dync_event_item.h"

#include <string>
#include <memory>
#include <vector>

#include <QString>


class QXmlStreamWriter;
class QXmlStreamReader;

/*! \class CDyncFileOpEventItem DynamicDef.h  "DynamicDef.h"
*  \brief 文件操作命令
*  \details  文件的操作模式有：MDI方式新打开文件，模式打开新文件，关闭文件，打开下一个，上一个
*/
class  CDyncFileOpEventItem : public CBaseDyncEventItem
{
public:
	CDyncFileOpEventItem();
	CDyncFileOpEventItem(const CDyncFileOpEventItem *);
	virtual ~CDyncFileOpEventItem();

	CDyncFileOpEventItem& operator=(const CDyncFileOpEventItem& src);
	bool operator==(const CDyncFileOpEventItem& src) const;
public:
	enum FILE_OP_TYPE
	{
		FILE_OPEN_NORMAL = 0, //!< 关闭，并打开一个新的文件，
		FILE_CLOSE_FILE,     //!< 关闭
		FILE_MODAL_OPEN_FILE, //!< 以模式对话框打开
		FILE_MDI_OPEN_FILE,
		FILE_OPEN_NEXT_FILE,
		FILE_OPEN_PREV_FILE,
		MAX_FILE_OP_NUM
	};
public:
	virtual std::string  GetEventDescript()const;
// 	virtual void SaveXml(JWXml::CXmlNodePtr  pDyncPtr) const;
// 	virtual bool LoadXml(JWXml::CXmlNodePtr  pDyncPtr);
// 
	//! 画面文件操作类型文本
	static std::array<QString, MAX_FILE_OP_NUM> m_arrFileOpTypeTxt;

	const std::string  & GetGraphFileName() const;
	// 文件操作类型
	void SetFileOpType(const QString  & szTxt);
	QString GetFileOpTypeTxt() const;
	// 设置画面位置
	void SetGraphPos(const std::string  & szX, const std::string  & szY);
	void SetGraphXPos(const std::string  & szX);
	void SetGraphYPos(const std::string  & szY);

	void SetFileName(const std::string  & szName);

	unsigned int GetGraphXPos() const
	{
		return m_nXPos;
	}
	unsigned int GetGraphYPos() const
	{
		return m_nYPos;
	}
	// 处理事件
	virtual void DoAction(CEventIntent &intent);
private:
	//! 文件操作类型
	FILE_OP_TYPE  m_FileOpType;
	//! 文件名，格式： 目录\文件 不包含后缀
	std::string  m_szGraphFileName;

	unsigned int m_nXPos;
	unsigned int m_nYPos;
};
#endif // BASE_DYNC_EVENT_H


/** @}*/
