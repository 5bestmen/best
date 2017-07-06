#ifndef HISSAVE_H
#define HISSAVE_H

#include <QObject>

class IMainWindow;
class IMainModuleInterface;

class CHisSave : public QObject
{
	Q_OBJECT

public:
	CHisSave(IMainModuleInterface *pCore);
	~CHisSave();

	static const QString strTmp;

	void Disconnect()
	{
		
	}

private slots:
	void DoubleClicked(const QModelIndex &index);
private:
	IMainWindow *m_pUi;
	IMainModuleInterface *m_pCore;
};

#endif // HISSAVE_H
