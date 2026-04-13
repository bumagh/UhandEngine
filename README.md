# UhandEngine

UhandEngine 是一个基于 `C + SDL2` 的轻量游戏引擎原型，当前面向两个目标平台：

- **Native Desktop**
- **Web / Emscripten**

这个项目目前处于“从原型走向可持续架构”的阶段。现有代码已经具备对象、组件、生命周期、UI、事件与 Web 构建基础，接下来将逐步演进为一个 **AI-first** 的游戏引擎，并优先向 **PhaserJS** 的 2D 开发体验与概念模型对齐。

## 项目目标

- **轻量运行时内核**
  - 保持 C 语言和 SDL2 的小而稳内核

- **PhaserJS 对齐**
  - 优先对齐 `Scene`、`GameObject`、`Container`、`Sprite`、`Text`、`Input`、`Loader`、`Camera`、`Animation`、`Tween`

- **AI-first 工作流**
  - 让文档、目录、命名和模块边界更适合 AI 阅读、生成和维护

## 当前项目状态

当前仓库已经具备以下基础：

- **运行入口**
  - `src/main.c` 负责初始化、主循环、事件轮询、渲染与退出

- **对象模型**
  - `GameObject` 作为基础实体
  - `Component` 作为组件抽象
  - `Transform` 作为默认基础组件

- **对象集合与生命周期**
  - `GameObjectList` 负责对象存储与批量调用 `Awake / Start / Update / LateUpdate / FixedUpdate / Destroy`

- **UI 与交互雏形**
  - `UIComponent` 已提供基本文本与点击处理能力

- **事件系统**
  - `event.c / event.h` 提供简单事件注册与派发能力

- **Scene 雏形**
  - `Scene.c / Scene.h` 已存在，但尚未成为真正的运行时主单元

- **双端构建能力**
  - 已支持桌面构建
  - 已支持 `Emscripten` Web 输出

## 当前核心目录

```text
UhandEngine/
├─ README.md
├─ .windsurfrules
├─ doc/
│  ├─ architecture.md
│  ├─ scene-schema.md
│  ├─ component-contract.md
│  ├─ runtime-api.md
│  ├─ ai-agent-workflow.md
│  ├─ one-week-mvp-plan.md
│  ├─ phaser-ai-alignment-roadmap.md
│  ├─ monthly-update-plan.md
│  └─ swe1.6-prompt-and-skills.md
├─ src/
│  ├─ main.c
│  ├─ makefile
│  ├─ assets/
│  └─ engine/
├─ test/
├─ web/
├─ include/
└─ lib/
```

## 核心架构摘要

当前运行时主流程：

1. 初始化 SDL / TTF / Mixer
2. 创建窗口与渲染器
3. 加载资源
4. 创建对象列表
5. 执行主循环
6. 更新对象与处理事件
7. 渲染并退出释放

当前最重要的架构判断是：

- **现状可用，但仍偏原型化**
- **`Scene` 需要升级为真正的运行时主单元**
- **`GameObject` / `Component` 边界需要继续标准化**
- **`Input` / `Asset` / `Camera` / `Animation` 等子系统需要显式化**

## 为什么以 PhaserJS 为首要对齐目标

选择 PhaserJS 作为第一对齐目标，主要因为：

- **2D 场景模型成熟**
- **概念清晰，便于 AI 学习与生成**
- **Web 心智模型强，与 Emscripten 方向一致**
- **适合作为轻量引擎的 API 设计参考**

UhandEngine 的目标不是复制 PhaserJS，而是借鉴它的：

- **场景组织方式**
- **对象与容器模型**
- **2D 游戏开发体验**
- **适合快速原型的接口风格**

## 文档索引

项目文档已整理到 `doc/`：

- **`doc/architecture.md`**
  - 当前项目架构、模块职责、问题与未来目录建议

- **`doc/scene-schema.md`**
  - 定义面向 AI 和工具链的声明式场景结构与字段约束

- **`doc/component-contract.md`**
  - 定义组件标准协议、生命周期边界与 Scene Schema 映射方式

- **`doc/runtime-api.md`**
  - 定义运行时 API 分层、核心模块边界与建议接口命名

- **`doc/ai-agent-workflow.md`**
  - 定义 AI 代理在本仓库中的标准协作流程与任务执行顺序

- **`doc/one-week-mvp-plan.md`**
  - 定义一周内交付 Web-first + AI API 接入 + SWE1.6 Prompt 可用的 MVP 计划

- **`doc/phaser-ai-alignment-roadmap.md`**
  - 面向 AI 版游戏引擎、并向 PhaserJS 对齐的路线图

- **`doc/monthly-update-plan.md`**
  - 未来一个月的更新计划与阶段目标

- **`doc/swe1.6-prompt-and-skills.md`**
  - 面向 AI 编程代理的提示词、任务模板与技能定义

## 构建说明

当前构建文件位于：`src/makefile`

### 桌面端

项目使用本地 `SDL2`、`SDL_ttf`、`SDL_mixer` 进行桌面构建。

主要特点：

- **编译器**：`gcc`
- **平台宏**：`pc_build`
- **输出位置**：`bin/UhandEngine/`

### Web 端

项目已提供 `Emscripten` 构建命令，用于生成浏览器可运行版本。

主要特点：

- **使用 `emcc` 构建**
- **预加载 `assets`**
- **输出位置**：`web/UhandEngine/`

## 近期优先事项

未来近一阶段，优先推进以下方向：

- **Scene-first 架构落地**
- **Transform 向 2D-first 升级**
- **抽象 InputManager 与 AssetManager**
- **建立 Phaser-like 的 `Text` / `Sprite` / `Container` 能力**
- **为 AI 协作建立 Prompt、Skills、Rules 与场景描述规范**

## 仓库规则

仓库级协作规则位于：

- **`.windsurfrules`**

该文件用于约束：

- 架构演进方向
- 文档同步要求
- AI 协作方式
- 代码修改边界

## 适合这个仓库的开发原则

- **优先做小步、可验证修改**
- **优先读文档再改架构**
- **避免把新功能继续堆进 `main.c`**
- **优先建立模块边界，而不是无计划扩展功能**
- **所有重要架构变动都同步更新 `doc/`**

## 下一步建议

如果接下来继续迭代，建议从以下顺序开始：

1. 让 `Scene` 成为运行时主拥有者
2. 规范 `GameObject` / `Component` 协议
3. 建立 `AssetManager`
4. 建立 `InputManager`
5. 添加 `Text` / `Sprite` / `Container`
6. 引入 `Camera` 与 `Animation` 基础能力
7. 建立 AI 可读的 Scene Schema

## 总结

UhandEngine 当前最重要的工作不是继续堆砌零散功能，而是把现有原型沉淀为：

- **对开发者更清晰的引擎架构**
- **对 AI 更友好的文档和规则体系**
- **对 PhaserJS 2D 心智模型更一致的运行时方向**

这也是当前仓库文档整理与后续架构演进的核心目标。
