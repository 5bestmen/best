#ifndef GRAPHTOOLSWGT_H
#define GRAPHTOOLSWGT_H

#include <QTreeWidget>
#include <QMimeData>
#include <QDrag>
#include <memory>
#include <QXmlStreamReader>


class CGraphToolsWgt : public QTreeWidget
{
    Q_OBJECT

public:
    CGraphToolsWgt(QWidget *parent=0);
    ~CGraphToolsWgt();

    //���ӻ�ͼ�ؼ�
    void CreateGraphTreeItems();

    //����ͼ��
    void CreateBasicGraphicItems(QTreeWidgetItem *pBaseGrahpicsItem);
    //��������
    void CreateBasicControlItems(QTreeWidgetItem *pControlItem);
	//ͼ��ؼ�
	void CreateChartItems(QTreeWidgetItem *pControlItem);

	//����ģ���ӽڵ�
	void AddTemplateItem(const QString &strFileName);
    //����ģ�������ļ�
	void AnalyseTemplate(const QString &strFilename);

	//�����ļ�
	bool AnalyseManagerFile(std::shared_ptr<QXmlStreamReader> pXml);
	//������ڵ�
	void AnalyseGroupNode(std::shared_ptr<QXmlStreamReader> pXml, QTreeWidgetItem *pGroupItem);


protected:
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent);


private:
	//ģ����ڵ�
	QTreeWidgetItem *m_pTemplateParent;
    
};

#endif // GRAPHTOOLSWGT_H
