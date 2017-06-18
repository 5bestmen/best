#include <QPainter>
#include <QPainterPath>
#include <QLine>
#include <QDebug>
#include "lcddisplaywgt.h"

CLcdDisplayWgt::CLcdDisplayWgt(QWidget *parent)
    : QWidget(parent)
{
    //从上到下  从左到右
    for (int i = 0; i < 12; i++)
    {
        m_MapLightTColor.insert(i, 1);
    }

    m_MapColorIndex.insert(1, QColor(242,242,242));
    m_MapColorIndex.insert(2, QColor(90,170,21));
    m_MapColorIndex.insert(3, QColor(250,206,32));
    m_MapColorIndex.insert(4, QColor(228,30,37));

    m_strDeviceVersion = tr("DAS800-D");

    m_strDeviceName = tr("Terminal");

    //setMinimumHeight(600);

    m_nRowCursor = 0;
    m_nColumnCursor = 0;

//      char xx[241] = "wjfdklsajfl efwojf fdsal fdsajfl fdsaofjo fdsalfjl  王维 fdsalj fasll fdsalfj fdsafl fdsfds fds wef few fdsfs grgr fsda";
//      UpdateLcdDisplayData(QByteArray(xx,241),1,1);

}

CLcdDisplayWgt::~CLcdDisplayWgt()
{

}

void CLcdDisplayWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(0, 0, this->width(), this->height(), QColor(39,46,43));

    painter.fillRect(this->width() / 6.0, this->height() / 6.0, this->width() * 2 / 3.0, this->height() * 3 / 6.0,QColor(128,128,255));

    //版本
//     painter.fillRect(30,10,90,25,QColor(157,119,6));
    painter.setPen(QPen(QColor(150, 158, 151)));
//     
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
// 
// 
//     painter.drawText(QRectF(33, 12, 84, 21), m_strDeviceVersion);
//     //配电终端
    font.setPixelSize(15);
    font.setBold(true);
    painter.setFont(font);

    painter.drawText(QRectF(this->width()-120,12,90,21),m_strDeviceName);

    QFont font2("SimHei");
    painter.setFont(font2);
    //直线
    painter.setPen(QPen(QColor(150,158,151)));
    painter.drawLine(QLine(this->width() / 6.0, this->height() * 5 / 6.0, this->width() * 5 / 6.0,this->height()* 5 / 6.0));

    //写字
    
    for (int i = 0; i < 6; i++)
    {
        QPoint tPoint1;
        tPoint1.setX(this->width() / 6.0 + this->width() * 2 / 3.0 / 6 * i + 10);
        tPoint1.setY(this->height() * 5 / 6.0 -30);

        QPoint tPoint2;
        tPoint2.setX(this->width() / 6.0 + this->width() * 2 / 3.0 / 6 * i + 10);
        tPoint2.setY(this->height() * 5 / 6.0 + 30);

        QPoint tPointL1;
        tPointL1.setX(this->width() / 6.0 + this->width() * 2 / 3.0 / 6 * i + 15);
        tPointL1.setY(this->height() * 5 / 6.0 - 10);

        QPoint tPointL2;
        tPointL2.setX(this->width() / 6.0 + this->width() * 2 / 3.0 / 6 * i + 15);
        tPointL2.setY(this->height() * 5 / 6.0 + 10);

        if (i == 0)
        {
            painter.setPen(QPen(QColor(150, 158, 151)));
            painter.drawText(tPoint1,tr("Run"));
            painter.drawText(tPoint2, tr("L1Error"));

        }
        else if (i == 1)
        {
            painter.setPen(QPen(QColor(150, 158, 151)));
            painter.drawText(tPoint1, tr("Warn"));
            painter.drawText(tPoint2, tr("L2Error"));
        }
        else if (i == 2)
        {
            painter.setPen(QPen(QColor(150, 158, 151)));
            painter.drawText(tPoint1, tr("Ctrl"));
            painter.drawText(tPoint2, tr("L3Error"));
        }
        else if (i == 3)
        {
            painter.setPen(QPen(QColor(150, 158, 151)));
            painter.drawText(tPoint1, tr("Remote"));
            painter.drawText(tPoint2, tr("L4Error"));
        }
        else if (i == 4)
        {
            painter.setPen(QPen(QColor(150, 158, 151)));
            painter.drawText(tPoint1, tr("Dischg"));
            painter.drawText(tPoint2, tr("L5Error"));
        }
        else if (i == 5)
        {
            painter.setPen(QPen(QColor(150, 158, 151)));
            painter.drawText(tPoint1, tr("BatErr"));
            painter.drawText(tPoint2, tr("L6Error"));
        }

        painter.setBrush(m_MapColorIndex[m_MapLightTColor[i]]);
        painter.setPen(m_MapColorIndex[m_MapLightTColor[i]]);
        painter.drawEllipse(tPointL1, 5, 5);

        painter.setBrush(m_MapColorIndex[m_MapLightTColor[i + 6]]);
        painter.setPen(m_MapColorIndex[m_MapLightTColor[i + 6]]);
        painter.drawEllipse(tPointL2, 5, 5);

    }

    //写入数据
    painter.setPen(Qt::white);
    font.setPixelSize(16);
    painter.setFont(font);

