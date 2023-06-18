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
    QStringList csvData = fileData.split(";");
    bool index = 0;
    ChartDataPoint dataPoint;
    foreach (const QString & value, csvData) {
        if (index) {
            dataPoint.value = value;
            data.points.push_back(dataPoint);
            index = !index;
        } else {
            dataPoint.date = value;
            index = !index;
        }
    }
    if (!index) {
        readError = "Файл не корректен";
        return false;
    }
    return true;
}
