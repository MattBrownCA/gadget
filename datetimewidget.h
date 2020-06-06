#ifndef DATETIMEWIDGET_H
#define DATETIMEWIDGET_H

#include "paintedwidget.h"

class DateTimeWidget : public PaintedWidget
{
public:
    DateTimeWidget(QWidget *parent = nullptr);

protected:
    virtual void paintWidget(QPainter *painter) override;
};

#endif // DATETIMEWIDGET_H
