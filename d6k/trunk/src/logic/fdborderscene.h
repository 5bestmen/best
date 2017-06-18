#ifndef CFDBORDERSCENE
#define CFDBORDERSCENE

#include <QGraphicsScene>
#include <memory>
#include "seqdefine.h"

class QGraphicsSceneMouseEvent;
class QGraphicsLineItem;
class QPaintEvent;
class QCursor;
class CCommentItem;
class QKeyEvent;
class CConnectionRouter;
class QGraphicsRectItem;
class CLogicObjectModel;
class CAbstractFactory;
class CFdbOrderBaseItem;

class CFdbOrderScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CFdbOrderScene(QObject *parent = 0, std::shared_ptr<CLogicObjectModel> pObjectModel = 0);
    ~CFdbOrderScene();
    void SetMode(int nMode);
    int GetMode() const;
    //检测是否为合法终点
    void UpdateItemPoint(const QPointF &pfPoint);
    //构造保存数据
    void MakeSaveData(const QString &filename);
    //更新需要保存的list
    void UpdateOperate();
    //
    void SaveOperateData(std::shared_ptr<CLogicObjectModel>);
    //制作新界面
    void MakeNewGraphic();
    //清空
    void ClearAllItems();
    //
    std::shared_ptr<CLogicObjectModel> GetObjectModel();
    //
    void UpdateModelData();

    enum {
             COPY_ONLY = 1,
             CUT_ONLY
         };

    CFdbOrderBaseItem * CreateItemObj(int nChangeType, int nIndex);

public slots:
    void Slot_OnLineAct();
    //注释
    void Slot_AddCommentAct();
    //保存
    void Slot_SaveAsAct();
    //打开
    void Slot_OpenAct();
    // 撤销
    void Slot_UndoAct();
    //重做
    void Slot_RedoAct();
    //清空
    void Slot_ClearAllAct();
    //复制
    void Slot_CopyAct();
    //粘贴
    void Slot_PasteAct();
    //剪切
    void Slot_CutAct();
    //仿真
    void Slot_Emulation();
    //
    void Slot_TimeOut();
    //删除comment
    void Slot_DeleteComment();
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void keyPressEvent(QKeyEvent *event);

signals:
private:
    //模式
    int m_nMode;
    //中间线
    QGraphicsLineItem *m_pLineItem;
    //点击的点
    QPointF m_pfClickBeg;
    //当前点
    QPointF m_pfCurrentPoint;
    //回退到第几部
    int m_nControlNum;
    //选中框 
    QGraphicsRectItem *m_pChoseItem;
    //选中框的起始点
    QPointF m_pfChooseBeg;
    //选中框的结束点
    QPointF m_pfChooseEnd;

    //复制的数据
    //GRAHPICDATA m_CpyData;
    //
    CLogicObjectModel *m_pCpyData;
    //剪切的数据
    //GRAHPICDATA m_CutData;
    //
    CLogicObjectModel *m_pCutData;
    //复制 
    int m_nCpyCutFlag;
    //选中框的起始点
    QPointF m_pfChooseAreaStart;
    //点击copy后保存的点
    QPointF m_pfCpyCutPoint;
    //右击时鼠标所在位置 
    QPointF m_pfRightPoint;
    //定时器
    QTimer *m_pTimer;
    //
    std::shared_ptr<CLogicObjectModel> m_pSecenModel;
    //历史文件   
    QList<std::shared_ptr<CLogicObjectModel>> m_lstHistoryData;
    //创建对象
    CAbstractFactory *m_pItemFactory;
};

#endif // CPLCORDERSCENE
