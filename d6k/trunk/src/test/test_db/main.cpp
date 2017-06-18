#include <QtCore/QCoreApplication>

#include <QDebug>
#include <QStringList>


#include "dbapi.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	qDebug() << "Availabledrivers:";
	QStringList drivers = QSqlDatabase::drivers();
	foreach(QString driver, drivers)
	{
		qDebug() << "\t" << driver;
	}

	dbapi db("test");
	bool res = db.open("d6000", "d6k", "d6k2", 1);
	if (!res)
	{
		return -1;
	}

	/*
CREATE TABLE `TEST`
(
  `ID`    int(10) NOT NULL,
  `DESCRIBE`    varchar(40) NOT NULL,
  `VALUE` 	varchar(16) DEFAULT '0.000',
  `UNIT` varchar(10) DEFAULT NULL,
  `TIME_OF_VALUE` varchar(20) DEFAULT 0,
  `TYPE` TINYINT(4) DEFAULT NULL,
   PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	*/
	const QString strSQL = QString("INSERT INTO `TEST` VALUES (2, '%1', '0.000', 'kWh', '0', 1);").arg(QStringLiteral("elec"));
	res = db.exec(strSQL);
	if (!res)
	{
		qDebug() << db.lastError();
		return -1;
	}

	if (!db.commit())
	{
		return -1;
	}
	return a.exec();
}
