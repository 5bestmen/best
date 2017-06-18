#ifndef DATAIOANALYSE_H
#define DATAIOANALYSE_H

#include <QObject>
#include <QMap>
#include <memory>
#include "seqdefine.h"

class CFdbOrderScene;
class CFdbOrderBaseItem;
class CBlockProperty;
class CLineProperty;
class CCommentProperty;

class CDataIOAnalyse
{
public:
    CDataIOAnalyse(CFdbOrderScene *pScene);

    //����block
    void CopyLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock);
    //����line
    void CopyLineItem(std::vector<std::shared_ptr<CLineProperty>> tLine);
    //����comment
    void CopyCommentItem(std::vector<std::shared_ptr<CCommentProperty>> tComment);

    //������ʼ��ַ
    void SetStartPoint(QPointF pfStart);
    //����Ŀ��λ��
    void SetEndPoint(QPointF pfEnd);


    //����
    void CutLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock);
    //
    void CutLineItem(std::vector<std::shared_ptr<CLineProperty>> tLine);
    //
    void CutCommentItem(std::vector<std::shared_ptr<CCommentProperty>> tComment);

    ~CDataIOAnalyse();
    //����
    void CreateLogic();
    //
    void CreateLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock);
    //
    void CreateLogicLine(std::vector<std::shared_ptr<CLineProperty>> tLine);
    //
    void CreateLogicComment(std::vector<std::shared_ptr<CCommentProperty>> tComment);
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
