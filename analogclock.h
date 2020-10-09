#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H

#include "paintedwidget.h"
#include <QPointF>

class AnalogClock :
        public PaintedWidget
{
public:
    AnalogClock(Widget *parent = nullptr);

protected:
    virtual void paintWidget(QPainter *p);

private:
    QPointF _center;

    void draw_clock(QPainter *painter, struct timespec *now);
};

#endif // ANALOGCLOCK_H
