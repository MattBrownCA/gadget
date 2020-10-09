#ifndef DATETIMEWIDGET_H
#define DATETIMEWIDGET_H

#include "paintedwidget.h"

class DateTimeWidget : public PaintedWidget
{
public:
    DateTimeWidget(Widget *parent = nullptr);

protected:
    virtual void paintWidget(QPainter *painter) override;
};

#endif // DATETIMEWIDGET_H
