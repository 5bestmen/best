/*! @file graph_serializer.h
<PRE>
********************************************************************************
模块名       :
文件名       :  graph_serializer.h
文件实现功能 :  图形文件序列化基类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  类似QT的抽象工厂设计模式，为今后的多种序列化方式预留接口
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图形文件序列化基类
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
	// 写文件,子类重写该函数，可以实现用xml，或者json，或者balabala的各种格式
	virtual bool SaveToFile(CGraphFile *pGraph, const QString & szFilePath) = 0;
	virtual bool LoadFromFile(const QString & szFilePah, CGraphFile *pFile) = 0;

};



#endif // GRAPH_SERIALIZER_H

/** @}*/

