/*! @file
<PRE>
********************************************************************************
模块名       :	可拖拉table
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
#include "analogdragtablewgt.h"
#include "virtualtablewgt.h"

/*! \fn CChoosePointsWgt::CAnalogDragTableWgt(QWidget *parent)
*********************************************************************************************************
** \brief CChoosePointsWgt::CAnalogDragTableWgt(QWidget *parent)
** \details   拖拉table
** \param  parent
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
CAnalogDragTableWgt::CAnalogDragTableWgt(QWidget *parent):
    QTableWidget(parent)
{
    setAcceptDrops(true);
    setSelectionMode(MultiSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
	//隐藏列标题
	this->verticalHeader()->setHidden(true);
    m_bSecondFlag = false;

    m_pDropInItem = nullptr;
}

void CAnalogDragTableWgt::SetPointType(const QString &strPonitType)
{
    m_strPointType = strPonitType;
}


/*! \fn CChoosePointsWgt::SetSourceTableWgt()
*********************************************************************************************************
** \brief CChoosePointsWgt::SetSourceTableWgt()
** \details   设置 sorce
** \param  parent
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CAnalogDragTableWgt::SetSourceTableWgt()
{
	setColumnCount(9);
	QStringList headers;
	headers << tr("DeviceNum") << tr("GroupName") << tr("Destriber") << tr("Unit")<<tr("b")
		<<tr("k")<<tr("UpLimit")<<tr("DownLimit")<<tr("Threshold");
	setHorizontalHeaderLabels(headers);
	setColumnWidth(2, 300);

}

/*! \fn CChoosePointsWgt::SetDescTableWgt()
*********************************************************************************************************
** \brief CChoosePointsWgt::SetSourceTableWgt()
** \details   设置des
** \param  parent
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CAnalogDragTableWgt::SetDescTableWgt()
{
	setColumnCount(10);
	QStringList headers2;
	headers2 << tr("RemoteNum") << tr("DeviceNum") << tr("GroupName") << tr("Destriber") << tr("Unit") << tr("b")
		<< tr("k") << tr("UpLimit") << tr("DownLimit") << tr("Threshold");;
	setHorizontalHeaderLabels(headers2);
	setColumnWidth(3, 300);
}

/*! \fn CChoosePointsWgt::SelectAllRows()
*********************************************************************************************************
** \brief CChoosePointsWgt::SelectAllRows()
** \details   选择row
** \param  起始点  结束点
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CAnalogDragTableWgt::SelectAllRows(const QPoint &beginPoint, const QPoint &endPoint)
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
        if (this->item(i,0))
        {
            if (!this->item(i, 0)->isSelected())
            {
                this->selectRow(i);
            }
        }

    }

}

/*! \fn CChoosePointsWgt::mousePressEvent(QMouseEvent *event)
*********************************************************************************************************
** \brief CChoosePointsWgt::mousePressEvent(QMouseEvent *event)
** \details   鼠标点击事件
** \param  起始点  结束点
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CAnalogDragTableWgt::mousePressEvent(QMouseEvent *event)
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

/*! \fn CChoosePointsWgt::mouseMoveEvent(QMouseEvent *event)
*********************************************************************************************************
** \brief CChoosePointsWgt::mouseMoveEvent(QMouseEvent *event)
** \details   鼠标移动事件
** \param  
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CAnalogDragTableWgt::mouseMoveEvent(QMouseEvent *event)
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

void CAnalogDragTableWgt::dragEnterEvent(QDragEnterEvent *event)
{
	CAnalogDragTableWgt *source = qobject_cast<CAnalogDragTableWgt *>(event->source());

    if (source && source != this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
        return;
    }

    CVirtualTableWgt *vSoruce = qobject_cast<CVirtualTableWgt *>(event->source());

    if (vSoruce)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CAnalogDragTableWgt::dragMoveEvent(QDragMoveEvent *event)
{
	CAnalogDragTableWgt *source = qobject_cast<CAnalogDragTableWgt *>(event->source());

    if (source && source != this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
        return;
    }

    CVirtualTableWgt *vSoruce = qobject_cast<CVirtualTableWgt *>(event->source());

    if (vSoruce)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

bool CAnalogDragTableWgt::performDrag()
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


void CAnalogDragTableWgt::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/csv"))
    {
        QByteArray csvData = event->mimeData()->data("text/csv");
        QString csvText = QString::fromUtf8(csvData);
        m_pDropInItem = itemAt(event->pos());
        fromCsv(csvText);
        event->acceptProposedAction();
		//发出信号  更新数据
		emit Signal_UpdateTableInfo();
		if (this->columnCount() == 10)
		{
			//发出信号  已经存在数据
			emit Signal_ExistListID(m_lstObserveIDs);
		}
		else if (this->columnCount() == 9)
		{
			emit Signal_ExistListID(m_lstRecoverIDs);
		}


    }
}


QString CAnalogDragTableWgt::selectionText() const
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

QString CAnalogDragTableWgt::toHtml(const QString &plainText) const
{
    QString result = plainText.toHtmlEscaped();
    return result;
}

QString CAnalogDragTableWgt::toCsv(const QString &plainText) const
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

void CAnalogDragTableWgt::fromCsv(const QString &csvText)
{
    //qDebug()<<"text:"<<csvText;
    QStringList rows = csvText.split("\n");

    if (rows.count() == 0)
    {
        return;
    }
	//清空被选中的数据
	m_lstRecoverIDs.clear();

    int nCurrentRow = 0;
    
    if (m_pDropInItem != nullptr)
    {
        nCurrentRow = m_pDropInItem->row();
    }

    for (int r = 1; r < rows.size(); ++r)
    {
        QStringList row = rows.at(r).split(", ");


		if (this->columnCount() == 9)
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

            int rowCount = 0;

            if (m_pDropInItem == nullptr)
            {
                rowCount = this->rowCount();;
            }
            else
            {
                rowCount = nCurrentRow++;
            }
			//int rowCount = this->rowCount();

			this->insertRow(rowCount);

			setItem(rowCount, 0, new QTableWidgetItem(QString::number(rowCount)));
			setItem(rowCount, 1, new QTableWidgetItem(row.at(0).trimmed().replace('"', "")));
			setItem(rowCount, 2, new QTableWidgetItem(row.at(1).trimmed().replace('"', "")));

			setItem(rowCount, 3, new QTableWidgetItem(row.at(2).trimmed().replace('"', "")));
			setItem(rowCount, 4, new QTableWidgetItem(row.at(3).trimmed().replace('"', "")));

			setItem(rowCount, 5, new QTableWidgetItem(row.at(4).trimmed().replace('"', "")));
			setItem(rowCount, 6, new QTableWidgetItem(row.at(5).trimmed().replace('"', "")));

			setItem(rowCount, 7, new QTableWidgetItem(row.at(6).trimmed().replace('"', "")));
			setItem(rowCount, 8, new QTableWidgetItem(row.at(7).trimmed().replace('"', "")));
			setItem(rowCount, 9, new QTableWidgetItem(row.at(8).trimmed().replace('"', "")));

		}
    }

	if (this->columnCount() == 9)
	{

		//当前被选中的数据
		//改编颜色
		for (int i = 0; i < rowCount(); i++)
		{
			if (m_lstRecoverIDs.contains(item(i, 0)->text().toUInt()))
			{
				item(i, 0)->setBackgroundColor(QColor(255, 255, 255));
				item(i, 1)->setBackgroundColor(QColor(255, 255, 255));
				item(i, 2)->setBackgroundColor(QColor(255, 255, 255));
				item(i, 3)->setBackgroundColor(QColor(255, 255, 255));

				item(i, 4)->setBackgroundColor(QColor(255, 255, 255));
				item(i, 5)->setBackgroundColor(QColor(255, 255, 255));
				item(i, 6)->setBackgroundColor(QColor(255, 255, 255));
				item(i, 7)->setBackgroundColor(QColor(255, 255, 255));
				item(i, 8)->setBackgroundColor(QColor(255, 255, 255));
			}

		}

	}
    else
    {
        //从新编号
        if (m_pDropInItem != nullptr)
        {
            for (int i = 0; i < this->rowCount(); i++)
            {
                item(i, 0)->setText(QString::number(i));
            }
        }
    }
}


//获取当前table里面的值
QMap<unsigned int, RPT> CAnalogDragTableWgt::GetCurrentTableData()
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
void CAnalogDragTableWgt::Slot_SetSelectedState(QList<unsigned int> lstIDs)
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
void CAnalogDragTableWgt::Slot_setRecoverState(QList<unsigned int> lstIDs)
{
	int irowCount = rowCount();

	for (int i = irowCount-1; i >=0; i--)
	{
		if (lstIDs.contains(item(i, 1)->text().toUInt()))
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
void CAnalogDragTableWgt::ClearAllData()
{
	m_lstObserveIDs.clear();
	m_lstRecoverIDs.clear();
}

void CAnalogDragTableWgt::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ShiftModifier)
    {
        setSelectionMode(ExtendedSelection);
    }

    QTableWidget::keyPressEvent(event);
}

void CAnalogDragTableWgt::keyReleaseEvent(QKeyEvent *event)
{
    setSelectionMode(MultiSelection);

    QTableWidget::keyReleaseEvent(event);
}
