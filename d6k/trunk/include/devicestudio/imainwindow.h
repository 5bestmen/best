#ifndef IMAIN_WIN_H
#define IMAIN_WIN_H

#include <QMainWindow>

#pragma execution_character_set("utf-8")

class CTabWidget;
class CTreeWidget;
class QTableView;

class IMainWindow : public QMainWindow
{
//	Q_OBJECT
public:
	//ICoreui(QWidget *parent);
	virtual ~IMainWindow()
	{

	}
	//给外部模块接口
	//获得左边树指针
	virtual CTreeWidget *GetLeftTree()=0;
	//获得tab的指针
	virtual CTabWidget *GetTabWidget() = 0;
	//获取输出数据的tableview
	virtual QTableView *GetOutputTableView() = 0;
	
private:
	
};

#endif // IMAIN_WIN_H
