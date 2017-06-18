#ifndef FDBORDERBASEITEM_H
#define FDBORDERBASEITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QColor>
#include <memory>
#include "seqdefine.h"

class CConnectionRouter;
class QCursor;
class QTimer;
class CBinProperty;

class CFdbOrderBaseItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:

    CFdbOrderBaseItem();
    ~CFdbOrderBaseItem();
public:
    //����item����
    void SetItemType(int nItemType);
    //����createitem
    void SetCreateNum(int nCreateNum);
    //��ȡitem  rect
    const QRectF& GetItemRect();
    //��ȡ�������
    int GetCreateNum();
    //��ȡִ�б��
    int GetIndexNum();
    //��ȡitem��ɫ
    const QColor & GetItemColor();
    //��ȡitem����
    const QString& GetItemType();
    //��ȡע�Ͷ���
    QGraphicsTextItem *GetCommentItem();

    void SetItemStatus(int nStatus);
    //
    const std::vector<std::shared_ptr<CBinProperty>> GetInputPros();

    const std::vector<std::shared_ptr<CBinProperty>> GetOutPutPros();

    //�����������
    void SetInPutIndex(int nInputIndex);
    //�����������
    void SetOutPutIndex(int nOutputIndex);
    //��ѡ�п�
    void AddHoverRect(QPainter *painter);
    //����λ��
    virtual void AnalysePointPosition(const QPointF &pointCurrent);
    //ͨ��scene����������λitem������
    void AnalyseSencePoint(const QPointF &pScene);
    //ͨ��item�����ҳ�һ����ȷ�ĵ�
    virtual void GetLinePoint(const QPointF &pItem);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    //item����ֵ
    virtual int GetLogicValue();
    //
    virtual void AnalseBitInput();
    //���µ�ǰ����
    void UpdateCurrentItemInfo();
    //��ȡoutput pins��Ϣ
    const QStringList & GetOutputPins();
    //
    void SetComment(const QString &strComment);
public:
    //�жϵ�ǰitem����
    virtual  QString  AnalyseItemType(int nType);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;

    void RemovePloyLine(CConnectionRouter *ployline);
    void RemovePloyLines();
    void AddPloyLine(CConnectionRouter *ployline);
    //��ȡ����
    int GetPointWidth() const;
    //��ȡ�߶�
    int getPointHeight() const;
    void ResetRect(QRectF rect);

    virtual int type() const{ return OBJEC_TITEM; }
    //��ȡ���ߵ�״̬
    int GetItemIOStatus();
    //����ִ��˳����
    void SetIndexNumber(int nIndex);
    //��ȡ������
    int GetInputIndex();
    //��ȡ������
    int GetOutputIndex();
    //��ȡ���list
    QMap<int, QList<CFdbOrderBaseItem*> > GetOutPutList();
    //��ȡ����ĸ���
    int GetInputNums();
    //��ȡ�������
    int GetOutputNums();
    //��������item
    void InsertInputItem(int nIndex, CFdbOrderBaseItem* mapItem);
    //�������item
    void InsertOuputItem(int nIndex,CFdbOrderBaseItem* pItem);
    //ɾ�������item
    void DeleteInputItem(CFdbOrderBaseItem* pItem);
    //ɾ�����������item
    void DeleteOneInputItem(CFdbOrderBaseItem* pItem, int nIndex);
    //ɾ��output
    void DeleteOutputItem(CFdbOrderBaseItem* pItem);
    //ɾ��һ��output
    void DeleteOneOutputItem(CFdbOrderBaseItem* pItem,int nIndex);
    //pin��Ӧ��ϵ����
    void InsertPins(int nEndIndex,int nBeginIndex);
    //��ȡǰ������
    QMap<int, CFdbOrderBaseItem*> GetInputItems();
    //�����������߶�
    void SetMaxInputHeight(int nMaxHeight);
    //��ȡ�������
    QList<QPointF> GetInputLines(int nInputIndex);
    //
    //��ȡ�����scene��
    QRectF GetSceneRectf();
    //������ȸ���
    void SetInputNum();
    //��
    int MinusInputNum();
    //
    void MoveItemFromKey(double disHeight,double disWidth);
    //
    bool GetEnoFlag();
    //��ʾeno��Ϣ
    void ShowEnoItems();
    //�ر�eno��Ϣ
    void CloseEnoItems();
    //��ȡע����Ϣ
    const QString& GetCommentData();
    //��ȡitemtype��� 
    int GetItemTypNum();
    //���������������ֵ
    void SetInputMaxNum(int nIndex);
    //��ȡ�����������ֵ
    int GetInputMaxNum();
    //��������������ֵ
    void SetOutPutMaxNum(int nIndex);
    //��ȡ����������ֵ
    int GetOutPutMaxNum();
    //��������ܽ�����
    void SetInputPinNames(const QStringList &lstNames);
    //��������ܽ�����
    void SetOutputPinNames(const QStringList &lstNames);
    //��ȡmap
    QMap<int, int> GetBinMapInfo();
    //remove pin in
    void RemoveOneInputPin(int nPinIndex);
    //add bin text
    void AddTextBinInfo();
    //����״̬��ɫ
    void SetStatusColor(const QColor &statusColor);
    //
    QList<CConnectionRouter*> GetAllLines();
    //
    void SetCurrentInputNum(int nInputNum);
    //
    int GetCurrentInpusNum();
    //����EN��Ϣ
    void SetEnData(int nEnData);
    int GetEnData();
    //����ENO
    void SetEnoData(int nEnoData);
    int GetEnoData();

