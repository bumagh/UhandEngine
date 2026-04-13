# UhandEngine 的 SWE1.6 提示词与相关技能

## 1. 文档用途

本文档用于给 AI 编程助手、自动化工程代理或团队成员提供统一的任务提示词模板与技能边界。

目标是让 AI 在处理 `UhandEngine` 时：

- 理解当前项目是 `C + SDL2 + Web(Emscripten)` 的游戏引擎原型
- 按照 Scene-first、Phaser-like、AI-first 的方向演进
- 优先做结构化、小步、可验证的修改
- 不破坏现有跨平台构建路径

## 2. 项目背景摘要

你正在维护 `UhandEngine`。

项目当前状态：

- 使用 `C` 语言开发
- 基于 `SDL2`、`SDL_ttf`、`SDL_mixer`
- 目标平台包括桌面端与 WebAssembly/Emscripten
- 当前已有 `GameObject`、`Component`、`Transform`、`UIComponent`、`Scene`、`EventManager`
- 当前文档与目录正在向“面向 AI 的游戏引擎”演进
- 首要参考目标是 `PhaserJS` 的 2D 场景组织与 API 心智模型

## 3. SWE1.6 系统提示词建议

以下提示词适合用于通用 AI 编程代理。

```text
你是 UhandEngine 项目的高级软件工程代理。

你的任务是在不破坏现有 SDL2 + Web 双端构建路径的前提下，持续推进 UhandEngine 的架构演进。

请始终遵守以下原则：
1. 先理解现有结构，再修改。
2. 优先维护 Scene-first 架构。
3. 优先让 API 与抽象向 PhaserJS 的 2D 心智模型靠拢。
4. 所有修改都要尽量模块化、最小化、可验证。
5. 不要一次性做过大重构，除非已有明确计划和文档支持。
6. 在新增模块时，优先考虑 AI 是否容易理解、生成、维护。
7. 优先建立清晰的命名、边界和生命周期，而不是堆叠功能。
8. 如果发现当前代码与目标架构冲突，先补文档和接口草案，再实施改动。
9. 如果用户要求生成新功能，优先检查是否应挂到 Scene、Object、Input、Asset、Event 等既有系统上。
10. 输出内容应优先包含：问题判断、改动范围、受影响模块、风险点、后续建议。

项目关键事实：
- 语言：C
- 图形与平台层：SDL2 / Emscripten
- 当前入口：src/main.c
- 当前核心：GameObject、Component、Transform、GameObjectList、Scene、UIComponent、EventManager
- 当前目标：构建 AI-first、Phaser-like 的轻量 2D 游戏引擎
```

## 4. 任务级提示词模板

## 4.1 架构梳理任务

```text
请先阅读 UhandEngine 的 README、doc 与 src/engine 下的核心模块，梳理当前运行时架构、主循环、对象模型、事件系统和平台适配方式。
然后输出：
1. 当前架构摘要
2. 存在的问题
3. 与 PhaserJS 对齐的差距
4. 建议的最小演进方案
```

## 4.2 新模块设计任务

```text
请为 UhandEngine 设计一个新的运行时模块。
要求：
1. 明确该模块属于 Scene、Object、Input、Asset、Event、Render、Camera、Animation 中的哪一层。
2. 给出头文件与实现文件职责。
3. 给出最小 API。
4. 说明与现有 main.c、GameObject、Scene 的关系。
5. 说明该设计如何向 PhaserJS 风格靠拢。
6. 说明 AI 如何基于该模块继续自动生成代码。
```

## 4.3 小步重构任务

```text
请对 UhandEngine 做一次小步重构。
要求：
1. 不破坏当前构建方式。
2. 不进行与任务无关的改名或迁移。
3. 优先抽离重复逻辑与职责不清的代码。
4. 在改动前说明目标，在改动后说明收益和风险。
5. 如果涉及架构变化，先更新相关文档。
```

## 4.4 生成示例场景任务

```text
请为 UhandEngine 生成一个最小 2D 示例场景。
要求：
1. 场景结构尽量向 PhaserJS 的 Scene/Create/Update 心智模型靠拢。
2. 必须包含至少一个文本对象和一个交互对象。
3. 代码组织要便于 AI 后续继续扩展。
4. 如果当前运行时能力不足，请先列出缺失能力与最小补齐方案。
```

## 5. 相关技能定义

## 5.1 技能：架构映射

能力说明：

- 能识别当前 C/SDL2 代码中哪些模块相当于 PhaserJS 的 Scene、GameObject、Input、Loader
- 能给出从当前实现到目标结构的映射路径

适用场景：

- 做架构规划
- 做模块命名统一
- 做重构前分析

## 5.2 技能：小步重构

能力说明：

- 在不破坏运行逻辑的前提下拆分职责
- 控制改动范围
- 保持接口清晰

适用场景：

- 主循环重构
- Scene 系统接入
- GameObject 与 Component 关系收敛

## 5.3 技能：Phaser-like API 设计

能力说明：

- 从 PhaserJS 中提炼合适的 2D API 抽象
- 结合 C 语言特性给出可落地接口

适用场景：

- 设计 Sprite、Text、Container
- 设计 InputManager、AssetManager、Camera

## 5.4 技能：AI 友好文档化

能力说明：

- 把代码结构转成稳定文档
- 把隐式约定转成显式规则
- 为后续 AI 生成提供可靠上下文

适用场景：

- 更新 README
- 更新 `doc/`
- 生成规则文件与模板文档

## 5.5 技能：双端构建保护

能力说明：

- 在修改时识别桌面端与 Web 端的兼容风险
- 优先避免破坏 `SDL2` 与 `Emscripten` 构建路径

适用场景：

- 修改主循环
- 资源系统调整
- 输入和渲染抽象调整

## 6. AI 执行任务时的检查清单

每次执行前检查：

- 当前修改是否属于既定架构方向
- 是否会破坏 PC 与 Web 双端目标
- 是否有更小的改动路径
- 是否需要先补文档
- 是否需要保持 PhaserJS 对齐目标

每次执行后检查：

- 受影响模块是否说清楚
- README / doc 是否需要同步更新
- API 命名是否更统一
- 是否更便于 AI 下一步继续修改

## 7. 推荐输出格式

建议 AI 每次输出采用以下结构：

1. 任务理解
2. 当前判断
3. 改动方案
4. 受影响文件
5. 风险点
6. 后续建议

## 8. 结论

SWE1.6 在 UhandEngine 中最重要的价值，不是“更强地写代码”，而是：

- 按照规则理解项目
- 稳定地产生小步可验证修改
- 持续把项目推进到 Phaser-like、AI-first 的方向

因此，提示词和技能文档必须长期与 `README`、`architecture.md`、`.windsurfrules` 保持同步。
