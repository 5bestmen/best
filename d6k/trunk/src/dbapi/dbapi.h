#pragma once

#include "dbapi_global.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtCore/QVariantList>
#include <QMutex>

#include "var_info.h"

class DBAPI_EXPORT dbapi
{
	enum { ORACLE, MYSQL };

public:
	dbapi(QString strConnName);
	~dbapi();

	bool open(const QString& strDns = SERVICENAME, const QString& strUserName = UID,
		const QString& strPassword = PWD, int nType = ORACLE, int timeout = 20);

	QString lastError();

	bool exec(const QString &strSql);

	QSqlQuery& getSqlQuery();

	int getFieldCounts();

	int getFieldIndex(QString &strFieldName);

	bool next();

	QVariant getValue(int nPos);

	QVariant getValue(QString strFieldName);

	bool transaction();

	bool commit();

	bool rollback();

	bool prepare(QString &strSql);
	void bindValue(QString strField, QVariant varValue);

	void bindValue(int &nPos, QVariant varValue);

	bool exec();

	void addBindValue(QVariant varValue);

	void addBatchBindValue(QVariantList &varList);

	bool execBatch();

	bool getFieldType(QString strTable, QString strFieldAlasName, QString strFieldName, CVarinfo &info);

	QString trimmed(QString &strData) const;

	QString getFieldName(int nPos);

	bool getVec(QString strSql, TwoVec &vec);

	void close();

	bool getFieldsTypeFromTable(QString strTable, std::vector<CVarinfo> &info);

public:
	QString m_strConnName;

	QSqlDatabase m_dbQt;

	QSqlQuery m_query;

	QString m_strError;

	static QMutex m_mutex;
};


