#include "csvreader.h"

bool CsvReader::readData(const QString filePath, DataTable &data, QString &readError) {
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
    QStringList csvData = fileData.split(";");
    bool index = 0;
    DataList dataList;
    int id;
    int key;
    foreach (const QString & value, csvData) {
        if (index) {
            key = value.toInt();
            QPointF value((qreal) key, (qreal) id);
            QString label = "Label " + id;
            dataList << Data(value, label);
            index = !index;
        } else {
            id = value.toInt();
            index = !index;
        }
    }
    if (!index) {
        readError = "Файл не корректен";
        return false;
    }
    data << dataList;
    return true;
}
