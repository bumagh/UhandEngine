# UhandEngine 项目架构说明

## 1. 文档目标

本文档用于整理 `UhandEngine` 当前的项目结构、运行机制、模块边界以及未来演进方向。

本项目当前是一个以 `C + SDL2` 为基础、同时面向桌面与 WebAssembly/Emscripten 的轻量游戏引擎原型。未来将逐步演进为：

- **运行时内核仍然保持轻量与高性能**
- **对外能力向 PhaserJS 的开发体验靠拢**
- **在工具链与资产生产层面优先面向 AI 协作**

## 2. 当前项目结构

```text
UhandEngine/
├─ README.md
├─ doc/
├─ bin/
├─ include/
├─ lib/
├─ src/
│  ├─ main.c
│  ├─ draw.c / draw.h
│  ├─ util.c / util.h
│  ├─ global.h
│  ├─ makefile
│  ├─ assets/
│  └─ engine/
│     ├─ component.c / component.h
│     ├─ transform.c / transform.h
│     ├─ gameObject.c / gameObject.h
│     ├─ gameObjectList.c / gameObjectList.h
│     ├─ uiComponent.c / uiComponent.h
│     ├─ event.c / event.h
│     ├─ Scene.c / Scene.h
│     ├─ list.c / list.h
│     ├─ algorithm.c / algorithm.h
│     └─ UI/
├─ test/
└─ web/
   ├─ UhandEngine/
   └─ html_template/
```

## 3. 当前运行架构

## 3.1 启动入口

当前入口位于 `src/main.c`。

核心流程如下：

1. `init()`
   - 初始化 `SDL`、`SDL_ttf`、`SDL_mixer`
   - 创建窗口与渲染器
2. `loadMedia()`
   - 加载字体等资产
3. `gameInit()`
   - 创建 `Scene`
   - 调用 `Scene_Awake()` / `Scene_Start()`
   - 添加 MVP UI 组件到 Scene
4. `loop()`
   - 桌面端进入 while 循环
   - Web 端使用 `emscripten_set_main_loop`
5. `tick()`
   - 处理事件
   - 调用 `Scene_Update()` 更新 GameObject
   - 调用 `Scene_UpdateUI()` 更新 UI 组件
   - 清屏与提交渲染
6. `quit()`
   - 调用 `Scene_Destroy()` 释放所有资源
   - 释放渲染器、窗口、字体

## 3.2 数据流与控制流

```text
main
 ├─ init
 ├─ loadMedia
 ├─ gameInit
 │   └─ Scene_Create
 │       ├─ Scene_Awake
 │       ├─ Scene_Start
 │       └─ Scene_AddUIComponent
 └─ loop
     └─ tick
         ├─ Scene_Update
         ├─ SDL_PollEvent
         ├─ Scene_UpdateUI
         ├─ SDL_RenderClear
         ├─ Scene_RenderUI
         └─ SDL_RenderPresent
```

## 4. 当前核心模块

## 4.1 `GameObject`

文件：`src/engine/gameObject.h`、`src/engine/gameObject.c`

职责：

- 表达场景中的实体对象
- 维护组件链表
- 维护生命周期函数指针
- 挂载默认 `Transform`

当前特征：

- 拥有 `Awake`、`Start`、`Update`、`LateUpdate`、`FixedUpdate`、`Destroy`
- `render` 与 `getComponent` 通过函数指针暴露
- 使用链表方式管理组件

## 4.2 `Component`

文件：`src/engine/component.h`、`src/engine/component.c`

职责：

- 提供组件基类能力
- 保存名字、链表连接、更新函数、渲染函数、释放函数

当前能力偏基础，后续需要增强：

- 类型识别统一化
- 生命周期与宿主对象绑定
- 序列化信息
- 编辑器元数据

## 4.3 `Transform`

文件：`src/engine/transform.h`、`src/engine/transform.c`

职责：

- 保存基础空间信息
- 提供位置设置与打印接口

当前模型较轻量，未来需要扩展：

- 2D 优先的 `x / y / scale / rotation / origin`
- 父子节点层级
- 世界坐标与局部坐标

## 4.4 `GameObjectList`

文件：`src/engine/gameObjectList.h`、`src/engine/gameObjectList.c`

职责：

- 存储场景对象集合
- 批量调用生命周期函数

这是当前最接近“世界对象容器”的模块，但尚未形成完整场景图。

## 4.5 `UIComponent`

文件：`src/engine/uiComponent.h`、`src/engine/uiComponent.c`

职责：

- 提供基础 UI 元素的绘制与点击处理
- 当前包括矩形区域、文字、颜色、回调

这说明项目已经具备“引擎 + UI”混合方向的雏形，适合未来接入调试面板、AI 命令面板和编辑器浮层。

## 4.6 `Scene`

文件：`src/engine/Scene.h`、`src/engine/Scene.c`

职责：

