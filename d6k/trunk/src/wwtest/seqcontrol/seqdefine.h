#ifndef SEQDEFINE
#define SEQDEFINE
#include <QRectF>

enum PROJECTITEMTYPE
{
    PROJECT = 1000,        //程序
    TASK = 1100,           //任务
    MAST = 1200,            //mast
    MASTSEG = 1300,         //主任务段
    MASTSRSEG = 1400,      //字程序段

    FAST = 1500,          //快速任务
    FASTSEG = 1600,       //快速任务段
    FASTSRSEG = 1700,     //快速任务子程序

    EVENT = 1800,          //事件
    TIMEEVENT = 1900,       //定时事件
    IOEVENT = 2000,          //i/o事件

    AUX0 = 2100,          //辅助任务
    AUX0SEG = 2200,       //辅助任务段
    AUX0SRSEG = 2300,       //辅助任务子程序

    FDBORDER = 3000,         //PLC命令

    EDITITEM = 4000          //可编辑的item

};
enum BITTYPE
{
    //位指令
    BITAND = 100,     //逻辑与
    BITOR,      //逻辑或
    BITNOT,     //逻辑not
    BITXOR,     //逻辑异或
    //位移指令
    BITSHL,      //左移
    BITSHR,      //右移
    BITROL,        //左移循环
    BITROR,      //右移循环
    BITBTST,         //位测试
    BITBSET,     //置位
    BITCLR,       //清位
    //数学指令
    MATHMOVE,   //赋值
    MATHADD,    //加法
    MATHSUB,    //减法
    MATHMUL,    //乘法
    MATHDIV,     //除法
    MATHMOD,     //求余
    MATHSQRT,   //平方根
    MATHABS,    //
    MATHLOG,     //十为底的对数
    MATHLN,     //自然对数
    MATHEXP,   //自然指数
    MATHEXPT,   //指数
    MATHSIN,   //正弦
    MATHCOS,   //余弦
    MATHTAN,  //正切
    MATHASIN,   //反正弦
    MATHACOS,   //反余弦
    MATHATAN,   //反正切
    MATHMAX,   //最大值
    MATHMIN,    //最小值
    //比较指令
    COMPAREEQ,  //等于
    COMPAREGE,   //大于或等于
    COMPAREGT,   //大于
    COMPARELE,   //小于或等于
    COMPARELT,   //小于
    COMPARENE,   //不等于
    //高级功能
    CALLSR,    //子程序
    GTOT,          //跳转
    LABEL,    //标号
    PID,     //调节pid
    //定时器
    TIMERCTD,   //减计数器
    TIMERCTU,   //加计数器
    TIMERCTUD,   //加/减计数器
    TIMERTOF,   //关闭延迟
    TIMERTON,   //接通延迟
    TIMERTP,    //脉冲
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
     CURSORARROW = 1, //鼠标正常状态
     CURSORLINE = 2   //连线状态
};

enum ITEMTYPE
{
    LINEITEM = 1,  //当前控件为直线
    OBJECTITEM,     //当前控件为实际对象
    COMMENTITEM = 8,     //注释控件  QGraphicsTextItem固定为8
    GRAPHICSCOMMENT = 9  //GRAPHICSA注释
};

enum ITEMIOTYPE
{
    IOERROR = -1,    //无效
    IOINPUT = 1,     //输入端
    IOOUTPUT = 2     //输出端
};

struct OUTITEMINFO
{
    int nCreateNum;             //创建编号
    int nExecNum;               //执行编号
    int nOutPutIndex;           //输出编号
    QString strOutPutPinName;   //输出bin名称
};

struct CELLINFO
{
    QString strCell;
    QString strInputType;     //元件类型
    int nElement;             //元件编号
    QString strChangeAddr;    //变址指针
    int nStatus;              //开关状态
};

struct LOGICITEMPRO
{
    int nPinType;              //端子类型    1:output   2:元件   3:空

    bool bEnFlag;             //是否显示EN 信息

    OUTITEMINFO outPinInfo;    //对端输出信息

    int nIndex;               //端子编号
    QString strPinName;       //端子名称    

    CELLINFO sCellInfo;       //元件信息 

    QString strComment;
};



struct ITEMINFO
{
    int nItemType;
    QRectF itemRect;                  //item  RECT
    QString strItemComment;          //item  注释
    int nCreateNum;                  //item创建编号
    int nExecNum;                    //item执行编号
    bool bEx;                        //是否有ex信息

    QList<LOGICITEMPRO> lstInInfo;   //item  输入端
};

struct LINEROUNTER
{
    QString strBgeItem;            //起始item
    QString strEndItem;            //结束item
    QString strEndIndex;           //ln?
    //add by wangwei
    QString strBeginIndex;         //out?
    QList<QPointF> lstAllPoints;   //所有点的集合
};

//注释
struct COMMENTDATA
{
    QPointF pfPos;           //坐标位置
    QString strComment;      //注释内容
    QRectF  rfRect;          //rect
};

struct GRAHPICDATA
{
    QList<ITEMINFO> itemData;
    QList<LINEROUNTER> lineData;
    QList<COMMENTDATA> commentData;
};

#endif // CPOLYLINE_H
