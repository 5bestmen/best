
#include <QApplication>
#include <QFile>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QTime>
#include <QDebug>
#include <QClipboard>
#include <QMouseEvent>
#include <QTableWidgetSelectionRange>
#include <QFontDialog>
#include <QPalette>
#include <QtCharts/QAbstractAxis>
#include "public/dataconvert.h"
#include "qxmlfunction.h"
#include "qspreadsheet.h"
#include "aqp/aqp.hpp"
#include "ctrlparadialog.h"
#include "pieview.h"
#include "barview.h"
#include "chartview.h"

int G_TABLE_TYPE = -1;

CSpreadSheet::CSpreadSheet(QWidget *parent)
	: QTableWidget(parent)
{
	m_bAutoRecalc = true;
	setItemPrototype(new QCell);
	setSelectionMode(ContiguousSelection);
	connect(this, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(somethingChanged()));
	clear();
}

CSpreadSheet::~CSpreadSheet()
{

}
/*! \fn void CSpreadSheet::InitSpreadSheet(CRptFile *pReport)
********************************************************************************************************* 
** \brief CSpreadSheet::InitSpreadSheet 
** \details 初始化界面
** \param pReport 
** \return void 
** \author LiJin 
** \date 2016年8月18日 
** \note 
********************************************************************************************************/
void CSpreadSheet::InitSpreadSheet(CRptFile *pReport)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	Q_ASSERT(pReport);
	if (pReport == nullptr)
		return;

	CTableInfo * pInfo = pReport->GetRptInfo();

	Q_ASSERT(pInfo);
	if (!pInfo)
	{
		return;
	}

	//clear(pInfo->m_RowCount, pInfo->m_ColCount);
	//合并信息再次记录

	m_lstMergeCells.clear();
	
	m_lstMergeCells = pInfo->m_MergeItemList;
	// 检查是否有错
	CheckMergeInfo();
	SetCellSpan(*pInfo);

	m_TableData.m_nTableType = pInfo->m_nTableType;
	G_TABLE_TYPE = pInfo->m_nTableType;

	QMap<int, float>nRowHeight = pInfo->m_RowHeight;
	QMapIterator<int, float> i(nRowHeight);
	while (i.hasNext())
	{
		i.next();
		setRowHeight(i.key(), i.value());
	}

	QMap<int, float>nColWidth = pInfo->m_ColWidth;
	QMapIterator<int, float> j(nColWidth);
	while (j.hasNext())
	{
		j.next();
		setColumnWidth(j.key(), j.value());
	}

	quint16 nRow = 0;
	quint16 nColumn = 0;

	//初始化单元格
	QList<CCellData*> &lstData = pReport->GetCellList();
	foreach(CCellData *pCellData, lstData)
	{
		Q_ASSERT(pCellData);
		if (pCellData)
		{
			nRow = pCellData->m_nRowIndex;
			nColumn = pCellData->m_nColIndex;
			SetCellFormat(nRow, nColumn, *pCellData);
		}
	}

	//初始化控件
	QList<CCtrlData*> &lstCtrlData = pReport->GetCtrlList();

	for each (CCtrlData* var in lstCtrlData)
	{
		QVector<CCtrlGernalValue*> vecData;

		for (int i = 0; i < 5; ++i)
		{
			CCtrlGernalValue* pVecData = new CCtrlGernalValue();
			pVecData->m_szName = tr("Converter#%1").arg(i);
			pVecData->m_TagName = tr("TagName%2").arg(i);
			pVecData->m_vecValue.append(CPoint(rand() % 10, rand() % 10));
			pVecData->m_vecValue.append(CPoint(rand() % 20, rand() % 20));
			pVecData->m_vecValue.append(CPoint(rand() % 30, rand() % 30));
			pVecData->m_vecValue.append(CPoint(rand() % 40, rand() % 40));
			pVecData->m_vecValue.append(CPoint(rand() % 50, rand() % 50));
			vecData.append(pVecData);
		}
		var->SetCtrlData(vecData);

		int nType = var->GetCtrlType();

		switch (nType)
		{
		case PieType:
		{
			CPieView* pView = new CPieView(var);
			this->setCellWidget(var->GetRowIndex(), var->GetColIndex(), pView);
			break;
		}
		case BarType:
		{
			CBarView* pView = new CBarView(var);
			this->setCellWidget(var->GetRowIndex(), var->GetColIndex(), pView);
			break;
		}
		case ChartType:
		{
			CChartView* pView = new CChartView(var);
			this->setCellWidget(var->GetRowIndex(), var->GetColIndex(), pView);
			break;
		}
			
		default:
			break;
		}
	}

	QApplication::restoreOverrideCursor();
}

int CSpreadSheet::GetTableTypeInfo()
{
	return m_TableData.m_nTableType;
}