- 作为运行时主单元，管理场景中的 GameObject 和 UIComponent
- 提供场景生命周期管理（Awake、Start、Update、Destroy）
- 管理 GameObjectList 作为内部对象容器
- 临时管理 UIComponent 链表（用于 MVP 支持）

当前状态：

- 已成为真正的运行入口
- 已接入主循环的更新和渲染流程
- 内部包含 GameObjectList 作为对象容器
- 提供 Scene_AddGameObject / Scene_AddUIComponent 接口
- 提供 Scene_Update / Scene_UpdateUI / Scene_RenderUI 接口

## 4.7 `EventManager`

文件：`src/engine/event.h`、`src/engine/event.c`

职责：

- 注册事件
- 注销事件
- 分发事件

该模块非常关键，未来会成为：

- 运行时事件系统
- 编辑器消息系统
- AI Agent 工具调用总线

## 5. 当前架构优点

- **跨平台基础已具备**：桌面端与 Web 端构建路径都已存在
- **ECS/组件化雏形明确**：GameObject + Component 是清晰的可扩展方向
- **生命周期模型清楚**：与主流游戏引擎心智模型一致
- **UI 与事件已有基础设施**：适合后续接编辑器与智能工具
- **代码规模较小**：适合快速重构与建立规范

## 6. 当前架构问题

## 6.1 主循环与渲染耦合偏紧

当前 `tick()` 中把更新、事件处理和渲染直接串在一起，后续若要支持：

- 固定步长更新
- 插值渲染
- 录制回放
- AI 仿真批处理

会比较受限。

## 6.2 Scene 尚未成为主组织单元

**（已解决）** 在第 1 周架构收敛中，Scene 已成为真正的运行时主单元：

- Scene 现在拥有 GameObjectList 作为内部对象容器
- Scene 提供完整的生命周期管理（Awake、Start、Update、Destroy）
- main.c 通过 Scene 初始化、更新和销毁所有资源
- GameObjectList 的职责已并入 Scene

## 6.3 组件系统还未形成统一协议

当前组件更像“链表节点 + update/draw/free 函数”，但还缺少：

- 组件类型 ID
- owner 指针
- 启用/禁用状态
- 序列化字段
- Inspector 元信息

## 6.4 对 PhaserJS 的映射尚未建立

当前引擎抽象虽然合理，但还没有与 PhaserJS 关键概念进行显式对齐，例如：

- `Scene`
- `GameObject`
- `Sprite`
- `Text`
- `Container`
- `Input`
- `Tween`
- `Loader`
- `Physics`
- `Animation`
- `Tilemap`

## 6.5 尚未具备 AI 原生工作流

目前项目更像传统引擎原型，还没有面向 AI 的关键能力：

- 结构化项目索引
- API 约束描述
- 场景声明格式
- 可由 AI 生成的模板骨架
- 可供 Agent 调用的编辑与校验规范

## 7. 面向 AI 版游戏引擎的目标架构

## 7.1 核心方向

未来架构建议拆成四层：

```text
AI 协作层
├─ Prompt / Rules / Skills / Workflows
├─ 场景描述 DSL / JSON
└─ 资源与代码生成接口

工具编辑层
├─ 调试 UI
├─ Inspector
├─ Scene Editor
└─ 命令控制台

引擎运行时层
├─ Scene System
├─ Entity / Component System
├─ Render System
├─ Input System
├─ Audio System
├─ Asset System
└─ Event Bus

平台适配层
├─ SDL2 Native
└─ WebAssembly / Browser
```

## 7.2 与 PhaserJS 的首要对齐目标

UhandEngine 不需要完全复制 PhaserJS，但建议优先对齐其“使用体验”和“概念组织方式”。

优先对齐的抽象：

1. `Scene`
2. `GameObject`
3. `Container`
4. `Sprite`
5. `Text`
6. `Input`
7. `Tween`
8. `Loader`
9. `Animation`
10. `Camera`

建议的映射关系：

| PhaserJS 概念 | UhandEngine 当前基础 | 建议演进方向 |
| --- | --- | --- |
| Scene | `Scene` / `GameObjectList` | 统一为真正的场景运行单元 |
| GameObject | `GameObject` | 作为所有实体基础类型 |
| Components | `Component` | 形成统一组件协议与反射描述 |
| Text | `UIComponent` + TTF | 拆出独立文本对象 |
| Container | 暂无 | 引入父子层级容器 |
| Input | SDL Event | 抽象为统一输入系统 |
| Loader | `loadMedia()` | 升级为资源管理器 |
| Time / Tween | 暂无 | 引入时间轴与补间系统 |
| Camera | 暂无 | 引入 2D 摄像机模型 |
| Animation | 暂无 | 建立序列帧与状态机 |

## 7.3 为什么优先对齐 PhaserJS

- **2D 场景模型成熟**
- **开发者心智负担低**
- **适合作为 AI 代码生成目标**
- **文档生态完善，便于构建映射 Prompt**
- **Web 导向强，与 Emscripten 目标一致**

