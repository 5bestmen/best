#ifndef HISTORYWGT_H
#define HISTORYWGT_H

#include <QWidget>
#include <QMap>
#include "ui_historywgt.h"
#include "asdu_info.h"

#pragma pack(1)

struct HISTORYALLINFO
{
    ASDUADDR2 xxdz;          //信息体地址
    unsigned char val;       //值
    CP56Time2a  tTime;       //时间
};
#pragma  pack()

class QListWidgetItem;
class CChoosePointsWgt;
class CHistoryWgt : public QWidget
{
    Q_OBJECT

public:
    CHistoryWgt(CChoosePointsWgt *pPontWgt,QWidget *parent = 0);
    void InitFile();
    //开始解析文件
    void AnalyseHistoryFile(const QString strFileName);
    //
    void AnalyseData(const QByteArray byData);
    //往表格中填充数据
    void InsertDataToTable(const QString &desctrber,int nAddress,int nValue,const QString &strTime);
    ~CHistoryWgt();
    

public slots:
    //点击槽函数
    void Slot_ClickeListItem(QListWidgetItem *item);
    //右击菜单
    void Slot_listContextMenuRequest(const QPoint &point);
    //更新菜单
    void Slot_UpdateFileAct();
private:
    Ui::CHistoryWgt ui;
    //文件名称  全路径文件名称
    QMap<QString, QString> m_mapFileName;
    CChoosePointsWgt *m_pChooseWgt;
};

#endif // HISTORYWGT_H
