#ifndef PRIMARYWIDGET_H
#define PRIMARYWIDGET_H

#include <QWidget>
#include <chrono>

#include "widget.h"

class PrimaryWidget : public Widget
{
public:
    explicit PrimaryWidget(QWidget *parent = nullptr);
};

#endif // PRIMARYWIDGET_H
