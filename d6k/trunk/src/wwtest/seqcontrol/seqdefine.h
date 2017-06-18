#ifndef SEQDEFINE
#define SEQDEFINE
#include <QRectF>

enum PROJECTITEMTYPE
{
    PROJECT = 1000,        //����
    TASK = 1100,           //����
    MAST = 1200,            //mast
    MASTSEG = 1300,         //�������
    MASTSRSEG = 1400,      //�ֳ����

    FAST = 1500,          //��������
    FASTSEG = 1600,       //���������
    FASTSRSEG = 1700,     //���������ӳ���

    EVENT = 1800,          //�¼�
    TIMEEVENT = 1900,       //��ʱ�¼�
    IOEVENT = 2000,          //i/o�¼�

    AUX0 = 2100,          //��������
    AUX0SEG = 2200,       //���������
    AUX0SRSEG = 2300,       //���������ӳ���

    FDBORDER = 3000,         //PLC����

    EDITITEM = 4000          //�ɱ༭��item

};
enum BITTYPE
{
    //λָ��
    BITAND = 100,     //�߼���
    BITOR,      //�߼���
    BITNOT,     //�߼�not
    BITXOR,     //�߼����
    //λ��ָ��
    BITSHL,      //����
    BITSHR,      //����
    BITROL,        //����ѭ��
    BITROR,      //����ѭ��
    BITBTST,         //λ����
    BITBSET,     //��λ
    BITCLR,       //��λ
    //��ѧָ��
    MATHMOVE,   //��ֵ
    MATHADD,    //�ӷ�
    MATHSUB,    //����
    MATHMUL,    //�˷�
    MATHDIV,     //����
    MATHMOD,     //����
    MATHSQRT,   //ƽ����
    MATHABS,    //
    MATHLOG,     //ʮΪ�׵Ķ���
    MATHLN,     //��Ȼ����
    MATHEXP,   //��Ȼָ��
    MATHEXPT,   //ָ��
    MATHSIN,   //����
    MATHCOS,   //����
    MATHTAN,  //����
    MATHASIN,   //������
    MATHACOS,   //������
    MATHATAN,   //������
    MATHMAX,   //���ֵ
    MATHMIN,    //��Сֵ
    //�Ƚ�ָ��
    COMPAREEQ,  //����
    COMPAREGE,   //���ڻ����
    COMPAREGT,   //����
    COMPARELE,   //С�ڻ����
    COMPARELT,   //С��
    COMPARENE,   //������
    //�߼�����
    CALLSR,    //�ӳ���
    GTOT,          //��ת
    LABEL,    //���
    PID,     //����pid
    //��ʱ��
    TIMERCTD,   //��������
    TIMERCTU,   //�Ӽ�����
    TIMERCTUD,   //��/��������
    TIMERTOF,   //�ر��ӳ�
    TIMERTON,   //��ͨ�ӳ�
    TIMERTP,    //����
};

enum INOUTMAXNUM
{
    INOUTMAX0 = 0,
    INOUTMAX1 = 1,
    INOUTMAX2 = 2,
    INOUTMAX3 = 3,
    INOUTMAX4 = 4,
    INOUTMAX5 = 5,
    INOUTMAX6 = 6,
    INOUTMAX7 = 7,
    INOUTMAX16 = 16,
};

enum CURSORSTATUS
{
     CURSORARROW = 1, //�������״̬
     CURSORLINE = 2   //����״̬
};

enum ITEMTYPE
{
    LINEITEM = 1,  //��ǰ�ؼ�Ϊֱ��
    OBJECTITEM,     //��ǰ�ؼ�Ϊʵ�ʶ���
    COMMENTITEM = 8,     //ע�Ϳؼ�  QGraphicsTextItem�̶�Ϊ8
    GRAPHICSCOMMENT = 9  //GRAPHICSAע��
};

enum ITEMIOTYPE
{
    IOERROR = -1,    //��Ч
    IOINPUT = 1,     //�����
    IOOUTPUT = 2     //�����
};

struct OUTITEMINFO
{
    int nCreateNum;             //�������
    int nExecNum;               //ִ�б��
    int nOutPutIndex;           //������
    QString strOutPutPinName;   //���bin����
};

struct CELLINFO
{
    QString strCell;
    QString strInputType;     //Ԫ������
    int nElement;             //Ԫ�����
    QString strChangeAddr;    //��ַָ��
    int nStatus;              //����״̬
};

struct LOGICITEMPRO
{
    int nPinType;              //��������    1:output   2:Ԫ��   3:��

    bool bEnFlag;             //�Ƿ���ʾEN ��Ϣ

    OUTITEMINFO outPinInfo;    //�Զ������Ϣ

    int nIndex;               //���ӱ��
    QString strPinName;       //��������    

    CELLINFO sCellInfo;       //Ԫ����Ϣ 

    QString strComment;
};



struct ITEMINFO
{
    int nItemType;
    QRectF itemRect;                  //item  RECT
    QString strItemComment;          //item  ע��
    int nCreateNum;                  //item�������
    int nExecNum;                    //itemִ�б��
    bool bEx;                        //�Ƿ���ex��Ϣ

    QList<LOGICITEMPRO> lstInInfo;   //item  �����
};

struct LINEROUNTER
{
    QString strBgeItem;            //��ʼitem
    QString strEndItem;            //����item
    QString strEndIndex;           //ln?
    //add by wangwei
    QString strBeginIndex;         //out?
    QList<QPointF> lstAllPoints;   //���е�ļ���
};

//ע��
struct COMMENTDATA
{
    QPointF pfPos;           //����λ��
    QString strComment;      //ע������
    QRectF  rfRect;          //rect
};

struct GRAHPICDATA
{
    QList<ITEMINFO> itemData;
    QList<LINEROUNTER> lineData;
    QList<COMMENTDATA> commentData;
};

#endif // CPOLYLINE_H
