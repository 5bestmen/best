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
    //设置名称
    void SetprojectName(const QString &projectName);
    //设置是否加密
    void SetProtectFlag(bool bFlag);
    //设置密码  TODO
    //设置注释
    void SetComment(const QString &strComment);


    //获取project名称
    const QString &GetProjectName();
    //获取密码
    const QString & GetPassWord();
    //获取注释
    const QString & GetComment();
    //
    bool GetCheckFlag();
public slots:
    void Slot_Comform();
signals:
    void Signal_Conform();
private:
    Ui::CPrjectAtrrWgt ui;
    //工程名称
    QString m_strProjectName;
    //密码
    QString m_strPassWd;
    //注释
    QString m_strComment;
};

#endif // PRJECTATRRWGT_H
