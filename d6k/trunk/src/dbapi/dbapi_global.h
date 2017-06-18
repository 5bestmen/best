#pragma once

#include <QtCore/qglobal.h>

# if defined(DBAPI_LIB)
#  define DBAPI_EXPORT Q_DECL_EXPORT
# else
#  define DBAPI_EXPORT Q_DECL_IMPORT
# endif


#define SERVICENAME "D6000"
#define UID "DMS"
#define PWD "smd123!"

typedef QVector<QVector<QString> > TwoVec;

#define ALL_TAB_COLUMN_NAME "COLUMN_NAME"
#define ALL_TAB_DATA_TYPE "DATA_TYPE"
#define ALL_TAB_COLUMN_DATA_LENGTH "DATA_LENGTH"
#define ALL_TAB_COLUMN_DATA_PRECISION "DATA_PRECISION"
#define ALL_TAB_COLUMN_DATA_SCALE	"DATA_SCALE"

#define DB_TYPE_NUMBER "NUMBER"
#define DB_TYPE_FLOAT "FLOAT"

#define DB_TYPE_CHAR "char"

#define DB_TYPE_VARCHAR "varchar"

