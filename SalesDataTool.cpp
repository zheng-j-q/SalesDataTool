#include "stdafx.h"
#include "SalesDataTool.h"
#include "ui_SalesDataTool.h"
#include "DataReader.h"
#include "DataProcessor.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QMessageBox>

#include <QStandardPaths>

SalesDataTool::SalesDataTool(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SalesDataToolClass)
    , m_reader(new DataReader())
    , m_processor(new DataProcessor())
{
    ui->setupUi(this);

    connect(ui->btnLoadCSV, &QPushButton::clicked,
        this, &SalesDataTool::onLoadCSV);
    connect(ui->btnAnalyze, &QPushButton::clicked,
        this, &SalesDataTool::onAnalyze);
    connect(ui->btnTestDB, &QPushButton::clicked,
        this, &SalesDataTool::onTestDB);
    connect(ui->btnSaveToDB, &QPushButton::clicked,
        this, &SalesDataTool::onSaveToDB);
    connect(ui->btnExport, &QPushButton::clicked,
        this, &SalesDataTool::onExport);
    connect(ui->btnCalculate, &QPushButton::clicked,
        this, &SalesDataTool::onCalculate);

    ui->lblStatus->setText("Ready");
}

SalesDataTool::~SalesDataTool()
{
    delete ui;
    delete m_reader;
    delete m_processor;
}

void SalesDataTool::onLoadCSV()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        "Select CSV File", "", "CSV Files (*.csv);;All Files (*)");

    if (filePath.isEmpty()) return;

    ui->lblStatus->setText("Loading...");

    m_currentData = m_reader->readCSV(filePath);

    if (m_currentData.isEmpty()) {
        QMessageBox::warning(this, "Info", "No data found. Please check the file format.");
        ui->lblStatus->setText("Load Failed");
        return;
    }

    ui->tableData->setRowCount(0);
    QStringList headers = m_currentData.first().keys();
    ui->tableData->setColumnCount(headers.size());
    ui->tableData->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < m_currentData.size() && i < 1000; ++i) {
        ui->tableData->insertRow(i);
        const auto& row = m_currentData[i];
        for (int j = 0; j < headers.size(); ++j) {
            ui->tableData->setItem(i, j,
                new QTableWidgetItem(row[headers[j]].toString()));
        }
    }

    ui->lblStatus->setText(QString("Loaded %1 rows").arg(m_currentData.size()));
}

void SalesDataTool::onAnalyze()
{
    if (m_currentData.isEmpty()) {
        QMessageBox::warning(this, "Info", "Please load data first.");
        return;
    }

    ui->lblStatus->setText("Analyzing...");

    auto byProduct = m_processor->summarizeByProduct(m_currentData);
    double total = m_processor->calculateTotalRevenue(m_currentData);

    QString resultText = "===== Summary =====\n\n";
    resultText += "By Product:\n";
    for (auto it = byProduct.begin(); it != byProduct.end(); ++it) {
        resultText += QString("  %1 : %2\n").arg(it.key()).arg(it.value().toString());
    }
    resultText += QString("\nTotal Revenue: %1\n").arg(total);

    if (ui->lineExpression) {
        QString expr = ui->lineExpression->text();
        if (!expr.isEmpty()) {
            double rpnResult = m_processor->evaluateRPN(expr);
            resultText += QString("\nExpression: %1 = %2\n").arg(expr).arg(rpnResult);
        }
    }

    ui->textResult->setText(resultText);
    ui->lblStatus->setText("Done");
}

void SalesDataTool::onTestDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("MySQLSales");  // 这是你在 ODBC 里起的名字
    db.setUserName("root");
    db.setPassword("123456");          // 改成你的密码

    if (!db.open()) {
        QMessageBox::warning(this, "connect default", db.lastError().text());
        return;
    }

    QMessageBox::information(this, "yes!", "have connected MySQL sales_data ！");

    // 可选：测试查询
    QSqlQuery query;
    if (query.exec("SHOW TABLES")) {
        QString msg = "当前数据库中的表:\n";
        while (query.next()) {
            msg += query.value(0).toString() + "\n";
        }
        QMessageBox::information(this, "list page", msg);
    }

    db.close();
}