void CSpreadSheet::SetMerge()
{
	setSpan(this->selectedRanges().first().topRow(),
		this->selectedRanges().first().leftColumn(),
		this->selectedRanges().last().bottomRow() - this->selectedRanges().first().topRow() + 1,
		this->selectedRanges().last().rightColumn() - this->selectedRanges().first().leftColumn() + 1);

	CMergeInfo tempData;
	tempData.m_nRow = selectedRanges().first().topRow();
	tempData.m_nColumn = selectedRanges().first().leftColumn();
	tempData.m_nRowCount = selectedRanges().last().bottomRow() - this->selectedRanges().first().topRow() + 1;
	tempData.m_nColCount = selectedRanges().last().rightColumn() - this->selectedRanges().first().leftColumn() + 1;
	if ((tempData.m_nRowCount == 1) && (tempData.m_nColCount == 1))
	{
	}
	else
	{
		RecoderMergeInfo(tempData);
	}


	//add to avoid null-pointer   
	QCell *c = cell(tempData.m_nRow, tempData.m_nColumn);

	if (!c)
	{
		c = new QCell;
		setItem(tempData.m_nRow, tempData.m_nColumn, c);
	}

	cell(tempData.m_nRow, tempData.m_nColumn)->m_ItemData.m_MergeRecoder.m_nRow = tempData.m_nRow;
	cell(tempData.m_nRow, tempData.m_nColumn)->m_ItemData.m_MergeRecoder.m_nColumn = tempData.m_nColumn;
	cell(tempData.m_nRow, tempData.m_nColumn)->m_ItemData.m_MergeRecoder.m_nRowCount = tempData.m_nRowCount;
	cell(tempData.m_nRow, tempData.m_nColumn)->m_ItemData.m_MergeRecoder.m_nColCount = tempData.m_nColCount;

}
void CSpreadSheet::SetUnMerge()
{
	this->clearSpans();

	int mCurrentRow = currentRow();
	int nCurrentColumn = currentColumn();

	if (!m_lstMergeCells.isEmpty())
	{
		//若此记录存在，则删除此记录
		for (int i = 0; i < m_lstMergeCells.size(); i++)
		{
			if (m_lstMergeCells.at(i).m_nRow == mCurrentRow&&m_lstMergeCells.at(i).m_nColumn == nCurrentColumn)
			{
				QCell  *c = static_cast<QCell *>(cell(mCurrentRow, nCurrentColumn));
				if (!c)
				{
					c = new QCell;
				}
				c->SetSpanClear();
				m_lstMergeCells.removeAt(i);
			}
		}
	}

	if (!m_lstMergeCells.isEmpty())
	{
		foreach(CMergeInfo tempData, m_lstMergeCells)
		{
			setSpan(tempData.m_nRow, tempData.m_nColumn, tempData.m_nRowCount, tempData.m_nColCount);

			QCell *c = static_cast<QCell *>(cell(tempData.m_nRow, tempData.m_nColumn));
			if (!c)
			{
				c = new QCell;
			}
			CMergeInfo recoder;
			recoder.m_nRow = tempData.m_nRow;
			recoder.m_nColumn = tempData.m_nColumn;
			recoder.m_nRowCount = tempData.m_nRowCount;
			recoder.m_nColCount = tempData.m_nColCount;

			c->SetSpanData(recoder);
			qDebug("row: %d columu: %d rowCount: %d colCount: %d ", tempData.m_nRow, tempData.m_nColumn, tempData.m_nRowCount, tempData.m_nColCount);
		}
	}
}

void CSpreadSheet::SetMyFont()
{
	bool ok;

	QCell *c = cell(currentRow(), currentColumn());
	if (!c)
	{
		c = new QCell;
	}
	const QFont &font = QFontDialog::getFont(&ok, c->font());
	QList<QTableWidgetItem *> items = selectedItems();
	if (!items.isEmpty())
	{
		foreach(QTableWidgetItem *pItem, items)
		{
			if ( pItem && ok )
			{
				pItem->setFont(font);
				somethingChanged();
			}
		}
	}
}

void CSpreadSheet::SetFontLeft()
{
	QList<QTableWidgetItem *> items = selectedItems();
	if (!items.isEmpty())
	{
		foreach(QTableWidgetItem *pItem, items)
		{
			if (pItem)
			{
				pItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
				somethingChanged();
			}
		}
	}
}
void CSpreadSheet::SetFontMiddle()
{
	QList<QTableWidgetItem *> items = selectedItems();
	if (!items.isEmpty())
	{
		foreach(QTableWidgetItem *pItem, items)
		{
			if (pItem)
			{
				pItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				somethingChanged();
			}
		}
	}
}

void CSpreadSheet::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		int nRow = selectedRanges().first().topRow();

		int nColumn = selectedRanges().first().leftColumn();

		int nRowCount = selectedRanges().last().bottomRow() - selectedRanges().first().topRow() + 1;

		int nColumnCount = selectedRanges().last().rightColumn() - selectedRanges().first().leftColumn() + 1;

		if (nRowCount == 1 && nColumnCount == 1)
		{
			emit cellSelected(CurrentLocation());
			return;
		}
		//start
		int nStep = nColumn / 26;
		int nLeft = nColumn % 26;

		QString startCell;
		QString endCell;

		if (nStep >= 1)//超过26
		{
			startCell = QChar('A' + nStep - 1) + QString(QChar('A' + nLeft)) + QString::number(nRow + 1);
		}
		else{
			startCell = QChar('A' + nColumn) + QString::number(nRow + 1);
		}
		//end
		nStep = (nColumn + nColumnCount - 1) / 26;
		nLeft = (nColumn + nColumnCount - 1) % 26;
		if (nStep >= 1)//超过26
		{
			endCell = QChar('A' + nStep - 1) + QString(QChar('A' + nLeft)) + QString::number(nRow + nRowCount);
		}
		else{
			endCell = QChar('A' + nColumn + nColumnCount - 1) + QString::number(nRow + nRowCount);
		}

		QString mystr = startCell + ':' + endCell;

		emit cellSelected(mystr);
	}
	QTableWidget::mouseReleaseEvent(event);
}

