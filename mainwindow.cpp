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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chartWindow = new ChartWindow(this);
    fileExplorerWindow = new FileExplorerWindow(this);
    connect(fileExplorerWindow, &FileExplorerWindow::dataChangeSignal, chartWindow, &ChartWindow::switchData);
    setWindowTitle(QString("MyGraf"));
    //Устанавливаем размер главного окна
    this->setMinimumSize(1000, 600);
    this->setWindowState(Qt::WindowMaximized);
    QSplitter *splitter = new QSplitter(parent);
    splitter->addWidget(fileExplorerWindow);
    splitter->addWidget(chartWindow);
    setCentralWidget(splitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}


