/*! @file reportdefine.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  reportdefine.h
�ļ�ʵ�ֹ��� :  ����һЩ�������Զ���
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ����һЩ�������Զ���
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef REPORT_DEFINE_H
#define REPORT_DEFINE_H




#define REPORT_ADD_GROUP      QStringLiteral("�½�����")
#define REPORT_DELETE_GROUP   QStringLiteral("ɾ������")
#define REPORT_NEW_CREATE   QStringLiteral("�½�����")
#define IMPORT_REPORT_ITEM  QStringLiteral("���뱨��")

#define DELETE_REPORT_ITEM  QStringLiteral("ɾ������")
#define MODITY_NAME_ITEM    QStringLiteral("�޸�����")
#define MODITY_TYPE_ITEM    QStringLiteral("�޸�����")


//����
#define REPORT_ROOT 23
//������
#define REPORT_GROUP_ITEM 231
//����Ԫ
#define REPORT_ITEM 232
//�����������Ŀ
#define MAX_REPORT_GROUP_COUNT 3

//�ձ���
#define REPORT_DAY_ITEM 235               //��Ϊ��ڵ�
#define REPORT_CHILD_DAY_GROUPITEM 2351   //��Ϊ�ӽڵ�
#define REPORT_CHILD_DAY_ITEM 2352
//�±���
#define REPORT_MONTH_ITEM 236
#define REPORT_CHILD_MONTH_GROUPITEM 2361
#define REPORT_CHILD_MONTH_ITEM 2362
//�걨��
#define REPORT_YEAR_ITEM 237
#define REPORT_CHILD_YEAR__GROUPITEM 2371
#define REPORT_CHILD_YEAR__ITEM 2372
//�Զ��屨��
#define REPORT_CUSTOM_ITEM 238
#define REPORT_CHILD_CUSTOM_GROUPITEM 2381
#define REPORT_CHILD_CUSTOM_ITEM 2382

#define REPORT_CHILD_TIME_ITEM 239

#define  REPORT_USER_DATA_ROLE  (Qt::UserRole + 100 )  

#define REPORT_DES (char *)tr("report").toStdString().c_str()

enum REPROTTYPE
{
	DAY_REPORT = 0,            //��<�ձ���
	WEEK_REPORT = 1,           //��<�ܱ���
	TEN_DAYS_REPORT = 2,       //��<Ѯ����
	MONTH_REPORT = 3,          //��<�±���
	QUARTER_REPORT = 4,        //��<���ȱ���
	YEAR_REPORT = 5,           //��<�걨��
	SPECIAL_REPORT = 6,        //��<���ⱨ��
	HOUR_STATICS_REPORT = 7,   //��<ʱ��ͳ�Ʊ���	
	UNKNOWN_REPORT = 100,      //! <δ֪���ͱ���
};

#endif