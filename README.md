# 销售数据统计分析工具 (SalesDataTool)

一个基于 **Qt 6** + **C++17** 开发的桌面业务工具，采用三层架构设计，集成了逆波兰表达式引擎和 MySQL 数据持久化功能。

## ✨ 核心特性

- **三层架构**：UI 层 / 业务逻辑层 / 数据层完全解耦，核心模块可独立编译运行
- **逆波兰表达式引擎**：支持 `+ - * /` 四则运算，可用于自定义统计公式
- **CSV 数据导入**：一键加载 CSV 文件，自动解析并显示在表格中
- **数据统计分析**：按产品汇总销量、计算总销售额
- **MySQL 数据持久化**：通过 ODBC 连接 MySQL，支持数据的保存与读取
- **报表导出**：将表格数据一键导出为 CSV 文件

## 🛠 技术架构

```text
┌─────────────────────────────────────────────────┐
│  UI 层 (SalesDataTool.ui + SalesDataTool.cpp)   │
│  - 界面布局 / 用户交互 / 事件响应                 │
├─────────────────────────────────────────────────┤
│  业务逻辑层 (DataProcessor)                      │
│  - 统计分析 / 逆波兰表达式引擎                   │
├─────────────────────────────────────────────────┤
│  数据层 (DataReader)                            │
│  - CSV 读取 / MySQL 读写（ODBC）                 │
└─────────────────────────────────────────────────┘
```


## 🚀 快速开始

### 环境要求

- Windows 10 / 11
- Visual Studio 2022
- Qt 6.9.3 + Qt VS Tools
- MySQL 8.0 + MySQL ODBC Connector

### 编译运行

1. 用 Visual Studio 打开 `SalesDataTool.sln`
2. 确保已配置 Qt 版本（项目属性 → Qt Project Settings → Qt Modules: `core;gui;widgets;sql`）
3. 点击"本地 Windows 调试器"运行

### ODBC 数据源配置

1. 打开 **ODBC 数据源管理器（64 位）**
2. 添加 **系统 DSN** → 选择 **MySQL ODBC 8.0 Unicode Driver**
3. Data Source Name: `MySQLSales`
4. Server: `localhost`，Port: `3306`
5. User: `root`，Password: 你的密码
6. Database: `sales_data`

### 使用说明

1. **加载数据**：点击按钮，选择 CSV 文件
2. **统计分析**：查看按产品汇总和总销售额
3. **保存到数据库**：将当前数据持久化到 MySQL
4. **计算表达式**：在输入框中输入逆波兰表达式（如 `3 4 + 3 *`），点击计算
5. **导出报表**：将数据导出为 CSV 文件

## 📁 项目结构

```text
SalesDataTool/
├── SalesDataTool.ui          # Qt 界面设计文件
├── SalesDataTool.h           # 主窗口头文件（UI 层）
├── SalesDataTool.cpp         # 主窗口实现（UI 层）
├── main.cpp                  # 程序入口
├── DataReader.h              # 数据层头文件
├── DataReader.cpp            # 数据层实现
├── DataProcessor.h           # 业务逻辑层头文件（含 RPN 引擎）
├── DataProcessor.cpp         # 业务逻辑层实现
├── stdafx.h                  # 预编译头
├── stdafx.cpp                # 预编译头实现
├── SalesDataTool.qrc         # Qt 资源文件（图标/样式表等）
├── SalesDataTool.vcxproj     # Visual Studio 项目文件
└── SalesDataTool.vcxproj.filters  # VS 文件筛选器
```

## 👤 作者

郑金群 - [GitHub主页](https://github.com/zheng-j-q)

## 📄 许可证

MIT License
