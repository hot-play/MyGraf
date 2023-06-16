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


ChartWindow::ChartWindow(QWidget *parent) :
    QWidget(parent),
    m_dataTable(generateRandomData(3, 10, 7)),
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

    //create charts

    QChartView *chartView;
    chartView = new QChartView(createAreaChart());
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

DataTable ChartWindow::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    // set seed for random stuff
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    // generate random data
    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) 3 / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}
/*
void ChartWindow::fillData() const
{
    DataTable dataTable;

    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal);
            QPointF value(yValue, yValue);
            QString label = "Slice ";
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}
*/
QComboBox *ChartWindow::createThemeBox() const
{
    // settings layout
    QComboBox *themeComboBox = new QComboBox();
    themeComboBox->addItem("Color", QChart::ChartThemeLight);
    themeComboBox->addItem("White-Black", QChart::ChartThemeDark);
    return themeComboBox;
}

QComboBox *ChartWindow::createTypeBox() const
{
    QComboBox *typeComboBox = new QComboBox();
    typeComboBox->addItem("Area Chart", 1);
    typeComboBox->addItem("Bar Chart", 2);
    typeComboBox->addItem("Line Chart", 3);
    typeComboBox->addItem("Pie Chart", 4);
    typeComboBox->addItem("Spline Chart", 5);
    typeComboBox->addItem("Scatter Chart", 6);
    return typeComboBox;
}

QChart *ChartWindow::createAreaChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Area chart");

    // The lower series initialized to zero values
    QLineSeries *lowerSeries = nullptr;
    QString name("Series ");
    int nameIndex = 0;
    for (int i(0); i < m_dataTable.count(); i++) {
        QLineSeries *upperSeries = new QLineSeries(chart);
        for (int j(0); j < m_dataTable[i].count(); j++) {
            Data data = m_dataTable[i].at(j);
            if (lowerSeries) {
                const QVector<QPointF>& points = lowerSeries->pointsVector();
                upperSeries->append(QPointF(j, points[i].y() + data.first.y()));
            } else {
                upperSeries->append(QPointF(j, data.first.y()));
            }
        }
        QAreaSeries *area = new QAreaSeries(upperSeries, lowerSeries);
        area->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(area);
        chart->createDefaultAxes();
        lowerSeries = upperSeries;
    }

    return chart;
}

QChart *ChartWindow::createBarChart(int valueCount) const
{
    Q_UNUSED(valueCount);
    QChart *chart = new QChart();
    chart->setTitle("Bar chart");

    QStackedBarSeries *series = new QStackedBarSeries(chart);
    for (int i(0); i < m_dataTable.count(); i++) {
        QBarSet *set = new QBarSet("Bar set " + QString::number(i));
        for (const Data &data : m_dataTable[i])
            *set << data.first.y();
        series->append(set);
    }
    chart->addSeries(series);
    chart->createDefaultAxes();

    return chart;
}

QChart *ChartWindow::createLineChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Line chart");

    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QLineSeries *series = new QLineSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();

    return chart;
}

QChart *ChartWindow::createPieChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");

    qreal pieSize = 1.0 / m_dataTable.count();
    for (int i = 0; i < m_dataTable.count(); i++) {
        QPieSeries *series = new QPieSeries(chart);
        for (const Data &data : m_dataTable[i]) {
            QPieSlice *slice = series->append(data.second, data.first.y());
            if (data == m_dataTable[i].first()) {
                slice->setLabelVisible();
                slice->setExploded();
            }
        }
        qreal hPos = (pieSize / 2) + (i / (qreal) m_dataTable.count());
        series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.5);
        chart->addSeries(series);
    }

    return chart;
}

QChart *ChartWindow::createSplineChart() const
{
    // spine chart
    QChart *chart = new QChart();
    chart->setTitle("Spline chart");
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QSplineSeries *series = new QSplineSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}

QChart *ChartWindow::createScatterChart() const
{
    // scatter chart
    QChart *chart = new QChart();
    chart->setTitle("Scatter chart");
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QScatterSeries *series = new QScatterSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}

void ChartWindow::switchType()
{
    QChart::ChartTheme type = static_cast<QChart::ChartTheme>(
                m_typeComboBox->itemData(m_typeComboBox->currentIndex()).toInt());
    QChartView * chart;
    if (type == 1) {
        chart = new QChartView(createAreaChart());
    } else if (type == 2) {
        chart = new QChartView(createBarChart(3));
    } else if (type == 3) {
        chart = new QChartView(createLineChart());
    } else if (type == 4) {
        chart = new QChartView(createPieChart());
    } else if (type == 5) {
        chart = new QChartView(createSplineChart());
    } else if (type == 6) {
        chart = new QChartView(createScatterChart());
    }
    delete m_charts;
    m_charts = chart;
    m_baseLayout->addWidget(chart, 1, 0);
    setLayout(m_baseLayout);
    updateUI();
}

void ChartWindow::switchData(DataTable data) {
    m_dataTable = data;
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


