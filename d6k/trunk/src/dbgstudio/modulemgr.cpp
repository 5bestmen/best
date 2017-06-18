/*! @file
<PRE>
********************************************************************************
ģ����       :  ����ģ��
�ļ���       :  managemodule.cpp
�ļ�ʵ�ֹ��� :  ��������ģ���ģ��
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date	2016.04.05
*/

#include "stl_util-inl.h"
#include "modulemgr.h"
#include "dbgstudio/plugin_module.h"
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

//���ض�̬��
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
				//���ü��صĲ��������
				pDllModule->SetLoadDeviceName(strDeviceName);
				//���
				CCaptureMsgWgt::GetInstance()->AddPluginSection(strDeviceName);
				// �˴��迼���Ƿ��μ���
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