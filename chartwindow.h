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

// Реализация данного класса теоретически нарушает принцип единственной ответственности
// в следствии, реализации здесь методов create...Chart
// Объясню свою точку зрения:
// Так как объекты QChartView, работают только с указателями QChart,
// использовать умные указатели мы не можем, следовательно, роль удаления QChart,
// лежит на этом классе.
// Получается два пути:
//     Реализовать 1 экземпляр QChart и динамически его обновлять
//     Оставить ответственность на создание и удаление QChart'ов на этом классе
// Полноценной документации по созданию динамических QChart, я не нашел,
// на форумах написанно, что проще "костыльно" подменя один QChart на другой.

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
