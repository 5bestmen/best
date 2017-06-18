
#include "graphtoolswgt.h"
#include "graphdefine.h"
#include "widget_factory.h"
#include "createnewtemplatewgt.h"
#include <QMimeData>
#include <QDrag>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>

CGraphToolsWgt::CGraphToolsWgt(QWidget *parent)
    : QTreeWidget(parent)
{
	m_pTemplateParent = nullptr;
    setColumnCount(1);
    setHeaderLabel(tr("ToolBox"));
    //
    //设置graphics items
    CreateGraphTreeItems();
}

CGraphToolsWgt::~CGraphToolsWgt()
{

}

void CGraphToolsWgt::CreateGraphTreeItems()
{
    //基本图形
    QTreeWidgetItem *pBasicGrahipcsItem = new QTreeWidgetItem(this, QStringList(tr("Basic Graphic")));
    CreateBasicGraphicItems(pBasicGrahipcsItem);

    //基本控制
    QTreeWidgetItem *pBasicControlItem = new QTreeWidgetItem(this, QStringList(tr("Basic Control")));
    CreateBasicControlItems(pBasicControlItem);

    //图表
	QTreeWidgetItem *pChartItem = new QTreeWidgetItem(this, QStringList(tr("Chart Statistics")));
	CreateChartItems(pChartItem);
}

void CGraphToolsWgt::CreateBasicGraphicItems(QTreeWidgetItem *pBaseGrahpicsItem)
{
    //直线
    QTreeWidgetItem *LineItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Line")));
    LineItem->setData(0, Qt::UserRole, DRAW_TOOLS_LINE);

    //矩形
    QTreeWidgetItem *pRectangleItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Rectangle")));
    pRectangleItem->setData(0, Qt::UserRole, DRAW_TOOLS_RECTANGLE);

    //圆
    QTreeWidgetItem *pCycleItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Circle")));
    pCycleItem->setData(0, Qt::UserRole, DRAW_TOOLS_ROUND);

    //椭圆
    QTreeWidgetItem *pEllipseItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Ellipse")));
    pEllipseItem->setData(0, Qt::UserRole, DRAW_TOOLS_ELLIPSE);

    //圆角矩形
    QTreeWidgetItem *pCycleRectanleItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("CycleRectanle")));
    pCycleRectanleItem->setData(0, Qt::UserRole, DRAW_TOOLS_ROUNDRECT);

    //菱形
    QTreeWidgetItem *pDiamondItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Diamond")));
    pDiamondItem->setData(0, Qt::UserRole, DRAW_TOOLS_DIAMOND);

    //三角形
    QTreeWidgetItem *pTriangleItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Triangle")));
    pTriangleItem->setData(0, Qt::UserRole, DRAW_TOOLS_TRIANGLE);

    //梯形
    QTreeWidgetItem *pTrapezoidItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Trapezoid")));
    pTrapezoidItem->setData(0, Qt::UserRole, DRAW_TOOLS_TRAPEZOID);

    //平行四边形
    QTreeWidgetItem *pParallelogramItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Parallelogram")));
    pParallelogramItem->setData(0, Qt::UserRole, DRAW_TOOLS_PARALLELOGRAM);

	//折线
	QTreeWidgetItem *pPolylineItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Polyline")));
	pPolylineItem->setData(0, Qt::UserRole, DRAW_TOOLS_POLYLINE);

	//弧形
	QTreeWidgetItem *pCamberItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Camber")));
	pCamberItem->setData(0, Qt::UserRole, DRAW_TOOLS_CAMBER);

	//弓形
	QTreeWidgetItem *pChordItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Chord")));
	pChordItem->setData(0, Qt::UserRole, DRAW_TOOLS_CHORD);

	//扇形
	QTreeWidgetItem *pPieItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Pie")));
	pPieItem->setData(0, Qt::UserRole, DRAW_TOOLS_PIE);

	//多边形
	QTreeWidgetItem *pPolygonItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Polygon")));
	pPolygonItem->setData(0, Qt::UserRole, DRAW_TOOLS_POLYGON);

	//管道
	QTreeWidgetItem *pPipeItem = new QTreeWidgetItem(pBaseGrahpicsItem, QStringList(tr("Pipe")));
	pPipeItem->setData(0, Qt::UserRole, DRAW_TOOLS_PIPE);

}

