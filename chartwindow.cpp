#include "chartwindow.h"
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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>

#include <QtCore/QTime>
#include <QDate>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>

#include <QPrinter>
#include <QPainter>
#include <QPdfWriter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>

ChartWindow::ChartWindow(QWidget *parent) :
    QWidget(parent)
{
    // Создадим лайаут
    baseLayout = new QGridLayout(this);
    QHBoxLayout * settingsLayout = new QHBoxLayout(this);
    // Добавим список с типами графиков
    settingsLayout->addWidget(new QLabel("Тип графика:", this));
    typeComboBox = new QComboBox(this);
    typeComboBox->addItem("Линейный график", 1);
    typeComboBox->addItem("Круговой график", 2);
    typeComboBox->addItem("Столбчатый график", 3);
    settingsLayout->addWidget(typeComboBox);
    // Добавим список с темами графиков
    settingsLayout->addWidget(new QLabel("Цвет печати:", this));
    themeComboBox = new QComboBox(this);
    themeComboBox->addItem("Цветной", 1);
    themeComboBox->addItem("Черно-белый", 2);
    settingsLayout->addWidget(themeComboBox);
    // Добавим кнопку сохранения файла
    saveButton = new QPushButton("Сохранить", this);
    settingsLayout->addWidget(saveButton);
    settingsLayout->addStretch();

    // Подключим сигналы
    connect(typeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ChartWindow::switchType);
    connect(saveButton,
            &QPushButton::released,
            this, &ChartWindow::printChartToPdf);

    // Отрисуем все копмоненты
    baseLayout->addLayout(settingsLayout, 0, 0, 1, 3);
    chart = new QChartView(this);
    baseLayout->addWidget(chart, 1, 0);
    setLayout(baseLayout);
}

QChart *ChartWindow::createLineChart() const
{
    QChart * newChart = new QChart();
    auto series = new QLineSeries (newChart);

    for (auto point: chartData.points) {
        float x, y;

        auto date = QDateTime {{QDate::fromString(point.date, "yyyy.MM")}};
        x = (float) date.toMSecsSinceEpoch();
        y = point.value;
        series->append({x, y});
    }

    newChart->addSeries(series);
    newChart->legend()->hide();

    auto dateTimeAxis = new QDateTimeAxis (newChart);
    dateTimeAxis->setTitleText(chartData.dateAxisTitle);
    auto valueAxis = new QValueAxis (newChart);
    valueAxis->setTitleText(chartData.valueAxisTitle);

    dateTimeAxis->setFormat("yyyy.MM");
    dateTimeAxis->setTickCount(7);

    newChart->addAxis(dateTimeAxis, Qt::AlignBottom);
    newChart->addAxis(valueAxis, Qt::AlignLeft);

    series->attachAxis(dateTimeAxis);
    series->attachAxis(valueAxis);

    return newChart;
}

QChart *ChartWindow::createPieChart() const
{
    QChart * newChart = new QChart();
    auto series = new QPieSeries (newChart);

    for (auto point: chartData.points) {
        float value {point.value};
        series->append(point.date, value);
    }

    newChart->addSeries(series);
    newChart->legend()->show();

    return newChart;
}

QChart *ChartWindow::createBarChart() const
{
    QChart *newChart = new QChart();
    auto series = new QBarSeries (newChart);
    for (auto point: chartData.points) {
        auto value {point.value};
        auto barSet = new QBarSet (point.date, newChart);
        barSet->append(value);
        series->append(barSet);
    }
    auto valueAxis = new QValueAxis(newChart);
    valueAxis->setTitleText(chartData.valueAxisTitle);
    newChart->legend()->show();
    newChart->addSeries(series);
    newChart->addAxis(valueAxis, Qt::AlignLeft);
    series->attachAxis(valueAxis);
    return newChart;
}

void ChartWindow::switchType()
{
    QChart::ChartTheme type = static_cast<QChart::ChartTheme>(
                typeComboBox->itemData(typeComboBox->currentIndex()).toInt());
    QChartView * newChart;
    if (type == 1) {
        newChart = new QChartView(createLineChart());
    } else if (type == 2) {
        newChart = new QChartView(createPieChart());
    } else if (type == 3) {
        newChart = new QChartView(createBarChart());
    }
    delete chart;
    chart = newChart;
    baseLayout->addWidget(newChart, 1, 0);
    setLayout(baseLayout);
}

void ChartWindow::switchData(ChartData data)
{
    chartData = data;
    switchType();
}

void ChartWindow::printChartToPdf() const
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
                themeComboBox->itemData(themeComboBox->currentIndex()).toInt());
    QPrinter printer;
    if (theme == 2) {
        printer.setColorMode(QPrinter::GrayScale);// установка ч/б цвета
    }
    //С помощью QPainter выделение области для формирования pdf
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("result.pdf");
    QPainter painter(&printer);
    chart->render(&painter);
    painter.end();
}


