
#ifndef _MODEL_DEFINE_H_
#define _MODEL_DEFINE_H_

// ������νṹ������ʼ����
#define M_SCENE_NAME	    QStringLiteral("�����볡��")
// ������νṹ����ʼ����
#define M_GROUP_NAME	    QStringLiteral("��������")
// ������νṹ�ͺ���ʼ����
#define M_MODEL_NO_NAME	    QStringLiteral("�������ͺ�")

// ����role
#define ROLE_ROOT            Qt::UserRole       // root��ɫ
#define ROLE_SCENE           Qt::UserRole + 1   // ������ɫ
#define ROLE_EQUIPMENT       Qt::UserRole + 2   // һ���豸��ɫ
#define ROLE_MODEL_NO        Qt::UserRole + 3   // �ͺŽ�ɫ

#define ROLE_MARK_FOLDER     Qt::UserRole + 2
#define ROLE_MARK_ITEM       Qt::UserRole + 3

#define TYPE_ROOT            1 // root
#define TYPE_SCENE           2 // ��������
#define TYPE_EQUIPMENT       3 // һ���豸����
#define TYPE_MODEL_NO        4 // �豸�ͺ�

#define MARK_FOLDER         2 // ����Ŀ¼���
#define MARK_ITEM           3 // ��Ŀ���

// ��ӦROLE_MARK_ITEM���item����
#define MARK_ITEM_SCENE      1 // ������Ŀ
#define MARK_ITEM_FOLDER     2 // Ŀ¼��Ŀ
#define MARK_ITEM_FILE       3 // �ļ���Ŀ

#define M_MAX_CONTENT_LEN   256 // ������󳤶�

// ģ�͹����ļ����������ֶ����õ���Ϣ
#define M_MODEL_CONFIG_NAME_XML     "modeltool.xml"
#define M_SAVE_MODEL_FOLDER_NAME    QStringLiteral("model")
#define M_FILE_TYPE                 ".xml"

#define M_BIT                       QStringLiteral("bit");
#define M_SIGNED_CHAR               QStringLiteral("signed char");
#define M_UNSIGNED_CHAR             QStringLiteral("unsigned char");
#define M_SIGNED_WORD               QStringLiteral("signed word");
#define M_UNSIGNED_WORD             QStringLiteral("unsigned word");
#define M_SIGNED_DOUBLE_WORD        QStringLiteral("signed double word");
#define M_UNSIGNED_DOUBLE_WORD      QStringLiteral("unsigned double word");
#define M_FLOAT                     QStringLiteral("float");
#define M_DOUBLE                    QStringLiteral("double");
#define M_STRING                    QStringLiteral("string");
#define M_ARRIBUTE                  QStringLiteral("arr");

// ��������
enum
{
    DATATYPE_ANALOG,
    DATATYPE_BINARY,
};

#include <QString>

// ��������combo����
typedef struct
{
    QString ID;
    QString desc;
} IDDESC;



#endif // _MODEL_DEFINE_H_
