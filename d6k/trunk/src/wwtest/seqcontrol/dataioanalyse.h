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
    //��װitem
    void MakeLogicItem(const QList<ITEMINFO> &lstItems);
    //װ��line
    void MaieLineItem(const QList<LINEROUNTER> &lstLines);
    //comment
    void MakeCommentItem(const QList<COMMENTDATA> &lstLines);

    //����item
    void CopyLogicItem(const QList<ITEMINFO> &lstItems);
    //����line
    void CopyLineItem(const QList<LINEROUNTER> &lstLines);
    //����comment
    void CopyCommentItem(const QList<COMMENTDATA> &lstLines);
    //������ʼ��ַ
    void SetStartPoint(QPointF pfStart);
    //����Ŀ��λ��
    void SetEndPoint(QPointF pfEnd);

    //����item
    void CutLoginItem(const QList<ITEMINFO> &lstItems);
    //����line
    void CutLineItem(const QList<LINEROUNTER> &lstLines);
    //����comment
    void CutCommentItem(const QList<COMMENTDATA> &lstLines);

    ~CDataIOAnalyse();
    //��������
private:
    CFdbOrderScene *m_pScene;
    //�������->item
    QMap<int, CFdbOrderBaseItem*> m_mapCreateNumItem;
    //���Ʊ��->item
    QMap<int, CFdbOrderBaseItem*> m_mapCopyNumItem;
    //����  ���е���ʼ��ַ
    QPointF m_pfStartPoint;
    //����  ����  ����λ��
    QPointF m_pfEndPoint;
    
};

#endif // DATAIOANALYSE_H
