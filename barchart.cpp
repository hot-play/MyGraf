#include "barchart.h"

BarChart::BarChart(ChartData chartData)
{
    chart = new QChart();
    chart->setTitle(chartData.chartTitle);
    auto series = new QBarSeries (chart);
    // Записываем все данные в серии
    for (auto point: chartData.points) {
        auto value {point.value};
        auto barSet = new QBarSet (point.date, chart);
        barSet->append(value);
        series->append(barSet);
    }
    // Формируем измерение value
    auto valueAxis = new QValueAxis(chart);
    valueAxis->setTitleText(chartData.valueAxisTitle);
    chart->legend()->show();
    // Добавляем серии
    chart->addSeries(series);
    chart->addAxis(valueAxis, Qt::AlignLeft);
    // Закрепляем измерение за серией
    series->attachAxis(valueAxis);
}
