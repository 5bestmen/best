#ifndef CMDHISTORYWGT_H
#define CMDHISTORYWGT_H

#include <QWidget>
#include "ui_cmdhistorywgt.h"


class QListWidgetItem;
class CCmdHistoryWgt : public QWidget
{
    Q_OBJECT

public:
    CCmdHistoryWgt(QWidget *parent = 0);
    void InitHistory();
    ~CCmdHistoryWgt();
public slots:
    void Slot_ClickeListItem(QListWidgetItem *lstItem);
    //
    void Slot_listContextMenuRequest(const QPoint &pfPoint);
    //
    void Slot_UpdateFileAct();

private:
    Ui::CCmdHistoryWgt ui;
    //文件名称  对应的目录
    QMap<QString, QString> m_mapFileName;
};

#endif // CMDHISTORYWGT_H
