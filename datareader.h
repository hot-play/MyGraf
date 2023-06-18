#ifndef DATAREADER_H
#define DATAREADER_H
#include <chartwindow.h>
#include <memory>

struct avg
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
