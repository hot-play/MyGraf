#include "fileexplorerwindow.h"
#include <QString>
#include <QTime>
#include <QLabel>
#include <QSplitter>

#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>

#include <datareader.h>
#include <sqlreader.h>
#include <csvreader.h>
#include <jsonreader.h>
#include <chartwindow.h>

#include <QDebug>

FileExplorerWindow::FileExplorerWindow(QWidget *parent)
    : QWidget(parent)
{
    //Устанавливаем размер главного окна
    baseLayout = new QGridLayout();
    QString homePath = QDir::homePath();
    // Определим  файловой системы:
    leftPartModel =  new QFileSystemModel(this);
    QStringList filters;
        filters << "*.sqlite" << "*.csv" << "*.json";
    leftPartModel->setNameFilters(filters);
    leftPartModel->setNameFilterDisables(false);
    leftPartModel->setRootPath(homePath);
    //Показатьв виде "дерева". Пользуемся готовым видом(TreeView):
    treeView = new QTreeView();
    // Устанавливаем модель данных для отображения
    treeView->setModel(leftPartModel);
    //Раскрываем все папки первого уровня
    treeView->expandAll();
    // Создаем объект "сплиттер(разделитель)"
    baseLayout->addWidget(treeView);
    setLayout(baseLayout);
    QItemSelectionModel *selectionModel = treeView->selectionModel();
    treeView->header()->resizeSection(0, 200);
    errorMessager = new QErrorMessage(this);
    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &FileExplorerWindow::on_selectionChangedSlot);
    //Пример организации установки курсора в TreeView относительно модельного индекса
    QItemSelection toggleSelection;
    //Объявили модельный индекс topLeft
    QModelIndex topLeft;
    //Получили индекс из модели
    topLeft = leftPartModel->index(homePath);
    toggleSelection.select(topLeft, topLeft);
    selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}

void FileExplorerWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    //Q_UNUSED(selected);
    Q_UNUSED(deselected);

    QModelIndexList indexs =  selected.indexes();

    QString filePath = leftPartModel->filePath(indexs.constFirst());
    if (CheckFileType(filePath)) {
        // Подключение слота
        ChartData data;
        QString error;
        DataReaderPtr reader = GetReaderByFile(filePath);
        bool readResult = reader->readData(filePath, data, error);
        if (readResult) {
            dataChangeSignal(data);
        } else {
            errorMessager->showMessage(error);
        }
    }
}

DataReaderPtr FileExplorerWindow::GetReaderByFile(QString filePath) {
    QFileInfo file(filePath);
    if (file.suffix() == "csv") {
        return std::make_shared<CsvReader>();
    } else if (file.suffix() == "json") {
        return std::make_shared<JsonReader>();
    } else if (file.suffix() == "sqlite") {
        return std::make_shared<SqlReader>();
    }
    return nullptr;
}

bool FileExplorerWindow::CheckFileType(QString filePath) {
    QFileInfo file(filePath);
    if ((file.suffix() == "csv") || (file.suffix() == "json") || (file.suffix() == "sqlite"))
        return true;
    return false;
}


