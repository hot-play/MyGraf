#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>

#include <QWidget>
#include <QtWidgets/QGridLayout>
#include <QPushButton>
#include <chartdata.h>
#include <ichart.h>
QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class ChartWindow: public QWidget
{
    Q_OBJECT
private:
    QGridLayout * baseLayout;
    QChartView * chartView;
    IChart * chart;
    ChartData chartData;
    QComboBox * themeComboBox;
    QComboBox * typeComboBox;
    QPushButton * saveButton;
public:
    explicit ChartWindow(QWidget *parent = nullptr);
    ~ChartWindow();
private:
    void initBarChart();
    void initPieChart();
    void initLineChart();
public slots:
    void switchData(ChartData data);
    void printChartToPdf() const;
private slots:
    void switchType();
};

#endif // CHARTWINDOW_H
