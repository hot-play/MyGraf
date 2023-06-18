#ifndef SQLREADER_H
#define SQLREADER_H
#include <datareader.h>

class SqlReader: public DataReader
{
public:
    SqlReader() = default;
    bool readData(QString const filePath, ChartData& data, QString& readError) override;
};

#endif // SQLREADER_H
