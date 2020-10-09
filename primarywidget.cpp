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

PrimaryWidget::PrimaryWidget(Widget *parent) :
    Widget("Primary", parent)
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
}
