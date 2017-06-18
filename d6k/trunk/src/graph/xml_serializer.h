/*! @file xml_serializer.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  xml_serializer.h
�ļ�ʵ�ֹ��� :  ͼ���ļ����л� xml ��
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
	// д�ļ�,������д�ú���������ʵ����xml������json������balabala�ĸ��ָ�ʽ
	virtual bool SaveToFile(CGraphFile *pGraph, const QString & szFilePath);

	virtual bool LoadFromFile(const QString & szFilePah, CGraphFile *pFile) ;
	//
	void LoadGraphcInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile);
	//
	void LoadGraphItemInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile, int nLayer);
	//����groupinfo   nChild1��ʼ
	void LoadGroupItemInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile, int nLayer, CGroupWidget *pGroupWgt, int nChild, std::vector <CBaseWidget *> arrChild);
	//��������
	void LoadAttrInfo(std::shared_ptr<QXmlStreamReader> pXml,CBaseWidget *pBaseWgt);
	// ����Ŀ¼
	void CreatePath(const QString &strFilePath);
	//����ģ��
	void LoadGraphTemplateInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile, int nLayer, const QRectF & rcPos);

private:
	void SaveFileVerInfo(std::shared_ptr<QXmlStreamWriter> pXmlWriter,const CGraphFileVerInfo *pInfo) const;

	void SaveLayer(std::shared_ptr<QXmlStreamWriter> pXmlWriter, std::shared_ptr<CGraphicsLayer> pLayer) const;

//	std::shared_ptr<QXmlStreamWriter> m_pXmlWriter;
};




#endif // XML_SERIALIZER_H

/** @}*/

