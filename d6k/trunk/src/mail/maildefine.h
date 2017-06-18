
#ifndef _MAIL_DEFINE_H
#define _MAIL_DEFINE_H

#include "datatypes.h"
#include "mail/mail.h"

enum
{
	MAIL_MAX_COUNT = 100,	//每个邮箱最大邮件数
	MAIL_MAGIC_CODE = 0X494C,	//校验码
};

#pragma pack (1)
struct LETTER
{
	int16u	 MagicHead; 	 //! 邮件头校验
	int32u   Id;             //! 邮件标识
	DMSG     Body;
	int16u	 MagicTail;	     //! 邮件尾校验		
};
#pragma pack ()

#endif // _MAIL_DEFINE_H


