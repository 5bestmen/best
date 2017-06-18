#ifndef SEQCONTROLWGT_H
#define SEQCONTROLWGT_H

#include <QWidget>
#include <QMap>

class CTreeWidget;
class CFdbOrderScene;
class CFdbOrderView;
class QToolBar;
class QTabWidget;
class QTreeWidgetItem;
class CFdbItemChooseWgt;

class CSeqControlWgt : public QWidget
{
    Q_OBJECT

public:
    CSeqControlWgt(QWidget *parent = 0);
    ~CSeqControlWgt();
    void CreateToolBar();
public slots:
    void Slot_ChooseCursor();
    void Slot_LineCursor();
    //双击打开
    void Slot_DoubleClickTreeItem(QTreeWidgetItem *pItem, int nColumn);
    //关闭tab
    void Slot_CloseTabWidget(int nIndex);

private:
    //工具框
    CTreeWidget *m_pTreeInfo;
    //sense
//     CPlcOrderScene *m_pGraphicScene;
    //view
//     CPlcOrderView *m_pGraphicsView;
    //toolbar
    QToolBar *m_pCursorToolBar;
    //tabwidget
    QTabWidget *m_pTabWgt;
    //
    QMap<QTreeWidgetItem*, QWidget*> m_mapItemWgt;
    //fbd选择界面
    CFdbItemChooseWgt *m_pFdbItemChooseWgt;
};

#endif // SEQCONTROLWGT_H
