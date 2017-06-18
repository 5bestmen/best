#include "dbapplitonapi.h"
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QMutexLocker>

QMutex dbapplitonapi::m_mutex;

dbapplitonapi::dbapplitonapi(QString strConnName)
{
    QMutexLocker locker(&m_mutex);
	
	m_strConnName = strConnName;
}

dbapplitonapi::~dbapplitonapi()
{
    QMutexLocker locker(&m_mutex);
}
#include <QDebug>
bool dbapplitonapi::open(const QString &strDns, const QString &strUserName, const QString &strPassword, int nType, int timeout)
{
    QMutexLocker locker(&m_mutex);
	
	if(m_dbQt.contains(m_strConnName))
    {
		m_strError = "连接已存在!!!";
         qDebug()<<"连接已存在";
		return false;
    }
    else
    {
        //qDebug()<<"1112";
        m_dbQt= QSqlDatabase::addDatabase("QOCI", m_strConnName);

        //qDebug()<<"1113";
       m_dbQt.setDatabaseName(strDns);    //数据库名
       m_dbQt.setPort(1521);
       m_dbQt.setUserName(strUserName);        //数据库用户名
       m_dbQt.setPassword(strPassword);       //数据库密码

       if(!m_dbQt.open())
       {
           m_strError = m_dbQt.lastError().text();
           qDebug()<<m_dbQt.lastError().text();
           return false;
       }
       else
       {
           m_dbQt.transaction();

            m_query = QSqlQuery(m_dbQt);
       }
    }

    return true;
}

QString dbapplitonapi::lastError()
{
    
	QMutexLocker locker(&m_mutex);    
	
	return m_strError;
}

bool dbapplitonapi::exec(QString &strSql)
{ 	
	QMutexLocker locker(&m_mutex);	

	if(m_query.exec(strSql))
	{  
		return true;
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}
}

QSqlQuery& dbapplitonapi::getSqlQuery()
{
	QMutexLocker locker(&m_mutex);
	
	return m_query;
}

int dbapplitonapi::getFieldCounts()
{
    QMutexLocker locker(&m_mutex);    
	
	return m_query.record().count();
}

int dbapplitonapi::getFieldIndex(QString &strFieldName)
{
    QMutexLocker locker(&m_mutex);    
	
	return m_query.record().indexOf(strFieldName);
}

bool dbapplitonapi::next()
{
    QMutexLocker locker(&m_mutex);    
	
	return m_query.next();
}

QVariant dbapplitonapi::getValue(int nPos)
{
    QMutexLocker locker(&m_mutex);    
	
	return m_query.value(nPos);
}

QVariant dbapplitonapi::getValue(QString strFieldName)
{
    QMutexLocker locker(&m_mutex);    
	
	int nIndex = m_query.record().indexOf(strFieldName);

    return m_query.value(nIndex);
}

bool dbapplitonapi::transaction()
{
    QMutexLocker locker(&m_mutex);    
	
	return m_dbQt.transaction();
}

bool dbapplitonapi::commit()
{
    QMutexLocker locker(&m_mutex);    
	
	return m_dbQt.commit();
}

bool dbapplitonapi::rollback()
{
    QMutexLocker locker(&m_mutex);    
	
	return m_dbQt.rollback();
}

/*
QSqlQuery query;
query.prepare("INSERT INTO person (id, forename, surname) "
        "VALUES (:id, :forename, :surname)");
query.bindValue(":id", 1001);
query.bindValue(":forename", "Bart");
query.bindValue(":surname", "Simpson");
query.exec();
*/
bool dbapplitonapi::prepare(QString &strSql)
{
    QMutexLocker locker(&m_mutex);    
	
	return m_query.prepare(strSql);
}

void dbapplitonapi::bindValue(QString strField, QVariant varValue)
{
    QMutexLocker locker(&m_mutex);    
	
	m_query.bindValue(strField, varValue);
}

bool dbapplitonapi::exec()
{	
	QMutexLocker locker(&m_mutex);

	if(m_query.exec())
	{
		return true;
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}
}

void dbapplitonapi::addBindValue(QVariant varValue)
{
    QMutexLocker locker(&m_mutex);    
	
	m_query.addBindValue(varValue);
}

void dbapplitonapi::bindValue(int &nPos, QVariant varValue)
{
    QMutexLocker locker(&m_mutex);    
	
	m_query.bindValue(nPos, varValue);
}

void dbapplitonapi::addBatchBindValue(QVariantList &varList)
{
    QMutexLocker locker(&m_mutex);    
	
	m_query.addBindValue(varList);
}

bool dbapplitonapi::execBatch()
{
	QMutexLocker locker(&m_mutex);    

	if(m_query.execBatch())
	{
		return true;
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}
}

