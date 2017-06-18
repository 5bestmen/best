
#ifndef _MODEL_DEFINE_H_
#define _MODEL_DEFINE_H_

// 左边树形结构场景起始名称
#define M_SCENE_NAME	    QStringLiteral("请输入场景")
// 左边树形结构组起始名称
#define M_GROUP_NAME	    QStringLiteral("请输入组")
// 左边树形结构型号起始名称
#define M_MODEL_NO_NAME	    QStringLiteral("请输入型号")

// 场景role
#define ROLE_ROOT            Qt::UserRole       // root角色
#define ROLE_SCENE           Qt::UserRole + 1   // 场景角色
#define ROLE_EQUIPMENT       Qt::UserRole + 2   // 一次设备角色
#define ROLE_MODEL_NO        Qt::UserRole + 3   // 型号角色

#define ROLE_MARK_FOLDER     Qt::UserRole + 2
#define ROLE_MARK_ITEM       Qt::UserRole + 3

#define TYPE_ROOT            1 // root
#define TYPE_SCENE           2 // 场景类型
#define TYPE_EQUIPMENT       3 // 一次设备类型
#define TYPE_MODEL_NO        4 // 设备型号

#define MARK_FOLDER         2 // 这是目录标记
#define MARK_ITEM           3 // 条目标记

// 对应ROLE_MARK_ITEM标记item种类
#define MARK_ITEM_SCENE      1 // 场景条目
#define MARK_ITEM_FOLDER     2 // 目录条目
#define MARK_ITEM_FILE       3 // 文件条目

#define M_MAX_CONTENT_LEN   256 // 内容最大长度

// 模型工具文件名，保存手动配置的信息
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

// 数据类型
enum
{
    DATATYPE_ANALOG,
    DATATYPE_BINARY,
};

#include <QString>

// 数据类型combo内容
typedef struct
{
    QString ID;
    QString desc;
} IDDESC;



#endif // _MODEL_DEFINE_H_
