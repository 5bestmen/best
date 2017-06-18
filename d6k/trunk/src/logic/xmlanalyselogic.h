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

//     //保存工程文件
//     void SaveLogicObj(const QString &strFilename, CLogicContainter *pLogicContainter);
//     //保存工程属性
//     void SaveProAttr(CLogicContainter *pLogicContainter, QXmlStreamWriter &xmlWriter);
//     //组装Node属性
//     void SaveNodeAttr(std::shared_ptr<CNodeLogicContainer> pNodeContainer, QXmlStreamWriter &xmlWriter);
// 
//     //组装mast属性
//     void SaveMastAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter);
//     //组装fast属性
//     void SaveFastAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter);
//     //组装aux0属性
//     void SaveAuxAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter);


    //打开工程文件
    bool OpenLogicObj(QDomDocument *pXml, QDomElement *pRoot, const QString &strFilePath, CLogicContainter *pLogicContainter);
    //分析工程
   // bool AnalyseLogicObj(QIODevice *pDevice, CLogicContainter *pLogicContainter);
    //组装Node
    //void AnalyseNodeAttr(QXmlStreamReader &xmlReader, CLogicContainter *pLogicContainter);
    //分析节点
    void AnalyseNodeObj(const QString &strTagName, QDomNode &NodeDom, std::shared_ptr<CNodeLogicContainer> tNodeContainer);
    //mast
    void AnalyseMastObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer);
    //fast
    void AnalyseFastObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer);
    //aux
    void AnalyseAuxObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer);

    //组装mast
//     void AnalyseMastAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tMastContaner,const QString strNode);
//     //组装fast
//     void AnalyseFastAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tFastContaner, const QString strNode);
//     //组装aux
//     void AnalyseAuxAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tAuxContaner, const QString strNode);

    //const LOGICDETAIL & GetReadData();

private:
    //LOGICDETAIL m_LogicDetail;
    //logic路径
    QString m_strLogicPath;
    CLogicXmlReader *m_pObjectInfo;
};

#endif // XMLANALYSELOGIC_H
