#ifndef SYSTEMLOADWIDGET_H
#define SYSTEMLOADWIDGET_H
#include <QList>

#include "paintedwidget.h"

class SystemLoadWidget : public PaintedWidget
{
public:
    static constexpr size_t SECONDS_PER_ROUND = 60;
    static constexpr size_t MAX_RADIUS = 90;
    static constexpr size_t CENTER_SIZE = 15.;

    SystemLoadWidget(Widget *parent = nullptr);

    QPointF center();

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

    void graph_load_points(MeasureList &list, QPainter *painter, const QColor &color);
    void draw_clock(QPainter *painter, struct timespec *now);
    void clean_measure_list(MeasureList &list);

    MeasureList cpu_load_points;
    MeasureList ram_load_points;
    MeasureList swap_load_points;

    QPointF _center;

    uint64_t prev_total;
    uint64_t prev_idle;

    uint64_t interval;
};

#endif // SYSTEMLOADWIDGET_H
