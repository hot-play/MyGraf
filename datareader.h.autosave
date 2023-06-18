#ifndef DATAREADER_H
#define DATAREADER_H
#include <chartdata.h>
#include <QString>
#include <QMap>
#include <QFile>
#include <QVariant>
#include <memory>

struct forAvgComputingValue
{
    float value;
    int count;
};

class DataReader
{
public:
    DataReader() = default;
    virtual bool readData(QString const filePath, ChartData& data, QString& readError) = 0;
};

using DataReaderPtr = std::shared_ptr<DataReader>;

#endif // DATAREADER_H
