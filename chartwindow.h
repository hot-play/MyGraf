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

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

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
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
    DataTable openData() const;
    QComboBox *createThemeBox() const;
    QComboBox *createTypeBox() const;
    void connectSignals();
public:
    QChart *createAreaChart() const;
    QChart *createBarChart(int valueCount) const;
    QChart *createPieChart() const;
    QChart *createLineChart() const;
    QChart *createSplineChart() const;
    QChart *createScatterChart() const;

private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;

    QGridLayout *m_baseLayout;
    QChartView *m_charts;
    DataTable m_dataTable;
    QComboBox *m_themeComboBox;
    QComboBox *m_typeComboBox;
};

#endif // CHARTWINDOW_H
