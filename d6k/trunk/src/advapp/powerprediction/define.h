#ifndef _DEFINE_H_
#define _DEFINE_H_

#define  MAX_GROUP_NUM				  	 3                         //��վ�����3��
#define  MAX_ITEM_NUM					 3
#define  PREDICT_MAX_NAME_LENGTH				 512
#define  ROOT_GROUP						 0
#define  LEVEL_1						 1
#define  STRING_PAD						 sizeof(size_t)

#define  TREE_TOOL_TIP_ROLE				 Qt::ToolTipRole
//#define  PREDICT_ROOT_ROLE				 Qt::UserRole			   //����ɫ
//#define  PREDICT_GROUP_ROLE				 Qt::UserRole + 425 + 10   //���ɫ
//#define  PREDICT_ITEM_ROLE				 Qt::UserRole + 425 + 20   //item��ɫ
//#define  PREDICT_QUOTE_ITEM_ROLE		 Qt::UserRole + 425 + 30   //�ؼ���ɫ
//#define  PREDICT_DEVICE_ROLE			 Qt::UserRole + 425 + 60   //�豸��ɫ
//#define  PREDICT_OLD_NAME_ROLE			 Qt::UserRole + 425 + 40   //
//#define  PREDICT_PATH_ROLE				 Qt::UserRole + 425 + 50   //
//#define  LASTVALUE						 Qt::UserRole + 11111 + 425
//#define  POINTERDATA					 Qt::UserRole + 11111 + 426
//#define  GROUP_NUMBER_ROLE				 Qt::UserRole + 11111 + 427

//#define  POWER_PREDICT_ROOT_TYPE		 425                       //������
//#define  POWER_PREDICT_GROUP_TYPE	     426					   //������
//#define  POWER_PREDICT_DEVICE_TYPE     427					   //device����
//#define  POWER_PREDICT_ITEM_TYPE		 428					   //item����

#define  POWER_PREDICT_DEVICE_AI		 429
#define  POWER_PREDICT_DEVICE_DI		 430
#define  POWER_PREDICT_DEVICE_PATAM		 431

#define  POWERPREDICT_SAVE_FOLDER_NAME   QStringLiteral("powerpredict")
#define  POWERPREDICT_NODE_NAME          QStringLiteral("powerpredict")
#define  POWERPREDICT_GROUP_NODE_NAME    QStringLiteral("group")
#define  POWERPREDICT_ITEM_NODE_NAME     QStringLiteral("item")
#define  PROJECT_ROOT_NODE_NAME          QStringLiteral("Project")
#define  EXTEND_SERVICE_NAME             QStringLiteral("extend service")
#define  POWERPREDICT_FILE_NAME          QStringLiteral("file")


#endif  //_DEFINE_H_