void CSpreadSheet::moveEvent(QMoveEvent *event)
{
	event->MouseButtonRelease;
}

void CSpreadSheet::SetFontRight()
{
	QList<QTableWidgetItem *> items = selectedItems();
	if (!items.isEmpty())
	{
		foreach(QTableWidgetItem *pItem, items)
		{
			if (pItem)
			{
				pItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
				somethingChanged();
			}
		}
	}
}

//清理表格内容
void CSpreadSheet::clear(int newRow, int newColumn)
{
	setRowCount(0);
	setColumnCount(0);
	setRowCount(newRow);
	setColumnCount(newColumn);

	for (int i = 0; i < columnCount(); ++i) {
		QTableWidgetItem *item = new QTableWidgetItem;
		if (i < 26)
		{
			item->setText(QString(QChar('A' + i)));
		}
		else
		{
			item->setText(QString("%1%2").arg(QChar('A' + i / 26 - 1)).arg(QChar('A' + i % 26)));
		}

		setHorizontalHeaderItem(i, item);
	}

	for (int i = 0; i < rowCount(); i++)
	{
		setRowHeight(i, 25);
	}
	setCurrentCell(0, 0);
}

//返回指定单元格指针
QCell *CSpreadSheet::cell(int row, int column) const
{
	return dynamic_cast<QCell *>(item(row, column));
}

//返回指定单元格内容文本
QString CSpreadSheet::Text(int row, int column) const
{
	QCell *c = cell(row, column);
	if (c)
	{
		return c->text();
	}
	else
	{
		return "";
	}
}

//返回指定单元格公式
QString CSpreadSheet::Formula(int row, int column) const
{
	QCell *c = cell(row, column);
	if (c)
	{
		return c->Formula();
	}
	else
	{
		return "";
	}
}

//设置指定单元格公式
void CSpreadSheet::SetFormula(int row, int column,
	const QString &formula)
{
	QCell *c = cell(row, column);
	if (!c)
	{
		setItem(row, column, c);
	}
	c->SetFormula(formula);
}

//返回当前单元格位置，型如A12
QString CSpreadSheet::CurrentLocation() const
{
	int nCurrentColumn = currentColumn();
	int nCurrentRow = currentRow();
	int nStep = nCurrentColumn / 26;
	int nLeft = nCurrentColumn % 26;

	if (nStep >= 1)//超过26
	{
		return QChar('A' + nStep - 1) + QString(QChar('A' + nLeft)) + QString::number(currentRow() + 1);
	}
	else{
		return QChar('A' + currentColumn()) + QString::number(currentRow() + 1);
	}
}

QString CSpreadSheet::CurrentSelection()const
{
	int nRow = selectedRanges().first().topRow();

	int nColumn = selectedRanges().first().leftColumn();

	int nRowCount = selectedRanges().last().bottomRow() - selectedRanges().first().topRow() + 1;

	int nColumnCount = selectedRanges().last().rightColumn() - selectedRanges().first().leftColumn() + 1;

	if (nRowCount == 1 && nColumnCount == 1)
	{
		return CurrentLocation();
	}

	QString startCell = QChar('A' + nColumn) + QString::number(nRow + 1);

	QString endCell = QChar('A' + nColumn + nColumnCount - 1) + QString::number(nRow + nRowCount);

	QString mystr = startCell + ':' + endCell;

	return mystr;
}

//返回当前单元格内容
QString CSpreadSheet::CurrentFormula() const
{
	return Formula(currentRow(), currentColumn());
}

//有变动重写计算 并且发送modified消息
void CSpreadSheet::somethingChanged()
{
	if (m_bAutoRecalc)
		recalculate();
	emit modified();
}

//读文档
#if 0
bool CSpreadSheet::readFile(const QString &fileName)
{

	QFile file(fileName);

	if (!file.exists())
	{
		return false;
	}
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("Spreadsheet"),
			tr("Cannot read file %1:\n%2.")
			.arg(file.fileName())
			.arg(file.errorString()));
		QApplication::restoreOverrideCursor();
		return false;
	}
	QFileInfo info(file);
	if (info.suffix() == "xml")
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		//按照XML格式解析
		QXmlReader *nReader = new QXmlReader;
		if (!nReader->OnReadXmlFile(fileName))
		{
			QMessageBox::information(this, "ERROR", "XML文件打开错误");
			delete nReader;
			nReader = Q_NULLPTR;
			return false;
		}
		CTableInfo myInfo;
		myInfo = *(nReader->GetReportInfo());
		clear(myInfo.m_RowCount, myInfo.m_ColCount);
		//合并信息再次记录
		m_lstMergeCells.clear();
		m_lstMergeCells = myInfo.m_ItemList;
		CheckSpan();
		SetCellSpan(myInfo);
		m_TableData.m_nTableType = myInfo.m_nTableType;
		G_TABLE_TYPE = myInfo.m_nTableType;
		QMap<int, float>nRowHeight = myInfo.m_RowHeight;
		QMapIterator<int, float> i(nRowHeight);
		while (i.hasNext())
		{
			i.next();
			setRowHeight(i.key(), i.value());
		}
		QMap<int, float>nColWidth = myInfo.m_ColWidth;
		QMapIterator<int, float> j(nColWidth);
		while (j.hasNext())
		{
			j.next();
			setColumnWidth(j.key(), j.value());
		}
		quint16 nRow = 0;
		quint16 nColumn = 0;
		//QApplication::setOverrideCursor(Qt::WaitCursor);
		QList<CCellData>nData = nReader->GetCellList();
		foreach(CCellData nCellData, nData)
		{
			nRow = nCellData.m_nRowIndex;
			nColumn = nCellData.m_nColIndex;
			SetCellFormat(nRow, nColumn, nCellData);
		}
		QApplication::restoreOverrideCursor();
	}
	else
	{
		//格式错误
		QMessageBox::warning(this, "ERROR", "格式错误，未知格式");
		QApplication::restoreOverrideCursor();
		return false;
	}
	QApplication::restoreOverrideCursor();
	return true;
}

