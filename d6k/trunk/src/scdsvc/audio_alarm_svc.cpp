/*! @file audio_alarm_svc.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  audio_alarm_svc.cpp
�ļ�ʵ�ֹ��� :  �����澯ģ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����澯ģ��
*  @author  LiJin
*  @version 1.0
*  @date    2016.12.01
*******************************************************************************/
#include "audio_alarm_svc.h"
#include "log/log.h"
#include "mail/mail.h"
#include <QObject> 
#include <QString> 
#include <QApplication>
#include <QLoggingCategory>

CAudioAlarmSvc::CAudioAlarmSvc(CScadaSvc* pServer, const std::string & szMailBoxName, int &nMailBoxID) : CBaseModule(pServer, szMailBoxName, nMailBoxID)
{
	m_bStopFlag = false;
	m_bNext = false;
	m_pMediaPlayer = std::make_shared<QMediaPlayer>(this);
	QLoggingCategory::setFilterRules(QStringLiteral("qt.speech.tts=true \n qt.speech.tts.*=true"));
	m_pTtsPlayer = std::make_shared<QTextToSpeech>("sapi",this);
	QObject::connect(m_pTtsPlayer.get(), &QTextToSpeech::stateChanged, this, &CAudioAlarmSvc::slot_OnStateChanged,Qt::QueuedConnection);	
	QObject::connect(this, &CAudioAlarmSvc::sig_OneTaskProcessed, this,&CAudioAlarmSvc::slot_OnDealTaskResult);
}

CAudioAlarmSvc::~CAudioAlarmSvc(void)
{ 

}

/*! \fn bool CAudioAlarmSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CAudioAlarmSvc::Initialize 
** \details ��ʼ��  ����proj�ļ����ֱ���߸��������ļ�
** \return bool 
** \author LiJin 
** \date 2016��9��1�� 
** \note  
********************************************************************************************************/
bool CAudioAlarmSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{ 
	QString szLog;
	if (pszDataPath && strlen((pszDataPath)))
	{
		szLog = QString(QObject::tr("Start project [%1]...")).arg(pszDataPath);
	}
	else
	{
		szLog = QObject::tr("Start project...");
	}
	
	LogMsg(szLog.toStdString().c_str(), 0);


	m_voices = m_pTtsPlayer->availableVoices();
	m_pTtsPlayer->setVoice(m_pTtsPlayer->voice());

	m_pTtsPlayer->setVolume(100);
	m_pTtsPlayer->setPitch(10);
	m_pTtsPlayer->setRate(0);
	QVector<QLocale> locales = m_pTtsPlayer->availableLocales();
	QLocale nLoval = m_pTtsPlayer->locale();
	m_pTtsPlayer->setLocale(nLoval);

	return true;
}

void CAudioAlarmSvc::Run()
{
	m_bContinue = true;
	m_bNext = true;
	StartModule();
}

void CAudioAlarmSvc::Shutdown()
{
	emit sig_OneTaskProcessed(-1);

	m_bContinue = false;
	m_bNext = true;

	QEventLoop loop;
	QObject::connect(this,&CAudioAlarmSvc::sig_ProcessOver,&loop,&QEventLoop::quit);
	loop.exec();

	StopModule();
}

/*! \fn void CAudioAlarmSvc::AddAudioAlarmInfo(CAudioBaseData_DEF& pAudioData)
********************************************************************************************************* 
** \brief CAudioAlarmSvc::AddAudioAlarmInfo 
** \details ����Ϣ��������Ӹ澯��Ϣ
** \param pAudioData 
** \return void 
** \author xingzhibing
** \date 2017��3��22�� 
** \note 
********************************************************************************************************/
void CAudioAlarmSvc::AddAudioAlarmInfo(CAudioBaseData_DEF& pAudioData)
{
	Q_ASSERT(pAudioData);
	if (!pAudioData)
	{
		return;
	}
	if (!m_bContinue)
	{
		m_arrAudioDataUint.push_back(pAudioData);
	}	
}

void CAudioAlarmSvc::MainLoop()
{
	bool bRet = false;
	while (m_arrAudioDataUint.size())
	{
		if (m_bContinue)
		{
			CAudioBaseData_DEF pData = m_arrAudioDataUint.front();
			if (pData)
			{
				//������Ӧ����������
				bRet = PlayAudioSound(pData);
				if (!bRet)
				{
					LogMsg(QObject::tr("Error alarm audio sound!").toStdString().c_str(), 0);
					m_arrAudioDataUint.pop_front();
				}
				else
				{

				}
				switch (pData->m_nAckType)
				{
				case AFTER_CONFIRM_DELETE:
				{
					if (pData->m_bAck)
					{
						AppendAlarmUnit(pData);
					}
					m_arrAudioDataUint.pop_front();
					break;
				}
				case AFTER_PLAY_DELETE:
				{
					m_arrAudioDataUint.pop_front();
					break;
				}
				default:
					break;
				}
			}
		}	
		m_bContinue = false;
		emit sig_OneTaskProcessed(1);
	}
	emit sig_OneTaskProcessed(1);
}

