#ifndef LINECHART_H
#define LINECHART_H
#include <QtCharts/QChartGlobal>
#include <ichart.h>

class LineChart: public IChart
{
public:
    LineChart(ChartData chartData);
};

#endif // LINECHART_H
