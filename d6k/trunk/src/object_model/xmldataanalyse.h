#ifndef XMLDATAANALYSE_H
#define XMLDATAANALYSE_H

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "model_container.h"
#include "object_model.h"

class CModelContainer;
class CXmlDataAnalyse : public QObject
{
    Q_OBJECT

public:
    CXmlDataAnalyse(CModelContainer *pModelContainer, QObject *parent = 0);
    //解析树文件
    ~CXmlDataAnalyse();
    //
    void AnalyseTreeItems(const QString &strFilename);
    //解析文件
    void AnalyseTreeXmlData(QIODevice *pDevice);
    //解析场景下面数据
    void AnalyseSceneChildData(QXmlStreamReader &xmlReader, std::shared_ptr<CModelScene> pScene);
    //解析组下数据
    void AnalyseGroupChildData(QXmlStreamReader &xmlReader, std::shared_ptr<CModelGroup> pGroup);
    //分析模型先数据
    void AnalyseModelChildData(QXmlStreamReader &xmlReader, std::shared_ptr<CObjectModelInfo> pModel);

    //保存文件
    void SaveDataToFile();
    //写入应用信息
    void WriteApplicationInfo(QXmlStreamWriter &xmlWriter);

private:
    //数据容器
    CModelContainer *m_pDataContainer;
    //文件名称
    QString m_strFilename;
    
};

#endif // XMLDATAANALYSE_H
