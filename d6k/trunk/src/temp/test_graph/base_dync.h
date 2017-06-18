/*! @file base_dync.h
<PRE>
********************************************************************************
模块名       :
文件名       : base_dync.h
文件实现功能 : 基本的动态定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  基本的动态定义
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#ifndef BASE_DYNC_DATA_H
#define BASE_DYNC_DATA_H
 
#include <string>


class CEventIntent
{
public:
	CEventIntent();
	virtual ~CEventIntent();
public:
	//! 打开新图形
	//std::function< bool(const CString&)> m_fnOpenNewGraph;


};
  
class CBaseDyncData  
{
public:
	CBaseDyncData();
	~CBaseDyncData();
public:
public:
	enum  DYNC_TYPE
	{
		DYNC_NULL = 0,
		DYNC_VALUE,          //!< 值显示
		DYNC_STRING,         //!< 字符串显示
		DYNC_COLOR,          //!< 颜色
		DYNC_VISIBLE,        //!< 可见
		DYNC_MOVE,           //!< 移动
		DYNC_SIZE,           //!< 大小变化
		DYNC_ROTATION,       //!< 旋转
		DYNC_TRANSPARENCY,   //!< 透明
		DYNC_FILLING,        //!< 填充
		DYNC_SYMBOL,
		DYNC_DRAGGING,       //!< 运行时图元的位置移动或者大小变化
		DYNC_GRID,           //!< 动态网格
		DYNC_EVENT,          //!< 动态事件，包含鼠标单击，按下，释放，激活，取消激活
		//	LineColor,FillColor,TextColor,Visible,Flicker,Rotate,Animation,
		//	SetValue,SetState,Graph,Operate,Logic,Menu,Ctrl
	};
public:
	//! 是否启用 也可以组态了，但是不启用他
	bool m_bEnable;
	//! 动态链接类型
	DYNC_TYPE m_nDyncType;
	//! 动态链接相关的测点
	std::string m_szTagName;
	//! 动态执行时的时间变量
	//boost::posix_time::ptime  m_tmLast;
public:
	// 比较两个动态连接是否是一样的类型
	virtual bool IsDyncTypeEqual(CBaseDyncData *pData) const;
	// 检查是否合法
	virtual bool CheckDyncData();
	// 预备动态
	virtual void PrepareWidgetDync(CBaseWidget *pWidget);
	// 动态部分的处理
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// 处理动态事件
//	virtual bool ProcessAction(CBaseWidget *pWidget, CEventIntent& intent);
	// 申请空间并复制自己
//	virtual CBaseDyncData *CloneDyncData() = 0;
protected:
 
private:

};


#endif // BASE_DYNC_DATA_H


/** @}*/
