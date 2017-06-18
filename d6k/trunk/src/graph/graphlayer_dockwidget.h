#ifndef CGRAPHLAYERDOCKWIDGET_H
#define CGRAPHLAYERDOCKWIDGET_H

#include <QDockWidget>

class CGraphLayerDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	//CGraphLayerDockWidget();
	explicit CGraphLayerDockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~CGraphLayerDockWidget();
};

#endif // CGRAPHLAYERDOCKWIDGET_H
