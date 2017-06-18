#ifndef POWERPREDICTION_H
#define POWERPREDICTION_H

#include <QtWidgets/QMainWindow>
#include "ui_powerprediction.h"

class powerprediction : public QMainWindow
{
	Q_OBJECT

public:
	powerprediction(QWidget *parent = 0);
	~powerprediction();

private:
	Ui::powerpredictionClass ui;
};

#endif // POWERPREDICTION_H
