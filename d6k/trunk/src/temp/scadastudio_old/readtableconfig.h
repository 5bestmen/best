#ifndef READTABLECONFIG_H
#define READTABLECONFIG_H

#include <QXmlStreamReader>
#include "define.h"
#include <QFile>

class QXmlStreamReader;

class CReadTableConfig
{
public:
	CReadTableConfig();
	~CReadTableConfig();

	bool ReadFieldData(QString &strTableName, QVector<CHANNEL> &channelStr);

	//»ñµÃheader
	QStringList getHeaderList();

private:
	QXmlStreamReader *m_xmlRead;
	QFile *m_file;

	QVector<CHANNEL> m_channelDataVec;
};

#endif // sREADTABLECONFIG_H