//     for (int i = 0; i < 8; i++)
//     {
//         painter.drawText(QRectF(this->width() / 6.0, this->height() / 6.0 + i*20, this->width() * 2 / 3.0, this->height() * 3 / 6.0), Qt::AlignTop, m_strDetailInfo.mid(i*30,30));
//     }

    if (m_btDetailInfo.length() < 200)
    {
        return;
    }

    for (int i = 0; i < 8; i++)
    {
        if (m_btDetailInfo.length() < i * 30 + 30)
        {
            break;
        }
        QByteArray btLineData = m_btDetailInfo.mid(i*30,30);

        QString strLineData = QString::fromLocal8Bit(m_btDetailInfo.mid(i * 30, 30));

        for (int j = 0; j < 30; j++)
        {

            if (((unsigned char)btLineData.at(j) >= 0xB0) && (j < 29) && ((unsigned char)btLineData.at(j + 1) >= 0xA1))
            {
                //汉字
                QByteArray bb = btLineData.mid(j, 2);
                QString strChText = QString::fromLocal8Bit(btLineData.mid(j, 2));

                painter.drawText(QRectF(5 + this->width() / 6.0 + this->width()* 2.0 / 3 / 30 * j, 3+this->height() / 6.0 + this->height() / 2.0 / 8 * i, 15, 20), Qt::AlignTop, strChText);

                if (i == m_nRowCursor && j == m_nColumnCursor)
                {
                    painter.fillRect(QRectF(5 + this->width() / 6.0 + this->width()* 2.0 / 3 / 30 * j - 2, 3+this->height() / 6.0 + this->height() / 2.0 / 8 * i, 17, 20), QColor(0, 0, 255, 100));
                }

                j++;
            }
            else
            {
                //普通字符
                painter.drawText(QRectF(5+this->width() / 6.0 + this->width()* 2.0 / 3 / 30 * j, 3+this->height() / 6.0 + this->height()/2.0/8*i, 15, 20), Qt::AlignTop, QString(btLineData.at(j)));

                if (i == m_nRowCursor && j == m_nColumnCursor)
                {
                    painter.fillRect(QRectF(5+this->width() / 6.0 + this->width()* 2.0 / 3 / 30 * j - 2, 3+this->height() / 6.0 + this->height() / 2.0 / 8 * i, 10, 20), QColor(0, 0, 255, 100));
                }
            }

        }

    }

//     m_pfCursor = QPoint(this->width() / 6.0 + 10, this->height() / 6.0+3);
// 
//     painter.fillRect(QRectF(m_pfCursor.x(), m_pfCursor.y(), 10, 20), QColor(0, 0, 255,100));

}

void CLcdDisplayWgt::UpdateLcdDisplayData(const QByteArray &btData, int nRow, int nColumn)
{
    m_btDetailInfo = btData;

    m_nRowCursor = nRow;
    m_nColumnCursor = nColumn;

    update();
}

void CLcdDisplayWgt::UpdateLEDLight(int nIndex, int nColor)
{
    m_MapLightTColor.insert(nIndex,nColor);
}

