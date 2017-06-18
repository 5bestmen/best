
#ifndef OBJECT_DEFINE_H
#define OBJECT_DEFINE_H

#define OBJECT_ROOT_ROLE         Qt::UserRole  // 对象根角色
#define OBJECT_GROUP_ROLE        Qt::UserRole + 10  // 对象组角色
#define OBJECT_ITEM_ROLE         Qt::UserRole + 20  // 对象item角色

#define QUOTE_ITEM_ROLE          Qt::UserRole + 30  // 树形控件角色

// 路径role
#define OBJECT_PATH_ROLE          Qt::UserRole + 666 + 1

// 前一个旧名字role
#define OBJECT_OLD_NAME_ROLE      Qt::UserRole + 888 + 1

#define OBJECT_ROOT_TYPE         10000      // 对象根类型
#define OBJECT_GROUP_TYPE        20000      // 对象组类型
#define OBJECT_ITEM_TYPE         30000      // 对象item类型

#define MAX_OBJECT_GROUP_COUNT   3          // 组最大数目

#define ADD_OBJECT_GROUP            QStringLiteral("新建对象组")
#define DELETE_OBJECT_GROUP         QStringLiteral("删除对象组")
#define ADD_OBJECT_ITEM             QStringLiteral("新建对象模型")
#define DELETE_OBJECT_ITEM          QStringLiteral("删除对象模型")
#define RENAME_OBJECT_TREE_ITEM     QStringLiteral("重命名")

#define CREATE_OBJECT_GROUP_NAME        QStringLiteral("新建组")


#define READ_ATTRIBUTE       QStringLiteral("R")
#define WRITE_ATTRIBUTE      QStringLiteral("W")

#define HEAD_NAME_ID                    QStringLiteral("ID")
#define HEAD_NAME_NAME                  QStringLiteral("名称")
#define HEAD_NAME_DESCIBE               QStringLiteral("描述")
#define HEAD_NAME_TYPE                  QStringLiteral("类型")
#define HEAD_NAME_RW_ATTR               QStringLiteral("读写属性")
#define HEAD_NAME_RELATION_POINT        QStringLiteral("关联测点");

#define TARGETNAME_CHOOSE               QStringLiteral("点击选择")

#define NAME_LEFT_BRACKETS              QStringLiteral("(")
#define NAME_RIGHT_BRACKETS             QStringLiteral(")")

#define OBJECT_NODE_NAME                QStringLiteral("object")
#define OBJECT_GROUP_NODE_NAME          QStringLiteral("group")
#define OBJECT_ITEM_NODE_NAME           QStringLiteral("item")

#define OBJECT_ROOT_NAME                QStringLiteral("对象组态")

#define PROJECT_ROOT_NODE_NAME          QStringLiteral("Project")
// 保存目录名
#define OBJECT_SAVE_FOLDER_NAME         QStringLiteral("object")

#define OBJECT_FILE_TYPE                 ".xml"

#endif // OBJECT_DEFINE_H