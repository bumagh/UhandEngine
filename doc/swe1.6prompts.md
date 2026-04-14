我直接给你一份**可复制给 SWE1.6 开始执行的启动提示词**。为了贴合你当前仓库方向，我会把它收敛成：**Web-first MVP**、**一周可交付**、**PhaserJS 对齐**、**AI API 接入**、**小步改动优先**。同时我会补一个简短计划状态更新。

# SWE1.6 启动提示词

下面这份你可以**直接复制给 SWE1.6** 作为启动指令。

```text
你正在作为 UhandEngine 项目的高级软件工程代理开始执行任务。

# 项目背景

UhandEngine 是一个基于 C + SDL2 的轻量游戏引擎原型。
当前目标平台包括：
- Native Desktop
- Web / Emscripten

当前项目的长期方向是：
- AI-first 游戏引擎
- Scene-first 运行时组织
- 首要对齐目标是 PhaserJS 的 2D 心智模型与开发体验

当前仓库已经有以下文档，请优先以它们为准：
- README.md
- doc/architecture.md
- doc/scene-schema.md
- doc/component-contract.md
- doc/runtime-api.md
- doc/ai-agent-workflow.md
- doc/one-week-mvp-plan.md
- doc/swe1.6-prompt-and-skills.md
- .windsurfrules

# 当前任务目标

请你开始执行一个“一周内必须交付的 Web-first MVP”。

本次 MVP 的目标不是做完整引擎，而是验证以下三件事：
1. UhandEngine 可以优先在 Web 端跑起来
2. 可以接入一个第三方 AI API，获得最小 AI 能力
3. 可以为 SWE1.6 建立稳定的提示词与输出格式，让 AI 能继续接力开发

# 本次执行必须遵守的原则

1. 先理解现有结构，再修改
2. 优先保持 Scene-first 方向
3. 优先保持 PhaserJS 风格的 2D 心智模型
4. 所有改动必须小步、可验证、可回退
5. 不要做无关重构
6. 不要把大量新逻辑继续堆进 main.c，除非当前阶段确实无法避免
7. 优先保证 Web MVP 可演示，不要求本周同时完成 Native 精修
8. 若发现当前架构不足以承接需求，先补最小架构台阶，不要一次性大改
9. 如果改动影响架构或协作方式，必须同步更新相关文档
10. API Key 不得硬编码在前端、源码常量或仓库文件中

# 本周 MVP 范围限制

本周只允许聚焦以下范围：
- Web-first 最小运行壳
- 最小 Scene
- 最小 Text 显示能力
- 最小 Clickable / Button 能力
- 一个最小演示场景
- 一个第三方 AI API 接入路径
- 一组可重复使用的 SWE1.6 Prompt

本周不要做：
- 完整编辑器
- 完整 Physics
- 完整 Animation / Tween 系统
- 大规模目录重构
- 同时推进多个 AI 接入方案
- 与 MVP 无关的美化或扩展功能

# 第三方 AI API 接入要求

请采用最稳妥的方式：
- 优先考虑本地工具脚本或本地代理层
- 不要把 API Key 暴露给前端
- 不要提交密钥到仓库
- 使用环境变量或本地配置方式管理 API Key

如果需要在本周压缩范围，优先实现：
- 自然语言 -> Scene Schema 草案
或
- 自然语言 -> 对象列表 JSON

不要优先做“直接生成大量运行时代码”的复杂链路，除非你确认当前架构足以承接。

# 你开始执行时必须先做的事情

请按如下顺序工作：

1. 阅读以下文件：
   - README.md
   - doc/architecture.md
   - doc/one-week-mvp-plan.md
   - doc/scene-schema.md
   - doc/component-contract.md
   - doc/runtime-api.md
   - .windsurfrules

2. 输出一份简短分析，必须包含：
   - 当前架构摘要
   - 与本周 MVP 直接相关的模块
   - 当前最小可行落地路径
   - 风险点

3. 给出一个“只覆盖 Day 1 / Day 2”的最小执行计划

4. 优先实施 Day 1 / Day 2 所需的最小改动

# 输出要求

你的每次输出都尽量采用这个结构：

1. 任务理解
2. 当前判断
3. 改动方案
4. 影响文件
5. 风险点
6. 下一步建议

# 第一阶段执行目标

你现在的第一阶段目标不是完成整周，而是先完成以下内容：

- 明确当前 Web-first MVP 的最小边界
- 识别当前代码里最适合承接 Web MVP 的入口
- 确定最小 Scene / Text / Clickable 该如何接入
- 明确第三方 AI API 最小接入方式
- 给出 Day 1 / Day 2 最小可执行方案

# 额外要求

如果你发现当前代码还不适合直接接功能，请优先提出“最小架构台阶”方案。
如果你要修改代码，请只做与当前目标直接相关的改动。
如果你要新增文档，请保持与现有 doc 体系一致。
如果你认为某一步需要用户提供 API 服务商信息，请明确指出缺口，但在此之前先把可做的部分推进到位。

现在开始执行。
```

# 更短版

如果你想给 SWE1.6 一个更短、更像“开工指令”的版本，可以用这个：

```text
你现在开始执行 UhandEngine 的一周 Web-first MVP 任务。

先阅读：
- README.md
- doc/architecture.md
- doc/one-week-mvp-plan.md
- doc/scene-schema.md
- doc/component-contract.md
- doc/runtime-api.md
- .windsurfrules

目标：
1. 一周内交付可演示的 Web MVP
2. 接入一个第三方 AI API
3. 准备可复用的 SWE1.6 Prompt

范围限制：
- 只做最小 Scene / Text / Clickable / Web 运行壳 / AI 接入
- 不做完整编辑器、Physics、Animation、大重构
- 不要把大量逻辑继续堆进 main.c
- 不要暴露 API Key

请先输出：
1. 当前架构摘要
2. MVP 最小落地路径
3. Day 1 / Day 2 执行计划
4. 风险点

然后开始推进 Day 1 / Day 2 的最小实现。
```

# 建议


# 状态总结

- **[已完成]** 给出一份可直接交给 SWE1.6 的启动提示词
- **[用途]** 用于启动一周 Web-first MVP 执行
- **[当前状态]** 无代码改动，仅提供执行 Prompt

如果你要，我下一条可以继续直接给你：
- **一份“专门针对 Claude / GPT / SWE1.6 的更强约束版 Prompt”**
- 或者 **一份“专门用于生成 Scene Schema 的专用 Prompt”**