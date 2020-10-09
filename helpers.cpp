#include "helpers.h"
#include <QtMath>

QPointF polar_to_cartisan(double r, double theta)
{
    QPointF ret;
    ret.setX(r * qCos(qDegreesToRadians(theta)));
    ret.setY(r * qSin(qDegreesToRadians(theta)));
    return ret;
}
