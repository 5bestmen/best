#include "tabwidgetredefine.h"

#include "scadastudio/base.h"
#include "scadastudio/icore.h"
#include "powerpredictdefine.h"
#include "scadastudio/define.h"
#include <QMessageBox>

extern IMainModuleInterface *s_pGlobleCore;


CTabWidget::CTabWidget(QWidget *parent)
	: QTabWidget(parent), m_pBeforeCloseTabData(nullptr)
{
	this->setTabsClosable(true);

	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTileWidget(int)));
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

	m_mapTabKey.clear();
	m_mapWidget.clear();
}


CTabWidget::~CTabWidget()
{

}


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


void CTabWidget::tabChanged(int index)
{
	Q_UNUSED(index);
}


bool CTabWidget::IsWidgetInTab(const QString &strTagName, const int &nType, const QString &strBigTagName)
{
	auto bFlag = strTagName.isEmpty() || strTagName.size() > MAX_TAGNAME_LEN_PREDICTION;
	/*|| strBigTagName.isEmpty() || strBigTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO*/;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		QMessageBox box;
		box.setText(QObject::tr("tagname is invalid!!!"));
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
		box.setText(QObject::tr("tagname is invalid!!!"));
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
	auto bFlag = m_mapWidget.contains(lastKey);
	if (bFlag)
	{
		auto ite = m_mapWidget.find(lastKey);
		auto pWidget = ite.value();

		m_mapWidget.erase(ite);

		auto bFlag = newKey.BigTagName.isEmpty() || newKey.BigTagName.size() >= MAX_TAGNAME_LEN_PREDICTION;
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