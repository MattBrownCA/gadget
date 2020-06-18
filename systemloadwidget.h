#ifndef SYSTEMLOADWIDGET_H
#define SYSTEMLOADWIDGET_H
#include <QList>

#include "paintedwidget.h"

class SystemLoadWidget : public PaintedWidget
{
public:
    SystemLoadWidget(QWidget *parent = nullptr);

protected:
    virtual void paintWidget(QPainter *painter) override;

    virtual void resizeEvent(QResizeEvent *e) override;

private:
    struct MeasurePoint
    {
        double load;
        time_t timepoint;
    };

    using MeasureList = QList<MeasurePoint>;

    double calc_new_average();
    void get_mem_details(double *ram, double *swap);
    QPointF polar_to_cartisan(double r, double theta);

    void graph_load_points(MeasureList &list, QPainter *painter, const QColor &color);
    void clean_measure_list(MeasureList &list);

    MeasureList cpu_load_points;
    MeasureList ram_load_points;
    MeasureList swap_load_points;

    QPointF center;

    uint64_t prev_total;
    uint64_t prev_idle;

    uint64_t interval;
};

#endif // SYSTEMLOADWIDGET_H
