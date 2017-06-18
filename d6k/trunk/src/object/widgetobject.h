#ifndef WIDGET_OBJECT_H
#define WIDGET_OBJECT_H

#include <QWidget>
#include <QTableView>

#include "../include/scadastudio/tabwidget.h"
#include "../include/scadastudio/icore.h"
#include "scadastudio/base.h"

#include "datainfo.h"
#include "contentmodel.h"

class CObjectView : public QTableView, public IBase
{
    Q_OBJECT

public:
    CObjectView(CObjectItem* pObjItem);
    ~CObjectView();

public:
    virtual void Save();
    virtual void Refresh();
    virtual void SetModifyFlag(bool bFlag);
    virtual bool GetModifyFlag();

    CObjectItem* GetObjItem() const;
private:

    CObjectItem* m_pObjItem;
};


class CObjectWidget : public QWidget
{
    Q_OBJECT
public:
    CObjectWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
    ~CObjectWidget();
public:

    void AddToTableCtrl(CObjectItem* pObjItem);

    void ReleaseData();
    void SetTabViewHead();
private:

    CContentModel* m_pContentModel;

    CTabWidget *m_pTableWidget; // tab¿Ø¼þ

    IMainModuleInterface *m_pCore;
};

#endif // WIDGET_OBJECT_H
