/*! @file audio_alarm_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  audio_alarm_svc.h
�ļ�ʵ�ֹ��� :  �����澯ģ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ע�⣺ 
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
#ifndef AUDIO_ALARM_SVC_MODULE_H
#define AUDIO_ALARM_SVC_MODULE_H

#include "base_module.h"
#include "audiodataunit.h"
#include <memory>
#include <vector>
#include <QObject>
#include <QMediaPlayer>
#include <QTextToSpeech>
class CAudioAlarmSvc : public QObject, CBaseModule
{
	Q_OBJECT
public:
	CAudioAlarmSvc(CScadaSvc* pServer, const std::string &szMailBoxName, int &MailID);
	virtual ~CAudioAlarmSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();
	//�����ṩ��ӽӿ�
	void AddAudioAlarmInfo(CAudioBaseData_DEF& pAudioData);
protected: 
	void MainLoop();
	void LogMsg(const char* pszText,int nLevel);
	bool PlayAudioSound(CAudioBaseData_DEF& pAudioData);
public: 
	void AppendAlarmUnit(CAudioBaseData_DEF& pAudioData);
protected Q_SLOTS:
	void slot_OnStateChanged(QTextToSpeech::State state);
	void slot_OnDealTaskResult(int nType);
Q_SIGNALS:
	void sig_TTSAudioStoped();
	void sig_OneTaskProcessed(int nType);
	void sig_ProcessOver();
private:
	std::shared_ptr<QMediaPlayer> m_pMediaPlayer;
private:
	std::shared_ptr<QTextToSpeech> m_pTtsPlayer;
	QVector<QVoice> m_voices;
private: 
	//mutable std::mutex m_mutThread;
	volatile bool m_bNext;
	bool m_bContinue;
	CAudioBaseDataDeque m_arrAudioDataUint;

};

#endif // AUDIO_ALARM_SVC_MODULE_H


/** @}*/