void CGraphToolsWgt::CreateBasicControlItems(QTreeWidgetItem *pControlItem)
{
    QTreeWidgetItem *pPushButtontem = new QTreeWidgetItem(pControlItem, QStringList(tr("NormalButton")));
    pPushButtontem->setData(0, Qt::UserRole, DRAW_TOOLS_BTN_NORMAL); 

	QTreeWidgetItem *p3D1Buttontem = new QTreeWidgetItem(pControlItem, QStringList(tr("3D1Button")));
	p3D1Buttontem->setData(0, Qt::UserRole, DRAW_TOOLS_BTN_3D1);

	QTreeWidgetItem *p3D2Buttontem = new QTreeWidgetItem(pControlItem, QStringList(tr("3D2Button")));
	p3D2Buttontem->setData(0, Qt::UserRole, DRAW_TOOLS_BTN_3D2);

	QTreeWidgetItem *p3D3Buttontem = new QTreeWidgetItem(pControlItem, QStringList(tr("3D3Button")));
	p3D3Buttontem->setData(0, Qt::UserRole, DRAW_TOOLS_BTN_3D3);

	QTreeWidgetItem *p3D4Buttontem = new QTreeWidgetItem(pControlItem, QStringList(tr("3D4Button")));
	p3D4Buttontem->setData(0, Qt::UserRole, DRAW_TOOLS_BTN_3D4);
}

void CGraphToolsWgt::CreateChartItems(QTreeWidgetItem *pControlItem)
{
	QTreeWidgetItem *pCurePlotItem = new QTreeWidgetItem(pControlItem, QStringList(tr("CurePlot")));
	pCurePlotItem->setData(0, Qt::UserRole, DRAW_ADDR_CURE_PLOT);
}

//增加模板子节点
void CGraphToolsWgt::AddTemplateItem(const QString &strFileName)
{
	if (m_pTemplateParent == nullptr)
	{
		m_pTemplateParent = new QTreeWidgetItem(this, QStringList(tr("GraphicsLibrary")));
	}
	
	QTreeWidgetItem *pOneTemplate = new QTreeWidgetItem(m_pTemplateParent, QStringList(strFileName.split("/").last().remove(".xml")));

	pOneTemplate->setData(0, Qt::UserRole, DRAW_TOOLS_TEMPLATE);
	pOneTemplate->setData(0, Qt::UserRole+1, strFileName);
}


void CGraphToolsWgt::AnalyseTemplate(const QString &strFilename)
{
	//加载配置文件
	QFile tFile(strFilename);
	if (!tFile.open(QFile::ReadOnly | QFile::Text))
	{
		qDebug() << tFile.errorString();
		return;
	}

	//解析文件
	std::shared_ptr<QXmlStreamReader> pXml = std::make_shared<QXmlStreamReader>(&tFile);

	if (!AnalyseManagerFile(pXml))
	{
		QMessageBox::warning(this, tr("warning"), tr("libary manger file error!"));
	}

	tFile.close();
}