/*! \fn void CAudioAlarmSvc::LogMsg(char* pszText, int nLevel)
********************************************************************************************************* 
** \brief CAudioAlarmSvc::LogMsg 
** \details logmsg
** \param pszText 
** \param nLevel 
** \return void 
** \author xingzhibing
** \date 2017��2��14�� 
** \note 
********************************************************************************************************/
void CAudioAlarmSvc::LogMsg(const char* pszText, int nLevel)
{
	::LogMsg("CAudioAlarmSvc",pszText,nLevel,nullptr);
}

/*! \fn bool CAudioAlarmSvc::PlayAudioSound(CAudioBaseData_DEF& pAudioData)
********************************************************************************************************* 
** \brief CAudioAlarmSvc::PlayAudioSound 
** \details ����������Ϣ
** \param pAudioData 
** \return bool 
** \author xingzhibing
** \date 2017��2��14�� 
** \note 
********************************************************************************************************/
bool CAudioAlarmSvc::PlayAudioSound(CAudioBaseData_DEF& pAudioData)
{
	Q_ASSERT(pAudioData);
	if (!pAudioData)
	{
		return false;
	}
	if (pAudioData->m_bSoundEnable)
	{
		//Q_ASSERT(pAudioData->m_bSoundEnable  && strlen(pAudioData->m_szSoundFile) >0 );
		if (!pAudioData->m_bSoundEnable || strlen(pAudioData->m_szSoundFile) == 0)
		{
			LogMsg(QObject::tr("%1 sound unenable or sound file empty").arg(QString::fromStdString(pAudioData->m_szTagName)).toStdString().c_str(),0);
		}
		else
		{
			m_pMediaPlayer->setMedia(QUrl::fromLocalFile(QString::fromLocal8Bit(pAudioData->m_szSoundFile)));
			m_pMediaPlayer->setVolume(100);
			m_pMediaPlayer->play();
		}
	}
	if (pAudioData->m_bSpeechTextEnable)
	{
		Q_ASSERT(pAudioData->m_bSpeechTextEnable  &&  strlen ( pAudioData->m_szSpeechText) >0  );
		if (!pAudioData->m_bSpeechTextEnable  || strlen(pAudioData->m_szSpeechText) ==0)
		{
			LogMsg(QObject::tr("%1 text to speech  unenable or text file empty").arg(QString::fromStdString(pAudioData->m_szTagName)).toStdString().c_str(), 0);
		}
		else
		{
			QEventLoop loop;
			connect(this, &CAudioAlarmSvc::sig_TTSAudioStoped, &loop,&QEventLoop::quit);
			m_pTtsPlayer->say(QString::fromLocal8Bit(pAudioData->m_szSpeechText));
			loop.exec();
		}
	}
	if (pAudioData->m_bBeepEnable)
	{
#ifdef WIN32
		//QApplication::beep();
#else
		beep();
#endif // WIN32	
	}
	return true;
}

/*! \fn void CAudioAlarmSvc::AppendAlarmUnit(CAudioBaseData_DEF& pAudioData)
********************************************************************************************************* 
** \brief CAudioAlarmSvc::AppendAlarmUnit 
** \details ���Ӹ澯���ݵ�Ԫ
** \param pAudioData 
** \return void 
** \author xingzhibing
** \date 2017��2��14�� 
** \note 
********************************************************************************************************/
void CAudioAlarmSvc::AppendAlarmUnit(CAudioBaseData_DEF& pAudioData)
{
	Q_ASSERT(pAudioData);
	if (!pAudioData)
	{
		//TODO log ����һ�Ƿ�����������
		return;
	}
	m_arrAudioDataUint.push_back(pAudioData);
}

/*! \fn void CAudioAlarmSvc::slot_OnStateChanged(QTextToSpeech::State state)
********************************************************************************************************* 
** \brief CAudioAlarmSvc::slot_OnStateChanged 
** \details TTS ����״̬�ı�
** \param state 
** \return void 
** \author xingzhibing
** \date 2017��2��15�� 
** \note 
********************************************************************************************************/
void CAudioAlarmSvc::slot_OnStateChanged(QTextToSpeech::State state)
{
#include <QDebug>
	static int nnn = 0;
	qDebug() << state << nnn++;
	if (state==QTextToSpeech::Ready)
	{
		emit sig_TTSAudioStoped();
	}
}

void CAudioAlarmSvc::slot_OnDealTaskResult(int nType)
{
	if (nType==1)
	{
		if (m_bNext)
		{
			emit sig_ProcessOver();
			m_bContinue = false;
			m_bNext = false;
			m_arrAudioDataUint.clear();
		}
		else
		{
			m_bContinue = true;
		}		
	}
	if (nType==-1)
	{
		m_bContinue = false;
		m_bNext = true;
	}
}

/** @}*/
