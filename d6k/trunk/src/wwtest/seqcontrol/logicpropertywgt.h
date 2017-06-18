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
    //���±��������
    void UpdateItemPro(const QList<LOGICITEMPRO>& lstInput, const QList<LOGICITEMPRO>& lstOutput);
    //���� input����
    void SetInputNumHidden();
    //����item���ͱ���
    void SetItemTypeCode(int nCode);
    //�����ӳ����б�
    void SetChildLst(const QStringList &lstChildNames);

public slots:
    void Slot_Conform();
    //ȡ��
    void Slot_Cancel();
    //˫��
    void Slot_DoubleClickTableItem(QTableWidgetItem *pItem);
signals:
    void Signal_ConformComment(const QString &strComment);
    //��ʾ  en/eno �ź�  0����ʾ   1��ʾ
    void Signal_EnoDisplay(int nFlag);

private:
    Ui::CLogicPropertyWgt ui;
    //ע��
    QString m_strComment;
    //item���ͱ���
    int m_nItemTypeCode;
    //�ӳ��������б�
    QStringList m_lstChildNames;
};

#endif // LOGICPROPERTYWGT_H
