#include <QString>

#include <QMap>
#include <QDomElement>
#include <QXmlStreamReader>


typedef struct _tagPPoint
{
	int nID;
	int nType;
	QString szName;
	QString szTagName;
}PPoint;


class CReadXml
{
public:
	CReadXml();
	~CReadXml();

private:

	bool ReadGroupInfo(const QString& szPath);

	bool ReadPredictNodeInfo();

	bool ReadPredictFile();

	bool ReadStationData(QXmlStreamReader& reader);

	bool ReadPlantData(QXmlStreamReader& reader);

	bool ReadWeatherData(QXmlStreamReader& reader);

	bool ReadPredictData(QXmlStreamReader& reader);

	bool ReadInverterData(QXmlStreamReader& reader);

	bool ReadData();

	bool ReadADIN();

	bool ReadAi();

	bool ReadDi();


private:

	PPoint plantPointinfo;
	PPoint weatherPointinfo;
	PPoint predictPointinfo; 
	PPoint inverterAIPointinfo;
	PPoint inverterDIPointinfo;

	QMap<QString, PPoint> map_PointInfo;

	QMap<QString, QString>  map_TagName;

};


class CReadDatabase
{
public:
	CReadDatabase();
	~CReadDatabase();

public:


};