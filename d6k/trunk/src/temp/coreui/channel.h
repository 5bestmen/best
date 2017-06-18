#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
class CCoreui;

class CChannel : public QObject
{
	Q_OBJECT

public:
	CChannel(CCoreui *pUi);
	~CChannel();

	void AddChannelGroup();

	void ShowMenu(QModelIndex &index);

	void AddChannel();

	void AddDevice();
public slots:
	void showMouseRightButton(const QPoint& point);

private:
	CCoreui *m_pUi;
};

#endif // CHANNEL_H
