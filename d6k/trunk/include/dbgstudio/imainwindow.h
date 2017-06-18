#ifndef IMAIN_WIN_H
#define IMAIN_WIN_H

#include <QMainWindow>

#pragma execution_character_set("utf-8")

class CTabWidget;
class CTreeWidget;
class QTableView;

class IMainWindow : public QMainWindow
{ 
public:
	//ICoreui(QWidget *parent);
	virtual ~IMainWindow()
	{

	}
	//���ⲿģ��ӿ�
	//��������ָ��
	virtual CTreeWidget *GetLeftTree()=0;
	//���tab��ָ��
	virtual CTabWidget *GetTabWidget() = 0;
	//��ȡ������ݵ�tableview
	virtual QTableView *GetOutputTableView() = 0;
	
private:
	
};

#endif // IMAIN_WIN_H
