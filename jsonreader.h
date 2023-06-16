#ifndef JSONREADER_H
#define JSONREADER_H
#include <datareader.h>

class JsonReader: public DataReader
{
public:
    bool readData(QString const filePath, DataTable& data, QString& readError);
};
#endif // JSONREADER_H
