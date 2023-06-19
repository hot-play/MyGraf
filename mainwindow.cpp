#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QTime>
#include <QLabel>
#include <QSplitter>
QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

MainWindow::MainWindow()
{
    chartWindow = new ChartWindow(this);
    fileExplorerWindow = new FileExplorerWindow(this);
    connect(fileExplorerWindow, &FileExplorerWindow::dataChangeSignal, chartWindow, &ChartWindow::switchData);
    setWindowTitle(QString("MyGraf"));
    //Устанавливаем минимальный размер главного окна
    this->setMinimumSize(1000, 600);
    //Устанавливаем размер главного окна в полный экран
    this->setWindowState(Qt::WindowMaximized);
    // Сплитер для разделения окна проводника и графика
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(fileExplorerWindow);
    splitter->addWidget(chartWindow);
    setCentralWidget(splitter);
}


