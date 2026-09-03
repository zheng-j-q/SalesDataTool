#pragma once
#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QVector>
#include <QVariantMap>
#include <QString>

// 业务逻辑层：统计分析 + 逆波兰表达式引擎
class DataProcessor
{
public:
    DataProcessor();

    // 按产品汇总
    QVariantMap summarizeByProduct(const QVector<QVariantMap>& data);

    // 计算总销售额
    double calculateTotalRevenue(const QVector<QVariantMap>& data);

    // 逆波兰表达式计算（支持 + - * / ( )）
    double evaluateRPN(const QString& expression);
};

#endif