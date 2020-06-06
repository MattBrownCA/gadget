#include "paintedwidget.h"

#include <QPaintEvent>
#include <QTimer>
#include <QDebug>

PaintedWidget::PaintedWidget(QWidget *parent) :
    Widget(parent)
{

}

void PaintedWidget::setPaintInterval(uint32_t ms)
{
    QTimer *paint_timer = new QTimer(this);
    connect(paint_timer, SIGNAL(timeout()), this, SLOT(update()));
    paint_timer->start(ms);
}

void PaintedWidget::paintEvent(QPaintEvent *e)
{
    e->accept();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font = painter.font();
    font.setFamily("Z003");
    font.setPixelSize(30);
    painter.setFont(font);
    QPen pen(QColor(44,44,44), 30, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    paintWidget(&painter);
}
