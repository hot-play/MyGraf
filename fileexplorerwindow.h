#ifndef FILEEXPLORERWINDOW_H
#define FILEEXPLORERWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QtWidgets/QGridLayout>
#include <chartwindow.h>
#include <QErrorMessage>
#include <datareader.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class FileExplorerWindow;
}
QT_END_NAMESPACE

class FileExplorerWindow : public QWidget

{
    Q_OBJECT
private:
    QErrorMessage * errorMessager;
    QGridLayout * baseLayout;
    QFileSystemModel * leftPartModel;
    QTreeView * treeView;
public:
    explicit FileExplorerWindow(QWidget *parent = nullptr);
private:
    DataReaderPtr GetReaderByFile(QString filePath);
    bool CheckFileType(QString filePath);
signals:
    void dataChangeSignal(ChartData data);
private slots:
    void selectionFile(const QItemSelection &selected);
};

#endif // FILEEXPLORERWINDOW_H