//写文档
bool CSpreadSheet::writeFile(const QString &fileName)
{
	QFile file(fileName);
	if (fileName.isEmpty())
	{
		return false;
	}
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, tr("Spreadsheet"),
			tr("Cannot write file %1:\n%2.")
			.arg(file.fileName())
			.arg(file.errorString()));
		return false;
	}
	m_TableData.m_szTableName = fileName;

	GetGernelTableInfo();

	QXmlWriter *nFunc = new QXmlWriter;
	nFunc->SetReportInfo(&m_TableData);

	QList<CCellData>nData;
	QApplication::setOverrideCursor(Qt::WaitCursor);
	//4、数据表格内容写入
	for (int row = 0; row < rowCount(); ++row)
	{
		for (int column = 0; column < columnCount(); ++column)
		{
			QCell *mycell = cell(row, column);
			if (mycell)
			{
				CCellData newData = cell(row, column)->GetFormatData();
				QString str = Formula(row, column);
				if (!str.isEmpty())
				{
					nData.append(newData);
				}
			}
		}
	}
	nFunc->SetReportCell(nData);
	nFunc->OnWriteXmlFile(fileName);
	QApplication::restoreOverrideCursor();
	return true;
}
#endif

//剪切
void CSpreadSheet::cut()
{
	copy();
	del();
}

//复制
void CSpreadSheet::copy()
{
	QTableWidgetSelectionRange range = QTableWidgetSelectionRange(this->selectedRanges().first().topRow(),
		this->selectedRanges().first().leftColumn(),
		this->selectedRanges().last().bottomRow(),
		this->selectedRanges().last().rightColumn());
	QString str;
	for (int i = 0; i < range.rowCount(); ++i)
	{
		if (i > 0)
			str += "\n";
		for (int j = 0; j < range.columnCount(); ++j)
		{
			if (j > 0)
				str += "\t";
			str += Formula(range.topRow() + i, range.leftColumn() + j);
		}
	}
	QApplication::clipboard()->setText(str);
}

//粘贴
void CSpreadSheet::paste()
{
	QTableWidgetSelectionRange range = selectedRange();
	QString str = QApplication::clipboard()->text();
	QStringList rows = str.split('\n');
	int numRows = rows.count();
	int numColumns = rows.first().count('\t') + 1;
	if (range.rowCount() * range.columnCount() != 1
		&& (range.rowCount() != numRows
		|| range.columnCount() != numColumns))
	{
		QMessageBox::information(this, tr("report"),
			tr("The information cannot be pasted because the "
			"copy and paste aren't the same size"));
		return;
	}
	for (int i = 0; i < numRows; ++i)
	{
		QStringList columns = rows[i].split('\t');
		if (columns.size() == 1)
		{
			if (columns[0].isEmpty())
			{
				continue;
			}
		}

		for (int j = 0; j < numColumns; ++j)
		{
			int row = range.topRow() + i;
			int column = range.leftColumn() + j;

			QCell *c = cell(row, column);
			if (!c)
			{
				c = new QCell;
				setItem(row, column, c);
			}
			if (row < rowCount() && column < columnCount())
			{
				SetFormula(row, column, columns[j]);
			}
		}
	}
	somethingChanged();
}

//删除
void CSpreadSheet::del()
{
	QList<QTableWidgetItem *> items = selectedItems();
	if (!items.isEmpty())
	{
		foreach(QTableWidgetItem *item, items)
			delete item;
		somethingChanged();
	}
}

//选择当前行
void CSpreadSheet::selectCurrentRow()
{
	selectRow(currentRow());
}

//选择当前列
void CSpreadSheet::selectCurrentColumn()
{
	selectColumn(currentColumn());
}

//重新计算
void CSpreadSheet::recalculate()
{
	for (int row = 0; row < RowCount; ++row)
	{
		for (int column = 0; column < ColumnCount; ++column)
		{
			if (cell(row, column))
				cell(row, column)->SetDirty();
		}
	}
	viewport()->update();
}

//设置是否自动计算
void CSpreadSheet::SetAutoRecalculate(bool recalc)
{
	m_bAutoRecalc = recalc;
	if (m_bAutoRecalc)
		recalculate();
}

//选择范围
QTableWidgetSelectionRange CSpreadSheet::selectedRange() const
{
	QList<QTableWidgetSelectionRange> ranges = selectedRanges();
	if (ranges.isEmpty())
		return QTableWidgetSelectionRange();
	return ranges.first();
}

