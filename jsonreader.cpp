#include "jsonreader.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

bool JsonReader::readData(const QString filePath, DataTable &data, QString &readError) {
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
        DataList dataList;
        foreach (const QJsonValue & value, jsonArray) {
            if (value.isObject())
            {
                QJsonObject obj = value.toObject();
                int id = obj["id"].toInt();
                int key = obj["key"].toInt();
                QPointF value((qreal) key, (qreal) id);
                QString label = "Label " + id;
                dataList << Data(value, label);
            }
            else {
                readError = "Ошибка при чтении файла";
                return false;
            }
        }
        data << dataList;
        return true;
    }
    readError = error.errorString();
    return false;
}
