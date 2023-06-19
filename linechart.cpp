#include "linechart.h"

LineChart::LineChart(ChartData chartData) {
    chart = new QChart();
    chart->setTitle(chartData.chartTitle);
    auto series = new QLineSeries (chart);
    // Записываем все данные в серии
    for (auto point: chartData.points) {
        float x, y;

        auto date = QDateTime {{QDate::fromString(point.date, "yyyy.MM")}};
        x = (float) date.toMSecsSinceEpoch();
        y = point.value;
        series->append({x, y});
    }
    // Добавляем серии на график
    chart->addSeries(series);
    chart->legend()->hide();
    // Измерение даты
    auto dateTimeAxis = new QDateTimeAxis (chart);
    dateTimeAxis->setTitleText(chartData.dateAxisTitle);
    // Измерение value
    auto valueAxis = new QValueAxis (chart);
    valueAxis->setTitleText(chartData.valueAxisTitle);
    // Для корректной работы
    dateTimeAxis->setFormat("yyyy.MM");
    dateTimeAxis->setTickCount(7);
    // Добавим измерения
    chart->addAxis(dateTimeAxis, Qt::AlignBottom);
    chart->addAxis(valueAxis, Qt::AlignLeft);
    // Закрепляем измерения за сериями

    series->attachAxis(dateTimeAxis);
    series->attachAxis(valueAxis);
}
