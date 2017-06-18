/*! @file
<PRE>
********************************************************************************
模块名       :  读取xml配置接口
文件名       :
文件实现功能 :
作者         :  HuangYuqing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author  HuangYuqing
*  @version 1.0
*  @date    2016.8.18
*/
#ifndef EXCHANGE_XML_H
#define EXCHANGE_XML_H

#include <QIODevice>
#include <QXmlStreamReader>
#include<QVector>
#include <QMap>
#include <QDomElement>

#include "data_struct.h"


class CReadXML
{
public:
	CReadXML();
	~CReadXML();

	bool ReadXML(const QString &strFilename);
	//解析xml文件
	void AnalyseXMLData(QDomElement* root);
	//解析厂站数据
	bool ReadStation(QDomElement& dElement);
	//解析设备数据
	bool ReadDevice(QDomElement& dElement);
	//解析参数
	bool ReadParaconfig(QDomElement& dElement);

	// 解析单个设备信息
	bool ReadSingleDeviceInfo(QDomElement& dElement, int DeviceID);

	// 解析Analog
	bool ReadAnalog(QDomElement& dElement, int DeviceID);

	//解析Binary
	bool ReadBinary(QDomElement& dElement, int DeviceID);

	//解析KWH
	bool ReadKWH(QDomElement& dElement, int DeviceID);
	//获取device信息
	QMap<int, SDeviceInfo> GetDeviceInfo();


	//解析转发表达式
	bool AnalyzeExpression(const char* source, STransInfo& transInfo);

	/**********************  目标  ***********************/
	// 解析目标
	bool AnalyzeTarget(const char* source, STransInfo& transInfo);
	// 解析单个目标
	bool AnalyzeTargetSingle(const char* src, STransInfo& transInfo, int nLen);

	/**********************  条件  ***********************/
	// 解析条件
	bool AnalyzeCond(const char* source, STransInfo& transInfo);
	// 解析单个条件
	bool AnalyzeCondSingle(const char* src, STransInfo& transInfo, int nLen);

	// 返回下标
	int FindPosition(const char * src, char flag);

	// 返回数量
	int FlagCount(const char * src, char flag);
    //获取开关量 
    QMap<int, QVector<SDiInfo> > GetBinaryInfo();
	//获取模拟量
	QMap<int, QVector<SAiInfo> > GetAnalogInfo();

	void ltrim(char *s)
	{
		if (s == NULL)
		{
			return;
		}

		char *p;
		p = s;
		while (*p == ' ' || *p == '\t')
		{
			*p++;
		}

		strcpy(s, p);
	}

	void rtrim(char *s)
	{
		int i;

		i = strlen(s) - 1;
		while ((s[i] == ' ' || s[i] == '\t') && i >= 0)
		{
			i--;
		};

		s[i + 1] = '\0';
	}

	void trim(char *s)
	{
		ltrim(s);
		rtrim(s);
	}

public:
	

	// key:deviceID(channelID)
	QMap<int, SDeviceInfo> m_mapDevice;

	QMap<int, QVector<SAiInfo> > m_mapAnalog;

	QMap<int, QVector<SDiInfo> > m_mapBinary;

	QMap<int, QVector<SKwhInfo> > m_mapKwh;
	
	
};



#endif // EXCHANGE_XML_H



