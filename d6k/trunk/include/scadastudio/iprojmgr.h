#ifndef IPROJMGR_H
#define IPROJMGR_H

class QXmlStreamReader;
class QDomDocument;
class QFile;

class IProjMgr
{
public:
	//IProjMgr();
	virtual ~IProjMgr()
	{

	}

	virtual QDomDocument *GetDocument() = 0;

	virtual QFile *GetFile() = 0;

	virtual QString GetProjectPath() = 0;
private:
	
};

#endif // IPROJMGR_H
