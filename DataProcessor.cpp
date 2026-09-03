#include "stdafx.h"
#include "DataProcessor.h"
#include <QDebug>
#include <QStack>
#include <QQueue>
#include <QMap>
#include <QStringList>

DataProcessor::DataProcessor() {}

QVariantMap DataProcessor::summarizeByProduct(const QVector<QVariantMap>& data)
{
    QVariantMap result;
    for (const auto& row : data) {
        QString product = row["product"].toString();
        if (product.isEmpty()) product = row["Product"].toString();
        if (product.isEmpty()) continue;

        double quantity = row["sales"].toDouble();
        if (quantity == 0) quantity = row["quantity"].toDouble();
        if (quantity == 0) quantity = row["Sales"].toDouble();

        result[product] = result[product].toDouble() + quantity;
    }
    return result;
}

double DataProcessor::calculateTotalRevenue(const QVector<QVariantMap>& data)
{
    double total = 0.0;
    for (const auto& row : data) {
        double value = row["amount"].toDouble();
        if (value == 0) value = row["revenue"].toDouble();
        if (value == 0) value = row["Amount"].toDouble();
        if (value == 0) value = row["Revenue"].toDouble();
        total += value;
    }
    return total;
}

// ---------- 逆波兰表达式引擎 ----------
static bool isOperator(const QString& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "(" || token == ")";
}

static int precedence(const QString& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

double DataProcessor::evaluateRPN(const QString& expression)
{
    QStringList tokens = expression.split(" ", Qt::SkipEmptyParts);
    QStack<double> stack;

    for (const QString& token : tokens) {
        // 判断是否为数字（包括负数和小数）
        bool isNumber;
        double num = token.toDouble(&isNumber);
        if (isNumber) {
            stack.push(num);
        }
        else {
            // 运算符：弹出两个操作数，计算后压回栈
            if (stack.size() < 2) {
                qDebug() << "表达式错误：操作数不足";
                return 0.0;
            }
            double b = stack.pop();
            double a = stack.pop();

            if (token == "+") {
                stack.push(a + b);
            }
            else if (token == "-") {
                stack.push(a - b);
            }
            else if (token == "*") {
                stack.push(a * b);
            }
            else if (token == "/") {
                if (b == 0.0) {
                    qDebug() << "除数不能为零";
                    return 0.0;
                }
                stack.push(a / b);
            }
            else {
                qDebug() << "未知运算符:" << token;
                return 0.0;
            }
        }
    }

    if (stack.isEmpty()) {
        return 0.0;
    }

    return stack.top();
}