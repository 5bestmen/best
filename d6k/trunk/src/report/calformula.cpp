#include <QFocusEvent>
#include "calformula.h"
#include "report.h"



QCalFormula::QCalFormula(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);   
    InitData();
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    setFixedSize(this->width(), this->height());  
}

QCalFormula::~QCalFormula()
{

}
void QCalFormula::slot_on_ok()
{
    foreach(QLineEdit *temp,m_Edit)
    {
       if (temp->isEnabled()&&!(temp->text().isEmpty()))
       {
           m_DataList.append(temp->text());
       }
    }
    
    QString str=m_DataList.join(",");


    int  index=ui.listWidget->currentRow();

    switch(index)
    {
    case PLUS:
        {
            m_szResult=QString("=PLUS(%1)").arg(str);
        }
        break;
    case MINUS:
        {
            m_szResult=QString("=MINUS(%1)").arg(str);
        }
        break;
    case MULTIPY:
        {
            m_szResult=QString("=MULTIPY(%1)").arg(str);
        }
        break;
    case DIVIDE:
        {
            m_szResult=QString("=DIVIDE(%1)").arg(str);
        }
        break;
    case MAX:
        {
            m_szResult=QString("=MAX(%1)").arg(str);
        }
        break;
    case MIN:
        {
            m_szResult=QString("=MIN(%1)").arg(str);
        }
        break;
    case SUM:
        {
            m_szResult=QString("=SUM(%1)").arg(str);
        }
        break;
    case AVG:
        {
            m_szResult=QString("=AVG(%1)").arg(str);
        }
        break;
    default:
        break;

    }
    QDialog::accept();
}
void QCalFormula::slot_on_cancel()
{
    QDialog::reject();
}
void QCalFormula::slot_on_listChange(int index)
{
    QString strContent=ui.listWidget->currentItem()->text();

    switch(index)
    {
    case PLUS:
            {
                DealPlus();
            }
            break;
    case MINUS:
            {
                DealMinus();
            }
            break;
    case MULTIPY:
            {
                DealMultipy();
            }
            break;
    case DIVIDE:
            {
                DealDivide();
            }
            break;
    case MAX:
            {
                DealMax();
            }
            break;
    case MIN:
            {
                DealMax();
            }
            break;
    case SUM:
            {
                DealMax();
            }
            break;
    case AVG:
            {
                DealAvg();
            }
            break;
    default:
        break;

    }
}

void QCalFormula::slot_on_dealData(const QString& mydata)
{  
   m_CurrentEdit->setText(mydata);
}