//排序
void CSpreadSheet::sort(const SpreadsheetCompare &compare)
{
	QList<QStringList> rows;
	QTableWidgetSelectionRange range = selectedRange();
	int i;
	for (i = 0; i < range.rowCount(); ++i)
	{
		QStringList row;
		for (int j = 0; j < range.columnCount(); ++j)
		{
			row.append(Formula(range.topRow() + i,
				range.leftColumn() + j));
		}
		rows.append(row);
	}
	qStableSort(rows.begin(), rows.end(), compare);

	for (i = 0; i < range.rowCount(); ++i)
	{
		for (int j = 0; j < range.columnCount(); ++j)
		{
			SetFormula(range.topRow() + i, range.leftColumn() + j,
				rows[i][j]);

		}
	}
	clearSelection();
	somethingChanged();
}

//往后查找
void CSpreadSheet::FindNext(const QString &str, Qt::CaseSensitivity cs)
{
	int row = currentRow();
	int column = currentColumn() + 1;
	while (row < RowCount)
	{
		while (column < ColumnCount)
		{
			if (Text(row, column).contains(str, cs))
			{
				clearSelection();
				setCurrentCell(row, column);
				activateWindow();
				return;
			}
			++column;
		}
		column = 0;
		++row;
	}
	QApplication::beep();
}

//往前查找
void CSpreadSheet::FindPrevious(const QString &str, Qt::CaseSensitivity cs)
{
	int row = currentRow();
	int column = currentColumn() - 1;
	while (row >= 0)
	{
		while (column >= 0)
		{
			if (Text(row, column).contains(str, cs))
			{
				clearSelection();
				setCurrentCell(row, column);
				activateWindow();
				return;
			}
			--column;
		}
		column = ColumnCount - 1;
		--row;
	}
	QApplication::beep();
}

//不合理  只按第一列来排
bool SpreadsheetCompare::operator ()(const QStringList &row1,
	const QStringList &row2) const
{
	qDebug() << ascending[0] << ascending[1] << ascending[2];
	for (int i = 0; i < KeyCount; ++i)
	{
		int column = keys[i];
		if (column != -1)
		{
			if (row1[column] != row2[column])
			{
				if (ascending[i])
					return row1[column] < row2[column];
				else
					return row1[column] > row2[column];
			}
		}
	}
	return false;
}

#if 0
void CSpreadSheet::sleep(int msec)
{
	QTime dieTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
#endif

void CSpreadSheet::CheckMergeInfo()
{
	QList<CMergeInfo>::iterator it = m_lstMergeCells.begin();
	for (; it != m_lstMergeCells.end(); ++it)
	{
		if (((*it).m_nRowCount == 1) && ((*it).m_nColCount == 1))
		{
			it = m_lstMergeCells.erase(it);
			continue;
		}
	}
}
void CSpreadSheet::RecoderMergeInfo(CMergeInfo & m_data)
{
	if ((m_data.m_nRowCount == 1) && (m_data.m_nColCount == 1))
	{
	}
	else
	{
		m_lstMergeCells.append(m_data);
	}
	foreach(CMergeInfo temp, m_lstMergeCells)
	{
		if (temp.m_nRowCount == 1 || temp.m_nColCount == 1)
		{
			qDebug("a just fair one");
		}
	}
}

//未记录任何格式的输入输出
void CSpreadSheet::ProduceHtmlPackage()
{
	const QString htmlFileName = QString("%1/%2").arg(qApp->applicationDirPath()).arg("myTable.html");

	QFile file(htmlFileName);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		//  MSG(QString("Can`t create file %1").arg(htmlFileName));
		//return false;
		QMessageBox::information(this, "", "");
	}

	QTextStream out(&file);

	const int rowCount = this->rowCount();
	const int columnCount = this->columnCount();

	out << "<html>\n"
		"<head>\n"
		"<meta Content=\"Text/html; charset=Windows-1251\">\n"
		<< QString("<title>%1</title>\n").arg("tytyty")
		<< "</head>\n"
		"<body bgcolor=#ffffff link=#5000A0>\n"
		"<table border=1 cellspacing=0 cellpadding=2>\n";

	// headers
	out << "<tr bgcolor=#f0f0f0>";
	for (int column = 0; column < columnCount; column++)
		if (!this->isColumnHidden(column))
			out << QString("<th>%1</th>").arg(this->model()->headerData(column, Qt::Horizontal).toString());
	out << "</tr>\n";
	file.flush();

	// data table
	for (int row = 0; row < rowCount; row++) {
		out << "<tr>";
		for (int column = 0; column < columnCount; column++) {
			if (!this->isColumnHidden(column)) {
				QString data = this->model()->data(this->model()->index(row, column)).toString().simplified();
				out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
			}
		}
		out << "</tr>\n";
	}
	out << "</table>\n"
		"</body>\n"
		"</html>\n";

	file.close();
}

int CSpreadSheet::IfInRange(int row, int column)
{
	foreach(CMergeInfo temp, m_lstMergeCells)
	{
		//merge start point 
		if (temp.m_nRow == row&&temp.m_nColumn == column)
		{
			return point_start;
		}
		//check row & column 
		if (temp.m_nRow <= row
			&& (temp.m_nRow + temp.m_nRowCount - 1) >= row
			&&temp.m_nColumn <= column
			&& (temp.m_nColumn + temp.m_nColCount - 1) >= column)
		{
			return point_in_range;
		}
	}
	return point_normal;
}

