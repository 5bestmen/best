#ifndef LCDDISPLAYWGT_H
#define LCDDISPLAYWGT_H

#include <QFrame>
#include <QMap>

class CLcdDisplayWgt : public QWidget
{
    Q_OBJECT

public:
    CLcdDisplayWgt(QWidget *parent=0);
    ~CLcdDisplayWgt();
    //更新显示内容
    void UpdateLcdDisplayData(const QByteArray &btData,int nRow,int nColumn);
    //填充颜色
    void UpdateLEDLight(int nIndex,int nColor);
    
protected:
    virtual void paintEvent(QPaintEvent *event);


private:
    //编号   ->  颜色1:白色  2：绿色  3 黄色  4 红色
    QMap<int, int> m_MapLightTColor;
    //编号   ->  颜色
    QMap<int, QColor> m_MapColorIndex;
    //设备版本
    QString m_strDeviceVersion;
    //设备名称
    QString m_strDeviceName;
    //显示内容
    QByteArray m_btDetailInfo;
    //显示的行
    int m_nRowCursor;
    //显示的列
    int m_nColumnCursor;
};

#endif // LCDDISPLAYWGT_H
