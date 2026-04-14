# UhandEngine MVP 验收标准

## 1. 目标

本文档定义当前 Web-first MVP 的最小验收标准。

本周 MVP 是否达标，不看功能是否“大而全”，只看是否验证了既定路线。

## 2. 核心验收目标

当前 MVP 必须验证以下三点：

- UhandEngine 可以在 Web 端跑起来
- 可以接入第三方 AI API，得到结构化结果
- AI 输出可以参与当前 MVP 的演示链路

## 3. 功能验收项

## 3.1 Web 运行壳

满足以下条件视为通过：

- 可以完成 Emscripten 构建
- 浏览器可以打开 `web/UhandEngine/index.html`
- 页面可稳定显示场景结果

## 3.2 最小场景显示

满足以下条件视为通过：

- 页面中可看到标题文本
- 页面中至少有一个可交互按钮
- 当前版本应显示：
  - `Main Menu`
  - `Start Game`
  - `Settings`
  - `Quit`

## 3.3 最小交互能力

满足以下条件视为通过：

- 点击按钮时有响应
- 响应至少体现为控制台输出或状态变化
- 当前版本点击三个按钮应分别输出对应日志

## 3.4 AI API 接入

满足以下条件视为通过：

- 本地脚本可调用第三方 AI API
- API Key 不写入源码或仓库文件
- AI 返回结果为结构化内容
- 当前版本使用 `scripts/ai-client.py` 完成该目标

## 3.5 AI 输出参与演示链路

满足以下条件视为通过：

- 仓库内存在 AI 输出示例
- 示例内容与当前 MVP 演示主题一致
- 有文档说明 AI 输出如何映射到当前实现
- 当前版本使用：
  - `examples/scene-schema-example.json`
  - `doc/ai-to-mvp-mapping.md`

## 3.6 AI 协作可延续

满足以下条件视为通过：

- 仓库内存在 SWE1.6 可复用提示词
- 提示词边界明确，不鼓励大重构
- 后续 AI 代理可以基于当前状态继续工作

## 4. 非目标说明

以下内容不属于本周 MVP 验收条件：

- 完整 Scene-first 架构改造
- 完整 Scene Schema runtime loader
- 完整编辑器
- 完整 Physics / Animation / Tween
- Native 与 Web 双端同时精修

## 5. 当前通过标准

只要满足以下条件，即可视为本周 MVP 达标：

- Web 页面可运行
- 最小菜单场景可显示
- 按钮可点击
- AI 脚本可生成结构化场景结果
- 至少一个 AI 输出已进入演示流程
- 有演示步骤文档
- 有验收标准文档

## 6. 下一阶段不属于本次验收但建议进入 backlog 的事项

- `Scene` 成为真正 runtime owner
- 从手工映射升级为半自动映射
- 把 menu demo 轻量下沉出 `main.c`
- 增加更稳定的 Text / Button 抽象
