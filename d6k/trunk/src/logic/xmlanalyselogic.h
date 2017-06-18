#ifndef XMLANALYSELOGIC_H
#define XMLANALYSELOGIC_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <memory>
#include <QDomNode>
#include <QDomElement>
#include "seqdefine.h"

class CLogicContainter;
class CNodeLogicContainer;
class CTaskContainer;
class CLogicXmlReader;
class QDomDocument;
class QDomElement;

class CXmlAnalyseLogic : public QObject
{
    Q_OBJECT

public:
    CXmlAnalyseLogic(QObject *parent=0);
    ~CXmlAnalyseLogic();

//     //���湤���ļ�
//     void SaveLogicObj(const QString &strFilename, CLogicContainter *pLogicContainter);
//     //���湤������
//     void SaveProAttr(CLogicContainter *pLogicContainter, QXmlStreamWriter &xmlWriter);
//     //��װNode����
//     void SaveNodeAttr(std::shared_ptr<CNodeLogicContainer> pNodeContainer, QXmlStreamWriter &xmlWriter);
// 
//     //��װmast����
//     void SaveMastAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter);
//     //��װfast����
//     void SaveFastAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter);
//     //��װaux0����
//     void SaveAuxAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter);


    //�򿪹����ļ�
    bool OpenLogicObj(QDomDocument *pXml, QDomElement *pRoot, const QString &strFilePath, CLogicContainter *pLogicContainter);
    //��������
   // bool AnalyseLogicObj(QIODevice *pDevice, CLogicContainter *pLogicContainter);
    //��װNode
    //void AnalyseNodeAttr(QXmlStreamReader &xmlReader, CLogicContainter *pLogicContainter);
    //�����ڵ�
    void AnalyseNodeObj(const QString &strTagName, QDomNode &NodeDom, std::shared_ptr<CNodeLogicContainer> tNodeContainer);
    //mast
    void AnalyseMastObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer);
    //fast
    void AnalyseFastObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer);
    //aux
    void AnalyseAuxObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer);

    //��װmast
//     void AnalyseMastAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tMastContaner,const QString strNode);
//     //��װfast
//     void AnalyseFastAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tFastContaner, const QString strNode);
//     //��װaux
//     void AnalyseAuxAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tAuxContaner, const QString strNode);

    //const LOGICDETAIL & GetReadData();

private:
    //LOGICDETAIL m_LogicDetail;
    //logic·��
    QString m_strLogicPath;
    CLogicXmlReader *m_pObjectInfo;
};

#endif // XMLANALYSELOGIC_H
