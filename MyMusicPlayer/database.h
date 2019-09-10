#pragma once

#include <QObject>
#include <QColor>

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    void readData();

signals:
    void readingData(int num);
    void startReadData(int total, const QString& msg);
};
