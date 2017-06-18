/*! @file xml_serializer.h
<PRE>
********************************************************************************
模块名       :
文件名       :  xml_serializer.h
文件实现功能 :  图形文件序列化 xml 类
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
#ifndef XML_SERIALIZER_H
#define XML_SERIALIZER_H
  

#include "graph_serializer.h"


class CGraphicsLayer;
class CGraphFileVerInfo;
class QXmlStreamWriter;
 
class CGraphXmlSerializer : public CGraphSerializer
{
public:
	CGraphXmlSerializer()
	{

	}
	virtual ~CGraphXmlSerializer()
	{

	} 	
public:
	// 写文件,子类重写该函数，可以实现用xml，或者json，或者balabala的各种格式
	virtual bool SaveToFile(CGraphFile *pGraph, const QString & szFilePath);

	virtual bool LoadFromFile(const QString & szFilePah, CGraphFile *pFile) ;
	//
	void LoadGraphcInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile);
	//
	void LoadGraphItemInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile, int nLayer);
	//加载groupinfo   nChild1开始
	void LoadGroupItemInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile, int nLayer, CGroupWidget *pGroupWgt, int nChild, std::vector <CBaseWidget *> arrChild);
	//加载属性
	void LoadAttrInfo(std::shared_ptr<QXmlStreamReader> pXml,CBaseWidget *pBaseWgt);
	// 创建目录
	void CreatePath(const QString &strFilePath);
	//创建模板
	void LoadGraphTemplateInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile, int nLayer, const QRectF & rcPos);

private:
	void SaveFileVerInfo(std::shared_ptr<QXmlStreamWriter> pXmlWriter,const CGraphFileVerInfo *pInfo) const;

	void SaveLayer(std::shared_ptr<QXmlStreamWriter> pXmlWriter, std::shared_ptr<CGraphicsLayer> pLayer) const;

//	std::shared_ptr<QXmlStreamWriter> m_pXmlWriter;
};




#endif // XML_SERIALIZER_H

/** @}*/

