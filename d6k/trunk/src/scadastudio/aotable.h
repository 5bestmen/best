#ifndef AOTABLE_H
#define AOTABLE_H

#include <QTableView>
#include "datatypes.h"

class CSortFilterModel;
class IMainModuleInterface;
class CAOModel;

namespace Config
{
	class CAnalogOutput;
	class CFesData;
}

class CAOTable : public QTableView
{
	Q_OBJECT

public:
	CAOTable(QWidget *parent, std::vector <Config::CAnalogOutput *> &arrAOs, IMainModuleInterface *pCore, void *pModule, Config::CFesData *pFes);
	~CAOTable();
public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex & index);
protected:
	void mouseReleaseEvent(QMouseEvent * event);
private:
	void AddAnalogPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &index);
signals:
	void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

private:
	CSortFilterModel *m_pSortModel;
	CAOModel *m_pModel;
	Config::CFesData *m_pFes;
};

#endif // AOTABLE_H
