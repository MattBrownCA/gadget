#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "paintedwidget.h"

class BatteryWidget : public PaintedWidget
{
public:
    BatteryWidget(Widget *parent = nullptr);

protected:
    virtual void paintWidget(QPainter *painter) override;
};

#endif // BATTERYWIDGET_H
