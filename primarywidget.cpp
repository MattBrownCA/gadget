#include <QApplication>
#include <QScreen>
#include <QPalette>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QTimer>
#include <QPainter>

#include <chrono>
#include <cstring>
#include <ctime>

#include "primarywidget.h"

PrimaryWidget::PrimaryWidget(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{
    //setWindowOpacity(0);
    QRect size = QApplication::primaryScreen()->geometry();
    setWindowFlag(Qt::WindowStaysOnBottomHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setWindowFlag(Qt::Tool);

    startTimer(std::chrono::seconds(1));

    int width = 400;
    int height = 1000;

    setGeometry(size.width() - (width + 40), size.y() + 40, width, height);

    QTimer * draw_timer = new QTimer(this);
    connect(draw_timer, SIGNAL(timeout()), this, SLOT(update()));
    draw_timer->start(1000);
}

void PrimaryWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(QColor(44,44,44), 30, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    drawTime(&painter);

    event->accept();
}

void PrimaryWidget::drawTime(QPainter *painter)
{
    painter->save();
    time_t current_time = time(nullptr);
    tm *local_time = localtime(&current_time);
    char formatted_time[512];

    strftime(formatted_time, 512, "%T ", local_time);

    QFont font = painter->font();
    font.setFamily("Z003");

    font.setPixelSize(50);

    painter->setFont(font);
    painter->drawText(QRect(0, 30, width(), 50), Qt::AlignTop | Qt::AlignRight, formatted_time);

    font.setPixelSize(30);
    painter->setFont(font);

    strftime(formatted_time, 512, "%A %d %b %Y ", local_time);
    painter->drawText(QRect(0, 80, width(), 30), Qt::AlignTop | Qt::AlignRight, formatted_time);

    painter->restore();
}
