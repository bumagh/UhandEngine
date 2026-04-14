# SWE1.6 当前态续开发提示词

## 1. 用途

本文档用于在 **Day 5 已完成** 的基础上，让 SWE1.6 或其他 AI 编程代理继续接手 UhandEngine 的 Web-first MVP。

这不是从 0 开始的启动提示词，而是面向“当前仓库状态”的续开发提示词。

## 2. 当前仓库状态摘要

当前已完成：

- Web / Emscripten 构建链路已打通
- 浏览器中可显示最小菜单场景
- 当前 MVP 已有：
  - `Main Menu` 标题
  - `Start Game` 按钮
  - `Settings` 按钮
  - `Quit` 按钮
- 按钮点击已有最小响应
- 已接入 Claude API
- 已有本地 AI 工具脚本：`scripts/ai-client.py`
- 已有 AI 输出示例：`examples/scene-schema-example.json`
- 已有 AI 输出到 MVP 的映射说明：`doc/ai-to-mvp-mapping.md`
- 已有演示步骤与验收文档

当前仍未完成：

- `Scene` 还不是 runtime owner
- 还没有完整 Scene Schema runtime loader
- 当前 AI -> MVP 接入仍以“示例 JSON + 手工映射”为主
- 当前演示逻辑仍主要在 `src/main.c`

## 3. 可直接复制给 SWE1.6 的续开发提示词

```text
你现在继续接手 UhandEngine 的 Web-first MVP，但注意：当前仓库不是从 0 开始，而是已经完成了 Day 5 的状态。

# 当前已完成状态

当前仓库已经具备以下事实：
- Web / Emscripten 构建可用
- 浏览器中可显示最小菜单场景
- 当前菜单包含：Main Menu、Start Game、Settings、Quit
- 按钮点击已有最小响应
- 已接入 Claude API
- 已有 AI 工具脚本：scripts/ai-client.py
- 已有 AI 输出示例：examples/scene-schema-example.json
- 已有 AI -> MVP 映射说明：doc/ai-to-mvp-mapping.md

# 你必须先阅读

- README.md
- doc/architecture.md
- doc/scene-schema.md
- doc/component-contract.md
- doc/runtime-api.md
- doc/one-week-mvp-plan.md
- doc/ai-to-mvp-mapping.md
- doc/mvp-demo-steps.md
- doc/mvp-acceptance.md
- .windsurfrules

# 你当前阶段的目标

你当前只允许聚焦以下目标：
1. 让当前 MVP 更容易演示、验收、交接
2. 为下一轮最小架构台阶做准备，但不要大改

# 你当前优先做什么

优先级从高到低：
1. 演示与验收文档收口
2. AI 接力开发提示词收口
3. 如有必要，只做最小架构台阶建议
4. 必要时的极小代码修补

# 你当前不要做什么

不要做：
- 完整 Scene Schema runtime loader
- 完整编辑器
- 完整 Physics / Animation / Tween
- 大规模目录重构
- 多 AI 服务商并行接入
- 与本周 MVP 无关的功能扩展

# 如果继续代码推进，只能优先考虑

1. 把当前 menu demo 的职责从 main.c 中轻量下沉一点点
2. 为 Scene ownership 补一个最小台阶
3. 为 AI 输出 -> 运行时对象映射补一个最小半自动步骤

# 你的输出格式

你的每次输出都尽量采用这个结构：
1. 任务理解
2. 当前判断
3. 改动方案
4. 影响文件
5. 风险点
6. 下一步建议

# 你的输出约束

1. 必须先总结当前已完成状态，再提修改
2. 必须明确区分“本轮立即做”和“下一轮再做”
3. 如果要改代码，优先做最小补丁
4. 如果改动影响协作方式或交付方式，必须同步更新 doc/
5. 不要把 API Key 写入源码、前端、仓库文本常量
6. 如果发现当前结构不适合直接扩展，先提出“最小架构台阶”方案

# 本阶段成功标准

满足以下条件即可视为本阶段完成：
- 新读者可以按文档完成演示
- 新 AI 代理可以按提示词继续工作而不跑偏
- 当前 MVP 的边界、状态和下一步都清楚
- 没有引入与本周目标无关的大改
```

## 4. 更短版

```text
你现在继续接手 UhandEngine，但仓库不是从 0 开始。

当前已完成：
- Web MVP 已可运行
- 菜单场景已可显示与点击
- Claude API 已接通
- 已有 Scene Schema 示例和 AI -> MVP 映射说明

你先阅读：
- README.md
- doc/ai-to-mvp-mapping.md
- doc/mvp-demo-steps.md
- doc/mvp-acceptance.md
- .windsurfrules

你当前只做：
1. 演示/验收收口
2. AI 接力提示词收口
3. 如有必要，只做最小架构台阶

不要做：
- 大重构
- 完整 loader
- 完整编辑器
- 与本周 MVP 无关的扩展

输出时必须包含：
1. 当前已完成状态
2. 本轮要做的最小工作
3. 影响文件
4. 风险点
5. 下一步建议
```
