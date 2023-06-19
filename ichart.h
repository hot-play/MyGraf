#ifndef ICHART_H
#define ICHART_H
#include<chartdata.h>
#include<QChart>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>

#include <QtCore/QTime>
#include <QDate>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE
class IChart
{
public:
    IChart() = default;
    ~IChart();
    QChart * getChart();
protected:
    QChart * chart;
};

#endif // ICHART_H
