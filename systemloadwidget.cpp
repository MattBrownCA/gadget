#include "systemloadwidget.h"

#include <QFile>
#include <QTextCodec>
#include <QDebug>
#include <QProcess>
#include <QRadialGradient>
#include <QtMath>
#include <QPainterPath>
#include <QResizeEvent>

#include "helpers.h"

/*
 * PrevIdle = previdle + previowait
 * Idle = idle + iowait
 *
 * PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
 * NonIdle = user + nice + system + irq + softirq + steal
 *
 * PrevTotal = PrevIdle + PrevNonIdle
 * Total = Idle + NonIdle
 *
 * differentiate: actual value minus the previous one
 * totald = Total - PrevTotal
 * idled = Idle - PrevIdle
 *
 * CPU_Percentage = (totald - idled)/totald
 */
SystemLoadWidget::SystemLoadWidget(Widget *parent)
    : PaintedWidget("SystemLoad", parent), prev_total(0), prev_idle(0)
{
    interval = 1000;
    setPaintInterval(interval);
    setMinimumHeight(200);

    _center = QPointF(width() - (MAX_RADIUS + CENTER_SIZE), height() / 2);

    auto now = time(NULL);

    for (size_t i = 0; i < SECONDS_PER_ROUND; ++i)
    {
        cpu_load_points.push_front({0, static_cast<time_t>(now - (SECONDS_PER_ROUND - i))});
    }
}

QPointF SystemLoadWidget::center()
{
    return _center;
}

void SystemLoadWidget::paintWidget(QPainter *painter)
{
    struct timespec now;
    double cpu_load = calc_new_average();
    double ram = 0.;
    double swap = 0.;

    clock_gettime(CLOCK_REALTIME, &now);
    draw_clock(painter, &now);
    get_mem_details(&ram, &swap);

    cpu_load_points.push_front({cpu_load, now.tv_sec});
    clean_measure_list(cpu_load_points);

    ram_load_points.push_front({ram / 100., now.tv_sec});
    clean_measure_list(ram_load_points);

    swap_load_points.push_front({swap / 100., now.tv_sec});
    clean_measure_list(swap_load_points);

    // paint the graph circle
    painter->save();

    for(size_t i = 0; i < 10; ++i)
    {
        QPen p = painter->pen();
        p.setWidth(0);
        QColor c = p.color();
        c.setAlpha(255 - ((255. / 10.) * i));
        p.setColor(c);
        painter->setPen(p);
        painter->drawEllipse(_center,
                             (MAX_RADIUS / 10) * i + CENTER_SIZE,
                             (MAX_RADIUS / 10) * i + CENTER_SIZE);
    }
    painter->restore();

    {
        painter->save();
        QFont f = painter->font();
        f.setPixelSize(18);

        QString text = QString::number(static_cast<size_t>(cpu_load*100));

        QFontMetrics m(f);
        int width = m.horizontalAdvance(text);

        painter->setFont(f);
        painter->drawText(_center + QPoint(-(width / 2), m.height() / 4), text);
        painter->restore();
    }

    graph_load_points(ram_load_points, painter, QColor(51, 105, 30, 255));
    graph_load_points(swap_load_points, painter, QColor(255, 0, 0, 130));
    graph_load_points(cpu_load_points, painter, QColor(0, 0, 0, 100));
}

void SystemLoadWidget::resizeEvent(QResizeEvent *e)
{
    _center = QPointF(width() - (MAX_RADIUS + CENTER_SIZE), height() / 2);
    e->accept();
}

double SystemLoadWidget::calc_new_average()
{
    QFile stat_file("/proc/stat");
    if(stat_file.open(QIODevice::ReadOnly) == false)
    {
        qDebug() << "Unable to open stat file";
        return 0.;
    }

    QString line = QString(stat_file.readLine()).trimmed();
    stat_file.close();

    auto cols = line.split(" ");
    unsigned long user = cols[1].toULong();
    unsigned long nice = cols[2].toULong();
    unsigned long sys = cols[3].toULong();
    unsigned long idle = cols[4].toULong();
    unsigned long iowait = cols[5].toULong();
    unsigned long irq = cols[6].toULong();
    unsigned long softirq = cols[7].toULong();

    unsigned long t_idle = idle + iowait;
    unsigned long t_non_idle = user + nice + sys+ irq + softirq;

    unsigned long total = t_idle + t_non_idle;

    unsigned long totald = total - prev_total;
    unsigned long idled = t_idle - prev_idle;

    double cpu_load = (static_cast<double>(totald) - static_cast<double>(idled))/static_cast<double>(totald);

    prev_total = total;
    prev_idle = t_idle;

    return cpu_load;
}

void SystemLoadWidget::get_mem_details(double *ram, double *swap)
{
    QProcess p;
    p.setProgram("free");
    p.setArguments({"free"});
    p.start();
    p.waitForFinished();

    p.readLine();
    auto ram_cols = QString(p.readLine()).simplified().split(" ");
    auto swap_cols = QString(p.readLine()).simplified().split(" ");

    *ram = (ram_cols[2].toDouble() / ram_cols[1].toDouble()) * 100.;
    *swap = (swap_cols[2].toDouble() / swap_cols[1].toDouble()) * 100.;
}

void SystemLoadWidget::graph_load_points(MeasureList &list, QPainter *painter, const QColor &color)
{
    painter->save();

    {
        QPen p = painter->pen();
        p.setColor(color);
        p.setWidth(2);
        painter->setPen(p);
    }

    QPainterPath path;
    bool first = true;

    for(auto &p : list)
    {
        double angle = (p.timepoint % SECONDS_PER_ROUND) *
                (360 / SECONDS_PER_ROUND);
        angle -= 90; // adjust for 90 degree offset from clock;
        double radius = MAX_RADIUS * p.load + CENTER_SIZE;

        QPointF point = polar_to_cartisan(radius, angle);

        if(first)
        {
            path.moveTo(point + _center);
            first = false;
        }
        else
        {
            path.lineTo(point + _center);
        }
    }
    //path.closeSubpath();
    painter->drawPath(path);
    painter->restore();
}

void SystemLoadWidget::draw_clock(QPainter *painter, struct timespec *now)
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

void SystemLoadWidget::clean_measure_list(SystemLoadWidget::MeasureList &list)
{
    while(static_cast<size_t>(list.size()) > (SECONDS_PER_ROUND + 1))
    {
        list.pop_back();
    }
}
