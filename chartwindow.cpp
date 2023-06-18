#include "chartwindow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
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
#include <QtCharts/QBarCategoryAxis>
#include <QDate>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QPainter>
#include <QPdfWriter>

ChartWindow::ChartWindow(QWidget *parent) :
    QWidget(parent)
{
    // Создадим лайаут
    baseLayout = new QGridLayout(this);
    QHBoxLayout *settingsLayout = new QHBoxLayout(this);
    // Добавим список с темами графиков
    settingsLayout->addWidget(new QLabel("Тема:", this));
    themeComboBox = new QComboBox();
    themeComboBox->addItem("Color", QChart::ChartThemeLight);
    themeComboBox->addItem("White-Black", QChart::ChartThemeDark);
    settingsLayout->addWidget(themeComboBox);
    // Добавим список с типами графиков
    settingsLayout->addWidget(new QLabel("Тип графика:", this));
    typeComboBox = new QComboBox();
    typeComboBox->addItem("Линейный график", 1);
    typeComboBox->addItem("Круговой график", 2);
    typeComboBox->addItem("Столбчатый график", 3);
    settingsLayout->addWidget(typeComboBox);
    // Добавим кнопку сохранения файла
    saveButton = new QPushButton("Сохранить");
    settingsLayout->addWidget(saveButton);
    settingsLayout->addStretch();

    // Подключим сигналы
    connect(themeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ChartWindow::updateUI);
    connect(typeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ChartWindow::switchType);
    connect(saveButton,
            &QPushButton::released,
            this, &ChartWindow::printChartToPdf);

    // Отрисуем все копмоненты
    baseLayout->addLayout(settingsLayout, 0, 0, 1, 3);
    charts = new QChartView(this);
    baseLayout->addWidget(charts, 1, 0);
    setLayout(baseLayout);
    updateUI();
}

QChart *ChartWindow::createLineChart() const
{
    QChart * chart = new QChart();
    auto series = new QLineSeries (chart);

    for (auto point: chartData.points) {
        float x, y;

        auto date = QDateTime {{QDate::fromString(point.date, "yyyy.MM")}};
        x = (float) date.toMSecsSinceEpoch();
        y = point.value;
        series->append({x, y});
    }

    chart->addSeries(series);
    chart->legend()->hide();

    auto dateTimeAxis = new QDateTimeAxis (chart);
    dateTimeAxis->setTitleText(chartData.dateAxisTitle);
    auto valueAxis = new QValueAxis (chart);
    valueAxis->setTitleText(chartData.valueAxisTitle);

    dateTimeAxis->setFormat("yyyy.MM");
    dateTimeAxis->setTickCount(7);

    chart->addAxis(dateTimeAxis, Qt::AlignBottom);
    chart->addAxis(valueAxis, Qt::AlignLeft);

    series->attachAxis(dateTimeAxis);
    series->attachAxis(valueAxis);

    return chart;
}

QChart *ChartWindow::createPieChart() const
{
    QChart * chart = new QChart();
    auto series = new QPieSeries (chart);

    for (auto point: chartData.points) {
        float value {point.value};
        series->append(point.date, value);
    }

    chart->addSeries(series);
    chart->legend()->show();

    return chart;
}

QChart *ChartWindow::createBarChart() const
{
    QChart *chart = new QChart();
    auto series = new QBarSeries (chart);
    for (auto point: chartData.points) {
        auto value {point.value};
        auto barSet = new QBarSet (point.date, chart);
        barSet->append(value);
        series->append(barSet);
    }
    auto valueAxis = new QValueAxis(chart);
    valueAxis->setTitleText(chartData.valueAxisTitle);
    chart->legend()->show();
    chart->addSeries(series);
    chart->addAxis(valueAxis, Qt::AlignLeft);
    series->attachAxis(valueAxis);
    return chart;
}

void ChartWindow::switchType()
{
    QChart::ChartTheme type = static_cast<QChart::ChartTheme>(
                typeComboBox->itemData(typeComboBox->currentIndex()).toInt());
    QChartView * chart;
    if (type == 1) {
        chart = new QChartView(createLineChart());
    } else if (type == 2) {
        chart = new QChartView(createPieChart());
    } else if (type == 3) {
        chart = new QChartView(createBarChart());
    }
    delete charts;
    charts = chart;
    baseLayout->addWidget(chart, 1, 0);
    setLayout(baseLayout);
    updateUI();
}

void ChartWindow::switchData(ChartData data)
{
    chartData = data;
    switchType();
    updateUI();
}

void ChartWindow::printChartToPdf() const
{
    QPdfWriter writer("out.pdf");
    writer.setCreator("Someone");
    writer.setPageSize(QPagedPaintDevice::A4);
    QPainter painter(&writer);
    charts->render(&painter);
    painter.end();
}

void ChartWindow::updateUI()
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
                themeComboBox->itemData(themeComboBox->currentIndex()).toInt());

    const auto chartsUI = charts;
    if (charts->chart()->theme() != theme) {
        chartsUI->chart()->setTheme(theme);
    }
}


