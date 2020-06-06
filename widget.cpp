#include "widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{
    setWindowFlag(Qt::WindowStaysOnBottomHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowFlag(Qt::Tool);
}


