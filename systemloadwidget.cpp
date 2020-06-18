#include "systemloadwidget.h"

#include <QFile>
#include <QTextCodec>
#include <QDebug>
#include <QProcess>
#include <QRadialGradient>
#include <QtMath>
#include <QPainterPath>
#include <QResizeEvent>

#define SECONDS_PER_ROUND 60
#define MAX_RADIUS 90
#define CENTER_SIZE 15.

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
SystemLoadWidget::SystemLoadWidget(QWidget *parent)
    : PaintedWidget(parent), prev_total(0), prev_idle(0)
{
    interval = 1000;
    setPaintInterval(interval);
    setMinimumHeight(200);

    center = QPointF(width() - (MAX_RADIUS + CENTER_SIZE), height() / 2);

    auto now = time(NULL);

    for (size_t i = 0; i < SECONDS_PER_ROUND; ++i)
    {
        cpu_load_points.push_front({0, static_cast<time_t>(now - (SECONDS_PER_ROUND - i))});
    }
}

void SystemLoadWidget::paintWidget(QPainter *painter)
{
    double cpu_load = calc_new_average();
    double ram = 0.;
    double swap = 0.;

    get_mem_details(&ram, &swap);

    auto now = time(nullptr);

    cpu_load_points.push_front({cpu_load, now});
    clean_measure_list(cpu_load_points);

    ram_load_points.push_front({ram / 100., now});
    clean_measure_list(ram_load_points);

    swap_load_points.push_front({swap / 100., now});
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
        painter->drawEllipse(center,
                             (MAX_RADIUS / 10) * i + CENTER_SIZE,
                             (MAX_RADIUS / 10) * i + CENTER_SIZE);
    }
    painter->restore();

    {
        painter->save();
        QFont f = painter->font();
        f.setPixelSize(18);

        painter->setFont(f);
        painter->drawText(center + QPoint(-5, 4), QString::number(static_cast<size_t>(cpu_load*100)));
        painter->restore();
    }

    graph_load_points(ram_load_points, painter, QColor(0, 0, 255, 130));
    graph_load_points(swap_load_points, painter, QColor(255, 0, 0, 130));
    graph_load_points(cpu_load_points, painter, Qt::black);

}

void SystemLoadWidget::resizeEvent(QResizeEvent *e)
{
    center = QPointF(width() - (MAX_RADIUS + CENTER_SIZE), height() / 2);
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

QPointF SystemLoadWidget::polar_to_cartisan(double r, double theta)
{
    QPointF ret;
    ret.setX(r * qCos(qDegreesToRadians(theta)));
    ret.setY(r * qSin(qDegreesToRadians(theta)));
    return ret;
}

void SystemLoadWidget::graph_load_points(MeasureList &list, QPainter *painter, const QColor &color)
{
    painter->save();

    {
        QPen p = painter->pen();
        p.setColor(color);
        p.setWidth(1);
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
            path.moveTo(point + center);
            first = false;
        }
        else
        {
            path.lineTo(point + center);
        }
    }
    //path.closeSubpath();
    painter->drawPath(path);
    painter->restore();
}

void SystemLoadWidget::clean_measure_list(SystemLoadWidget::MeasureList &list)
{
    while(list.size() > (SECONDS_PER_ROUND + 1))
    {
        list.pop_back();
    }
}