public slots:
    void Slot_DeleteAct();
    //
    void Slot_UpdateComment(const QString &strComment);
    //����eno��Ϣ
    void Slot_UpdateEnoInfo(int nFlag);
	//���°���Ϣֵ
	void Slot_updateBindValueInfo(QMap<int,QString>& mapBindValue);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
public:
    //�����������㴦
    int m_nInputIndex;
    //������������
    int m_nOutputIndex;
    //�������߶�
    int m_nInsertMaxHeight;
    //������߶�
    int m_nOutMaxHeight;
    //��ǰѡ�е�ĳ���
    int  m_nCurrentWidth;
    //��ǰѡ�е�ĸ߶�
    int m_nCurrentHeight;
    //���ߵ�״̬
    int m_nItemStatus;
    //���������Ϣ
    void SetOutputData(int nOutPinNum,int nValue);
    //��ȡ�����Ϣ
    QMap<int,int> GetOutputData();

private:
    QColor m_ItemColor;
    //item���� AND OR ....
    QString m_strItemType;
    //�߼���
    QList<CConnectionRouter*>  m_polyLines;
    //��С
    QRectF m_rect;
    QCursor* m_cursor;
    //����  ����1  ����2
    int m_nChangeSize;
    //������ʼ��
    QPointF m_nMoveBge;
    //����������
    QPointF m_nMoveEnd;
    bool m_bScanl;
    //�Ƿ����
    bool m_bChangeFlag;
    //���������
    QPointF m_pfClickPoint;
    //keyress
    QPointF m_pfKeyPoint;
    //ִ�б��
    int m_nIndexNumber;
    //�������
    int m_nCreateNum;
    //ǰ   �����ź�item
    QMap<int, CFdbOrderBaseItem*>  m_mapPreInfos;
    //��  �����ź�item
    QMap<int, QList<CFdbOrderBaseItem*>> m_mapNextInfos;
    //���
    int m_nInputNums;
    //eno����
    bool m_bEnoFlag;
    //ע�Ͷ���
    QGraphicsTextItem *m_pCommentTextItem;
    //�Ƿ���ʾeno items
    //ע������
    QString m_strComment;
    //itemtype  num
    int m_nItemTypeNum;
    //���ֵ
    int m_nInputMaxNum;
    //output ���ֵ
    int m_nOutMaxNum;
    //����˶�����Ϣ
    std::vector<std::shared_ptr<CBinProperty>> m_vInputPros;

    //����˶�����Ϣ
    std::vector<std::shared_ptr<CBinProperty>> m_vOutputPros;

    //�����б������
    QStringList m_lstInputPin;
    //����б�����
    QStringList m_lstOutputPin;
    //item���߶�
    int m_nItemMaxHeight;
    //�������->�����������
    QMap<int, int> m_mapPins;
    //��ǰ��Ҫ���ĵ�
    QPointF m_pfPinText;
    //����
    QMap<int, QGraphicsTextItem*> m_mapPinIndexText;
    //���
    QMap<int, QGraphicsTextItem*> m_mapOutPinIndexText;
    //����״̬�任��ɫ
    QColor m_colorStatus;
    //��������Ӹ���
    int m_nCurrentInputNums;
    //��������Ӹ���
    int m_nCurrentOutputNums;
    //EN ��Ϣ 
    int m_nEnFlag;
    //ENO ��Ϣ
    int m_nEnoFlag;
    //�������  ��  ���ֵ
    QMap<int, int> m_mapOutput;
};

#endif // PLCORDERBASEITEM_H
