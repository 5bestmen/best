/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  logicpropertywgt.cpp
文件实现功能 :  双击逻辑块 显示的属性widget
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 逻辑编辑
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2017.1.4
*/
#include <QComboBox>
#include <QSpinBox>
#include <QDebug>
#include "logicmodel.h"
#include "logicpropertywgt.h"
#include "seqdefine.h"
#include "pincallwgt.h"
#include "scadastudio/icore.h"
#include "variant.h"
#include "logic_module.h"



/*! \fn CLogicPropertyWgt::CLogicPropertyWgt(QWidget *parent)
*********************************************************************************************************
** \brief CLogicPropertyWgt::CLogicPropertyWgt(QWidget *parent)
** \details 构造函数
** \param  父类
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CLogicPropertyWgt::CLogicPropertyWgt(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	setWindowTitle(tr("Attr"));
    m_nItemTypeCode = 0;
    connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_Cancel()));

    ui.tableWidget->setColumnWidth(1, 300);
    ui.tableWidget->verticalHeader()->setHidden(true);
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui.tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(Slot_DoubleClickTableItem(QTableWidgetItem *)));
}

CLogicPropertyWgt::~CLogicPropertyWgt()
{

}

/*! \fn CLogicPropertyWgt::Slot_Conform()
*********************************************************************************************************
** \brief CLogicPropertyWgt::Slot_Conform()
** \details 点击确认后的槽函数
** \param  
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CLogicPropertyWgt::Slot_Conform()
{
    //发送注释信息 供逻辑块更新内容显示
    emit Signal_ConformComment(ui.textEdit->toPlainText());

    //发送是否显示eno信息
    if (ui.checkBox->checkState() == Qt::Checked)
    {
        emit Signal_EnoDisplay(1);
    }
    else
    {
        emit Signal_EnoDisplay(0);
    }

	//绑点信息
	emit Signal_ChangeBindValue(m_mapBindValues);
    this->close();
}

void CLogicPropertyWgt::Slot_Cancel()
{
    this->close();
}


/*! \fn CLogicPropertyWgt::Slot_DoubleClickTableItem(QTableWidgetItem *pItem)
*********************************************************************************************************
** \brief CLogicPropertyWgt::Slot_DoubleClickTableItem(QTableWidgetItem *pItem)
** \details 双击表格触发槽函数   待定
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CLogicPropertyWgt::Slot_DoubleClickTableItem(QTableWidgetItem *pItem)
{
    //框架提供
    if (m_nItemTypeCode == LOGIC_CALLSR)
    {
		//子节点控件  不需要梆点
        CPinCallWgt *pCallWgt = new CPinCallWgt;
        pCallWgt->exec();
    }
	else
	{
		m_arrDataType.clear();

		std::string strNodeTagname;
		std::string strTagname;
		std::string strFilename;
		//
		m_arrDataType.push_back(DT_NULL);
		m_arrDataType.push_back(DT_BOOLEAN);
		m_arrDataType.push_back(DT_CHAR);
		m_arrDataType.push_back(DT_BYTE);
		m_arrDataType.push_back(DT_SHORT);
		m_arrDataType.push_back(DT_WORD);
		m_arrDataType.push_back(DT_INT);
		m_arrDataType.push_back(DT_DWORD);
		m_arrDataType.push_back(DT_LLONG);
		m_arrDataType.push_back(DT_LWORD);
		m_arrDataType.push_back(DT_FLOAT);
		m_arrDataType.push_back(DT_DOUBLE);
		m_arrDataType.push_back(DT_STRING);
		m_arrDataType.push_back(DT_BLOCK);
		m_arrDataType.push_back(DT_MAX);
		//
		bool bSelectFlag = GetIMainInterface()->SelectTag(m_arrDataType, true, strNodeTagname, strTagname, strFilename);

		if (bSelectFlag)
		{
			QString strResult = QString(strNodeTagname.c_str()) + "." + QString(strTagname.c_str()) + "." + QString(strFilename.c_str());
			UpdateBindInfo(strResult);
		}
	}
}

/*! \fn CLogicPropertyWgt::SetItemTypeCode(int nCode)
*********************************************************************************************************
** \brief CLogicPropertyWgt::SetItemTypeCode(int nCode)
** \details 设置类型编码
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CLogicPropertyWgt::SetItemTypeCode(int nCode)
{
    m_nItemTypeCode = nCode;
}

/*! \fn CLogicPropertyWgt::SetChildLst(const QStringList &lstChildNames)
*********************************************************************************************************
** \brief CLogicPropertyWgt::SetChildLst(const QStringList &lstChildNames)
** \details 设置子程序列表
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CLogicPropertyWgt::SetChildLst(const QStringList &lstChildNames)
{
    m_lstChildNames = lstChildNames;
}

/*! \fn CLogicPropertyWgt::UpdateItemDetail(std::vector<std::shared_ptr<CBinProperty>> lstInput, std::vector<std::shared_ptr<CBinProperty>> lstOutput)
*********************************************************************************************************
** \brief CLogicPropertyWgt::UpdateItemDetail(std::vector<std::shared_ptr<CBinProperty>> lstInput, std::vector<std::shared_ptr<CBinProperty>> lstOutput)
** \details lstInput:输入端子信息   lstOutput:输出端子信息
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CLogicPropertyWgt::UpdateItemDetail(std::vector<std::shared_ptr<CBinProperty>> lstInput, std::vector<std::shared_ptr<CBinProperty>> lstOutput)
{
	m_mapBindValues.clear();
	m_pBinPro = lstInput;

    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);

    //输入行
    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(tr("Input"));

    ui.tableWidget->insertRow(0);
    ui.tableWidget->setItem(0, 0, item0);
    ui.tableWidget->setRowHeight(0, 25);

    int nRowNum = 1;
    //遍历端子信息
    for each (std::shared_ptr<CBinProperty> pinPro in lstInput)
    {
        ui.tableWidget->insertRow(nRowNum);
        ui.tableWidget->setRowHeight(nRowNum, 25);

        QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setText(pinPro->GetName());

        ui.tableWidget->setItem(nRowNum, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem;
        if (pinPro->GetPinType() == 1)
        {
            //output
            item1->setText("F" + QString::number(pinPro->GetOtherModel()->GetCreateNum())
                + "_(" + QString::number(pinPro->GetOtherModel()->GetExecNum()) +
                +")." + pinPro->GetOtherModel()->GetOutPinName());

        }
        else if (pinPro->GetPinType() == 2)
        {
            //元件
            item1->setText(pinPro->GetCellPro()->GetCellName());
        }
        else if (pinPro->GetPinType() == 3)
        {
            item1->setText(tr("null"));
        }

        ui.tableWidget->setItem(nRowNum, 1, item1);

        if (nRowNum == 1)
        {
            //EN
            QTableWidgetItem *item2 = new QTableWidgetItem;
            item2->setText(tr("Enable"));
            ui.tableWidget->setItem(nRowNum, 2, item2);
        }
        else
        {
            if (m_nItemTypeCode >= LOGIC_AND && m_nItemTypeCode <= LOGIC_XOR)
            {
                //逻辑运算
                QTableWidgetItem *item2 = new QTableWidgetItem;
                item2->setText(tr("Operator Bit %1").arg(nRowNum-1));
                ui.tableWidget->setItem(nRowNum, 2, item2);

            }
            else if (m_nItemTypeCode == LOGIC_CALLSR)
            {
                //子程序
                QTableWidgetItem *item2 = new QTableWidgetItem;
                item2->setText(tr("Child Name"));
                ui.tableWidget->setItem(nRowNum, 2, item2);
            }
        }

        if ( m_bEnoFlag == false && nRowNum == 1)
        {
            ui.tableWidget->setRowHidden(nRowNum, true);
        }
        else
        {

        }
        nRowNum++;
    }

    //输出行
    int nOutRow = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(nOutRow);
    ui.tableWidget->setRowHeight(nOutRow, 25);

    QTableWidgetItem *itemOut = new QTableWidgetItem;
    itemOut->setText(tr("Output"));
    ui.tableWidget->setItem(nOutRow, 0, itemOut);

    int nOutNums = 0;

    for each (std::shared_ptr<CBinProperty> tPinPro in lstOutput)
    {
        ui.tableWidget->insertRow(nOutRow + nOutNums + 1);

        ui.tableWidget->setRowHeight(nOutRow + nOutNums + 1, 25);

        QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setText(tPinPro->GetName());
        ui.tableWidget->setItem(nOutRow + nOutNums + 1, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem;

        if (tPinPro->GetPinType() == 2)
        {
            //元件
            item1->setText(tPinPro->GetCellPro()->GetCellName());
        }
        else if (tPinPro->GetPinType() == 3)
        {
            item1->setText(tr("null"));
        }

        ui.tableWidget->setItem(nOutRow + nOutNums + 1, 1, item1);

        if (nOutNums == 0)
        {
            //ENO
            QTableWidgetItem *item2 = new QTableWidgetItem;
            item2->setText(tr("Enable Output"));
            ui.tableWidget->setItem(nOutRow + nOutNums + 1, 2, item2);
        }
        else
        {
            if (m_nItemTypeCode >= LOGIC_AND && m_nItemTypeCode <= LOGIC_XOR)
            {
                //逻辑运算
                QTableWidgetItem *item2 = new QTableWidgetItem;
                item2->setText(tr("Status Output"));
                ui.tableWidget->setItem(nOutRow + nOutNums + 1, 2, item2);

            }
        }

        //是否显示eno信息
        if (m_bEnoFlag == false && nOutNums == 0)
        {
            ui.tableWidget->setRowHidden(nOutRow + nOutNums + 1, true);
        }

        nOutNums++;
    }
}

/*! \fn CLogicPropertyWgt::SetEnoFlag(bool bEnoFlag)
*********************************************************************************************************
** \brief CLogicPropertyWgt::SetEnoFlag(bool bEnoFlag)
** \details 设置ENO显示标识
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CLogicPropertyWgt::SetEnoFlag(bool bEnoFlag)
{
    m_bEnoFlag = bEnoFlag;
}

/*! \fn CLogicPropertyWgt::SetComment(const QString &strComment)
*********************************************************************************************************
** \brief CLogicPropertyWgt::SetComment(const QString &strComment)
** \details 设置
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CLogicPropertyWgt::SetComment(const QString &strComment)
{
    ui.textEdit->setText(strComment);
}

void CLogicPropertyWgt::UpdateBindInfo(const QString &strBindValue)
{
	//当前行
	int nRow = ui.tableWidget->currentRow();
	if (nRow < 0)
	{
		return;
	}

	//设置绑定值
	ui.tableWidget->item(nRow, 1)->setText(strBindValue);
	//端子名称
	QString strItemName = ui.tableWidget->item(nRow, 0)->text();

	for (auto item : m_pBinPro)
	{
		//输入可以绑点  输出不可以绑点
		if (item->GetName() == strItemName && item->GetPinType() !=1)
		{
			//当前端子
// 			item->SetPinType(2);
// 			if (item->GetCellPro() == nullptr)
// 			{
// 				item->CreateCellPro()->SetCellName(strBindValue);
// 			}
// 			else
// 			{
// 				item->GetCellPro()->SetCellName(strBindValue);
// 			}

			//更改的绑点
			m_mapBindValues[item->GetPinIndex()] = strBindValue;
			
		}
	}
}
