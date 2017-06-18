#ifndef SPLITTER_H
#define SPLITTER_H

#include <QObject>
#include <QSplitter>

class CSplitter : public QSplitter
{
	Q_OBJECT

public:
	CSplitter(QWidget *parent);
	~CSplitter();
	
private slots:
	void SetOri(Qt::Orientation ori);
};

#endif // SPLITTER_H
