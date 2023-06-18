#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QtWidgets/QGridLayout>
#include <QPushButton>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


struct ChartDataPoint {
    QString date;
    float value;
};

struct ChartData {
    QString dateAxisTitle = "date";
    QString valueAxisTitle = "value";
    QString chartTitle;
    QVector<ChartDataPoint> points;
};

class WidgetChart:public QWidget
{
    Q_OBJECT
public:
    explicit WidgetChart(QWidget *parent = nullptr);
    ~WidgetChart(){}
};
class ChartWindow: public QWidget
{
    Q_OBJECT
public:
    explicit ChartWindow(QWidget *parent = nullptr);
private Q_SLOTS:
    void updateUI();
    void switchType();
private:
    QComboBox *createThemeBox() const;
    QComboBox *createTypeBox() const;
    void connectSignals();
    QChart *createBarChart() const;
    QChart *createPieChart() const;
    QChart *createLineChart() const;
    QPushButton *createPushButton() const;
public slots:
    void switchData(ChartData data);
    void printChartToPdf() const;
private:
    QGridLayout *baseLayout;
    QChartView *charts;
    ChartData chartData;
    QComboBox *themeComboBox;
    QComboBox *typeComboBox;
    QPushButton *saveButton;
};

#endif // CHARTWINDOW_H
