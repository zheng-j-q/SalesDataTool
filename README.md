# 离线本地销售数据智能分析 Agent（SalesDataTool）

把一次 12 步菜单的高频统计变成一句话，同时**保证业务数据一步都不出设备**。

> 产品定义与验收口径：[PRD.md](PRD.md)
> 个人项目｜独立设计与开发｜2026.06 - 2026.07｜B 端 AI 产品作品集之一
> 另两个项目：[MemoryPool](https://github.com/zheng-j-q/MemoryPool) · [AsyncLogger](https://github.com/zheng-j-q/AsyncLogger)

## 这个仓库里有什么

| 层 | 内容 | 状态 |
|---|---|---|
| 底层业务工具 | Qt 桌面销售数据工具（三层架构、RPN 表达式引擎、MySQL 持久化） | **已开源，可编译运行** |
| Agent 产品层 | 用户场景、离线约束下的方案取舍、意图槽位与歧义处理、验收口径 | **设计定义见 [PRD](PRD.md)** |

## 解决什么问题

原有 Qt 桌面工具功能完整，但一次高频销售统计要沿菜单点完 12 步：加载 CSV、选维度、汇总、写表达式、存库、导出报表。路径深、顺序不能错。

更硬的一条约束是：**业务数据严禁出公网**。所以「接一个云端大模型，把菜单变成对话框」从一开始就不成立——它解决交互问题，付出的却是数据合规代价。本项目要回答的是：在不改业务代码、不把数据送出设备的前提下，能不能把 12 步变成一句话。

## 产品判断

1. **全离线轻量模型，不接云端 API。** 数据不出设备是硬约束，不是偏好；纯规则匹配又覆盖不了自然语言的表达变化。
2. **零侵入薄封装，不重写业务代码。** 稳定核心逻辑改造收益有限、回归风险高，只在外层增加自然语言交互层。
3. **缺槽位就反问，不猜。** 统计口径错了会输出一份看起来完全正常的错报表，比直接报错更危险。
4. **结果附口径与所用公式。** 业务用户要能复核；这也是验收依据。
5. **工具入参来自槽位，不由模型自由生成。** 明确模型能调什么、不能调什么。

## 效果与口径

| 指标 | 变化 | 说明 |
|---|---|---|
| 高频任务操作步骤 | 12 步 → 1 句指令 | 完成一次典型统计所需的显式用户操作数；评测场景与计数规则见 [PRD 第 8 节](PRD.md) |

其余效率类数值涉及样本任务与计时口径，统一放在 PRD 的「指标定义与验收口径」中说明后再对外引用。

## 运行与验证

环境：Windows 10 / 11、Visual Studio 2022、Qt 6.9.3 + Qt VS Tools、MySQL 8.0 + MySQL ODBC Connector。

```bash
git clone https://github.com/zheng-j-q/SalesDataTool.git
# 用 Visual Studio 打开 .sln，确保 Qt 模块包含 core;gui;widgets;sql
# 点击「本地 Windows 调试器」运行
```

### ODBC 数据源配置（仅本地测试用）

1. 打开「ODBC 数据源管理器（64 位）」。
2. 添加系统 DSN，选择 MySQL ODBC 8.0 Unicode Driver。
3. Data Source Name：`MySQLSales`。
4. Server：`localhost`，Port：`3306`。
5. User / Password：填写你自己的本地测试凭据，**不要使用真实业务账号，也不要把口令写进仓库**。

### 原有工具操作流程（作为交互改造的基线）

加载 CSV → 按产品汇总与总销售额统计 → 保存到 MySQL → 输入逆波兰表达式（如 `3 4 + 3 *`）计算 → 导出 CSV 报表。

测试覆盖：CSV 导入、按产品汇总、RPN 表达式引擎（+ - * /）、MySQL 持久化、CSV 报表导出、三层架构解耦验证。

### 项目结构

```text
SalesDataTool/
├── SalesDataTool.ui / .h / .cpp        # UI 层
├── DataReader.h / .cpp                 # 数据层
├── DataProcessor.h / .cpp              # 业务逻辑层（含 RPN 引擎）
├── main.cpp / stdafx.h / stdafx.cpp
├── SalesDataTool.qrc / .vcxproj / .vcxproj.filters
├── PRD.md                              # 产品定义与验收口径
└── README.md
```

## 关于这个作品

郑金群，独立设计与开发。三个作品集项目分别对应不同的产品判断：本项目的「数据合规与交互歧义」、[MemoryPool](https://github.com/zheng-j-q/MemoryPool) 的「存量系统零侵入」、[AsyncLogger](https://github.com/zheng-j-q/AsyncLogger) 的「运维动作权限分级」。

- 简历与作品集入口：[github.com/zheng-j-q](https://github.com/zheng-j-q)
- 许可：MIT（见 [LICENSE](LICENSE)）

