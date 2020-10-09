#include "analogclock.h"

#include <QDebug>

#include "systemloadwidget.h"
#include "helpers.h"

AnalogClock::AnalogClock(Widget *parent) :
    PaintedWidget("AnalogClock", parent)
{
    SystemLoadWidget *sys_load = dynamic_cast<SystemLoadWidget *>(
                get_widget_by_name("SystemLoad"));
    if (nullptr == sys_load) {
        throw std::runtime_error("System load widget is null");
    }

    setPaintInterval(1000);
    setMinimumHeight(sys_load->height());
    setGeometry(sys_load->geometry());

    _center = sys_load->center();
}

void AnalogClock::paintWidget(QPainter *p)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    qDebug() << geometry();

    draw_clock(p, &now);
}


void AnalogClock::draw_clock(QPainter *painter, struct timespec *now)
{
    painter->save();

    struct tm *tm = localtime(&now->tv_sec);
    qreal angle;
    qreal last = 0;
    QBrush brush;
    QPen pen;
    QPointF inner;
    QPointF outer;
    std::vector<std::tuple<int, int, qreal>> values = {
        {tm->tm_sec, 60, SystemLoadWidget::MAX_RADIUS * .95},
        {tm->tm_min, 60, SystemLoadWidget::MAX_RADIUS * .75},
        {tm->tm_hour, 12, SystemLoadWidget::MAX_RADIUS * .55},
    };

    brush = painter->brush();
    brush.setColor(Qt::black);
    painter->setBrush(brush);

    pen = painter->pen();
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter->setPen(pen);

    for (const auto &item : values) {
        // second hand
        angle = (qreal)(std::get<0>(item) % std::get<1>(item)) * (360. / std::get<1>(item)) +
                6 * (last / 360.);
        angle -= 90; //adjust for 90 degree offset from clock

        inner = polar_to_cartisan(SystemLoadWidget::CENTER_SIZE, angle) + _center;
        outer = polar_to_cartisan(std::get<2>(item), angle) + _center;

        last = angle + 90;

        painter->drawLine(inner, outer);
    }

    painter->restore();
}
