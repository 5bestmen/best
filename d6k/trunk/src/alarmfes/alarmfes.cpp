#include "alarmfes.h"
#include "alarmwindow.h"
#include <QMessageBox>

static std::shared_ptr<CAlarmWindow> m_pMainWindow = nullptr;

/*! \fn int StartAlarmWin()
********************************************************************************************************* 
** \brief StartAlarmWin 
** \details 启动告警模块
** \return int 
** \author xingzhibing
** \date 2016年10月30日 
** \note 
********************************************************************************************************/
int StartAlarmWin(const QString& pszDataPath)
{
	Q_ASSERT(pszDataPath.size());
	if (pszDataPath .isEmpty())
	{
		QMessageBox::information(0,"LostFile","lost proj file");
		return -1;
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

/*! \fn void StopAlarmWin()
********************************************************************************************************* 
** \brief StopAlarmWin 
** \details 停止告警模块
** \return void 
** \author xingzhibing
** \date 2016年10月30日 
** \note 
********************************************************************************************************/
void StopAlarmWin()
{
	m_pMainWindow.reset();
}

/*! \fn QWidget*  GetAlarmMainWindow()
********************************************************************************************************* 
** \brief GetAlarmMainWindow 
** \details 导出告警模块窗口
** \return QWidget* 
** \author xingzhibing
** \date 2016年10月30日 
** \note 
********************************************************************************************************/
QWidget*  GetAlarmMainWindow(const QString& pszDataPath)
{
	Q_ASSERT(!pszDataPath.isEmpty() && pszDataPath.size()>0);
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
}