#include "alarmfes.h"
#include "alarmwindow.h"
#include <QMessageBox>

static std::shared_ptr<CAlarmWindow> m_pMainWindow = nullptr;

/*! \fn int StartAlarmWin()
********************************************************************************************************* 
** \brief StartAlarmWin 
** \details �����澯ģ��
** \return int 
** \author xingzhibing
** \date 2016��10��30�� 
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
** \details ֹͣ�澯ģ��
** \return void 
** \author xingzhibing
** \date 2016��10��30�� 
** \note 
********************************************************************************************************/
void StopAlarmWin()
{
	m_pMainWindow.reset();
}

/*! \fn QWidget*  GetAlarmMainWindow()
********************************************************************************************************* 
** \brief GetAlarmMainWindow 
** \details �����澯ģ�鴰��
** \return QWidget* 
** \author xingzhibing
** \date 2016��10��30�� 
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