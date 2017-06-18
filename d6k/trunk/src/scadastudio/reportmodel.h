#ifndef REPORTMODEL_H
#define REPORTMODEL_H

#include <QObject>

class IMainWindow;

class CReportModel : public QObject
{
	Q_OBJECT

public:
	CReportModel(IMainWindow *pUi);
	~CReportModel();

	void Init(IMainWindow *pUi);

	void UnInit();

public slots:
	void showMouseRightButton(const QPoint& point);

private:
	IMainWindow *m_pUi;
};

#endif // REPORTMODEL_H
