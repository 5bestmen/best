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
    //������ʾ����
    void UpdateLcdDisplayData(const QByteArray &btData,int nRow,int nColumn);
    //�����ɫ
    void UpdateLEDLight(int nIndex,int nColor);
    
protected:
    virtual void paintEvent(QPaintEvent *event);


private:
    //���   ->  ��ɫ1:��ɫ  2����ɫ  3 ��ɫ  4 ��ɫ
    QMap<int, int> m_MapLightTColor;
    //���   ->  ��ɫ
    QMap<int, QColor> m_MapColorIndex;
    //�豸�汾
    QString m_strDeviceVersion;
    //�豸����
    QString m_strDeviceName;
    //��ʾ����
    QByteArray m_btDetailInfo;
    //��ʾ����
    int m_nRowCursor;
    //��ʾ����
    int m_nColumnCursor;
};

#endif // LCDDISPLAYWGT_H
