#include "sqlreader.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QDebug>

bool SqlReader::readData(const QString filePath, ChartData &data, QString &readError) {
    if (!QFile::exists(filePath)) {
        readError = "Файл не существует";
        return false;
    }
    auto static dataBase = QSqlDatabase::addDatabase("QSQLITE");

    dataBase.setDatabaseName(filePath);

    if (!dataBase.open()) {
        readError = "База данных повреждена";
        return false;
    }
    auto tables = dataBase.tables();

    if (tables.empty()) {
        readError = "В данной базе данных нет таблиц";
        return false;
    }

    auto table = tables.first();
    auto query = QSqlQuery {"select * from " + table};
    ChartDataPoint dataPoint;
    dataPoint.date = query.value(0).toString();
    dataPoint.value = query.value(1).toString();
    data.points.push_back(dataPoint);
    while (query.next()) {
        dataPoint.date = query.value(0).toString();
        dataPoint.value = query.value(1).toString();
        data.points.push_back(dataPoint);
    }
    dataBase.close();
    return true;
}
