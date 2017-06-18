#ifndef BUTTONEIDT_H
#define BUTTONEIDT_H

#include <QWidget>

class CButtonEdit : public QWidget
{
	Q_OBJECT

public:
	CButtonEdit(QWidget *parent);
	~CButtonEdit();

private slots:
	void buttonClicked();

private:
	
};

#endif // BUTTONEIDT_H
