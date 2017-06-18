#ifndef PDR_H
#define PDR_H

#include <QObject>

class IMainWindow;

class CPdr : public QObject
{
	Q_OBJECT

public:
	CPdr(IMainWindow *pUi);
	~CPdr();

	void Disconnect()
	{

	}

public slots:
	void showMouseRightButton(const QPoint& point);

private:
	IMainWindow *m_pUi;
};

#endif // PDR_H
