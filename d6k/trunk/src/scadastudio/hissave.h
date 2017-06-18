#ifndef HISSAVE_H
#define HISSAVE_H

#include <QObject>

class IMainWindow;

class CHisSave : public QObject
{
	Q_OBJECT

public:
	CHisSave(IMainWindow *pUi);
	~CHisSave();

	static const QString strTmp;

	void Disconnect()
	{
		
	}

private:
	IMainWindow *m_pUi;
};

#endif // HISSAVE_H
