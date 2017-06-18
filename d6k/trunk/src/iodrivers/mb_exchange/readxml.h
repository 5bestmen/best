/*! @file
<PRE>
********************************************************************************
ģ����       :  ��ȡxml���ýӿ�
�ļ���       :
�ļ�ʵ�ֹ��� :
����         :  HuangYuqing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
	//����xml�ļ�
	void AnalyseXMLData(QDomElement* root);
	//������վ����
	bool ReadStation(QDomElement& dElement);
	//�����豸����
	bool ReadDevice(QDomElement& dElement);
	//��������
	bool ReadParaconfig(QDomElement& dElement);

	// ���������豸��Ϣ
	bool ReadSingleDeviceInfo(QDomElement& dElement, int DeviceID);

	// ����Analog
	bool ReadAnalog(QDomElement& dElement, int DeviceID);

	//����Binary
	bool ReadBinary(QDomElement& dElement, int DeviceID);

	//����KWH
	bool ReadKWH(QDomElement& dElement, int DeviceID);
	//��ȡdevice��Ϣ
	QMap<int, SDeviceInfo> GetDeviceInfo();


	//����ת�����ʽ
	bool AnalyzeExpression(const char* source, STransInfo& transInfo);

	/**********************  Ŀ��  ***********************/
	// ����Ŀ��
	bool AnalyzeTarget(const char* source, STransInfo& transInfo);
	// ��������Ŀ��
	bool AnalyzeTargetSingle(const char* src, STransInfo& transInfo, int nLen);

	/**********************  ����  ***********************/
	// ��������
	bool AnalyzeCond(const char* source, STransInfo& transInfo);
	// ������������
	bool AnalyzeCondSingle(const char* src, STransInfo& transInfo, int nLen);

	// �����±�
	int FindPosition(const char * src, char flag);

	// ��������
	int FlagCount(const char * src, char flag);
    //��ȡ������ 
    QMap<int, QVector<SDiInfo> > GetBinaryInfo();
	//��ȡģ����
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



