#ifndef JSONREADER_H
#define JSONREADER_H
#include <datareader.h>

class JsonReader: public DataReader
{
public:
    JsonReader() = default;
    bool readData(QString const filePath, ChartData& data, QString& readError) override;
};
#endif // JSONREADER_H
