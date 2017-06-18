/*! @file
<PRE>
********************************************************************************
模块名       :  管理模块
文件名       :  managemodule.cpp
文件实现功能 :  惯例各个模块的模块
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date	2016.04.05
*/

#include "stl_util-inl.h"
#include "modulemgr.h"
#include "devicestudio/plugin_module.h"
#include "capturemsgwgt.h"

#include <QLibrary>


typedef IPluginModule *(*FunInitModel)();

CModuleMgr::CModuleMgr(IMainModule *pMainModule)
{
	m_pCore = pMainModule;
}

CModuleMgr::~CModuleMgr()
{
	STLDeleteElements(&m_arrModules);
}

//加载动态库
bool CModuleMgr::LoadModule(const QString &module, const QString &strDeviceName)
{
	QLibrary mylib(module);
	if (mylib.load())
	{
		FunInitModel CreateModule = (FunInitModel)mylib.resolve("CreateModule");
		if (CreateModule == nullptr)
		{
			return false;
		}
		else
		{
			IPluginModule *pDllModule = CreateModule();
			if (pDllModule == nullptr)
			{
				return false;
			}
			else
			{
				pDllModule->Init(m_pCore);
				//设置加载的插件的名称
				pDllModule->SetLoadDeviceName(strDeviceName);
				//插件
				CCaptureMsgWgt::GetInstance()->AddPluginSection(strDeviceName);
				// 此处需考虑是否多次加载
				auto it_find = std::find(m_arrModules.begin(), m_arrModules.end(), pDllModule);
				if (it_find == m_arrModules.end())
				{
					m_arrModules.push_back(pDllModule);
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}