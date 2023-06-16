#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartGlobal>
#include <chartwindow.h>
#include <fileexplorerwindow.h>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow

{
    Q_OBJECT
public:

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FileExplorerWindow *fileExplorerWindow;
    ChartWindow *chartWindow;
};

#endif // MAINWINDOW_H
