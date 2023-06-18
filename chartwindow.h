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
    QString value;
};

struct ChartData {
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
public:
    QChart *createBarChart(int valueCount) const;
    QChart *createPieChart() const;
    QChart *createLineChart() const;
public slots:
    void switchData(ChartData data);
private:
    QGridLayout *m_baseLayout;
    QChartView *m_charts;
    ChartData chartData;
    QComboBox *m_themeComboBox;
    QComboBox *m_typeComboBox;
};

#endif // CHARTWINDOW_H
