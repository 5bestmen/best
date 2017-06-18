/*! @file dataconvert.h
<PRE>
********************************************************************************
模块名       :
文件名       :  dataconvert.h
文件实现功能 :  数据类型定义转换
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   数据类型定义转换
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef _DATA_CONVERT_H
#define _DATA_CONVERT_H


#include <QString>
#include <QColor>
#include <QFont>
#include <QVariant>

namespace  DataConvert
{
    inline QString ColorToInt(const QColor &color)
    {
        long int temp=(uint)(((unsigned int)color.red()<< 16) | 
                               (((unsigned short)color.green()<< 8) | 
                                (unsigned short)color.blue()));  
		if (color.red() == 0 && color.green() == 0 && color.blue()==0)
		{
			return "#000000";
		}
		else if (color.red()==0&&color.green()==0)
		{
			return "#0000"+QString::number(temp,16);
		}
		else if (color.red()==0)
		{
			return "#00"+QString::number(temp,16);
		}
		else
		{
			return "#"+QString::number(temp,16); 
		}        
    }
    inline QString AlignToString(int data)
    {
        QString tempStr;

        switch(data)
        {
        case 0x0081:
                tempStr="align=left valign=middle";
            break;
        case 0x0082:
                tempStr="align=right valign=middle";
            break;
        case  0x0084:
                tempStr="align=center valign=middle";
            break;
        default:
            break;
        }
        return tempStr;
    }
    inline QString FontToString(const QFont &font,const QString& str,const QColor& color)
    {
        if (str.isEmpty())
        {
            return 0;
        }
        QString strFamily=font.family();

            bool isBond=font.bold();

        bool isIt=font.italic();

            QFontInfo info(font);

        int nSize=info.pixelSize();

       // qDebug("font-family :%s \t isBond: %d \t IsIt: %d \t size: %d \t",strFamily.toStdString(),isBond,isIt,nSize);

        QString tempStr;

        if (isBond&&!isIt)
        {
            tempStr=QString("<font face=\"%1\"  style=\"font-size:%2px\" color=%3 ><b>%4</b></font>").
                                                            arg(strFamily).
                                                            arg(nSize).
                                                            arg(QVariant(color).toString()).
                                                            arg(str);
        }
        if(isIt&&!isBond)
        {
            tempStr=QString("<font face=\"%1\" style=\"font-size:%2px\" color=#%3 ><i>%4</i></font>").
                                                            arg(strFamily).
                                                            arg(nSize).
                                                            arg(QVariant(color).toString()).
                                                            arg(str);
        }
        if (isBond&&isIt)
        {
            tempStr=QString("<font face=\"%1\" style=\"font-size:%2px\" color=%3 ><b><i>%4</i></b></font>").
                                                            arg(strFamily).
                                                            arg(nSize).
                                                            arg(QVariant(color).toString()).
                                                            arg(str);
        }
        if (!isIt&&!isBond)
        {
            tempStr=QString("<font face=\"%1\" style=\"font-size:%2px\" color=%3 >%4</font>").
                                                            arg(strFamily).
                                                            arg(nSize).
                                                            arg(QVariant(color).toString()).
                                                            arg(str);
        }

        return tempStr;
    }  
    inline QColor  IntToQColor(const int &intColor)  
    {  
       // int red = intColor & 255;  
       // int green = intColor >> 8 & 255;  
       // int blue = intColor >> 16 & 255;  
		QColor nColor=QVariant(intColor).value<QColor>();
        return nColor;//QColor(red, green, blue);  
    } 
}
#endif