bool QCalFormula::eventFilter(QObject *watched,QEvent *event)
{
    if (watched==ui.lineEdit_1)         //首先判断控件(这里指 lineEdit1)  
    {  
        if (event->type()==QEvent::FocusIn)     //然后再判断控件的具体事件 (这里指获得焦点事件)  
        {  
            QPalette p=QPalette();  
            p.setColor(QPalette::Base,Qt::gray);  
            ui.lineEdit_1->setPalette(p);  
        }  
        else if (event->type()==QEvent::FocusOut)    // 这里指 lineEdit1 控件的失去焦点事件  
        {  
            QPalette p=QPalette();  
            p.setColor(QPalette::Base,Qt::red);  
            ui.lineEdit_1->setPalette(p);  
            m_CurrentEdit=ui.lineEdit_1;
        }          
    }  
    if (watched==ui.lineEdit_2)           //这里来处理 lineEdit2 , 和处理lineEdit1 是一样的  
    {  
        if (event->type()==QEvent::FocusIn)  
        {  
            QPalette p=QPalette();  
            p.setColor(QPalette::Base,Qt::green);  
            ui.lineEdit_2->setPalette(p);  
        }  
        else if (event->type()==QEvent::FocusOut)  
        {  
            QPalette p=QPalette();  
            p.setColor(QPalette::Base,Qt::white);  
            ui.lineEdit_2->setPalette(p);  
             m_CurrentEdit=ui.lineEdit_2;
        } 
       
    } 
    if (watched==ui.lineEdit_3)
    {
        if (event->type()==QEvent::FocusOut)  
        {  
            m_CurrentEdit=ui.lineEdit_3;
        }  
    }
    if (watched==ui.lineEdit_4)
    {
        if (event->type()==QEvent::FocusOut)  
        {  
            m_CurrentEdit=ui.lineEdit_4;
        }  
    }
    if (watched==ui.lineEdit_5)
    {
        if (event->type()==QEvent::FocusOut)  
        {  
            m_CurrentEdit=ui.lineEdit_5;
        }  
    }
    if (watched==ui.lineEdit_6)
    {
        if (event->type()==QEvent::FocusOut)  
        {  
            m_CurrentEdit=ui.lineEdit_6;
        }  
    }
    if (watched==ui.lineEdit_7)
    {
        if (event->type()==QEvent::FocusOut)  
        {  
            m_CurrentEdit=ui.lineEdit_7;
        }  
      
    }
    if (watched==ui.lineEdit_8)
    {
        if (event->type()==QEvent::FocusOut)  
        {  
             m_CurrentEdit=ui.lineEdit_8;
        }    
    }
    
   
    return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框  
}
void QCalFormula::InitData()
{
    m_Edit.append(ui.lineEdit_1);
    m_Edit.append(ui.lineEdit_2);
    m_Edit.append(ui.lineEdit_3);
    m_Edit.append(ui.lineEdit_4);
    m_Edit.append(ui.lineEdit_5);
    m_Edit.append(ui.lineEdit_6);
    m_Edit.append(ui.lineEdit_7);
    m_Edit.append(ui.lineEdit_8);

 //   ui.listWidget->setCurrentIndex(0);
    ui.lineEdit_1->installEventFilter(this);
    ui.lineEdit_2->installEventFilter(this);
    ui.lineEdit_3->installEventFilter(this);
    ui.lineEdit_4->installEventFilter(this);
    ui.lineEdit_5->installEventFilter(this);
    ui.lineEdit_6->installEventFilter(this);
    ui.lineEdit_7->installEventFilter(this);
    ui.lineEdit_8->installEventFilter(this);


    ui.listWidget->setCurrentRow(0);
}
void QCalFormula::DealPlus()
{
    foreach(QLineEdit *temp,m_Edit)
    {
        temp->setEnabled(true);
    }
}
void QCalFormula::DealMinus()
{
    foreach(QLineEdit *temp,m_Edit)
    {
        if (temp==ui.lineEdit_1||temp==ui.lineEdit_2)
        {
            temp->setEnabled(true);
        }
        else
        {
            temp->setEnabled(false);
        }
    }
}
void QCalFormula::DealMultipy()
{
    foreach(QLineEdit *temp,m_Edit)
    {
        if (temp==ui.lineEdit_1||temp==ui.lineEdit_2)
        {
            temp->setEnabled(true);
        }
        else
        {
            temp->setEnabled(false);
        }
    }
}
void QCalFormula::DealDivide()
{
    foreach(QLineEdit *temp,m_Edit)
    {
        if (temp==ui.lineEdit_1||temp==ui.lineEdit_2)
        {
            temp->setEnabled(true);
        }
        else
        {
            temp->setEnabled(false);
        }
    }
}
void QCalFormula::DealMax()
{
    foreach(QLineEdit *temp,m_Edit)
    {
        temp->setEnabled(true);      
    }
}
void QCalFormula::DealMin()
{
    foreach(QLineEdit *temp,m_Edit)
    {
        temp->setEnabled(true);      
    }
}
void QCalFormula::DealSum()
{
    foreach(QLineEdit *temp,m_Edit)
    {
        temp->setEnabled(true);      
    }
}
void QCalFormula::DealAvg()
{
    foreach(QLineEdit *temp,m_Edit)
    {
        temp->setEnabled(true);      
    }
}
QString&  QCalFormula::GetResult()
{
    return m_szResult;
}