#ifndef DATAREADER_H
#define DATAREADER_H
#include <chartwindow.h>

class DataReader
{
public:
    virtual bool readData(QString const filePath, DataTable& data, QString& readError) = 0;
};

#endif // DATAREADER_H
