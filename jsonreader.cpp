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
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
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
        ChartDataPoint dataPoint;
        foreach (const QJsonValue & value, jsonArray) {
            if (value.isObject())
            {
                QJsonObject obj = value.toObject();
                dataPoint.date = obj["date"].toString();
                dataPoint.value = obj["value"].toDouble();
                data.points.push_back(dataPoint);
            }
            else {
                readError = "Ошибка при чтении файла";
                return false;
            }
        }
    }
    return true;
}