//格式化输出html
QString &CSpreadSheet::ProduceHtml()
{
	using namespace DataConvert;

	int m_rowCount = this->rowCount();
	int m_colCount = this->columnCount();

	int width = m_colCount * 30;

	QString html_header = tr("<html><head></head><body><TABLE width=%1  BORDER=1 >").arg(width);

	QString html_body = NULL;

	int nEndRow = GetEndRow();
	int nEndColumn = GetEndColumn();
	int ok;
	for (int i = 0; i < nEndRow; i++)
	{
		html_body += "<tr>";
		for (int j = 0; j < nEndColumn; j++)
		{
			ok = IfInRange(i, j);
			switch (ok)
			{
			case point_normal:
			{
				QCell *c = cell(i, j);
				if (!c)
				{
					html_body += QString("<td></td>");
				}
				else
				{
					Q_ASSERT(c);
					int align = cell(i, j)->textAlignment();
					QString str = AlignToString(align);
					html_body += QString("<td  %1 bgcolor=%2 > %3 </td>").
						arg(AlignToString(cell(i, j)->textAlignment())).
						arg(QVariant(cell(i, j)->backgroundColor()).toString()).
						arg(FontToString(cell(i, j)->font(),
						cell(i, j)->data(Qt::DisplayRole).toString(),
						cell(i, j)->textColor()));
				}

				break;
			}
			case  point_start:
			{
				html_body += QString("<td rowspan= %1 colspan=%2 bgcolor=%3  %4> %5</td>").
					arg(cell(i, j)->m_ItemData.m_MergeRecoder.m_nRowCount).
					arg(cell(i, j)->m_ItemData.m_MergeRecoder.m_nColCount).
					arg(QVariant(cell(i, j)->backgroundColor()).toString()).
					arg(AlignToString(cell(i, j)->textAlignment())).
					arg(FontToString(cell(i, j)->font(),
					cell(i, j)->data(Qt::DisplayRole).toString(),
					cell(i, j)->textColor()));
				break;
			}
			case  point_in_range:
				break;
			default:
				break;
			}
		}
		html_body += "</tr>";
	}
	QString html_end = "</body></html>";

	QString html_all = html_header + html_body + html_end;

	m_szSheetAsHtml = html_all;

	return m_szSheetAsHtml;
}
void CSpreadSheet::SavePageOrExcel(const QString &str)
{
	QTextEdit *textEdit = new QTextEdit;
	textEdit->setText(str);

	QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
		QString(), tr("EXCEL files (*.xls);;ODS files (*.ods);;ODF files (*.odt);;HTML-Files (*.htm *.html);;All Files (*)"));

	if (fn.isEmpty())
		return;
	if (!(fn.endsWith(".xls", Qt::CaseInsensitive) || fn.endsWith(".odt", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive) || fn.endsWith(".ods", Qt::CaseInsensitive)))
		fn += ".xls"; // default

	if (fn.endsWith(".odt", Qt::CaseInsensitive))
	{
		QTextDocumentWriter writer(fn);
		writer.write(textEdit->document());
	}
	else
	{
		QFile index(fn);
		if (index.open(QFile::WriteOnly | QIODevice::Text))
		{
			QTextStream liu(&index);
			liu.setCodec("UTF-8");
			liu << textEdit->document()->toHtml("UTF-8");
		}
	}
}

void CSpreadSheet::SetFontColor()
{
	QPalette palette = cell(currentRow(), currentColumn())->textColor();
	const QColor &color = QColorDialog::getColor(palette.color(QPalette::Base), this);

	QList<QTableWidgetItem *> items = selectedItems();
	if (!items.isEmpty())
	{
		foreach(QTableWidgetItem *pItem, items)
		{
			if (pItem&&color.isValid())
			{
				pItem->setTextColor(color);
				somethingChanged();
			}
		}
	}
}
int CSpreadSheet::GetRowCount()
{
	return rowCount();
}
int CSpreadSheet::GetColumnCount()
{
	return columnCount();
}
QList<CMergeInfo> & CSpreadSheet::GetSpanData()
{
	return m_lstMergeCells;
}

CTableInfo &CSpreadSheet::GetGernelTableInfo()
{
	m_TableData.m_RowCount = rowCount();
	m_TableData.m_ColCount = columnCount();
	if (rowCount() && columnCount())
	{
		m_TableData.m_nRowHeigth = height() / rowCount();
		m_TableData.m_nColumnWidth = width() / columnCount();
	}
	else
	{
		m_TableData.m_nRowHeigth = 0;
		m_TableData.m_nColumnWidth = 0;
	}
	int nEndRow = GetEndRow();

	for (int i = 0; i < nEndRow; ++i)
	{
		m_TableData.m_RowHeight[i] = rowHeight(i);
	}

	int nEndCol = GetEndColumn();

	for (int i = 0; i < nEndCol; ++i)
	{
		m_TableData.m_ColWidth[i] = columnWidth(i);
	}
	CheckMergeInfo();

	m_TableData.m_MergeItemList = m_lstMergeCells;

	return m_TableData;
}

void CSpreadSheet::SetTableInfo(const CTableInfo &_tableData)
{
	//单元格信息
	m_lstMergeCells = _tableData.m_MergeItemList;
	foreach(CMergeInfo temp, m_lstMergeCells)
	{
		setSpan(temp.m_nRow, temp.m_nColumn, temp.m_nRowCount, temp.m_nColCount);
	}
}

void  CSpreadSheet::SetCellFormat(int row, int column, CCellData& cellContent)
{
	QCell *c = cell(row, column);
	if (!c)
	{
		c = new QCell;
		setItem(row, column, c);
	}
	c->SetFormatData(cellContent);
	c->SetDate(m_Date);
}

