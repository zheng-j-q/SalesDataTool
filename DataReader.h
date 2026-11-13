#pragma once
#ifndef DATAREADER_H
#define DATAREADER_H

#include <QString>
#include <QVector>
#include <QVariantMap>

// 数据层：负责读取 CSV 和数据库数据
class DataReader
{
public:
    DataReader();

    // 读取 CSV 文件，返回行数据列表（每行是一个 QVariantMap）
    QVector<QVariantMap> readCSV(const QString& filePath);

    // 预留：从 MySQL 读取数据
    QVector<QVariantMap> readFromMySQL(const QString& connectionStr);
};

#endif