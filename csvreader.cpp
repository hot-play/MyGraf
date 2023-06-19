#include "csvreader.h"

bool CsvReader::readData(const QString filePath, ChartData &data, QString &readError) {
    // Открываем и считываем содержимое файла
    QString fileData;
    QFile file;
    file.setFileName(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
            readError = "Файл не может быть открыт";
            return false;
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    fileData = file.readAll();
    file.close();
    // Начинаем парсить данные
    QStringList csvData = fileData.split("\n");
    QMap<QString, forAvgComputingValue> dataMap;
    foreach (const QString & csvStroke, csvData) {
        QString csvDate = csvStroke.split(',').at(0);
        QString csvValue = csvStroke.split(',').at(1);
        auto date = csvDate.split(' ').first().split(".").at(2) + "." +
                csvDate.split(' ').first().split(".").at(1);
        float value = csvValue.toFloat();
        dataMap[date].value += value;
        dataMap[date].count += 1;
    }
    // Так как данные представляю в формате yyyy.MM и value, то нужно посчитать
    // cреднее значение value
    ChartDataPoint dataPoint;
    for(auto pair : dataMap.toStdMap()) {
        dataPoint.date = pair.first;
        dataPoint.value = (pair.second.value) / (pair.second.count);
        data.points.push_back(dataPoint);
    }
    return true;
}
