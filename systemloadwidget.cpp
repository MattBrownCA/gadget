#include "systemloadwidget.h"

#include <QFile>
#include <QTextCodec>
#include <QDebug>
#include <QProcess>

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
    setPaintInterval(5000);
    setMinimumWidth(85);
}

void SystemLoadWidget::paintWidget(QPainter *painter)
{
    double cpu_load = calc_new_average() * 100.;
    double ram = 0.;
    double swap = 0.;

    get_mem_details(&ram, &swap);

    QString formatted;
    formatted = QString::asprintf("CPU: %0.2lf%% ", cpu_load);

    painter->drawText(QRect(0, 0, width(), 30), Qt::AlignTop | Qt::AlignRight, formatted);

    formatted = QString::asprintf("RAM: %0.2lf%% ", ram);
    painter->drawText(QRect(0, 35, width(), 30), Qt::AlignTop | Qt::AlignRight, formatted);

    formatted = QString::asprintf("Swap: %0.2lf%%  ", swap);
    painter->save();
    QFont f = painter->font();
    f.setPixelSize(20);
    painter->setFont(f);
    painter->drawText(QRect(0, 57, width(), 25), Qt::AlignTop | Qt::AlignRight, formatted);
    painter->restore();
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
