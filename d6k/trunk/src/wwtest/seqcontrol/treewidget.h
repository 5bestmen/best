#ifndef CTREEWIDGET_H
#define CTREEWIDGET_H

#include <QTreeWidget>

class CAttrWgt;
class CPrjectAtrrWgt;
class CTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit CTreeWidget(QWidget *parent = 0);
    ~CTreeWidget();
    //设置程序item
    void CreateProjectItems();

public slots :
    void Slot_ContextMenuRequest(const QPoint &pPoint);
    //创建新工程
    void Slot_CreateNewProject();
    //创建新段
    void Slot_CreateNewSeg();
    //mast attr
    void Slot_MastAttr();
    //fast attr
    void Slot_FastAttr();
    //aux attr
    void Slot_AuxAttr();
    //project attr
    void Slot_ProjectAttr();
    //
    void Slot_ProjectConform();


public slots:
private:
    //mast
    CAttrWgt *m_pMastAttrWgt;
    //fast
    CAttrWgt *m_pFastAttrWgt;
    //aux
    CAttrWgt *m_pAuxAttrWgt;
    //project
    CPrjectAtrrWgt *m_pProjectWgt;
};

#endif // CTREEWIDGET_H
