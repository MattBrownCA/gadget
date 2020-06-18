#include <QApplication>
#include <QDebug>
#include <QScreen>
#include <QPalette>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QTimer>
#include <QPainter>
#include <QVBoxLayout>
#include <QSpacerItem>

#include <chrono>
#include <cstring>
#include <ctime>

#include "primarywidget.h"
#include "datetimewidget.h"
#include "batterywidget.h"
#include "systemloadwidget.h"

PrimaryWidget::PrimaryWidget(QWidget *parent) :
    Widget(parent)
{
    //setWindowOpacity(0);
    QRect size = QApplication::primaryScreen()->geometry();

    int width = 400;
    int height = 400;

    setGeometry(size.width() - (width + 40), size.y() + 40, width, height);

    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    layout->addWidget(new DateTimeWidget());
    layout->addWidget(new BatteryWidget());
    layout->addWidget(new SystemLoadWidget());

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    //layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
}
