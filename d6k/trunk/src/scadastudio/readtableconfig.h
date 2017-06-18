#ifndef READTABLECONFIG_H
#define READTABLECONFIG_H

#include "scadastudiodefine.h"
#include <QVector>

class QXmlStreamReader;
class QFile;

class CReadTableConfig
{
public:
	CReadTableConfig();
	~CReadTableConfig();

	bool ReadFieldData(QString &strTableName, QVector<CHANNELSCADASTUDIO> &channelStr);

	//»ñµÃheader
	QStringList GetHeaderList();

protected:
	bool LogString(const char *szLogTxt, int nLevel);


private:
	QXmlStreamReader *m_pXmlRead;
	QFile *m_pFile;

	QVector<CHANNELSCADASTUDIO> m_arrChannelData;
};

#endif // sREADTABLECONFIG_H
