
#ifndef OBJECT_DEFINE_H
#define OBJECT_DEFINE_H

#define OBJECT_ROOT_ROLE         Qt::UserRole  // �������ɫ
#define OBJECT_GROUP_ROLE        Qt::UserRole + 10  // �������ɫ
#define OBJECT_ITEM_ROLE         Qt::UserRole + 20  // ����item��ɫ

#define QUOTE_ITEM_ROLE          Qt::UserRole + 30  // ���οؼ���ɫ

// ·��role
#define OBJECT_PATH_ROLE          Qt::UserRole + 666 + 1

// ǰһ��������role
#define OBJECT_OLD_NAME_ROLE      Qt::UserRole + 888 + 1

#define OBJECT_ROOT_TYPE         10000      // ���������
#define OBJECT_GROUP_TYPE        20000      // ����������
#define OBJECT_ITEM_TYPE         30000      // ����item����

#define MAX_OBJECT_GROUP_COUNT   3          // �������Ŀ

#define ADD_OBJECT_GROUP            QStringLiteral("�½�������")
#define DELETE_OBJECT_GROUP         QStringLiteral("ɾ��������")
#define ADD_OBJECT_ITEM             QStringLiteral("�½�����ģ��")
#define DELETE_OBJECT_ITEM          QStringLiteral("ɾ������ģ��")
#define RENAME_OBJECT_TREE_ITEM     QStringLiteral("������")

#define CREATE_OBJECT_GROUP_NAME        QStringLiteral("�½���")


#define READ_ATTRIBUTE       QStringLiteral("R")
#define WRITE_ATTRIBUTE      QStringLiteral("W")

#define HEAD_NAME_ID                    QStringLiteral("ID")
#define HEAD_NAME_NAME                  QStringLiteral("����")
#define HEAD_NAME_DESCIBE               QStringLiteral("����")
#define HEAD_NAME_TYPE                  QStringLiteral("����")
#define HEAD_NAME_RW_ATTR               QStringLiteral("��д����")
#define HEAD_NAME_RELATION_POINT        QStringLiteral("�������");

#define TARGETNAME_CHOOSE               QStringLiteral("���ѡ��")

#define NAME_LEFT_BRACKETS              QStringLiteral("(")
#define NAME_RIGHT_BRACKETS             QStringLiteral(")")

#define OBJECT_NODE_NAME                QStringLiteral("object")
#define OBJECT_GROUP_NODE_NAME          QStringLiteral("group")
#define OBJECT_ITEM_NODE_NAME           QStringLiteral("item")

#define OBJECT_ROOT_NAME                QStringLiteral("������̬")

#define PROJECT_ROOT_NODE_NAME          QStringLiteral("Project")
// ����Ŀ¼��
#define OBJECT_SAVE_FOLDER_NAME         QStringLiteral("object")

#define OBJECT_FILE_TYPE                 ".xml"

#endif // OBJECT_DEFINE_H