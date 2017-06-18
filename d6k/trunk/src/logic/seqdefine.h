#ifndef SEQDEFINE
#define SEQDEFINE
#include <QRectF>

#define  LOGICFILE     "/logic/logic.xml"
#define  LOGIC_PATH    "/logic"

#define  LOGICMAST     "/mast"
#define  LOGICMASTSEG     "/mast/seg"
#define  LOGICMASTSR     "/mast/sr"


#define  LOGICFAST     "/fast"
#define  LOGICFASTSEG     "/fast/seg"
#define  LOGICFASTSR     "/fast/sr"

#define  LOGICAUX0     "/aux0"
#define  LOGICAUX0SEG     "/aux0/seg"
#define  LOGICAUX0SR     "/aux0/sr"




enum LOGIC_PROJECT_ITEM_TYPE
{
    LOGIC_PROJECT = 1000,        //程序
    LOGIC_TAG = 1050,      //tag
    LOGIC_TASK = 1100,           //任务

    LOGIC_MAST = 1200,            //mast
    LOGIC_MASTSEG = 1300,         //主任务段
    LOGIC_MASTSRSEG = 1400,      //字程序段

    LOGIC_FAST = 1500,          //快速任务
    LOGIC_FAST_SEG = 1600,       //快速任务段
    LOGIC_FAST_SRSEG = 1700,     //快速任务子程序

    LOGIC_EVENT = 1800,          //事件
    LOGIC_TIME_EVENT = 1900,       //定时事件
    IOEVENT = 2000,          //i/o事件

    LOGIC_AUX0 = 2100,          //辅助任务
    LOGIC_AUX0SEG = 2200,       //辅助任务段
    LOGIC_AUX0_SRSEG = 2300,       //辅助任务子程序

    LOGIC_FDB_ORDER = 3000,         //PLC命令

    LOGIC_EDIT_ITEM = 4000          //可编辑的item

};
enum LOGIC_ITEM_TYPE
{
    //位指令
    LOGIC_AND = 100,     //逻辑与
    LOGIC_OR,      //逻辑或
    LOGIC_NOT,     //逻辑not
    LOGIC_XOR,     //逻辑异或
    //位移指令
    LOGIC_SHL,      //左移
    LOGIC_SHR,      //右移
    LOGIC_ROL,        //左移循环
    LOGIC_ROR,      //右移循环
    LOGIC_BTST,         //位测试
    LOGIC_BSET,     //置位
    LOGIC_CLR,       //清位
    //数学指令
    LOGIC_MATH_MOVE,   //赋值
    LOGIC_MATH_ADD,    //加法
    LOGIC_MATH_SUB,    //减法
    LOGIC_MATH_MUL,    //乘法
    LOGIC_MATH_DIV,     //除法
    LOGIC_MATH_MOD,     //求余
    LOGIC_MATH_SQRT,   //平方根
    LOGIC__MATH_ABS,    //
    LOGIC_MATH_LOG,     //十为底的对数
    LOGIC__MATH_LN,     //自然对数
    LOGIC_MATH_EXP,   //自然指数
    LOGIC_MATH_EXPT,   //指数
    LOGIC_MATH_SIN,   //正弦
    LOGIC_MATH_COS,   //余弦
    LOGIC_MATH_TAN,  //正切
    LOGIC_MATH_ASIN,   //反正弦
    LOGIC_MATH_ACOS,   //反余弦
    LOGIC_MATH_ATAN,   //反正切
    LOGIC_MATH_MAX,   //最大值
    LOGIC_MATH_MIN,    //最小值
    //比较指令
    LOGIC_COMPARE_EQ,  //等于
    LOGIC_COMPARE_GE,   //大于或等于
    LOGIC_COMPARE_GT,   //大于
    LOGIC_COMPARE_LE,   //小于或等于
    LOGIC_COMPARE_LT,   //小于
    LOGIC_COMPARE_NE,   //不等于
    //高级功能
    LOGIC_CALLSR,    //子程序
    LOGIC_GTOT,          //跳转
    LOGIC_LABEL,    //标号
    LOGIC_PID,     //调节pid
    //定时器
    LOGIC_TIMERCTD,   //减计数器
    LOGIC_TIMER_CTU,   //加计数器
    LOGIC_TIMER_CTUD,   //加/减计数器
    LOGIC_TIMER_TOF,   //关闭延迟
    LOGIC_TIMER_TON,   //接通延迟
    LOGIC_TIMER_TP,    //脉冲
};

enum INOUT_MAX_NUM
{
    INOUT_MAX0 = 0,
    INOUT_MAX1 = 1,
    INOUT_MAX2 = 2,
    INOUT_MAX3 = 3,
    INOUT_MAX4 = 4,
    INOUT_MAX5 = 5,
    INOUT_MAX6 = 6,
    INOUT_MAX7 = 7,
    INOUT_MAX16 = 16,
};

enum CURSOR_STATUS
{
     CURSOR_ARROW = 1, //鼠标正常状态
     CURSOR_LINE = 2   //连线状态
};

enum ITEM_TYPE
{
    LINE_ITEM = 1,  //当前控件为直线
    OBJEC_TITEM,     //当前控件为实际对象
    COMMENT_ITEM = 8,     //注释控件  QGraphicsTextItem固定为8
    GRAPHICS_COMMENT = 9  //GRAPHICSA注释
};

enum ITEM_IO_TYPE
{
    IO_ERROR = -1,    //无效
    IO_INPUT = 1,     //输入端
    IO_OUTPUT = 2     //输出端
};



#endif // CPOLYLINE_H
