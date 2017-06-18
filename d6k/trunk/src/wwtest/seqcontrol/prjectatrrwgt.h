#ifndef PRJECTATRRWGT_H
#define PRJECTATRRWGT_H

#include <QDialog>
#include "ui_prjectatrrwgt.h"

class CPrjectAtrrWgt : public QDialog
{
    Q_OBJECT

public:
    CPrjectAtrrWgt(QWidget *parent = 0);
    ~CPrjectAtrrWgt();
    //��������
    void SetprojectName(const QString &projectName);
    //��ȡproject����
    QString GetProjectName();
    //��ȡ����
    QString GetPassWord();
    //��ȡע��
    QString GetComment();
    //
public slots:
    void Slot_Comform();
signals:
    void Signal_Conform();
private:
    Ui::CPrjectAtrrWgt ui;
    //��������
    QString m_strProjectName;
    //����
    QString m_strPassWd;
    //ע��
    QString m_strComment;
};

#endif // PRJECTATRRWGT_H
