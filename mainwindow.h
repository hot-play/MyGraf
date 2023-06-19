#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartGlobal>
#include <chartwindow.h>
#include <fileexplorerwindow.h>


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

public:
    MainWindow();
private:
    FileExplorerWindow * fileExplorerWindow;
    ChartWindow * chartWindow;
};

#endif // MAINWINDOW_H
