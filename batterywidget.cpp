#include "batterywidget.h"

#include <QFile>
#include <QString>
#include <QDebug>

BatteryWidget::BatteryWidget(QWidget *parent)
    : PaintedWidget(parent)
{
    setPaintInterval(5000);
    setMinimumHeight(20);
}

void BatteryWidget::paintWidget(QPainter *painter)
{
    QString batt_cap;
    QString batt_stat;

    QFile batt_file("/sys/class/power_supply/BAT1/capacity");
    QFile batt_stat_file("/sys/class/power_supply/BAT1/status");

    if((batt_file.open(QIODevice::ReadOnly) == false) ||
            (batt_stat_file.open(QIODevice::ReadOnly) == false))
    {
        qDebug() << "Unable to open file" << batt_file.errorString();
        return;
    }

    batt_cap = batt_file.readAll().trimmed();
    batt_file.close();

    batt_stat = batt_stat_file.readAll().trimmed() + " ";
    batt_stat_file.close();

    QString formatted = QString("Battery: ") + batt_cap + "% ";

    painter->drawText(QRect(0, 0, width(), 30), Qt::AlignTop | Qt::AlignRight, formatted);
    painter->save();
    auto font = painter->font();
    font.setPixelSize(20);
    painter->setFont(font);
    painter->drawText(QRect(0, 27, width(), 20), Qt::AlignTop | Qt::AlignRight, batt_stat);
    painter->restore();
}
