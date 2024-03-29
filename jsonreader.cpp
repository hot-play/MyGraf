#include "jsonreader.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

bool JsonReader::readData(const QString filePath, ChartData &data, QString &readError) {
    // Открываем и считываем содержимое файла
    QString fileData;
    QFile file;
    file.setFileName(filePath);
    if (!tryOpenFile(filePath)) {
        readError = "Файл не может быть открыт";
        return false;
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    fileData = file.readAll();
    file.close();
    // Начинаем парсить данные
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(fileData.toUtf8(), &error);
    if (doc.isObject()) {
        QJsonObject jsonData = doc.object();
        QJsonArray jsonArray = jsonData["chartData"].toArray();
        if (jsonArray.isEmpty()) {
            readError = "В файле нет массива chartData, формат не поддерживается";
            return false;
        }
        QMap<QString, forAvgComputingValue> dataMap;
        foreach (const QJsonValue & value, jsonArray) {
            if (value.isObject())
            {
                QJsonObject obj = value.toObject();
                QString date  = obj["date"].toString().split(' ').first().split(".").at(2) + "." +
                        obj["date"].toString().split(' ').first().split(".").at(1);
                float value = obj["value"].toDouble();
                dataMap[date].value += value;
                dataMap[date].count += 1;
            }
            else {
                readError = "Ошибка при чтении файла";
                return false;
            }
        }
        // Так как данные представляю в формате yyyy.MM и value, то нужно посчитать
        // cреднее значение value
        ChartDataPoint dataPoint;
        for(auto pair : dataMap.toStdMap()) {
            dataPoint.date = pair.first;
            dataPoint.value = (pair.second.value) / (pair.second.count);
            data.points.push_back(dataPoint);
        }
        data.chartTitle = filePath.split('/').last();
    } else {
        readError = "Ошибка чтения файла";
        return false;
    }
    return true;
}
