#include <QKeyEvent>
#include "lcdcontrolwgt.h"

CLcdControlWgt::CLcdControlWgt(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_RetClick()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_AddClick()));
    connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(Slot_CalClick()));

    connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(Slot_DelClick()));
    connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(Slot_UpClick()));
    connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(Slot_LeftClick()));

    connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(Slot_EtrClick()));
    connect(ui.pushButton_8, SIGNAL(clicked()), this, SLOT(Slot_RightClick()));

    connect(ui.pushButton_9, SIGNAL(clicked()), this, SLOT(Slot_DownClick()));
    setFocusPolicy(Qt::StrongFocus);

    //grabKeyboard();
}

CLcdControlWgt::~CLcdControlWgt()
{

}

void CLcdControlWgt::Slot_RetClick()
{
    emit Signal_ClickOperator(8);
}

void CLcdControlWgt::Slot_AddClick()
{
    emit Signal_ClickOperator(4);
}

void CLcdControlWgt::Slot_UpClick()
{
    emit Signal_ClickOperator(0);
}

void CLcdControlWgt::Slot_LeftClick()
{
    emit Signal_ClickOperator(2);
}

void CLcdControlWgt::Slot_EtrClick()
{
    emit Signal_ClickOperator(6);
}

void CLcdControlWgt::Slot_RightClick()
{
    emit Signal_ClickOperator(3);
}

void CLcdControlWgt::Slot_DownClick()
{
    emit Signal_ClickOperator(1);
}

void CLcdControlWgt::Slot_DelClick()
{
    emit Signal_ClickOperator(5);
}

void CLcdControlWgt::Slot_CalClick()
{
    emit Signal_ClickOperator(7);
}

void CLcdControlWgt::keyPressEvent(QKeyEvent *event)
{
    if (!this->isVisible())
    {
        keyPressEvent(event);
        return;
    }

    switch (event->key())
    {
    case Qt::Key_Left:
    {
        Slot_LeftClick();
        break;
    }
    case Qt::Key_Right:
    {
        Slot_RightClick();
        break;
    }
    case Qt::Key_Up:
    {
        Slot_UpClick();
        break;
    }
    case Qt::Key_Down:
    {
        Slot_DownClick();
        break;
    }
    case Qt::Key_Enter:
    {
        Slot_EtrClick();
        break;
    }
    case Qt::Key_Return:
    {
        Slot_EtrClick();
        break;
    }
    case Qt::Key_Escape:
    {
        Slot_CalClick();
        break;
    }
    case Qt::Key_Plus:
    {
        Slot_AddClick();
        break;
    }
    case Qt::Key_Minus:
    {
        Slot_DelClick();
        break;
    }
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}
