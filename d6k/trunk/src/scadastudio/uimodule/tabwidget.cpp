/*! @file
<PRE>
********************************************************************************
模块名       :  Tab
文件名       :  tabwidget.cpp
文件实现功能 :  分离器
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
*  @date    2015.11.15
*/

#include "scadastudio/tabwidget.h"
#include "scadastudio/base.h"
#include "scadastudio/icore.h"
#include "scadastudiodefine.h"
#include "scadastudio/define.h"
#include <QMessageBox>

extern IMainModuleInterface *s_pGlobleCore;

/*! \fn CTabWidget::CTabWidget(QObject *parent)
********************************************************************************************************* 
** \brief CTabWidget::CTabWidget 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CTabWidget::CTabWidget(QWidget *parent)
	: QTabWidget(parent), m_pBeforeCloseTabData(nullptr)
{
	this->setTabsClosable(true);

	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTileWidget(int)));
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

	m_mapTabKey.clear();
	m_mapWidget.clear();
}

/*! \fn CTabWidget::~CTabWidget()
********************************************************************************************************* 
** \brief CTabWidget::~CTabWidget 
** \details 
** \return  
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CTabWidget::~CTabWidget()
{

}

/*! \fn void CTabWidget::closeTileWidget(int index)
********************************************************************************************************* 
** \brief CTabWidget::closeTileWidget 
** \details 保存widget数据
** \param index 
** \return void 
** \author gw
** \date 2016年2月18日 
** \note 
********************************************************************************************************/
void CTabWidget::closeTileWidget(int index)
{
	if (m_fnBeforeCloseTab)
	{
		m_fnBeforeCloseTab(m_pBeforeCloseTabData);
	}
	
	auto pWdiget = widget(index);
	Q_ASSERT(pWdiget);
	if (!pWdiget)
	{
		return;
	}

	if (m_mapTabKey.contains(pWdiget))
	{
		auto &info = m_mapTabKey[widget(index)];

		if (m_mapWidget.contains(info))
		{
			m_mapWidget.remove(info);
			m_mapTabKey.remove(pWdiget);
		}
	}

	IBase *pBase = dynamic_cast<IBase *>(pWdiget);

	//发给别的模块
	emit SendCloseWidget(widget(index));

	Q_ASSERT(pBase);
	if (pBase)
	{
		pBase->Save();

		this->removeTab(index);

		//pBase->Delete();

		delete pWdiget;
	}
}

/*! \fn 
********************************************************************************************************* 
** \brief CTabWidget::tabChanged 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年2月18日 
** \note 
********************************************************************************************************/
void CTabWidget::tabChanged(int index)
{
	Q_UNUSED(index);
}

/*! \fn bool CTabWidget::IsWidgetInTab(const QString &strTagName, const int &nType)
*********************************************************************************************************
** \brief CTabWidget::IsWidgetInTab
** \details 判断tab是否存在该窗口
** \param strTagName
** \param nType
** \return bool
** \author gw
** \date 2016年7月26日
** \note
********************************************************************************************************/
bool CTabWidget::IsWidgetInTab(const QString &strTagName, const int &nType, const QString &strBigTagName)
{
	auto bFlag = strTagName.isEmpty() || strTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		QMessageBox box;
		box.setText(QObject::tr("tagname %1 is invalid!!!").arg(strTagName));
		box.exec();

		return false;
	}

	//Bigtagname有的模块是空的
	bFlag = /*strBigTagName.isEmpty() || */strBigTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		QMessageBox box;
		box.setText(QObject::tr("Bigtagname %1 is longer %2!!!").arg(strBigTagName).arg(MAX_TAGNAME_LEN_SCADASTUDIO));
		box.exec();

		return false;
	}

	TabKeyStruct info;
	info.TagName = strTagName;
	info.Type = nType;
	info.BigTagName = strBigTagName;

	bFlag = m_mapWidget.contains(info);

	if (bFlag)
	{
		setCurrentWidget(m_mapWidget[info]);
	}

	return bFlag;
}

void CTabWidget::SetTabName(const QString &strTagName, const int &nType, const QString &strLastTag)
{
	TabKeyStruct info;
	info.TagName = strLastTag;
	info.Type = nType;

	auto bFlag = m_mapWidget.contains(info);
	if (bFlag)
	{
		auto ite = m_mapWidget.find(info);
		auto pWidget = ite.value();
		
		m_mapWidget.erase(ite);
		
		info.TagName = strTagName;
		info.Type = nType;

		m_mapWidget.insert(info, pWidget);
		if (m_mapTabKey.contains(pWidget))
		{
			m_mapTabKey[pWidget] = info;
		}
		else
		{
			auto strError = tr("can not find widget!!!");
			s_pGlobleCore->LogMsg(TABWIDGET_DESC, strError.toStdString().c_str(), LEVEL_1);
		}

	}
	else
	{
		QMessageBox box;
		box.setText(QObject::tr("Tabwidget module:tagname is invalid!!!"));
		box.exec();
	}

	auto nIndex = this->indexOf(m_mapWidget[info]);
	if (nIndex == -1)
	{
		auto strError = tr("can not find widget!!!");
		s_pGlobleCore->LogMsg(TABWIDGET_DESC, strError.toStdString().c_str(), LEVEL_1);
	}
	setTabText(nIndex, strTagName);
}

