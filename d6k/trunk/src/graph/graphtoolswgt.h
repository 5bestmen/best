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

    //增加绘图控件
    void CreateGraphTreeItems();

    //基本图形
    void CreateBasicGraphicItems(QTreeWidgetItem *pBaseGrahpicsItem);
    //基本控制
    void CreateBasicControlItems(QTreeWidgetItem *pControlItem);
	//图表控件
	void CreateChartItems(QTreeWidgetItem *pControlItem);

	//增加模板子节点
	void AddTemplateItem(const QString &strFileName);
    //解析模板配置文件
	void AnalyseTemplate(const QString &strFilename);

	//解析文件
	bool AnalyseManagerFile(std::shared_ptr<QXmlStreamReader> pXml);
	//解析组节点
	void AnalyseGroupNode(std::shared_ptr<QXmlStreamReader> pXml, QTreeWidgetItem *pGroupItem);


protected:
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent);


private:
	//模板根节点
	QTreeWidgetItem *m_pTemplateParent;
    
};

#endif // GRAPHTOOLSWGT_H
