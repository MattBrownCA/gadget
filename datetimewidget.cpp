#include "datetimewidget.h"

#include <QTimer>
#include <QPainter>

DateTimeWidget::DateTimeWidget(Widget *parent)
    : PaintedWidget("DateTime", parent)
{
    setPaintInterval(1000);

    setMinimumHeight(110);
}

void DateTimeWidget::paintWidget(QPainter *painter)
{
    painter->save();
    time_t current_time = time(nullptr);
    tm *local_time = localtime(&current_time);
    char formatted_time[512];

    strftime(formatted_time, 512, "%T ", local_time);

    QFont font = painter->font();

    font.setPixelSize(50);

    painter->setFont(font);
    painter->drawText(QRect(0, 30, width(), 50), Qt::AlignTop | Qt::AlignRight, formatted_time);
    painter->restore();

    strftime(formatted_time, 512, "%A %d %b %Y ", local_time);
    painter->drawText(QRect(0, 80, width(), 30), Qt::AlignTop | Qt::AlignRight, formatted_time);
}


