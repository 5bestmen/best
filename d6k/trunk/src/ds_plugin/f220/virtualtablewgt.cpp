/*! @file
<PRE>
********************************************************************************
模块名       :	可拖拉虚遥信table
文件名       :	dragtablewgt.cpp
文件实现功能 :
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2016.9.27
*/
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QDebug>
#include <QHeaderView>
#include <QDragEnterEvent> 
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFile>
#include "virtualtablewgt.h"
#include "dragtablewgt.h"


/*! \fn CVirtualTableWgt::CVirtualTableWgt(QWidget *parent)
*********************************************************************************************************
** \brief CVirtualTableWgt::CVirtualTableWgt(QWidget *parent)
** \details   拖拉table
** \param  parent
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
CVirtualTableWgt::CVirtualTableWgt(QWidget *parent) :
    QTableWidget(parent)
{
    setAcceptDrops(true);
    setSelectionMode(MultiSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
	//隐藏列标题
	this->verticalHeader()->setHidden(true);
    m_bSecondFlag = false;

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));
}

void CVirtualTableWgt::SetPointType(const QString &strPonitType)
{
    m_strPointType = strPonitType;
}

/*! \fn CVirtualTableWgt::SetDescTableWgt()
*********************************************************************************************************
** \brief CVirtualTableWgt::SetDescTableWgt()
** \details   设置des
** \param  parent
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CVirtualTableWgt::SetDescTableWgt()
{
	setColumnCount(8);
	QStringList headers2;
	headers2 << tr("Position1Num") << tr("Position1Group") << tr("Position1Desc") << "" 
        << tr("Position2Num")<<tr("Position2Group")<<tr("Position2Desc")<<tr("TotalDesc");
	setHorizontalHeaderLabels(headers2);

    setColumnWidth(0, 50);
    setColumnWidth(1, 60);
    setColumnWidth(2, 200);
	setColumnWidth(3, 50);

    setColumnWidth(4, 50);
    setColumnWidth(5, 60);
    setColumnWidth(6, 200);

}

/*! \fn CVirtualTableWgt::SelectAllRows()
*********************************************************************************************************
** \brief CVirtualTableWgt::SelectAllRows()
** \details   选择row
** \param  起始点  结束点
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CVirtualTableWgt::SelectAllRows(const QPoint &beginPoint, const QPoint &endPoint)
{
    this->clearSelection();
    //setSelection(QRect(beginPoint,endPoint),QItemSelectionModel::Rows);

    int iBegin = 0;
    int iEnd = 0;

    if (beginPoint.y() > endPoint.y())
    {
        iBegin = endPoint.y();
        iEnd = beginPoint.y();
    }
    else
    {
        iBegin = beginPoint.y();
        iEnd = endPoint.y();
    }

    //
    int iBeginRow = 0;
    int iEndRow = 0;

    if (this->itemAt(0,iEnd) == NULL)
    {
        iEndRow = this->rowCount()-1;
    }
    else
    {
        iEndRow = this->itemAt(0,iEnd)->row();
    }
    //
    if (this->itemAt(0,iBegin) == NULL)
    {
        iBeginRow = this->rowCount()-1;
    }
    else
    {
        iBeginRow = this->itemAt(0,iBegin)->row();
    }

    //选中

    for (int i=iBeginRow; i<=iEndRow; i++)
    {
        if (!this->item(i,0)->isSelected())
        {
            this->selectRow(i);
        }
    }

}

/*! \fn CVirtualTableWgt::mousePressEvent(QMouseEvent *event)
*********************************************************************************************************
** \brief CVirtualTableWgt::mousePressEvent(QMouseEvent *event)
** \details   鼠标点击事件
** \param  起始点  结束点
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CVirtualTableWgt::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        //this->clearSelection();
        m_pStartPos = event->pos();
        if (itemAt(m_pStartPos) == NULL)
        {
            clearSelection();
        }

        if (itemAt(m_pStartPos) != NULL)
        {
            if (!(itemAt(m_pStartPos)->isSelected()))
            {
                clearSelection();
            }
        }

        //如果有选中的直接返回

        if (selectedRanges().count() > 0)
        {
            return;
        }
    }

    QTableWidget::mousePressEvent(event);
}

/*! \fn CVirtualTableWgt::mouseMoveEvent(QMouseEvent *event)
*********************************************************************************************************
** \brief CVirtualTableWgt::mouseMoveEvent(QMouseEvent *event)
** \details   鼠标移动事件
** \param  
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CVirtualTableWgt::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (this->itemAt(m_pStartPos) == NULL)
        {
            SelectAllRows(m_pStartPos,event->pos());
            return;
        }

        int distance = (event->pos() - m_pStartPos).manhattanLength();
		
        if (distance >= QApplication::startDragDistance())
        {
			
			if (!performDrag())
            {
                return;
            }
        }
    }

}

void CVirtualTableWgt::dragEnterEvent(QDragEnterEvent *event)
{
    CDragTableWgt *source = qobject_cast<CDragTableWgt *>(event->source());

    if (source)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CVirtualTableWgt::dragMoveEvent(QDragMoveEvent *event)
{
    CDragTableWgt *source = qobject_cast<CDragTableWgt *>(event->source());

    if (source)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

bool CVirtualTableWgt::performDrag()
{
    QString selectedString = selectionText();

    if (selectedString.isEmpty())
    {
        return false;
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setHtml(toHtml(selectedString));
    mimeData->setData("text/csv", toCsv(selectedString).toUtf8());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
	QPixmap pixmap;

	QString strRunPath = qApp->applicationDirPath();
	pixmap.load(strRunPath + "/ini/dragtable.png");
	pixmap = pixmap.scaled(QSize(400, selectedString.split("\n").count() * 15));
	
	drag->setPixmap(pixmap);
    if (drag->exec(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction)
    {
         selectionModel()->clearSelection();
    }
    return true;
}


void CVirtualTableWgt::dropEvent(QDropEvent *event)
{
    
    if (event->mimeData()->hasFormat("text/csv"))
    {
        QByteArray csvData = event->mimeData()->data("text/csv");
        QString csvText = QString::fromUtf8(csvData);
        fromCsv(csvText,event);
        event->acceptProposedAction();
		//发出信号  更新数据
		emit Signal_UpdateTableInfo();
		if (this->columnCount() == 8)
		{
			//发出信号  已经存在数据
			emit Signal_ExistListID(m_lstObserveIDs);
		}
		else if (this->columnCount() == 4)
		{
			emit Signal_ExistListID(m_lstRecoverIDs);
		}

    }
}


QString CVirtualTableWgt::selectionText() const
{
    QString selectionString;
    QString headerString;
    QAbstractItemModel *itemModel = model();
    if (selectedRanges().count() == 0)
    {
        return "";
    }

    for (int begin=0; begin<selectedRanges().count(); begin++)
    {
        QTableWidgetSelectionRange selection = selectedRanges().at(begin);


        for (int row = selection.topRow(), firstRow = row;row <= selection.bottomRow(); row++)
        {
            for (int col = selection.leftColumn(); col <= selection.rightColumn(); col++)
            {
                if (row == firstRow)
                {
                    headerString.append(horizontalHeaderItem(col)->text()).append("\t");
                }

                QModelIndex index = itemModel->index(row, col);

                selectionString.append(index.data().toString()).append("\t");
            }

            selectionString = selectionString.trimmed();
            selectionString.append("\n");
        }

    }

    return headerString.trimmed() + "\n" + selectionString.trimmed();
}

QString CVirtualTableWgt::toHtml(const QString &plainText) const
{
    QString result = plainText.toHtmlEscaped();
    return result;
}

QString CVirtualTableWgt::toCsv(const QString &plainText) const
{
    QString result = plainText;
    result.replace("\\", "\\\\");
    result.replace("\"", "\\\"");
    result.replace("\t", "\", \"");
    result.replace("\n", "\"\n\"");
    result.prepend("\"");
    result.append("\"");
    return result;
}

void CVirtualTableWgt::fromCsv(const QString &csvText, QDropEvent *event)
{
    //qDebug()<<"text:"<<csvText;
    QStringList rows = csvText.split("\n");
	//清空被选中的数据
	m_lstRecoverIDs.clear();

    for (int r = 1; r < rows.size(); ++r)
    {
        QStringList row = rows.at(r).split(", ");

        //不存在补0
        for (int i=row.count(); i<4; i++)
        {
            row.append("");
        }

		if (this->columnCount() == 4)
		{
			//当前被选中的数据
			m_lstRecoverIDs.append(row.at(1).trimmed().replace('"', "").toUInt());

		}
		else
		{
			if (m_lstObserveIDs.contains(row.at(0).trimmed().replace('"', "").toUInt()))
			{
				//如果已经存在  则不增加
				continue;
			}
			else
			{
				m_lstObserveIDs.append(row.at(0).trimmed().replace('"', "").toUInt());
			}

            QTableWidgetItem *pCurrentItem = nullptr;

            if (rows.size() == 2)
            {
                pCurrentItem = itemAt(event->pos());
            }
            

            if (pCurrentItem != nullptr)
            {
                int nRow = pCurrentItem->row();

                if (item(nRow,4)->text().isEmpty())
                {
                    item(nRow, 4)->setText(row.at(0).trimmed().replace('"', ""));
                    item(nRow, 5)->setText(row.at(1).trimmed().replace('"', ""));
                    item(nRow, 6)->setText(row.at(2).trimmed().replace('"', ""));
                }
            }
            else
            {
                int rowCount = this->rowCount();
                this->insertRow(rowCount);

                //setItem(rowCount, 0, new QTableWidgetItem(QString::number(rowCount + 1)));
                setItem(rowCount, 0, new QTableWidgetItem(row.at(0).trimmed().replace('"', "")));
                setItem(rowCount, 1, new QTableWidgetItem(row.at(1).trimmed().replace('"', "")));
                setItem(rowCount, 2, new QTableWidgetItem(row.at(2).trimmed().replace('"', "")));

                setItem(rowCount, 3, new QTableWidgetItem(""));
                setItem(rowCount, 4, new QTableWidgetItem(""));
                setItem(rowCount, 5, new QTableWidgetItem(""));
                setItem(rowCount, 6, new QTableWidgetItem(""));
                setItem(rowCount, 7, new QTableWidgetItem(""));

            }
		}
    }
// 
// 	if (this->columnCount() == 4)
// 	{
// 		//当前被选中的数据
// 		//改编颜色
// 		for (int i = 0; i < rowCount(); i++)
// 		{
// 			if (m_lstRecoverIDs.contains(item(i, 0)->text().toUInt()))
// 			{
// 				item(i, 0)->setBackgroundColor(QColor(255, 255, 255));
// 				item(i, 1)->setBackgroundColor(QColor(255, 255, 255));
// 				item(i, 2)->setBackgroundColor(QColor(255, 255, 255));
// 				item(i, 3)->setBackgroundColor(QColor(255, 255, 255));
// 			}
// 
// 		}
// 
// 	}
}


//获取当前table里面的值
QMap<unsigned int, RPT> CVirtualTableWgt::GetCurrentTableData()
{
	QMap<unsigned int, RPT> mapPointInfo;
	for (int i = 0; i < this->rowCount(); i++)
	{
		//往里面插入数据
		RPT srpt;
		srpt.NUM = this->item(i, 0)->text().toUInt();
		srpt.NUM2 = this->item(i, 1)->text().toUInt();
		srpt.GroupNum = 0;
		strcpy(srpt.Destriber, this->item(i, 3)->text().toStdString().c_str());
		strcpy(srpt.Unit, this->item(i, 4)->text().toStdString().c_str());
		mapPointInfo.insert(srpt.NUM, srpt);
	}
	return mapPointInfo;
}

//设置已提取状态槽函数
void CVirtualTableWgt::Slot_SetSelectedState(QList<unsigned int> lstIDs)
{
	for (int i = 0; i < rowCount(); i++)
	{
		if (lstIDs.contains(item(i,0)->text().toUInt()))
		{
			item(i, 0)->setBackgroundColor(QColor(0,255,255));
			item(i, 1)->setBackgroundColor(QColor(0, 255, 255));
			item(i, 2)->setBackgroundColor(QColor(0, 255, 255));
			item(i, 3)->setBackgroundColor(QColor(0, 255, 255));
		}
		else
		{
			item(i, 0)->setBackgroundColor(QColor(255, 255, 255));
			item(i, 1)->setBackgroundColor(QColor(255, 255, 255));
			item(i, 2)->setBackgroundColor(QColor(255, 255, 255));
			item(i, 3)->setBackgroundColor(QColor(255, 255, 255));
		}
	}
}


//设置复原状态的槽函数
void CVirtualTableWgt::Slot_setRecoverState(QList<unsigned int> lstIDs)
{
	int irowCount = rowCount();

	for (int i = irowCount-1; i >=0; i--)
	{
		if (lstIDs.contains(item(i, 0)->text().toUInt()))
		{
			this->removeRow(i);
		}
	}

	for (int m = 0; m < lstIDs.count(); m++)
	{
		m_lstObserveIDs.removeAll(lstIDs.at(m));
	}
	emit Signal_UpdateTableInfo();
	
}


//清空数据
void CVirtualTableWgt::ClearAllData()
{
	m_lstObserveIDs.clear();
	m_lstRecoverIDs.clear();
}

void CVirtualTableWgt::Slot_ContextMenuRequest(const QPoint &cPoint)
{
    Q_UNUSED(cPoint);
    QMenu *pMenu = new QMenu(this);

    QAction *pSaveAct = new QAction(tr("Save"), this);

    pMenu->addAction(pSaveAct);

    connect(pSaveAct, SIGNAL(triggered()), this, SLOT(Slot_SavaAction()));

    pMenu->exec(QCursor::pos());
    pMenu->deleteLater();
}

void CVirtualTableWgt::Slot_SavaAction()
{
    QByteArray btData;
    //保存数据
    //读取数据
    for (int i = 0; i < rowCount(); i++)
    {
        if (item(i,0)->text().isEmpty() || item(i,4)->text().isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"),tr("Row %1 is not right!").arg(i+1));
            return;
        }

        btData.append(item(i, 0)->text().toInt());
        btData.append(item(i, 4)->text().toInt());
    }
    
    QFile tFile(m_strFilename);

    if (!tFile.open(QIODevice::WriteOnly | QFile::Text))
    {
        qDebug() << tr("Write Virtual Binary Error");
        tFile.close();
        QMessageBox::warning(this, tr("Warning"), tr("Write Virtual Binary Error"));
    }
    else
    {
        tFile.write(btData);
        tFile.close();
        QMessageBox::warning(this, tr("Warning"), tr("Save Success!"));
    }

    
}

void CVirtualTableWgt::SetSaveFileName(const QString &strFilaPath)
{
    m_strFilename = strFilaPath + "/merge.bin";


}
