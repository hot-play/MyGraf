#ifndef CSVREADER_H
#define CSVREADER_H
#include <datareader.h>

class CsvReader: public DataReader
{
public:
    bool readData(QString const filePath, ChartData& data, QString& readError) override;
};

#endif // CSVREADER_H
