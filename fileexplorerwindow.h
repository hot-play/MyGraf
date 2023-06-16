#ifndef FILEEXPLORERWINDOW_H
#define FILEEXPLORERWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QtWidgets/QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class FileExplorerWindow;
}
QT_END_NAMESPACE

class FileExplorerWindow : public QWidget

{
    Q_OBJECT

public:
    FileExplorerWindow(QWidget *parent = nullptr);

private slots:
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    bool CheckFileType(QString filePath);

private:
    QGridLayout * baseLayout;
    QStatusBar * statusBar;
    QFileSystemModel *leftPartModel;
    QTreeView *treeView;
};

#endif // FILEEXPLORERWINDOW_H
