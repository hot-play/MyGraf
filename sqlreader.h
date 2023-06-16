#ifndef SQLREADER_H
#define SQLREADER_H
#include <datareader.h>

class SqlReader: DataReader
{
public:
    bool readData(QString const filePath, DataTable& data, QString& readError) override;
};

#endif // SQLREADER_H