bool CGraphToolsWgt::AnalyseManagerFile(std::shared_ptr<QXmlStreamReader> pXml)
{
	if (m_pTemplateParent != nullptr)
	{
		delete m_pTemplateParent;
	}

    m_pTemplateParent = new QTreeWidgetItem(this, QStringList(tr("GraphicsLibrary")));
	

	while (!pXml->atEnd())
	{
		pXml->readNext();

		if (pXml->isStartElement())
		{
			if (pXml->name().toString() == "Group")
			{
				//根节点
				QTreeWidgetItem *pGroup = new QTreeWidgetItem;
				QString strGroupName = pXml->attributes().value("name").toString();

				pGroup->setText(0, strGroupName);
				pGroup->setIcon(0, QIcon(qApp->applicationDirPath() + TEMPLATE_FOLDER_ICON));
				m_pTemplateParent->addChild(pGroup);

				pGroup->setData(0, Qt::UserRole, DRAW_TOOLS_TEMPLATE_FOLDER);
				//
				AnalyseGroupNode(pXml, pGroup);
			}
			else if (pXml->name().toString() == "Item")
			{
				//子节点
				QTreeWidgetItem *pChild = new QTreeWidgetItem;
				//显示的名称
				QString strItemName = pXml->attributes().value("name").toString();
				pChild->setText(0, strItemName);

				//文件名称
				QString strFileName = pXml->attributes().value("filename").toString();
				pChild->setData(0, Qt::UserRole, DRAW_TOOLS_TEMPLATE);
				pChild->setData(0, Qt::UserRole + 1, strFileName);

				m_pTemplateParent->addChild(pChild);

			}
		}
	}
	return true;
}

void CGraphToolsWgt::AnalyseGroupNode(std::shared_ptr<QXmlStreamReader> pXml, QTreeWidgetItem *pGroupItem)
{
	Q_ASSERT(pGroupItem);
	if (pGroupItem == nullptr)
	{
		return;
	}

	while (!pXml->atEnd())
	{
		pXml->readNext();

		if (pXml->isStartElement())
		{
			if (pXml->name().toString() == "Group")
			{
				//根节点
				QTreeWidgetItem *pGroup = new QTreeWidgetItem;
				QString strGroupName = pXml->attributes().value("name").toString();

				pGroup->setText(0, strGroupName);
				pGroup->setIcon(0, QIcon(qApp->applicationDirPath() + TEMPLATE_FOLDER_ICON));
				pGroupItem->addChild(pGroup);
				pGroup->setData(0, Qt::UserRole, DRAW_TOOLS_TEMPLATE_FOLDER);

				//
				AnalyseGroupNode(pXml, pGroup);
			}
			else if (pXml->name().toString() == "Item")
			{
				//子节点
				QTreeWidgetItem *pChild = new QTreeWidgetItem;
				//显示的名称
				QString strItemName = pXml->attributes().value("name").toString();
				pChild->setText(0, strItemName);

				//文件名称
				QString strFileName = pXml->attributes().value("filename").toString();
				pChild->setData(0, Qt::UserRole + 1, strFileName);

				//文件icon
				QString strFileIcon = pXml->attributes().value("icon").toString();
				pChild->setData(0, Qt::UserRole + 2, strFileIcon);

				pChild->setData(0, Qt::UserRole, DRAW_TOOLS_TEMPLATE);
				pChild->setData(0, Qt::UserRole + 1, strFileName);


				pGroupItem->addChild(pChild);
			}
		}
		else
		{
			if (pXml->isEndElement())
			{
				if (pXml->name().toString() == "Group")
				{
					return;
				}
			}
		}

	}
}

void CGraphToolsWgt::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent);
    //只有子节点可以拖动
    if (currentItem() == nullptr)
    {
        return;
    }

    if (currentItem()->parent() == nullptr)
    {
        return;
    }

	if (currentItem()->data(0,Qt::UserRole).toInt() == DRAW_TOOLS_TEMPLATE_FOLDER)
	{
		return;
	}

    //拖动的数据 为 类型标识
    QByteArray itemData;
    itemData.setNum(currentItem()->data(0, Qt::UserRole).toInt());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("text/plain", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    //拖动时显示的图片
    QPixmap pixmap;
    //pixmap.load(":/images/logicmove.png");
    //drag->setPixmap(pixmap);
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
    {

    }
}
