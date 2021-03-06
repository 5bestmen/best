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
    //�����Ƿ����
    void SetProtectFlag(bool bFlag);
    //��������  TODO
    //����ע��
    void SetComment(const QString &strComment);


    //��ȡproject����
    const QString &GetProjectName();
    //��ȡ����
    const QString & GetPassWord();
    //��ȡע��
    const QString & GetComment();
    //
    bool GetCheckFlag();
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
