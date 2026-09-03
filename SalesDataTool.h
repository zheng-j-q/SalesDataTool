#pragma once

#include <QtWidgets/QMainWindow>
#include <QVector>
#include <QVariantMap>
#include "ui_SalesDataTool.h"
#include "DataReader.h"
#include "DataProcessor.h"

class SalesDataTool : public QMainWindow
{
    Q_OBJECT

public:
    SalesDataTool(QWidget* parent = nullptr);
    ~SalesDataTool();

private slots:
    void onLoadCSV();
    void onAnalyze();
    void onTestDB();
    void onSaveToDB();
    void onExport();
    void onCalculate();

private:
    Ui::SalesDataToolClass* ui;   // ← 注意是 SalesDataToolClass，不是 SalesDataTool
    DataReader* m_reader;
    DataProcessor* m_processor;
    QVector<QVariantMap> m_currentData;
};