void CSpreadSheet::SetCellSpan(const CTableInfo&data)
{
	QList<CMergeInfo> mList = data.m_MergeItemList;

	foreach(CMergeInfo tempData, mList)
	{
		setSpan(tempData.m_nRow, tempData.m_nColumn, tempData.m_nRowCount, tempData.m_nColCount);

		QCell *c = static_cast<QCell *>(cell(tempData.m_nRow, tempData.m_nColumn));
		if (!c)
		{
			c = new QCell;
			setItem(tempData.m_nRow, tempData.m_nColumn, c);
		}
		CMergeInfo recoder;
		recoder.m_nRow = tempData.m_nRow;
		recoder.m_nColumn = tempData.m_nColumn;
		recoder.m_nRowCount = tempData.m_nRowCount;
		recoder.m_nColCount = tempData.m_nColCount;

		c->SetSpanData(recoder);
		qDebug("row: %d columu: %d rowCount: %d colCount: %d ", tempData.m_nRow, tempData.m_nColumn, tempData.m_nRowCount, tempData.m_nColCount);
	}
}

//获取打印行列
int CSpreadSheet::GetEndColumn()
{
	for (int i = columnCount() - 1; i >= 0; i--)
	{
		for (int j = 0; j < rowCount(); j++)
		{
			QCell *c = cell(j, i);
			if (c)
			{
				return i + 1;
			}
		}
	}
	return 0;
}

int CSpreadSheet::GetEndRow()
{
	for (int i = rowCount() - 1; i >= 0; i--)
	{
		for (int j = 0; j < columnCount(); j++)
		{
			QCell *c = cell(i, j);
			if (c)
			{
				return i + 1;
			}
		}
	}
	return 0;
}

int CSpreadSheet::GetTime(int index, int row, int column)
{
	int m_time = 0;

	switch (index)
	{
	case 0://day   1-24 转换为分钟
	{
		QCell *c = cell(row, column);
		if (!c)
		{
			AQP::warning(this, tr("无时间列"), tr("请设置时间列!!!"));
			//  AQP::warning(this,tr("No Time Column"),tr("Please reset Time COlumn!!!"));
			return -1;
		}

		QString data = cell(row, column)->data(Qt::EditRole).toString();

		if (data.contains(':'))
		{
			m_time = data.section(':', 0, 0).toInt() * 60 + data.section(':', 1, 1).toInt();
		}
	}
	break;
	case 1://month
	{
		QCell *c = cell(row, column);
		if (!c)
		{
			AQP::warning(this, tr("无时间列"), tr("请设置时间列!!!"));
			//   AQP::warning(this,tr("No Time Column"),tr("Please reset Time COlumn!!!"));
			return -1;
		}
		QString data = cell(row, column)->data(Qt::EditRole).toString();
		m_time = data.toInt();
	}
	break;
	case 2://year
	{
		QCell *c = cell(row, column);
		if (!c)
		{
			AQP::warning(this, tr("无时间列"), tr("请设置时间列!!!"));
			// AQP::warning(this,tr("No Time Column"),tr("Please reset Time COlumn!!!"));
			return -1;
		}
		QString data = cell(row, column)->data(Qt::EditRole).toString();
		m_time = data.toInt();
	}
	break;
	case 3://special
		break;
	default:
		break;
	}

	return m_time;
}

/*! \fn bool CSpreadSheet::ReadGeneralInfo(const QString &fileName)
*********************************************************************************************************
** \brief CSpreadSheet::ReadGeneralInfo
** \details 读取文件大概信息
** \param fileName
** \return bool
** \author xingzhibing
** \date 2016年8月9日
** \note
********************************************************************************************************/
bool CSpreadSheet::ReadGeneralInfo(const QString &fileName)
{
	QFile file(fileName);

	if (!file.exists())
	{// todo!  输出log日志
		return false;
	}

	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("Spreadsheet"),
			tr("Cannot read file %1:\n%2.")
			.arg(file.fileName())
			.arg(file.errorString()));

		// todo!  输出log日志
		return false;
	}
	QFileInfo info(file);
	if (info.suffix() == "xml")
	{
		//按照XML格式解析
		QXmlReader *nReader = new QXmlReader;
		if (!nReader->OnReadXmlFile(fileName))
		{
			QMessageBox::information(this, "ERROR", "XML文件打开错误");
			return false;
		}
		CTableInfo myInfo;
		myInfo = *(nReader->GetReportInfo());
		m_TableData.m_nTableType = myInfo.m_nTableType;
		G_TABLE_TYPE = myInfo.m_nTableType;
	}
	else
	{
		//格式错误
		QMessageBox::warning(this, "ERROR", "格式错误，未知格式");
		return false;
	}
	file.close();
	return true;
}

