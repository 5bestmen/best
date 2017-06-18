
#ifndef _MAIL_DEFINE_H
#define _MAIL_DEFINE_H

#include "datatypes.h"
#include "mail/mail.h"

enum
{
	MAIL_MAX_COUNT = 100,	//ÿ����������ʼ���
	MAIL_MAGIC_CODE = 0X494C,	//У����
};

#pragma pack (1)
struct LETTER
{
	int16u	 MagicHead; 	 //! �ʼ�ͷУ��
	int32u   Id;             //! �ʼ���ʶ
	DMSG     Body;
	int16u	 MagicTail;	     //! �ʼ�βУ��		
};
#pragma pack ()

#endif // _MAIL_DEFINE_H


