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


ChartWindow::ChartWindow(QWidget *parent) :
    QWidget(parent),
    m_themeComboBox(createThemeBox()),
    m_typeComboBox(createTypeBox())
{
    connectSignals();
    // create layout
    m_baseLayout = new QGridLayout();
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(new QLabel("Theme:"));
    settingsLayout->addWidget(m_themeComboBox);
    settingsLayout->addWidget(new QLabel("Type:"));
    settingsLayout->addWidget(m_typeComboBox);
    settingsLayout->addStretch();
    m_baseLayout->addLayout(settingsLayout, 0, 0, 1, 3);

    QChartView *chartView;
    chartView = new QChartView(createPieChart());
    m_baseLayout->addWidget(chartView, 1, 0);
    m_charts = chartView;

    setLayout(m_baseLayout);
    updateUI();
}

void ChartWindow::connectSignals()
{
    connect(m_themeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ChartWindow::updateUI);
    connect(m_typeComboBox,
            static_cast<void (QComboBox::*)(int )>(&QComboBox::currentIndexChanged),
            this, &ChartWindow::switchType);
}

QComboBox *ChartWindow::createThemeBox() const
{
    QComboBox *themeComboBox = new QComboBox();
    themeComboBox->addItem("Color", QChart::ChartThemeLight);
    themeComboBox->addItem("White-Black", QChart::ChartThemeDark);
    return themeComboBox;
}

QComboBox *ChartWindow::createTypeBox() const
{
    QComboBox *typeComboBox = new QComboBox();
    typeComboBox->addItem("Line Chart", 1);
    typeComboBox->addItem("Pie Chart", 2);
    typeComboBox->addItem("Bar Chart", 3);
    return typeComboBox;
}

QChart *ChartWindow::createLineChart() const
{
    auto series = new QLineSeries {};
    QChart * chart = new QChart();

    for (auto point: chartData.points) {
        float x, y;

        auto date = QDateTime {{QDate::fromString(point.date, "yyyy.MM")}};
        x = (float) date.toMSecsSinceEpoch();
        y = point.value;
        series->append({x, y});
    }

    chart->addSeries(series);
    chart->legend()->hide();

    auto dateTimeAxis = new QDateTimeAxis {};
    dateTimeAxis->setTitleText(chartData.dateAxisTitle);
    auto valueAxis = new QValueAxis {};
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
    auto series = new QPieSeries {};
    QChart * chart = new QChart();

    for (auto point: chartData.points) {
        float value {point.value};
        series->append(point.date, value);
    }

    chart->addSeries(series);
    chart->legend()->show();

    return chart;
}

QChart *ChartWindow::createBarChart(int count) const
{
    auto series = new QBarSeries {};
    QChart *chart = new QChart();
    for (auto point: chartData.points) {
        auto value {point.value};
        auto barSet = new QBarSet {point.date};
        barSet->append(value);
        series->append(barSet);
    }
    auto valueAxis = new QValueAxis {};
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
                m_typeComboBox->itemData(m_typeComboBox->currentIndex()).toInt());
    QChartView * chart;
    if (type == 1) {
        chart = new QChartView(createLineChart());
    }
    else if (type == 2) {
        chart = new QChartView(createPieChart());
    } else if (type == 3) {
        chart = new QChartView(createBarChart(chartData.points.count()));
    }
    delete m_charts;
    m_charts = chart;
    m_baseLayout->addWidget(chart, 1, 0);
    setLayout(m_baseLayout);
    updateUI();
}

void ChartWindow::switchData(ChartData data) {
    chartData = data;
    switchType();
    updateUI();
}

void ChartWindow::updateUI()
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
                m_themeComboBox->itemData(m_themeComboBox->currentIndex()).toInt());

    const auto charts = m_charts;
    if (m_charts->chart()->theme() != theme) {
        charts->chart()->setTheme(theme);
    }
}


