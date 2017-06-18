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
    LOGIC_PROJECT = 1000,        //����
    LOGIC_TAG = 1050,      //tag
    LOGIC_TASK = 1100,           //����

    LOGIC_MAST = 1200,            //mast
    LOGIC_MASTSEG = 1300,         //�������
    LOGIC_MASTSRSEG = 1400,      //�ֳ����

    LOGIC_FAST = 1500,          //��������
    LOGIC_FAST_SEG = 1600,       //���������
    LOGIC_FAST_SRSEG = 1700,     //���������ӳ���

    LOGIC_EVENT = 1800,          //�¼�
    LOGIC_TIME_EVENT = 1900,       //��ʱ�¼�
    IOEVENT = 2000,          //i/o�¼�

    LOGIC_AUX0 = 2100,          //��������
    LOGIC_AUX0SEG = 2200,       //���������
    LOGIC_AUX0_SRSEG = 2300,       //���������ӳ���

    LOGIC_FDB_ORDER = 3000,         //PLC����

    LOGIC_EDIT_ITEM = 4000          //�ɱ༭��item

};
enum LOGIC_ITEM_TYPE
{
    //λָ��
    LOGIC_AND = 100,     //�߼���
    LOGIC_OR,      //�߼���
    LOGIC_NOT,     //�߼�not
    LOGIC_XOR,     //�߼����
    //λ��ָ��
    LOGIC_SHL,      //����
    LOGIC_SHR,      //����
    LOGIC_ROL,        //����ѭ��
    LOGIC_ROR,      //����ѭ��
    LOGIC_BTST,         //λ����
    LOGIC_BSET,     //��λ
    LOGIC_CLR,       //��λ
    //��ѧָ��
    LOGIC_MATH_MOVE,   //��ֵ
    LOGIC_MATH_ADD,    //�ӷ�
    LOGIC_MATH_SUB,    //����
    LOGIC_MATH_MUL,    //�˷�
    LOGIC_MATH_DIV,     //����
    LOGIC_MATH_MOD,     //����
    LOGIC_MATH_SQRT,   //ƽ����
    LOGIC__MATH_ABS,    //
    LOGIC_MATH_LOG,     //ʮΪ�׵Ķ���
    LOGIC__MATH_LN,     //��Ȼ����
    LOGIC_MATH_EXP,   //��Ȼָ��
    LOGIC_MATH_EXPT,   //ָ��
    LOGIC_MATH_SIN,   //����
    LOGIC_MATH_COS,   //����
    LOGIC_MATH_TAN,  //����
    LOGIC_MATH_ASIN,   //������
    LOGIC_MATH_ACOS,   //������
    LOGIC_MATH_ATAN,   //������
    LOGIC_MATH_MAX,   //���ֵ
    LOGIC_MATH_MIN,    //��Сֵ
    //�Ƚ�ָ��
    LOGIC_COMPARE_EQ,  //����
    LOGIC_COMPARE_GE,   //���ڻ����
    LOGIC_COMPARE_GT,   //����
    LOGIC_COMPARE_LE,   //С�ڻ����
    LOGIC_COMPARE_LT,   //С��
    LOGIC_COMPARE_NE,   //������
    //�߼�����
    LOGIC_CALLSR,    //�ӳ���
    LOGIC_GTOT,          //��ת
    LOGIC_LABEL,    //���
    LOGIC_PID,     //����pid
    //��ʱ��
    LOGIC_TIMERCTD,   //��������
    LOGIC_TIMER_CTU,   //�Ӽ�����
    LOGIC_TIMER_CTUD,   //��/��������
    LOGIC_TIMER_TOF,   //�ر��ӳ�
    LOGIC_TIMER_TON,   //��ͨ�ӳ�
    LOGIC_TIMER_TP,    //����
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
     CURSOR_ARROW = 1, //�������״̬
     CURSOR_LINE = 2   //����״̬
};

enum ITEM_TYPE
{
    LINE_ITEM = 1,  //��ǰ�ؼ�Ϊֱ��
    OBJEC_TITEM,     //��ǰ�ؼ�Ϊʵ�ʶ���
    COMMENT_ITEM = 8,     //ע�Ϳؼ�  QGraphicsTextItem�̶�Ϊ8
    GRAPHICS_COMMENT = 9  //GRAPHICSAע��
};

enum ITEM_IO_TYPE
{
    IO_ERROR = -1,    //��Ч
    IO_INPUT = 1,     //�����
    IO_OUTPUT = 2     //�����
};



#endif // CPOLYLINE_H
