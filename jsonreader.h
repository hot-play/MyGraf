#ifndef JSONREADER_H
#define JSONREADER_H
#include <datareader.h>

class JsonReader: public DataReader
{
public:
    bool readData(QString const filePath, ChartData& data, QString& readError);
};
#endif // JSONREADER_H