bool dbapplitonapi::getFieldType(QString strTable, QString strFieldAlasName, QString strFieldName, CVarinfo &info)
{
	QMutexLocker locker(&m_mutex);
	
	QString strUser = UID;
	
	QString strSql = "SELECT * FROM all_tab_columns t WHERE t.OWNER='" + strUser + "'" +
								" AND t.TABLE_NAME='" + strTable + "' AND t.COLUMN_NAME='" + strFieldName + "'";

	if(m_query.exec(strSql))
	{
		while(m_query.next())
		{
			QString strFieldName = ALL_TAB_COLUMN_NAME;

			int nPos = m_query.record().indexOf(strFieldName);

			info.name = strFieldAlasName.toStdString();

			strFieldName = ALL_TAB_DATA_TYPE;
			nPos = m_query.record().indexOf(strFieldName);
			QString strTmp = m_query.value(nPos).toString();
			if(strTmp.contains(DB_TYPE_NUMBER) ||
				strTmp.contains(DB_TYPE_FLOAT))
			{
				info.dbtype = 2;
			}
			else if(strTmp.contains(DB_TYPE_CHAR, Qt::CaseInsensitive) ||
				strTmp.contains(DB_TYPE_VARCHAR, Qt::CaseInsensitive))
			{
				info.dbtype = 3;
			}

			strFieldName = ALL_TAB_COLUMN_DATA_LENGTH;
			nPos = m_query.record().indexOf(strFieldName);
			info.dbsize = m_query.value(nPos).toInt();

			strFieldName = ALL_TAB_COLUMN_DATA_PRECISION;
			nPos = m_query.record().indexOf(strFieldName);
			info.prec = m_query.value(nPos).toInt();

			strFieldName = ALL_TAB_COLUMN_DATA_SCALE;
			nPos = m_query.record().indexOf(strFieldName);
			
			if(strTmp.contains(DB_TYPE_FLOAT))
			{
				info.scale = 129;	//float
			}
			else
			{
				info.scale = m_query.value(nPos).toInt();
			}
			
		}
	}
	else
	{
		m_strError = m_query.lastError().text();
		
		return false;
	}

	return true;
}

QString dbapplitonapi::trimmed(QString &strData) const
{
	QMutexLocker locker(&m_mutex);
	
	return strData.trimmed();
}

QString dbapplitonapi::getFieldName(int nPos)
{
	QMutexLocker locker(&m_mutex);
	
	return m_query.record().fieldName(nPos);
}

bool dbapplitonapi::getVec(QString strSql, TwoVec &vec)
{
	QMutexLocker locker(&m_mutex);
	
	vec.clear();
	
	if(m_query.exec(strSql))
	{
		int nCols = m_query.record().count();
		
		while(m_query.next())
		{
			QVector<QString> vecTmp;

			for(int i = 0; i < nCols; i++)
			{
				vecTmp.push_back( m_query.value(i).toString().trimmed());
			}

			vec.push_back(vecTmp);
		}
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}

	return true;
}

#include <QStringList>

void dbapplitonapi::close()
{
	QMutexLocker locker(&m_mutex);
	
	QStringList list = QSqlDatabase::connectionNames();
	
	m_dbQt.close();
}

bool dbapplitonapi::getFieldsTypeFromTable(QString strTable, std::vector<CVarinfo> &info)
{
	QMutexLocker locker(&m_mutex);
	
	QString strUser = UID;

	QString strSql = "SELECT * FROM all_tab_columns t WHERE t.OWNER='" + strUser + "'" +
		" AND t.TABLE_NAME='" + strTable;

	info.clear();
	

	if(m_query.exec(strSql))
	{
		while(m_query.next())
		{
			CVarinfo infoTmp;
			
			QString strFieldName = ALL_TAB_COLUMN_NAME;

			int nPos = m_query.record().indexOf(strFieldName);

			infoTmp.name = m_query.value(nPos).toString().toStdString();

			strFieldName = ALL_TAB_DATA_TYPE;
			nPos = m_query.record().indexOf(strFieldName);
			QString strTmp = m_query.value(nPos).toString();
			if(strTmp.contains(DB_TYPE_NUMBER) ||
				strTmp.contains(DB_TYPE_FLOAT))
			{
				infoTmp.dbtype = 2;
			}

			strFieldName = ALL_TAB_COLUMN_DATA_LENGTH;
			nPos = m_query.record().indexOf(strFieldName);
			infoTmp.dbsize = m_query.value(nPos).toInt();

			strFieldName = ALL_TAB_COLUMN_DATA_PRECISION;
			nPos = m_query.record().indexOf(strFieldName);
			infoTmp.prec = m_query.value(nPos).toInt();

			strFieldName = ALL_TAB_COLUMN_DATA_SCALE;
			nPos = m_query.record().indexOf(strFieldName);

			if(strTmp.contains(DB_TYPE_FLOAT))
			{
				infoTmp.scale = 129;	//float
			}
			else
			{
				infoTmp.scale = m_query.value(nPos).toInt();
			}

			info.push_back(infoTmp);
		}
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}

	return true;
}
