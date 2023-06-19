#include "piechart.h"

PieChart::PieChart(ChartData chartData) {
    chart = new QChart();
    chart->setTitle(chartData.chartTitle);
    auto series = new QPieSeries (chart);
    // Записываем все данные в серии
    for (auto point: chartData.points) {
        float value {point.value};
        series->append(point.date, value);
    }
    // Добавляем серии
    chart->addSeries(series);
    chart->legend()->show();
}
