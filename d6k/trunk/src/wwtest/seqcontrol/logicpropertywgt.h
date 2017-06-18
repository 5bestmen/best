#ifndef LOGICPROPERTYWGT_H
#define LOGICPROPERTYWGT_H

#include <QWidget>
#include "ui_logicpropertywgt.h"
#include "seqdefine.h"

class QTableWidgetItem;
class CLogicPropertyWgt : public QWidget
{
    Q_OBJECT

public:
    CLogicPropertyWgt(QWidget *parent = 0);
    ~CLogicPropertyWgt();
    //更新表格中内容
    void UpdateItemPro(const QList<LOGICITEMPRO>& lstInput, const QList<LOGICITEMPRO>& lstOutput);
    //隐藏 input数量
    void SetInputNumHidden();
    //设置item类型编码
    void SetItemTypeCode(int nCode);
    //设置子程序列表
    void SetChildLst(const QStringList &lstChildNames);

public slots:
    void Slot_Conform();
    //取消
    void Slot_Cancel();
    //双击
    void Slot_DoubleClickTableItem(QTableWidgetItem *pItem);
signals:
    void Signal_ConformComment(const QString &strComment);
    //显示  en/eno 信号  0不显示   1显示
    void Signal_EnoDisplay(int nFlag);

private:
    Ui::CLogicPropertyWgt ui;
    //注释
    QString m_strComment;
    //item类型编码
    int m_nItemTypeCode;
    //子程序名称列表
    QStringList m_lstChildNames;
};

#endif // LOGICPROPERTYWGT_H
