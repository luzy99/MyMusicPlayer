#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

void DataBase::readData()
{
    int max = 1000;
    emit startReadData(max, "正在打开Rainbow音乐播放器");
    for (int i = 0; i < max; ++i)
    {
        emit this->readingData(i);
    }
}
