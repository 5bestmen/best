/*! @file
<PRE>
********************************************************************************
模块名       : 主程序模块内核提供接口 【对外接口】供插件模块调用
文件名       :
文件实现功能 :
作者         :
版本         :
--------------------------------------------------------------------------------
备注         :
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
*  
********************************************************************************
</PRE>
*  @brief 主程序提供接口【对外接口】.
*  @author gw
*  @version 1.0
*  @date    2016.08.16
*/

#ifndef ICORE_H
#define ICORE_H

#include <QString>
#include <vector>

class IMainWindow;
class IModuleMgr;
class IProjMgr;
class IModule;
class CExtendServiceModuleMgr;

class IMainModuleInterface
{
public:
	//获得main ui指针
	virtual IMainWindow *GetUIMgr() = 0;

	//获得模块管理指针

	//virtual CModuleMgr *GetModuleMgr() = 0;

	//获得工程管理指针
	virtual IProjMgr *GetProjMgr() = 0;

	//获得扩展服务模块管理指针
	virtual CExtendServiceModuleMgr *GetExtendServiceModuleMgr() = 0;

	//virtual bool LoadModule(int nModuleID) = 0;
	// 由主程序实现，插件可以调用该接口
	virtual bool LogMsg(const char *pszMoudleName,const char *szLogTxt, int nLevel) = 0;
	// 由主程序实现，插件可以调用该接口
	virtual bool LogCheckMsg(const char *pszMoudleName, const char *szDescript, int nLevel,int nErrNo) = 0;

	virtual bool GetModulesName(std::vector<QString> & arrNames) = 0;
	// 获取到某个模块的指针
	virtual IModule *GetModule(const char *szTxt) = 0;

	/*! \fn virtual bool SelectTag(const std::vector<int>& arrAllowDataTypes,bool bFilterEnWrite, std::string & szNodeTagName,
	std::string & szTagName, std::string &szFiledName);
	*********************************************************************************************************
	** \brief CCore::SelectTag
	** \details
	** \param arrAllowDataTypes 输入参数，允许的数据类型，比如：允许选择Float,Double
	** \param bFilterEnWrite   输入参数： 如果该参数为true，说明选择可写的属性
	** \param szNodeTagName 输出参数：节点名称，如果是选择变量，需要定义节点TAGNAME，如果是选择对象，则该参数无需返回
							应用程序（图形、逻辑、报表等等）对测点、对象的读写访问采用这种模式：
							节点名.变量名.属性

							如：  F1.AIN004.VALUE  前置节点F1的模拟量AIN004的测值
							F2.DIN008.QUA    前置节点F2的开关量DIN008的品质
							F4.$CPU          前置节点F4的CPU负载，系统变量$CPU

	** \param szTagName     输出参数：变量名称
	** \param szFiledName   输出参数：属性名称，比如测值是Value，品质是Quality...，如果为空，则默认认为是Value
	** \return bool         输出参数：如果是false，则返回参数不可用
	** \author LiJin
	** \date 2016年10月13日
	** \note
	********************************************************************************************************/
	virtual bool SelectTag(const std::vector<int>& arrAllowDataTypes, bool bFilterEnWrite, std::string & szNodeTagName, std::string & szTagName, std::string &szFiledName) = 0;


	/*virtual bool SelectPoint(std::string & szTagName) = 0;*/
};

/** @}*/
#endif // ICORE_H
