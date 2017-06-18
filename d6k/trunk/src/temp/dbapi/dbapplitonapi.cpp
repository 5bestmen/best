/*! @file
<PRE>
********************************************************************************
模块名       :  数据库
文件名       :  dbapplitonapi.cpp
文件实现功能 :  数据库
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2015.11.26
*/

#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QMutexLocker>
#include "dbapplitonapi.h"

QMutex CDbapplitonapi::m_mutex;

/*! \fn dbapplitonapi::dbapplitonapi(QString strConnName)
********************************************************************************************************* 
** \brief dbapplitonapi::dbapplitonapi 
** \details 
** \param strConnName 
** \return  
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CDbapplitonapi::CDbapplitonapi(QString strConnName)
{
    QMutexLocker locker(&m_mutex);
	
	m_strConnName = strConnName;
}

/*! \fn CDbapplitonapi::~CDbapplitonapi()
********************************************************************************************************* 
** \brief CDbapplitonapi::~CDbapplitonapi 
** \details 
** \return  
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CDbapplitonapi::~CDbapplitonapi()
{
    QMutexLocker locker(&m_mutex);
	
	m_query.clear();
}

/*! \fn bool CDbapplitonapi::Open(const QString &strDns, const QString &strUserName, const QString &strPassword, int nType, int timeout)
********************************************************************************************************* 
** \brief CDbapplitonapi::Open 
** \details 打开数据库
** \param strDns 
** \param strUserName 
** \param strPassword 
** \param nType 
** \param timeout 
** \return bool 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
bool CDbapplitonapi::Open(const QString &strDns, const QString &strUserName, const QString &strPassword, int nType, int timeout)
{
    QMutexLocker locker(&m_mutex);
	
	if(m_dbQt.contains(m_strConnName))
    {
		m_strError = "连接已存在!!!";

		return false;
    }
    else
    {
       //m_dbQt = QSqlDatabase::addDatabase("QOCI", m_strConnName);

       //m_dbQt.setDatabaseName(strDns);    //数据库名
       //m_dbQt.setPort(1521);
       //m_dbQt.setUserName(strUserName);        //数据库用户名
       //m_dbQt.setPassword(strPassword);       //数据库密码

		m_dbQt = QSqlDatabase::addDatabase("QSQLITE", m_strConnName);

		m_dbQt.setDatabaseName("d6k.db");

       if(!m_dbQt.open())
       {
           m_strError = m_dbQt.lastError().text();

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

/*! \fn QString CDbapplitonapi::LastError()
********************************************************************************************************* 
** \brief CDbapplitonapi::LastError 
** \details 错误提示
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
QString CDbapplitonapi::LastError()
{
	QMutexLocker locker(&m_mutex);    
	
	return m_strError;
}

/*! \fn bool CDbapplitonapi::Exec(QString &strSql)
********************************************************************************************************* 
** \brief CDbapplitonapi::Exec 
** \details 执行
** \param strSql 
** \return bool 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
bool CDbapplitonapi::Exec(QString &strSql)
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

/*! \fn QSqlQuery& CDbapplitonapi::GetSqlQuery()
********************************************************************************************************* 
** \brief CDbapplitonapi::GetSqlQuery 
** \details 
** \return QSqlQuery& 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
QSqlQuery& CDbapplitonapi::GetSqlQuery()
{
	QMutexLocker locker(&m_mutex);
	
	return m_query;
}

/*! \fn int CDbapplitonapi::GetFieldCounts()
********************************************************************************************************* 
** \brief CDbapplitonapi::GetFieldCounts 
** \details 列数
** \return int 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
int CDbapplitonapi::GetFieldCounts()
{
    QMutexLocker locker(&m_mutex);    
	
	return m_query.record().count();
}

/*! \fn int CDbapplitonapi::GetFieldIndex(QString &strFieldName)
********************************************************************************************************* 
** \brief CDbapplitonapi::GetFieldIndex 
** \details 
** \param strFieldName 
** \return int 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
int CDbapplitonapi::GetFieldIndex(QString &strFieldName)
{
    QMutexLocker locker(&m_mutex);    
	
	return m_query.record().indexOf(strFieldName);
}

/*! \fn bool CDbapplitonapi::Next()
********************************************************************************************************* 
** \brief CDbapplitonapi::Next 
** \details 
** \return bool 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
bool CDbapplitonapi::Next()
{
    QMutexLocker locker(&m_mutex);    
	
	return m_query.next();
}

/*! \fn QVariant CDbapplitonapi::GetValue(int nPos)
********************************************************************************************************* 
** \brief CDbapplitonapi::GetValue 
** \details 
** \param nPos 
** \return QT_NAMESPACE::QVariant 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
QVariant CDbapplitonapi::GetValue(int nPos)
{
    QMutexLocker locker(&m_mutex);    
	
	return m_query.value(nPos);
}

/*! \fn QVariant CDbapplitonapi::GetValue(QString strFieldName)
********************************************************************************************************* 
** \brief CDbapplitonapi::GetValue 
** \details 
** \param strFieldName 
** \return QT_NAMESPACE::QVariant 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
QVariant CDbapplitonapi::GetValue(QString strFieldName)
{
    QMutexLocker locker(&m_mutex);    
	
	int nIndex = m_query.record().indexOf(strFieldName);

    return m_query.value(nIndex);
}

/*! \fn bool CDbapplitonapi::Transaction()
********************************************************************************************************* 
** \brief CDbapplitonapi::Transaction 
** \details 
** \return bool 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
bool CDbapplitonapi::Transaction()
{
    QMutexLocker locker(&m_mutex);    
	
	return m_dbQt.transaction();
}

/*! \fn bool CDbapplitonapi::Commit()
********************************************************************************************************* 
** \brief CDbapplitonapi::Commit 
** \details 
** \return bool 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
bool CDbapplitonapi::Commit()
{
    QMutexLocker locker(&m_mutex);    
	
	return m_dbQt.commit();
}

/*! \fn bool CDbapplitonapi::Rollback()
********************************************************************************************************* 
** \brief CDbapplitonapi::Rollback 
** \details 
** \return bool 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
bool CDbapplitonapi::Rollback()
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


/*! \fn bool CDbapplitonapi::Prepare(QString &strSql)
********************************************************************************************************* 
** \brief CDbapplitonapi::Prepare 
** \details 
** \param strSql 
** \return bool 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
bool CDbapplitonapi::Prepare(QString &strSql)
{
    QMutexLocker locker(&m_mutex);    
	
	return m_query.prepare(strSql);
}

/*! \fn void CDbapplitonapi::BindValue(QString strField, QVariant varValue)
********************************************************************************************************* 
** \brief CDbapplitonapi::BindValue 
** \details 
** \param strField 
** \param varValue 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CDbapplitonapi::BindValue(QString strField, QVariant varValue)
{
    QMutexLocker locker(&m_mutex);    
	
	m_query.bindValue(strField, varValue);
}

/*! \fn bool CDbapplitonapi::Exec()
********************************************************************************************************* 
** \brief CDbapplitonapi::Exec 
** \details 
** \return bool 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
bool CDbapplitonapi::Exec()
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

/*! \fn void CDbapplitonapi::AddBindValue(QVariant varValue)
********************************************************************************************************* 
** \brief CDbapplitonapi::AddBindValue 
** \details 
** \param varValue 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CDbapplitonapi::AddBindValue(QVariant varValue)
{
    QMutexLocker locker(&m_mutex);    
	
	m_query.addBindValue(varValue);
}

/*! \fn void CDbapplitonapi::BindValue(int &nPos, QVariant varValue)
********************************************************************************************************* 
** \brief CDbapplitonapi::BindValue 
** \details 
** \param nPos 
** \param varValue 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CDbapplitonapi::BindValue(int &nPos, QVariant varValue)
{
    QMutexLocker locker(&m_mutex);    
	
	m_query.bindValue(nPos, varValue);
}

/*! \fn void CDbapplitonapi::AddBatchBindValue(QVariantList &varList)
********************************************************************************************************* 
** \brief CDbapplitonapi::AddBatchBindValue 
** \details 
** \param varList 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CDbapplitonapi::AddBatchBindValue(QVariantList &varList)
{
    QMutexLocker locker(&m_mutex);    
	
	m_query.addBindValue(varList);
}

/*! \fn bool CDbapplitonapi::ExecBatch()
********************************************************************************************************* 
** \brief CDbapplitonapi::ExecBatch 
** \details 
** \return bool 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
bool CDbapplitonapi::ExecBatch()
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

/*! \fn QString CDbapplitonapi::Trimmed(QString &strData) const
********************************************************************************************************* 
** \brief CDbapplitonapi::Trimmed 
** \details 
** \param strData 
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
QString CDbapplitonapi::Trimmed(QString &strData) const
{
	QMutexLocker locker(&m_mutex);
	
	return strData.trimmed();
}

/*! \fn QString CDbapplitonapi::GetFieldName(int nPos)
********************************************************************************************************* 
** \brief CDbapplitonapi::GetFieldName 
** \details 
** \param nPos 
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
QString CDbapplitonapi::GetFieldName(int nPos)
{
	QMutexLocker locker(&m_mutex);
	
	return m_query.record().fieldName(nPos);
}

/*! \fn void CDbapplitonapi::Close()
********************************************************************************************************* 
** \brief CDbapplitonapi::Close 
** \details 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CDbapplitonapi::Close()
{
	QMutexLocker locker(&m_mutex);
	
	//QStringList list = QSqlDatabase::connectionNames();
	
	m_dbQt.close();
}