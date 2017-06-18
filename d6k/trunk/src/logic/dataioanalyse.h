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

    //复制block
    void CopyLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock);
    //复制line
    void CopyLineItem(std::vector<std::shared_ptr<CLineProperty>> tLine);
    //复制comment
    void CopyCommentItem(std::vector<std::shared_ptr<CCommentProperty>> tComment);

    //设置起始地址
    void SetStartPoint(QPointF pfStart);
    //设置目的位置
    void SetEndPoint(QPointF pfEnd);


    //剪切
    void CutLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock);
    //
    void CutLineItem(std::vector<std::shared_ptr<CLineProperty>> tLine);
    //
    void CutCommentItem(std::vector<std::shared_ptr<CCommentProperty>> tComment);

    ~CDataIOAnalyse();
    //创建
    void CreateLogic();
    //
    void CreateLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock);
    //
    void CreateLogicLine(std::vector<std::shared_ptr<CLineProperty>> tLine);
    //
    void CreateLogicComment(std::vector<std::shared_ptr<CCommentProperty>> tComment);
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
