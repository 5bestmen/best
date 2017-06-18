#ifndef DATAIOANALYSE_H
#define DATAIOANALYSE_H

#include <QObject>
#include <QMap>
#include "seqdefine.h"

class CFdbOrderScene;
class CFdbOrderBaseItem;

class CDataIOAnalyse : public QObject
{
    Q_OBJECT

public:
    CDataIOAnalyse(CFdbOrderScene *pScene,QObject *parent = 0);
    //组装item
    void MakeLogicItem(const QList<ITEMINFO> &lstItems);
    //装置line
    void MaieLineItem(const QList<LINEROUNTER> &lstLines);
    //comment
    void MakeCommentItem(const QList<COMMENTDATA> &lstLines);

    //复制item
    void CopyLogicItem(const QList<ITEMINFO> &lstItems);
    //复制line
    void CopyLineItem(const QList<LINEROUNTER> &lstLines);
    //复制comment
    void CopyCommentItem(const QList<COMMENTDATA> &lstLines);
    //设置起始地址
    void SetStartPoint(QPointF pfStart);
    //设置目的位置
    void SetEndPoint(QPointF pfEnd);

    //剪切item
    void CutLoginItem(const QList<ITEMINFO> &lstItems);
    //剪切line
    void CutLineItem(const QList<LINEROUNTER> &lstLines);
    //剪切comment
    void CutCommentItem(const QList<COMMENTDATA> &lstLines);

    ~CDataIOAnalyse();
    //解析数据
private:
    CFdbOrderScene *m_pScene;
    //创建编号->item
    QMap<int, CFdbOrderBaseItem*> m_mapCreateNumItem;
    //复制编号->item
    QMap<int, CFdbOrderBaseItem*> m_mapCopyNumItem;
    //复制  剪切的起始地址
    QPointF m_pfStartPoint;
    //复制  剪切  方的位置
    QPointF m_pfEndPoint;
    
};

#endif // DATAIOANALYSE_H
