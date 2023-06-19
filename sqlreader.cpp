#include "sqlreader.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>

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

    QMap<QString, forAvgComputingValue> dataMap;
    while (query.next()) {
        QString date = query.value(0).toString().split(' ').first().split(".").at(2) + "." +
                query.value(0).toString().split(' ').first().split(".").at(1);
        float value = query.value(1).toFloat();

        dataMap[date].value += value;
        dataMap[date].count += 1;
    }
    ChartDataPoint dataPoint;
    for(auto pair : dataMap.toStdMap()) {
        dataPoint.date = pair.first;
        dataPoint.value = (pair.second.value) / (pair.second.count);
        data.points.push_back(dataPoint);
    }
    dataBase.close();
    return true;
}
