#include "alarmlite.h"
#include "alarmwindow.h"
#include <QWidget>
#include <QMessageBox>
#include <QTranslator>
static std::shared_ptr<CAlarmWindow> m_pMainWindow = nullptr;

extern "C"
{
	/*! \fn SCD_ALARM_EXPORT int StartAlarmWin(const QString& pszDataPath)
	********************************************************************************************************* 
	** \brief StartAlarmWin 
	** \details 
	** \param pszDataPath 
	** \return SCD_ALARM_EXPORT int 
	** \author xingzhibing
	** \date 2017��3��16�� 
	** \note ����scada �澯ģ��
	********************************************************************************************************/
	SCD_ALARM_EXPORT int StartAlarmWin(const QString& pszDataPath)
	{
		Q_ASSERT(pszDataPath.size());
		if (pszDataPath.isEmpty())
		{
			QMessageBox::information(0, "LostFile", "lost proj file");
			return -1;
		}

		QTranslator *qtPlugini = new QTranslator();

		QString strRunPath = qApp->applicationDirPath();

		if (qtPlugini->load(strRunPath + "/locales/alarmwin_zh.qm"))
		{
			qApp->installTranslator(qtPlugini);
		}

		if (!m_pMainWindow)
		{
			m_pMainWindow = std::make_shared<CAlarmWindow>(nullptr, pszDataPath);
		}
		else
		{
			m_pMainWindow.reset();
			m_pMainWindow = std::make_shared<CAlarmWindow>(nullptr, pszDataPath);
		}
		m_pMainWindow->show();
		return 0;
	}

	/*! \fn QWidget* GetAlarmMainWindow(const QString& pszDataPath)
	********************************************************************************************************* 
	** \brief GetAlarmMainWindow 
	** \details 
	** \param pszDataPath 
	** \return SCD_ALARM_EXPORT QWidget* 
	** \author xingzhibing
	** \date 2017��3��16�� 
	** \note  �κ�ģ�鶼�ɻ�ȡ��ģ�飬Ƕ�뵽�κλ�����
	********************************************************************************************************/
	SCD_ALARM_EXPORT QWidget* GetAlarmMainWindow(const QString& pszDataPath)
	{
		Q_ASSERT(!pszDataPath.isEmpty() && pszDataPath.size() > 0);
		if (pszDataPath.isEmpty())
		{
			return nullptr;
		}
		if (m_pMainWindow)
		{
			return m_pMainWindow.get();
		}
		else
		{
			m_pMainWindow = std::make_shared<CAlarmWindow>(nullptr, pszDataPath);
			return m_pMainWindow.get();
		}
		return nullptr;
	}

	/*! \fn  void StopAlarmWin()
	********************************************************************************************************* 
	** \brief StopAlarmWin 
	** \details 
	** \return SCD_ALARM_EXPORT void 
	** \author xingzhibing
	** \date 2017��3��16�� 
	** \note   ֹͣʹ�ø澯ģ��
	********************************************************************************************************/
	SCD_ALARM_EXPORT void StopAlarmWin()
	{
		m_pMainWindow.reset();
	}
}
