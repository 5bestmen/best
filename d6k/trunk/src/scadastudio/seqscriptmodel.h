#ifndef SEQSCRIPTMODEL_H
#define SEQSCRIPTMODEL_H

#include <QObject>

class IMainWindow;

class CSeqscriptModel : public QObject
{
	Q_OBJECT

public:
	CSeqscriptModel(IMainWindow *pUi);
	~CSeqscriptModel();

	void Init(IMainWindow *pUi);
	void UnInit();

private slots:

private:
	IMainWindow *m_pUi;
};

#endif // SEQSCRIPTMODEL_H
