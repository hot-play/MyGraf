#ifndef CHARTDATA_H
#define CHARTDATA_H
#include <QString>
#include <QVector>

struct ChartDataPoint {
    QString date;
    float value;
};

struct ChartData {
    QString dateAxisTitle = "date";
    QString valueAxisTitle = "value";
    QString chartTitle;
    QVector<ChartDataPoint> points;
};

#endif // CHARTDATA_H
