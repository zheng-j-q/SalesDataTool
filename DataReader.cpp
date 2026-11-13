#include "stdafx.h"
#include "DataReader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

DataReader::DataReader() {}

QVector<QVariantMap> DataReader::readCSV(const QString& filePath)
{
    QVector<QVariantMap> result;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << filePath;
        return result;
    }

    QTextStream in(&file);
    QString headerLine = in.readLine();
    QStringList headers = headerLine.split(',');

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;
        QStringList fields = line.split(',');

        QVariantMap row;
        for (int i = 0; i < headers.size() && i < fields.size(); ++i) {
            row[headers[i].trimmed()] = fields[i].trimmed();
        }
        result.append(row);
    }
    file.close();
    return result;
}

QVector<QVariantMap> DataReader::readFromMySQL(const QString& connectionStr)
{
    // 预留：后续实现 MySQL 连接
    Q_UNUSED(connectionStr);
    return QVector<QVariantMap>();
}