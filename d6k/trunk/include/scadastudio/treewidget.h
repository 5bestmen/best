#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeView>

class CTreeWidget : public QTreeView
{
	Q_OBJECT

public:
	enum{LEFTCLICK,LEFTDOUBLECLICK,RIGHTCLICK};

public:
	CTreeWidget(QWidget *parent);
	~CTreeWidget();

public slots:
	void showMouseRightButton(const QPoint& point);

protected:
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);

signals:
	//左边树形添加通道通道组
	void SendFesType(int nType, QModelIndex &indexSelect, int nMmouseClickType);
private:
	void CheckType(int nType, QModelIndex &indexSelect, int nMouseClickType);
};

#endif // TREEWIDGET_H