/*! \fn void CSpreadSheet::SetChartView(CCtrlData* pData)
********************************************************************************************************* 
** \brief CSpreadSheet::SetChartView 
** \details 曲线绘制
** \return void 
** \author xingzhibing
** \date 2016年8月23日 
** \note 
********************************************************************************************************/
bool CSpreadSheet::SetChartView(CCtrlData* pData)
{
	Q_ASSERT(pData);
	if (!pData)
	{
		return false;
	}

	int nRowIndex = currentRow();

	int nColIndex = currentColumn();

	pData->SetCtrlPosition(nRowIndex, nColIndex);

	CCtrlParaDialog *dialog = new CCtrlParaDialog(pData);

	connect(this, SIGNAL(cellSelected(const QString&)), dialog, SLOT(slot_OnDealData(const QString&)));

	bool nRet = false;

	//获取相应数据
	if (QDialog::Accepted == dialog->exec())
	{
		nRet = true;
	}
	else
	{
		nRet = false;
		return nRet;
	}

	QVector<CCtrlGernalValue*> vecData;

	for (int i = 0; i < 5; ++i)
	{
		CCtrlGernalValue* pVecData = new CCtrlGernalValue();
		pVecData->m_szName = tr("Converter#%1").arg(i);
		pVecData->m_TagName = tr("TagName%2").arg(i);
		pVecData->m_vecValue.append(CPoint(rand() % 10, rand() % 10));
		pVecData->m_vecValue.append(CPoint(rand() % 20, rand() % 20));
		pVecData->m_vecValue.append(CPoint(rand() % 30, rand() % 30));
		pVecData->m_vecValue.append(CPoint(rand() % 40, rand() % 40));
		pVecData->m_vecValue.append(CPoint(rand() % 50, rand() % 50));
		vecData.append(pVecData);
	}
	pData->SetCtrlName("CtrlChartData");

	pData->SetCtrlData(vecData);

	dialog->deleteLater();

	CChartView* pView = new CChartView(pData);

	this->setCellWidget(nRowIndex, nColIndex, pView);

	return nRet;
}

/*! \fn void CSpreadSheet::SetBarView(CCtrlData* pData)
********************************************************************************************************* 
** \brief CSpreadSheet::SetBarView 
** \details 棒状图绘制
** \return void 
** \author xingzhibing
** \date 2016年8月23日 
** \note 
********************************************************************************************************/
bool CSpreadSheet::SetBarView(CCtrlData* pData)
{
	Q_ASSERT(pData);
	if (!pData)
	{
		return false;
	}

	int nRowIndex = currentRow();

	int nColIndex = currentColumn();

	pData->SetCtrlPosition(nRowIndex, nColIndex);
	
	CCtrlParaDialog *dialog = new CCtrlParaDialog(pData);

	connect(this, SIGNAL(cellSelected(const QString&)), dialog, SLOT(slot_OnDealData(const QString&)));

	bool nRet = false;

	//获取相应数据
	if (QDialog::Accepted == dialog->exec())
	{
		nRet = true;
	}
	else
	{
		nRet = false;
		return nRet;
	}

	QVector<CCtrlGernalValue*> vecData;

	for (int i = 0; i < 5; ++i)
	{
		CCtrlGernalValue* pVecData = new CCtrlGernalValue();
		pVecData->m_szName = tr("Converter#%1").arg(i);
		pVecData->m_TagName = tr("TagName%2").arg(i);
		pVecData->m_vecValue.append(CPoint(rand() % 100, rand() / 100));
		pVecData->m_vecValue.append(CPoint(rand() % 150, rand() / 150));
		pVecData->m_vecValue.append(CPoint(rand() % 10, rand() / 10));
		pVecData->m_vecValue.append(CPoint(rand() % 280, rand() / 280));
		pVecData->m_vecValue.append(CPoint(rand() % 380, rand() / 380));
		vecData.append(pVecData);
	}
	pData->SetCtrlName("CtrlBarData");

	pData->SetCtrlData(vecData);

	dialog->deleteLater();

	CBarView* pView = new CBarView(pData);

	this->setCellWidget(nRowIndex, nColIndex, pView);

	return nRet;
}

/*! \fn void CSpreadSheet::SetPieView(CCtrlData* pData)
********************************************************************************************************* 
** \brief CSpreadSheet::SetPieView 
** \details 饼状图绘制
** \return void 
** \author xingzhibing
** \date 2016年8月23日 
** \note 
********************************************************************************************************/
bool CSpreadSheet::SetPieView(CCtrlData* pData)
{
	Q_ASSERT(pData);
	if (!pData)
	{
		return false;
	}
	int nRowIndex = currentRow();

	int nColIndex = currentColumn();

	pData->SetCtrlPosition(nRowIndex, nColIndex);

	CCtrlParaDialog *dialog=new CCtrlParaDialog(pData);

	connect(this, SIGNAL(cellSelected(const QString&)), dialog, SLOT(slot_OnDealData(const QString&)));

	bool nRet = false;

	//获取相应数据
	if (QDialog::Accepted == dialog->exec())
	{
		nRet = true;
	}
	else
	{
		nRet = false;
		return nRet;
	}
	//TODO  以后此处数据为解析好数据直接写进

	//在编辑状态下，模拟数据显示
	QVector<CCtrlGernalValue*> vecData;

	for (int i = 0; i < 5;++i )
	{
		CCtrlGernalValue* pVecData = new CCtrlGernalValue();
		pVecData->m_szName  = tr("Converter#%1").arg(i);
		pVecData->m_TagName = tr("TagName%2").arg(i);
		pVecData->m_vecValue.append(CPoint(rand() % 100, rand() / 100));
		pVecData->m_vecValue.append(CPoint(rand() % 150, rand() / 150));
		pVecData->m_vecValue.append(CPoint(rand() % 180, rand() / 180));
		vecData.append(pVecData);
	}
	pData->SetCtrlName("CtrlPieData");

	pData->SetCtrlData(vecData);

	//界面显示控件
	CPieView* pView = new CPieView(pData);

	this->setCellWidget(nRowIndex, nColIndex, pView);


	return nRet;
}
