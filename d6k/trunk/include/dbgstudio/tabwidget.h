#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <functional>

class CTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	CTabWidget(QWidget *parent);
	~CTabWidget();

	void AddTab(QWidget *widget, const QString &strTitle, const QString &strType)
	{
		addTab(widget, strTitle);

		widget->setObjectName(strType);
	}

	std::function< int(void* pData)> m_fnBeforeCloseTab;
	void *m_pBeforeCloseTabData;


private slots:
	void closeTileWidget(int index);
	void tabChanged(int index);
};

#endif // TABWIDGET_H