## 8. 建议的未来目录架构

```text
UhandEngine/
├─ README.md
├─ .windsurfrules
├─ doc/
│  ├─ architecture.md
│  ├─ phaser-ai-alignment-roadmap.md
│  ├─ monthly-update-plan.md
│  └─ swe1.6-prompt-and-skills.md
├─ src/
│  ├─ core/
│  │  ├─ app/
│  │  ├─ memory/
│  │  ├─ time/
│  │  └─ log/
│  ├─ runtime/
│  │  ├─ scene/
│  │  ├─ object/
│  │  ├─ component/
│  │  ├─ render/
│  │  ├─ input/
│  │  ├─ asset/
│  │  ├─ event/
│  │  └─ audio/
│  ├─ gameplay/
│  ├─ editor/
│  ├─ ai/
│  │  ├─ prompts/
│  │  ├─ schema/
│  │  └─ skills/
│  └─ platform/
│     ├─ sdl/
│     └─ web/
└─ test/
```

## 9. 文档体系建议

建议 `doc/` 下至少长期维护以下文档：

- `architecture.md`
- `scene-schema.md`
- `component-contract.md`
- `runtime-api.md`
- `ai-agent-workflow.md`
- `one-week-mvp-plan.md`
- `phaser-ai-alignment-roadmap.md`
- `monthly-update-plan.md`
- `swe1.6-prompt-and-skills.md`

若后续继续完善，建议补充：

- `doc/editor-vision.md`
- `doc/scene-pipeline.md`

## 10. 统一命名规范

### 文件命名

- C 源文件：小写，使用下划线分隔
  - 例：`gameObject.c`、`uiComponent.c`、`scene.c`
- 头文件：与源文件同名，`.h` 扩展名
  - 例：`gameObject.h`、`uiComponent.h`、`scene.h`
- 文档文件：小写，使用连字符分隔
  - 例：`architecture.md`、`main-loop-refactor-checklist.md`

### 函数命名

- 模块函数：`模块名_动作名` 格式
  - 例：`Scene_Create()`、`GameObjectList_Add()`、`Scene_UpdateUI()`
- 通用函数：小写，使用下划线分隔
  - 例：`createGameObjectList()`、`updateUIComponent()`

### 类型命名

- 结构体：大驼峰
  - 例：`Scene`、`GameObject`、`Component`、`UIComponent`
- 指针类型：`类型名 *变量名`
  - 例：`Scene *scene`、`GameObject *go`

### 变量命名

- 局部变量：小写，使用下划线分隔
  - 例：`current_time`、`delta_time`
- 全局变量：小写，使用下划线分隔，可加 `g` 前缀
  - 例：`gQuit`、`mainScene`
- 成员变量：小写，使用下划线分隔
  - 例：`gameObjectList`、`uiComponents`

### 常量命名

- 宏常量：全大写，使用下划线分隔
  - 例：`SDL_DELAY`、`FRAMERATE`、`WINDOW_W`
- 枚举常量：全大写，使用下划线分隔
  - 例：`STATE_IDLE`、`STATE_RUNNING`

### 模块边界命名

- 每个模块的公开接口函数使用统一前缀
  - Scene 模块：`Scene_` 前缀
  - GameObject 模块：`GameObject_` 前缀
  - GameObjectList 模块：`GameObjectList_` 前缀
  - UIComponent 模块：`createUIComponent`、`updateUIComponent`、`renderUIComponent`

### 生命周期方法命名

- 统一使用以下生命周期方法名：
  - `Awake()` - 对象创建时调用
  - `Start()` - 对象第一次更新前调用
  - `Update()` - 每帧调用
  - `LateUpdate()` - 每帧 Update 之后调用
  - `FixedUpdate()` - 固定时间步长调用
  - `Destroy()` - 对象销毁时调用

## 11. 下一阶段的架构原则

- **Scene First**：让场景成为运行时第一组织单元
- **Phaser-like API**：优先向 PhaserJS 的 2D API 心智靠拢
- **AI First Tooling**：所有核心模块都要考虑 AI 是否能理解、生成、修改
- **Data Driven**：尽量为场景、对象、组件保留声明式表达能力
- **Native + Web Dual Target**：继续保持 SDL2 与 Web 双端输出能力
- **Small Core, Extensible Modules**：核心最小化，功能模块化

## 12. 当前建议结论

从现状看，`UhandEngine` 最值得保留的不是已有功能的数量，而是已经具备了以下可演进骨架：

- 生命周期
- 对象与组件
- UI
- 事件
- Web 构建

接下来最重要的不是继续零散加功能，而是把架构从“原型功能集合”升级成“可持续演进、可被 AI 正确理解和生成”的引擎框架。

而这个升级过程，建议以 **PhaserJS 的概念模型作为第一对齐目标**。
