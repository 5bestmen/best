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
    //�������ļ�
    ~CXmlDataAnalyse();
    //
    void AnalyseTreeItems(const QString &strFilename);
    //�����ļ�
    void AnalyseTreeXmlData(QIODevice *pDevice);
    //����������������
    void AnalyseSceneChildData(QXmlStreamReader &xmlReader, std::shared_ptr<CModelScene> pScene);
    //������������
    void AnalyseGroupChildData(QXmlStreamReader &xmlReader, std::shared_ptr<CModelGroup> pGroup);
    //����ģ��������
    void AnalyseModelChildData(QXmlStreamReader &xmlReader, std::shared_ptr<CObjectModelInfo> pModel);

    //�����ļ�
    void SaveDataToFile();
    //д��Ӧ����Ϣ
    void WriteApplicationInfo(QXmlStreamWriter &xmlWriter);

private:
    //��������
    CModelContainer *m_pDataContainer;
    //�ļ�����
    QString m_strFilename;
    
};

#endif // XMLDATAANALYSE_H
