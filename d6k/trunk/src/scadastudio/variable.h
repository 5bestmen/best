#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>

namespace Config
{
	class CUserVariableGroup;
	class CFesData;
}

class IMainWindow;
class IMainModuleInterface;
class QStandardItem;
class QAbstractItemModel;

class CVariable : public QObject
{
	Q_OBJECT

public:
	CVariable(IMainModuleInterface *pCore);
	~CVariable();

	void ShowMenu(QModelIndex &index);

	void AddUserGroup(QModelIndex &index);

	void DeleteUserGroup(QModelIndex &index);

	void Disconnect();

public slots:
	void showMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex &index);

	void itemChanged(QStandardItem *item);

private:
	QString GetGroupName(QModelIndex index);
	QString GetLastGroupName(QModelIndex index);
	void GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec);
	bool DeleteUserVaraibleCombineRelation(Config::CUserVariableGroup *pGroup, Config::CFesData *pFesData);
private:
	IMainWindow *m_pUi;
	IMainModuleInterface *m_pCore;
};

#endif // VARIABLE_H
