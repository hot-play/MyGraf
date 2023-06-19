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

#include<linechart.h>
#include<piechart.h>
#include<barchart.h>

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
    chartView = new QChartView(this);
    baseLayout->addWidget(chartView, 1, 0);
    setLayout(baseLayout);
}

ChartWindow::~ChartWindow() {
    delete chart;
}

void ChartWindow::initLineChart()
{
    if (chart) {
        delete chart;
    }
    chart = new LineChart(chartData);
}

void ChartWindow::switchType()
{
    // По значению в списке typeComboBox, меняем график
    QChart::ChartTheme type = static_cast<QChart::ChartTheme>(
                typeComboBox->itemData(typeComboBox->currentIndex()).toInt());
    QChartView * newChartView;
    if (type == 1) {
        chart = new LineChart(chartData);
    } else if (type == 2) {
        chart = new PieChart(chartData);
    } else if (type == 3) {
        chart = new BarChart(chartData);
    }
    QChart * newChart = chart->getChart();
    newChartView = new QChartView(newChart);
    delete chartView;
    chartView = newChartView;
    baseLayout->addWidget(newChartView, 1, 0);
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
    chartView->render(&painter);
    painter.end();
}


