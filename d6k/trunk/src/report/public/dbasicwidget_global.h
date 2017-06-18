#ifndef DBASICWIDGET_GLOBAL_H
#define DBASICWIDGET_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QByteArray>
#include <QString>
#include <QObject>

#if defined(DBASICWIDGET_LIBRARY)
#  define DBASICWIDGETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DBASICWIDGETSHARED_EXPORT Q_DECL_IMPORT
#endif

#define c_RGB(r,g,b) ((quint32)(((quint8)(r)|((quint16)((quint8)(g))<<8)|((quint32)((quint8)(b))<<16))))

#define b_Q(bytes) QString::fromLocal8Bit(bytes)
#define q_B(str) (QByteArray)(str).toLocal8Bit()

#define r_RGB(rgb)	((quint8)(rgb))
#define g_RGB(rgb)  ((quint8)(((quint16)rgb)>>8))
#define b_RGB(rgb)	((quint8)((rgb)>>16))

#define NULLWIDGET      -3
#define BRUSH_COLOR		QColor(145,204,225,255)

static struct
{
    quint32    color;
    QByteArray    name;

} RGB[]=
{
    {c_RGB(0xFF,0xFF,0xFF),QObject::tr("白色的啊").toLocal8Bit()},
    {c_RGB(0xFF,0x00,0x00),"红色"},
    {c_RGB(0xFF,0xFF,0x99),"浅黄"},
    {c_RGB(0xFF,0xFF,0x00),"黄色"},
    {c_RGB(0xFF,0xCC,0x99),"棕黄"},
    {c_RGB(0xFF,0xCC,0x00),"金黄"},
    {c_RGB(0xFF,0x99,0xCC),("玫红")},
    {c_RGB(0xFF,0x99,0x00),"浅桔"},
    {c_RGB(0xFF,0x66,0x00),"桔黄"},
    {c_RGB(0xFF,0x00,0xFF),"粉红"},
    {c_RGB(0xD4,0xD0,0xC0),"亮灰"},
    {c_RGB(0xCC,0xFF,0xFF),"浅青"},
    {c_RGB(0xCC,0xFF,0xCC),"浅绿"},
    {c_RGB(0xCC,0x99,0xFF),"淡紫"},
    {c_RGB(0xC0,0xC0,0xC0),"银灰"},
    {c_RGB(0x99,0xCC,0xFF),"淡蓝"},
    {c_RGB(0x99,0xCC,0x00),"酸橙"},
    {c_RGB(0x99,0x99,0x99),"浅灰"},
    {c_RGB(0x99,0x30,0x66),"梅红"},
    {c_RGB(0x80,0x80,0x80),"灰色"},
    {c_RGB(0x80,0x80,0x00),"深黄"},
    {c_RGB(0x80,0x00,0x80),"紫色"},
    {c_RGB(0x80,0x00,0x00),"深红"},
    {c_RGB(0x66,0x66,0x99),"灰蓝"},
    {c_RGB(0x40,0x40,0x40),"暗灰"},
    {c_RGB(0x33,0x99,0x66),"海绿"},
    {c_RGB(0x33,0x66,0xFF),"浅蓝"},
    {c_RGB(0x33,0x66,0xCC),"宝蓝"},
    {c_RGB(0x33,0x33,0x99),"靛蓝"},
    {c_RGB(0x33,0x33,0x00),"橄绿"},
    {c_RGB(0x00,0xFF,0xFF),"蓝绿"},
    {c_RGB(0x00,0xFF,0x00),"浅绿"},
    {c_RGB(0x00,0xCC,0xFF),"天蓝"},
    {c_RGB(0x00,0x80,0x80),"深青"},
    {c_RGB(0x00,0x80,0x00),"绿色"},
    {c_RGB(0x00,0x33,0x66),"灰蓝"},
    {c_RGB(0x00,0x33,0x00),"深绿"},
    {c_RGB(0x00,0x00,0xFF),"蓝色"},
    {c_RGB(0x00,0x00,0x80),"深蓝"},
    {c_RGB(0x00,0x00,0x00),"黑色"}
};

enum EPenOption
{
	P_PEN_STYLE,
	P_PEN_WIDTH
};

enum HoverStatus
{
    H_NORMAL,
    H_PUSHED,
    H_NORMALHOT,
    H_PUSHEDHOT
};

enum ActionType
{
    A_NULL,
    A_ROTATEANGLE,
    A_ROTATELEFT,
    A_ROTATERIGHT,
    A_ROTATEHORZ,
    A_ROTATEVERT,
    A_ZOOMIN,
    A_ZOOMOUT,
    A_ZOOMNORMAL,
    A_ZOOMAUTOSIZE,
    A_ZOOMRECT,
    A_ALIGNLEFT,
    A_ALIGNRIGHT,
    A_ALIGNTOP,
    A_ALIGNBOTTOM,
    A_ALIGNCENTERH,
    A_ALIGNCERTERV,
    A_ALIGNINTERVALH,
    A_ALIGNINTERVALV,
    A_ALIGNAEQUILATE,
    A_ALIGNALTITUDE,
    A_ALIGNBOTH,
	A_GROUP,
	A_UNGROUP,
	A_ADD_LAYER,
	A_DELETE_LAYER,
	A_MOVE_TO_LAYER
};
#endif // DBASICWIDGET_GLOBAL_H
