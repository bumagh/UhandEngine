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
  - `Transform` 作为默认基础组件（2D-first）

- **对象集合与生命周期**
  - `GameObjectList` 负责对象存储与批量调用 `Awake / Start / Update / LateUpdate / FixedUpdate / Destroy`

- **UI 与交互雏形**
  - `UIComponent` 已提供基本文本与点击处理能力
  - `TextComponent` 支持中文文本渲染

- **事件系统**
  - `event.c / event.h` 提供简单事件注册与派发能力

- **Scene 系统**
  - `Scene` 已成为真正的运行时主单元
  - 集成 `RenderQueue` 支持 depth 排序渲染

- **资源管理**
  - `AssetManager` 统一管理纹理、字体、声音、音乐
  - 支持资源缓存和引用计数

- **输入系统**
  - `InputManager` 统一处理键盘和鼠标输入
  - 支持按下、按下瞬间、释放瞬间检测

- **摄像机系统**
  - `Camera` 支持位置、缩放、旋转控制
  - 提供世界坐标与屏幕坐标转换

- **动画系统**
  - `Animation` 支持帧动画播放
  - `Animator` 支持循环、暂停、速度控制

- **补间系统**
  - `Tween` 支持属性补间动画
  - 多种缓动函数（Linear, Quad, Cubic, Elastic）

- **双端构建能力**
  - 已支持桌面构建
  - 已支持 `Emscripten` Web 输出

- **示例程序**
  - 13 个示例程序展示各个系统的使用
  - 综合示例展示系统集成

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
│  ├─ ai-to-mvp-mapping.md
│  ├─ mvp-demo-steps.md
│  ├─ mvp-acceptance.md
│  ├─ phaser-ai-alignment-roadmap.md
│  ├─ monthly-update-plan.md
│  ├─ swe1.6-prompt-and-skills.md
│  ├─ swe1.6-current-state-prompt.md
│  └─ swe1.6-release-promotion-engine-first-prompt.md
├─ src/
│  ├─ main.c
│  ├─ makefile
│  ├─ assets/
│  └─ engine/
├─ scripts/
│  ├─ ai-client.py
│  └─ README.md
├─ examples/
│  └─ scene-schema-example.json
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

- **`doc/ai-to-mvp-mapping.md`**
  - 说明 AI 生成的 Scene Schema 如何映射到当前 MVP 代码实现

- **`doc/mvp-demo-steps.md`**
  - 说明如何演示当前 Web-first MVP

- **`doc/mvp-acceptance.md`**
  - 定义当前 MVP 的最小验收标准

- **`doc/web-release-checklist.md`**
  - 面向当前 Web-first MVP 的发布执行清单，覆盖构建、验证、AI 工具链、安全检查与静态部署前检查

- **`doc/main-loop-refactor-checklist.md`**
  - 主循环重构清单，记录从原型功能集合到 Scene-first 运行时骨架的完整重构步骤

- **`doc/phaser-ai-alignment-roadmap.md`**
  - 面向 AI 版游戏引擎、并向 PhaserJS 对齐的路线图

- **`doc/monthly-update-plan.md`**
  - 未来一个月的更新计划与阶段目标

- **`doc/swe1.6-prompt-and-skills.md`**
  - 面向 AI 编程代理的提示词、任务模板与技能定义

- **`doc/swe1.6-current-state-prompt.md`**
  - 面向当前 Day 5 之后仓库状态的 SWE1.6 续开发提示词

- **`doc/swe1.6-release-promotion-engine-first-prompt.md`**
  - 面向发布级推进目标的 SWE1.6 多步骤提示词，重点覆盖 Scene-first 骨架、多个示例、Web 发布与推广收口

## 当前 MVP 交付说明

当前 MVP 已经完成以下最小交付：

- **Web 可运行演示**
  - 浏览器中可打开 Web 版本并看到菜单场景

- **最小显示与交互闭环**
  - 已有标题文本与多个按钮
  - 按钮点击已有最小响应

- **AI API 最小接入**
  - 已有本地脚本通过 Claude API 生成 Scene Schema

- **AI 输出参与演示链路**
  - 已保存示例 JSON
  - 已有 AI 输出到当前 MVP 的映射说明

当前 MVP 仍然是一个受控范围内的最小版本，不包含：

- 完整 Scene Schema runtime loader
- 完整编辑器
- 完整 Physics / Animation / Tween
- 完整 Scene-first 架构改造

如果你想快速理解当前交付路径，建议按以下顺序阅读：

1. `doc/mvp-demo-steps.md`
2. `doc/mvp-acceptance.md`
3. `doc/web-release-checklist.md`
4. `doc/main-loop-refactor-checklist.md`
5. `doc/ai-to-mvp-mapping.md`
6. `doc/swe1.6-current-state-prompt.md`
7. `doc/swe1.6-release-promotion-engine-first-prompt.md`

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
