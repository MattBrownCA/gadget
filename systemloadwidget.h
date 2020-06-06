#ifndef SYSTEMLOADWIDGET_H
#define SYSTEMLOADWIDGET_H

#include "paintedwidget.h"

class SystemLoadWidget : public PaintedWidget
{
public:
    SystemLoadWidget(QWidget *parent = nullptr);

protected:
    virtual void paintWidget(QPainter *painter) override;

private:
    double calc_new_average();
    void get_mem_details(double *ram, double *swap);

    uint64_t prev_total;
    uint64_t prev_idle;
};

#endif // SYSTEMLOADWIDGET_H
