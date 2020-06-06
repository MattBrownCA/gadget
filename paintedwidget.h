#ifndef PAINTEDWIDGET_H
#define PAINTEDWIDGET_H

#include "widget.h"

class PaintedWidget : public Widget
{
public:
    PaintedWidget(QWidget *parent = nullptr);

protected:
    void setPaintInterval(uint32_t ms);
    virtual void paintEvent(QPaintEvent *e) override;

    virtual void paintWidget(QPainter *p) = 0;
};

#endif // PAINTEDWIDGET_H
