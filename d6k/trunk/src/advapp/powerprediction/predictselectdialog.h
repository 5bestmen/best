#ifndef  PREDICT_SELECT_DIALOG_H
#define PREDICT_SELECT_DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>

class CPredictSelectDialog : public QDialog
{
	Q_OBJECT

public:

	CPredictSelectDialog(QWidget* parent = 0);

	~CPredictSelectDialog();

private:
	
	QPushButton* okButton;
	QPushButton* cancelButton;
	QComboBox* comboBox_1;
	QComboBox* comboBox_2;
	QComboBox* comboBox_3;
	QLabel* label_1;
	QLabel* label_2;
	QLabel* label_3;
	QGroupBox* groupBox_1;
	QGroupBox* groupBox_2;
	QGroupBox* groupBox_3;

};







#endif

