/*! @file graph_serializer.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graph_serializer.h
�ļ�ʵ�ֹ��� :  ͼ���ļ����л�����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ����QT�ĳ��󹤳����ģʽ��Ϊ���Ķ������л���ʽԤ���ӿ�
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼ���ļ����л�����
*  @author  LiJin
*  @version 1.0
*  @date    2016.08.28
*******************************************************************************/
#ifndef GRAPH_SERIALIZER_H
#define GRAPH_SERIALIZER_H   

//#include "graph_file.h"

class CGraphFile;
 
class CGraphSerializer
{
public:
	CGraphSerializer()
	{

	}
	virtual ~CGraphSerializer()
	{

	}
protected:
	void LogMsg(const char *szLogTxt, int nLevel);
public:
	// д�ļ�,������д�ú���������ʵ����xml������json������balabala�ĸ��ָ�ʽ
	virtual bool SaveToFile(CGraphFile *pGraph, const QString & szFilePath) = 0;
	virtual bool LoadFromFile(const QString & szFilePah, CGraphFile *pFile) = 0;

};



#endif // GRAPH_SERIALIZER_H

/** @}*/

