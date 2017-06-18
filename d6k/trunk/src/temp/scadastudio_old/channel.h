#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

class CScadastudio;
class QModelIndex;

class CChannel : public QObject
{
	Q_OBJECT

public:
	CChannel(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType);
	~CChannel();

	void AddChannelGroup();

	void ShowMenu();

	void AddChannel();

	void AddDevice();

private:
	CScadastudio *m_pUi;

	int m_nMouseClickType;

	QModelIndex &m_index;
};

#endif // CHANNEL_H
