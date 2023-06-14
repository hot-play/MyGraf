#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QTime>
#include <QLabel>
#include <QSplitter>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>

#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>


//#include <QtCharts/QAbstractAxis>
//#include <QtCharts/QAbstractSeries>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chartWindow = new ChartWindow();

    //Устанавливаем размер главного окна
    this->setGeometry(100, 100, 1500, 1000);
    // ---------------------------------------
    QString homePath = QDir::homePath();
    // Определим  файловой системы:
    leftPartModel =  new QFileSystemModel(this);
    leftPartModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    leftPartModel->setRootPath(homePath);

    rightPartModel = new QFileSystemModel(this);
    rightPartModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    rightPartModel->setRootPath(homePath);
    //Показатьв виде "дерева". Пользуемся готовым видом(TreeView):
    treeView = new QTreeView();
    // Устанавливаем модель данных для отображения
    treeView->setModel(leftPartModel);
    //Раскрываем все папки первого уровня
    treeView->expandAll();
    // Создаем объект "сплиттер(разделитель)"
    QSplitter *splitter = new QSplitter(parent);
    tableView = new QTableView;
    tableView->setModel(rightPartModel);
    splitter->addWidget(treeView);
    splitter->addWidget(tableView);
    setCentralWidget(splitter);
    /*
     * QItemSelectionModel *selectionModel отслеживает выбранные элементы в представлении treeView,
     * также отслеживает текущий выбранный элемент в представлении treeView.
    */
    QItemSelectionModel *selectionModel = treeView->selectionModel();
    treeView->header()->resizeSection(0, 200);

    //Выполняем соединения слота и сигнала который вызывается когда осуществляется выбор элемента в TreeView

    /*connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));*/

    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::on_selectionChangedSlot);

    //Пример организации установки курсора в TreeView относительно модельного индекса
    QItemSelection toggleSelection;
    //Объявили модельный индекс topLeft
    QModelIndex topLeft;
    //Получили индекс из модели
    topLeft = leftPartModel->index(homePath);
    toggleSelection.select(topLeft, topLeft);
    selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
    // ---------------------------------------

    //Еще добавми chartWindow. Теперь будут отображаться три объекта разделенные сплиттером.
    splitter->addWidget(chartWindow);
    setCentralWidget(splitter);
}

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    //Q_UNUSED(selected);
    Q_UNUSED(deselected);

    QModelIndex index = treeView->selectionModel()->currentIndex();

    QModelIndexList indexs =  selected.indexes();

    QString filePath = "";
    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = leftPartModel->filePath(ix);
        this->statusBar()->showMessage("Выбранный путь : " + leftPartModel->filePath(indexs.constFirst()));
    }
    tableView->setRootIndex(rightPartModel->setRootPath(filePath));
}

MainWindow::~MainWindow()
{
    delete ui;
}