void CTabWidget::SetTabName(const TabKeyStruct &newKey, const TabKeyStruct &lastKey)
{
	auto bFlag = newKey.TagName.isEmpty() || lastKey.TagName.isEmpty() || newKey.TagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO
		|| lastKey.TagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		auto strError = QObject::tr("Functon SetTabName tagname length is invalid!!!");
		s_pGlobleCore->LogMsg(SCADASTUDIO_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	//有的模块BigTagName为空
	bFlag = newKey.BigTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO
		|| lastKey.BigTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		auto strError = QObject::tr("Functon SetTabName BigTagName length is invalid!!!");
		s_pGlobleCore->LogMsg(SCADASTUDIO_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	
	bFlag = m_mapWidget.contains(lastKey);
	if (bFlag)
	{
		auto ite = m_mapWidget.find(lastKey);
		auto pWidget = ite.value();

		m_mapWidget.erase(ite);

		auto bFlag = newKey.BigTagName.isEmpty() || newKey.BigTagName.size() >= MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}
		
		m_mapWidget.insert(newKey, pWidget);
		if (m_mapTabKey.contains(pWidget))
		{
			m_mapTabKey[pWidget] = newKey;
		}
		else
		{
			auto strError = tr("can not find widget!!!");
			s_pGlobleCore->LogMsg(TABWIDGET_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

	}
	else
	{
		Q_ASSERT(false);
		QMessageBox box;
		box.setText(QObject::tr("key info is error!!!"));
		box.exec();

		return;
	}

	auto nIndex = this->indexOf(m_mapWidget[newKey]);
	if (nIndex == -1)
	{
		auto strError = tr("can not find widget!!!");
		s_pGlobleCore->LogMsg(TABWIDGET_DESC, strError.toStdString().c_str(), LEVEL_1);
	}
	
	setTabText(nIndex, newKey.TagName);
}

/*! \fn bool CTabWidget::RefeshWidget(int nType)
********************************************************************************************************* 
** \brief CTabWidget::RefeshWidget 
** \details 
** \param nType 窗口类型 
** \return bool 
** \author gw
** \date 2017年2月3日 
** \note 
********************************************************************************************************/
bool CTabWidget::RefeshWidget(int nType)
{
	for (int i = 0; i < count(); i++)
	{
		auto it = m_mapTabKey.find(widget(i));
		if (it != m_mapTabKey.end())
		{
			if (it->Type == nType)
			{
				IBase *pBase = dynamic_cast<IBase *>(widget(i));
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return false;
				}

				pBase->Refresh();
			}
		}
	}
	
	return true;
}

/*! \fn void CTabWidget::AddTab(QWidget *widget, const QString &strTitle, const int &nType, const QString &strTagName, QIcon &icon, const QString &strBigTagName)
********************************************************************************************************* 
** \brief CTabWidget::AddTab 
** \details 
** \param widget 
** \param strTitle 
** \param nType 
** \param strTagName 小项TagName 例如通道
** \param icon 
** \param strBigTagName 大项 例如前置
** \return void 
** \author gw
** \date 2017年2月8日 
** \note 
********************************************************************************************************/
//void CTabWidget::AddTab(QWidget *widget, const QString &strTitle, TabKeyStruct &keyInfo/*, const int &nType, const QString &strTagName, const QString &strBigTagName*/, QIcon &icon)
//{
//	m_mapWidget.insert(keyInfo, widget);
//	m_mapTabKey.insert(widget, keyInfo);
//
//	addTab(widget, icon, strTitle);
//
//	setCurrentWidget(widget);
//
//	QString strType = QString("%1").arg(keyInfo.Type);
//
//	widget->setObjectName(strType);
//}

bool CTabWidget::ModifyBigTagName(const QString &strBigTagName, int nType, const QString &strLastBigTagName)
{
	for (auto key : m_mapWidget.keys())
	{
		if (key.BigTagName == strLastBigTagName && key.Type == nType)
		{
			auto pWidget = m_mapWidget[key];

			if (m_mapTabKey.contains(pWidget))
			{
				auto it = m_mapWidget.find(key);
				if (it != m_mapWidget.end())
				{
					
					key.BigTagName = strBigTagName;
					std::swap(m_mapWidget[key], it.value());
					m_mapWidget.erase(it);
				}

				m_mapTabKey[pWidget].BigTagName = strBigTagName;
			}
			else
			{
				return false;
			}

			
		}
	}

	return true;
}

/*! \fn bool CTabWidget::ModifyFesBigTagName(const QString &strBigTagName, const QString &strLastBigTagName)
********************************************************************************************************* 
** \brief CTabWidget::ModifyFesBigTagName 
** \details tabwdiget 前置tagname是主键之一，前置修改要在m_mapWidget里也修改
** \param strBigTagName 
** \param strLastBigTagName 
** \return bool 
** \author gw
** \date 2017年4月24日 
** \note 
********************************************************************************************************/
bool CTabWidget::ModifyFesBigTagName(const QString &strBigTagName, const QString &strLastBigTagName)
{
	for (auto key : m_mapWidget.keys())
	{//FES_ANALOG_ALARM_TYPE, FES_ANALOG_ALARM_LIMIT_TYPE, FES_DIGITAL_ALARM_TYPE, FES_DIGITAL_ALARM_LIMIT_TYPE
		if (key.BigTagName == strLastBigTagName && (key.Type == FES_WIDGET_TYPE || key.Type == CHANNEL_WIDGET_TYPE
			|| key.Type == DEVICE_WIDGET_TYPE || key.Type == DEVICE_AI_WIDGET_TYPE || key.Type == DEVICE_DI_WIDGET_TYPE
			|| key.Type == DEVICE_AO_WIDGET_TYPE || key.Type == DEVICE_DO_WIDGET_TYPE || key.Type == SCALE_WIDGET_TYPE
			|| key.Type == FES_ANALOG_ALARM_TYPE || key.Type == FES_ANALOG_ALARM_LIMIT_TYPE || key.Type == FES_DIGITAL_ALARM_TYPE
			|| key.Type == FES_DIGITAL_ALARM_LIMIT_TYPE
			|| key.Type == VARIABLE_SYSTEM_WIDGET_TYPE || key.Type == VARIABLE_USER_GROUP_WIDGET_TYPE
			|| key.Type == FORWARDING_CHANNEL_WIDGET_TYPE
			|| key.Type == FORWARDING_CHANNEL_DI_WIDGET_TYPE || key.Type == FORWARDING_CHANNEL_AO_WIDGET_TYPE 
			|| key.Type == FORWARDING_CHANNEL_AI_WIDGET_TYPE || key.Type == FORWARDING_CHANNEL_CG_WIDGET_TYPE
			|| key.Type == FORWARDING_CHANNEL_DO_WIDGET_TYPE || key.Type == FORWARDING_CHANNEL_KWH_WIDGET_TYPE))
		{
			auto pWidget = m_mapWidget[key];

			if (m_mapTabKey.contains(pWidget))
			{
				auto it = m_mapWidget.find(key);
				if (it != m_mapWidget.end())
				{

					key.BigTagName = strBigTagName;
					std::swap(m_mapWidget[key], it.value());
					m_mapWidget.erase(it);
				}

				m_mapTabKey[pWidget].BigTagName = strBigTagName;
			}
			else
			{
				return false;
			}


		}
	}

	return true;
}

bool CTabWidget::ModifyScadaVaraibleBigTagName(const QString &strBigTagName, const QString &strLastBigTagName)
{
	for (auto key : m_mapWidget.keys())
	{//FES_ANALOG_ALARM_TYPE, FES_ANALOG_ALARM_LIMIT_TYPE, FES_DIGITAL_ALARM_TYPE, FES_DIGITAL_ALARM_LIMIT_TYPE
		if (key.BigTagName == strLastBigTagName && (key.Type == SCADA_WIDGET_TYPE ||
			key.Type == SCADA_VARIABLE_SYSTEM_TYPE || key.Type == SCADA_VARIABLE_USER_GROUP_TYPE ||
			key.Type == SCADA_ANALOG_ALARM_TYPE || key.Type == SCADA_ANALOG_ALARM_LIMIT_TYPE || key.Type == SCADA_DIGITAL_ALARM_TYPE || key.Type == SCADA_DIGITAL_ALARM_LIMIT_TYPE ||
			key.Type == SCADA_SCALE_LINEAR_TYPE || key.Type == SCADA_SCALE_NONLINEAR_TYPE))
		{
			auto pWidget = m_mapWidget[key];

			if (m_mapTabKey.contains(pWidget))
			{
				auto it = m_mapWidget.find(key);
				if (it != m_mapWidget.end())
				{

					key.BigTagName = strBigTagName;
					std::swap(m_mapWidget[key], it.value());
					m_mapWidget.erase(it);
				}

				m_mapTabKey[pWidget].BigTagName = strBigTagName;
			}
			else
			{
				return false;
			}


		}
	}

	return true;
}