void SalesDataTool::onSaveToDB()
{
    // 1. 检查是否有数据
    if (m_currentData.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先加载 CSV 数据！");
        return;
    }

    // 2. 连接数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("MySQLSales");
    db.setUserName("root");
    db.setPassword("123456");  // 改成你的密码

    if (!db.open()) {
        QMessageBox::warning(this, "数据库连接失败", db.lastError().text());
        return;
    }

    // 🔑 添加这一行：告诉 MySQL 使用 sales_data 数据库
    QSqlQuery useQuery(db);
    useQuery.exec("USE sales_data;");

    // 3. 创建表（如果不存在）
    QSqlQuery query;
    QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS sales_data.sales_records (
            id INT AUTO_INCREMENT PRIMARY KEY,
            date VARCHAR(50),
            product VARCHAR(255),
            sales INT,
            amount INT
        )
    )";
    if (!query.exec(createTableSQL)) {
        QMessageBox::warning(this, "建表失败", query.lastError().text());
        db.close();
        return;
    }

    // 4. 准备插入语句（使用预处理语句，防止 SQL 注入，性能也更好）
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO sales_records (date, product, sales, amount) VALUES (?, ?, ?, ?)");

    // 5. 遍历数据，逐行插入
    int successCount = 0;
    for (const auto& row : m_currentData) {
        QString date = row["date"].toString();
        QString product = row["product"].toString();
        int sales = row["sales"].toInt();
        int amount = row["amount"].toInt();

        // 如果字段名不匹配，尝试其他常见写法
        if (date.isEmpty()) date = row["Date"].toString();
        if (product.isEmpty()) product = row["Product"].toString();
        if (sales == 0) sales = row["Sales"].toInt();
        if (amount == 0) amount = row["Amount"].toInt();

        insertQuery.bindValue(0, date);
        insertQuery.bindValue(1, product);
        insertQuery.bindValue(2, sales);
        insertQuery.bindValue(3, amount);

        if (insertQuery.exec()) {
            successCount++;
        }
        else {
            // 打印错误，但不中断循环
            qDebug() << "插入失败:" << insertQuery.lastError().text();
        }
    }

    // 6. 关闭连接
    db.close();

    // 7. 显示结果
    QMessageBox::information(this, "✅ 保存完成",
        QString("成功保存 %1 条记录到数据库！").arg(successCount));
}

void SalesDataTool::onExport()
{
    if (m_currentData.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先加载数据！");
        return;
    }

    // 获取桌面路径
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filePath = desktopPath + "/export_result.csv";

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // 写表头
        QStringList headers = m_currentData.first().keys();
        out << headers.join(",") << "\n";

        // 写数据
        for (const auto& row : m_currentData) {
            QStringList values;
            for (const QString& key : headers) {
                values << row[key].toString();
            }
            out << values.join(",") << "\n";
        }

        file.close();
        QMessageBox::information(this, "✅ 导出成功", "文件已保存到桌面：\n" + filePath);
    }
    else {
        QMessageBox::warning(this, "导出失败", "无法保存文件！");
    }
}

void SalesDataTool::onCalculate()
{
    if (!ui->lineExpression) {
        QMessageBox::warning(this, "提示", "界面缺少表达式输入框！");
        return;
    }

    QString expr = ui->lineExpression->text();
    if (expr.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入表达式！\n\n示例: 3 4 + 2 *");
        return;
    }

    // 调用逆波兰引擎计算
    double result = m_processor->evaluateRPN(expr);

    // 显示结果
    QString resultText = QString("【表达式计算】: %1 = %2").arg(expr).arg(result);
    ui->textResult->setText(resultText);

    ui->lblStatus->setText("计算完成");
}